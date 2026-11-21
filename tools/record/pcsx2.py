"""Driving the Launcher's PCSX2 for recordings: start it with the dev disc, press the controller's
buttons through the keyboard keys bound to them (launcher/pcsx2/inis/PCSX2.ini, Pad1), and take
pictures of its window.

  BUTTONS                the PS2 buttons and the keys bound to them
  start(), game_window(), focus(), press("cross"), hold("right", 0.5), shot("path.png")
"""
import ctypes
import ctypes.wintypes as wt
import subprocess
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
EXE = ROOT / "launcher" / "pcsx2" / "pcsx2-qt.exe"
DISC = ROOT / "launcher" / "game" / "dotr-dev.iso"

# PS2 button -> Windows virtual key (the keyboard key bound to it in PCSX2's Pad1).
BUTTONS = {"up": 0x26, "down": 0x28, "left": 0x25, "right": 0x27, "cross": ord("K"), "circle": ord("L"),
           "square": ord("J"), "triangle": ord("I"), "start": 0x0D, "select": 0x08, "l1": ord("Q"),
           "r1": ord("E"), "l2": ord("1"), "r2": ord("3")}
EXTENDED = {0x25, 0x26, 0x27, 0x28}

user32 = ctypes.WinDLL("user32", use_last_error=True)
ctypes.windll.shcore.SetProcessDpiAwareness(2)   # pixel coordinates, whatever the display scaling


class KEYBDINPUT(ctypes.Structure):
    _fields_ = [("wVk", wt.WORD), ("wScan", wt.WORD), ("dwFlags", wt.DWORD), ("time", wt.DWORD),
                ("dwExtraInfo", ctypes.c_size_t)]


class INPUT(ctypes.Structure):
    class _U(ctypes.Union):
        _fields_ = [("ki", KEYBDINPUT), ("pad", ctypes.c_byte * 32)]
    _anonymous_ = ("u",)
    _fields_ = [("type", wt.DWORD), ("u", _U)]


def _key(vk, up):
    flags = (0x0002 if up else 0) | (0x0001 if vk in EXTENDED else 0)
    scan = user32.MapVirtualKeyW(vk, 0)
    i = INPUT(type=1)
    i.ki = KEYBDINPUT(vk, scan, flags, 0, 0)
    user32.SendInput(1, ctypes.byref(i), ctypes.sizeof(INPUT))


def pids():
    out = subprocess.run(["tasklist", "/FI", "IMAGENAME eq pcsx2-qt.exe", "/FO", "CSV", "/NH"],
                         capture_output=True, text=True).stdout
    return {int(line.split('","')[1]) for line in out.splitlines() if line.startswith('"pcsx2-qt.exe"')}


def game_window():
    """(hwnd, title) of PCSX2's biggest visible window: the game's, while a game runs."""
    ids, best = pids(), None
    proc = wt.DWORD()
    rect = wt.RECT()

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def each(hwnd, _):
        nonlocal best
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(proc))
        if proc.value in ids and user32.IsWindowVisible(hwnd):
            user32.GetClientRect(hwnd, ctypes.byref(rect))
            area = rect.right * rect.bottom
            n = user32.GetWindowTextLengthW(hwnd)
            title = ctypes.create_unicode_buffer(n + 1)
            user32.GetWindowTextW(hwnd, title, n + 1)
            if best is None or area > best[2]:
                best = (hwnd, title.value, area)
        return True
    user32.EnumWindows(each, 0)
    return best[:2] if best else None


def start(disc=DISC, wait=60):
    """Starts PCSX2 with the disc (unless it runs already) and waits for the game's window."""
    if not pids():
        # Detached, so it keeps running after the script that started it ends.
        flags = 0x00000008 | 0x00000200 | 0x01000000   # DETACHED, NEW_PROCESS_GROUP, BREAKAWAY_FROM_JOB
        try:
            subprocess.Popen([str(EXE), "-fastboot", "--", str(disc)], cwd=str(EXE.parent), creationflags=flags)
        except OSError:
            subprocess.Popen([str(EXE), "-fastboot", "--", str(disc)], cwd=str(EXE.parent),
                             creationflags=0x00000008 | 0x00000200)
    t = time.time()
    while time.time() - t < wait:
        w = game_window()
        if w and w[1] and "PCSX2" not in w[1][:5]:
            return w
        time.sleep(1)
    return game_window()


def focus():
    w = game_window()
    if not w:
        raise RuntimeError("PCSX2 is not running")
    hwnd = w[0]
    # Windows lets a program bring a window forward right after a key press of its own.
    _key(0x12, False)
    _key(0x12, True)
    user32.ShowWindow(hwnd, 9)
    already = user32.GetForegroundWindow() == hwnd
    user32.SetForegroundWindow(hwnd)
    time.sleep(0.1 if already else 0.8)   # PCSX2 drops the first key just after it gets the focus
    return hwnd


def press(button, hold=0.08, after=0.25):
    vk = BUTTONS[button]
    _key(vk, False)
    time.sleep(hold)
    _key(vk, True)
    time.sleep(after)


def hold(button, seconds):
    press(button, hold=seconds, after=0.1)


def keys(*vks):
    """A key chord (virtual keys), e.g. Alt+Return for full screen."""
    for vk in vks:
        _key(vk, False)
    time.sleep(0.05)
    for vk in reversed(vks):
        _key(vk, True)


def rect():
    """The game window's client area on the screen: (left, top, right, bottom)."""
    hwnd = game_window()[0]
    r = wt.RECT()
    user32.GetClientRect(hwnd, ctypes.byref(r))
    p = wt.POINT(0, 0)
    user32.ClientToScreen(hwnd, ctypes.byref(p))
    return p.x, p.y, p.x + r.right, p.y + r.bottom


def shot(path):
    """A picture of the game window (it must be in front)."""
    from PIL import ImageGrab
    img = ImageGrab.grab(bbox=rect(), all_screens=True)
    img.save(path)
    return path
