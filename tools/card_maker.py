#!/usr/bin/env python3
"""Card Maker: a window for every card, the game's own and the new cards of the DLC
(mods/widescreen/cards.json), and for building the disc image with them.

The game's cards (0-853) are listed with their own values; saving one writes only what differs
into a cards.json entry of its number (tools/dlc_cards.py load_edits), and "Back to the game's
card" removes it. Every new card number in use (871 up to the end of the placeholder range, 999 skipped) is listed too. One that cards.json does not
name is a placeholder ("Placeholder <number>", the "?" picture, password 00000<number>); saving
a card turns it into a card of its own, and "Back to placeholder" undoes that.

A card copies a donor: an existing card whose card type (monster, magic, ...), effect, 3D model,
animations and sounds it takes. Name, description, password, picture, monster type, attribute,
level, deck cost, attack and defense are the card's own; a field left on "donor's" keeps the
donor's value. Pictures may be PNG, JPG, BMP or GIF: they are converted to PNG in
mods/widescreen/art/ (the game crops them to a square of 128 x 128 pixels).

"Build disc image" runs mods/widescreen/build.sh in Linux (WSL) and shows its output (15-20 minutes).

Run it with "Card Maker.bat" in the project folder, or: python tools/card_maker.py
"""

import json
import queue
import subprocess
import sys
import threading
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox, ttk

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import card_eras  # noqa: E402
import dlc_cards  # noqa: E402
import dlc_frames  # noqa: E402
import dlc_shop  # noqa: E402
import gui_theme  # noqa: E402
import model_texture as mt  # noqa: E402
from card_edit import fit  # noqa: E402

ROOT = TOOLS.parent
MOD = ROOT / "mods" / "widescreen"
CARDS = MOD / "cards.json"
ART = MOD / "art"
EXE = ROOT / "out" / "ntsc-u" / "SLUS_205.15"
PICTURE_ON_DISC = 0x1250800  # DATA/PICTURE.MRG in the untouched disc image
WSL_ROOT = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
# The new card numbers in use (871-998, then 1000 on; 999 is "no card"): up to the highest
# cards.json names or covers with its placeholder range.
NUMBERS = dlc_cards.listed_numbers(CARDS)
# The game's own cards (671 is not one); their changes are cards.json entries of their number.
GAME_NUMBERS = [n for n in range(dlc_cards.ORIGINAL) if n != 671]
DONOR = "(donor's)"
ALLOWED = "letters, digits, space and - . ' ! ,"
RARITY = dlc_shop.rarity_path(MOD)
AUTO = "Automatic"
# The monster frame choices (tools/dlc_frames.py) as the form shows them.
FRAME_NAMES = {"automatic": "Automatic (game's)", "normal": "Normal (orange)", "effect": "Effect (dark orange)",
               "red": "Red", "ritual": "Blue (Ritual)", "link": "Blue (Link)", "white": "White",
               "purple": "Purple", "black": "Black"}
# The eras (tools/card_eras.py) as the form shows them; "automatic" is the card's release year's.
ERA_AUTO = "Automatic (release year)"
ERA_NAMES = {"og": "OG (before 2004)", "gx": "GX (2004-2007)", "5ds": "5D's (2008-2010)",
             "zexal": "ZEXAL (2011-2013)", "arc-v": "ARC-V (2014-2016)", "vrains": "VRAINS (2017-2019)",
             "modern": "Modern (2020 on)"}
# Starting cards (dlc/dlc_options.h DLC_GIVE_ALL_CARDS, DLC_GIVE_ALL_GAME): every new save and
# every load tops the player up to three copies.
OPTIONS_H = MOD / "dlc" / "dlc_options.h"
GIVE_ALL_CHOICES = {"off": "Off (cards are earned)", "all": "Give all (3 of every card)",
                    "dlc": "Give all DLC (3 of every new card)"}


def read_give_all():
    import re
    text = OPTIONS_H.read_text(encoding="utf-8")
    count = re.search(r"#define DLC_GIVE_ALL_CARDS (\d+)", text)
    game = re.search(r"#define DLC_GIVE_ALL_GAME (\d+)", text)
    if not count or int(count.group(1)) == 0:
        return "off"
    return "all" if game and int(game.group(1)) else "dlc"


def write_give_all(choice):
    import re
    text = OPTIONS_H.read_text(encoding="utf-8")
    text = re.sub(r"#define DLC_GIVE_ALL_CARDS \d+", f"#define DLC_GIVE_ALL_CARDS {0 if choice == 'off' else 3}", text)
    text = re.sub(r"#define DLC_GIVE_ALL_GAME \d+", f"#define DLC_GIVE_ALL_GAME {1 if choice == 'all' else 0}", text)
    OPTIONS_H.write_text(text, encoding="utf-8", newline="\n")


def read_rarity():
    """rarity.json as {card number: tier name}."""
    if not RARITY.is_file():
        return {}
    return {int(k): dlc_shop.TIERS[dlc_shop.tier_index(v)] for k, v in json.loads(RARITY.read_text()).items()
            if k.isdigit()}


def read_not_in_shop():
    """The cards the card shop never offers (rarity.json's not_in_shop)."""
    return dlc_shop.not_in_shop(MOD)


def write_rarity(tiers, kept_out=None):
    """rarity.json: the tiers, and the cards kept out of the shop (unchanged when kept_out is None)."""
    if kept_out is None:
        kept_out = read_not_in_shop()
    data = {str(k): tiers[k] for k in sorted(tiers)}
    if kept_out:
        data[dlc_shop.NOT_IN_SHOP] = sorted(kept_out)
    RARITY.write_text(json.dumps(data, indent=2) + "\n", newline="\n")


class RarityWindow(tk.Toplevel):
    """The shop rarity of every card, the game's and the new ones (rarity.json). A card without one
    of its own gets one from its deck cost (tools/dlc_shop.py)."""

    def __init__(self, maker):
        super().__init__(maker)
        self.title("Shop rarity - Duelists of the Roses")
        self.minsize(560, 520)
        self.maker = maker
        self.tiers = read_rarity()
        self.auto = dlc_shop.automatic(dlc_shop.card_costs(MOD, maker.exe))
        self.names = {n: maker.names[n] for n in range(854) if n != 671}
        for number, card in maker.explicit().items():
            self.names[number] = card.get("name", f"Card {number}")
        for n in self.auto:
            self.names.setdefault(n, f"Placeholder {n}")
        self.numbers = sorted(self.auto)
        self.shown = []
        chances = ", ".join(f"{t} {c:g}%" for t, c in zip(dlc_shop.TIERS, dlc_shop.CHANCES))
        ttk.Label(self, text="Each of the shop's 25 places draws a rarity by its chance, then a card of it:\n"
                             + chances, foreground=gui_theme.HINT, justify="left").pack(anchor="w", padx=8, pady=4)
        top = ttk.Frame(self)
        top.pack(fill="x", padx=8)
        ttk.Label(top, text="Find").pack(side="left")
        self.find = tk.StringVar()
        self.find.trace_add("write", lambda *a: self.refresh())
        ttk.Entry(top, textvariable=self.find, width=24).pack(side="left", padx=6)
        self.only_set = tk.BooleanVar()
        ttk.Checkbutton(top, text="Only cards set by hand", variable=self.only_set,
                        command=self.refresh).pack(side="left")
        self.list = tk.Listbox(self, height=22, font=("Consolas", 10), exportselection=False, selectmode="extended")
        self.list.pack(fill="both", expand=True, padx=8, pady=4)
        bottom = ttk.Frame(self)
        bottom.pack(fill="x", padx=8, pady=6)
        ttk.Label(bottom, text="Selected cards:").pack(side="left")
        self.choice = tk.StringVar(value=dlc_shop.TIERS[0])
        ttk.Combobox(bottom, textvariable=self.choice, state="readonly", width=14,
                     values=dlc_shop.TIERS + [AUTO]).pack(side="left", padx=6)
        ttk.Button(bottom, text="Set", command=self.set_selected).pack(side="left")
        ttk.Button(bottom, text="Take out of shop", command=lambda: self.set_sold(False)).pack(side="left", padx=(12, 0))
        ttk.Button(bottom, text="Put back in shop", command=lambda: self.set_sold(True)).pack(side="left", padx=4)
        self.kept_out = read_not_in_shop()
        self.status = tk.StringVar()
        ttk.Label(bottom, textvariable=self.status, foreground=gui_theme.STATUS).pack(side="left", padx=10)
        self.refresh()

    def tier_of(self, n):
        return self.tiers.get(n) or dlc_shop.TIERS[self.auto[n]]

    def refresh(self):
        typed = self.find.get().strip().lower()
        self.list.delete(0, "end")
        self.shown = []
        for n in self.numbers:
            name = self.names.get(n, "")
            if typed and typed not in name.lower() and typed != str(n):
                continue
            if self.only_set.get() and n not in self.tiers:
                continue
            mark = "" if n in self.tiers else "  (auto)"
            if n in self.kept_out:
                mark += "  NOT SOLD"
            self.list.insert("end", f"{n:03d}  {name[:30]:30}  {self.tier_of(n)}{mark}")
            if n not in self.tiers or n in self.kept_out:
                self.list.itemconfig("end", foreground=gui_theme.HINT)
            self.shown.append(n)
        counts = [sum(1 for n in self.numbers if self.tier_of(n) == t and n not in self.kept_out)
                  for t in dlc_shop.TIERS]
        self.status.set("  ".join(f"{t}: {c}" for t, c in zip(dlc_shop.TIERS, counts))
                        + f"  Not sold: {len(self.kept_out)}")

    def set_selected(self):
        picked = [self.shown[i] for i in self.list.curselection()]
        if not picked:
            return
        for n in picked:
            if self.choice.get() == AUTO:
                self.tiers.pop(n, None)
            else:
                self.tiers[n] = self.choice.get()
        write_rarity(self.tiers, self.kept_out)
        keep = self.list.yview()[0]
        self.refresh()
        self.list.yview_moveto(keep)
        self.maker.on_select()

    def set_sold(self, sold):
        """The selected cards in (sold) or out of the card shop; their tiers are kept."""
        picked = [self.shown[i] for i in self.list.curselection()]
        if not picked:
            return
        for n in picked:
            if sold:
                self.kept_out.discard(n)
            else:
                self.kept_out.add(n)
        write_rarity(self.tiers, self.kept_out)
        keep = self.list.yview()[0]
        self.refresh()
        self.list.yview_moveto(keep)
        self.maker.on_select()


def clean_image():
    for path in sorted((ROOT / "baserom").glob("*.iso")):
        return path
    return None


class CardMaker(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Card Maker - Duelists of the Roses")
        self.minsize(980, 640)
        gui_theme.apply(self)   # follows Windows' light or dark app mode
        self.exe = dlc_cards.game_exe(EXE)   # with SPEX, as the game plays
        self.text = dlc_cards.Text(self.exe)
        self.names = [self.text.decode(dlc_cards.NAMES + n) or f"Card {n}" for n in range(854)]
        meta = dlc_cards.inventory(self.exe)
        self.records = [dlc_cards.record(self.exe, n, meta) for n in range(854)]
        self.descriptions = [self.text.decode(dlc_cards.DESCRIPTIONS + n) for n in range(854)]
        self.passwords = [dlc_cards.game_password(self.exe, n, meta) for n in range(854)]
        self.shown = []
        self.donor_values = [f"{n:03d}  {self.names[n]}" for n in range(854) if n != 671]
        self.iso = clean_image()
        self.entries = json.loads(CARDS.read_text())
        self.number = None
        self.preview_image = None
        self._build_ui()
        self.refresh_list()
        self.list.selection_set(0)
        self.on_select()

    # ---------------------------------------------------------------- data
    def placeholder_entry(self):
        for e in self.entries:
            if "placeholders" in e:
                return e
        return None

    def reload(self):
        """cards.json as it is now on disk: another tool (or a second Card Maker) may have changed it
        since, and a save must not write an old copy back over that."""
        try:
            self.entries = json.loads(CARDS.read_text())
        except (OSError, ValueError):
            pass

    def explicit(self):
        return {int(e["number"]): e for e in self.entries if "number" in e}

    def card(self, number):
        """The card as cards.json gives it (a placeholder filled in), and whether it is its own."""
        own = self.explicit().get(number)
        if number < dlc_cards.ORIGINAL:
            # One of the game's cards: its own values, with cards.json's changes over them.
            card = self.game_card(number)
            card.update(own or {})
            return card, own is not None
        if own is not None:
            return dict(own), True
        p = self.placeholder_entry() or {"donor": 35}
        card = {k: v for k, v in p.items() if k != "placeholders"}
        card.update(number=number, name=f"Placeholder {number}", password=f"{number:08d}")
        card.setdefault("description", f"Placeholder card {number}.\nTo be replaced.")
        return card, False

    def game_card(self, number):
        """One of the game's cards as the game has it, in cards.json's fields."""
        r = self.records[number]
        card = dict(number=number, donor=number, name=self.names[number],
                    description=self.descriptions[number], password=self.passwords[number],
                    deck_cost=r["deck_cost"])
        if r["monster"]:
            card.update(monster_type=dlc_cards.MONSTER_TYPES[r["monster_type"]] if r["monster_type"] < 21 else None,
                        attribute=dlc_cards.ATTRIBUTES[r["attribute"]] if r["attribute"] < 6 else None,
                        level=r["level"], attack=r["attack"], defense=r["defense"])
        return card

    def donor_number(self, donor):
        if isinstance(donor, int):
            return donor
        if isinstance(donor, str) and donor.strip()[:3].isdigit():
            return int(donor.strip()[:3])
        lowered = [n.lower() for n in self.names]
        return lowered.index(donor.strip().lower()) if donor and donor.strip().lower() in lowered else 35

    # ---------------------------------------------------------------- window
    def _build_ui(self):
        pad = dict(padx=6, pady=3)
        left = ttk.Frame(self)
        left.pack(side="left", fill="y", padx=8, pady=8)
        ttk.Label(left, text=f"Every card: the game's (0-853) and new ones ({NUMBERS[0]}-{NUMBERS[-1]})").pack(anchor="w")
        find = ttk.Frame(left)
        find.pack(fill="x", pady=(2, 4))
        ttk.Label(find, text="Find").pack(side="left")
        self.find = tk.StringVar()
        self.find.trace_add("write", lambda *a: self.refresh_list(select_first=True))
        ttk.Entry(find, textvariable=self.find, width=20).pack(side="left", padx=6)
        self.show = tk.StringVar(value="All cards")
        show = ttk.Combobox(find, textvariable=self.show, state="readonly", width=14,
                            values=["All cards", "Game's cards", "New cards", "Changed cards"])
        show.pack(side="left")
        show.bind("<<ComboboxSelected>>", lambda e: self.refresh_list(select_first=True))
        ttk.Label(left, text="* changed from the game's", foreground=gui_theme.HINT).pack(anchor="w")
        self.list = tk.Listbox(left, width=40, height=30, exportselection=False, font=("Consolas", 10))
        self.list.pack(fill="y", expand=True)
        self.list.bind("<<ListboxSelect>>", lambda e: self.on_select())

        right = ttk.Frame(self)
        right.pack(side="left", fill="both", expand=True, padx=8, pady=8)
        form = ttk.Frame(right)
        form.pack(side="top", fill="x")
        fields = ttk.Frame(form)
        fields.pack(side="left", fill="x", expand=True)

        self.v = {k: tk.StringVar() for k in ("number", "name", "password", "donor", "kind", "monster_type",
                                                "attribute", "level", "deck_cost", "attack", "defense", "art",
                                                "status", "output", "effect_from", "model", "model_texture",
                                                "model_size", "rarity", "frame", "era", "give_all")}
        self.arms_down = tk.BooleanVar()
        self.turn = tk.BooleanVar()
        row = 0

        def label(text):
            nonlocal row
            ttk.Label(fields, text=text).grid(row=row, column=0, sticky="e", **pad)

        label("Card number")
        ttk.Label(fields, textvariable=self.v["number"], font=("Segoe UI", 10, "bold")).grid(row=row, column=1, sticky="w", **pad)
        row += 1
        label("Name")
        ttk.Entry(fields, textvariable=self.v["name"], width=40).grid(row=row, column=1, sticky="we", **pad)
        row += 1
        label("Description")
        self.desc = tk.Text(fields, width=40, height=4, font=("Segoe UI", 9))
        self.desc.grid(row=row, column=1, sticky="we", **pad)
        row += 1
        ttk.Label(fields, text=f"Text may use {ALLOWED}. Enter starts a new line.", foreground=gui_theme.HINT).grid(
            row=row, column=1, sticky="w", padx=6)
        row += 1
        label("Password")
        pw = ttk.Frame(fields)
        pw.grid(row=row, column=1, sticky="w", **pad)
        ttk.Entry(pw, textvariable=self.v["password"], width=12).pack(side="left")
        ttk.Button(pw, text="00000 + number", command=self.default_password).pack(side="left", padx=6)
        ttk.Label(pw, text="8 digits or letters; empty = none").pack(side="left")
        row += 1
        label("Donor card")
        donor = ttk.Combobox(fields, textvariable=self.v["donor"], values=self.donor_values, width=38)
        donor.grid(row=row, column=1, sticky="we", **pad)
        self.donor_box = donor
        gui_theme.wide_dropdown(donor)
        donor.bind("<<ComboboxSelected>>", lambda e: self.on_donor())
        donor.bind("<Return>", lambda e: self.find_donor(donor))
        row += 1
        ttk.Label(fields, textvariable=self.v["kind"], foreground=gui_theme.HINT, wraplength=360, justify="left").grid(
            row=row, column=1, sticky="w", padx=6)
        row += 1
        label("Monster type")
        self.monster_type = ttk.Combobox(fields, textvariable=self.v["monster_type"], state="readonly", width=20,
                                         values=[DONOR] + dlc_cards.MONSTER_TYPES)
        self.monster_type.grid(row=row, column=1, sticky="w", **pad)
        row += 1
        label("Attribute")
        self.attribute = ttk.Combobox(fields, textvariable=self.v["attribute"], state="readonly", width=20,
                                      values=[DONOR] + dlc_cards.ATTRIBUTES)
        self.attribute.grid(row=row, column=1, sticky="w", **pad)
        row += 1
        label("Level")
        self.level = ttk.Combobox(fields, textvariable=self.v["level"], state="readonly", width=20,
                                  values=[DONOR] + [str(n) for n in range(1, 13)])
        self.level.grid(row=row, column=1, sticky="w", **pad)
        row += 1
        label("Frame colour")
        self.frame = ttk.Combobox(fields, textvariable=self.v["frame"], state="readonly", width=20,
                                  values=[FRAME_NAMES[f] for f in dlc_frames.FRAMES])
        self.frame.grid(row=row, column=1, sticky="w", **pad)
        row += 1
        label("Era")
        self.era = ttk.Combobox(fields, textvariable=self.v["era"], state="readonly", width=26,
                                values=[ERA_AUTO] + [ERA_NAMES[e] for e in card_eras.ERAS])
        self.era.grid(row=row, column=1, sticky="w", **pad)
        row += 1
        for key, text in (("deck_cost", "Deck cost (0-255)"), ("attack", "Attack (0-8191)"), ("defense", "Defense (0-8191)")):
            label(text)
            ttk.Entry(fields, textvariable=self.v[key], width=10).grid(row=row, column=1, sticky="w", **pad)
            row += 1
        ttk.Label(fields, text="Leave a number empty to keep the donor's.", foreground=gui_theme.HINT).grid(
            row=row, column=1, sticky="w", padx=6)
        row += 1
        label("Effect")
        self.effect = ttk.Combobox(fields, textvariable=self.v["effect_from"], width=38,
                                   values=[DONOR] + self.donor_values)
        self.effect.grid(row=row, column=1, sticky="we", **pad)
        gui_theme.wide_dropdown(self.effect)
        row += 1
        ttk.Button(fields, text="Effects and leader abilities...", command=self.open_effects).grid(
            row=row, column=1, sticky="w", **pad)
        row += 1
        label("Shop rarity")
        rarity = ttk.Frame(fields)
        rarity.grid(row=row, column=1, sticky="w", **pad)
        ttk.Combobox(rarity, textvariable=self.v["rarity"], state="readonly", width=16,
                     values=[AUTO] + dlc_shop.TIERS).pack(side="left")
        self.in_shop = tk.BooleanVar(value=True)
        ttk.Checkbutton(rarity, text="Sold in the card shop", variable=self.in_shop).pack(side="left", padx=8)
        ttk.Button(rarity, text="Every card's rarity...", command=lambda: RarityWindow(self)).pack(side="left", padx=6)
        row += 1
        label("Picture")
        art = ttk.Frame(fields)
        art.grid(row=row, column=1, sticky="we", **pad)
        ttk.Entry(art, textvariable=self.v["art"], width=28, state="readonly").pack(side="left")
        ttk.Button(art, text="Choose...", command=self.choose_art).pack(side="left", padx=4)
        ttk.Button(art, text="Donor's", command=lambda: self.set_art("")).pack(side="left")
        row += 1
        label("3D model")
        model = ttk.Frame(fields)
        model.grid(row=row, column=1, sticky="we", **pad)
        ttk.Entry(model, textvariable=self.v["model"], width=28, state="readonly").pack(side="left")
        ttk.Button(model, text="Choose...", command=self.choose_model).pack(side="left", padx=4)
        ttk.Button(model, text="Donor's", command=lambda: (self.v["model"].set(""), self.v["model_texture"].set(""))
                   ).pack(side="left")
        row += 1
        label("Model texture")
        mtex = ttk.Frame(fields)
        mtex.grid(row=row, column=1, sticky="we", **pad)
        ttk.Entry(mtex, textvariable=self.v["model_texture"], width=28, state="readonly").pack(side="left")
        ttk.Button(mtex, text="Choose...", command=self.choose_model_texture).pack(side="left", padx=4)
        row += 1
        options = ttk.Frame(fields)
        options.grid(row=row, column=1, sticky="w", **pad)
        ttk.Checkbutton(options, text="Lower the arms (model stored in a T-pose, FBX)", variable=self.arms_down).pack(side="left")
        ttk.Checkbutton(options, text="Turn around", variable=self.turn).pack(side="left", padx=8)
        row += 1
        label("Model size")
        size = ttk.Frame(fields)
        size.grid(row=row, column=1, sticky="we", **pad)
        ttk.Spinbox(size, textvariable=self.v["model_size"], from_=10, to=500, increment=5, width=6).pack(side="left")
        ttk.Label(size, text="% of the donor's height (tools/model_sizes.py sets these; a size you change here is kept)",
                  foreground=gui_theme.HINT).pack(side="left", padx=6)
        row += 1
        ttk.Label(fields, text="FBX (binary) or OBJ, one texture. It moves with the donor's animations.",
                  foreground=gui_theme.HINT).grid(row=row, column=1, sticky="w", padx=6)
        row += 1
        fields.columnconfigure(1, weight=1)

        side = ttk.Frame(form)
        side.pack(side="left", padx=12, anchor="n")
        ttk.Label(side, text="Picture in the game (128 x 128)").pack()
        self.preview = tk.Label(side, width=256, height=256, bg="#222")
        self.preview.pack(pady=4)

        buttons = ttk.Frame(right)
        buttons.pack(side="top", fill="x", pady=6)
        ttk.Button(buttons, text="Save card", command=self.save_card).pack(side="left")
        self.reset_button = ttk.Button(buttons, text="Back to placeholder", command=self.reset_card)
        self.reset_button.pack(side="left", padx=6)
        ttk.Label(buttons, textvariable=self.v["status"], foreground=gui_theme.STATUS).pack(side="left", padx=10)

        build = ttk.LabelFrame(right, text="Disc image")
        build.pack(side="top", fill="both", expand=True, pady=6)
        top = ttk.Frame(build)
        top.pack(fill="x", padx=6, pady=4)
        ttk.Label(top, text="File name").pack(side="left")
        self.v["output"].set("dotr-cards.iso")
        ttk.Entry(top, textvariable=self.v["output"], width=28).pack(side="left", padx=6)
        self.build_button = ttk.Button(top, text="Build disc image", command=self.build)
        self.build_button.pack(side="left")
        ttk.Button(top, text="Open folder", command=self.open_folder).pack(side="left", padx=6)
        start = ttk.Frame(build)
        start.pack(fill="x", padx=6)
        ttk.Label(start, text="Starting cards").pack(side="left")
        self.v["give_all"].set(GIVE_ALL_CHOICES[read_give_all()])
        give = ttk.Combobox(start, textvariable=self.v["give_all"], state="readonly", width=34,
                            values=list(GIVE_ALL_CHOICES.values()))
        give.pack(side="left", padx=6)
        give.bind("<<ComboboxSelected>>", lambda e: self.set_give_all())
        ttk.Label(start, text="(for testing; Off for a release)", foreground=gui_theme.HINT).pack(side="left")
        self.log = tk.Text(build, height=10, font=("Consolas", 9), bg="#111", fg="#ddd")
        self.log.pack(fill="both", expand=True, padx=6, pady=4)

    def refresh_list(self, select_first=False):
        own = self.explicit()
        typed = self.find.get().strip().lower()
        show = self.show.get()
        self.list.delete(0, "end")
        self.shown = []
        for n in GAME_NUMBERS + list(NUMBERS):
            game = n < dlc_cards.ORIGINAL
            if (show == "Game's cards" and not game) or (show == "New cards" and game) \
                    or (show == "Changed cards" and n not in own):
                continue
            if game:
                name = own.get(n, {}).get("name") or self.names[n]
            else:
                name = own[n].get("name", "") if n in own else "Placeholder"
            if typed and typed not in name.lower() and typed != str(n):
                continue
            mark = " *" if game and n in own else ""
            self.list.insert("end", f"{n:03d}  {name}{mark}")
            if not game and n not in own:
                self.list.itemconfig("end", foreground=gui_theme.HINT)
            self.shown.append(n)
        if self.number in self.shown and not select_first:
            at = self.shown.index(self.number)
        elif self.shown and select_first:
            at = 0
        else:
            return
        self.list.selection_set(at)
        self.list.see(at)
        if select_first:
            self.on_select()

    # ---------------------------------------------------------------- one card
    def on_select(self):
        sel = self.list.curselection()
        if not sel:
            return
        self.number = self.shown[sel[0]]
        self.reload()
        card, own = self.card(self.number)
        donor = self.donor_number(card.get("donor", 35))
        game = self.number < dlc_cards.ORIGINAL
        if game:
            note = "   (the game's card, changed)" if own else "   (the game's card: save to change it)"
        else:
            note = "" if own else "   (placeholder: save to make it a card of its own)"
        self.v["number"].set(f"{self.number}{note}")
        self.donor_box.configure(state="disabled" if game else "normal")
        self.reset_button.configure(text="Back to the game's card" if game else "Back to placeholder")
        self.v["name"].set(card.get("name", ""))
        self.desc.delete("1.0", "end")
        self.desc.insert("1.0", card.get("description", ""))
        self.v["password"].set(card.get("password", ""))
        self.v["donor"].set(f"{donor:03d}  {self.names[donor]}")
        self.v["monster_type"].set(self._name_or_donor(card.get("monster_type"), dlc_cards.MONSTER_TYPES))
        self.v["attribute"].set(self._name_or_donor(card.get("attribute"), dlc_cards.ATTRIBUTES))
        self.v["level"].set(str(card["level"]) if card.get("level") not in (None, "") else DONOR)
        for key in ("deck_cost", "attack", "defense"):
            self.v[key].set("" if card.get(key) in (None, "") else str(card[key]))
        self.v["art"].set(card.get("art", "") or "")
        effect = card.get("effect_from")
        if effect in (None, ""):
            self.v["effect_from"].set(DONOR)
        else:
            e = self.donor_number(effect)
            self.v["effect_from"].set(f"{e:03d}  {self.names[e]}")
        self.v["model"].set(card.get("model", "") or "")
        self.v["model_texture"].set(card.get("model_texture", "") or "")
        self.arms_down.set(bool(card.get("model_arms_down")))
        self.turn.set(bool(card.get("model_turn")))
        self.v["model_size"].set(str(round(float(card.get("model_size", 1.0)) * 100)))
        self.v["rarity"].set(read_rarity().get(self.number, AUTO))
        frame = str(card.get("frame", "automatic")).lower()
        self.v["frame"].set(FRAME_NAMES.get(dlc_frames.FRAME_ALIASES.get(frame, frame), FRAME_NAMES["automatic"]))
        self.in_shop.set(self.number not in read_not_in_shop())
        # Era: the game's cards are all OG; a new card's is its release year's unless chosen here.
        if game:
            self.v["era"].set(ERA_NAMES["og"])
            self.era.configure(state="disabled")
        else:
            self.era.configure(state="readonly")
            era = str(card.get("era", "")).lower()
            self.v["era"].set(ERA_NAMES[era] if card.get("era_manual") and era in ERA_NAMES else ERA_AUTO)
        self.v["status"].set("")
        self.on_donor()

    def set_give_all(self):
        choice = next(k for k, v in GIVE_ALL_CHOICES.items() if v == self.v["give_all"].get())
        write_give_all(choice)
        self.v["status"].set(f"Starting cards: {self.v['give_all'].get()} (in the next build).")

    @staticmethod
    def _name_or_donor(value, names):
        if value in (None, ""):
            return DONOR
        return names[value] if isinstance(value, int) else value

    def find_donor(self, box):
        typed = self.v["donor"].get().strip().lower()
        matches = [v for v in self.donor_values if typed in v.lower()]
        if matches:
            box.configure(values=matches)
            box.event_generate("<Down>")
        else:
            box.configure(values=self.donor_values)

    def on_donor(self):
        donor = self.donor_number(self.v["donor"].get())
        r = self.records[donor]
        kind = dlc_cards.CARD_TYPES[r["card_type"]] if r["card_type"] < len(dlc_cards.CARD_TYPES) else "?"
        if self.number is not None and self.number < dlc_cards.ORIGINAL:
            info = (f"The game's card: {kind}, effect {'none' if r['effect'] == 0xffff else 'yes'}. Its own values: "
                    + (f"{dlc_cards.MONSTER_TYPES[r['monster_type']] if r['monster_type'] < 21 else '?'}, "
                       f"{dlc_cards.ATTRIBUTES[r['attribute']] if r['attribute'] < 6 else '?'}, level {r['level']}, "
                       f"cost {r['deck_cost']}, {r['attack']} / {r['defense']}." if r["monster"]
                       else f"deck cost {r['deck_cost']}."))
        elif r["monster"]:
            info = (f"From the donor: {kind}, effect {'none' if r['effect'] == 0xffff else 'yes'}, its 3D model. "
                    f"Its values: {dlc_cards.MONSTER_TYPES[r['monster_type']] if r['monster_type'] < 21 else '?'}, "
                    f"{dlc_cards.ATTRIBUTES[r['attribute']] if r['attribute'] < 6 else '?'}, level {r['level']}, "
                    f"cost {r['deck_cost']}, {r['attack']} / {r['defense']}.")
        else:
            info = f"From the donor: {kind} and its effect. Its deck cost: {r['deck_cost']}."
        self.v["kind"].set(info)
        state = "readonly" if r["monster"] else "disabled"
        for box in (self.monster_type, self.attribute, self.level):
            box.configure(state=state)
            if not r["monster"]:
                box.set(DONOR)
        # Frame colours are for monster cards.
        self.frame.configure(state=state)
        if not r["monster"]:
            self.frame.set(FRAME_NAMES["automatic"])
        # The effect may come from another card of the same kind (monster or not).
        self.effect.configure(values=[DONOR] + [v for v in self.donor_values
                                                if (int(v[:3]) < 683) == r["monster"]])
        self.show_preview()

    def open_effects(self):
        """The Effects window (tools/card_effects_window.py) on this card; any card can be chosen there.
        A new card must be saved first (its donor decides whether it is a monster)."""
        import card_effects_window
        if self.number is not None and self.number not in self.explicit():
            messagebox.showinfo("Effects", "Save this card first; the Effects window then opens on it.")
            return
        try:
            card_effects_window.EffectsWindow(self, self.exe, MOD / "effects.json", CARDS, card=self.number)
        except (ValueError, KeyError, OSError) as error:
            messagebox.showerror("Effects", str(error))

    def copy_into_mod(self, path, folder):
        """Copies a file the card uses into the mod (so the build finds it); returns the path
        relative to the mod."""
        target = MOD / folder
        target.mkdir(parents=True, exist_ok=True)
        out = target / Path(path).name
        if Path(path).resolve() != out.resolve():
            out.write_bytes(Path(path).read_bytes())
        return out.relative_to(MOD).as_posix()

    def choose_model(self):
        path = filedialog.askopenfilename(title="3D model for the card", filetypes=[
            ("3D models", "*.fbx *.obj"), ("All files", "*.*")])
        if not path:
            return
        if path.lower().endswith(".fbx"):
            import fbx_read
            try:
                report = {}
                fbx_read.load_fbx(path, 0, report)
            except (OSError, ValueError) as error:
                messagebox.showerror("3D model", str(error))
                return
            self.v["status"].set(f"Model: {report['triangles']} triangles.")
        self.v["model"].set(self.copy_into_mod(path, f"models/card_{self.number}"))

    def choose_model_texture(self):
        path = filedialog.askopenfilename(title="Texture of the 3D model", filetypes=[
            ("PNG pictures", "*.png"), ("All files", "*.*")])
        if path:
            self.v["model_texture"].set(self.copy_into_mod(path, f"models/card_{self.number}"))

    def default_password(self):
        if self.number is not None:
            self.v["password"].set(f"{self.number:08d}")

    def choose_art(self):
        path = filedialog.askopenfilename(title="Picture for the card", filetypes=[
            ("Pictures", "*.png *.jpg *.jpeg *.bmp *.gif"), ("All files", "*.*")])
        if not path:
            return
        ART.mkdir(exist_ok=True)
        out = ART / f"card_{self.number}.png"
        source = str(Path(path)).replace("'", "''")
        target = str(out).replace("'", "''")
        script = (
            "Add-Type -AssemblyName System.Drawing; "
            f"$i = [System.Drawing.Image]::FromFile('{source}'); "
            "$s = [Math]::Min(1.0, 512.0 / [Math]::Max($i.Width, $i.Height)); "
            "$w = [int]($i.Width * $s); $h = [int]($i.Height * $s); "
            "$b = New-Object System.Drawing.Bitmap($w, $h, [System.Drawing.Imaging.PixelFormat]::Format24bppRgb); "
            "$g = [System.Drawing.Graphics]::FromImage($b); "
            "$g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic; "
            "$g.DrawImage($i, 0, 0, $w, $h); "
            f"$b.Save('{target}', [System.Drawing.Imaging.ImageFormat]::Png); $g.Dispose(); $b.Dispose(); $i.Dispose()")
        result = subprocess.run(["powershell", "-NoProfile", "-Command", script], capture_output=True, text=True,
                                creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
        if result.returncode != 0 or not out.is_file():
            messagebox.showerror("Picture", "Could not read that picture:\n" + (result.stderr or "")[-600:])
            return
        self.set_art(out.relative_to(MOD).as_posix())

    def set_art(self, relative):
        self.v["art"].set(relative)
        self.show_preview()

    def show_preview(self):
        rows = None
        try:
            art = self.v["art"].get()
            if art:
                width, height, pixels = mt.read_png(MOD / art)
                rows = fit(pixels, width, height, 128, 128)
            elif self.iso is not None:
                donor = self.donor_number(self.v["donor"].get())
                with open(self.iso, "rb") as stream:
                    stream.seek(PICTURE_ON_DISC + donor * dlc_cards.PICTURE_SIZE)
                    data = stream.read(dlc_cards.PICTURE_SIZE)
                _, _, rows = mt.decode(data, 0)
        except (OSError, ValueError) as error:
            self.v["status"].set(f"Picture: {error}")
        image = tk.PhotoImage(width=128, height=128)
        if rows:
            image.put(" ".join("{" + " ".join("#%02x%02x%02x" % tuple(p[:3]) for p in row) + "}" for row in rows))
        self.preview_image = image.zoom(2)
        self.preview.configure(image=self.preview_image, width=256, height=256)

    def gather(self):
        """The card from the form, as a cards.json entry."""
        card = {"number": self.number, "donor": self.donor_number(self.v["donor"].get()),
                "name": self.v["name"].get().strip(),
                "description": self.desc.get("1.0", "end").rstrip("\n").strip()}
        if not card["name"]:
            raise ValueError("the card needs a name")
        if self.v["password"].get().strip():
            card["password"] = self.v["password"].get().strip()
        for key in ("monster_type", "attribute"):
            if self.v[key].get() not in ("", DONOR):
                card[key] = self.v[key].get()
        if self.v["level"].get() not in ("", DONOR):
            card["level"] = int(self.v["level"].get())
        for key in ("deck_cost", "attack", "defense"):
            value = self.v[key].get().strip()
            if value:
                if not value.isdigit():
                    raise ValueError(f"{key.replace('_', ' ')} must be a number")
                card[key] = int(value)
        if self.v["art"].get():
            card["art"] = self.v["art"].get()
        if self.v["effect_from"].get() not in ("", DONOR):
            card["effect_from"] = self.donor_number(self.v["effect_from"].get())
        frame = next((k for k, v in FRAME_NAMES.items() if v == self.v["frame"].get()), "automatic")
        if frame != "automatic" and self.records[card["donor"]]["monster"]:
            card["frame"] = frame
        if self.v["model"].get():
            card["model"] = self.v["model"].get()
            if self.v["model_texture"].get():
                card["model_texture"] = self.v["model_texture"].get()
            if self.arms_down.get():
                card["model_arms_down"] = 65
            if self.turn.get():
                card["model_turn"] = True
            text = self.v["model_size"].get().strip().rstrip("%") or "100"
            if not text.isdigit() or not 10 <= int(text) <= 500:
                raise ValueError("model size must be a percent from 10 to 500")
            old = self.explicit().get(self.number, {})
            before = round(float(old.get("model_size", 1.0)) * 100)
            if int(text) != 100 or "model_size" in old:
                card["model_size"] = int(text) / 100
            # A size changed here is the user's: tools/model_sizes.py --write leaves it.
            if int(text) != before or old.get("model_size_fixed"):
                card["model_size_fixed"] = True
        if not card["description"]:
            card["description"] = card["name"]
        # Fields the form does not show (model_up, model_front, model_size ...) are kept.
        shown = {"name", "description", "password", "donor", "monster_type", "attribute", "level", "deck_cost", "frame",
                 "attack", "defense", "art", "effect_from", "model", "model_texture", "model_arms_down", "model_turn",
                 "model_size", "model_size_fixed", "era", "era_manual"}
        old = self.explicit().get(self.number, {})
        if self.number >= dlc_cards.FIRST:
            era = next((k for k, v in ERA_NAMES.items() if v == self.v["era"].get()), None)
            if era:                          # chosen here: tools/card_eras.py leaves it
                card["era"] = era
                card["era_manual"] = True
            else:                            # its release year's
                card["era"] = old.get("era") if old.get("era") and not old.get("era_manual") else card_eras.era_for(card["name"])
        if card.get("model"):
            card.update({k: v for k, v in old.items() if k not in shown and k != "number"})
        else:
            card.update({k: v for k, v in old.items() if k not in shown and k != "number" and not k.startswith("model")})
        if old.get("model_arms_down") and self.arms_down.get():
            card["model_arms_down"] = old["model_arms_down"]
        if self.number < dlc_cards.ORIGINAL:
            card = self.game_changes(card)
        return card

    def game_changes(self, card):
        """For one of the game's cards: only what differs from the game's own values."""
        n = self.number
        game = self.game_card(n)
        changes = {"number": n}
        for key, value in card.items():
            if key in ("number", "donor"):
                continue
            if key == "description" and value == card.get("name") and self.desc.get("1.0", "end").strip() == "":
                continue                                  # left empty: the game's text
            if key in ("name", "description"):
                if key in game and game[key].strip() == str(value).strip():
                    continue
            elif key in game and str(game[key]).lower() == str(value).lower():
                continue
            changes[key] = value
        # A password taken away is kept as "" (none), as the game's card has one.
        if self.passwords[n] and not self.v["password"].get().strip():
            changes["password"] = ""
        return changes

    def write(self, entries):
        """Checks the whole file as the build would, then writes it."""
        check = MOD / ".cards_check.json"
        check.write_text(json.dumps(entries, indent=2), newline="\n")
        try:
            cards = dlc_cards.load_cards(check, self.text)
            for c in cards:
                self.text.encode(c["name"])
                self.text.encode(c.get("description", c["name"]))
            for c in dlc_cards.load_edits(check, self.text):
                for key in ("name", "description"):
                    if c.get(key):
                        self.text.encode(c[key])
        finally:
            check.unlink()
        CARDS.write_text(json.dumps(entries, indent=2) + "\n", newline="\n")
        self.entries = entries

    def save_card(self):
        try:
            self.reload()
            card = self.gather()
            entries = [e for e in self.entries if e.get("number") != self.number]
            # A game card with nothing changed needs no entry.
            mine = [card] if len(card) > 1 or self.number >= dlc_cards.ORIGINAL else []
            explicit = sorted([e for e in entries if "number" in e] + mine, key=lambda e: e["number"])
            rest = [e for e in entries if "number" not in e]
            self.write(explicit + rest)
            tiers = read_rarity()
            if self.v["rarity"].get() in dlc_shop.TIERS:
                tiers[self.number] = self.v["rarity"].get()
            else:
                tiers.pop(self.number, None)
            kept_out = read_not_in_shop()
            if self.in_shop.get():
                kept_out.discard(self.number)
            else:
                kept_out.add(self.number)
            write_rarity(tiers, kept_out)
        except (ValueError, KeyError) as error:
            messagebox.showerror("Card not saved", str(error))
            return
        self.refresh_list()
        self.on_select()
        self.v["status"].set(f"Card {self.number} saved.")

    def reset_card(self):
        self.reload()
        if self.number not in self.explicit():
            return
        game = self.number < dlc_cards.ORIGINAL
        title = "Back to the game's card" if game else "Back to placeholder"
        question = (f"Undo every change to card {self.number}?" if game
                    else f"Turn card {self.number} back into a placeholder?")
        if not messagebox.askyesno(title, question):
            return
        self.write([e for e in self.entries if e.get("number") != self.number])
        self.refresh_list()
        self.on_select()
        self.v["status"].set(f"Card {self.number} is the game's again." if game
                             else f"Card {self.number} is a placeholder again.")

    # ---------------------------------------------------------------- building
    def build(self):
        name = self.v["output"].get().strip()
        if not name.lower().endswith(".iso") or "/" in name or "\\" in name or " " in name:
            messagebox.showerror("Disc image", "Give a file name ending in .iso, without spaces.")
            return
        self.build_button.configure(state="disabled")
        self.log.delete("1.0", "end")
        self.log.insert("end", "Building (15-20 minutes; the window stays usable)...\n")
        target = f"build/ntsc-u/mods/obelisk-test/{name}"
        command = (f"cd '{WSL_ROOT}' && for f in mods/widescreen/cards.json mods/widescreen/effects.json "
                   f"mods/widescreen/dlc/*.h mods/widescreen/build.sh; "
                   f"do sed -i 's/\\r//' \"$f\"; done && TEST={target} bash mods/widescreen/build.sh 2>&1; "
                   f"echo \"exit $?\"; grep -E '^error' -A 20 build/ntsc-u/mods/widescreen-anchored.log")

        lines = queue.Queue()

        # The build runs in a thread that only fills the queue; the window reads it (tkinter may
        # only be used from the window's own thread).
        def run():
            try:
                process = subprocess.Popen(["wsl.exe", "-d", "Ubuntu-22.04", "-e", "bash", "-lc", command],
                                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                                           encoding="utf-8", errors="replace",
                                           creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
                for line in process.stdout:
                    lines.put(line.replace("\x00", ""))
                process.wait()
            except OSError as error:
                lines.put(f"could not start the build: {error}\n")
            lines.put(None)

        def poll():
            while True:
                try:
                    line = lines.get_nowait()
                except queue.Empty:
                    self.after(100, poll)
                    return
                if line is None:
                    self.done(target)
                    return
                self.log.insert("end", line)
                self.log.see("end")

        threading.Thread(target=run, daemon=True).start()
        self.after(100, poll)

    def done(self, target):
        self.build_button.configure(state="normal")
        text = self.log.get("1.0", "end")
        if "exit 0" in text:
            path = ROOT / target
            self.log.insert("end", f"\nDone: {path}\n")
            self.v["status"].set("Disc image built.")
        else:
            self.log.insert("end", "\nThe build failed. If PCSX2 has an image of this name open, close it "
                                   "or use another file name.\n")
        self.log.see("end")

    def open_folder(self):
        folder = ROOT / "build" / "ntsc-u" / "mods" / "obelisk-test"
        folder.mkdir(parents=True, exist_ok=True)
        subprocess.Popen(["explorer", str(folder)])


if __name__ == "__main__":
    CardMaker().mainloop()
