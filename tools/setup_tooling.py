#!/usr/bin/env python3
"""Download, verify and install the pinned build tools (Metrowerks 2.3.3, wibo, MIPS binutils) into .tooling/."""

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import tarfile
import urllib.request
import zipfile

from runtime import ROOT, TOOLS, platform_artifacts


def select_artifacts(artifacts, tool_names=None):
    """Return the named tools' artifacts in lock order; with no names, every artifact."""
    if not tool_names:
        return list(artifacts)
    selected = set(tool_names)
    unknown = selected - {artifact["tool"] for artifact in artifacts}
    if unknown:
        raise ValueError("unknown tool selector: " + ", ".join(sorted(unknown)))
    return [artifact for artifact in artifacts if artifact["tool"] in selected]


def install_standalone_binary(archive, install_path):
    """Copy a verified standalone executable to a file inside .tooling/ (no lock entry uses this yet)."""
    destination = (ROOT / install_path).resolve()
    tooling_root = TOOLS.resolve()
    if destination == tooling_root or not destination.is_relative_to(tooling_root):
        raise ValueError("standalone install_path must be a file within workspace .tooling")
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(archive, destination)
    destination.chmod(0o755)
    return destination


def unzip(archive, destination):
    with zipfile.ZipFile(archive) as stream:
        for entry in stream.infolist():
            target = (destination / entry.filename).resolve()
            if not target.is_relative_to(destination.resolve()):
                raise ValueError("unsafe archive path")
        stream.extractall(destination)
        for entry in stream.infolist():
            mode = entry.external_attr >> 16
            if mode & 0o111:
                (destination / entry.filename).chmod(mode & 0o777)


def untar(archive, destination, strip_components=0):
    """Extract only regular files/directories; do not follow archive links."""
    with tarfile.open(archive, "r:gz") as stream:
        entries = stream.getmembers()
        for entry in entries:
            parts = Path(entry.name).parts
            if ".." in parts or Path(entry.name).is_absolute():
                raise ValueError("unsafe tar archive entry")
            entry.name = str(Path(*parts[strip_components:]))
            target = (destination / entry.name).resolve()
            if not target.is_relative_to(destination.resolve()) or not (entry.isfile() or entry.isdir()):
                raise ValueError("unsafe tar archive entry")
        stream.extractall(destination, members=entries)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--download", action="store_true", help="download missing pinned archives")
    parser.add_argument("--install", action="store_true", help="extract or install the verified archives")
    parser.add_argument("--tool", action="append", metavar="NAME",
                        help="select a pinned tool; repeat to select several (default: all)")
    args = parser.parse_args(argv)
    lock = json.loads((ROOT / "config/tooling-lock.json").read_text())
    try:
        artifacts = select_artifacts(platform_artifacts(lock), args.tool)
    except ValueError as error:
        parser.error(str(error))
    downloads = TOOLS / "downloads"
    downloads.mkdir(parents=True, exist_ok=True)
    for artifact in artifacts:
        archive = downloads / artifact["file"]
        if not archive.is_file():
            if not args.download:
                raise ValueError(f"missing {archive}; use --download")
            print(f"Downloading {artifact['tool']}", flush=True)
            partial = archive.with_suffix(archive.suffix + ".partial")
            with urllib.request.urlopen(artifact["url"], timeout=60) as source, partial.open("wb") as output:
                for chunk in iter(lambda: source.read(1024 * 1024), b""):
                    output.write(chunk)
            partial.rename(archive)
        digest = hashlib.sha256()
        with archive.open("rb") as stream:
            for chunk in iter(lambda: stream.read(1024 * 1024), b""):
                digest.update(chunk)
        if digest.hexdigest() != artifact["sha256"]:
            raise ValueError(f"checksum mismatch: {archive}")
        print(f"Verified {artifact['tool']}")
        if not args.install:
            continue
        if artifact.get("kind") == "standalone-binary":
            install_standalone_binary(archive, artifact["install_path"])
            continue
        name = artifact["tool"]
        destination = TOOLS / artifact.get("install_dir", name)
        destination.mkdir(parents=True, exist_ok=True)
        if archive.suffix == ".zip":
            unzip(archive, destination)
        elif name.startswith("mwcps2-"):
            destination = TOOLS / "compilers" / archive.name.removesuffix(".tar.gz")
            destination.mkdir(parents=True, exist_ok=True)
            untar(archive, destination)
        elif name == "wibo":
            archive.chmod(0o755)
        elif archive.suffix == ".deb":
            subprocess.run(["dpkg-deb", "-x", str(archive), str(destination)], check=True)
        else:
            raise ValueError(f"no install rule for {name}")


if __name__ == "__main__":
    main()
