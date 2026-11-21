#!/usr/bin/env python3
"""Saves the game's background music (SOUND/BGM/*/*.PCM) from your own Duelists of the Roses disc
as FLAC files.

Each .PCM file is raw 16-bit little-endian stereo, left and right interleaved, at 24 kHz (the sound
driver writes each sample twice for the IOP's 48 kHz, docs/modules/other.md _iDecodeLPCM).

  py tools/dotr_music.py OUT_FOLDER [--iso DISC.iso]
"""
import argparse
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import iso_files  # noqa: E402
import project  # noqa: E402

RATE = 24000


def main():
    p = argparse.ArgumentParser()
    p.add_argument("out")
    p.add_argument("--iso")
    a = p.parse_args()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    disc = iso_files.Image(project.find_iso(a.iso), read_only=True)
    try:
        tracks = [(path, sector, size) for path, sector, size in disc.files()
                  if path.upper().startswith("/SOUND/BGM/") and path.upper().split(";")[0].endswith(".PCM")]
        for path, sector, size in sorted(tracks):
            name = path.split("/")[-1].split(";")[0][:-4]
            data = disc.read(sector, (size + 2047) // 2048)[:size // 4 * 4]
            target = out / f"{name}.flac"
            subprocess.run(["ffmpeg", "-v", "error", "-y", "-f", "s16le", "-ar", str(RATE), "-ac", "2", "-i", "-",
                            "-c:a", "flac", str(target)], input=data, check=True)
            print(f"{name}  {len(data) / 4 / RATE:6.1f} s")
    finally:
        disc.close()


if __name__ == "__main__":
    main()
