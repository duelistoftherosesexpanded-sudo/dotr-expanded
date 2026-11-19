#!/usr/bin/env python3
"""Builds a monster model slot with a skinned mesh, a skeleton and motions of its own from an FBX
with animation clips (tools/fbx_anim.py), on a donor's slot (2 MiB, tools/dlc_cards.py).

Kept from the donor: the header, the textures (the first replaced by the card's), the draw list's
first texture and material records, the blocks after its motions (effect textures, sound cues),
and its place in the game (battle code, sounds, effects). New, written to the free end of the slot
(segment layout as tools/motion_research.py documents it):

  segment 0  the node tree: a root carrying the model's scale, then the FBX bones that move
             vertices and their ancestors, in walk order; every bone has a matrix
  segment 1  the draw list: the jump over the matrix area, the matrix area (7 quadwords a bone),
             the texture and material records, then per batch up to 8 matrix records and one
             reference to the batch's strips; segment 2 follows it directly, as in the game's
  segment 2  the strips: weighted (up to 3 bones a point), in VU-sized chunks
  segments 6 and 8   the address lists the game rebases on load (the jump and matrix records;
             the strip references), each ending with 0xffffffff
  motions    one per DotR motion slot from the FBX clips, sampled at 60 frames a second with the
             keys a straight line would give dropped; then the donor's later blocks, moved along

Clips go to motion slots by CLIP_SLOTS (slot: clip letter), changeable per card.

  model_skinned.py DONOR FBX [--texture PNG] --out SLOT.bin
"""

import argparse
import math
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import fbx_anim as fa  # noqa: E402
import model_import as mi  # noqa: E402
import model_research as mr  # noqa: E402
import model_texture as mt  # noqa: E402
import motion_research as mo  # noqa: E402

CLIP_SLOTS = {0: "b", 1: "a", 2: "c", 3: "d", 4: "e", 5: "g", 6: "f"}
FPS = 60.0
VU_MATRICES = [4, 11, 18, 25, 32, 39, 46, 53]
CHUNK_QUADWORDS = 200      # strip data a VU chunk (the game's own stay under about 216)
STRIP_POINTS = 16
MAX_WEIGHTS = 3
ROT_TOLERANCE = 48         # BAMS (65536 to a turn)
POS_TOLERANCE = 0.002      # of the model's height


def invert(m):
    a = [row[:3] for row in m[:3]]
    det = (a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
           + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]))
    i3 = [[(a[(j + 1) % 3][(i + 1) % 3] * a[(j + 2) % 3][(i + 2) % 3]
            - a[(j + 1) % 3][(i + 2) % 3] * a[(j + 2) % 3][(i + 1) % 3]) / det for j in range(3)] for i in range(3)]
    t = [m[i][3] for i in range(3)]
    return [i3[i] + [-sum(i3[i][k] * t[k] for k in range(3))] for i in range(3)] + [[0, 0, 0, 1]]


def apply(m, p):
    return [m[i][0] * p[0] + m[i][1] * p[1] + m[i][2] * p[2] + m[i][3] for i in range(3)]


def bams(degrees):
    return int(round(degrees * 65536.0 / 360.0))


def clamp16(v):
    return max(-32767, min(32767, int(round(v))))


def simplify(keys, tolerance):
    """The keys (frame, x, y, z) a straight line between kept keys does not give within
    `tolerance`; the first and last always stay."""
    if len(keys) <= 2:
        return keys
    if all(max(abs(k[i] - keys[0][i]) for i in (1, 2, 3)) <= tolerance for k in keys):
        return [keys[0], keys[-1]]
    kept, start = [0], 0
    for i in range(2, len(keys)):
        a, b = keys[start], keys[i]
        for j in range(start + 1, i):
            t = (keys[j][0] - a[0]) / ((b[0] - a[0]) or 1)
            if any(abs(a[c] + (b[c] - a[c]) * t - keys[j][c]) > tolerance for c in (1, 2, 3)):
                kept.append(i - 1)
                start = i - 1
                break
    kept.append(len(keys) - 1)
    if len(kept) == 2:
        # The game reads a track of fewer than three keys as its first key alone (no
        # interpolation), so a straight-line track keeps a middle key.
        kept.insert(1, len(keys) // 2)
    return [keys[i] for i in kept]


def unwrap(values):
    out = [values[0]]
    for v in values[1:]:
        out.append(out[-1] + ((v - out[-1] + 0x8000) % 0x10000 - 0x8000))
    return out


def split_strip(points, limit):
    """Pieces of at most `limit` points keeping each triangle's winding (each starts at an even
    point of the original and overlaps the previous one by two)."""
    if len(points) <= limit:
        return [points]
    limit -= limit % 2
    pieces, start = [], 0
    while start < len(points) - 2:
        pieces.append(points[start:start + limit])
        start += limit - 2
    return pieces


def motion_file(template, node_tracks, frames, kind):
    """A MOT file. `node_tracks`: per node (position keys or None, rotation keys or None, scale
    keys or None); `template`: the bytes of one of the donor's motions (header words kept)."""
    tracks = 3 if kind == 7 else 2
    head = bytearray(template[:0x7c])
    count = len(node_tracks)
    table = 0x7c
    body = bytearray()
    entries = []
    at = table + 8 * tracks * count
    for keys in node_tracks:
        offsets, counts = [], []
        for t in range(tracks):
            k = keys[t]
            if not k:
                offsets.append(0)
                counts.append(0)
                continue
            offsets.append(at + len(body))
            counts.append(len(k))
            for key in k:
                if t == 1:
                    body += struct.pack("<I3i", key[0], *(int(round(v)) for v in key[1:]))
                else:
                    body += struct.pack("<I3f", key[0], *key[1:])
        entries.append(struct.pack(f"<{tracks}I{tracks}I", *offsets, *counts))
    out = head + b"".join(entries) + body
    out += bytes(-len(out) % 16)
    struct.pack_into("<I", out, 4, len(out))
    struct.pack_into("<H", out, 0x10, count)
    struct.pack_into("<II", out, 0x14, 0x6c, 0x1c)
    struct.pack_into("<IIH", out, 0x6c, table, frames, kind)
    return bytes(out)


class TooBig(ValueError):
    def __init__(self, need, have, strip_bytes, triangles):
        super().__init__(f"the model needs {need} bytes; the slot has {have}")
        self.need, self.have, self.strip_bytes, self.triangles = need, have, strip_bytes, triangles


def build(donor, path, texture=None, fill=1.0, clip_slots=None, joint_scale=True, nested_roots=True, fit=None):
    """(the slot, a report). A mesh too big for the slot is reduced (model_import.decimate, each
    remaining point keeping its skin weights) until it fits. `joint_scale` False drops the joints'
    own rest scales (node flag 4) unless an animation scales; `nested_roots` goes to
    hsd_anim.load."""
    if str(path).lower().endswith(".rig.json"):
        import rig_transfer      # a static mesh put on another model's skeleton and clips
        model = rig_transfer.load(path)
    elif str(path).lower().endswith(".dae"):
        import dae_anim
        model = dae_anim.load(path)
    elif str(path).lower().endswith(".dat"):
        import hsd_anim
        model = hsd_anim.load(path, nested_roots=nested_roots, fit=fit)
    elif str(path).lower().endswith(".glb"):
        import glb_anim          # Cross Duel exports (tools/crossduel_export.py)
        model = glb_anim.load(path)
    else:
        model = fa.load(path)
    model["joint_scale"] = joint_scale
    limit = None
    for _ in range(8):
        try:
            slot, report = build_once(donor, model, texture, fill, clip_slots, limit)
            # Stand the idle pose (motion 0, first frame), not the bind pose, on the donor's lowest
            # point: Duel Links idles often lift the body well above the bind pose.
            found = dict(mo.motions(slot))
            if 0 in found:
                idle = mo.read_motion(slot, found[0])
                low = min(p[1] for p in mo.posed_mesh(slot, mo.posed_matrices(slot, idle, 0))[0])
                donor_low = min(v[1] for v in mr.mesh(donor)[0])
                drop = (low - donor_low) / (report["scale"] * report["unit"])
                if abs(low - donor_low) > 0.01:
                    slot, report = build_once(donor, model, texture, fill, clip_slots, limit, drop)
            report["source_triangles"] = len(model["mesh"][2])
            return slot, report
        except TooBig as error:
            spare = error.strip_bytes - (error.need - error.have)
            limit = int(error.triangles * max(0.1, spare / error.strip_bytes) * 0.95)
    raise ValueError("the mesh could not be reduced enough to fit")


def build_once(donor, model, texture, fill, clip_slots, max_triangles, drop=0.0):
    clip_slots = clip_slots or CLIP_SLOTS
    bones = model["bones"]
    positions, coords, triangles = model["mesh"]
    segs = mr.segments(donor)
    data = bytearray(donor)

    # Weights: up to three bones a vertex.
    weights = [[] for _ in positions]
    for b, pairs in model["skin"].items():
        for v, w in pairs:
            if w > 0:
                weights[v].append((w, b))
    for v in range(len(weights)):
        top = sorted(weights[v], reverse=True)[:MAX_WEIGHTS]
        total = sum(w for w, _ in top)
        weights[v] = [(w / total, b) for w, b in top] if total else []
    triangles = [t for t in triangles if all(weights[c[0]] for c in t)]
    if max_triangles and len(triangles) > max_triangles:
        mapping = []
        positions, coords, triangles = mi.decimate(positions, coords, triangles, max_triangles, mapping)
        weights = [weights[old] for old in mapping]
    skinned = {b for ws in weights for _, b in ws}
    if not skinned:
        raise ValueError("the FBX mesh has no skin weights")

    # Bones kept: those that move vertices and their ancestors, in DotR walk order.
    keep = set()
    for b in skinned:
        while b >= 0 and b not in keep:
            keep.add(b)
            b = bones[b]["parent"]
    kids = {b: [] for b in keep}
    tops = []
    for b in sorted(keep):
        (kids[bones[b]["parent"]] if bones[b]["parent"] in keep else tops).append(b)
    walk = []

    def visit(b):
        walk.append(b)
        for c in kids[b]:
            visit(c)

    for t in tops:
        visit(t)
    node_of = {b: i + 1 for i, b in enumerate(walk)}     # node 0 is the root

    # Units: positions are 16-bit in each bone's space; `unit` stored units to one FBX unit.
    inv_bind = {}
    for b in skinned:
        if b not in model["bind"]:
            raise ValueError(f"bone {bones[b]['name']} has weights but no bind matrix")
        inv_bind[b] = invert(model["bind"][b])
    reach = max(abs(c) for v, ws in enumerate(weights) for _, b in ws for c in apply(inv_bind[b], positions[v]))
    unit = 30000.0 / (reach or 1.0)
    ys = [p[1] for p in positions]
    height = (max(ys) - min(ys)) or 1.0
    donor_vertices = mr.mesh(donor)[0]
    donor_lo = min(v[1] for v in donor_vertices)
    donor_height = (max(v[1] for v in donor_vertices) - donor_lo) or 1.0
    scale = fill * donor_height / (height * unit)
    lift = donor_lo / (scale * unit) - min(ys) - drop     # FBX units, added to the top bones' y

    # Normals, then in each bone's space.
    smooth = mi.normals(positions, triangles)

    # Segment 0: the nodes (64 bytes: scale, flags, matrix, rotation, child, translation, next,
    # bounding sphere: centre and radius).
    centre = [(max(p[i] for p in positions) + min(p[i] for p in positions)) / 2 for i in range(3)]
    radius = max(math.dist(p, centre) for p in positions) * 1.5
    rest_world = fa.world(model, [(b["t"], b["r"], b["s"]) for b in bones])
    scaled_anim = any(curve and any(abs(v - 1.0) > 1e-3 for v in curve[1])
                      for clip in model["clips"] for tr in clip["tracks"].values() for curve in tr.get("S", []))
    # Node flag 4 makes the game ignore the node's own scale: only when nothing is scaled.
    scaled_rest = model.get("joint_scale", True) and any(abs(v - 1.0) > 1e-3 for b in walk for v in bones[b]["s"])
    flags = 0 if scaled_anim or scaled_rest else 4
    seg0 = bytearray(64 * (len(walk) + 1))
    # The root's sphere is in world units (RhModel_Draw uses it without the root's scale); the
    # other nodes' centres are in their stored units and their radii in world units (the
    # culling test multiplies them by the accumulated scale and the header's rate, 1 / scale).
    world = scale * unit
    struct.pack_into("<3fHH3iI3fI4f", seg0, 0, scale, scale, scale, 0x800b, 0, 0, 0, 0, 64, 0.0, 0.0, 0.0, 0,
                     centre[0] * world, (centre[1] + lift) * world, centre[2] * world, radius * world)
    for i, b in enumerate(walk):
        bone = bones[b]
        child = node_of[kids[b][0]] * 64 if kids[b] else 0
        siblings = kids[bone["parent"]] if bone["parent"] in keep else tops
        k = siblings.index(b)
        nxt = node_of[siblings[k + 1]] * 64 if k + 1 < len(siblings) else 0
        top = bone["parent"] not in keep
        t = [bone["t"][0] * unit, (bone["t"][1] + (lift if top else 0.0)) * unit, bone["t"][2] * unit]
        local_centre = apply(invert(rest_world[b]), centre)
        struct.pack_into("<3fHH3iI3fI4f", seg0, 64 * (i + 1), *bone["s"], flags, 1 + 7 * i,
                         *(bams(r) for r in bone["r"]), child, *t, nxt,
                         *(c * unit for c in local_centre), radius * world)

    # Segment 2: batches of triangles using at most 8 bones; weighted strips in VU chunks.
    def bones_of(tri):
        return frozenset(b for corner in tri for _, b in weights[corner[0]])

    order = sorted(range(len(triangles)), key=lambda t: sorted(bones_of(triangles[t])))
    batches, current, members = [], set(), []
    for t in order:
        need = bones_of(triangles[t])
        if len(need) > 8:
            continue
        if len(current | need) > 8:
            batches.append((sorted(current), members))
            current, members = set(), []
        current |= need
        members.append(triangles[t])
    if members:
        batches.append((sorted(current), members))

    def units_of(points):
        return sum(1 + 3 * len(weights[p[0]]) for p in points)

    seg2 = bytearray()
    refs = []
    for batch_bones, tris in batches:
        vu = {b: VU_MATRICES[i] for i, b in enumerate(batch_bones)}
        start = len(seg2)
        strips = [piece for strip in mi.stripify(tris) for piece in split_strip(strip, STRIP_POINTS)]

        def flush(chunk):
            addr = 0
            for j, points in enumerate(chunk):
                units = units_of(points)
                seg2.extend(struct.pack("<5I", 0x68018000 | addr, len(points) | (0x8000 if j == len(chunk) - 1 else 0),
                                        0x301e4000, 0x8412, 0x69008000 | (units << 16) | (addr + 1)))
                for position, coord in points:
                    u, v = coords[coord] if coord >= 0 else (0.0, 0.0)
                    ws = weights[position]
                    seg2.extend(struct.pack("<hhH", clamp16(u * 4096), clamp16((1.0 - v) * 4096), len(ws)))
                    q = [int(round(w * 4096)) for w, _ in ws]
                    q[0] += 4096 - sum(q)
                    for (w, b), qw in zip(ws, q):
                        local = apply(inv_bind[b], positions[position])
                        n = [sum(inv_bind[b][r][c] * smooth[position][c] for c in range(3)) for r in range(3)]
                        length = math.sqrt(sum(x * x for x in n)) or 1.0
                        seg2.extend(struct.pack("<HHH3h3h", vu[b], qw, 1,
                                                *(clamp16(32767 * x / length) for x in n),
                                                *(clamp16(c * unit) for c in local)))
                seg2.extend(bytes(-len(seg2) % 4))
                addr += 1 + units
            seg2.extend(struct.pack("<I", 0x15000000))
            seg2.extend(bytes(-len(seg2) % 16))

        address, chunk = 0, []
        for points in strips:
            need = 1 + units_of(points)
            if chunk and address + need > CHUNK_QUADWORDS:
                flush(chunk)
                address, chunk = 0, []
            chunk.append(points)
            address += need
        if chunk:
            flush(chunk)
        refs.append((batch_bones, start, len(seg2) - start))

    # Segment 1: the jump and the matrix area, the donor's texture and material records, then
    # the batches.
    donor_list = donor[segs[1][0]:segs[1][0] + segs[1][1]]
    body = struct.unpack_from("<I", donor_list, 4)[0] & 0x0fffffff
    # The first texture record (the first texture, which the card's replaces) and the material
    # records around it, up to the first matrix record or a second texture.
    lead = bytearray()
    at, textures = body, 0
    while at < len(donor_list):
        word = struct.unpack_from("<I", donor_list, at)[0]
        if word == 0x10000008 and textures == 0:
            lead += donor_list[at:at + 0x90]
            at += 0x90
            textures += 1
        elif word == 0x10000003:
            lead += donor_list[at:at + 0x40]
            at += 0x40
        else:
            break
    if not textures:
        raise ValueError("the donor's draw list does not start with a texture record")
    area = 16 + 16 * 7 * len(walk)
    seg1 = bytearray(area)
    struct.pack_into("<4I", seg1, 0, 0x20000000, 0x20000000 | area, *struct.unpack_from("<2I", donor_list, 8))
    seg1 += lead
    matrix_words, strip_words = [4], []
    for batch_bones, start, size in refs:
        for i, b in enumerate(batch_bones):
            matrix_words.append(len(seg1) + 4)
            seg1 += struct.pack("<4I", 0x30000007, 0x20000000 | (16 * (1 + 7 * walk.index(b))), 0x10000000,
                                0x6c070000 | VU_MATRICES[i])
        strip_words.append(len(seg1) + 4)
        seg1 += struct.pack("<4I", 0x30000000 | (size >> 4), 0x30000000 | start, 0, 0)

    # Motions, from the clips.
    letters = {c["name"][-1].lower(): c for c in model["clips"]}
    templates = mo.motions(donor)
    if not templates:
        raise ValueError("the donor has no motions to copy a header from")
    template = donor[templates[0][1]:templates[0][1] + 0x7c]
    kind = 7 if scaled_anim else 3
    new_motions = {}
    for slot, letter in sorted(clip_slots.items()):
        # A clip letter (the last letter of the clip's name), or [letter or full clip name, start
        # seconds, end seconds] for a piece of a longer clip.
        start, end = 0.0, None
        if isinstance(letter, (list, tuple)):
            letter, start, end = letter[0], float(letter[1]), float(letter[2])
        clip = letters.get(str(letter).lower()) or next((c for c in model["clips"] if c["name"] == letter), None)
        if clip is None:
            continue
        end = clip["seconds"] if end is None else min(end, clip["seconds"])
        frames = max(2, int(round((end - start) * FPS)) + 1)
        samples = [fa.pose(model, clip, start + f / FPS) for f in range(frames)]
        node_tracks = [(None, None, None)]          # the root: no keys
        for b in walk:
            top = bones[b]["parent"] not in keep
            # Position keys are multiplied by the header's rate (1 / root scale) in game.
            pos = [(f, s[b][0][0] * world, (s[b][0][1] + (lift if top else 0.0)) * world, s[b][0][2] * world)
                   for f, s in enumerate(samples)]
            rot = list(zip(range(frames), *(unwrap([bams(s[b][1][k]) for s in samples]) for k in range(3))))
            scl = [(f, *s[b][2]) for f, s in enumerate(samples)]
            pos = simplify(pos, POS_TOLERANCE * height * world)
            rot = simplify(rot, ROT_TOLERANCE)
            scl = simplify(scl, 0.01) if kind == 7 else None
            node_tracks.append((pos, rot, scl))
        new_motions[slot] = motion_file(template, node_tracks, frames, kind)

    # The slot: the donor up to its motions (the header's +8..+0xc span is the texture block, which
    # the game sends to the GS whole, so nothing new may go inside it), then the new motions where
    # the donor's were, the donor's blocks that followed its motions, then the new segments.
    motion_start, = struct.unpack_from("<I", donor, 0xc)
    after = motion_start
    for _, at in templates:
        after = max(after, at + struct.unpack_from("<I", donor, at + 4)[0])
    tail_blocks = donor[after:mi.data_end(donor)]
    place = motion_start

    free_start = place
    size = len(data)

    def put(blob, align=16):
        nonlocal place
        place = (place + align - 1) & ~(align - 1)
        at = place
        if at + len(blob) <= size:
            data[at:at + len(blob)] = blob
        place += len(blob)
        return at

    def word_list(words, tag):
        out = b"".join(struct.pack("<I", tag | w) for w in words) + struct.pack("<I", 0xffffffff)
        return out + bytes(-len(out) % 16)

    first_motion = motion_start
    for slot in sorted(new_motions):
        put(new_motions[slot])
    put(tail_blocks)
    place = (place + 0xff) & ~0xff
    seg0_at = put(seg0)
    seg1_at = put(seg1)
    seg2_at = put(seg2)
    if seg2_at != seg1_at + len(seg1):
        raise ValueError("segment 2 must follow segment 1")
    seg6_at = put(word_list(matrix_words, 0x20000000))
    seg8_at = put(word_list(strip_words, 0x20000000))
    if place > size or len(data) != size:
        raise TooBig(place - free_start, size - free_start, len(seg2), len(triangles))
    for index, (offset, size) in ((0, (seg0_at, len(seg0))), (1, (seg1_at, len(seg1))), (2, (seg2_at, len(seg2))),
                                  (3, (seg2_at + len(seg2), 0)), (4, (seg2_at + len(seg2), 0)),
                                  (6, (seg6_at, 4 * len(matrix_words) + 4)), (8, (seg8_at, 4 * len(strip_words) + 4))):
        struct.pack_into("<II", data, 0x10 + 8 * index, offset, size)
    flags_word, = struct.unpack_from("<I", data, 4)
    flags_word = (flags_word & ~0xff) | sum(1 << s for s in new_motions)
    struct.pack_into("<II", data, 4, flags_word, struct.unpack_from("<I", data, 8)[0])
    struct.pack_into("<I", data, 0xc, first_motion)
    struct.pack_into("<f", data, 0x64, 1.0 / scale)        # the motion position rate
    # Segment 9, the donor's option objects (lRhModel_SetupOptionObject): lists of places in the
    # donor's draw list to blank out (its sword, a second head...). In the new draw list those
    # places are matrix records and strip references, so the table is switched off (size 0).
    struct.pack_into("<I", data, 0x10 + 8 * 9 + 4, 0)

    # Segment 5, the texture list (lRhModel_SetupTexture): per texture an item (its object, whose
    # register data the game copies each frame) and a run of patch words, offsets in the draw list
    # (tag 2) or in the translucent part (tag 4) to copy them to. The donor's words name places in
    # the donor's draw list; the new list has one texture record, at `area`, and no translucent
    # part (a tag 4 word would write near address 0). The item that patched the donor's first
    # texture record keeps one word, at the same place in the new record; the others none.
    list_at = segs[5][0]
    count, runs_offset = struct.unpack_from("<2i", data, list_at)
    chosen = None
    for i in range(count):
        n, words_offset = struct.unpack_from("<2i", data, list_at + runs_offset + 8 * i)
        for j in range(n):
            w, = struct.unpack_from("<I", data, list_at + words_offset + 4 * j)
            rel = (w & 0x0fffffff) - body
            if chosen is None and w >> 28 == 2 and 0 <= rel < 0x90:
                chosen = (i, words_offset, rel)
    if chosen is None:
        raise ValueError("the donor's texture list does not patch its first texture record")
    for i in range(count):
        struct.pack_into("<i", data, list_at + runs_offset + 8 * i, 1 if i == chosen[0] else 0)
    struct.pack_into("<I", data, list_at + chosen[1], 0x20000000 | (area + chosen[2]))

    if texture:
        at, = struct.unpack_from("<I", data, list_at + 16 + 16 * chosen[0])
        size = mt.info(data, at)
        w, h, rows = mt.read_png(texture)
        mt.encode(data, at, mt.resize(w, h, rows, size["width"], size["height"]))

    report = dict(bones=len(walk), batches=len(batches), triangles=len(triangles), strip_bytes=len(seg2),
                  motions={s: len(m) for s, m in new_motions.items()}, scale=scale, unit=unit,
                  end=place)
    return bytes(data), report


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("donor", type=int)
    parser.add_argument("fbx")
    parser.add_argument("--texture")
    parser.add_argument("--out", required=True)
    args = parser.parse_args(argv)
    import project
    donor = mr.slot(project.find_iso(None), args.donor).ljust(0x200000, b"\0")
    slot, report = build(donor, args.fbx, args.texture)
    Path(args.out).write_bytes(slot)
    print(report)
    return 0


if __name__ == "__main__":
    sys.exit(main())
