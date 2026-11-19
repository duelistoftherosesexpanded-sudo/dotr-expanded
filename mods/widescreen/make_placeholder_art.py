#!/usr/bin/env python3
"""Draws mods/widescreen/placeholder.png, the picture of the placeholder cards: a gold question
mark on a dark blue gradient inside a gold border, 128 x 128 like the game's card pictures."""

import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import model_texture as mt  # noqa: E402

SIZE = 128
GOLD, GOLD_DARK = (232, 190, 72), (150, 108, 30)


def inside_mark(x, y):
    """The question mark: the upper part of a ring, a stem below it and a dot."""
    cx, cy = 64, 44
    dx, dy = x - cx, y - cy
    r = math.hypot(dx, dy)
    if 17 <= r <= 30 and not (dx < 0 and dy > 0):
        return True
    if 57 <= x <= 71 and 60 <= y <= 88:
        return True
    return math.hypot(x - 64, y - 104) <= 9


def pixel(x, y):
    if x < 4 or y < 4 or x >= SIZE - 4 or y >= SIZE - 4:
        return GOLD if (x + y) % 2 else GOLD_DARK
    if inside_mark(x, y):
        return GOLD
    # A shadow one pixel down and right of the mark, then the gradient.
    if inside_mark(x - 2, y - 2):
        return (20, 14, 6)
    t = y / SIZE
    return (int(18 + 20 * t), int(30 + 40 * t), int(80 + 70 * t))


def main():
    rows = [[pixel(x, y) for x in range(SIZE)] for y in range(SIZE)]
    out = Path(__file__).resolve().parent / "placeholder.png"
    mt.write_png(out, SIZE, SIZE, rows)
    print(out)


if __name__ == "__main__":
    main()
