"""Light or dark look for the project's tkinter windows, following Windows' "app mode" setting
(Settings > Personalization > Colors). Call apply(root) before building a window's widgets, and
title_bar(window) for each Toplevel; use HINT / STATUS / TEXT_BG ... for colours set by hand."""

import sys
import tkinter as tk
from tkinter import ttk


def dark_mode():
    if sys.platform != "win32":
        return False
    try:
        import winreg
        key = winreg.OpenKey(winreg.HKEY_CURRENT_USER,
                             r"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize")
        value, _ = winreg.QueryValueEx(key, "AppsUseLightTheme")
        return value == 0
    except OSError:
        return False


DARK = dark_mode()
BG = "#202020" if DARK else "#f0f0f0"
FIELD = "#2b2b2b" if DARK else "#ffffff"
FG = "#e8e8e8" if DARK else "#000000"
HINT = "#a0a0a0" if DARK else "#666666"
STATUS = "#6cb6ff" if DARK else "#0066aa"
BORDER = "#3c3c3c" if DARK else "#c0c0c0"
SELECT = "#2f5f8f" if DARK else "#cce4f7"
TEXT_BG = "#1a1a1a" if DARK else "#ffffff"


def title_bar(window):
    """Dark title bar on Windows 10/11 (DWMWA_USE_IMMERSIVE_DARK_MODE)."""
    if not DARK or sys.platform != "win32":
        return
    try:
        import ctypes
        window.update_idletasks()
        hwnd = ctypes.windll.user32.GetParent(window.winfo_id())
        on = ctypes.c_int(1)
        for attribute in (20, 19):
            if ctypes.windll.dwmapi.DwmSetWindowAttribute(hwnd, attribute, ctypes.byref(on), 4) == 0:
                break
    except (OSError, AttributeError):
        pass


def apply(root):
    if not DARK:
        return
    style = ttk.Style(root)
    style.theme_use("clam")
    root.configure(bg=BG)
    style.configure(".", background=BG, foreground=FG, fieldbackground=FIELD, bordercolor=BORDER,
                    darkcolor=BG, lightcolor=BG, troughcolor=FIELD, selectbackground=SELECT,
                    selectforeground=FG, insertcolor=FG, arrowcolor=FG)
    for widget in ("TFrame", "TLabel", "TCheckbutton", "TRadiobutton", "TLabelframe", "TLabelframe.Label"):
        style.configure(widget, background=BG, foreground=FG)
    style.configure("TButton", background="#333333", foreground=FG, bordercolor=BORDER, padding=(8, 3))
    style.map("TButton", background=[("active", "#3d3d3d"), ("pressed", "#2a2a2a")])
    for widget in ("TEntry", "TCombobox", "TSpinbox"):
        style.configure(widget, fieldbackground=FIELD, foreground=FG, background="#333333", arrowcolor=FG)
        style.map(widget, fieldbackground=[("readonly", FIELD), ("disabled", BG)],
                  foreground=[("disabled", HINT)], selectbackground=[("readonly", SELECT)])
    style.configure("TNotebook", background=BG, bordercolor=BORDER)
    style.configure("TNotebook.Tab", background="#2b2b2b", foreground=FG, padding=(10, 4))
    style.map("TNotebook.Tab", background=[("selected", "#3a3a3a")], foreground=[("disabled", HINT)])
    style.map("TCheckbutton", background=[("active", BG)], indicatorcolor=[("selected", STATUS), ("!selected", FIELD)])
    # Plain tk widgets and the comboboxes' drop-down lists.
    for pattern, value in (("*Listbox.background", FIELD), ("*Listbox.foreground", FG),
                           ("*Listbox.selectBackground", SELECT), ("*Listbox.selectForeground", FG),
                           ("*Text.background", TEXT_BG), ("*Text.foreground", FG),
                           ("*Text.insertBackground", FG), ("*Label.background", BG), ("*Label.foreground", FG),
                           ("*TCombobox*Listbox.background", FIELD), ("*TCombobox*Listbox.foreground", FG),
                           ("*TCombobox*Listbox.selectBackground", SELECT)):
        root.option_add(pattern, value)
    title_bar(root)


def wide_dropdown(box, limit=1400):
    """Lets a combobox's list be as wide as its longest entry (up to `limit` pixels), so long
    entries are not cut off. ttk sizes the list from the box's width plus the style's postoffset,
    so each box gets a style of its own whose postoffset is set when the list opens."""
    import tkinter.font as tkfont
    name = f"W{id(box)}.TCombobox"
    box.configure(style=name)

    def post():
        values = box.cget("values") or ()
        font = tkfont.nametofont("TkDefaultFont")
        need = max([font.measure(str(v)) for v in values] + [0]) + 30
        extra = max(0, min(need, limit, box.winfo_screenwidth() - box.winfo_rootx() - 20) - box.winfo_width())
        ttk.Style(box).configure(name, postoffset=(0, 0, extra, 0))
    box.configure(postcommand=post)
