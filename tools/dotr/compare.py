"""Exact byte comparisons: no relocation masks or ignored instructions."""

import hashlib


def compare_bytes(original, rebuilt):
    differences = [i for i in range(min(len(original), len(rebuilt))) if original[i] != rebuilt[i]]
    differences.extend(range(min(len(original), len(rebuilt)), max(len(original), len(rebuilt))))
    return {
        "matching": original == rebuilt,
        "original_size": len(original), "rebuilt_size": len(rebuilt),
        "original_sha256": hashlib.sha256(original).hexdigest(),
        "rebuilt_sha256": hashlib.sha256(rebuilt).hexdigest(),
        "different_bytes": len(differences), "first_differences": differences[:32],
    }
