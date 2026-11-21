"""Where PCSX2 is on this computer, for the setup tool (tools/setup_tool.py) and the launcher
(tools/launcher.py).

  find()             the folder holding pcsx2-qt.exe: the one chosen before, the usual install
                     folders, the registry's uninstall entries, then Desktop / Downloads / Documents
  data_folder(f)     where that PCSX2 keeps its settings, BIOS and memory cards: the folder itself
                     for a portable copy, else Documents\\PCSX2
  remember(f)        keeps the player's choice for next time
  ask(parent)        lets the player point to pcsx2-qt.exe (tkinter), remembers and returns its folder
"""
import json
import os
from pathlib import Path

EXE = "pcsx2-qt.exe"
SETTINGS = Path(os.environ.get("APPDATA", str(Path.home()))) / "DotR Expanded" / "pcsx2.json"


def is_pcsx2(folder):
    return bool(folder) and (Path(folder) / EXE).is_file()


def remembered():
    try:
        folder = json.loads(SETTINGS.read_text(encoding="utf-8")).get("folder")
    except (OSError, ValueError):
        return None
    return Path(folder) if is_pcsx2(folder) else None


def remember(folder):
    SETTINGS.parent.mkdir(parents=True, exist_ok=True)
    SETTINGS.write_text(json.dumps({"folder": str(folder)}, indent=1), encoding="utf-8")


def _registry():
    """Install folders of PCSX2 from Windows' uninstall entries."""
    try:
        import winreg
    except ImportError:
        return []
    found = []
    keys = [(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"),
            (winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall"),
            (winreg.HKEY_CURRENT_USER, r"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall")]
    for root, path in keys:
        try:
            with winreg.OpenKey(root, path) as key:
                for i in range(winreg.QueryInfoKey(key)[0]):
                    try:
                        with winreg.OpenKey(key, winreg.EnumKey(key, i)) as entry:
                            name = winreg.QueryValueEx(entry, "DisplayName")[0]
                            if "pcsx2" not in str(name).lower():
                                continue
                            for value in ("InstallLocation", "DisplayIcon"):
                                try:
                                    v = str(winreg.QueryValueEx(entry, value)[0]).strip('"').split(",")[0]
                                except OSError:
                                    continue
                                found.append(Path(v).parent if v.lower().endswith(".exe") else Path(v))
                    except OSError:
                        continue
        except OSError:
            continue
    return found


def _search(root, depth):
    """Folders holding pcsx2-qt.exe under root, at most depth levels down."""
    root = Path(root)
    if not root.is_dir():
        return
    stack = [(root, 0)]
    while stack:
        folder, d = stack.pop()
        if (folder / EXE).is_file():
            yield folder
            continue
        if d >= depth:
            continue
        try:
            for sub in folder.iterdir():
                if sub.is_dir() and not sub.name.startswith(".") and sub.name.lower() not in ("node_modules", "$recycle.bin"):
                    stack.append((sub, d + 1))
        except OSError:
            continue


def find(skip=()):
    """The PCSX2 folder, or None. skip: folders not to offer (the tools' own copies)."""
    skip = {Path(s).resolve() for s in skip}

    def ok(f):
        return is_pcsx2(f) and Path(f).resolve() not in skip
    choice = remembered()
    if choice and ok(choice):
        return choice
    env = os.environ
    for f in [Path(env.get("ProgramFiles", r"C:\Program Files")) / "PCSX2",
              Path(env.get("ProgramFiles(x86)", r"C:\Program Files (x86)")) / "PCSX2",
              Path(env.get("LOCALAPPDATA", "")) / "Programs" / "PCSX2"] + _registry():
        if ok(f):
            return f
    home = Path.home()
    for root in (home / "Desktop", home / "Downloads", home / "Documents", Path("C:/Emulators"), Path("C:/Games")):
        for f in _search(root, 3):
            if ok(f):
                return f
    return None


def data_folder(folder):
    """Where this PCSX2 keeps its settings: the folder itself when portable."""
    folder = Path(folder)
    if (folder / "portable.txt").exists() or (folder / "portable.ini").exists():
        return folder
    return Path.home() / "Documents" / "PCSX2"


def ask(parent=None, title="Where is PCSX2?"):
    """Asks the player for pcsx2-qt.exe; remembers its folder. None when they cancel."""
    from tkinter import filedialog, messagebox
    messagebox.showinfo(title, "PCSX2 wasn't found on this computer.\n\nPoint to pcsx2-qt.exe in the folder "
                        "where you installed or unzipped PCSX2.", parent=parent)
    while True:
        path = filedialog.askopenfilename(parent=parent, title="Choose pcsx2-qt.exe",
                                          filetypes=[("PCSX2", EXE), ("Programs", "*.exe")])
        if not path:
            return None
        folder = Path(path).parent
        if is_pcsx2(folder):
            remember(folder)
            return folder
        messagebox.showerror(title, f"{Path(path).name} isn't PCSX2. Choose pcsx2-qt.exe.", parent=parent)
