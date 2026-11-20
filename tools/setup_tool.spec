# PyInstaller recipe for "DotR Expanded Setup.exe": tools/setup_tool.py with Python, its libraries
# (UnityPy, Pillow) and the release folder (tools/release.py) inside one file.
#
#   py -3.12 -m PyInstaller tools/setup_tool.spec --noconfirm      (from the project folder)
#
# The exe lands in dist/. Make the release first; the exe carries the release it was built with.
import os
from PyInstaller.utils.hooks import collect_all

PROJECT = os.path.dirname(SPECPATH)
datas, binaries, hiddenimports = [(os.path.join(PROJECT, "release"), "release"),
                                  (os.path.join(SPECPATH, "dev_kit"), "dev_kit")], [], []
for package in ("UnityPy", "PIL", "fsb5", "texture2ddecoder", "etcpak", "astc_encoder", "archspec", "brotli",
                "fmod_toolkit", "pyfmodex"):
    try:
        d, b, h = collect_all(package)
        datas += d
        binaries += b
        hiddenimports += h
    except Exception:
        pass
# The project's tools the setup tool reaches by name at run time.
hiddenimports += ["dod_export", "igb_read", "hsd_anim", "crossduel_export", "dlc_cards", "dlc_title", "dlc_world",
                  "dlc_community", "model_skinned", "model_import", "model_texture", "model_research", "glb_anim",
                  "fbx_anim", "fbx_read", "card_edit", "iso_files", "research", "runtime", "gui_theme",
                  "bch_read", "bch_export", "ctr_read", "glb_anim", "model_texture", "dod_models", "frame_model",
                  "dlc_regen", "dlc_buttons", "dlc_frames", "dlc_sleeves", "dlc_shop", "project"]

icon = os.path.join(PROJECT, "launcher", "icon.ico")
a = Analysis([os.path.join(SPECPATH, "setup_tool.py")], pathex=[SPECPATH], binaries=binaries, datas=datas,
             hiddenimports=hiddenimports)
pyz = PYZ(a.pure)
exe = EXE(pyz, a.scripts, a.binaries, a.datas, name="DotR Expanded Setup", console=False, upx=False,
          icon=icon if os.path.isfile(icon) else None)
