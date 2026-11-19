"""ROM-free checks for the library-block assembly renderer."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import library_asm


ROWS = {
    0x1000: (0x24030000, "addiu", "v1,zero,0"),
    0x1004: (0x10800001, "beq", "a0,zero,100c"),
    0x1008: (0x0c040000, "jal", "100000"),
    0x100c: (0x10000000, "b", "2000"),
    0x1010: (0x0000000d, "break", "0x7"),
}


class RenderTests(unittest.TestCase):
    def test_registers_labels_jumps_and_raw_words(self):
        text, where = library_asm.render(ROWS, 0x1000, 0x1014, {0x1010})
        body = text.splitlines()
        self.assertEqual(body[:4], [".set noreorder", ".set noat", ".set nomacro", ".text"])
        self.assertIn(".space 0", body)
        self.assertIn("    addiu $v1,$zero,0", body)
        self.assertIn("    beq $a0,$zero,L_0000100c", body)
        self.assertIn("    jal 0x100000", body)
        self.assertIn("L_0000100c:", body)
        self.assertIn("    .word 0x10000000", body)  # branch leaving the block stays raw
        self.assertIn("    .word 0x0000000d", body)  # word the assembler rejected
        self.assertEqual(body[next(n for n, a in where.items() if a == 0x1010) - 1], "    .word 0x0000000d")

    def test_unaligned_start_is_padded_to_the_section_boundary(self):
        text, _ = library_asm.render({0x100c: (0, "sll", "zero,zero,0x0")}, 0x100c, 0x1010, set())
        self.assertIn(".space 12", text.splitlines())


if __name__ == "__main__":
    unittest.main()
