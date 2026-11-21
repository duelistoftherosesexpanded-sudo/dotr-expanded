"""Cuts the announcement trailer from the recorded clips (tools/record/do.py) with ffmpeg.

  py tools/record/trailer.py [--music TRACK] [--out FILE]

Each scene is a piece of a clip with a caption; scenes are joined with short dips to black. The
game's own sound plays under it, or, with --music, the track (faded out at the end) with the game
sound low beneath it. Title and end cards are drawn here (Pillow).
"""
import argparse
import subprocess
import tempfile
from pathlib import Path

from PIL import Image, ImageDraw, ImageFilter, ImageFont

CLIPS = Path.home() / "Videos" / "DotR Expanded trailer"
W, H, FPS = 1920, 1080, 60
GOLD = (240, 200, 90)
FONT_BIG = "C:/Windows/Fonts/impact.ttf"
FONT_TEXT = "C:/Windows/Fonts/bahnschrift.ttf"
DISCORD = "discord.gg/ha6ANq8yDN"

# (clip, start, end, caption, subcaption, extras)
# The opening: new cards' models, before the title card.
INTRO = [
    ("08_models", 5.5, 8.6, None, None, {"zoom": (400, 0, 1120, 630)}),
    ("08_models", 129.0, 132.0, None, None, {"zoom": (480, 60, 960, 540)}),
    ("08_models", 208.0, 211.0, None, None, {"zoom": (400, 0, 1120, 630)}),
    ("08_models", 271.0, 274.5, None, None, {"zoom": (240, 0, 1440, 810)}),
]
SCENES = [
    ("03_card_shop", 0.0, 2.6, "NATIVE WIDESCREEN", "The whole game in true 16:9", {}),
    ("01_mod_options", 19.5, 28.2, "PICK YOUR ERA OF NOSTALGIA",
     "OG  â€¢  GX  â€¢  5D'S  â€¢  ZEXAL  â€¢  ARC-V  â€¢  VRAINS  â€¢  MODERN", {}),
    ("03_card_shop", 2.6, 6.0, "A CARD SHOP ON THE MAP", "25 cards by rarity, restocked after every duel", {}),
    ("03_card_shop", 9.2, 11.8, None, None, {}),
    ("03_card_shop", 19.0, 24.6, None, None, {}),
    ("03_card_shop", 24.6, 30.5, None, None, {}),
    ("06b_achievement_popups", 0.0, 6.0, "ACHIEVEMENTS", "Earn cards and sleeves as you play", {}),
    ("07_trophies", 2.5, 15.0, None, None, {}),
    ("03_card_shop", 54.5, 60.5, "NEW CARD SLEEVES", "Choose the back of your cards", {"inset": (893, 942, 72, 104)}),
    ("04_deck_master", 1.0, 6.5, "DECK MASTERS", "New opponents on the campaign map", {}),
    ("04_deck_master", 14.0, 21.6, "FIELDS BUILT FROM THE DECK", "A new board for every boss duel", {}),
    ("04_deck_master", 50.5, 54.2, "576 NEW CARDS", "From GX to today, with 3D models", {}),
    ("04_deck_master", 108.0, 112.0, None, None, {"zoom": (480, 240, 960, 540)}),
    ("04_deck_master", 144.5, 152.5, None, None, {"zoom": (480, 200, 960, 540)}),
    ("08_models", 437.0, 447.0, "PICTURE FRAME MODELS", "Add any card from Yu-Gi-Oh! history",
     {"zoom": (400, 0, 1280, 720)}),
    ("05_second_world", 0.5, 13.6, "A SECOND CAMPAIGN", "Set sail for a whole new map", {}),
    ("05_second_world", 162.0, 167.5, None, None, {}),
]


def font(path, size):
    return ImageFont.truetype(path, size)


def caption_png(path, title, sub):
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    band = Image.new("RGBA", (W, 230), (0, 0, 0, 0))
    bd = ImageDraw.Draw(band)
    for y in range(230):                       # a dark band fading upwards
        bd.line([(0, y), (W, y)], fill=(0, 0, 0, int(190 * y / 230)))
    img.alpha_composite(band, (0, H - 230))
    d = ImageDraw.Draw(img)
    big, small = font(FONT_BIG, 92), font(FONT_TEXT, 40)
    x, y = 90, H - 205
    d.text((x + 4, y + 4), title, font=big, fill=(0, 0, 0, 200))
    d.text((x, y), title, font=big, fill=GOLD + (255,))
    if sub:
        d.text((x + 2, y + 112), sub, font=small, fill=(255, 255, 255, 235))
    img.save(path)


def card_png(path, background, lines):
    """A full-screen card: a darkened, blurred frame of the game with centred lines of text."""
    if background:
        bg = Image.open(background).convert("RGB").resize((W, H)).filter(ImageFilter.GaussianBlur(6))
        bg = Image.blend(bg, Image.new("RGB", (W, H)), 0.62)
    else:
        bg = Image.new("RGB", (W, H), (8, 6, 12))
    d = ImageDraw.Draw(bg)
    total = sum(f.size + gap for _, f, _, gap in lines)
    y = (H - total) // 2
    for text, f, colour, gap in lines:
        w = d.textlength(text, font=f)
        d.text(((W - w) / 2 + 3, y + 3), text, font=f, fill=(0, 0, 0))
        d.text(((W - w) / 2, y), text, font=f, fill=colour)
        y += f.size + gap
    bg.save(path)


def run(args):
    subprocess.run(["ffmpeg", "-v", "error", "-y"] + args, check=True)


def encode(inputs, filters, out, duration):
    run(inputs + ["-filter_complex", filters, "-map", "[v]", "-map", "[a]", "-t", f"{duration:.3f}",
                  "-r", str(FPS), "-c:v", "libx264", "-preset", "medium", "-crf", "17", "-pix_fmt", "yuv420p",
                  "-c:a", "aac", "-b:a", "192k", "-ar", "48000", "-ac", "2", str(out)])


def scene(work, k, clip, start, end, title, sub, extras):
    out = work / f"s{k:02d}.mp4"
    dur = end - start
    inputs = ["-ss", f"{start:.3f}", "-t", f"{dur:.3f}", "-i", str(CLIPS / f"{clip}.mp4")]
    v = f"[0:v]scale={W}:{H},fps={FPS},setsar=1"
    if "zoom" in extras:                       # a closer view: this part of the picture, full size
        x, y, w, h = extras["zoom"]
        v += f",crop={w}:{h}:{x}:{y},scale={W}:{H}:flags=lanczos"
    chain = []
    if "inset" in extras:                      # a zoomed copy of a small part, top right
        x, y, w, h = extras["inset"]
        chain.append(f"{v},split[base][z];[z]crop={w}:{h}:{x}:{y},scale={w * 4}:{h * 4}:flags=lanczos,"
                     f"pad=iw+12:ih+12:6:6:color=0xF0C85A[zz];[base][zz]overlay=W-w-80:80[v0]")
    else:
        chain.append(f"{v}[v0]")
    last = "v0"
    if title:
        png = work / f"c{k:02d}.png"
        caption_png(png, title, sub)
        inputs += ["-loop", "1", "-t", f"{dur:.3f}", "-i", str(png)]
        show = min(dur, 4.0)
        chain.append(f"[1:v]format=rgba,fade=in:st=0.15:d=0.35:alpha=1,fade=out:st={show - 0.4:.2f}:d=0.4:alpha=1[cap];"
                     f"[{last}][cap]overlay=0:0:shortest=1[v1]")
        last = "v1"
    fade = 0.18
    chain.append(f"[{last}]fade=in:st=0:d={fade},fade=out:st={dur - fade:.3f}:d={fade}[v]")
    chain.append(f"[0:a]aresample=48000,afade=in:st=0:d=0.12,afade=out:st={dur - 0.15:.3f}:d=0.15[a]")
    encode(inputs, ";".join(chain), out, dur)
    return out


def still(work, name, png, dur):
    out = work / f"{name}.mp4"
    filters = (f"[0:v]scale={W}:{H},fps={FPS},format=yuv420p,fade=in:st=0:d=0.5,fade=out:st={dur - 0.6:.2f}:d=0.6[v];"
               "[1:a]anull[a]")
    encode(["-loop", "1", "-t", f"{dur}", "-i", str(png),
            "-f", "lavfi", "-t", f"{dur}", "-i", "anullsrc=r=48000:cl=stereo"], filters, out, dur)
    return out


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--music")
    p.add_argument("--game-volume", type=float, default=0.35, help="the game's sound under the music")
    p.add_argument("--out", default=str(CLIPS / "DotR Expanded v0.55 trailer.mp4"))
    a = p.parse_args()
    work = Path(tempfile.mkdtemp(prefix="trailer_"))

    # The title card over the game's own title menu; the end card over the second world.
    frame = work / "title_bg.png"
    run(["-ss", "0.5", "-i", str(CLIPS / "01_mod_options.mp4"), "-frames:v", "1", str(frame)])
    end_bg = work / "end_bg.png"
    run(["-ss", "166.5", "-i", str(CLIPS / "05_second_world.mp4"), "-frames:v", "1", str(end_bg)])
    title = work / "title.png"
    card_png(title, frame, [
        ("YU-GI-OH! THE DUELISTS OF THE ROSES", font(FONT_TEXT, 44), (255, 255, 255), 10),
        ("EXPANDED", font(FONT_BIG, 190), GOLD, 20),
        ("v0.55 BETA", font(FONT_TEXT, 48), (255, 255, 255), 0)])
    end = work / "end.png"
    card_png(end, end_bg, [
        ("DUELISTS OF THE ROSES EXPANDED", font(FONT_BIG, 110), GOLD, 26),
        ("v0.55 beta  â€¢  a proof of concept: new cards aren't balanced yet", font(FONT_TEXT, 38), (255, 255, 255), 50),
        ("JOIN THE DISCORD", font(FONT_TEXT, 46), (255, 255, 255), 8),
        (DISCORD, font(FONT_BIG, 96), GOLD, 50),
        ("Download, install guide and FAQ inside.  You need your own copy of the game.", font(FONT_TEXT, 32),
         (220, 220, 220), 10),
        ("Fan project, not affiliated with Konami.", font(FONT_TEXT, 28), (170, 170, 170), 0)])

    roadmap = work / "roadmap.png"
    card_png(roadmap, end_bg, [
        ("THE ROAD AHEAD", font(FONT_BIG, 120), GOLD, 40),
        ("Battle City arc: the second story", font(FONT_TEXT, 56), (255, 255, 255), 22),
        ("A roguelike endless tower", font(FONT_TEXT, 56), (255, 255, 255), 22),
        ("Card packs", font(FONT_TEXT, 56), (255, 255, 255), 22),
        ("Custom card effects", font(FONT_TEXT, 56), (255, 255, 255), 40),
        ("...and plenty more to come", font(FONT_TEXT, 44), GOLD, 0)])

    parts = []
    for k, s in enumerate(INTRO):
        parts.append(scene(work, 100 + k, *s))
    parts.append(still(work, "title", title, 4.0))
    for k, s in enumerate(SCENES):
        parts.append(scene(work, k, *s))
        print("scene", k, s[0], s[1], s[2])
    parts.append(still(work, "roadmap", roadmap, 6.5))
    parts.append(still(work, "end", end, 7.0))

    joined = work / "joined.mp4"
    (work / "list.txt").write_text("".join(f"file '{p.as_posix()}'\n" for p in parts))
    run(["-f", "concat", "-safe", "0", "-i", str(work / "list.txt"), "-c", "copy", str(joined)])

    if a.music:
        dur = float(subprocess.run(["ffprobe", "-v", "error", "-show_entries", "format=duration", "-of", "csv=p=0",
                                    str(joined)], capture_output=True, text=True).stdout)
        run(["-i", str(joined), "-stream_loop", "-1", "-i", a.music, "-filter_complex",
             f"[1:a]atrim=0:{dur:.2f},afade=out:st={dur - 3:.2f}:d=3,volume=0.9[m];[0:a]volume={a.game_volume}[g];"
             f"[m][g]amix=inputs=2:duration=first:normalize=0[a]",
             "-map", "0:v", "-map", "[a]", "-c:v", "copy", "-c:a", "aac", "-b:a", "192k", a.out])
    else:
        run(["-i", str(joined), "-c", "copy", a.out])
    print("trailer:", a.out)


if __name__ == "__main__":
    main()
