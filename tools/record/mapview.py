"""The campaign map's view, through PCSX2's PINE link (the Launcher's port 28012).

  py tools/record/mapview.py X Z      centres the view on map point (X, Z) (bosses.json's x and z)
                                      and puts the hand in the middle of the screen
  py tools/record/mapview.py          prints where the view is

The map's state is at 0x1ed0000 (bosses.c): the hand on the 640 x 448 screen (two shorts), the
camera's position (float x at +0x30 and +0x40, z at +0x38) and the point it looks at (z at +0x48,
96 less than the camera's).
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import pine  # noqa: E402

BASE = 0x1ED0000


def f2i(v):
    return struct.unpack("<I", struct.pack("<f", v))[0]


def goto(p, x, z, hand=(320, 224)):
    for o, v in ((0x30, x), (0x40, x), (0x38, z + 96), (0x48, z)):
        p.write32(BASE + o, f2i(v))
    p.write32(BASE, (hand[1] << 16) | hand[0])


def where(p):
    d = p.read(BASE, 0x50)
    return struct.unpack_from("<hh", d, 0), struct.unpack_from("<f", d, 0x30)[0], struct.unpack_from("<f", d, 0x48)[0]


if __name__ == "__main__":
    with pine.Pine(28012) as p:
        if len(sys.argv) > 2:
            goto(p, float(sys.argv[1]), float(sys.argv[2]))
        print(where(p))
