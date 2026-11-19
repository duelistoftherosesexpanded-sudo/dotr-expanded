"""ROM-free malformed-input, address-space and exact-comparison checks."""

import io
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from dotr.compare import compare_bytes
from dotr.disc import disc_files, overlay_candidates
from dotr.elf import inventory, file_offset, symbol_table
from test_tools import elf_fixture, iso_fixture, record
import research


def sectioned_fixture():
    data = bytearray(elf_fixture())
    data.extend(bytes(512 - len(data)))
    names = b"\0.shstrtab\0.text\0.bss\0"
    data[128:128 + len(names)] = names
    struct.pack_into("<I", data, 32, 256)
    struct.pack_into("<HHH", data, 46, 40, 4, 1)
    struct.pack_into("<10I", data, 296, 1, 3, 0, 0, 128, len(names), 0, 0, 1, 0)
    struct.pack_into("<10I", data, 336, 11, 1, 6, 0x100000, 96, 32, 0, 0, 16, 0)
    struct.pack_into("<10I", data, 376, 17, 8, 3, 0x100020, 9999, 32, 0, 0, 16, 0)
    return bytes(data)


class ElfResearchTests(unittest.TestCase):
    def test_sections_and_zero_fill_mapping(self):
        metadata = inventory(sectioned_fixture())
        self.assertEqual([s["name"] for s in metadata["sections"]], ["", ".shstrtab", ".text", ".bss"])
        self.assertEqual(file_offset(metadata, 0x100000, 32), 96)
        for address, size in ((0x100020, 1), (0x10001f, 2), (0x100000, 0), (0xffffffff, 2)):
            with self.assertRaises(ValueError):
                file_offset(metadata, address, size)
        metadata["program_headers"].append(metadata["program_headers"][0].copy())
        with self.assertRaisesRegex(ValueError, "unique"):
            file_offset(metadata, 0x100000)

    def test_invalid_section_names_and_ranges(self):
        for position, value in ((32, 500), (336, 999), (352, 999), (356, 999)):
            data = bytearray(sectioned_fixture())
            struct.pack_into("<I", data, position, value)
            with self.assertRaises(ValueError):
                inventory(bytes(data))

    def test_irx_type_requires_explicit_address_space(self):
        data = bytearray(sectioned_fixture())
        struct.pack_into("<H", data, 16, 0xff80)
        with self.assertRaises(ValueError):
            inventory(data)
        self.assertEqual(inventory(data, allowed_types=(0xff80,))["type"], 0xff80)

    def test_bounded_symbol_strings(self):
        data = bytearray(sectioned_fixture())
        # Replace NOBITS with a malformed symbol table linked to the string table.
        struct.pack_into("<10I", data, 376, 17, 2, 0, 0, 416, 16, 1, 0, 4, 16)
        struct.pack_into("<IIIBBH", data, 416, 999, 0, 4, 0x12, 0, 2)
        with self.assertRaisesRegex(ValueError, "symbol name"):
            symbol_table(data, inventory(data))


class DiscResearchTests(unittest.TestCase):
    def nested_fixture(self):
        data = bytearray(iso_fixture())
        data.extend(bytes(2 * 2048))
        parent = record(b"\0", 19, 2048, 2) + record(b"IOP", 22, 2048, 2)
        data[19 * 2048:20 * 2048] = bytes(2048)
        data[19 * 2048:19 * 2048 + len(parent)] = parent
        child = record(b"\0", 22, 2048, 2) + record(b"\1", 19, 2048, 2) + record(b"TEST.IRX;1", 23, 4)
        data[22 * 2048:22 * 2048 + len(child)] = child
        data[23 * 2048:23 * 2048 + 4] = b"TEST"
        return data

    def test_recursive_paths_and_extents(self):
        self.assertEqual(disc_files(io.BytesIO(self.nested_fixture())),
                         {"IOP/TEST.IRX": {"lba": 23, "offset": 23 * 2048, "size": 4}})

    def test_directory_cycles_and_dual_endian_disagreement(self):
        data = self.nested_fixture()
        child = record(b"LOOP", 19, 2048, 2)
        data[22 * 2048:22 * 2048 + len(child)] = child
        with self.assertRaisesRegex(ValueError, "cyclic"):
            disc_files(io.BytesIO(data))
        data = self.nested_fixture()
        struct.pack_into(">I", data, 16 * 2048 + 162, 999)
        with self.assertRaisesRegex(ValueError, "disagreement"):
            disc_files(io.BytesIO(data))

    def test_file_extent_and_path_rejections(self):
        for name, lba in ((b"../BAD;1", 23), (b"BAD;1", 1000)):
            data = self.nested_fixture()
            child = record(name, lba, 4)
            data[22 * 2048:23 * 2048] = bytes(2048)
            data[22 * 2048:22 * 2048 + len(child)] = child
            with self.assertRaises(ValueError):
                disc_files(io.BytesIO(data))

    def test_overlay_candidates_do_not_imply_payload_sizes(self):
        data = bytearray(3 * 2048)
        data[:4] = data[4096:4100] = b"MWo3"
        data[120:124] = b"MWo3"
        entries = overlay_candidates(data)
        self.assertEqual([e["offset"] for e in entries], [0, 4096])
        self.assertEqual([e["candidate_extent_size"] for e in entries], [4096, 2048])
        with self.assertRaises(ValueError):
            overlay_candidates(data, 0)


class ComparisonTests(unittest.TestCase):
    def test_exact_comparison_rejects_instruction_and_size_changes(self):
        original = bytes.fromhex("00000000 0800e003")
        self.assertTrue(compare_bytes(original, original)["matching"])
        changed = compare_bytes(original, b"\1" + original[1:])
        self.assertFalse(changed["matching"])
        self.assertEqual(changed["first_differences"], [0])
        longer = compare_bytes(original, original + bytes(4))
        self.assertFalse(longer["matching"])
        self.assertEqual(longer["different_bytes"], 4)


if __name__ == "__main__":
    unittest.main()
