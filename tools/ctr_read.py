#!/usr/bin/env python3
"""Reads a decrypted Nintendo 3DS game image (.cci / .3ds, NCSD), as the player's own
Yu-Gi-Oh! Saikyo Card Battle decrypts to: the first partition's RomFS (the game's files) and
ExeFS program (.code, decompressed).

  ctr_read.py GAME.cci --list
  ctr_read.py GAME.cci --extract OUT_FOLDER        (the RomFS files)
  ctr_read.py GAME.cci --code OUT.bin
"""
import argparse
import struct
from pathlib import Path

MEDIA = 0x200


class Cci:
    def __init__(self, path):
        self.path = Path(path)
        self.f = open(path, "rb")
        self.f.seek(0x100)
        if self.f.read(4) != b"NCSD":
            raise ValueError(f"{self.path.name} is not a decrypted 3DS game image (no NCSD header)")
        self.f.seek(0x120)
        self.part = struct.unpack("<I", self.f.read(4))[0] * MEDIA
        self.f.seek(self.part + 0x100)
        if self.f.read(4) != b"NCCH":
            raise ValueError(f"{self.path.name}: no NCCH partition")
        self.f.seek(self.part + 0x188 + 7)
        if not self.f.read(1)[0] & 4:
            raise ValueError(f"{self.path.name} is still encrypted: decrypt it first")
        self.f.seek(self.part + 0x150)
        self.product = self.f.read(16).rstrip(b"\0").decode("ascii", "replace")
        self._files = None

    def read_at(self, at, n):
        self.f.seek(at)
        return self.f.read(n)

    # ---------------------------------------------------------------- RomFS
    def files(self):
        """{path: (offset, size)} of the RomFS."""
        if self._files is not None:
            return self._files
        f = self.f
        f.seek(self.part + 0x1B0)
        rom_off = struct.unpack("<I", f.read(4))[0]
        rom = self.part + rom_off * MEDIA
        ivfc = self.read_at(rom, 0x5C)
        if ivfc[:4] != b"IVFC":
            raise ValueError("no RomFS")
        master = struct.unpack_from("<I", ivfc, 8)[0]
        block = 1 << struct.unpack_from("<I", ivfc, 0x4C)[0]
        l3 = rom + ((0x60 + master + block - 1) & ~(block - 1))
        h = struct.unpack("<10I", self.read_at(l3, 40))
        dirs = self.read_at(l3 + h[3], h[4])
        files = self.read_at(l3 + h[7], h[8])
        data = l3 + h[9]
        out = {}

        def walk(d, prefix):
            _, _, child, file, _, nlen = struct.unpack_from("<6I", dirs, d)
            name = dirs[d + 24:d + 24 + nlen].decode("utf-16-le")
            here = prefix + name + "/" if d else "/"
            fo = file
            while fo != 0xFFFFFFFF:
                _, sibling, off, size, _, flen = struct.unpack_from("<IIQQII", files, fo)
                out[here + files[fo + 32:fo + 32 + flen].decode("utf-16-le")] = (data + off, size)
                fo = sibling
            c = child
            while c != 0xFFFFFFFF:
                walk(c, here)
                c = struct.unpack_from("<I", dirs, c + 4)[0]
        walk(0, "")
        self._files = out
        return out

    def read(self, path):
        at, size = self.files()["/" + path.lstrip("/")]
        return self.read_at(at, size)

    # ---------------------------------------------------------------- ExeFS
    def code(self):
        exheader = self.read_at(self.part + 0x200, 0x400)
        compressed = bool(exheader[0xD] & 1)
        exefs = struct.unpack("<I", self.read_at(self.part + 0x1A0, 4))[0] * MEDIA + self.part
        header = self.read_at(exefs, 0x200)
        for i in range(10):
            name = header[i * 16:i * 16 + 8].rstrip(b"\0")
            off, size = struct.unpack_from("<II", header, i * 16 + 8)
            if name == b".code":
                data = self.read_at(exefs + 0x200 + off, size)
                return blz_decompress(data) if compressed else data
        raise ValueError("no .code in the ExeFS")


def blz_decompress(data):
    """The ExeFS code compression (LZ77 read backwards from the end)."""
    top_bottom, extra = struct.unpack_from("<II", data, len(data) - 8)
    top, bottom = top_bottom >> 24, top_bottom & 0xFFFFFF
    out = bytearray(len(data) + extra)
    out[:len(data)] = data
    src, dst, end = len(data) - top, len(out), len(data) - bottom
    while src > end:
        src -= 1
        flags = out[src]
        for _ in range(8):
            if src <= end:
                break
            if flags & 0x80:
                src -= 2
                v = out[src] | out[src + 1] << 8
                n, disp = ((v >> 12) & 0xF) + 3, (v & 0xFFF) + 3
                for _ in range(n):
                    dst -= 1
                    out[dst] = out[dst + disp]
            else:
                src -= 1
                dst -= 1
                out[dst] = out[src]
            flags = (flags << 1) & 0xFF
    return bytes(out)


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("cci")
    p.add_argument("--list", action="store_true")
    p.add_argument("--extract")
    p.add_argument("--code")
    a = p.parse_args()
    c = Cci(a.cci)
    if a.list:
        for path, (_, size) in sorted(c.files().items()):
            print(size, path)
    if a.extract:
        for path, (at, size) in c.files().items():
            t = Path(a.extract) / path.lstrip("/")
            t.parent.mkdir(parents=True, exist_ok=True)
            t.write_bytes(c.read_at(at, size))
    if a.code:
        Path(a.code).write_bytes(c.code())


if __name__ == "__main__":
    main()
