#!/usr/bin/env python3
"""Yu-Gi-Oh! Saikyo Card Battle's monster table, from its program (.code, tools/ctr_read.py):
which model, effect sequence and sounds each card (by Konami id) uses, and its display scale.

The table is rows of 20 bytes: Konami id (u32), scale (float), and pointers (program address,
0x100000 + offset) to the UTF-16 paths of the model (3D/Mo_*.bch or 3D/MBB_*.bch), its effect
sequence (Efseq/) and its sounds (sound/AnimSD/). It is found by Kuriboh's row (Konami id 4064)
and read in both directions while rows stay valid.

  saikyo_table.py GAME.cci [OUT.json]      (default build/saikyo/model_table.json)
"""
import json
import struct
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
ROOT = TOOLS.parent
BASE = 0x100000
ANCHOR = "3D/Mo_Kuriboh.bch"


def table(code):
    def text(va):
        o = va - BASE
        if not 0 <= o < len(code) - 1:
            return None
        e = o
        while code[e:e + 2] != b"\0\0":
            e += 2
            if e - o > 400:
                return None
        return code[o:e].decode("utf-16-le", "replace")

    def row(at):
        if at < 0 or at + 20 > len(code):
            return None
        cid, scale, model, efseq, sound = struct.unpack_from("<IfIII", code, at)
        path = text(model)
        if not path or not path.startswith("3D/") or not 0 < cid < 100000:
            return None
        return dict(cid=cid, scale=round(scale, 4), model=path, efseq=text(efseq), sound=text(sound))

    s = code.find(ANCHOR.encode("utf-16-le"))
    ref = code.find(struct.pack("<I", BASE + s))
    if s < 0 or ref < 0:
        raise ValueError("the monster table was not found (not Saikyo Card Battle?)")
    at = ref - 8
    while row(at - 20):
        at -= 20
    rows = []
    while row(at):
        rows.append(row(at))
        at += 20
    return rows


def main():
    import ctr_read
    import dod_models
    rows = table(ctr_read.Cci(sys.argv[1]).code())
    names = {int(k): v["name"] for k, v in dod_models.konami_names().items()}
    for r in rows:
        r["name"] = names.get(r["cid"], "?")
    out = Path(sys.argv[2]) if len(sys.argv) > 2 else ROOT / "build" / "saikyo" / "model_table.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(rows, indent=1, ensure_ascii=False), encoding="utf-8")
    print(f"{len(rows)} rows ({sum(1 for r in rows if '/Mo_' in r['model'])} 3D monsters) -> {out}")


if __name__ == "__main__":
    main()
