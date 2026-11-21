"""One step of a recording session, from the command line:

  py tools/record/do.py [rec-start NAME] BUTTON[*N][:WAIT] ... [shot NAME] [rec-stop]

  BUTTON     up down left right cross circle square triangle start select l1 r1 l2 r2
  *N         press it N times;  :WAIT  seconds to wait after it (default 0.35)
  wait:S     wait S seconds;    hold:BUTTON:S   hold a button for S seconds
  shot NAME  picture of the game to <scratch>/NAME.png (scratch: env RECORD_SHOTS or the cwd)
  rec-start NAME / rec-stop   OBS recording (the file is renamed to NAME.mp4 when it stops)
"""
import os
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import obs  # noqa: E402
import pcsx2  # noqa: E402

SHOTS = Path(os.environ.get("RECORD_SHOTS", "."))
STATE = Path(__file__).resolve().parent / ".recording"


def main(args):
    pcsx2.focus()
    i = 0
    while i < len(args):
        a = args[i]
        if a == "shot":
            path = SHOTS / (args[i + 1] + ".png")
            pcsx2.shot(path)
            from PIL import Image
            img = Image.open(path)
            w, h = img.size
            cw = h * 16 // 9                     # the game's 16:9 picture in the middle
            img.crop(((w - cw) // 2, 0, (w + cw) // 2, h)).resize((960, 540)).save(path)
            print("shot", path)
            i += 2
            continue
        if a == "rec-start":
            with obs.OBS() as o:
                o.call("StartRecord")
                time.sleep(0.5)
                if not o.call("GetRecordStatus")["outputActive"]:
                    raise SystemExit("OBS did not start recording (is its window open, not in the tray?)")
            STATE.write_text(args[i + 1])
            pcsx2.focus()
            print("recording", args[i + 1])
            i += 2
            continue
        if a == "rec-stop":
            with obs.OBS() as o:
                path = Path(o.call("StopRecord")["outputPath"])
            time.sleep(1.5)
            name = STATE.read_text().strip() if STATE.is_file() else path.stem
            target = path.with_name(name + path.suffix)
            if target.exists():
                target.unlink()
            path.rename(target)
            STATE.unlink(missing_ok=True)
            print("saved", target)
            i += 1
            continue
        if a.startswith("wait:"):
            time.sleep(float(a[5:]))
        elif a.startswith("hold:"):
            _, button, seconds = a.split(":")
            pcsx2.hold(button, float(seconds))
        else:
            wait = 0.35
            if ":" in a:
                a, w = a.split(":")
                wait = float(w)
            n = 1
            if "*" in a:
                a, n = a.split("*")
                n = int(n)
            for _ in range(n):
                pcsx2.press(a, after=wait)
        i += 1


if __name__ == "__main__":
    main(sys.argv[1:])
