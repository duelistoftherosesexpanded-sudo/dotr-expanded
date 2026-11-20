#!/usr/bin/env python3
"""Duelists of the Roses Expanded - Setup (public beta): makes the game from the player's own discs.

The player chooses their discs: Yu-Gi-Oh! The Duelists of the Roses (needed), and any of The Dawn
of Destiny, The Falsebound Kingdom and Cross Duel's data, which give the new cards' 3D models.
Choosing one shows which card models it gives. Building then:
  1. extracts each card's model from its game (tools/setup_core.py, recipes from the release's
     manifest.json) and checks every file against this release; a file that differs names its
     card, which the player disables or keeps without its model;
  2. gets the new cards' pictures (Cross Duel's illustrations, or the card database online);
  3. makes either a player build: the finished disc image and its own PCSX2 (a portable copy of
     the installed one, with the extra memory the mod needs) with a "Play" shortcut; or a developer
     build: the model folders and pictures written into a copy of the project, ready for the DotR
     Launcher and the Card Maker.
A card whose game is not given cannot be obtained in the game (it plays its donor's model).

  python tools/setup_tool.py           (or the packaged "DotR Expanded Setup.exe")
"""

import json
import os
import queue
import shutil
import sys
import threading
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox, ttk

if getattr(sys, "frozen", False):
    BUNDLE = Path(sys._MEIPASS)
    RELEASE = BUNDLE / "release"
    DEV_KIT = BUNDLE / "dev_kit"
else:
    BUNDLE = Path(__file__).resolve().parent.parent
    RELEASE = BUNDLE / "release"
    DEV_KIT = BUNDLE / "tools" / "dev_kit"
DEV_KIT_MARK = "dotr-expanded-dev-kit"
WORK = Path(os.environ.get("LOCALAPPDATA", Path.home())) / "DotR Expanded Setup" / "work"
WORK.mkdir(parents=True, exist_ok=True)
os.environ["DOTR_ROOT"] = str(WORK)      # before the project's tools are imported
sys.path.insert(0, str(Path(__file__).resolve().parent))

import setup_core as core  # noqa: E402

GAMES = [("dotr", "Yu-Gi-Oh! The Duelists of the Roses (PS2, North America) - needed", "Disc image", "*.iso"),
         ("dod", "Yu-Gi-Oh! The Dawn of Destiny (Xbox)", "Disc image", "*.iso"),
         ("fbk", "Yu-Gi-Oh! The Falsebound Kingdom (GameCube)", "Disc image", "*.iso *.gcm *.rvz"),
         ("cross", "Yu-Gi-Oh! Cross Duel (the app, .apk)", "Cross Duel app", "*.apk"),
         ("saikyo", "Yu-Gi-Oh! Duel Monsters Saikyo Card Battle (3DS: the eShop download, or decrypted)",
          "Saikyo Card Battle", "*.cia *.cci *.3ds")]
TITLE = "Duelists of the Roses Expanded"
INSTALLED_PCSX2 = Path(os.environ.get("ProgramFiles", r"C:\Program Files")) / "PCSX2"
PCSX2_DATA = Path.home() / "Documents" / "PCSX2"


def detect(path):
    """Which of GAMES a dropped file is ("dotr", "dod", "fbk", "cross", "saikyo"), or None."""
    p = Path(path)
    if p.is_dir():
        return None
    suffix = p.suffix.lower()
    if suffix == ".apk":
        return "cross"
    if suffix in (".rvz", ".wia", ".gcz"):
        return "fbk"
    if suffix == ".cia":                               # an eShop download: Saikyo Card Battle only
        try:
            return "saikyo" if core.saikyo_title_id(p) == core.SAIKYO_TITLE_ID else None
        except OSError:
            return None
    try:
        with open(p, "rb") as f:
            head = f.read(0x20)
            f.seek(0x100)
            ncsd = f.read(4)
            f.seek(0x8000)
            iso9660 = f.read(8)
            f.seek(0x10000)
            xbox = f.read(20)
    except OSError:
        return None
    if xbox == b"MICROSOFT*XBOX*MEDIA":
        return "dod"
    if ncsd == b"NCSD":                                # a 3DS game image
        return "saikyo"
    if head[0x1c:0x20] == b"\xc2\x33\x9f\x3d":         # GameCube disc
        return "fbk"
    if iso9660[1:6] == b"CD001":                       # a PS2 disc (prepare_workspace checks it is DotR)
        return "dotr"
    return None


class FileDrop:
    """Files dragged from Explorer onto a Tk window (Windows' WM_DROPFILES, through ctypes)."""
    WM_DROPFILES, GWLP_WNDPROC = 0x233, -4

    def __init__(self, root, callback):
        import ctypes
        from ctypes import wintypes
        self.ct = ctypes
        user32, self.shell32 = ctypes.windll.user32, ctypes.windll.shell32
        LRESULT = ctypes.c_ssize_t
        self.WNDPROC = ctypes.WINFUNCTYPE(LRESULT, wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPARAM)
        user32.SetWindowLongPtrW.restype = ctypes.c_void_p
        user32.SetWindowLongPtrW.argtypes = [wintypes.HWND, ctypes.c_int, ctypes.c_void_p]
        user32.CallWindowProcW.restype = LRESULT
        user32.CallWindowProcW.argtypes = [ctypes.c_void_p, wintypes.HWND, wintypes.UINT, wintypes.WPARAM,
                                           wintypes.LPARAM]
        self.shell32.DragQueryFileW.argtypes = [ctypes.c_void_p, wintypes.UINT, wintypes.LPWSTR, wintypes.UINT]
        self.shell32.DragFinish.argtypes = [ctypes.c_void_p]
        self.shell32.DragAcceptFiles.argtypes = [wintypes.HWND, wintypes.BOOL]
        root.update_idletasks()
        hwnd = int(root.wm_frame(), 16)
        self.user32, self.callback = user32, callback
        self.proc = self.WNDPROC(self.handle)          # kept alive here
        self.old = user32.SetWindowLongPtrW(hwnd, self.GWLP_WNDPROC, ctypes.cast(self.proc, ctypes.c_void_p))
        self.shell32.DragAcceptFiles(hwnd, True)

    def handle(self, hwnd, msg, wparam, lparam):
        if msg == self.WM_DROPFILES:
            drop = self.ct.c_void_p(wparam)
            paths = []
            for i in range(self.shell32.DragQueryFileW(drop, 0xFFFFFFFF, None, 0)):
                n = self.shell32.DragQueryFileW(drop, i, None, 0) + 1
                buffer = self.ct.create_unicode_buffer(n)
                self.shell32.DragQueryFileW(drop, i, buffer, n)
                paths.append(buffer.value)
            self.shell32.DragFinish(drop)
            self.callback(paths)        # no Tk calls in here: Tk is mid-dispatch (Python would abort)
            return 0
        return self.user32.CallWindowProcW(self.old, hwnd, msg, wparam, lparam)


def manifest():
    return json.loads((RELEASE / "manifest.json").read_text(encoding="utf-8"))


class Setup(tk.Tk):
    def __init__(self):
        super().__init__()
        try:
            import gui_theme
            gui_theme.apply(self)
        except Exception:
            pass
        self.title(f"{TITLE} - Setup")
        self.minsize(820, 640)
        self.cards = manifest()["cards"]
        self.paths = {k: tk.StringVar() for k, *_ in GAMES}
        self.status = {k: tk.StringVar(value="not chosen") for k, *_ in GAMES}
        self.ok = {}
        self.given = {}         # game: the card numbers its file gives models for
        self.kind = tk.StringVar(value="player")
        self.out = tk.StringVar(value=str(Path.home() / "Desktop" / TITLE))
        self.messages = queue.Queue()
        self.choice_reply = queue.Queue()
        self._ui()
        self.refresh_summary()
        self.after(100, self.pump)
        try:
            self.drops = FileDrop(self, lambda paths: self.messages.put(("drop", paths)))
        except Exception as error:      # dragging is a convenience; Choose... still works
            self.say(f"(Dragging files onto the window is not available: {error})")

    # ---------------------------------------------------------------- dropping
    def dropped(self, paths):
        for path in paths:
            k = detect(path)
            if k is None:
                self.say(f"{Path(path).name}: not one of the games this setup uses.")
                messagebox.showwarning(TITLE, f"{Path(path).name} is not one of the games this setup uses:\n\n"
                                              + "\n".join(label for _, label, *_ in GAMES))
                continue
            self.say(f"{Path(path).name}: {core.SOURCES.get(k, 'Duelists of the Roses')}.")
            self.use(k, path)

    # ---------------------------------------------------------------- window
    def _ui(self):
        games = ttk.LabelFrame(self, text="Your games (only your own legally owned copies)")
        games.pack(fill="x", padx=10, pady=8)
        for row, (k, label, _, _) in enumerate(GAMES):
            ttk.Label(games, text=label).grid(row=2 * row, column=0, columnspan=3, sticky="w", padx=6, pady=(6, 0))
            ttk.Entry(games, textvariable=self.paths[k], width=80).grid(row=2 * row + 1, column=0, sticky="we", padx=6)
            ttk.Button(games, text="Choose...", command=lambda k=k: self.choose(k)).grid(row=2 * row + 1, column=1, padx=4)
            ttk.Button(games, text="Cards", command=lambda k=k: self.show_cards(k)).grid(row=2 * row + 1, column=2, padx=4)
            ttk.Label(games, textvariable=self.status[k], foreground="#8ab").grid(row=2 * row + 1, column=3, sticky="w", padx=6)
        games.columnconfigure(0, weight=1)
        ttk.Label(games, text="Or drag your games (disc images, the Cross Duel app, Saikyo Card Battle) onto this "
                              "window: each is recognised and put in its place.", foreground="#8ab").grid(
            row=2 * len(GAMES), column=0, columnspan=4, sticky="w", padx=6, pady=6)
        self.summary = tk.StringVar()
        ttk.Label(self, textvariable=self.summary, wraplength=780).pack(fill="x", padx=12)

        build = ttk.LabelFrame(self, text="Build")
        build.pack(fill="x", padx=10, pady=8)
        ttk.Radiobutton(build, text="Player build: the game and its own PCSX2, with a Play shortcut",
                        variable=self.kind, value="player").grid(row=0, column=0, columnspan=3, sticky="w", padx=6)
        ttk.Radiobutton(build, text="Developer build: the models and pictures written into your copy of the project",
                        variable=self.kind, value="dev").grid(row=1, column=0, columnspan=3, sticky="w", padx=6)
        ttk.Label(build, text="Folder").grid(row=2, column=0, sticky="w", padx=6, pady=6)
        ttk.Entry(build, textvariable=self.out, width=70).grid(row=2, column=1, sticky="we", padx=4)
        ttk.Button(build, text="Choose...", command=self.choose_out).grid(row=2, column=2, padx=4)
        build.columnconfigure(1, weight=1)
        self.build_button = ttk.Button(self, text="Build", command=self.start)
        self.build_button.pack(pady=4)
        self.log = tk.Text(self, height=14, font=("Consolas", 9), bg="#111", fg="#ddd")
        self.log.pack(fill="both", expand=True, padx=10, pady=(0, 10))

    def say(self, text):
        self.messages.put(("log", text))

    def pump(self):
        try:
            while True:
                kind, value = self.messages.get_nowait()
                if kind == "log":
                    self.log.insert("end", value + "\n")
                    self.log.see("end")
                elif kind == "status":
                    k, text, ok = value
                    self.status[k].set(text)
                    self.ok[k] = ok
                    self.refresh_summary()
                elif kind == "drop":
                    self.dropped(value)
                elif kind == "ask":
                    self.choice_reply.put(self.ask_mismatches(value))
                elif kind == "done":
                    self.build_button.state(["!disabled"])
                    if value:
                        messagebox.showinfo(TITLE, value)
        except queue.Empty:
            pass
        self.after(100, self.pump)

    # ---------------------------------------------------------------- choosing
    def choose(self, k):
        _, label, what, pattern = next(g for g in GAMES if g[0] == k)
        path = filedialog.askopenfilename(title=label, filetypes=[(what, pattern), ("All files", "*.*")])
        self.use(k, path)

    def use(self, k, path):
        if path and k == "fbk" and Path(path).suffix.lower() in (".rvz", ".wia", ".gcz") \
                and core.find_dolphin_tool() is None:
            messagebox.showinfo(TITLE, "This disc image is compressed. Point to DolphinTool.exe (it comes with "
                                       "the Dolphin emulator) so it can be converted.")
            tool = filedialog.askopenfilename(title="DolphinTool.exe", filetypes=[("DolphinTool", "DolphinTool.exe")])
            if not tool:
                return
            core.DOLPHIN_TOOL = Path(tool)
        if path and k == "saikyo" and Path(path).suffix.lower() == ".cia":
            # The eShop download: Saikyo Card Battle only, decrypted by the player's own decryptor.
            if core.saikyo_title_id(path) != core.SAIKYO_TITLE_ID:
                messagebox.showerror(TITLE, f"{Path(path).name} is not Yu-Gi-Oh! Duel Monsters Saikyo Card Battle.")
                return
            if core.find_decryptor() is None:
                messagebox.showinfo(TITLE, "Saikyo Card Battle's download is encrypted. Choose the folder of your "
                                           "decryptor (the one with decrypt.exe and makerom.exe); it is used for "
                                           "this game only.")
                folder = filedialog.askdirectory(title="Your decryptor's folder")
                if not folder or not core.is_decryptor(folder):
                    if folder:
                        messagebox.showerror(TITLE, "That folder has no decrypt.exe and makerom.exe.")
                    return
                core.SAIKYO_DECRYPTOR = Path(folder)
        if path:
            self.paths[k].set(path)
            self.status[k].set("checking...")
            threading.Thread(target=self.check, args=(k, path), daemon=True).start()

    def check(self, k, path):
        try:
            if k == "dotr":
                core.prepare_workspace(RELEASE, WORK, path)
                self.messages.put(("status", (k, "OK", True)))
                return
            source = core.open_source(k, path, WORK, self.say)
            self.given[k] = {c["number"] for c in self.cards if c.get("game") == k and core.has_model(c, source)}
            n = len(self.given[k])
            self.messages.put(("status", (k, f"OK: {n} card models", True)))
            self.say(f"{core.SOURCES[k]}: gives the models of {n} cards (Cards shows them).")
            app = sum(1 for c in self.cards if c.get("game") == k and c["number"] not in self.given[k])
            if app:
                self.say(f"   {app} more cards' models are only in the Cross Duel app (.apk).")
        except Exception as error:
            self.messages.put(("status", (k, "not usable", False)))
            self.say(f"{Path(path).name}: {error}")

    def show_cards(self, k):
        if k == "dotr":
            messagebox.showinfo(TITLE, "Duelists of the Roses is the game itself: needed for every build.")
            return
        given = self.given.get(k)
        names = sorted(f"{c['number']}  {c['name']}" + ("" if given is None or c["number"] in given else "   (app only)")
                       for c in self.cards if c.get("game") == k)
        win = tk.Toplevel(self)
        win.title(f"{core.SOURCES[k]}: {len(names)} card models")
        box = tk.Listbox(win, width=60, height=30)
        box.pack(fill="both", expand=True)
        for n in names:
            box.insert("end", n)

    def choose_out(self):
        path = filedialog.askdirectory(title="Where to build")
        if path:
            self.out.set(path)

    def refresh_summary(self):
        have = {k for k, ok in self.ok.items() if ok} | {"frame"}     # picture frames need no game
        models = [c for c in self.cards if c.get("game")]
        got = [c for c in models if c["game"] in have and c["number"] in self.given.get(c["game"], {c["number"]})]
        lost = len(models) - len(got)
        self.summary.set(f"With these games: {len(got)} of {len(models)} new card models. "
                         f"{lost} cards will be unobtainable in the game (their game was not given, or their "
                         f"model comes from none of these games).")

    # ---------------------------------------------------------------- mismatches
    def ask_mismatches(self, bad):
        """{number: choice} for the cards whose files differ: "disabled" or "keep"."""
        win = tk.Toplevel(self)
        win.title("Some models differ from this build")
        win.grab_set()
        ttk.Label(win, wraplength=600, text=(
            "These cards' model files are not the same as this release's (another version of the game, "
            "or a changed file). Choose for each: Disable (it cannot be obtained, and computer decks use "
            "its donor card instead) or Keep (it stays in the game without its own 3D model).")).pack(padx=10, pady=8)
        frame = ttk.Frame(win)
        frame.pack(fill="both", expand=True, padx=10)
        choice = {}
        for row, (number, (name, files)) in enumerate(sorted(bad.items())):
            choice[number] = tk.StringVar(value="disabled")
            ttk.Label(frame, text=f"{number}  {name}").grid(row=row, column=0, sticky="w")
            ttk.Radiobutton(frame, text="Disable", variable=choice[number], value="disabled").grid(row=row, column=1)
            ttk.Radiobutton(frame, text="Keep, no model", variable=choice[number], value="keep").grid(row=row, column=2)
        result = {}

        def finish():
            result.update({n: v.get() for n, v in choice.items()})
            win.destroy()
        bar = ttk.Frame(win)
        bar.pack(pady=8)
        ttk.Button(bar, text="Disable all", command=lambda: [v.set("disabled") for v in choice.values()]).pack(side="left", padx=4)
        ttk.Button(bar, text="Keep all", command=lambda: [v.set("keep") for v in choice.values()]).pack(side="left", padx=4)
        ttk.Button(bar, text="Continue", command=finish).pack(side="left", padx=4)
        self.wait_window(win)
        return result

    # ---------------------------------------------------------------- building
    def start(self):
        if not self.ok.get("dotr"):
            messagebox.showerror(TITLE, "Choose your Yu-Gi-Oh! The Duelists of the Roses disc image first.")
            return
        self.build_button.state(["disabled"])
        threading.Thread(target=self.run, daemon=True).start()

    def run(self):
        try:
            message = self.build()
        except Exception as error:
            import traceback
            self.say(traceback.format_exc())
            message = f"The build stopped: {error}"
        self.messages.put(("done", message))

    def build(self):
        paths = {k: self.paths[k].get() for k, ok in self.ok.items() if ok and k != "dotr"}

        def ask(bad):
            self.messages.put(("ask", bad))
            return self.choice_reply.get()
        statuses = core.gather(RELEASE, WORK, self.paths["dotr"].get(), paths, ask, self.say)
        cards = json.loads((WORK / "mods/widescreen/cards.json").read_text(encoding="utf-8"))
        out = Path(self.out.get())
        out.mkdir(parents=True, exist_ok=True)
        if self.kind.get() == "dev":
            return self.dev_build(out, cards, statuses)
        iso = out / f"{TITLE}.iso"
        core.build_disc(RELEASE, WORK, self.paths["dotr"].get(), iso, statuses, self.say)
        self.player_launcher(out, iso)
        lost = sum(1 for s in statuses.values() if s != "normal")
        return (f"Done. Start the game with \"Play {TITLE}\" in {out}.\n\n"
                f"{lost} new cards cannot be obtained in this build (their model's game was not given or did not match).")

    def dev_build(self, out, cards, statuses):
        """The extracted models and pictures, and the cards' statuses, into a copy of the project."""
        if not (out / "tools" / "mod.py").is_file():
            raise ValueError(f"{out} is not a copy of the project (no tools/mod.py)")
        for c in self.cards:
            if c.get("folder") and (WORK / c["folder"]).is_dir():
                shutil.copytree(WORK / c["folder"], out / c["folder"], dirs_exist_ok=True)
        art = WORK / "mods/widescreen/art"
        if art.is_dir():
            shutil.copytree(art, out / "mods/widescreen/art", dirs_exist_ok=True)
        project_cards = out / "mods/widescreen/cards.json"
        mine = json.loads(project_cards.read_text(encoding="utf-8"))
        flags = {e["number"]: e for e in cards if "number" in e}
        for e in mine:
            f = flags.get(e.get("number"), {})
            for k in ("model_missing", "status", "art"):
                if k in f:
                    e[k] = f[k]
        project_cards.write_text(json.dumps(mine, indent=2) + "\n", encoding="utf-8")
        self.dev_kit(out)
        return f"Done. The models and pictures are in {out}; build and play with the DotR Launcher there."

    def dev_kit(self, out):
        """CLAUDE.md (how Claude Code works on this project: the developer's own work in my-dlc/,
        noted as it goes) and the developer's own DLC folder, which is never overwritten."""
        kit = DEV_KIT / "CLAUDE.md"
        target = out / "CLAUDE.md"
        if target.is_file() and DEV_KIT_MARK not in target.read_text(encoding="utf-8", errors="replace")[:200]:
            target = out / "CLAUDE.expanded.md"       # the project has a CLAUDE.md of its own: keep it
            self.say(f"CLAUDE.md is the project's own; Claude's notes for this build are in {target.name}.")
        shutil.copyfile(kit, target)
        mine = out / "mods/widescreen/my-dlc"
        (mine / "dlc").mkdir(parents=True, exist_ok=True)
        (mine / "art").mkdir(exist_ok=True)
        (mine / "models").mkdir(exist_ok=True)
        if not (mine / "cards.json").is_file():
            (mine / "cards.json").write_text("[]\n", encoding="utf-8")
        if not (mine / "NOTES.md").is_file():
            (mine / "NOTES.md").write_text("# My DLC: change notes\n\n", encoding="utf-8")
        self.say(f"Your own DLC folder: {mine}. Open this folder in Claude Code and it reads {target.name}.")

    def player_launcher(self, out, iso):
        """A portable copy of the installed PCSX2 with the player's own settings, BIOS and memory
        cards, the mod's graphics settings, the extra memory the mod needs, and a Play shortcut."""
        import configparser
        emu = out / "PCSX2"
        if not (emu / "pcsx2-qt.exe").is_file():
            if not (INSTALLED_PCSX2 / "pcsx2-qt.exe").is_file():
                raise ValueError(f"PCSX2 is not installed in {INSTALLED_PCSX2}: install PCSX2 (with your own BIOS) "
                                 "and build again.")
            self.say("Copying PCSX2...")
            shutil.copytree(INSTALLED_PCSX2, emu, dirs_exist_ok=True)
        (emu / "portable.txt").write_text("")
        ini = configparser.RawConfigParser(strict=False, interpolation=None, delimiters=("=",),
                                           comment_prefixes=("#",), inline_comment_prefixes=None)
        ini.optionxform = str
        own = emu / "inis" / "PCSX2.ini"
        source = own if own.is_file() else PCSX2_DATA / "inis" / "PCSX2.ini"
        if source.is_file():
            ini.read(source, encoding="utf-8-sig")
        # The graphics settings the mod was made with (tools/release.py pcsx2_graphics).
        graphics = RELEASE / "pcsx2_graphics.json"
        graphics = json.loads(graphics.read_text(encoding="utf-8")) if graphics.is_file() else {}
        if graphics:
            self.say(f"Graphics: the mod's own PCSX2 settings ({graphics.get('upscale_multiplier', '?')}x resolution, "
                     f"{graphics.get('AspectRatio', '?')}).")
        for section, values in {"EmuCore/GS": graphics,
                                "EmuCore/CPU": {"ExtraMemory": "true"},
                                "UI": {"SetupWizardIncomplete": "false", "ConfirmShutdown": "false"}}.items():
            if not ini.has_section(section):
                ini.add_section(section)
            for key, value in values.items():
                ini.set(section, key, value)
        if not ini.has_section("Folders"):
            ini.add_section("Folders")
        for key in ("Bios", "MemoryCards", "Savestates", "Snapshots", "Cheats", "Patches", "InputProfiles"):
            value = ini.get("Folders", key, fallback=key.lower())
            if not Path(value).is_absolute():
                value = str(PCSX2_DATA / value)
            ini.set("Folders", key, value)
        own.parent.mkdir(exist_ok=True)
        with own.open("w", encoding="utf-8") as f:
            ini.write(f, space_around_delimiters=True)
        (out / f"Play {TITLE}.bat").write_text(
            "@echo off\r\n"
            f"start \"\" \"%~dp0PCSX2\\pcsx2-qt.exe\" -nogui -fastboot -- \"%~dp0{iso.name}\"\r\n", encoding="ascii",
            newline="")


def self_test(args):
    """--selftest LOG [dod=PATH] [fbk=PATH] [cross=PATH]: makes one card's model from each game
    given, checks it against the release and writes the result to LOG (the exe has no console)."""
    import tempfile
    import traceback
    log = Path(args[0])
    lines = []
    try:
        cards = manifest()["cards"]
        lines.append(f"release: {len(cards)} cards")
        with tempfile.TemporaryDirectory() as tmp:
            for kind, path in (a.split("=", 1) for a in args[1:]):
                if kind == "dotr":
                    # The textures DLC.BIN is given from the player's disc (setup_core.regen_into).
                    info = json.loads((RELEASE / "release.json").read_text())
                    code = bytearray((RELEASE / "DLC.BIN").read_bytes())
                    core.regen_into(code, info["dlc"]["regen"], RELEASE / "mods" / "widescreen", path, lines.append)
                    lines.append(f"dotr: {len(info['dlc']['regen'])} textures made, DLC.BIN sha256 {core.sha256(bytes(code))}")
                    continue
                source = core.open_source(kind, path, Path(tmp), say=lines.append)
                card = next(c for c in cards if c.get("game") == kind)
                core.run_recipe(card["recipe"], source, Path(tmp) / card["folder"])
                differ = core.check_files(tmp, card["files"])
                lines.append(f"{kind}: {card['name']} ({card['number']}) "
                             + ("matches the release" if not differ else f"DIFFERS {sorted(differ)}"))
        lines.append("ok")
    except Exception:
        lines.append(traceback.format_exc())
    log.write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    if sys.argv[1:2] == ["--selftest"]:
        self_test(sys.argv[2:])
    else:
        Setup().mainloop()
