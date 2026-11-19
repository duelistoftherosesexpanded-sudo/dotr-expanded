#!/usr/bin/env python3
"""Exports Yu-Gi-Oh! Saikyo Card Battle (3DS) monsters to glTF (.glb), like the other games'
exporters (tools/dod_export.py): the skinned mesh, the skeleton, the textures (and the atlas
cards.json's "model_texture" takes) and the motions as clips.

The game has four motions a monster (some have more): A the summon, B and C idles (looping),
D the attack. DotR's motion slots take clips by letter (tools/model_skinned.py CLIP_SLOTS), so
the clips are named <name>_<letter>:
  a summon = A    b idle = B    c attack = D    d set/flip = C
  e damage = C    f attack wind-up = B    g attack return = B
Motions are sampled at 30 frames a second (the game's rate).

  bch_export.py ROMFS_3D/Mo_Kuriboh.bch OUT_FOLDER
"""
import argparse
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import bch_read as br  # noqa: E402
from dod_export import Glb, png_bytes  # noqa: E402

FPS = 30.0
CLIPS = {"a": "MotionA", "b": "MotionB", "c": "MotionD", "d": "MotionC", "e": "MotionC", "f": "MotionB", "g": "MotionB"}


def column_major(m):
    return [m[r][c] for c in range(4) for r in range(4)]


def r9(v):
    return round(v, 9) + 0.0


def pose_world(bones, anim, frame):
    """World matrices of the bones at a frame of an animation (the rest pose without one)."""
    world = [None] * len(bones)
    curves_of = anim["bones"] if anim else {}
    for k, bone in enumerate(bones):
        curves = curves_of.get(bone["name"], {})
        val = lambda ch, i, rest: br.evaluate(curves[ch], frame) if ch in curves else rest[i]
        t = [val("t" + c, i, bone["translation"]) for i, c in enumerate("xyz")]
        r = [val("r" + c, i, bone["rotation"]) for i, c in enumerate("xyz")]
        s = [val("s" + c, i, bone["scale"]) for i, c in enumerate("xyz")]
        local = br.local_matrix(t, r, s)
        world[k] = br.mat_mul(world[bone["parent"]], local) if bone["parent"] >= 0 else local
    return world


def export(path, out_dir, check=False, data=None, dump=False, model_index=0):
    """data: the .bch's bytes when it is not a file (read from the game image).
    dump: everything as the game has it (a model dump, tools/saikyo_dump.py): every triangle (no
    props left out) and every animation as a clip of its own name; otherwise the DotR card set-up
    (CLIPS, hidden props left out). model_index: which model of the file (some hold several)."""
    b = br.Bch(path, data)
    model = b.model(b.dict("models")[model_index])
    base = Path(path).stem + (f"_{model_index}" if model_index else "")
    bones = model["bones"]
    if not bones:                # a static model (arenas, stages): one root joint holds it
        bones = [dict(name=base + "_root", parent=-1, scale=[1.0, 1.0, 1.0], rotation=[0.0, 0.0, 0.0],
                      translation=[0.0, 0.0, 0.0])]
    glb = Glb()
    j = glb.json
    # Skeleton: a node a bone, rest pose from the bone's scale / Euler rotation / translation.
    j["nodes"] = []
    children = {k: [] for k in range(len(bones))}
    for k, bone in enumerate(bones):
        if bone["parent"] >= 0:
            children[bone["parent"]].append(k)
    world = [None] * len(bones)
    for k, bone in enumerate(bones):
        local = br.local_matrix(bone["translation"], bone["rotation"], bone["scale"])
        world[k] = br.mat_mul(world[bone["parent"]], local) if bone["parent"] >= 0 else local
        node = dict(name=bone["name"], translation=[r9(v) for v in bone["translation"]],
                    rotation=[r9(v) for v in br.euler_quat(bone["rotation"])], scale=[r9(v) for v in bone["scale"]])
        if children[k]:
            node["children"] = children[k]
        j["nodes"].append(node)
    roots = [k for k, bone in enumerate(bones) if bone["parent"] < 0]
    ibms = [[r9(v) for v in column_major(br.mat_inverse(w))] for w in world]
    # Textures: one material each.
    textures = b.textures()
    j["images"], j["textures"], j["materials"], j["samplers"] = [], [], [], [dict(magFilter=9729, minFilter=9729)]
    material_of = {}
    for name, (w, h, rows, fmt) in textures.items():
        rgb = [[p[:3] for p in row] for row in rows]
        j["images"].append(dict(name=name + ".png", mimeType="image/png", bufferView=glb.view(png_bytes(w, h, rgb))))
        j["textures"].append(dict(source=len(j["images"]) - 1, sampler=0))
        j["materials"].append(dict(name=name, pbrMetallicRoughness=dict(baseColorTexture=dict(index=len(j["textures"]) - 1),
                                                                         metallicFactor=0.0)))
        material_of[name] = len(j["materials"]) - 1
    report = []
    anims = {a["name"]: a for a in b.skeletal_animations()}
    # Where the idle's first frame puts each vertex (to find parts it parks away from the body).
    idle_world = pose_world(bones, anims.get("MotionB"), 0.0)
    skin_now = [br.mat_mul(idle_world[k], br.mat_inverse(world[k])) for k in range(len(bones))]
    parts = []
    for mesh in model["meshes"]:
        v = mesh["vertices"]
        pos = v.get(br.POSITION)
        if not pos:
            continue
        nrm = v.get(br.NORMAL)
        uv = v.get(br.TEX0) or [[0.0, 0.0]] * len(pos)
        bi, bw = v.get(br.BONE_INDEX), v.get(br.BONE_WEIGHT)
        tex = model["materials"][mesh["material"]]["texture"] if mesh["material"] < len(model["materials"]) else None
        for sub in mesh["submeshes"]:
            if sub["mode"] != 0:
                report.append(f"sub-mesh primitive mode {sub['mode']} skipped")
                continue
            used = sorted(set(sub["indices"]))
            remap = {o: n for n, o in enumerate(used)}
            P, N, T, J, W = [], [], [], [], []
            for o in used:
                p, n = pos[o][:3], (nrm[o][:3] if nrm else None)
                if bi is not None and bw is not None:          # smooth: model space
                    ids = [sub["bones"][x] if x < len(sub["bones"]) else 0 for x in bi[o]]
                    ws = list(bw[o])
                elif bi is not None or sub["bones"]:             # rigid: the bone's space
                    bone = sub["bones"][bi[o][0]] if bi is not None else sub["bones"][0]
                    m = world[bone]
                    p = [sum(m[r][c] * p[c] for c in range(3)) + m[r][3] for r in range(3)]
                    if n:
                        n = [sum(m[r][c] * n[c] for c in range(3)) for r in range(3)]
                    ids, ws = [bone], [1.0]
                else:
                    ids, ws = [mesh["node"] if mesh["node"] < len(bones) else 0], [1.0]
                pairs = sorted(zip(ws, ids), reverse=True)[:4]
                total = sum(w for w, _ in pairs) or 1.0
                pairs += [(0.0, 0)] * (4 - len(pairs))
                P.append([r9(x) for x in p])
                if n:
                    ln = math.sqrt(sum(x * x for x in n)) or 1.0
                    N.append([r9(x / ln) for x in n])
                # The decoded textures are upright (bch_read flips the 3DS's bottom-first rows) and
                # these models' V already counts from the top: kept as it is (checked against
                # Kaibaman's card picture and Gemini Elf in game).
                T.append([r9(uv[o][0]), r9(uv[o][1])])
                J.append([i for _, i in pairs])
                W.append([r9(w / total) for w, _ in pairs])
            idle = []
            for p, jj, ww in zip(P, J, W):
                q = [0.0, 0.0, 0.0]
                for k, w in zip(jj, ww):
                    if w:
                        m = skin_now[k]
                        for r in range(3):
                            q[r] += w * (sum(m[r][c] * p[c] for c in range(3)) + m[r][3])
                idle.append(q)
            tris = [[remap[i] for i in sub["indices"][t:t + 3]] for t in range(0, len(sub["indices"]) - 2, 3)]
            tris = [t for t in tris if len(set(t)) == 3]        # degenerate ones draw nothing
            parts.append(dict(P=P, N=N, T=T, J=J, W=W, idle=idle, tris=tris, tex=tex, smooth=bw is not None))
    # Parts the idle parks far from the body (props held under the floor until a summon shows
    # them, as the Gem-Knights' gems): DotR keeps one pose set, so their triangles are left out.
    # The body: the skinned parts (props are mostly rigid pieces), or everything without any.
    body = [part for part in parts if part["smooth"]] or parts

    def box(points):
        """10% / 90% bounds (a hidden piece can be over 5% of a mesh's points) and the size."""
        lo, hi = [], []
        for i in range(3):
            axis = sorted(q[i] for q in points)
            lo.append(axis[len(axis) // 10] if axis else 0.0)
            hi.append(axis[len(axis) * 9 // 10] if axis else 0.0)
        return lo, hi, max(hi[i] - lo[i] for i in range(3)) if points else 0.0
    # The body at rest (everything in place), and pieces the rest pose already keeps far away
    # (Zubeneschamali's damage eyes).
    rlo, rhi, rsize = box([q for part in body for q in part["P"]])
    away_rest = lambda q: any(q[i] < rlo[i] - 2 * rsize or q[i] > rhi[i] + 2 * rsize for i in range(3))

    # A parked bone: the idle moves it from its parent by more than the body's size beyond its
    # rest distance (gems, kunai, spare propellers, Utopia's folded wing blades); a tail segment
    # keeps its length.
    def gap(m, k):
        p = bones[k]["parent"]
        return math.dist([m[k][i][3] for i in range(3)], [m[p][i][3] for i in range(3)]) if p >= 0 else 0.0
    parked = {k for k in range(len(bones)) if gap(idle_world, k) - gap(world, k) > rsize}

    def held_away(k):
        while k >= 0:
            if k in parked:
                return True
            k = bones[k]["parent"]
        return False
    # The body in the idle: its points not on parked bones. Hidden props are parked several body
    # sizes off (100 units and more); tails and wings that swing out stay within about one.
    lo, hi, size = box([q for part in body for q, jj in zip(part["idle"], part["J"]) if not held_away(jj[0])]
                       or [q for part in body for q in part["idle"]])
    away = lambda q: any(q[i] < lo[i] - 2 * size or q[i] > hi[i] + 2 * size for i in range(3))
    if dump:
        away = lambda q: False
    prims = []
    dropped = 0
    dropped_bones = set()
    for part in parts:
        far = [away(q) for q in part["idle"]]
        # Whole pieces only: a piece (triangles joined by shared points) goes when every point of
        # it is far; a tail's flail swinging out on its chain stays with the chain.
        group = list(range(len(part["idle"])))

        def top(x):
            while group[x] != x:
                group[x] = group[group[x]]
                x = group[x]
            return x
        for t in part["tris"]:
            a = top(t[0])
            for x in t[1:]:
                group[top(x)] = a
        near_piece = {top(x) for x in range(len(far))
                      if not far[x] or not (held_away(part["J"][x][0]) or away_rest(part["P"][x]))}
        # And triangles all on moved-away bones and all far, even joined to the body (Utopia's
        # folded wing blades, held under the floor until the attack).
        parked_far = lambda t: all(far[x] and held_away(part["J"][x][0]) for x in t)
        gone = [t for t in part["tris"] if top(t[0]) not in near_piece or parked_far(t)]
        tris = [t for t in part["tris"] if not (top(t[0]) not in near_piece or parked_far(t))]
        dropped += len(gone)
        dropped_bones.update(bones[part["J"][t[0]][0]]["name"] for t in gone)
        used = sorted({x for t in tris for x in t})
        if not used:
            continue
        renum = {o: n for n, o in enumerate(used)}
        P, N, T, J, W = ([part[k][o] for o in used] if part[k] else [] for k in ("P", "N", "T", "J", "W"))
        idx = [renum[x] for t in tris for x in t]
        tex = part["tex"]
        if True:
            attrs = dict(POSITION=glb.accessor(P, "VEC3", "f", 5126, minmax=True), TEXCOORD_0=glb.accessor(T, "VEC2", "f", 5126),
                         JOINTS_0=glb.accessor(J, "VEC4", "B", 5121), WEIGHTS_0=glb.accessor(W, "VEC4", "f", 5126))
            if len(N) == len(P):
                attrs["NORMAL"] = glb.accessor(N, "VEC3", "f", 5126)
            prim = dict(attributes=attrs, indices=glb.accessor(idx, "SCALAR", "I", 5125), mode=4)
            if tex in material_of:
                prim["material"] = material_of[tex]
            prims.append(prim)
            report.append(f"mesh {tex}: {len(P)} vertices, {len(idx) // 3} triangles, "
                          f"{'smooth' if part['smooth'] else 'rigid'}")
    if dropped:
        report.append(f"{dropped} triangles the idle parks away from the body left out "
                      f"(on {', '.join(sorted(dropped_bones))})")
    j["meshes"] = [dict(name=base, primitives=prims)]
    holder = len(j["nodes"])
    j["nodes"].append(dict(name=base + "_mesh", mesh=0, skin=0))
    j["skins"] = [dict(joints=list(range(len(bones))), inverseBindMatrices=glb.accessor(ibms, "MAT4", "f", 5126),
                       skeleton=roots[0])]
    j["scenes"] = [dict(nodes=roots + [holder])]
    j["scene"] = 0
    # Motions.
    index = {bone["name"]: k for k, bone in enumerate(bones)}
    j["animations"] = []
    sampled = {}
    clips = {name: name for name in anims} if dump else CLIPS
    for letter, motion in clips.items():
        a = anims.get(motion) or anims.get("MotionB") or next(iter(anims.values()), None)
        if a is None:
            continue
        if a["name"] not in sampled:
            frames = max(2, int(math.floor(a["frames"])) + 1)
            times = [r9(f / FPS) for f in range(frames)]
            tracks = []
            for bone_name, curves in a["bones"].items():
                k = index.get(bone_name)
                if k is None:
                    continue
                rest = bones[k]
                tr, ro, sc = [], [], []
                last = None
                for f in range(frames):
                    val = lambda ch, i, rest_v: br.evaluate(curves[ch], f) if ch in curves else rest_v[i]
                    t = [val("t" + c, i, rest["translation"]) for i, c in enumerate("xyz")]
                    r = [val("r" + c, i, rest["rotation"]) for i, c in enumerate("xyz")]
                    s = [val("s" + c, i, rest["scale"]) for i, c in enumerate("xyz")]
                    q = list(br.euler_quat(r))
                    if last and sum(x * y for x, y in zip(q, last)) < 0:
                        q = [-x for x in q]
                    last = q
                    tr.append([r9(x) for x in t])
                    ro.append([r9(x) for x in q])
                    sc.append([r9(x) for x in s])
                tracks.append((k, tr, ro, sc))
            sampled[a["name"]] = (times, tracks)
        times, tracks = sampled[a["name"]]
        t_acc = glb.accessor(times, "SCALAR", "f", 5126, minmax=True)
        channels, samplers = [], []
        for k, tr, ro, sc in tracks:
            for path_, values, kind in (("translation", tr, "VEC3"), ("rotation", ro, "VEC4"), ("scale", sc, "VEC3")):
                samplers.append(dict(input=t_acc, output=glb.accessor(values, kind, "f", 5126), interpolation="LINEAR"))
                channels.append(dict(sampler=len(samplers) - 1, target=dict(node=k, path=path_)))
        j["animations"].append(dict(name=f"{base}_{letter}", channels=channels, samplers=samplers))
    report.append("clips " + ", ".join(f"{l}={clips[l]}" for l in clips) + f" (motions: {', '.join(anims)})")
    if not j["animations"]:
        del j["animations"]
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    out = out_dir / f"{base}.glb"
    glb.write(out)
    import glb_anim
    if j["images"]:
        glb_anim.atlas(out, out_dir / f"{base}_atlas.png")
    return out, report


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("bch")
    p.add_argument("out")
    a = p.parse_args()
    out, report = export(a.bch, a.out)
    print(out)
    print("\n".join(report))


if __name__ == "__main__":
    main()
