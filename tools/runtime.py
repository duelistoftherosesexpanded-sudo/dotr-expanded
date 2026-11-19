#!/usr/bin/env python3
"""Paths of the workspace-local tools, and a launcher for the pinned MIPS binutils."""

import argparse
import os
from pathlib import Path
import subprocess
import sys

# DOTR_ROOT: another folder laid out as the project (the player's setup tool's workspace).
ROOT = Path(os.environ["DOTR_ROOT"]) if os.environ.get("DOTR_ROOT") else Path(__file__).resolve().parents[1]
TOOLS = ROOT / ".tooling"
# On Windows the Metrowerks compiler runs directly (no wibo) and the MIPS binutils are a Windows
# build pinned under `windows_artifacts` in config/tooling-lock.json.
WINDOWS = os.name == "nt"
BINUTILS_DIR = TOOLS / ("mips-binutils-windows" if WINDOWS else "mips-binutils/usr/bin")
BINUTILS_PREFIX = "mips-ps2-decompals-" if WINDOWS else "mipsel-linux-gnu-"


def binutils_tool(name):
    return BINUTILS_DIR / (BINUTILS_PREFIX + name + (".exe" if WINDOWS else ""))


def platform_artifacts(lock):
    """The lock's artifacts for this machine, in lock order."""
    artifacts = list(lock["artifacts"])
    if not WINDOWS:
        return artifacts
    replacements = {artifact["tool"]: artifact for artifact in lock.get("windows_artifacts", [])}
    return [replacements.get(artifact["tool"], artifact) for artifact in artifacts if artifact["tool"] != "wibo"]


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("binutils", "doctor"))
    args, extra = parser.parse_known_args(argv)
    if extra[:1] == ["--"]:
        extra = extra[1:]
    try:
        if args.command == "doctor":
            paths = {"MIPS binutils": binutils_tool("objdump"),
                     "wibo": TOOLS / "downloads/wibo-1.2.0-x86_64",
                     "Metrowerks 2.3.3": TOOLS / "compilers/mwcps2-2.3.3-000906/mwccps2.exe"}
            if WINDOWS:
                del paths["wibo"]
            for name, path in paths.items():
                print(f"{name}: {'present' if path.is_file() else 'MISSING'}")
            return int(any(not path.is_file() for path in paths.values()))
        names = {"objdump", "readelf", "nm", "objcopy", "as", "ld", "size", "strings", "addr2line"}
        if not extra or extra[0] not in names:
            raise ValueError("binutils requires a supported tool name followed by its arguments")
        env = os.environ.copy()
        libs = str(TOOLS / "mips-binutils/usr/lib/x86_64-linux-gnu")
        env["LD_LIBRARY_PATH"] = libs + (":" + env["LD_LIBRARY_PATH"] if env.get("LD_LIBRARY_PATH") else "")
        return subprocess.call([str(binutils_tool(extra[0])), *extra[1:]], cwd=ROOT, env=env)
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
