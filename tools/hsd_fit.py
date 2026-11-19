#!/usr/bin/env python3
"""Fits the pieces of an HSD model (tools/hsd_anim.py) onto a reference model of the same monster,
for rips whose own pose has parts out of place (Falsebound Kingdom's Valkyrion: hanging wings,
swung-out arms): writes, per joint, a correction that tools/hsd_anim.load applies (its `fit`
argument; the card field "model_fit") to the vertices that joint drives, so the pieces sit as in
the reference in the first frame of the idle and still follow their joints in every motion.

  hsd_fit.py MODEL.dat REFERENCE(.pmx|.obj|.fbx) REFERENCE_TEXTURE.png OUT.json
             [--texture MODEL_ATLAS.png] [--skip J ...] [--min-vertices 30]

How: the model is posed in the first frame of its first clip; each vertex belongs to the joint
that weighs it most (a piece). The reference is scaled and moved onto the model (bounding
heights, then an iterative closest point fit of everything). Each piece is then fitted alone, a
rotation and a move (iterative closest point from a few starting turns, matching texture colour as
well as position so a wing goes to the reference's wing), and kept if it lies closer than
before. The correction is stored in bind space: B * W0^-1 * C * W0 * B^-1, with W0 the joint's
world matrix in that frame and B its bind matrix.
"""

import argparse
import json
import math
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import fbx_anim as fa  # noqa: E402
import hsd_anim as ha  # noqa: E402
import model_import as mi  # noqa: E402
import model_texture as mt  # noqa: E402

COLOUR_WEIGHT = 0.15     # model units per colour step (0-1 per channel)


# ---- small linear algebra ----------------------------------------------------------------------
def mat_mul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


def apply(m, p):
    return tuple(m[i][0] * p[0] + m[i][1] * p[1] + m[i][2] * p[2] + m[i][3] for i in range(3))


def rotation(axis, angle):
    c, s = math.cos(angle), math.sin(angle)
    x, y, z = axis
    return [[c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0],
            [y * x * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s, 0],
            [z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, c + z * z * (1 - c), 0],
            [0, 0, 0, 1]]


def translation(t):
    return [[1, 0, 0, t[0]], [0, 1, 0, t[1]], [0, 0, 1, t[2]], [0, 0, 0, 1]]


def best_rigid(src, dst):
    """The rotation and move taking points `src` closest to `dst` (Horn's quaternion method)."""
    n = len(src)
    cs = [sum(p[i] for p in src) / n for i in range(3)]
    cd = [sum(p[i] for p in dst) / n for i in range(3)]
    s = [[0.0] * 3 for _ in range(3)]
    for p, q in zip(src, dst):
        a = [p[i] - cs[i] for i in range(3)]
        b = [q[i] - cd[i] for i in range(3)]
        for i in range(3):
            for j in range(3):
                s[i][j] += a[i] * b[j]
    sxx, sxy, sxz = s[0]
    syx, syy, syz = s[1]
    szx, szy, szz = s[2]
    nm = [[sxx + syy + szz, syz - szy, szx - sxz, sxy - syx],
          [syz - szy, sxx - syy - szz, sxy + syx, szx + sxz],
          [szx - sxz, sxy + syx, -sxx + syy - szz, syz + szy],
          [sxy - syx, szx + sxz, syz + szy, -sxx - syy + szz]]
    shift = sum(abs(v) for row in nm for v in row) + 1.0
    q = [1.0, 0.0, 0.0, 0.0]
    for _ in range(60):   # power iteration on N + shift * I: the largest eigenvector
        q2 = [sum(nm[i][j] * q[j] for j in range(4)) + shift * q[i] for i in range(4)]
        length = math.sqrt(sum(v * v for v in q2)) or 1.0
        q = [v / length for v in q2]
    w, x, y, z = q
    r = [[w * w + x * x - y * y - z * z, 2 * (x * y - w * z), 2 * (x * z + w * y)],
         [2 * (x * y + w * z), w * w - x * x + y * y - z * z, 2 * (y * z - w * x)],
         [2 * (x * z - w * y), 2 * (y * z + w * x), w * w - x * x - y * y + z * z]]
    t = [cd[i] - sum(r[i][k] * cs[k] for k in range(3)) for i in range(3)]
    return [r[0] + [t[0]], r[1] + [t[1]], r[2] + [t[2]], [0, 0, 0, 1]]


# ---- nearest points (k-d tree over position + scaled colour) ------------------------------------
class Tree:
    def __init__(self, points):
        self.points = points
        self.root = self._build(list(range(len(points))), 0)

    def _build(self, idx, depth):
        if not idx:
            return None
        k = depth % len(self.points[0])
        idx.sort(key=lambda i: self.points[i][k])
        mid = len(idx) // 2
        return (idx[mid], k, self._build(idx[:mid], depth + 1), self._build(idx[mid + 1:], depth + 1))

    def nearest(self, p):
        best = [None, float("inf")]

        def visit(node):
            if node is None:
                return
            i, k, left, right = node
            q = self.points[i]
            d = sum((a - b) ** 2 for a, b in zip(p, q))
            if d < best[1]:
                best[0], best[1] = i, d
            diff = p[k] - q[k]
            first, second = (left, right) if diff < 0 else (right, left)
            visit(first)
            if diff * diff < best[1]:
                visit(second)

        visit(self.root)
        return best[0], best[1]


def colour_at(tex, uv, flip_v):
    if tex is None:
        return (0.5, 0.5, 0.5)
    w, h, rows = tex
    u, v = uv
    v = 1.0 - v if flip_v else v
    px = rows[min(h - 1, max(0, int((v % 1.0) * h)))][min(w - 1, max(0, int((u % 1.0) * w)))]
    return tuple(c / 255.0 for c in px[:3])


def vertex_colours(positions, coords, triangles, tex, flip_v):
    out = [None] * len(positions)
    for tri in triangles:
        for pi, ci in tri:
            if out[pi] is None and ci >= 0:
                out[pi] = colour_at(tex, coords[ci], flip_v)
    return [c or (0.5, 0.5, 0.5) for c in out]


def feature(p, c):
    return (p[0], p[1], p[2], c[0] * COLOUR_WEIGHT * 10, c[1] * COLOUR_WEIGHT * 10, c[2] * COLOUR_WEIGHT * 10)


def icp(points, colours, tree, ref, start, iterations=25, trim=0.8):
    m = start
    err = float("inf")
    for _ in range(iterations):
        moved = [apply(m, p) for p in points]
        pairs = []
        for p, c in zip(moved, colours):
            j, d = tree.nearest(feature(p, c))
            pairs.append((d, p, ref[j]))
        pairs.sort(key=lambda t: t[0])
        keep = pairs[:max(3, int(len(pairs) * trim))]
        err = sum(d for d, _, _ in keep) / len(keep)
        step = best_rigid([p for _, p, _ in keep], [q for _, _, q in keep])
        m = mat_mul(step, m)
    return m, err


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("model")
    parser.add_argument("reference")
    parser.add_argument("reference_texture")
    parser.add_argument("out")
    parser.add_argument("--texture", help="the model's packed texture (hsd_anim.atlas)")
    parser.add_argument("--skip", type=int, nargs="*", default=[])
    parser.add_argument("--min-vertices", type=int, default=30)
    parser.add_argument("--sample", type=int, default=160)
    args = parser.parse_args(argv)

    model = ha.load(args.model, fit=None)
    clip = model["clips"][0] if model["clips"] else None
    pos, coords, tris = fa.posed_mesh(model, clip, 0.0)
    tex = mt.read_png(args.texture) if args.texture else None
    colours = vertex_colours(pos, coords, tris, tex, True)

    rpos, rcoords, rtris = mi.load_mesh(args.reference)
    rtex = mt.read_png(args.reference_texture)
    rcol = vertex_colours(rpos, rcoords, rtris, rtex, True)
    # denser reference: triangle centres too
    for tri in rtris:
        a, b, c = (rpos[t[0]] for t in tri)
        rpos.append(tuple((a[i] + b[i] + c[i]) / 3 for i in range(3)))
        rcol.append(rcol[tri[0][0]])

    # Pieces: each vertex to the joint weighing it most.
    owner = {}
    for b, pairs in model["skin"].items():
        for v, w in pairs:
            if v not in owner or w > owner[v][1]:
                owner[v] = (b, w)
    pieces = {}
    for v, (b, _) in owner.items():
        pieces.setdefault(b, []).append(v)

    # Reference onto the model: heights, centres, then ICP over the big pieces.
    def span(points, axis, lo=0.05, hi=0.95):
        vals = sorted(p[axis] for p in points)
        return vals[int(lo * (len(vals) - 1))], vals[int(hi * (len(vals) - 1))]
    m_lo, m_hi = span(pos, 1)
    r_lo, r_hi = span(rpos, 1)
    scale = (m_hi - m_lo) / ((r_hi - r_lo) or 1.0)
    mc = [sorted(p[i] for p in pos)[len(pos) // 2] for i in range(3)]
    rc = [sorted(p[i] for p in rpos)[len(rpos) // 2] for i in range(3)]
    ref = [tuple((p[i] - rc[i]) * scale + mc[i] for i in range(3)) for p in rpos]
    # refine: move the model's big pieces onto the reference, then take the inverse for the reference
    big = [v for b, vs in pieces.items() if len(vs) >= 200 for v in vs[::7]]
    tree = Tree([feature(p, c) for p, c in zip(ref, rcol)])
    whole, err = icp([pos[v] for v in big], [colours[v] for v in big], tree, ref,
                     [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], iterations=30, trim=0.6)
    inv_whole = fa._invert(whole)
    ref = [apply(inv_whole, p) for p in ref]
    tree = Tree([feature(p, c) for p, c in zip(ref, rcol)])
    print(f"reference scaled by {scale:.3f}, whole-model fit error {err:.3f}")

    # Each piece alone.
    worlds0 = fa.world(model, fa.pose(model, clip, 0.0))
    starts = [None] + [(axis, angle) for axis in ((1, 0, 0), (0, 1, 0), (0, 0, 1))
                       for angle in (math.pi / 2, -math.pi / 2, math.pi)]
    out = {}
    for b, vs in sorted(pieces.items(), key=lambda kv: -len(kv[1])):
        if len(vs) < args.min_vertices or b in args.skip:
            continue
        step = max(1, len(vs) // args.sample)
        pts = [pos[v] for v in vs[::step]]
        cols = [colours[v] for v in vs[::step]]
        centre = [sum(p[i] for p in pts) / len(pts) for i in range(3)]
        identity = [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]
        _, before = icp(pts, cols, tree, ref, identity, iterations=1, trim=0.8)
        best = (before, identity)
        for s in starts:
            start = identity if s is None else mat_mul(translation(centre),
                                                       mat_mul(rotation(*s), translation([-c for c in centre])))
            m, err = icp(pts, cols, tree, ref, start)
            if err < best[0]:
                best = (err, m)
        err, c = best
        moved = sum(abs(c[i][3]) for i in range(3)) + sum(abs(c[i][j] - (i == j)) for i in range(3) for j in range(3))
        if err < before * 0.7 and moved > 1e-3:
            w0 = worlds0[b]
            bind = model["bind"][b]
            m_bind = mat_mul(bind, mat_mul(fa._invert(w0), mat_mul(c, mat_mul(w0, fa._invert(bind)))))
            out[str(b)] = m_bind
            print(f"joint {b:3d}: {len(vs):4d} vertices, error {before:.3f} -> {err:.3f}  fitted")
        else:
            print(f"joint {b:3d}: {len(vs):4d} vertices, error {before:.3f} -> {err:.3f}  left as it is")
    Path(args.out).write_text(json.dumps(out, indent=1))
    print("wrote", args.out, len(out), "pieces moved")
    return 0


if __name__ == "__main__":
    sys.exit(main())
