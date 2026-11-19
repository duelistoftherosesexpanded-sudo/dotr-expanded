"""Bounded recursive ISO9660 inventory and aligned overlay signature scanning.

Plain 2048-byte-sector ISO9660 only; no UDF, interleaving or multi-extent files.
"""

import struct
from .binary import SECTOR, read_at


def both_endian(record, offset):
    little = struct.unpack_from("<I", record, offset)[0]
    big = struct.unpack_from(">I", record, offset + 4)[0]
    if little != big:
        raise ValueError("ISO dual-endian field disagreement")
    return little


def disc_files(stream):
    pvd = read_at(stream, 16 * SECTOR, SECTOR)
    if pvd[:7] != b"\x01CD001\x01" or struct.unpack_from("<H", pvd, 128)[0] != SECTOR:
        raise ValueError("expected plain 2048-byte-sector ISO9660")
    if struct.unpack_from(">H", pvd, 130)[0] != SECTOR:
        raise ValueError("ISO logical block size disagreement")
    files, visited = {}, set()
    record_count = 0

    def visit(lba, size, prefix, depth):
        nonlocal record_count
        identity = (lba, size)
        if depth > 16 or identity in visited or not 0 < size <= 16 * 1024 * 1024:
            raise ValueError("cyclic, repeated or implausible ISO directory")
        visited.add(identity)
        directory = read_at(stream, lba * SECTOR, size)
        position = 0
        names = set()
        while position < size:
            length = directory[position]
            if not length:
                position = (position // SECTOR + 1) * SECTOR
                continue
            record = directory[position:position + length]
            record_count += 1
            if (record_count > 100000 or length < 34 or len(record) != length
                    or position % SECTOR + length > SECTOR or 33 + record[32] > length):
                raise ValueError("invalid ISO directory record")
            if record[25] & 0x80 or record[26] or record[27]:
                raise ValueError("multi-extent or interleaved ISO files are unsupported")
            raw_name = record[33:33 + record[32]]
            child_lba, child_size = both_endian(record, 2), both_endian(record, 10)
            position += length
            if raw_name in (b"\0", b"\1"):
                continue
            name = raw_name.decode("ascii").split(";")[0]
            if not name or name in (".", "..") or "/" in name or "\\" in name or name in names:
                raise ValueError("invalid or duplicate ISO name")
            names.add(name)
            path = prefix + name
            if record[25] & 2:
                visit(child_lba, child_size, path + "/", depth + 1)
            else:
                # Validate extents without reading potentially large assets.
                read_at(stream, child_lba * SECTOR + child_size, 0)
                files[path] = {"lba": child_lba, "offset": child_lba * SECTOR, "size": child_size}

    visit(both_endian(pvd, 158), both_endian(pvd, 166), "", 0)
    return files


def overlay_candidates(data, alignment=SECTOR):
    """Signature-delimited candidates, not validated executable/relocation sizes."""
    if alignment <= 0:
        raise ValueError("invalid overlay alignment")
    positions = [p for p in range(0, len(data), alignment) if data[p:p + 4] == b"MWo3"]
    return [{"index": i, "offset": p,
             "candidate_extent_size": (positions[i + 1] if i + 1 < len(positions) else len(data)) - p}
            for i, p in enumerate(positions)]
