"""Export Yu-Gi-Oh! Cross Duel monster models and animations to glTF (.glb).

Reads the game's Unity asset bundles (the `.Data` folder), finds every
monster rig (an Animator on a GameObject named like `07991_01J`), and
writes one .glb per rig with its skinned meshes, skeleton, textures and
every animation clip (a..h).  Unity is left-handed, so X is negated and
triangle winding reversed for glTF.

    py -3.12 -I tools/crossduel_export.py <.Data folder> <out folder> [names.json]
"""
import io, json, math, os, re, struct, sys, zlib
import UnityPy
from UnityPy.helpers.MeshHelper import MeshHandler

RIG = re.compile(r"^(\d{5,7})_(\d\d[A-Z]?)$")   # 5 digits a card; 6-7 (110009_01J) tokens and the like
CLIP_LABELS = {
    "a": "summon", "b": "idle", "c": "attack", "d": "set-flip", "e": "damage",
    "f": "attack-windup", "g": "attack-return", "h": "destroyed",
}


# ---------------------------------------------------------------- math
def qmul(a, b):
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    return (aw * bx + ax * bw + ay * bz - az * by,
            aw * by - ax * bz + ay * bw + az * bx,
            aw * bz + ax * by - ay * bx + az * bw,
            aw * bw - ax * bx - ay * by - az * bz)


def euler_to_quat(x, y, z):
    def axis(i, deg):
        h = math.radians(deg) / 2
        v = [0.0, 0.0, 0.0]
        v[i] = math.sin(h)
        return (v[0], v[1], v[2], math.cos(h))
    return qmul(qmul(axis(1, y), axis(0, x)), axis(2, z))  # Unity ZXY


def qnorm(q):
    n = math.sqrt(sum(c * c for c in q)) or 1.0
    return tuple(c / n for c in q)


def conv_pos(p):
    return (-p[0], p[1], p[2])


def conv_quat(q):
    return (q[0], -q[1], -q[2], q[3])


def conv_matrix(m):
    # S M S with S = diag(-1, 1, 1, 1); m is row-major 4x4
    s = (-1, 1, 1, 1)
    return [[m[r][c] * s[r] * s[c] for c in range(4)] for r in range(4)]


# ---------------------------------------------------------------- clips
def clip_curves(clip):
    """Return {curve_index: sampler(t)} and curve count for a Mecanim clip."""
    mc = clip.m_MuscleClip.m_Clip.data
    samplers = {}
    # streamed
    sc = mc.m_StreamedClip
    raw = struct.pack("<%dI" % len(sc.data), *sc.data)
    keys = {}
    off = 0
    while off + 8 <= len(raw):
        t, n = struct.unpack_from("<fI", raw, off)
        off += 8
        for _ in range(n):
            idx, c0, c1, c2, c3 = struct.unpack_from("<I4f", raw, off)
            off += 20
            keys.setdefault(idx, []).append((t, c0, c1, c2, c3))
    for idx, ks in keys.items():
        ks = [k for k in ks if math.isfinite(k[0])] or ks[:1]
        def s(t, ks=ks):
            k = ks[0]
            for kk in ks:
                if kk[0] <= t:
                    k = kk
                else:
                    break
            dt = max(0.0, t - k[0]) if math.isfinite(k[0]) else 0.0
            return ((k[1] * dt + k[2]) * dt + k[3]) * dt + k[4]
        samplers[idx] = s
    streamed = sc.curveCount
    # dense
    dc = mc.m_DenseClip
    for c in range(dc.m_CurveCount):
        def s(t, c=c, dc=dc):
            if dc.m_FrameCount <= 0:
                return 0.0
            f = (t - dc.m_BeginTime) * dc.m_SampleRate
            f = min(max(f, 0.0), dc.m_FrameCount - 1)
            i = int(f)
            j = min(i + 1, dc.m_FrameCount - 1)
            a = dc.m_SampleArray[i * dc.m_CurveCount + c]
            b = dc.m_SampleArray[j * dc.m_CurveCount + c]
            return a + (b - a) * (f - i)
        samplers[streamed + c] = s
    base = streamed + dc.m_CurveCount
    for c, v in enumerate(mc.m_ConstantClip.data):
        samplers[base + c] = (lambda t, v=v: v)
    return samplers


def clip_tracks(clip):
    """Yield (path_hash, kind, [curve sampler...]) for transform bindings."""
    samplers = clip_curves(clip)
    idx = 0
    for b in clip.m_ClipBindingConstant.genericBindings:
        type_id = getattr(b, "typeID", None)
        if type_id is None:
            type_id = getattr(b, "classID", 0)
        if type_id == 4:
            dim = {1: 3, 2: 4, 3: 3, 4: 3}.get(b.attribute, 1)
        else:
            dim = 1
        if type_id == 4 and b.attribute in (1, 2, 3, 4):
            kind = {1: "translation", 2: "rotation", 3: "scale", 4: "euler"}[b.attribute]
            yield b.path, kind, [samplers.get(idx + i, lambda t: 0.0) for i in range(dim)]
        idx += dim


# ---------------------------------------------------------------- glb
class Glb:
    def __init__(self):
        self.bin = bytearray()
        self.j = {"asset": {"version": "2.0", "generator": "dotr crossduel_export"},
                  "scene": 0, "scenes": [{"nodes": []}], "nodes": [], "meshes": [],
                  "accessors": [], "bufferViews": [], "buffers": [], "materials": [],
                  "textures": [], "images": [], "samplers": [{}], "skins": [], "animations": []}

    def view(self, data, target=None):
        while len(self.bin) % 4:
            self.bin.append(0)
        v = {"buffer": 0, "byteOffset": len(self.bin), "byteLength": len(data)}
        if target:
            v["target"] = target
        self.bin += data
        self.j["bufferViews"].append(v)
        return len(self.j["bufferViews"]) - 1

    def accessor(self, values, kind, ctype=5126, target=None, minmax=False):
        n = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}[kind]
        fmt = {5126: "f", 5125: "I", 5123: "H", 5121: "B"}[ctype]
        flat = []
        for v in values:
            if n == 1:
                flat.append(v)
            else:
                flat.extend(v)
        data = struct.pack("<%d%s" % (len(flat), fmt), *flat)
        a = {"bufferView": self.view(data, target), "componentType": ctype,
             "count": len(values), "type": kind}
        if minmax and values:
            cols = list(zip(*values)) if n > 1 else [values]
            a["min"] = [min(c) for c in cols]
            a["max"] = [max(c) for c in cols]
        self.j["accessors"].append(a)
        return len(self.j["accessors"]) - 1

    def save(self, path):
        while len(self.bin) % 4:
            self.bin.append(0)
        self.j["buffers"] = [{"byteLength": len(self.bin)}]
        for k in [k for k, v in self.j.items() if v == []]:
            del self.j[k]
        js = json.dumps(self.j, separators=(",", ":")).encode()
        while len(js) % 4:
            js += b" "
        with open(path, "wb") as f:
            f.write(struct.pack("<III", 0x46546C67, 2, 12 + 8 + len(js) + 8 + len(self.bin)))
            f.write(struct.pack("<II", len(js), 0x4E4F534A) + js)
            f.write(struct.pack("<II", len(self.bin), 0x004E4942) + self.bin)


# ---------------------------------------------------------------- export
def matrix(m):
    return [[getattr(m, "e%d%d" % (r, c)) for c in range(4)] for r in range(4)]


def texture_png(tex):
    buf = io.BytesIO()
    tex.image.save(buf, "PNG")
    return buf.getvalue()


def export_rig(animator, out_dir, base_name):
    go = animator.m_GameObject.read()
    root_ptr = next(c.component for c in go.m_Component if c.component.type.name == "Transform")
    root_tf = root_ptr.read()

    nodes = []           # (path_id, transform, path, parent_index)
    def walk(pid, tf, path, parent):
        i = len(nodes)
        nodes.append((pid, tf, path, parent))
        for c in tf.m_Children:
            ct = c.read()
            name = ct.m_GameObject.read().m_Name
            walk(c.path_id, ct, (path + "/" + name) if path else name, i)
    walk(root_ptr.path_id, root_tf, "", None)
    index_of = {pid: i for i, (pid, _, _, _) in enumerate(nodes)}
    by_hash = {zlib.crc32(path.encode()): i for i, (_, _, path, _) in enumerate(nodes)}

    g = Glb()
    children = {}
    for i, (pid, tf, path, parent) in enumerate(nodes):
        name = tf.m_GameObject.read().m_Name
        p = tf.m_LocalPosition
        q = tf.m_LocalRotation
        s = tf.m_LocalScale
        node = {"name": name, "translation": list(conv_pos((p.x, p.y, p.z))),
                "rotation": list(qnorm(conv_quat((q.x, q.y, q.z, q.w)))),
                "scale": [s.x, s.y, s.z]}
        g.j["nodes"].append(node)
        if parent is not None:
            children.setdefault(parent, []).append(i)
    for p, cs in children.items():
        g.j["nodes"][p]["children"] = cs
    g.j["scenes"][0]["nodes"] = [0]

    images = {}
    textures_written = []
    def material_for(mat_ptr):
        mat = mat_ptr.read()
        main = None
        for k, v in mat.m_SavedProperties.m_TexEnvs:
            if not v.m_Texture.path_id:
                continue
            try:
                tex = v.m_Texture.read()
            except Exception:
                continue
            if tex.m_Name not in images:
                png = texture_png(tex)
                with open(os.path.join(out_dir, tex.m_Name + ".png"), "wb") as f:
                    f.write(png)
                textures_written.append(tex.m_Name + ".png")
                if k in ("_Main_TEX", "_MainTex", "_BaseMap"):
                    g.j["images"].append({"bufferView": g.view(png), "mimeType": "image/png", "name": tex.m_Name})
                    g.j["textures"].append({"sampler": 0, "source": len(g.j["images"]) - 1})
                    images[tex.m_Name] = len(g.j["textures"]) - 1
                else:
                    images[tex.m_Name] = None
            if k in ("_Main_TEX", "_MainTex", "_BaseMap") and images.get(tex.m_Name) is not None:
                main = images[tex.m_Name]
        m = {"name": mat.m_Name, "pbrMetallicRoughness": {"metallicFactor": 0.0, "roughnessFactor": 1.0}}
        if main is not None:
            m["pbrMetallicRoughness"]["baseColorTexture"] = {"index": main}
        g.j["materials"].append(m)
        return len(g.j["materials"]) - 1

    def add_mesh(mesh, mats, skin_bones=None):
        h = MeshHandler(mesh)
        h.process()
        verts = [conv_pos(v) for v in h.m_Vertices]
        if not verts:
            return None
        attrs = {"POSITION": g.accessor(verts, "VEC3", target=34962, minmax=True)}
        if h.m_Normals and len(h.m_Normals) == len(verts):
            attrs["NORMAL"] = g.accessor([qnorm3(conv_pos(n)) for n in h.m_Normals], "VEC3", target=34962)
        if h.m_UV0 and len(h.m_UV0) == len(verts):
            attrs["TEXCOORD_0"] = g.accessor([(u[0], 1.0 - u[1]) for u in h.m_UV0], "VEC2", target=34962)
        if skin_bones is not None and h.m_BoneWeights:
            ws, js = [], []
            for w, bi in zip(h.m_BoneWeights, h.m_BoneIndices):
                w = list(w) + [0.0] * (4 - len(w))
                bi = list(bi) + [0] * (4 - len(bi))
                t = sum(w) or 1.0
                ws.append(tuple(x / t for x in w[:4]))
                js.append(tuple(min(int(b), len(skin_bones) - 1) for b in bi[:4]))
            ctype = 5123 if len(skin_bones) > 255 else 5121
            attrs["JOINTS_0"] = g.accessor(js, "VEC4", ctype, target=34962)
            attrs["WEIGHTS_0"] = g.accessor(ws, "VEC4", target=34962)
        prims = []
        for si, tris in enumerate(h.get_triangles()):
            flat = []
            for a, b, c in tris:
                flat.extend((a, c, b))
            if not flat:
                continue
            ct = 5125 if max(flat) > 65535 else 5123
            p = {"attributes": attrs, "indices": g.accessor(flat, "SCALAR", ct, target=34963)}
            if si < len(mats) and mats[si] is not None:
                p["material"] = mats[si]
            prims.append(p)
        if not prims:
            return None
        g.j["meshes"].append({"name": mesh.m_Name, "primitives": prims})
        return len(g.j["meshes"]) - 1

    meshes = 0
    for i, (pid, tf, path, parent) in enumerate(nodes):
        obj = tf.m_GameObject.read()
        for comp in obj.m_Component:
            t = comp.component.type.name
            if t == "SkinnedMeshRenderer":
                r = comp.component.read()
                if not r.m_Mesh.path_id or not r.m_Enabled:
                    continue
                mesh = r.m_Mesh.read()
                bones = [index_of.get(b.path_id) for b in r.m_Bones]
                if any(b is None for b in bones):
                    bones = [b if b is not None else 0 for b in bones]
                binds = [conv_matrix(matrix(m)) for m in mesh.m_BindPose]
                mats = [material_for(m) for m in r.m_Materials]
                mi = add_mesh(mesh, mats, bones)
                if mi is None:
                    continue
                ibm = [tuple(m[r][c] for c in range(4) for r in range(4)) for m in binds]  # column-major
                g.j["skins"].append({"joints": bones, "inverseBindMatrices": g.accessor(ibm, "MAT4"), "skeleton": 0})
                holder = {"name": obj.m_Name + "_skin", "mesh": mi, "skin": len(g.j["skins"]) - 1}
                g.j["nodes"].append(holder)
                g.j["scenes"][0]["nodes"].append(len(g.j["nodes"]) - 1)
                meshes += 1
            elif t == "MeshRenderer":
                r = comp.component.read()
                if not r.m_Enabled or obj.m_Name.startswith("EFF"):
                    continue
                mf = next((c.component.read() for c in obj.m_Component if c.component.type.name == "MeshFilter"), None)
                if mf is None or not mf.m_Mesh.path_id:
                    continue
                mesh = mf.m_Mesh.read()
                if mesh.m_Name.startswith("EFF"):
                    continue
                mats = [material_for(m) for m in r.m_Materials]
                mi = add_mesh(mesh, mats)
                if mi is not None:
                    g.j["nodes"][i]["mesh"] = mi
                    meshes += 1
    if not meshes:
        return None

    # animations through the override controller
    clips = []
    ctrl = animator.m_Controller.read() if animator.m_Controller.path_id else None
    pairs = []
    if ctrl is not None and hasattr(ctrl, "m_Clips"):
        for p in ctrl.m_Clips:
            if p.m_OverrideClip.path_id:
                orig = p.m_OriginalClip.read().m_Name if p.m_OriginalClip.path_id else ""
                pairs.append((orig, p.m_OverrideClip.read()))
    elif ctrl is not None and hasattr(ctrl, "m_AnimationClips"):
        pairs = [(c.read().m_Name, c.read()) for c in ctrl.m_AnimationClips]
    seen = set()
    for orig, clip in pairs:
        if clip.m_Name in seen:
            continue
        seen.add(clip.m_Name)
        letter = clip.m_Name.rsplit("_", 1)[-1]
        label = CLIP_LABELS.get(letter, "")
        start = clip.m_MuscleClip.m_StartTime
        stop = clip.m_MuscleClip.m_StopTime
        rate = clip.m_SampleRate or 30.0
        frames = max(2, int(round((stop - start) * rate)) + 1)
        times = [start + (stop - start) * f / (frames - 1) for f in range(frames)]
        out_times = [t - start for t in times]
        tacc = g.accessor(out_times, "SCALAR", minmax=True)
        anim = {"name": clip.m_Name + (" " + label if label else ""), "channels": [], "samplers": []}
        euler = {}
        for h, kind, curves in clip_tracks(clip):
            ni = by_hash.get(h)
            if ni is None:
                continue
            vals = [tuple(c(t) for c in curves) for t in times]
            if kind == "translation":
                out = [conv_pos(v) for v in vals]
                typ = "VEC3"
            elif kind == "rotation":
                out = [qnorm(conv_quat(v)) for v in vals]
                for k in range(1, len(out)):  # keep hemisphere continuous
                    if sum(a * b for a, b in zip(out[k], out[k - 1])) < 0:
                        out[k] = tuple(-x for x in out[k])
                typ = "VEC4"
            elif kind == "euler":
                out = [qnorm(conv_quat(euler_to_quat(*v))) for v in vals]
                kind = "rotation"
                typ = "VEC4"
            else:
                out = vals
                typ = "VEC3"
            anim["samplers"].append({"input": tacc, "output": g.accessor(out, typ), "interpolation": "LINEAR"})
            anim["channels"].append({"sampler": len(anim["samplers"]) - 1, "target": {"node": ni, "path": kind}})
        if anim["channels"]:
            g.j["animations"].append(anim)
            clips.append({"clip": clip.m_Name, "meaning": label, "seconds": round(stop - start, 3)})
    path = os.path.join(out_dir, base_name + ".glb")
    g.save(path)
    return {"file": base_name + ".glb", "bones": len(nodes), "meshes": meshes,
            "clips": clips, "textures": textures_written}


def qnorm3(v):
    n = math.sqrt(sum(c * c for c in v)) or 1.0
    return tuple(c / n for c in v)


def safe(name):
    return re.sub(r'[<>:"/\\|?*]', "", name).strip()


def main():
    src, out = sys.argv[1], sys.argv[2]
    names = json.load(open(sys.argv[3], encoding="utf-8")) if len(sys.argv) > 3 else {}
    only = set(sys.argv[4].split(",")) if len(sys.argv) > 4 else None
    os.makedirs(out, exist_ok=True)
    report = {}
    for fn in sorted(os.listdir(src)):
        p = os.path.join(src, fn)
        if not fn.endswith(".bundle"):
            continue
        try:
            env = UnityPy.load(p)
            animators = [o for o in env.objects if o.type.name == "Animator"]
        except Exception as e:
            print("skip", fn, e)
            continue
        for o in animators:
            try:
                a = o.read()
                go = a.m_GameObject.read()
            except Exception:
                continue
            m = RIG.match(go.m_Name)
            if not m or (only and m.group(1) not in only):
                continue
            rig = go.m_Name
            if rig in report:
                continue
            card = names.get(m.group(1).lstrip("0"), names.get(m.group(1), "Unknown " + m.group(1)))
            folder = os.path.join(out, safe(card))
            os.makedirs(folder, exist_ok=True)
            try:
                info = export_rig(a, folder, rig)
            except Exception as e:
                print("FAIL", rig, fn, repr(e))
                continue
            if info:
                info.update({"card": card, "bundle": fn})
                report[rig] = info
                print(rig, card, info["meshes"], "meshes", len(info["clips"]), "clips")
    with open(os.path.join(out, "models.json"), "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, ensure_ascii=False)


if __name__ == "__main__":
    main()
