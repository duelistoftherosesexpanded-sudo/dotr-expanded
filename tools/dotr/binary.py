"""Small, bounded readers for ISO9660 root files and ELF32 metadata.

Not a general disc extractor: no UDF, raw-sector or multi-extent support.
"""

import struct

SECTOR = 2048


def read_at(stream, offset, size):
    if offset < 0 or size < 0:
        raise ValueError("negative binary range")
    stream.seek(0, 2)
    if offset + size > stream.tell():
        raise ValueError("binary range exceeds file")
    stream.seek(offset)
    data = stream.read(size)
    if len(data) != size:
        raise ValueError("truncated binary range")
    return data


def root_files(stream):
    pvd = read_at(stream, 16 * SECTOR, SECTOR)
    if pvd[:7] != b"\x01CD001\x01":
        raise ValueError("expected ISO9660 primary descriptor in a 2048-byte-sector image")
    if struct.unpack_from("<H", pvd, 128)[0] != SECTOR:
        raise ValueError("unsupported logical block size")
    lba, size = struct.unpack_from("<I", pvd, 158)[0], struct.unpack_from("<I", pvd, 166)[0]
    if size > 16 * 1024 * 1024:
        raise ValueError("implausible root directory size")
    directory = read_at(stream, lba * SECTOR, size)
    files = {}
    pos = 0
    while pos < size:
        length = directory[pos]
        if not length:
            pos = (pos // SECTOR + 1) * SECTOR
            continue
        record = directory[pos:pos + length]
        if length < 34 or len(record) != length or 33 + record[32] > length:
            raise ValueError("invalid ISO directory record")
        if record[25] & 0x80:
            raise ValueError("multi-extent files are unsupported")
        name = record[33:33 + record[32]]
        if not record[25] & 2:
            name = name.decode("ascii").split(";")[0]
            if name in files:
                raise ValueError("duplicate ISO filename")
            files[name] = {
                "offset": struct.unpack_from("<I", record, 2)[0] * SECTOR,
                "size": struct.unpack_from("<I", record, 10)[0],
            }
        pos += length
    return files


def elf_metadata(data, *, allowed_types=(2,)):
    if len(data) < 52 or data[:7] != b"\x7fELF\x01\x01\x01":
        raise ValueError("expected little-endian ELF32")
    header = struct.unpack_from("<HHIIIIIHHHHHH", data, 16)
    kind, machine, version, entry, phoff = header[:5]
    ehsize, phsize, count = header[7:10]
    if kind not in allowed_types or machine != 8 or version != 1 or ehsize != 52:
        raise ValueError("unexpected MIPS ELF32 type or header")
    if (count and phsize != 32) or phoff + count * phsize > len(data):
        raise ValueError("invalid ELF program-header table")
    segments = []
    for index in range(count):
        values = struct.unpack_from("<8I", data, phoff + index * phsize)
        segment = dict(zip(("type", "offset", "vaddr", "paddr", "filesz", "memsz", "flags", "align"), values))
        if segment["offset"] + segment["filesz"] > len(data):
            raise ValueError("ELF segment exceeds file")
        if segment["type"] == 1 and segment["filesz"] > segment["memsz"]:
            raise ValueError("ELF load segment has invalid memory size")
        segments.append(segment)
    return {"type": kind, "machine": machine, "entry": entry, "flags": header[6], "program_headers": segments}
