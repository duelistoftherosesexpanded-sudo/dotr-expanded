"""Synthetic fixtures only: no original game data is used."""

import contextlib
import hashlib
import io
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import project
from dotr.binary import elf_metadata, read_at, root_files


def elf_fixture():
    data = bytearray(128)
    data[:7] = b"\x7fELF\x01\x01\x01"
    struct.pack_into("<HHIIIIIHHHHHH", data, 16,
                     2, 8, 1, 0x100008, 52, 0, 0, 52, 32, 1, 0, 0, 0)
    struct.pack_into("<8I", data, 52, 1, 96, 0x100000, 0x100000, 32, 64, 5, 16)
    return bytes(data)


def record(name, lba, size, flags=0):
    length = 33 + len(name)
    length += length % 2
    data = bytearray(length)
    data[0], data[25], data[32] = length, flags, len(name)
    struct.pack_into("<I", data, 2, lba)
    struct.pack_into(">I", data, 6, lba)
    struct.pack_into("<I", data, 10, size)
    struct.pack_into(">I", data, 14, size)
    data[33:33 + len(name)] = name
    return bytes(data)


def iso_fixture():
    data = bytearray(22 * 2048)
    pvd = 16 * 2048
    data[pvd:pvd + 7] = b"\x01CD001\x01"
    struct.pack_into("<H", data, pvd + 128, 2048)
    struct.pack_into(">H", data, pvd + 130, 2048)
    root = record(b"\x00", 19, 2048, 2)
    data[pvd + 156:pvd + 156 + len(root)] = root
    system = b"BOOT2 = cdrom0:\\SLUS_205.15;1\r\nVER = 1.00\r\nVMODE = NTSC\r\n"
    executable = elf_fixture()
    directory = root + record(b"SYSTEM.CNF;1", 20, len(system)) + record(b"SLUS_205.15;1", 21, len(executable))
    data[19 * 2048:19 * 2048 + len(directory)] = directory
    data[20 * 2048:20 * 2048 + len(system)] = system
    data[21 * 2048:21 * 2048 + len(executable)] = executable
    return bytes(data)


class BinaryTests(unittest.TestCase):
    def test_root_files_and_elf(self):
        stream = io.BytesIO(iso_fixture())
        files = root_files(stream)
        self.assertEqual(set(files), {"SYSTEM.CNF", "SLUS_205.15"})
        entry = files["SLUS_205.15"]
        metadata = elf_metadata(read_at(stream, entry["offset"], entry["size"]))
        self.assertEqual(metadata["entry"], 0x100008)
        self.assertEqual(metadata["program_headers"][0]["memsz"], 64)

    def test_truncated_reads(self):
        for offset, size in ((-1, 1), (0, -1), (2, 10)):
            with self.assertRaises(ValueError):
                read_at(io.BytesIO(b"123"), offset, size)

    def test_bad_disc_descriptor(self):
        with self.assertRaises(ValueError):
            root_files(io.BytesIO(bytes(22 * 2048)))

    def test_malformed_directory(self):
        data = bytearray(iso_fixture())
        data[19 * 2048] = 4
        with self.assertRaises(ValueError):
            root_files(io.BytesIO(data))

    def test_bad_elf_ranges_and_architecture(self):
        for offset, value in ((18, 3), (28, 1000), (68, 1000)):
            data = bytearray(elf_fixture())
            struct.pack_into("<H" if offset == 18 else "<I", data, offset, value)
            with self.assertRaises(ValueError):
                elf_metadata(data)


class ProjectTests(unittest.TestCase):
    def test_prepare_and_hash_rejection(self):
        fixture = iso_fixture()
        config = project.load_config()
        config.update(image_size=len(fixture), image_md5=hashlib.md5(fixture).hexdigest())
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = root / "fixture.iso"
            path.write_bytes(fixture)
            with patch.object(project, "ROOT", root), contextlib.redirect_stdout(io.StringIO()):
                project.prepare(path, config)
                self.assertEqual((root / "out/ntsc-u/SLUS_205.15").read_bytes(), elf_fixture())
                metadata = json.loads((root / "out/ntsc-u/elf.json").read_text())
                self.assertEqual(metadata["iso_extent"]["offset"], 21 * 2048)
                path.write_bytes(fixture[:-1] + b"X")
                with self.assertRaisesRegex(ValueError, "MD5"):
                    project.verify(path, config)
                path.write_bytes(b"short")
                with self.assertRaisesRegex(ValueError, "size"):
                    project.verify(path, config)

    def test_disc_selection(self):
        with tempfile.TemporaryDirectory() as temp, patch.dict(project.os.environ, {}, clear=True):
            root = Path(temp)
            (root / "baserom").mkdir()
            with patch.object(project, "ROOT", root):
                with self.assertRaises(ValueError):
                    project.find_iso(None)
                path = root / "baserom/a.iso"
                path.touch()
                self.assertEqual(project.find_iso(None), path)
                (root / "baserom/b.iso").touch()
                with self.assertRaises(ValueError):
                    project.find_iso(None)
                self.assertEqual(project.find_iso(str(path)), path)

    def test_registry_duplicate_and_source_escape(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "note.md").touch()
            header = ",".join(project.FIELDS) + "\n"
            row = "ee-main,0x100000,4,func_00100000,function,identified,note.md,note.md,c\n"
            registry = root / "symbols.csv"
            with patch.object(project, "ROOT", root), patch.object(project, "CONFIG", root):
                registry.write_text(header + row)
                self.assertEqual(len(project.symbols()), 1)
                registry.write_text(header + row + row)
                with self.assertRaisesRegex(ValueError, "duplicate"):
                    project.symbols()
                registry.write_text(header + row.replace("identified,note.md", "identified,../outside.md"))
                with self.assertRaisesRegex(ValueError, "source"):
                    project.symbols()
                registry.write_text(header + row.replace("note.md,note.md", "note.md,../outside.md"))
                with self.assertRaisesRegex(ValueError, "evidence"):
                    project.symbols()

    def test_cli_reports_missing_image(self):
        with contextlib.redirect_stderr(io.StringIO()) as output:
            self.assertEqual(project.main(["verify", "--iso", "/nonexistent/dotr.iso"]), 1)
        self.assertIn("ISO does not exist", output.getvalue())


def relocatable_fixture():
    """Two same-named code sections, as emitted per function by the target compiler."""
    names = b"\0.text\0.symtab\0.strtab\0.shstrtab\0"
    strings = b"\0first\0second\0inner\0"
    code = [bytes(range(4)), bytes(range(16, 24))]
    symbols = bytes(16) + b"".join(struct.pack("<IIIBBH", name, value, size, 0x12, 0, section)
                                   for name, value, size, section in
                                   ((1, 0, 4, 1), (7, 0, 8, 2), (14, 4, 4, 2)))
    bodies = [b"", code[0], code[1], symbols, strings, names]
    data = bytearray(52)
    offsets = []
    for body in bodies:
        offsets.append(len(data))
        data += body
    section_table = len(data)
    kinds = [(0, 0, 0, 0, 0), (1, 1, 0, 0, 0), (1, 1, 0, 0, 0), (7, 2, 4, 1, 16), (15, 3, 0, 0, 0), (23, 3, 0, 0, 0)]
    for (name, kind, link, info, entry), offset, body in zip(kinds, offsets, bodies):
        data += struct.pack("<10I", name, kind, 0, 0, offset if kind else 0, len(body), link, info, 1, entry)
    data[:7] = b"\x7fELF\x01\x01\x01"
    struct.pack_into("<HHIIIIIHHHHHH", data, 16, 1, 8, 1, 0, 0, section_table, 0, 52, 0, 0, 40, 6, 5)
    return bytes(data), code


class FunctionSectionTests(unittest.TestCase):
    def test_rename_function_sections(self):
        from dotr.elf import inventory, rename_function_sections, symbol_table
        data, code = relocatable_fixture()
        renamed = rename_function_sections(data, ["second", "first"])
        metadata = inventory(renamed, allowed_types=(1,))
        self.assertEqual([s["name"] for s in metadata["sections"][1:3]], [".text.first", ".text.second"])
        self.assertEqual([s["name"] for s in metadata["sections"][3:]], [".symtab", ".strtab", ".shstrtab"])
        for section, expected in zip(metadata["sections"][1:3], code):
            self.assertEqual(renamed[section["offset"]:section["offset"] + section["size"]], expected)
        self.assertEqual(symbol_table(renamed, metadata), symbol_table(data, inventory(data, allowed_types=(1,))))
        self.assertEqual(renamed[:16], data[:16])
        partial = inventory(rename_function_sections(data, ["first"]), allowed_types=(1,))
        self.assertEqual([s["name"] for s in partial["sections"][1:3]], [".text.first", ".text"])

    def test_rename_function_sections_rejects_unplaceable(self):
        from dotr.elf import rename_function_sections
        data, _ = relocatable_fixture()
        for names in (["missing"], ["inner"], ["first", "first"]):
            with self.assertRaises(ValueError):
                rename_function_sections(data, names)


if __name__ == "__main__":
    unittest.main()
