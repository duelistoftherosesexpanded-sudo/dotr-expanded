#!/usr/bin/env python3
"""DotR Launcher: plays the mod straight from the project, without making a disc image.

Its own copy of PCSX2 (launcher/pcsx2, set up from the installed one, with the same BIOS, memory
cards, controller and graphics settings) plays launcher/game/dotr-dev.iso: a copy of the game's
disc made once. A build (tools/mod.py --dev, in Linux/WSL) leaves its files in launcher/game, and
before the game starts the launcher writes onto that disc only what changed: the program, DLC.BIN
(about a second), and new cards' pictures and models or the second world's map when those changed.

"Build & apply" runs the build. When only cards.json, effects.json, bosses.json or
rarity.json changed and the game is running, the new tables are written into the running game
over PCSX2's PINE link (tools/pine.py) and take effect at once (a duel in progress uses them
from its next look-up; card stats from the next time a card is dealt). Any other change (code,
pictures, models) restarts the game with the new files.

Run it with "DotR Launcher.bat" in the project folder, or: python tools/launcher.py
"""

import configparser
import json
import os
import queue
import re
import shutil
import struct
import subprocess
import sys
import threading
import time
import tkinter as tk
from pathlib import Path
from tkinter import messagebox, ttk

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import gui_theme  # noqa: E402
import pine  # noqa: E402

ROOT = TOOLS.parent
MOD = ROOT / "mods" / "widescreen"
HOME = ROOT / "launcher"
GAME = HOME / "game"
BOOTED = GAME / "booted"
EMU = HOME / "pcsx2"
EXE = EMU / "pcsx2-qt.exe"
ISO = GAME / "dotr-dev.iso"
TEXTURES = HOME / "textures"
SLEEVES = MOD / "sleeves"
INSTALLED = Path(os.environ.get("ProgramFiles", r"C:\Program Files")) / "PCSX2"
USER_DATA = Path.home() / "Documents" / "PCSX2"
PORT = 28012
DLC_BASE = 0x2000000
DLC_MAGIC = 0x31434C44
LIVE_FLAG = DLC_BASE + 0x48
WSL_ROOT = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
NO_WINDOW = getattr(subprocess, "CREATE_NO_WINDOW", 0)
# Files whose saving starts a build when "Rebuild when files are saved" is on.
WATCHED = ["cards.json", "effects.json", "bosses.json", "decks.json", "community.json", "mod.json", "rarity.json",
           "achievements.json",
           "dlc/*.c", "dlc/*.h", "src/**/*.c", "art/*.png", "sleeves/*.png", "shop/*.png", "title/*.png",
           "my-dlc/cards.json", "my-dlc/hooks.json", "my-dlc/dlc/*.c", "my-dlc/dlc/*.h", "my-dlc/**/*.png"]
BUILD_COMMAND = (
    # Windows line endings are taken out, touching only files that have them (rewriting the others
    # would look like a new save to "Rebuild when files are saved").
    f"cd '{WSL_ROOT}' && for f in mods/widescreen/*.json mods/widescreen/dlc/*.h mods/widescreen/dlc/*.c "
    f"mods/widescreen/my-dlc/*.json mods/widescreen/my-dlc/dlc/*.h mods/widescreen/my-dlc/dlc/*.c; "
    f"do if grep -q $'\\r' \"$f\"; then sed -i 's/\\r//' \"$f\"; fi; done && python3 tools/mod.py widescreen --define WSUI_ANCHOR=1 "
    f"--variant anchored --dev launcher/game > build/ntsc-u/mods/launcher.log 2>&1; code=$?; "
    f"grep -E '^error' -A 30 build/ntsc-u/mods/launcher.log; echo \"exit $code\"")


def clean_image():
    for path in sorted((ROOT / "baserom").glob("*.iso")):
        return path
    return None


# ------------------------------------------------------------------------- PCSX2 set-up

def read_ini(path):
    ini = configparser.RawConfigParser(strict=False, interpolation=None, delimiters=("=",),
                                       comment_prefixes=("#",), inline_comment_prefixes=None)
    ini.optionxform = str
    if path.is_file():
        ini.read(path, encoding="utf-8-sig")
    return ini


def repeated_keys(path):
    """{(section, key): [values]} for keys PCSX2 writes more than once in a section: a controller
    button bound to a gamepad button and a key, say. configparser keeps only the last of them."""
    found, section = {}, None
    if path.is_file():
        for line in path.read_text(encoding="utf-8-sig").splitlines():
            s = line.strip()
            if s.startswith("[") and s.endswith("]"):
                section = s[1:-1]
            elif "=" in s and section is not None and not s.startswith("#"):
                key, value = (p.strip() for p in s.split("=", 1))
                found.setdefault((section, key), []).append(value)
    return {k: v for k, v in found.items() if len(v) > 1}


def write_ini(ini, path, repeated):
    """Writes the settings, each repeated key (repeated_keys) once per value again."""
    import io
    text = io.StringIO()
    ini.write(text, space_around_delimiters=True)
    out, section = [], None
    for line in text.getvalue().splitlines():
        s = line.strip()
        if s.startswith("[") and s.endswith("]"):
            section = s[1:-1]
        elif "=" in s and section is not None:
            key = s.split("=", 1)[0].strip()
            if (section, key) in repeated:
                out.extend(f"{key} = {value}" for value in repeated[(section, key)])
                continue
        out.append(line)
    path.write_text("\n".join(out) + "\n", encoding="utf-8")


def setup_emulator(say, dump_textures=False):
    """The launcher's own PCSX2: a copy of the installed program in portable mode. Its settings
    start as a copy of the installed PCSX2's (the first time) and are its own after that, so
    whatever is changed in it (PCSX2 settings button, or in the game's window) is kept; each start
    only sets again what the launcher needs: PINE, the extra memory, the texture folder.

    Texture replacement is on, from the launcher's own folder (launcher/textures/<serial>/
    replacements, PCSX2's names); dump_textures also saves each texture the game uses to
    launcher/textures/<serial>/dumps, to learn a texture's name."""
    global USER_DATA
    import pcsx2_find
    installed = pcsx2_find.find(skip=[EMU])
    if installed:
        USER_DATA = pcsx2_find.data_folder(installed)
    if not EXE.is_file():
        if not installed:
            raise RuntimeError("PCSX2 was not found: choose where it is (the launcher asks), or install it")
        say(f"Copying PCSX2 from {installed} (once)...")
        shutil.copytree(installed, EMU, dirs_exist_ok=True,
                        ignore=shutil.ignore_patterns("memcards", "sstates", "snaps", "cache", "logs"))
    # Empty: PCSX2 takes any text in it as the folder for its settings and data.
    (EMU / "portable.txt").write_text("")
    try:
        if set_exe_icon():
            say("The game's icon is set on the launcher's PCSX2.")
    except OSError:
        pass
    own = EMU / "inis" / "PCSX2.ini"
    first = not own.is_file()
    if first:
        say("Taking the settings of your installed PCSX2 (once).")
    source = USER_DATA / "inis" / "PCSX2.ini" if first else own
    user = read_ini(source)
    repeated = repeated_keys(source)
    for section, values in {
        "EmuCore": {"EnablePINE": "true", "PINESlot": str(PORT), "HostFs": "true"},
        "EmuCore/CPU": {"ExtraMemory": "true"},
        "UI": {"ConfirmShutdown": "false", "HideMainWindowWhenRunning": "true", "SetupWizardIncomplete": "false",
               "StartPaused": "false", "PauseOnFocusLoss": "false"},
        "AutoUpdater": {"CheckAtStartup": "false"},
        # Replacements need the hash cache (texture preloading "full").
        "EmuCore/GS": {"LoadTextureReplacements": "true", "texture_preloading": "2",
                       "DumpReplaceableTextures": "true" if dump_textures else "false"},
    }.items():
        if not user.has_section(section):
            user.add_section(section)
        for key, value in values.items():
            user.set(section, key, value)
    if not user.has_section("Folders"):
        user.add_section("Folders")
    # The installed PCSX2's folders (memory cards, save states, patches, ...), by full path.
    for key in ("Bios", "Snapshots", "Savestates", "MemoryCards", "Logs", "Cheats", "Patches", "UserResources",
                "InputProfiles", "Videos"):
        value = user.get("Folders", key, fallback=key.lower())
        if not Path(value).is_absolute():
            value = str(USER_DATA / value)
        user.set("Folders", key, value)
    TEXTURES.mkdir(parents=True, exist_ok=True)
    user.set("Folders", "Textures", str(TEXTURES))
    (EMU / "inis").mkdir(exist_ok=True)
    write_ini(user, EMU / "inis" / "PCSX2.ini", repeated)
    # Per-game settings: the installed PCSX2's, only where the launcher's PCSX2 has none of its own.
    if (USER_DATA / "gamesettings").is_dir():
        (EMU / "gamesettings").mkdir(exist_ok=True)
        for source in (USER_DATA / "gamesettings").iterdir():
            if source.is_file() and not (EMU / "gamesettings" / source.name).exists():
                shutil.copyfile(source, EMU / "gamesettings" / source.name)


def sync_disc(say):
    """Puts the last build on the working disc, writing only what changed (PCSX2 must be closed:
    it keeps the disc locked). tools/mod.py --dev leaves its files in launcher/game."""
    import iso_files
    info = json.loads((GAME / "build.json").read_text())
    staging = GAME / "disc"
    staged = json.loads((staging / "staged.json").read_text()) if (staging / "staged.json").is_file() else {}
    written_path = staging / "written.json"
    written = json.loads(written_path.read_text()) if written_path.is_file() else {}
    code = bytearray((GAME / "DLC.BIN").read_bytes())
    disc = iso_files.Image(ISO)
    try:
        for path, entry in staged.items():
            if written.get(path, {}).get("sha256") != entry["sha256"]:
                say(f"Writing {path.strip('/')} to the working disc...")
                sector = disc.put(path, (staging / entry["file"]).read_bytes())
                written[path] = dict(sha256=entry["sha256"], sector=sector)
                written_path.write_text(json.dumps(written, indent=2) + "\n")
        if "/DLCMDL.BIN" in staged:
            # dlc_header.models_sector: where the new cards' model slots are.
            struct.pack_into("<I", code, 0x4C, written["/DLCMDL.BIN"]["sector"])
        # Padded to whole MiB, so DLC.BIN keeps its place on the disc while the code grows.
        dlc_sector = disc.put("/DLC.BIN", bytes(code) + bytes(-len(code) % 0x100000))
    finally:
        disc.close()
    exe = bytearray((GAME / "SLUS_205.15").read_bytes())
    struct.pack_into("<III", exe, info["info_offset"], dlc_sector, len(code), 0)
    with ISO.open("r+b") as f:
        f.seek(info["iso_extent"])
        f.write(exe)


ICON = HOME / "icon.ico"   # the game window's (and the launcher's) icon, when there is one


def set_exe_icon(exe=EXE, icon=ICON):
    """Puts the icon into the launcher's copy of pcsx2-qt.exe (its main icon group), which the
    taskbar shows; the installed PCSX2 is not touched. Done again only when the icon changes."""
    import ctypes
    import struct as st
    from ctypes import wintypes
    stamp = HOME / "icon.applied"
    if not Path(icon).is_file() or not Path(exe).is_file():
        return False
    data = Path(icon).read_bytes()
    if stamp.is_file() and stamp.read_bytes() == data[:64] + st.pack("<Q", len(data)):
        return False
    k32 = ctypes.windll.kernel32
    k32.LoadLibraryExW.restype = wintypes.HMODULE
    k32.BeginUpdateResourceW.restype = wintypes.HANDLE
    k32.BeginUpdateResourceW.argtypes = [wintypes.LPCWSTR, wintypes.BOOL]
    k32.UpdateResourceW.argtypes = [wintypes.HANDLE, wintypes.LPVOID, wintypes.LPVOID, wintypes.WORD,
                                    wintypes.LPVOID, wintypes.DWORD]
    k32.EndUpdateResourceW.argtypes = [wintypes.HANDLE, wintypes.BOOL]
    # The exe's first icon group (the one Windows shows for it).
    module = k32.LoadLibraryExW(str(exe), None, 0x2)   # LOAD_LIBRARY_AS_DATAFILE
    names = []
    EnumProc = ctypes.WINFUNCTYPE(wintypes.BOOL, wintypes.HMODULE, wintypes.LPVOID, wintypes.LPVOID, wintypes.LPARAM)

    def each(_m, _t, name, _p):
        names.append(name if (name or 0) >> 16 else int(name or 0))
        return False   # the first is enough
    k32.EnumResourceNamesW.argtypes = [wintypes.HMODULE, wintypes.LPVOID, EnumProc, wintypes.LPARAM]
    k32.FreeLibrary.argtypes = [wintypes.HMODULE]
    callback = EnumProc(each)
    k32.EnumResourceNamesW(module, 14, callback, 0)   # RT_GROUP_ICON
    k32.FreeLibrary(module)
    if not names or not isinstance(names[0], int):
        return False
    group = names[0]
    count = st.unpack_from("<H", data, 4)[0]
    handle = k32.BeginUpdateResourceW(str(exe), False)
    if not handle:
        return False
    header = st.pack("<HHH", 0, 1, count)
    entries = b""
    for i in range(count):
        w, h, colours, _, planes, bits, size, offset = st.unpack_from("<BBBBHHII", data, 6 + 16 * i)
        image = data[offset:offset + size]
        k32.UpdateResourceW(handle, ctypes.c_void_p(3), ctypes.c_void_p(5000 + i), 0,
                            ctypes.c_char_p(image), len(image))   # RT_ICON
        entries += st.pack("<BBBBHHIH", w, h, colours, 0, planes, bits, size, 5000 + i)
    directory = header + entries
    k32.UpdateResourceW(handle, ctypes.c_void_p(14), ctypes.c_void_p(group), 0,
                        ctypes.c_char_p(directory), len(directory))
    ok = k32.EndUpdateResourceW(handle, False)
    if ok:
        stamp.write_bytes(data[:64] + st.pack("<Q", len(data)))
        # Windows keeps icons in a cache: tell it they changed.
        ctypes.windll.shell32.SHChangeNotify(0x08000000, 0, None, None)   # SHCNE_ASSOCCHANGED
    return bool(ok)


def set_window_icons(pid, icon=ICON):
    """Gives the visible windows of process pid the icon (title bar, taskbar, Alt+Tab) in place
    of PCSX2's own, through WM_SETICON; returns how many were changed."""
    import ctypes
    from ctypes import wintypes
    if not Path(icon).is_file():
        return 0
    user32 = ctypes.windll.user32
    user32.LoadImageW.restype = wintypes.HANDLE
    user32.SendMessageW.restype = wintypes.LPARAM
    user32.SendMessageW.argtypes = [wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPARAM]
    if not hasattr(set_window_icons, "handles"):
        big = user32.LoadImageW(None, str(icon), 1, 256, 256, 0x10)
        small = user32.LoadImageW(None, str(icon), 1, 32, 32, 0x10)
        set_window_icons.handles = (small, big)
    small, big = set_window_icons.handles
    changed = []

    @ctypes.WINFUNCTYPE(ctypes.c_bool, wintypes.HWND, wintypes.LPARAM)
    def each(hwnd, _):
        owner = wintypes.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(owner))
        if owner.value == pid and user32.IsWindowVisible(hwnd):
            if user32.SendMessageW(hwnd, 0x7F, 1, 0) != big:   # WM_GETICON, ICON_BIG
                user32.SendMessageW(hwnd, 0x80, 0, small)          # WM_SETICON, ICON_SMALL
                user32.SendMessageW(hwnd, 0x80, 1, big)            # WM_SETICON, ICON_BIG
                changed.append(hwnd)
        return True
    user32.EnumWindows(each, 0)
    return len(changed)


def live_apply(info, disc_dirty=False):
    """Compares the last build with what the running game was started with. When they differ only
    inside the live region (card, effect and boss tables), writes the changed bytes into the game
    over PINE and raises the DLC's live flag (header.c Dlc_LiveCheck). Returns (reasons the game
    must restart instead, bytes written)."""
    old_dlc, new_dlc = (BOOTED / "DLC.BIN").read_bytes(), (GAME / "DLC.BIN").read_bytes()
    old_exe, new_exe = (BOOTED / "SLUS_205.15").read_bytes(), (GAME / "SLUS_205.15").read_bytes()
    start, end = info.get("live", [0, 0])
    reasons = []
    if old_exe != new_exe:
        reasons.append("the game's program changed")
    if disc_dirty:
        reasons.append("pictures, models or the map changed")
    if len(old_dlc) != len(new_dlc) or old_dlc[:start] != new_dlc[:start] or old_dlc[end:] != new_dlc[end:]:
        reasons.append("DLC code changed")
    if reasons:
        return reasons, 0
    words = list(pine.changed_words(old_dlc, new_dlc, DLC_BASE, start, end))
    if words:
        with pine.Pine(PORT) as link:
            if link.read32(DLC_BASE) != DLC_MAGIC:
                raise pine.PineError("the DLC is not loaded yet (wait for the title screen)")
            link.write_words(words)
            link.write32(LIVE_FLAG, 1)
        shutil.copyfile(GAME / "DLC.BIN", BOOTED / "DLC.BIN")
    return [], 8 * len(words)


def setup_disc(say):
    if ISO.is_file():
        return
    source = clean_image()
    if source is None:
        raise RuntimeError("no disc image in baserom/")
    GAME.mkdir(parents=True, exist_ok=True)
    say("Copying the game's disc (once)...")
    shutil.copyfile(source, ISO)
    # A fresh copy has none of the build's files yet.
    (GAME / "disc" / "written.json").unlink(missing_ok=True)


# ------------------------------------------------------------------------- the window

class Launcher(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("DotR Launcher")
        self.minsize(760, 520)
        gui_theme.apply(self)
        if ICON.is_file():
            try:
                self.iconbitmap(default=str(ICON))
            except tk.TclError:
                pass
        self.process = None
        self.building = False
        self.build_again = False
        self.disc_dirty = False
        self.messages = queue.Queue()
        self.stamps = self.watched_stamps()
        self.changed_at = None
        self.v = {k: tk.StringVar() for k in ("status", "game")}
        self.watch = tk.BooleanVar(value=True)
        self.auto_restart = tk.BooleanVar(value=True)
        self.fullscreen = tk.BooleanVar(value=False)
        self.dump_textures = tk.BooleanVar(value=False)
        self._build_ui()
        self.after(100, self.pump)
        self.after(1000, self.tick)
        self.say("Ready. Play starts the game from the project's files; edits are applied as you save them.")

    def _build_ui(self):
        top = ttk.Frame(self)
        top.pack(fill="x", padx=10, pady=(10, 4))
        self.play_button = ttk.Button(top, text="Play", command=self.play)
        self.play_button.pack(side="left")
        self.build_button = ttk.Button(top, text="Build && apply", command=self.build)
        self.build_button.pack(side="left", padx=6)
        ttk.Button(top, text="Restart game", command=self.restart).pack(side="left")
        ttk.Button(top, text="Stop game", command=self.stop).pack(side="left", padx=6)
        ttk.Button(top, text="Card Maker", command=self.card_maker).pack(side="right")
        ttk.Button(top, text="PCSX2 settings", command=self.emulator_settings).pack(side="right", padx=6)

        options = ttk.Frame(self)
        options.pack(fill="x", padx=10, pady=4)
        ttk.Checkbutton(options, text="Rebuild and apply when mod files are saved", variable=self.watch).pack(side="left")
        ttk.Checkbutton(options, text="Restart when a change needs it", variable=self.auto_restart).pack(side="left", padx=12)
        ttk.Checkbutton(options, text="Fullscreen", variable=self.fullscreen).pack(side="left")
        ttk.Checkbutton(options, text="Save textures (to replace them)", variable=self.dump_textures).pack(side="left", padx=12)

        sleeves = ttk.LabelFrame(self, text="Card sleeves (your face-down cards; pick one in the game's MOD OPTIONS)")
        sleeves.pack(fill="x", padx=10, pady=4)
        self.sleeve_list = tk.Listbox(sleeves, height=4, exportselection=False)
        self.sleeve_list.pack(side="left", fill="x", expand=True, padx=6, pady=6)
        buttons = ttk.Frame(sleeves)
        buttons.pack(side="left", padx=6)
        ttk.Button(buttons, text="Add sleeve...", command=self.add_sleeve).pack(fill="x")
        ttk.Button(buttons, text="Remove", command=self.remove_sleeve).pack(fill="x", pady=4)
        ttk.Button(buttons, text="Open folder", command=lambda: os.startfile(str(SLEEVES))).pack(fill="x")
        self.refresh_sleeves()

        status = ttk.Frame(self)
        status.pack(fill="x", padx=10, pady=4)
        ttk.Label(status, textvariable=self.v["game"], foreground=gui_theme.STATUS).pack(side="left")
        ttk.Label(status, textvariable=self.v["status"], foreground=gui_theme.HINT).pack(side="right")

        self.log = tk.Text(self, height=20, font=("Consolas", 9), wrap="word")
        self.log.pack(fill="both", expand=True, padx=10, pady=(4, 10))
        self.v["game"].set("Game: not running")

    # ---------------------------------------------------------------- messages
    def say(self, text):
        """Thread-safe: queued, shown by pump()."""
        self.messages.put(("log", text))

    def pump(self):
        while True:
            try:
                kind, value = self.messages.get_nowait()
            except queue.Empty:
                break
            if kind == "log":
                line = time.strftime("%H:%M:%S  ") + value.rstrip() + "\n"
                self.log.insert("end", line)
                self.log.see("end")
                try:
                    with (HOME / "launcher.log").open("a", encoding="utf-8") as f:
                        f.write(line)
                except OSError:
                    pass
            elif kind == "call":
                value()
        self.after(100, self.pump)

    def later(self, function):
        self.messages.put(("call", function))

    # ---------------------------------------------------------------- the game
    def running(self):
        return self.process is not None and self.process.poll() is None

    def tick(self):
        self.v["game"].set("Game: running" if self.running() else "Game: not running")
        if self.running():
            # The game's own icon on PCSX2's window (set again on new windows, e.g. fullscreen).
            try:
                set_window_icons(self.process.pid)
            except (OSError, AttributeError):
                pass
        if self.watch.get():
            stamps = self.watched_stamps()
            if stamps != self.stamps:
                self.stamps = stamps
                self.changed_at = time.time()
            # Builds once the files have been quiet for a second (an editor may save in steps).
            if self.changed_at and time.time() - self.changed_at > 1.0:
                self.changed_at = None
                self.say("Mod files changed.")
                self.build()
        self.after(500, self.tick)

    def watched_stamps(self):
        stamps = {}
        for pattern in WATCHED:
            for path in MOD.glob(pattern):
                try:
                    stamps[str(path)] = path.stat().st_mtime_ns
                except OSError:
                    pass
        return stamps

    def play(self):
        if self.running():
            self.say("The game is already running.")
            return
        if not (GAME / "DLC.BIN").is_file() or not (GAME / "SLUS_205.15").is_file():
            self.say("No game files yet: building them first.")
            self.build(then_start=True)
            return
        self.start()

    def find_pcsx2(self):
        """True when the launcher has a PCSX2 (its own copy, or one found or chosen to copy)."""
        import pcsx2_find
        if EXE.is_file() or pcsx2_find.find(skip=[EMU]):
            return True
        if pcsx2_find.ask(self, "DotR Launcher"):
            return True
        self.say("No PCSX2 chosen: install PCSX2 (or unzip it), then press Play again.")
        return False

    def start(self):
        if not self.find_pcsx2():
            return
        dump = self.dump_textures.get()

        def run():
            try:
                setup_emulator(self.say, dump)
                setup_disc(self.say)
                sync_disc(self.say)
            except (OSError, RuntimeError, ValueError, KeyError) as error:
                self.say(f"Could not get the game ready: {error}")
                return
            self.later(self.launch)
        threading.Thread(target=run, daemon=True).start()

    def launch(self):
        BOOTED.mkdir(parents=True, exist_ok=True)
        for name in ("DLC.BIN", "SLUS_205.15", "build.json"):
            if (GAME / name).is_file():
                shutil.copyfile(GAME / name, BOOTED / name)
        self.disc_dirty = False
        command = [str(EXE), "-nogui", "-fastboot", "-fullscreen" if self.fullscreen.get() else "-nofullscreen",
                   "--", str(ISO)]
        try:
            self.process = subprocess.Popen(command, cwd=str(EMU), stdin=subprocess.DEVNULL,
                                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except OSError as error:
            self.say(f"Could not start PCSX2: {error}")
            return
        self.say("Game started.")

    def stop(self, quiet=False):
        if not self.running():
            if not quiet:
                self.say("The game is not running.")
            return
        # A polite close first (PCSX2 then writes its memory cards), then a hard one.
        subprocess.run(["taskkill", "/PID", str(self.process.pid)], capture_output=True, creationflags=NO_WINDOW)
        try:
            self.process.wait(timeout=8)
        except subprocess.TimeoutExpired:
            self.process.kill()
            self.process.wait(timeout=5)
        self.process = None
        self.say("Game stopped.")

    def restart(self):
        self.stop(quiet=True)
        self.start()

    def emulator_settings(self):
        """Opens the launcher's PCSX2 with its window, to change its settings (kept for the next
        Play). Not while the game runs: both would write the settings file."""
        if self.running():
            self.say("Stop the game first, then open PCSX2's settings.")
            return
        if not self.find_pcsx2():
            return
        try:
            setup_emulator(self.say, self.dump_textures.get())
            subprocess.Popen([str(EXE)], cwd=str(EMU))
        except (OSError, RuntimeError) as error:
            self.say(f"Could not open PCSX2: {error}")
            return
        self.say("PCSX2 opened. Change its settings there (Settings menu) and close it; Play uses them.")

    # ---------------------------------------------------------------- sleeves
    def refresh_sleeves(self):
        SLEEVES.mkdir(parents=True, exist_ok=True)
        self.sleeve_list.delete(0, "end")
        for path in sorted(SLEEVES.glob("*.png")):
            self.sleeve_list.insert("end", path.stem)

    def add_sleeve(self):
        from tkinter import filedialog
        path = filedialog.askopenfilename(title="Choose a picture for the sleeve",
                                          filetypes=[("Pictures", "*.png *.jpg *.jpeg *.bmp *.gif *.webp"),
                                                     ("All files", "*.*")])
        if not path:
            return
        try:
            from PIL import Image
            name = re.sub(r"[^A-Za-z0-9 _.-]", "", Path(path).stem).strip()[:16] or "sleeve"
            Image.open(path).convert("RGB").save(SLEEVES / f"{name}.png")
        except (OSError, ValueError) as error:
            messagebox.showerror("Card sleeves", f"Could not read that picture: {error}")
            return
        self.refresh_sleeves()
        self.say(f"Sleeve {name} added (it is cropped to a square). Building it in...")
        self.build()

    def remove_sleeve(self):
        sel = self.sleeve_list.curselection()
        if not sel:
            self.say("Choose a sleeve in the list first.")
            return
        name = self.sleeve_list.get(sel[0])
        if not messagebox.askyesno("Card sleeves", f"Remove the sleeve {name}?"):
            return
        (SLEEVES / f"{name}.png").unlink(missing_ok=True)
        self.refresh_sleeves()
        self.say(f"Sleeve {name} removed.")
        self.build()

    def card_maker(self):
        pyw = Path(sys.executable).with_name("pythonw.exe")
        subprocess.Popen([str(pyw if pyw.is_file() else sys.executable), str(TOOLS / "card_maker.py")])

    # ---------------------------------------------------------------- building
    def build(self, then_start=False):
        if self.building:
            self.build_again = True
            return
        self.building = True
        self.build_button.configure(state="disabled")
        self.v["status"].set("Building...")
        self.say("Building...")
        started = time.time()

        def run():
            output = ""
            try:
                setup_disc(self.say)
                process = subprocess.run(["wsl.exe", "-d", "Ubuntu-22.04", "-e", "bash", "-lc", BUILD_COMMAND],
                                         capture_output=True, text=True, encoding="utf-8", errors="replace",
                                         creationflags=NO_WINDOW)
                output = (process.stdout + process.stderr).replace("\x00", "")
            except (OSError, RuntimeError) as error:
                output = f"could not start the build: {error}"
            ok = "exit 0" in output
            seconds = time.time() - started
            self.later(lambda: self.built(ok, output, seconds, then_start))
        threading.Thread(target=run, daemon=True).start()

    def built(self, ok, output, seconds, then_start):
        self.building = False
        self.build_button.configure(state="normal")
        # Whatever the build itself touched is not a new save.
        if not self.build_again:
            self.stamps = self.watched_stamps()
            self.changed_at = None
        if not ok:
            lines = [l for l in output.splitlines() if l.strip() and "nested virtualization" not in l
                     and not l.startswith("exit ")]
            self.say("The build failed:\n" + "\n".join(lines[-40:]))
            self.say("Full log: build/ntsc-u/mods/launcher.log")
            self.v["status"].set("Build failed")
        else:
            info = json.loads((GAME / "build.json").read_text())
            if info.get("disc_changed"):
                self.disc_dirty = True
                self.say("Updated on the disc: " + ", ".join(p.strip("/") for p in info["disc_changed"]))
            self.say(f"Built in {seconds:.0f} s.")
            self.v["status"].set(time.strftime("Built %H:%M:%S"))
            if then_start:
                self.start()
            elif self.running():
                self.apply(info)
            else:
                self.say("Press Play to start the game with it.")
        if self.build_again:
            self.build_again = False
            self.build()

    def apply(self, info):
        """Writes the new tables into the running game, or restarts it when the change needs that."""
        try:
            reasons, count = live_apply(info, self.disc_dirty)
        except (OSError, pine.PineError) as error:
            self.say(f"Could not write into the game: {error}")
            return
        if not reasons:
            self.say(f"Applied live: {count} bytes of card, effect and boss tables." if count
                     else "Nothing changed for the game.")
            return
        if self.auto_restart.get():
            self.say("Restarting the game: " + "; ".join(reasons) + ".")
            self.restart()
        else:
            self.say("Restart the game to see this change: " + "; ".join(reasons) + ".")

    def destroy(self):
        if self.running() and messagebox.askyesno("DotR Launcher", "Stop the game too?"):
            self.stop(quiet=True)
        super().destroy()


if __name__ == "__main__":
    Launcher().mainloop()
