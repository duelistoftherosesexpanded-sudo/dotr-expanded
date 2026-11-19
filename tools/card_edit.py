#!/usr/bin/env python3
"""Change one card in a COPY of the disc image: attack, defence, name and picture (experimental).

  card record   0x2bf080 + 20 * card: attack and defence are the low 13 bits of the halfwords at
                +8 and +10 (layout from DotrModdingToolRedux)
  name          string 320 + card of the message table (offsets 0x2d19d0, 3,073 ints counting
                16-bit words into the blob after them). A word is a character (bits 0-12, bit 13
                on half-width ones), or with bit 14 the first of a pair that copies from another
                string: length in bits 0-5, start in bits 6-12, the string's number in the second
                word. Bit 15 ends the string. The blob has no free space and other strings copy
                from the names, so a new name is written elsewhere and the name's offset pointed
                at it: into the 24 bytes at 0x2d0814 left of the developers' screenshot file name
                patterns (the widescreen mod keeps its state in the bytes before them). That is
                room for 12 words and so for one renamed card, with long parts of the name copied
                from strings that have them.
  picture       DATA/PICTURE.MRG, 0x4800 bytes a card, 128 x 128; DATA/PICMINI.MRG, 0x1000 bytes,
                40 x 32; and DATA/PICPACK.MRG (0x4410 bytes, slots listed at 0x2ce9fc) where the
                card has a slot. All are RH2 textures (tools/model_texture.py).

Reads the clean image for comparison and writes only to the copy named with --into.
"""

import argparse
import json
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dotr.elf import file_offset, inventory  # noqa: E402
import iso_files  # noqa: E402
import model_texture as mt  # noqa: E402
import project  # noqa: E402
from runtime import ROOT  # noqa: E402

RECORDS, TABLE, STRINGS, WORDS, NAMES = 0x2bf080, 0x2d19d0, 3073, 74252, 320
BLOB = TABLE + 4 * STRINGS
SPARE, SPARE_WORDS = 0x2d0814, 12
PICTURE, PICMINI, PICPACK, PICPACK_TABLE = (0x1250800, 0x4800), (0xBE0800, 0x1000), (0xE9B800, 0x4410), 0x2ce9fc


def character(code):
    if 0x56 <= code < 0x70:
        return chr(ord("A") + code - 0x56)
    if 0x73 <= code < 0x8d:
        return chr(ord("a") + code - 0x73)
    if 0x8f <= code < 0x98:
        return chr(ord("1") + code - 0x8f)
    return {0x70: " ", 0x98: "0", 0xa2: "-", 0xa4: ".", 0x9f: "'", 0x99: "!", 0x02: ","}.get(code)


def encode_name(name, offsets, blob, own):
    """Words for `name`: characters, and pairs copying runs of three or more from other strings."""
    literal, texts = {}, []
    for index in range(STRINGS):
        at, text = offsets[index], ""
        while 0 <= at < WORDS and len(text) < 190:
            word = blob[at]
            if word & 0x4000:
                text += "\0\0"
                at += 2
            else:
                c = character(word & 0x1fff)
                text += c or "\0"
                if c:
                    literal.setdefault(c, {}).setdefault(word & 0x3fff, 0)
                    literal[c][word & 0x3fff] += 1
                at += 1
            if blob[at - 1] & 0x8000:
                break
        texts.append(text)
    words, position = [], 0
    while position < len(name):
        best = None
        for length in range(min(63, len(name) - position), 2, -1):
            piece = name[position:position + length]
            for index, text in enumerate(texts):
                start = text.find(piece)
                if 0 <= start < 128 and index != own:
                    best = (length, start, index)
                    break
            if best:
                break
        if best:
            length, start, index = best
            words += [0x4000 | start << 6 | length, 0x4000 | index]
            position += length
        else:
            c = name[position]
            if c not in literal:
                raise ValueError(f"no code known for the character {c!r}")
            words.append(max(literal[c], key=literal[c].get))
            position += 1
    words[-1] |= 0x8000
    return words


def fit(rows, width, height, new_width, new_height):
    """Centre-crop to the target's shape, then scale."""
    if width * new_height > height * new_width:
        keep = height * new_width // new_height
        rows = [row[(width - keep) // 2:(width - keep) // 2 + keep] for row in rows]
        width = keep
    else:
        keep = width * new_height // new_width
        rows = rows[(height - keep) // 2:(height - keep) // 2 + keep]
        height = keep
    return mt.resize(width, height, rows, new_width, new_height)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("card", type=int)
    parser.add_argument("--into", required=True, help="a copy of the disc image to change")
    parser.add_argument("--iso", help="clean disc image; otherwise DOTR_ISO or baserom/*.iso")
    parser.add_argument("--attack", type=int)
    parser.add_argument("--defense", type=int)
    parser.add_argument("--name")
    parser.add_argument("--art", help="8-bit RGB or RGBA PNG")
    args = parser.parse_args(argv)
    try:
        clean = project.find_iso(args.iso)
        target = Path(args.into).resolve()
        if target == clean.resolve() or (ROOT / "baserom") in target.parents:
            raise ValueError("--into must be a copy, not the image in baserom/")
        # A copy may have grown (tools/iso_files.py adds files at the end).
        if not target.is_file() or target.stat().st_size < clean.stat().st_size:
            raise ValueError("--into must be an existing copy of the disc image")
        baseline = json.loads((ROOT / "out/ntsc-u/elf.json").read_text())
        original = (ROOT / "out/ntsc-u/SLUS_205.15").read_bytes()
        metadata = inventory(original)
        base = baseline["iso_extent"]["offset"]
        report = {}
        with target.open("r+b") as stream, clean.open("rb") as source:
            def read(address, size):
                stream.seek(base + file_offset(metadata, address, size))
                return stream.read(size)

            def write(address, data):
                stream.seek(base + file_offset(metadata, address, len(data)))
                stream.write(data)

            for key, where in (("attack", 8), ("defense", 10)):
                value = getattr(args, key)
                if value is not None:
                    if not 0 <= value <= 8191:
                        raise ValueError(f"{key} must be 0 to 8191")
                    address = RECORDS + 20 * args.card + where
                    old, = struct.unpack("<H", read(address, 2))
                    write(address, struct.pack("<H", old & 0xe000 | value))
                    report[key] = f"{old & 0x1fff} -> {value}"
            if args.name:
                offsets = struct.unpack(f"<{STRINGS}i", original[file_offset(metadata, TABLE, 4 * STRINGS):][:4 * STRINGS])
                blob = struct.unpack(f"<{WORDS}H", original[file_offset(metadata, BLOB, 2 * WORDS):][:2 * WORDS])
                words = encode_name(args.name, offsets, blob, NAMES + args.card)
                if len(words) > SPARE_WORDS:
                    raise ValueError(f"the name needs {len(words)} words; there is room for {SPARE_WORDS}")
                write(SPARE, struct.pack(f"<{len(words)}H", *words) + bytes(2 * (SPARE_WORDS - len(words))))
                write(TABLE + 4 * (NAMES + args.card), struct.pack("<i", (SPARE - BLOB) // 2))
                report["name"] = f"{len(words)} words: " + " ".join(f"{w:04x}" for w in words)
            if args.art:
                width, height, rows = mt.read_png(args.art)
                written = []
                source.seek(PICTURE[0] + args.card * PICTURE[1])
                clean_picture = source.read(PICTURE[1])
                picture = None
                # Where the files are in the copy: tools/iso_files.py may have moved one that grew.
                disc = iso_files.Image(target)
                try:
                    where = {}
                    for name in ("PICTURE", "PICMINI", "PICPACK"):
                        _, _, entry = disc.lookup(f"/DATA/{name}.MRG")
                        where[name] = struct.unpack_from("<I", entry, 2)[0] * 2048
                finally:
                    disc.close()
                for name, (start, size) in (("PICTURE", (where["PICTURE"], PICTURE[1])),
                                            ("PICMINI", (where["PICMINI"], PICMINI[1]))):
                    stream.seek(start + args.card * size)
                    data = bytearray(stream.read(size))
                    if data[:4] != b"RH2\0":
                        continue
                    t = mt.info(data, 0)
                    mt.encode(data, 0, fit(rows, width, height, t["width"], t["height"]))
                    stream.seek(start + args.card * size)
                    stream.write(data)
                    written.append(f"{name} {t['width']}x{t['height']}")
                    if name == "PICTURE":
                        picture = bytes(data)
                slots = struct.unpack("<223H", original[file_offset(metadata, PICPACK_TABLE, 446):][:446])
                for slot, card in enumerate(slots):
                    if card != args.card or picture is None:
                        continue
                    source.seek(PICPACK[0] + slot * PICPACK[1])
                    # A pack slot is the start of the card's picture file; only then is it replaced.
                    if source.read(PICPACK[1]) == clean_picture[:PICPACK[1]]:
                        stream.seek(where["PICPACK"] + slot * PICPACK[1])
                        stream.write(picture[:PICPACK[1]])
                        written.append(f"PICPACK slot {slot}")
                    else:
                        written.append(f"PICPACK slot {slot} left alone (not a copy of the picture)")
                report["art"] = written
        print(json.dumps(report, indent=2))
        return 0
    except (OSError, ValueError, KeyError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
