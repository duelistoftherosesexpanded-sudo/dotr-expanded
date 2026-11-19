"""ROM-free checks for selecting and installing pinned tools."""

import hashlib
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import setup_tooling


class ToolingSelectionTests(unittest.TestCase):
    def setUp(self):
        # The installer prints a "Verified <tool>" line per artifact; keep it out of the test output.
        quiet = patch("sys.stdout", new_callable=io.StringIO)
        quiet.start()
        self.addCleanup(quiet.stop)

    def test_selection_preserves_lock_order_and_all_artifacts_for_tool(self):
        artifacts = [{"tool": "first"}, {"tool": "second"}, {"tool": "first"}]
        self.assertEqual(setup_tooling.select_artifacts(artifacts), artifacts)
        self.assertEqual(setup_tooling.select_artifacts(artifacts, ["first", "first"]),
                         [artifacts[0], artifacts[2]])
        self.assertEqual(setup_tooling.select_artifacts(artifacts, ["second", "first"]), artifacts)

    def test_unknown_selector_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "unknown tool selector: missing"):
            setup_tooling.select_artifacts([{"tool": "known"}], ["missing"])

    def test_standalone_copy_and_executable_mode(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            archive = root / "download"
            archive.write_bytes(b"pinned executable")
            with patch.object(setup_tooling, "ROOT", root), \
                    patch.object(setup_tooling, "TOOLS", root / ".tooling"):
                installed = setup_tooling.install_standalone_binary(archive, ".tooling/objdiff/cli")
            self.assertEqual(installed.read_bytes(), archive.read_bytes())
            if os.name != "nt":  # Windows has no executable permission bits
                self.assertEqual(installed.stat().st_mode & 0o777, 0o755)
                self.assertEqual(archive.stat().st_mode & 0o111, 0)

    def test_standalone_rejects_escape_and_symlink_parent(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            archive = root / "download"
            archive.write_bytes(b"executable")
            tooling = root / ".tooling"
            tooling.mkdir()
            outside = root / "outside"
            outside.mkdir()
            try:
                (tooling / "escape").symlink_to(outside, target_is_directory=True)
            except OSError:  # Windows without the symlink privilege
                self.skipTest("cannot create symbolic links here")
            with patch.object(setup_tooling, "ROOT", root), patch.object(setup_tooling, "TOOLS", tooling):
                for path in [".tooling", ".tooling/../bad", outside / "bad", ".tooling/escape/bad"]:
                    with self.subTest(path=path), self.assertRaisesRegex(ValueError, "within workspace .tooling"):
                        setup_tooling.install_standalone_binary(archive, path)
            self.assertFalse((outside / "bad").exists())
            self.assertFalse((root / "bad").exists())

    def test_selected_binary_install_does_not_configure_other_tools(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            tooling = root / ".tooling"
            (root / "config").mkdir()
            (tooling / "downloads").mkdir(parents=True)
            contents = b"selected executable"
            (tooling / "downloads/selected").write_bytes(contents)
            artifacts = [{"tool": "objdiff", "file": "selected", "kind": "standalone-binary",
                          "install_path": ".tooling/objdiff/cli", "sha256": hashlib.sha256(contents).hexdigest()},
                         {"tool": "wibo", "file": "missing"}]
            (root / "config/tooling-lock.json").write_text(json.dumps({"artifacts": artifacts}))
            with patch.object(setup_tooling, "ROOT", root), patch.object(setup_tooling, "TOOLS", tooling), \
                    patch.object(setup_tooling.subprocess, "run") as run:
                setup_tooling.main(["--install", "--tool", "objdiff", "--tool", "objdiff"])
            run.assert_not_called()
            self.assertEqual((tooling / "objdiff/cli").read_bytes(), contents)


if __name__ == "__main__":
    unittest.main()
