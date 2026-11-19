#!/usr/bin/env python3
"""Add, replace or grow files in a copy of the disc image (ISO 9660 only).

  iso_files.py IMAGE ls                      list the files with their sectors and sizes
  iso_files.py IMAGE put SOURCE /DIR/NAME    store SOURCE as /DIR/NAME (";1" is added)

A file that still fits in the sectors it has is rewritten in place; a bigger or new file goes to the
end of the image, which grows, and the volume size in the primary volume descriptor is updated. A
new file gets a record in its directory, kept in name order; the directory must have room in the
sectors it already has (they are mostly empty on this disc). Only the ISO 9660 side is updated: the
disc also carries a UDF description that now goes stale, but the PS2 (its CD/DVD file reader) and
PCSX2 read only ISO 9660. The game itself refreshes its built-in table of file positions from the
ISO 9660 directory at boot (RhFile_Init), so moved files are found.

Your own image under baserom/ is never written to.
"""

import argparse
import struct
import sys
from pathlib import Path

SECTOR = 2048
ROOT_DIR = Path(__file__).resolve().parent.parent


def both32(value):
    return struct.pack("<I", value) + struct.pack(">I", value)


def both16(value):
    return struct.pack("<H", value) + struct.pack(">H", value)


class Image:
    def __init__(self, path, read_only=False):
        """read_only: for reading (the clean image under baserom/ too); writing then fails."""
        self.path = Path(path)
        if not read_only and (ROOT_DIR / "baserom") in self.path.resolve().parents:
            raise ValueError("refusing to write to the image under baserom/; work on a copy")
        self.stream = self.path.open("rb" if read_only else "r+b")
        self.pvd = self.read(16)
        if self.pvd[0] != 1 or self.pvd[1:6] != b"CD001":
            raise ValueError("no primary volume descriptor at sector 16")
        self.volume = struct.unpack_from("<I", self.pvd, 80)[0]

    def read(self, lba, count=1):
        self.stream.seek(lba * SECTOR)
        return self.stream.read(count * SECTOR)

    def write(self, lba, data):
        self.stream.seek(lba * SECTOR)
        self.stream.write(data)

    def close(self):
        self.stream.close()

    def records(self, lba, size):
        """The records of a directory: (sector index, offset, raw record)."""
        data = self.read(lba, (size + SECTOR - 1) // SECTOR)
        result = []
        for start in range(0, size, SECTOR):
            o = start
            while o < start + SECTOR and data[o]:
                result.append(data[o:o + data[o]])
                o += data[o]
        return result

    def lookup(self, path):
        """(directory extent, directory size, record or None) for /A/B/NAME."""
        parts = [p for p in path.upper().split("/") if p]
        lba = struct.unpack_from("<I", self.pvd, 156 + 2)[0]
        size = struct.unpack_from("<I", self.pvd, 156 + 10)[0]
        for i, part in enumerate(parts):
            last = i == len(parts) - 1
            want = (part if ";" in part else part + ";1") if last else part
            found = None
            for record in self.records(lba, size):
                name = record[33:33 + record[32]].decode("ascii", "replace")
                if name == want or (not last and name == part):
                    found = record
            if last:
                return lba, size, found
            if found is None or not found[25] & 2:
                raise ValueError(f"no directory {part} in {path}")
            lba, size = struct.unpack_from("<I", found, 2)[0], struct.unpack_from("<I", found, 10)[0]
        raise ValueError("empty path")

    def files(self):
        root = struct.unpack_from("<I", self.pvd, 158)[0], struct.unpack_from("<I", self.pvd, 166)[0]
        stack = [("", *root)]
        while stack:
            prefix, lba, size = stack.pop()
            for record in self.records(lba, size):
                name = record[33:33 + record[32]]
                if name in (b"\0", b"\1"):
                    continue
                ext, length = struct.unpack_from("<I", record, 2)[0], struct.unpack_from("<I", record, 10)[0]
                if record[25] & 2:
                    stack.append((prefix + "/" + name.decode(), ext, length))
                else:
                    yield prefix + "/" + name.decode(), ext, length

    def set_volume(self, sectors):
        self.volume = sectors
        self.pvd = self.pvd[:80] + both32(sectors) + self.pvd[88:]
        self.write(16, self.pvd)
        self.stream.truncate(sectors * SECTOR)

    @staticmethod
    def pack_directory(records, size):
        """Packs records (the first two are . and ..) into the directory's sectors; returns the
        bytes to write and the directory's new data length. Directories on this disc record the
        bytes they use (422 for the root), not whole sectors."""
        sectors, current = [], b""
        for record in records:
            if len(current) + len(record) > SECTOR:
                sectors.append(current)
                current = b""
            current += record
        sectors.append(current)
        have = (size + SECTOR - 1) // SECTOR
        if len(sectors) > have:
            raise ValueError("the directory has no room for another record")
        length = (len(sectors) - 1) * SECTOR + len(sectors[-1]) if size % SECTOR else size
        data = b"".join(s + bytes(SECTOR - len(s)) for s in sectors)
        return data + bytes(have * SECTOR - len(data)), length

    def set_directory_length(self, lba, length):
        """Stores the new data length of the directory at lba in every record of it: its entry
        in the parent (or the root record of the volume descriptor), its own . and its
        subdirectories' .."""
        def patch(record):
            return record[:10] + both32(length) + record[18:]
        if struct.unpack_from("<I", self.pvd, 158)[0] == lba:
            self.pvd = self.pvd[:156] + patch(self.pvd[156:190]) + self.pvd[190:]
            self.write(16, self.pvd)
        root = struct.unpack_from("<I", self.pvd, 158)[0], struct.unpack_from("<I", self.pvd, 166)[0]
        stack, seen = [root], set()
        while stack:
            dlba, dsize = stack.pop()
            if dlba in seen:
                continue
            seen.add(dlba)
            count = (dsize + SECTOR - 1) // SECTOR
            data = bytearray(self.read(dlba, count))
            changed = False
            for start in range(0, dsize, SECTOR):
                o = start
                while o < start + SECTOR and data[o]:
                    n = data[o]
                    if data[o + 25] & 2:
                        ext = struct.unpack_from("<I", data, o + 2)[0]
                        if ext == lba:
                            data[o:o + n] = patch(bytes(data[o:o + n]))
                            changed = True
                        if data[o + 32] != 1 or data[o + 33] > 1:
                            stack.append((ext, struct.unpack_from("<I", data, o + 10)[0]))
                    o += n
            if changed:
                self.write(dlba, bytes(data))

    def put(self, path, data):
        """Stores data as path; returns its first sector."""
        dir_lba, dir_size, record = self.lookup(path)
        need = (len(data) + SECTOR - 1) // SECTOR
        if record is not None and record[25] & 2:
            raise ValueError(f"{path} is a directory")
        extent = None
        if record is not None:
            extent = struct.unpack_from("<I", record, 2)[0]
            have = (struct.unpack_from("<I", record, 10)[0] + SECTOR - 1) // SECTOR
            if need > have:
                extent = None
        grow = extent is None
        if grow:
            extent = self.volume
        name = path.upper().split("/")[-1]
        name = (name if ";" in name else name + ";1").encode("ascii")
        records = self.records(dir_lba, dir_size)
        date = records[0][18:25]
        body = (bytes([0]) + both32(extent) + both32(len(data)) + date + bytes([0, 0, 0]) + both16(1)
                + bytes([len(name)]) + name)
        new = bytes([33 + len(name) + (len(name) + 1) % 2]) + body + bytes((len(name) + 1) % 2)
        others = [r for r in records[2:] if r[33:33 + r[32]] != name]
        ordered = sorted(others + [new], key=lambda r: r[33:33 + r[32]])
        directory, length = self.pack_directory(records[:2] + ordered, dir_size)
        # Everything is checked; now write.
        if grow:
            self.set_volume(extent + max(need, 1))
        self.write(extent, data + bytes(need * SECTOR - len(data)))
        self.write(dir_lba, directory)
        if length != dir_size:
            self.set_directory_length(dir_lba, length)
        return extent


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("image")
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("ls")
    put = sub.add_parser("put")
    put.add_argument("source")
    put.add_argument("path")
    args = parser.parse_args(argv)
    try:
        image = Image(args.image)
        try:
            if args.command == "ls":
                for path, lba, size in sorted(image.files(), key=lambda f: f[1]):
                    print(f"{lba:8d} {size:12d}  {path}")
            else:
                data = Path(args.source).read_bytes()
                lba = image.put(args.path, data)
                print(f"{args.path}: {len(data)} bytes at sector {lba}; volume {image.volume} sectors")
        finally:
            image.close()
        return 0
    except (OSError, ValueError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
