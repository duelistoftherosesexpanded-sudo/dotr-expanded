"""ROM-free regression checks for complete data comparisons and match categories."""
import csv
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import match
import project


def section(name=".rodata", address=0x2000, size=8, flags=2, alignment=4):
    return dict(name=name, address=address, offset=4, size=size, type=1,
                flags=flags, alignment=alignment)


def metadata():
    return dict(sha256="fixture", sections=[section()], program_headers=[
        dict(type=1, vaddr=0x1000, offset=0, filesz=4),
        dict(type=1, vaddr=0x2000, offset=4, filesz=8)])


class ReadOnlyTests(unittest.TestCase):
    def setUp(self):
        # The builds under test print their MATCH/NONMATCH report lines; keep them out of the test output.
        quiet = patch("sys.stdout", new_callable=io.StringIO)
        quiet.start()
        self.addCleanup(quiet.stop)
        self.function = dict(address="0x1000", size="0x4", read_only_data=[
            dict(section=".rodata", address="0x2000", size="0x8")])
        self.placements = match.read_only_placements(self.function)

    def test_rejects_invalid_and_overlapping_placements(self):
        for entry in (dict(section=".text", address="0x2000", size="0x8"),
                      dict(section=".rodata", address="0x1000", size="0x8"),
                      dict(section=".rodata", address="0xfffffffc", size="0x8"),
                      dict(section=".rodata", address="0x2000", size="0x0"),
                      dict(section=".rodata", address="0x2000", size="0x8", mask="ff")):
            with self.subTest(entry=entry), self.assertRaises(ValueError):
                match.read_only_placements({**self.function, "read_only_data": [entry]})
        with self.assertRaises(ValueError):
            match.read_only_placements({**self.function, "read_only_data": self.function["read_only_data"] * 2})
        with self.assertRaises(ValueError):
            match.read_only_placements({**self.function, "read_only_data": {}})

    def test_requires_complete_unique_aligned_read_only_object(self):
        match.validate_read_only_object(metadata(), self.placements)
        for sections in ([section(size=12)], [section(flags=3)], [section(flags=6)],
                         [section(alignment=0x4000)], [section(alignment=3)],
                         [section(), section()], [section(), section(name=".data", flags=3)]):
            with self.subTest(sections=sections), self.assertRaises(ValueError):
                match.validate_read_only_object(dict(sections=sections), self.placements)

    def test_accepts_several_same_named_sections_only_as_one_complete_placement(self):
        # One section per table (two `switch` statements): accepted when, laid out in order with
        # their own alignments, they are exactly the placement.
        match.validate_read_only_object(dict(sections=[section(size=4), section(size=4)]), self.placements)
        match.validate_read_only_object(dict(sections=[section(size=2, alignment=2), section(size=4)]), self.placements)
        for sections in ([],                                                    # nothing to place
                         [section(size=4), section(size=8)],                    # too much
                         [section(size=4)],                                     # too little
                         [section(size=4), section(size=4, flags=3)],           # one of them writable
                         [section(size=4), section(size=4, alignment=8)],       # padding makes it 12
                         [section(size=4), section(size=4, alignment=3)],       # not a power of two
                         [section(size=4), section(size=0), section(size=4)]):  # an empty section
            with self.subTest(sections=sections), self.assertRaises(ValueError):
                match.validate_read_only_object(dict(sections=sections), self.placements)

    def test_compares_every_data_byte_and_accepts_flattened_retail_section(self):
        original = b"CODE12345678"
        original_metadata = metadata()
        original_metadata["sections"] = [section(flags=7)]
        with tempfile.TemporaryDirectory() as temp:
            output = Path(temp)
            good = match.compare_read_only(original, original_metadata, original, metadata(), self.placements, output)
            self.assertTrue(good[0]["matching"])
            bad = match.compare_read_only(original, original_metadata, b"CODE1234567X", metadata(), self.placements, output)
            self.assertFalse(bad[0]["matching"])
            self.assertEqual(bad[0]["different_bytes"], 1)
            self.assertEqual((output / "read-only-0.original.bin").read_bytes(), b"12345678")
            self.assertEqual((output / "read-only-0.rebuilt.bin").read_bytes(), b"1234567X")

    def test_rejects_changed_linked_extent_flags_and_address(self):
        for s in (section(size=4), section(flags=3), section(address=0x2010)):
            with tempfile.TemporaryDirectory() as temp, self.assertRaises(ValueError):
                match.compare_read_only(b"CODE12345678", metadata(), b"CODE12345678",
                                        {**metadata(), "sections": [s]}, self.placements, Path(temp))

    def test_data_failure_makes_function_build_nonmatching(self):
        report, command, script = self.build_fixture("c", b"CODE1234567X", data=True)
        self.assertTrue(report["function_matching"])
        self.assertFalse(report["matching"])
        self.assertIn(" .rodata 0x2000", script)
        self.assertEqual(report["read_only_data"][0]["different_bytes"], 1)
        self.assertIn("-c", command)

    def test_asm_uses_pinned_assembler_and_stays_separate(self):
        report, command, _ = self.build_fixture("asm", b"CODE12345678")
        self.assertTrue(report["matching"])
        self.assertEqual(report["match_kind"], "asm")
        self.assertIn("-march=r5900", command)
        self.assertNotIn("-c", command)
        self.assertIsNone(report["compiler_version"])
        self.assertEqual(report["assembler_sha256"], "assembler-fixture")

    def test_warm_compiles_warm_up_file_first_in_one_run(self):
        report, command, _ = self.build_fixture("c", b"CODE12345678", warm=True)
        self.assertTrue(report["matching"])
        self.assertTrue(report["warm"])
        warm_source, source = command[-2], command[-1]
        self.assertTrue(warm_source.endswith("dotr_warm.c"))
        self.assertTrue(Path(source).as_posix().endswith("src/ee/f.c"))
        self.assertTrue(Path(command[command.index("-o") + 1]).name == "warm")
        with self.assertRaises(ValueError):
            self.build_fixture("asm", b"CODE12345678", warm=True)

    def build_fixture(self, kind, rebuilt, data=False, warm=False):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / ("asm/ee/f.s" if kind == "asm" else "src/ee/f.c")
            source.parent.mkdir(parents=True)
            source.write_text("synthetic fixture")
            f = dict(name="func_fixture", module="ee-main", address="0x1000", size="0x4",
                     source=str(source.relative_to(root)), headers=[], globals={}, match_kind=kind)
            if data:
                f["read_only_data"] = self.function["read_only_data"]
            if warm:
                f["warm"] = True
            commands = []

            def run(command, **kwargs):
                commands.append(command)
                path = Path(command[command.index("-o") + 1])
                if path.is_dir():  # warm run: one object per source in the output directory
                    for name in command[command.index("-o") + 2:]:
                        (path / (Path(name).stem + ".o")).write_bytes(b"object")
                    return
                path.write_bytes(b"object" if path.suffix == ".o" else rebuilt)

            artifact = dict(version="fixture", executable_sha256="fixture", assembler_sha256="assembler-fixture")
            symbol = dict(name="func_fixture", type=2, value=0x1000, size=4)
            obj_metadata = dict(sections=[section(address=0)])
            with patch.object(match, "ROOT", root), patch.object(match, "checked_tool", return_value=(Path("/fixture-tool"), artifact)), \
                    patch.object(match.subprocess, "run", side_effect=run), \
                    patch.object(match, "inventory", side_effect=lambda b, **kw: obj_metadata if b == b"object" else metadata()), \
                    patch.object(match, "symbol_table", return_value=[symbol]):
                report = match.build_function(f, dict(compiler="fixture", flags=["-O4,p"]),
                                              b"CODE12345678", metadata(), dict(iso_extent=dict(offset=0)), {})
            output = root / "out/research/ntsc-u/matches/func_fixture"
            return report, commands[0], (output / "function.ld").read_text()


class CategoryTests(unittest.TestCase):
    def test_registry_build_validation_preserves_comparison_units(self):
        row = dict(module="ee-main", name="f", kind="function", status="matching",
                   source="src/ee/f.c", address="0x1000", size="4", match_kind="c")
        f = dict(module="ee-main", name="f", source="src/ee/unit.c", address="0x1000",
                 size="0x4", unit_sources=["src/ee/f.c"])
        match.validate_registry_builds([f], [row])
        with self.assertRaises(ValueError):
            match.validate_registry_builds([f], [{**row, "match_kind": "asm"}])
        with self.assertRaises(ValueError):
            match.validate_registry_builds([{**f, "unit_sources": []}], [row])

    def test_progress_keeps_totals_and_separates_c_from_asm(self):
        rows = [dict(kind="function", status="matching", size="0x10", match_kind="c"),
                dict(kind="function", status="matching", size="0x20", match_kind="asm"),
                dict(kind="function", status="nonmatching", size="0x40", match_kind="c"),
                dict(kind="data", status="matching", size="0x80", match_kind="none")]
        r = project.progress(rows)
        self.assertEqual((r["registered_functions"], r["matching_functions"], r["matching_bytes"]), (3, 2, 48))
        self.assertEqual((r["matching_c_functions"], r["matching_c_bytes"]), (1, 16))
        self.assertEqual((r["matching_asm_functions"], r["matching_asm_bytes"]), (1, 32))

    def test_registry_rejects_unclassified_asm_and_incompatible_source(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "f.s").touch()
            (root / "note.md").touch()
            row = dict(module="ee-main", address="0x1000", size="4", name="f", kind="function",
                       status="matching", source="f.s", evidence="note.md", match_kind="asm")
            with patch.object(project, "ROOT", root), patch.object(project, "CONFIG", root):
                for kind in ("asm", "c", "none", "unknown"):
                    row["match_kind"] = kind
                    with (root / "symbols.csv").open("w", newline="") as stream:
                        writer = csv.DictWriter(stream, fieldnames=project.FIELDS)
                        writer.writeheader()
                        writer.writerow(row)
                    if kind == "asm":
                        self.assertEqual(project.symbols()[0]["match_kind"], "asm")
                    else:
                        with self.assertRaises(ValueError):
                            project.symbols()


if __name__ == "__main__":
    unittest.main()
