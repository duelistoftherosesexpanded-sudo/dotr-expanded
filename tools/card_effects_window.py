"""The card maker's Effects window: a card's effect slots and leader abilities, saved to
mods/widescreen/effects.json (tools/dlc_effects.py turns it into the DLC's tables).

Any card can be edited, the game's (0-853) or a new one (871-998). A monster has five slots
(ATTACK, MOVEMENT, NATURE EFFECT, FLIP, DESTRUCTION), any other card one. For each slot:

  Effect   "(keep)" the card's own, "(none)" no effect, or one of the game's effects for that slot
           (every card that has one, with its description)
  fields   type, whose cards (scope), which cards (target), value, extra: filled in from the chosen
           effect and free to change; a changed field makes the slot a custom one
  Add effect  stacks another effect on the slot (up to five in all; Remove takes one off); every
           slot stacks (dlc/stack.c)

Leader abilities (monsters): each of the 17 is kept, removed, or set with the rank it needs and,
for those that have one, a value. docs/topics/card-effects.md explains the numbers."""

import json
import tkinter as tk
from tkinter import messagebox, ttk

import card_effects
import dlc_effects
import gui_theme

KEEP, NONE_ = "(keep)", "(none)"
SLOT_TITLES = ["ATTACK", "MOVEMENT", "NATURE EFFECT", "FLIP", "DESTRUCTION"]
SCOPE_NAMES = ["self", "own", "enemy", "both"]


def target_names():
    return [f"{k}  {v}" for k, v in sorted(card_effects.TARGETS.items())]


class EffectsWindow(tk.Toplevel):
    def __init__(self, master, exe, effects_path, cards_path, card=None):
        super().__init__(master)
        self.title("Card effects and leader abilities")
        self.minsize(900, 640)
        self.geometry("1120x820")
        self.configure(bg=gui_theme.BG)
        self.effects_path, self.cards_path = effects_path, cards_path
        self.fx = dlc_effects.Effects(exe, cards_path)
        self.catalogue = card_effects.catalogue(exe)
        self.entries = json.loads(effects_path.read_text()) if effects_path.is_file() else []
        self.card_values = [f"{n:03d}  {self.fx.text.decode(card_effects.NAMES + n)}" for n in range(854) if n != 671]
        self.card_values += [f"{n}  {c['name']}" for n, c in sorted(self.fx.new.items())]
        self.v_card = tk.StringVar()
        self.v_from = tk.StringVar()
        self._build()
        start = card if card is not None else 0
        for value in self.card_values:
            if int(value.split()[0]) == start:
                self.v_card.set(value)
        self.load()
        gui_theme.title_bar(self)

    # ------------------------------------------------------------------ layout
    def _build(self):
        top = ttk.Frame(self)
        top.pack(fill="x", padx=8, pady=6)
        ttk.Label(top, text="Card").pack(side="left")
        box = ttk.Combobox(top, textvariable=self.v_card, values=self.card_values, width=40)
        box.pack(side="left", padx=6)
        gui_theme.wide_dropdown(box)
        box.bind("<<ComboboxSelected>>", lambda e: self.load())
        ttk.Button(top, text="Save", command=self.save).pack(side="left", padx=6)
        ttk.Button(top, text="Back to the card's own", command=self.remove).pack(side="left")
        self.status = tk.StringVar()
        ttk.Label(top, textvariable=self.status, foreground=gui_theme.STATUS).pack(side="left", padx=10)

        self.book = ttk.Notebook(self)
        self.book.pack(fill="both", expand=True, padx=8, pady=4)
        self.effects_tab = ttk.Frame(self.book)
        self.leader_tab = ttk.Frame(self.book)
        self.book.add(self.effects_tab, text="Effects")
        self.book.add(self.leader_tab, text="Leader abilities")

        start = ttk.Frame(self.effects_tab)
        start.pack(fill="x", pady=4)
        ttk.Label(start, text="Start from").pack(side="left")
        self.from_box = ttk.Combobox(start, textvariable=self.v_from, width=40)
        self.from_box.pack(side="left", padx=6)
        gui_theme.wide_dropdown(self.from_box)
        self.from_box.bind("<<ComboboxSelected>>", lambda e: self.load_slots())
        ttk.Label(start, text="(another card's effects as the base; slots left on (keep) take them)",
                  foreground=gui_theme.HINT).pack(side="left")

        # The slots scroll: stacked effects make the tab taller than the window.
        holder = ttk.Frame(self.effects_tab)
        holder.pack(fill="both", expand=True)
        scroller = tk.Canvas(holder, bg=gui_theme.BG, highlightthickness=0)
        bar = ttk.Scrollbar(holder, orient="vertical", command=scroller.yview)
        scroller.configure(yscrollcommand=bar.set)
        bar.pack(side="right", fill="y")
        scroller.pack(side="left", fill="both", expand=True)
        self.slots_area = ttk.Frame(scroller)
        window = scroller.create_window((0, 0), window=self.slots_area, anchor="nw")
        self.slots_area.bind("<Configure>", lambda e: scroller.configure(scrollregion=scroller.bbox("all")))
        scroller.bind("<Configure>", lambda e: scroller.itemconfigure(window, width=e.width))
        self.bind_all("<MouseWheel>", lambda e: scroller.yview_scroll(int(-e.delta / 120), "units")
                      if str(e.widget).startswith(str(self)) else None)

        self.slots = []
        for i in range(5):
            frame = ttk.LabelFrame(self.slots_area, text=SLOT_TITLES[i])
            frame.pack(fill="x", pady=3)
            rows = ttk.Frame(frame)
            rows.pack(fill="x")
            foot = ttk.Frame(frame)
            foot.pack(fill="x", padx=4, pady=2)
            add = ttk.Button(foot, text="Add effect", command=lambda i=i: self.add_row(i, extra=True))
            add.pack(side="left")
            note = tk.StringVar()
            ttk.Label(foot, textvariable=note, foreground=gui_theme.HINT).pack(side="left", padx=8)
            self.slots.append(dict(frame=frame, rows_frame=rows, rows=[], add=add, note=note, options=[]))
            self.add_row(i, extra=False)

        canvas = ttk.Frame(self.leader_tab)
        canvas.pack(fill="both", expand=True)
        ttk.Label(canvas, text="Ability", font=("Segoe UI", 9, "bold")).grid(row=0, column=0, sticky="w", padx=4)
        ttk.Label(canvas, text="State").grid(row=0, column=1)
        ttk.Label(canvas, text="Rank needed (0-12)").grid(row=0, column=2)
        ttk.Label(canvas, text="Value").grid(row=0, column=3)
        self.abilities = []
        for r, (number, name, _w, _h, rank_high, has_value) in enumerate(card_effects.ABILITIES, start=1):
            a = {k: tk.StringVar() for k in ("state", "rank", "value")}
            ttk.Label(canvas, text=f"{number:2d}  {name}").grid(row=r, column=0, sticky="w", padx=4, pady=1)
            ttk.Combobox(canvas, textvariable=a["state"], values=["keep", "none", "set"], width=6,
                         state="readonly").grid(row=r, column=1, padx=4)
            if rank_high is not None:
                ttk.Spinbox(canvas, from_=0, to=12, textvariable=a["rank"], width=5).grid(row=r, column=2, padx=4)
            if has_value:
                ttk.Entry(canvas, textvariable=a["value"], width=6).grid(row=r, column=3, padx=4)
            self.abilities.append(a)
        ttk.Label(canvas, foreground=gui_theme.HINT, justify="left", text=(
            "Values: terrain change - a terrain (0 forest ... 9 crush); level cost reduction - levels; "
            "strength for same type - points / 100;\nweaken enemy type - a monster type number; "
            "summoning power bonus - extra summoning power.")).grid(row=20, column=0, columnspan=4, sticky="w", padx=4, pady=6)

    # ------------------------------------------------------------------ data
    def card(self):
        return int(self.v_card.get().split()[0])

    def entry(self):
        return next((e for e in self.entries if self.fx.number(e["card"]) == self.card()), {})

    def slot_options(self, i, monster):
        """[(label, word0, word1, description)] of the game's effects for slot i."""
        # Like effects are bundled (tools/card_effects.py bundle): one entry per effect, with the
        # cards that have it. Effects of the other kind of card that this trigger also runs
        # (card_effects.RUNS) follow, marked: a spell's for a monster's FLIP / DESTRUCTION, a
        # monster's FLIP / DESTRUCTION for a spell.
        spells = [(g, mark) for kind, mark in (("Magic", "[from a spell] "), ("Trap (full)", "[from a trap] "))
                  for g in self.catalogue["card_bundles"].get(kind, [])]
        groups = []   # (bundle, source slot name or None, mark)
        if monster:
            groups += [(g, dlc_effects.SLOT_NAMES[i], "") for g in self.catalogue["monster_bundles"].get(str(i), [])]
            runs = {3: card_effects.RUNS["flip"], 4: card_effects.RUNS["destruction"]}.get(i)
            if runs:
                groups += [(g, None, mark) for g, mark in spells if g["type"] in runs]
        else:
            groups += [(g, None, "") for kind in self.catalogue["card_bundles"].values() for g in kind]
            groups.sort(key=lambda x: (x[0]["type"], x[0]["extra"], x[0]["value"]))
            for s, slot in ((3, "flip"), (4, "destruction")):
                groups += [(g, slot, f"[from a monster's {slot.upper()}] ")
                           for g in self.catalogue["monster_bundles"].get(str(s), [])
                           if g["type"] in card_effects.RUNS["spell"]]
        out = []
        for g, slot, mark in groups:
            names = ", ".join(c["name"] for c in g["cards"][:3]) + ("..." if len(g["cards"]) > 3 else "")
            n = len(g["cards"])
            label = f"{mark}{g['type_name']}: {g['text']}   [{n} card{'s' if n > 1 else ''}: {names}]"
            if any(o[0] == label for o in out):
                label += f"  (type {g['type']}, {g['value']}, {g['extra']})"
            detail = (f"{g['type_name']} (type {g['type']}). {g['text']}\nCards with this effect: "
                      + ", ".join(c["name"] for c in g["cards"]))
            if mark:
                detail += ("\nBorrowed: this trigger runs the same type, so the effect should work here; "
                           "the amounts are read the same way in most cases, but try it in game.")
            out.append((label, int(g["word0"], 16), int(g["word1"], 16), detail, g["cards"][0]["card"], slot))
        return out

    def warning(self, i, monster, w0):
        """A note when the slot's trigger does not run the type (it would then do nothing)."""
        if w0 == dlc_effects.NONE:
            return ""
        t = w0 >> 8
        runs = card_effects.RUNS["spell"] if not monster else \
            {3: card_effects.RUNS["flip"], 4: card_effects.RUNS["destruction"]}.get(i)
        if runs is not None and t not in runs:
            where = "a spell" if not monster else SLOT_TITLES[i]
            return f"\nWarning: {where} does not run type {t} ({card_effects.type_name(t)}); it would do nothing."
        return ""

    # ------------------------------------------------------------------ effect rows
    def add_row(self, i, extra):
        """One effect of slot i: the slot's own (extra False, with (keep) and (none)) or a stacked one."""
        s = self.slots[i]
        if extra and len(s["rows"]) > dlc_effects.STACK_MAX:
            return None
        row = {k: tk.StringVar() for k in ("pick", "type", "scope", "target", "value", "extra", "desc")}
        f = ttk.Frame(s["rows_frame"])
        f.pack(fill="x", padx=4, pady=(6 if extra else 2, 0))
        head = ttk.Frame(f)
        head.grid(row=0, column=0, columnspan=10, sticky="we", pady=2)
        if extra:
            ttk.Label(head, text="+", font=("Segoe UI", 11, "bold"), foreground=gui_theme.STATUS).pack(side="left", padx=(0, 4))
        pick = ttk.Combobox(head, textvariable=row["pick"], width=104, state="readonly")
        pick.pack(side="left", fill="x", expand=True)
        gui_theme.wide_dropdown(pick)
        pick.bind("<<ComboboxSelected>>", lambda e, i=i, row=row: self.on_pick(i, row))
        if extra:
            ttk.Button(head, text="Remove", command=lambda: self.remove_row(i, row)).pack(side="left", padx=6)
        for col, (key, label, width) in enumerate((("type", "type", 6), ("scope", "whose", 8),
                                                   ("target", "which", 18), ("value", "value", 7),
                                                   ("extra", "extra", 7))):
            ttk.Label(f, text=label).grid(row=1, column=2 * col, sticky="e", padx=2)
            if key == "scope":
                w = ttk.Combobox(f, textvariable=row[key], values=SCOPE_NAMES, width=width, state="readonly")
            elif key == "target":
                w = ttk.Combobox(f, textvariable=row[key], values=target_names(), width=width)
            else:
                w = ttk.Entry(f, textvariable=row[key], width=width)
            w.grid(row=1, column=2 * col + 1, sticky="w", padx=2)
        ttk.Label(f, textvariable=row["desc"], foreground=gui_theme.HINT, wraplength=1040, justify="left").grid(
            row=2, column=0, columnspan=10, sticky="w")
        f.columnconfigure(9, weight=1)
        row.update(frame=f, box=pick, is_extra=extra)
        labels = [o[0] for o in s["options"]]
        pick.configure(values=labels if extra else [KEEP, NONE_] + labels)
        if extra:
            row["desc"].set("Choose an effect to stack on this slot.")
        s["rows"].append(row)
        self.update_add(i)
        return row

    def remove_row(self, i, row):
        row["frame"].destroy()
        self.slots[i]["rows"].remove(row)
        self.update_add(i)

    def update_add(self, i):
        s = self.slots[i]
        monster = getattr(self, "monster", True)
        stackable = monster and dlc_effects.SLOT_NAMES[i] in dlc_effects.STACKABLE
        full = len(s["rows"]) > dlc_effects.STACK_MAX
        s["add"].configure(state="normal" if stackable and not full else "disabled")
        if not stackable:
            s["note"].set("")
        elif full:
            s["note"].set(f"At most {dlc_effects.STACK_MAX + 1} effects in one slot.")
        else:
            n = len(s["rows"])
            after = "; the stacked ones play after the first, in order." if i >= 3 else \
                "; each stacked one works alongside the first."
            s["note"].set(f"{n} effect{'s' if n > 1 else ''}" + (after if n > 1 else ""))

    def row_spec(self, i, row, label):
        """A row's effect as effects.json has it ({"copy"...} or the two words)."""
        try:
            w0, w1 = self.fields(row)
        except (ValueError, IndexError):
            raise ValueError(f"{label}: type, whose, which, value and extra must be filled in")
        match = next((o for o in self.slots[i]["options"] if (o[1], o[2]) == (w0, w1)), None)
        if match:
            # The copy names the card it comes from and, for a monster's, which of its slots.
            return {"copy": match[4], "slot": match[5]} if match[5] else {"copy": match[4]}
        return {"word0": f"{w0:#010x}", "word1": f"{w1:#010x}"}

    def show(self, i, row, w0, w1, prefix=""):
        self.set_fields(row, w0, w1)
        match = next((o for o in self.slots[i]["options"] if o[1] == w0 and o[2] == w1), None)
        if w0 == dlc_effects.NONE:
            text = "no effect"
        else:
            text = match[3] if match else "Custom effect (no other card has it)."
        row["desc"].set(prefix + text + self.warning(i, self.monster, w0))
        return match

    def set_fields(self, s, w0, w1):
        if w0 == dlc_effects.NONE:
            for k in ("type", "value", "extra", "target"):
                s[k].set("")
            s["scope"].set("")
            return
        s["type"].set(str(w0 >> 8))
        s["scope"].set(SCOPE_NAMES[(w0 >> 6) & 3])
        t = w0 & 63
        s["target"].set(f"{t}  {card_effects.TARGETS.get(t, '')}".strip())
        s["value"].set(str(w1 & 0xffff))
        s["extra"].set(str(w1 >> 16))

    def fields(self, s):
        t = s["target"].get().split()
        return (int(s["type"].get()) << 8 | SCOPE_NAMES.index(s["scope"].get()) << 6 | int(t[0]),
                int(s["extra"].get() or 0) << 16 | int(s["value"].get() or 0))

    def load(self):
        n = self.card()
        monster = self.fx.is_monster(n)
        e = self.entry()
        same = [v for v in self.card_values if self.fx.is_monster(int(v.split()[0])) == monster]
        self.from_box.configure(values=["(its own)"] + same)
        spec = e.get("effects" if monster else "effect", {}) if e else {}
        self.v_from.set("(its own)")
        if monster and isinstance(spec, dict) and spec.get("from") is not None:
            f = self.fx.number(spec["from"])
            self.v_from.set(next((v for v in same if int(v.split()[0]) == f), "(its own)"))
        self.load_slots()
        # Slots the entry sets: one effect, or a list (the slot's own, then the stacked ones).
        for i, s in enumerate(self.slots):
            given = (spec.get(dlc_effects.SLOT_NAMES[i]) if monster else (spec or None)) if spec else None
            if given is None:
                continue
            items = given if isinstance(given, list) else [given]
            for k, item in enumerate(items):
                row = s["rows"][0] if k == 0 else self.add_row(i, extra=True)
                if row is None:
                    break
                if item == "none":
                    row["pick"].set(NONE_)
                    self.show(i, row, dlc_effects.NONE, 0)
                    continue
                w0, w1 = self.fx.slot(item, i, "slot")
                match = self.show(i, row, w0, w1)
                row["pick"].set(match[0] if match else "(custom)")
        # Leader abilities.
        base = self.fx.leader_row(n) if monster else [dlc_effects.NONE] * 10
        given = e.get("leader", {}).get("abilities", {}) if e else {}
        for a, (number, name, w, high, rank_high, has_value) in zip(self.abilities, card_effects.ABILITIES):
            half = base[w] >> 16 if high else base[w] & 0xffff
            a["state"].set("keep")
            if rank_high is None:
                a["rank"].set("")
            elif half != 0xffff:
                a["rank"].set(str(half >> 8 if rank_high else half & 0xff))
                a["value"].set(str((half & 0xff if rank_high else half >> 8)) if has_value else "")
            else:
                a["rank"].set("")
                a["value"].set("")
            g = given.get(str(number))
            if g == "none":
                a["state"].set("none")
            elif isinstance(g, dict):
                a["state"].set("set")
                a["rank"].set(str(g.get("rank", "")))
                a["value"].set(str(g.get("value", "")) if has_value else "")
        self.book.tab(self.leader_tab, state="normal" if monster else "disabled")
        self.status.set("")

    def base_words(self):
        n = self.card()
        base_card = n if self.v_from.get() in ("", "(its own)") else int(self.v_from.get().split()[0])
        return self.fx.effect_row(base_card)

    def keep_primary(self, i):
        """The slot's own effect back to the card's (or "Start from"'s)."""
        words = self.base_words()
        row = self.slots[i]["rows"][0]
        row["pick"].set(KEEP)
        self.show(i, row, words[2 * i], words[2 * i + 1], prefix="Kept: ")

    def load_slots(self):
        self.monster = self.fx.is_monster(self.card())
        for i, s in enumerate(self.slots):
            for row in s["rows"][1:]:
                row["frame"].destroy()
            del s["rows"][1:]
            if not self.monster and i > 0:
                s["frame"].pack_forget()
                continue
            s["frame"].pack(fill="x", pady=3)
            s["frame"].configure(text=SLOT_TITLES[i] if self.monster else "EFFECT")
            s["options"] = self.slot_options(i, self.monster)
            s["rows"][0]["box"].configure(values=[KEEP, NONE_] + [o[0] for o in s["options"]])
            self.keep_primary(i)
            self.update_add(i)

    def on_pick(self, i, row):
        s = self.slots[i]
        label = row["pick"].get()
        if label == KEEP:
            self.keep_primary(i)
            return
        if label == NONE_:
            self.show(i, row, dlc_effects.NONE, 0)
            row["desc"].set("No effect.")
            return
        o = next(o for o in s["options"] if o[0] == label)
        self.show(i, row, o[1], o[2])

    # ------------------------------------------------------------------ saving
    def gather(self):
        n = self.card()
        monster = self.fx.is_monster(n)
        entry = {"card": n}
        specs = {}
        base = n if self.v_from.get() in ("", "(its own)") else int(self.v_from.get().split()[0])
        base_words = self.fx.effect_row(base)
        for i, s in enumerate(self.slots[:5 if monster else 1]):
            title = SLOT_TITLES[i] if monster else "EFFECT"
            first, extras = s["rows"][0], s["rows"][1:]
            stacked = []
            for k, row in enumerate(extras):
                if not row["type"].get():
                    raise ValueError(f"{title}: stacked effect {k + 2} has no effect chosen (choose one or Remove it)")
                stacked.append(self.row_spec(i, row, f"{title}, stacked effect {k + 2}"))
            pick = first["pick"].get()
            if pick == NONE_:
                if stacked:
                    raise ValueError(f"{title}: the first effect cannot be (none) when others are stacked on it")
                specs[dlc_effects.SLOT_NAMES[i]] = "none"
                continue
            if pick == KEEP and not first["type"].get():
                if stacked:
                    raise ValueError(f"{title}: the card has no effect here; choose a first effect to stack on")
                continue
            own = self.row_spec(i, first, title)
            if pick == KEEP and not stacked and self.fields(first) == (base_words[2 * i], base_words[2 * i + 1]):
                continue
            specs[dlc_effects.SLOT_NAMES[i]] = [own] + stacked if stacked else own
        if monster:
            if base != n:
                specs["from"] = base
            if specs:
                entry["effects"] = specs
        elif "attack" in specs:
            entry["effect"] = specs["attack"]
        if monster:
            given = {}
            for a, (number, name, _w, _h, rank_high, has_value) in zip(self.abilities, card_effects.ABILITIES):
                state = a["state"].get()
                if state == "none":
                    given[str(number)] = "none"
                elif state == "set":
                    spec = {}
                    if rank_high is not None:
                        if not a["rank"].get().isdigit():
                            raise ValueError(f"leader ability {name}: give the rank it needs (0-12)")
                        spec["rank"] = int(a["rank"].get())
                    if has_value:
                        if not a["value"].get().isdigit():
                            raise ValueError(f"leader ability {name}: give its value")
                        spec["value"] = int(a["value"].get())
                    given[str(number)] = spec
            if given:
                entry["leader"] = {"abilities": given}
        return entry

    def write(self, entries):
        for e in entries:   # resolved as the build will, so an error shows here
            self.fx.entry(e)
        self.effects_path.write_text(json.dumps(entries, indent=2) + "\n", newline="\n")
        self.entries = entries

    def save(self):
        try:
            entry = self.gather()
            rest = [e for e in self.entries if self.fx.number(e["card"]) != self.card()]
            self.write(rest + ([entry] if len(entry) > 1 else []))
        except (ValueError, KeyError) as error:
            messagebox.showerror("Not saved", str(error), parent=self)
            return
        self.status.set("Saved to effects.json." if len(entry) > 1 else "Nothing changed: the card keeps its own.")
        self.load()

    def remove(self):
        self.write([e for e in self.entries if self.fx.number(e["card"]) != self.card()])
        self.load()
        self.status.set("The card has its own effects again.")
