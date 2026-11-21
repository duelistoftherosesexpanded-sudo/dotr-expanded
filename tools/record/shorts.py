"""Vertical YouTube Shorts (1080 x 1920) from the recorded clips (tools/record/do.py).

  py tools/record/shorts.py NAME [--music TRACK]

Each short is a list of pieces: a piece of a clip, a crop of its 16:9 picture (x, y, w, h in the
1920 x 1080 frame) scaled to fill the width, a playback speed and a caption at the top. The music
plays under it with the game's sound low beneath.
"""
import argparse
import subprocess
import tempfile
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

CLIPS = Path.home() / "Videos" / "DotR Expanded trailer"
OUT = CLIPS / "shorts"
W, H, FPS = 1080, 1920, 60
GOLD = (240, 200, 90)
FONT_BIG = "C:/Windows/Fonts/impact.ttf"
FONT_TEXT = "C:/Windows/Fonts/bahnschrift.ttf"
DISCORD = "discord.gg/ha6ANq8yDN"

# (clip, start, end, crop (x, y, w, h), speed, caption lines)
SHORTS = {
    "01_new_cards": [
        ("09_chest_scroll", 0.0, 9.5, (380, 40, 1160, 1000), 2.0, ["576 NEW CARDS", "in Duelists of the Roses"]),
        ("09_chest_scroll", 9.5, 20.5, (380, 40, 1160, 1000), 2.0, ["FROM GX", "TO TODAY"]),
        ("09_chest_scroll", 38.0, 44.0, (380, 40, 1160, 1000), 2.0, ["PICK YOUR ERA", "OF NOSTALGIA"]),
        ("08_models", 4.0, 9.0, (520, 0, 880, 760), 1.0, ["WITH 3D MODELS", "Black Rose Dragon"]),
        ("08_models", 128.5, 133.0, (600, 0, 780, 700), 1.0, ["Red Dragon Archfiend"]),
        ("08_models", 203.0, 210.0, (520, 0, 880, 760), 1.0, ["Stardust Dragon"]),
        ("10_models2", 26.0, 33.0, (720, 0, 600, 620), 1.0, ["Constellar Algiedi"]),
        ("10_models2", 165.0, 172.0, (720, 60, 600, 560), 1.0, ["Dragonpit Magician"]),
        ("04_deck_master", 144.5, 151.0, (600, 300, 720, 780), 1.0, ["ON THE FIELD", "Gagaga Clerk"]),
    ],
    "02_og_cards": [
        ("11_og_models", 28.0, 34.5, (380, 40, 1160, 1000), 1.0, ["NEW OG-ERA CARDS", "in Duelists of the Roses"]),
        ("11_og_models", 46.0, 53.0, (520, 0, 880, 760), 1.0, ["Valkyrion the Magna Warrior"]),
        ("11_og_models", 14.0, 19.5, (640, 0, 640, 640), 1.0, ["Buster Blader"]),
        ("11_og_models", 79.0, 85.0, (700, 60, 520, 520), 1.0, ["Thousand-Eyes Restrict"]),
        ("11_og_models", 112.0, 118.0, (560, 0, 800, 760), 1.0, ["Dark Paladin"]),
        ("11_og_models", 176.0, 182.0, (700, 60, 520, 520), 1.0, ["Dark Necrofear"]),
        ("11_og_models", 208.0, 215.0, (520, 0, 880, 760), 1.0, ["XYZ-Dragon Cannon"]),
        ("11_og_models", 242.0, 249.0, (520, 0, 880, 760), 1.0, ["Tyrant Dragon"]),
    ],
}


def font(path, size):
    return ImageFont.truetype(path, size)


def overlay_png(path, lines, footer=True):
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    y = 150
    for i, text in enumerate(lines):
        f = font(FONT_BIG, 120 if i == 0 else 72)
        colour = GOLD if i == 0 else (255, 255, 255)
        w = d.textlength(text, font=f)
        while w > W - 80:
            f = font(FONT_BIG, f.size - 6)
            w = d.textlength(text, font=f)
        x = (W - w) / 2
        for dx in range(-6, 7, 3):
            for dy in range(-6, 7, 3):
                d.text((x + dx, y + dy), text, font=f, fill=(0, 0, 0, 255))
        d.text((x, y), text, font=f, fill=colour + (255,))
        y += f.size + 24
    if footer:
        for i, (text, size, colour) in enumerate((("DUELISTS OF THE ROSES EXPANDED", 58, GOLD),
                                                  ("Join the Discord: " + DISCORD, 44, (255, 255, 255)))):
            f = font(FONT_BIG if i == 0 else FONT_TEXT, size)
            w = d.textlength(text, font=f)
            yy = H - 300 + i * 80
            d.text(((W - w) / 2 + 3, yy + 3), text, font=f, fill=(0, 0, 0, 255))
            d.text(((W - w) / 2, yy), text, font=f, fill=colour + (255,))
    img.save(path)


def run(args):
    subprocess.run(["ffmpeg", "-v", "error", "-y"] + args, check=True)


def piece(work, k, clip, start, end, crop, speed, lines):
    out = work / f"p{k:02d}.mp4"
    x, y, w, h = crop
    dur = (end - start) / speed
    png = work / f"o{k:02d}.png"
    overlay_png(png, lines)
    sh = round(h * W / w / 2) * 2
    top = max((H - sh) // 2, 430)                     # below the caption
    filters = (f"[0:v]setpts=PTS/{speed},crop={w}:{h}:{x}:{y},scale={W}:{sh}:flags=lanczos,fps={FPS}[g];"
               f"[0:v]setpts=PTS/{speed},scale={W}:{H}:force_original_aspect_ratio=increase,crop={W}:{H},"
               f"boxblur=30:3,eq=brightness=-0.25,fps={FPS}[bg];"
               f"[bg][g]overlay=0:{top}[v0];[1:v]format=rgba[o];[v0][o]overlay=0:0,"
               f"fade=in:st=0:d=0.15,fade=out:st={dur - 0.15:.3f}:d=0.15,format=yuv420p[v];"
               f"[0:a]atempo={speed},aresample=48000[a]")
    run(["-ss", f"{start}", "-t", f"{end - start}", "-i", str(CLIPS / f"{clip}.mp4"),
         "-loop", "1", "-t", f"{dur:.3f}", "-i", str(png), "-filter_complex", filters,
         "-map", "[v]", "-map", "[a]", "-t", f"{dur:.3f}", "-r", str(FPS),
         "-c:v", "libx264", "-preset", "medium", "-crf", "18", "-c:a", "aac", "-b:a", "192k", "-ac", "2", str(out)])
    return out


def main():
    p = argparse.ArgumentParser()
    p.add_argument("name")
    p.add_argument("--music", default=str(CLIPS / "dotr_intro_music.flac"))
    p.add_argument("--game-volume", type=float, default=0.15)
    a = p.parse_args()
    OUT.mkdir(exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="short_"))
    parts = [piece(work, k, *s) for k, s in enumerate(SHORTS[a.name])]
    (work / "list.txt").write_text("".join(f"file '{q.as_posix()}'\n" for q in parts))
    joined = work / "joined.mp4"
    run(["-f", "concat", "-safe", "0", "-i", str(work / "list.txt"), "-c", "copy", str(joined)])
    dur = float(subprocess.run(["ffprobe", "-v", "error", "-show_entries", "format=duration", "-of", "csv=p=0",
                                str(joined)], capture_output=True, text=True).stdout)
    out = OUT / f"{a.name}.mp4"
    run(["-i", str(joined), "-stream_loop", "-1", "-i", a.music, "-filter_complex",
         f"[1:a]atrim=0:{dur:.2f},afade=out:st={dur - 1.5:.2f}:d=1.5,volume=0.9[m];[0:a]volume={a.game_volume}[g];"
         f"[m][g]amix=inputs=2:duration=first:normalize=0[a]",
         "-map", "0:v", "-map", "[a]", "-c:v", "copy", "-c:a", "aac", "-b:a", "192k", str(out)])
    print(out, f"{dur:.1f}s")


if __name__ == "__main__":
    main()
