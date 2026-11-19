#!/usr/bin/env python3
"""The textures of a monster model ("RH2" files after the model in its slot): decode and replace.

Layout, for the 8-bit textures the monsters use (read from the files and from the upload records
at their start; the pixel order follows DotrModdingToolRedux's ImageCreator):

  +0x08  size of the file          +0x50 pixel format (0x13: 8 bits, palette)
  +0x54  width, +0x56 height       +0x58 number of upload packets (palette + image blocks)
  +0x10  offsets: [2] the palette packet, [4] the first image block packet
  a packet is 0x60 bytes of tags and registers, then its data
  palette: 256 colours of 16 bits (5 red, 5 green, 5 blue from the low end, then the alpha bit),
           entries 8-15 and 16-23 of every 32 exchanged
  image:   blocks of min(width, 128) x min(height, 64) pixels, left to right then down, each
           stored in the order the console gives 8-bit pixels uploaded as 32-bit ones
"""

import struct
import zlib


def _place(index):
    kind = (index % 32) // 8
    return index + 8 if kind == 1 else index - 8 if kind == 2 else index


def _positions(width, height):
    """For each byte of each block, the (x, y) it holds, or None; and the block size in bytes."""
    if width < 128:   # small pictures (the 40 x 32 card miniatures) are one block in plain order
        return [[(i % width, i // width) for i in range(width * height)]], width * height
    block_w, block_h = min(width, 128), min(height, 64)
    per_row = block_w // 16
    size = block_w * block_h
    blocks = (width // block_w) * (height // block_h)
    table = []
    for section in range(blocks):
        row = []
        for i in range(size):
            block = i // 32
            horizontal = 16 * (block % per_row)
            block_row = block // per_row
            vertical = 2 * block_row - block_row % 2
            sub = i % 4
            offset = (block // 16) % 2
            within = (4 * (offset + sub) + i // 4) % 8
            x = horizontal + 8 * (sub // 2) + within + block_w * (section % (width // block_w))
            y = vertical + 2 * (sub % 2) + block_h * (section // (width // block_w))
            row.append((x, y) if x < width and y < height else None)
        table.append(row)
    return table, size


def info(data, at):
    if data[at:at + 4] != b"RH2\0":
        raise ValueError("not an RH2 texture")
    size, = struct.unpack_from("<I", data, at + 8)
    table = struct.unpack_from("<8I", data, at + 0x10)
    fmt, _, width, height, packets = struct.unpack_from("<5H", data, at + 0x50)
    if fmt != 0x13:
        raise ValueError(f"unsupported pixel format 0x{fmt:x}")
    return dict(size=size, width=width, height=height, packets=packets,
                palette=at + table[2] + 0x60, image=at + table[4])


def decode(data, at):
    """(width, height, rows of (r, g, b)) of the texture at `at`."""
    t = info(data, at)
    palette = []
    for i in range(256):
        p, = struct.unpack_from("<H", data, t["palette"] + 2 * i)
        palette.append(((p & 31) * 255 // 31, (p >> 5 & 31) * 255 // 31, (p >> 10 & 31) * 255 // 31))
    table, size = _positions(t["width"], t["height"])
    pixels = [[(0, 0, 0)] * t["width"] for _ in range(t["height"])]
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        for i, where in enumerate(row):
            if where:
                pixels[where[1]][where[0]] = palette[_place(data[base + i]) % 256]
    return t["width"], t["height"], pixels


def encode(data, at, pixels):
    """Write rows of (r, g, b) of the texture's own size into a bytearray, with a new palette."""
    t = info(data, at)
    if len(pixels) != t["height"] or len(pixels[0]) != t["width"]:
        raise ValueError("picture size differs from the texture")
    colours, indexed = quantize(pixels, 256)
    for i in range(256):
        r, g, b = colours[i] if i < len(colours) else (0, 0, 0)
        struct.pack_into("<H", data, t["palette"] + 2 * _place(i), 0x8000 | r >> 3 | (g >> 3) << 5 | (b >> 3) << 10)
    table, size = _positions(t["width"], t["height"])
    for section, row in enumerate(table):
        base = t["image"] + section * (size + 0x60) + 0x60
        for i, where in enumerate(row):
            if where:
                data[base + i] = indexed[where[1]][where[0]]


def quantize(pixels, limit):
    """Median cut on 5-bit channels: (palette, rows of palette indices)."""
    counts = {}
    for row in pixels:
        for r, g, b in row:
            key = (r >> 3, g >> 3, b >> 3)
            counts[key] = counts.get(key, 0) + 1
    boxes = [list(counts.items())]
    while len(boxes) < limit:
        best, axis, spread = None, 0, 0
        for index, box in enumerate(boxes):
            if len(box) < 2:
                continue
            for channel in range(3):
                values = [colour[channel] for colour, _ in box]
                width = (max(values) - min(values)) * sum(n for _, n in box) ** 0.5
                if width > spread:
                    best, axis, spread = index, channel, width
        if best is None:
            break
        box = sorted(boxes.pop(best), key=lambda item: item[0][axis])
        half, running, cut = sum(n for _, n in box) / 2, 0, 1
        for position, (_, n) in enumerate(box):
            running += n
            if running >= half:
                cut = min(max(position, 1), len(box) - 1)
                break
        boxes += [box[:cut], box[cut:]]
    palette, lookup = [], {}
    for index, box in enumerate(boxes):
        total = sum(n for _, n in box)
        palette.append(tuple(min(255, int(sum(colour[c] * n for colour, n in box) / total * 8 + 4)) for c in range(3)))
        for colour, _ in box:
            lookup[colour] = index
    return palette, [[lookup[(r >> 3, g >> 3, b >> 3)] for r, g, b in row] for row in pixels]


def write_png(path, width, height, pixels):
    raw = b"".join(b"\0" + bytes(v for pixel in row for v in pixel) for row in pixels)

    def chunk(kind, body):
        return struct.pack(">I", len(body)) + kind + body + struct.pack(">I", zlib.crc32(kind + body))

    with open(path, "wb") as stream:
        stream.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0))
                     + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def read_png(path):
    """(width, height, rows of (r, g, b)) of an 8-bit RGB or RGBA PNG without interlace."""
    with open(path, "rb") as stream:
        blob = stream.read()
    if blob[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("not a PNG file")
    at, body, header = 8, b"", None
    while at < len(blob):
        length, kind = struct.unpack_from(">I4s", blob, at)
        if kind == b"IHDR":
            header = struct.unpack_from(">IIBBBBB", blob, at + 8)
        elif kind == b"IDAT":
            body += blob[at + 8:at + 8 + length]
        at += 12 + length
    width, height, depth, colour, _, _, interlace = header
    if depth != 8 or colour not in (2, 6) or interlace:
        raise ValueError("only 8-bit RGB or RGBA PNG files without interlace are supported")
    n = 4 if colour == 6 else 3
    raw, stride = zlib.decompress(body), width * n
    rows, previous = [], bytearray(stride)
    for y in range(height):
        kind = raw[y * (stride + 1)]
        line = bytearray(raw[y * (stride + 1) + 1:(y + 1) * (stride + 1)])
        for i in range(stride):
            left = line[i - n] if i >= n else 0
            up, corner = previous[i], previous[i - n] if i >= n else 0
            if kind == 1:
                line[i] = (line[i] + left) & 255
            elif kind == 2:
                line[i] = (line[i] + up) & 255
            elif kind == 3:
                line[i] = (line[i] + (left + up) // 2) & 255
            elif kind == 4:
                p = left + up - corner
                pa, pb, pc = abs(p - left), abs(p - up), abs(p - corner)
                line[i] = (line[i] + (left if pa <= pb and pa <= pc else up if pb <= pc else corner)) & 255
        rows.append([tuple(line[x * n:x * n + 3]) for x in range(width)])
        previous = line
    return width, height, rows


def resize(width, height, rows, new_width, new_height):
    """Area average down (or nearest up) to the new size."""
    result = []
    for y in range(new_height):
        y0, y1 = y * height // new_height, max(y * height // new_height + 1, (y + 1) * height // new_height)
        line = []
        for x in range(new_width):
            x0, x1 = x * width // new_width, max(x * width // new_width + 1, (x + 1) * width // new_width)
            total, n = [0, 0, 0], (y1 - y0) * (x1 - x0)
            for yy in range(y0, y1):
                for xx in range(x0, x1):
                    pixel = rows[yy][xx]
                    total[0] += pixel[0]
                    total[1] += pixel[1]
                    total[2] += pixel[2]
            line.append((total[0] // n, total[1] // n, total[2] // n))
        result.append(line)
    return result
