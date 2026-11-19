#!/usr/bin/env python3
"""Reads Alchemy .igb scene files (Intrinsic Graphics; the version on Yu-Gi-Oh! The Dawn of Destiny,
Xbox: magic 0xFADA, version 0x80000005), and the Xbox disc image they are on (XDVDFS).

An .igb file, as found on that disc (all little-endian):

  header        12 words: directory bytes and entries, object type bytes and count, object bytes
                and count, memory bytes and count, field type bytes and count, 0xFADA, version
  field types   per type: name length, major, minor (3 words); then the names
  memory kinds  its byte size, 1, a count, the name lengths, the names
  object types  per type: name length, major, minor, field count, parent type, slot count (6 words);
                then per type its name and its fields (field type, slot, size: 3 halfwords each)
  directory     one record per object and per memory block, each an object of its own
                (igObjectDirEntry: name, type; igMemoryDirEntry: name, byte size, element field
                type, a flag, alignment)
  one word      (the scene's root entry, it seems), not counted in the object bytes
  objects       the objects in directory order: type, record size, then each field in order
  memory        the memory blocks in directory order, back to back

A record's fields are 4 bytes each (numbers, enums, bools, chars, object and memory references:
directory indices, -1 for none), 8 for longs and doubles, 12 / 16 / 64 for vectors and matrices,
and a string is its padded byte length then the bytes.

  Igb(bytes).objects {directory index: dict(type, fields)}, .memory {index: bytes}, .tname(i)
  XIso(path).files() {path: (sector, size)}, .read(path)
"""

import struct
from collections import Counter

SECTOR = 2048


class XIso:
    """An Xbox disc image (XDVDFS: the volume descriptor at 0x10000)."""

    def __init__(self, path):
        self.f = open(path, "rb")
        self.f.seek(0x10000)
        head = self.f.read(0x24)
        if head[:20] != b"MICROSOFT*XBOX*MEDIA":
            raise ValueError(f"{path} is not an Xbox disc image")
        self.root, self.root_size = struct.unpack_from("<II", head, 20)
        self._files = None

    def _sector(self, sector, size):
        self.f.seek(sector * SECTOR)
        return self.f.read(size)

    def _walk(self, sector, size, prefix):
        if size == 0:
            return
        table = self._sector(sector, size)
        seen, stack = set(), [0]
        while stack:
            off = stack.pop()
            if off in seen or off * 4 + 14 > len(table):
                continue
            seen.add(off)
            at = off * 4
            left, right, start, length, attr, nlen = struct.unpack_from("<HHIIBB", table, at)
            if left == 0xFFFF and right == 0xFFFF:
                continue
            path = prefix + "/" + table[at + 14:at + 14 + nlen].decode("latin-1")
            if attr & 0x10:
                yield from self._walk(start, length, path)
            else:
                yield path, start, length
            for nxt in (left, right):
                if nxt:
                    stack.append(nxt)

    def files(self):
        if self._files is None:
            self._files = {p: (s, n) for p, s, n in self._walk(self.root, self.root_size, "")}
        return self._files

    def read(self, path):
        sector, size = self.files()[path]
        return self._sector(sector, size)


class Igb:
    def __init__(self, data):
        self.d = data
        (self.entry_size, self.entry_count, self.mo_size, self.mo_count, self.obj_size, self.obj_count,
         self.mem_size, self.mem_count, self.mf_size, self.mf_count, magic, self.version) = struct.unpack_from("<12I", data, 0)
        if magic != 0xFADA:
            raise ValueError("not an .igb file")
        at = 0x30
        recs = [struct.unpack_from("<3I", data, at + 12 * i) for i in range(self.mf_count)]
        at += 12 * self.mf_count
        self.field_types = []
        for n, _, _ in recs:
            self.field_types.append(data[at:at + n].split(b"\0")[0].decode())
            at += n
        size = struct.unpack_from("<I", data, at)[0]
        at += size                                   # the memory kind names
        recs = [struct.unpack_from("<6I", data, at + 24 * i) for i in range(self.mo_count)]
        p = at + 24 * self.mo_count
        self.types = []
        for n, _, _, nfields, parent, slots in recs:
            name = data[p:p + n].split(b"\0")[0].decode()
            p += n
            fields = [struct.unpack_from("<3H", data, p + 6 * k) for k in range(nfields)]
            p += 6 * nfields
            self.types.append(dict(name=name, parent=parent, fields=fields))
        if p != at + self.mo_size:
            raise ValueError("object type table size differs")
        self.entries = []
        for _ in range(self.entry_count):
            rec, p = self._record(p)
            self.entries.append(rec)
        self.root_index = struct.unpack_from("<I", data, p)[0]
        q = p + 4
        self.objects = {}
        for i, e in enumerate(self.entries):
            if self.types[e["type"]]["name"] == "igObjectDirEntry":
                self.objects[i], q = self._record(q)
        if q != p + 4 + self.obj_size:
            raise ValueError("object section size differs")
        self.memory = {}
        for i, e in enumerate(self.entries):
            if self.types[e["type"]]["name"] == "igMemoryDirEntry":
                n = e["fields"][1]
                self.memory[i] = data[q:q + n]
                q += (n + 3) & ~3          # each block starts on 4 bytes (the header's size counts the padding)
        if q > len(data):
            raise ValueError("memory section runs past the file")   # (some files have padding after it)

    def _record(self, p):
        t, size = struct.unpack_from("<2I", self.d, p)
        q = p + 8
        fields = []
        for ft, _, fsize in self.types[t]["fields"]:
            kind = self.field_types[ft]
            if kind == "igStringMetaField":
                n = struct.unpack_from("<I", self.d, q)[0]
                fields.append(self.d[q + 4:q + 4 + n].split(b"\0")[0].decode("latin-1"))
                q += 4 + n
            elif kind == "igFloatMetaField":
                fields.append(struct.unpack_from("<f", self.d, q)[0])
                q += 4
            elif kind == "igDoubleMetaField":
                fields.append(struct.unpack_from("<d", self.d, q)[0])
                q += 8
            elif kind in ("igLongMetaField", "igUnsignedLongMetaField"):
                fields.append(struct.unpack_from("<q", self.d, q)[0])
                q += 8
            elif kind == "igMatrix44fMetaField":
                fields.append(struct.unpack_from("<16f", self.d, q))
                q += 64
            elif kind == "igVec3fMetaField":
                fields.append(struct.unpack_from("<3f", self.d, q))
                q += 12
            elif kind == "igVec4fMetaField":
                fields.append(struct.unpack_from("<4f", self.d, q))
                q += 16
            elif kind == "igVec4fArrayMetaField":
                fields.append(struct.unpack_from(f"<{fsize // 4}f", self.d, q))
                q += fsize
            elif kind == "igUnsignedCharArrayMetaField":
                fields.append(self.d[q:q + fsize])
                q += (fsize + 3) & ~3
            else:
                fields.append(struct.unpack_from("<i", self.d, q)[0])
                q += 4
        if q != p + size:
            raise ValueError(f"{self.types[t]['name']} at {p:#x}: fields take {q - p:#x} bytes, record {size:#x}")
        return dict(type=t, fields=fields), p + size

    def tname(self, i):
        return self.types[(self.objects.get(i) or self.entries[i])["type"]]["name"]

    def f(self, i):
        return self.objects[i]["fields"]

    def list_items(self, i, fmt="i"):
        """The items of an ig*List object (count, capacity, data block)."""
        if i < 0 or i not in self.objects:
            return []
        count, _, block = self.f(i)[:3]
        if block < 0 or count <= 0:
            return []
        n = struct.calcsize("<" + fmt)
        return [struct.unpack_from("<" + fmt, self.memory[block], k * n) for k in range(count)] if len(fmt) > 1 \
            else list(struct.unpack_from(f"<{count}{fmt}", self.memory[block]))

    def counts(self):
        return Counter(self.tname(i) for i in self.objects)
