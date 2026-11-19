/* SzDuel_SetupCard_Pipe (0x1da7f0, 5,192 bytes), builds the GS packet of a card face (frame, picture,
 * attribute, stars, name, ATK/DEF digits) and returns the next packet address.
 * Yu-Gi-Oh! The Duelists of the Roses, NTSC-U SLUS_205.15. Evidence: docs/modules/duel.md.
 *
 * ORIGINAL-FILE UNIT: the 28 functions that precede SzDuel_SetupCard_Pipe in the original source
 * file (lSzDuel_InitHandUp 0x1d7d10 ... SzDuel_SetupCard 0x1da7a0) are compiled first, in this
 * one translation unit, as compiler context (docs/DECOMPILATION.md, "Original-file units").
 * SzDuel_SetupCard_Pipe compiles to the original only after all of them, in this order; tools/match.py
 * places them at their own addresses and compares SzDuel_SetupCard_Pipe. Each predecessor also
 * has its own source elsewhere in the tree. The host build sees only SzDuel_SetupCard_Pipe (the
 * block is `#ifdef __MWERKS__`).
 */
#include "dotr/ee/setup_card.h"

#ifdef __MWERKS__
/* ---- Compiler context: the original file's functions before SzDuel_SetupCard_Pipe. ---- */

void lSzDuel_InitHandUp(s32 arg0, s32 arg1) {
    u16 ids[4];
    s16 w[4];
    u8 *g = data_00317490.b;
    u8 *p;
    s32 i;
    s32 x;
    s32 cnt;
    s32 flag;
    s32 ok;
    s32 id;
    s32 n;
    s32 have;
    s32 v;
    s16 r;
    s32 idx;

    p = data_00317490.b + arg0 * 0x350;
    SzDuel_PackHand(arg0);
    lSzDuel_InitHandInfo(arg0, 0, 999, 0, 0, 0, 0);
    x = -250;
    for (i = 0; i < g[0xD49]; i++) {
        v = SzDuel_InqHand(arg0, i);
        if (v < 0) {
            v = 999;
        } else {
            v &= 0xFFF;
        }
        lSzDuel_InitHandInfo(arg0, i + 1, v, x, 0x1C0, x, -8);
        x += 0x7D;
    }
    if (arg1 != 0) {
        have = SzDuel_GetHandCount(arg0);
        n = SzDuel_GetDeckCount(arg0);
        cnt = have + n;
        cnt = cnt < 5 ? cnt : 5;
        ok = n > 0;
        flag = 0;
        if (ok) {
            ok = p[0x344] > 0;
        }
        if (ok && !(p[0x344] & 0x80)) {
            flag = 1;
        }
        if (flag) {
            cnt = cnt + 1 < 5 ? cnt + 1 : 5;
        }
        for (; i < cnt; i++) {
            id = 999;
            if (flag && i + 1 >= cnt) {
                if (Ta_GetDestinyDraw(arg0, ids, w, 0) >= 0) {
                    r = func_001a22c0(100.0f * ((f32)func_0019a5c8() / 2147483648.0f));
                    idx = 2;
                    if (r < w[0]) {
                        idx = 0;
                    } else if (r < w[0] + w[1]) {
                        idx = 1;
                    } else {
                    }
                    id = ids[idx];
                }
            }
            if (id == 999) {
                id = SzDuel_DrawDeck(arg0);
            }
            SzDuel_AddHand(arg0, id);
            lSzDuel_InitHandInfo(arg0, i + 1, id, 0x2EE, 0x1C0, 0x2EE, -8);
        }
        for (; i < 5; i++) {
            SzDuel_AddHand(arg0, 999);
            lSzDuel_InitHandInfo(arg0, i + 1, 999, 0, 0, 0, 0);
        }
    } else {
        for (; i < 5; i++) {
            lSzDuel_InitHandInfo(arg0, i + 1, 999, 0, 0, 0, 0);
        }
    }
    g[0xD4A] = SzDuel_GetHandCount(arg0);
    for (i = 0; i < g[0xD4A]; i++) {
        g[0xD2C + i] = i + 1;
    }
}

extern s32 data_00317b5c;

typedef struct {
    s8 bit0 : 1;
    s8 bit1 : 1;
    s8 bit2 : 1;
    s8 bit3 : 1;
    s8 bit4 : 1;
    s8 bit5 : 1;
    s8 bit6 : 1;
    s8 bit7 : 1;
} Flags_001D8080;

static inline u8 *lookup_001D8080(s32 index) {
    if (index < 0 || index >= 6) {
        return NULL;
    }
    return gSzDuel_aCardControl_bytes + index * 0xA0;
}

static inline void releaseModel_001D8080(u8 *model) {
    extern void SzDuel_ClearDrawCard();

    if (model != NULL) {
        SzDuel_ClearDrawCard(model);
        *(s32 *)(model + 0x90) = 0;
    }
}

static inline void freeSlot_001D8080(u8 *slot) {
    if (slot != NULL) {
        if (*(u8 **)slot != NULL) {
            releaseModel_001D8080(*(u8 **)slot);
        }
        SzDuel_ClearCardControl(slot);
    }
}

s32 lSzDuel_InitHandCancel(s32 arg0) {
    u8 *base;
    u8 *slot;
    s32 index;
    s32 step;
    s32 result;
    u8 *candidate;

    base = data_00317490.b;
    if (data_00317b5c != 0 && *(void **)(*(u8 **)(base + 0x6CC) + 4) != NULL) {
        if ((slot = gSzDuel_aCardControl_bytes) != NULL && *(u8 **)slot != NULL) {
            freeSlot_001D8080(slot);
        }
        ((Flags_001D8080 *)(*(u8 **)(base + 0x6CC) + 0x57))->bit7 = 0;
        ((Flags_001D8080 *)(*(u8 **)(base + 0x6CC) + 0x58))->bit0 = 0;
        ((Flags_001D8080 *)(*(u8 **)(base + 0x6CC) + 0x58))->bit2 = 0;
        ((Flags_001D8080 *)(*(u8 **)(*(u8 **)(base + 0x6CC) + 4) + 0x83))->bit7 = 0;
    }

    step = -0xFA;
    index = 0;
    do {
        result = SzDuel_InqHand(arg0, index);
        if (result >= 0 && (result & 0xFFF) != 999) {
            candidate = lookup_001D8080(index + 1);
            if (candidate != NULL && *(s32 *)candidate != 0) {
                ((Flags_001D8080 *)(candidate + 0x9F))->bit2 = 1;
                ((Flags_001D8080 *)(candidate + 0x9F))->bit4 = 0;
                lSzDuel_SetCardCtl(candidate, step, 0x1C0, step, -8, 0x80, 0x80);
            }
        }
        index++;
        step += 0x7D;
    } while (index < 5);

    base[0xD4A] = SzDuel_GetHandCount(arg0);
    for (index = 0; index < base[0xD4A]; index++) {
        base[0xD2C + index] = index + 1;
    }
    return 1;
}

extern u8 data_003181de;

typedef struct Flags_001D82C0 {
    s8 bit0 : 1;
    s8 bit1 : 1;
    s8 bit2 : 1;
    s8 bit3 : 1;
    s8 bit4 : 1;
    s8 bit5 : 1;
    s8 bit6 : 1;
    s8 bit7 : 1;
} Flags_001D82C0;

typedef struct Vec4_001D82C0 {
    f32 x, y, z, w;
} Vec4_001D82C0;

typedef struct Model_001D82C0 {
    Vec4_001D82C0 position;
    Vec4_001D82C0 rotation;
    u8 pad20[0x10];
    Vec4_001D82C0 scale;
    u8 color[4];
    u8 pad44[0x3D];
    Flags_001D82C0 flags81;
    u8 pad82;
    Flags_001D82C0 flags83;
    u8 pad84[0xC];
    void *owner;
    u8 pad94[0xC];
} Model_001D82C0;

typedef struct Item_001D82C0 {
    s32 unk0;
    Model_001D82C0 *model;
    u8 pad8[0x4B];
    u8 unk53;
    u8 pad54[3];
    Flags_001D82C0 flags57;
    Flags_001D82C0 flags58;
    u8 pad59[3];
} Item_001D82C0;

typedef struct Copy90_001D82C0 {
    __int128 words[9];
} Copy90_001D82C0;

typedef struct Node_001D82C0 {
    Model_001D82C0 *model;
    u8 pad4[0xC];
    Vec4_001D82C0 position;
    Vec4_001D82C0 target;
    Vec4_001D82C0 rotation;
    s32 values40[4];
    s32 values50[4];
    Vec4_001D82C0 values60;
    Vec4_001D82C0 scale;
    s32 values80[4];
    u8 pad90[8];
    s16 speed;
    s16 unk9A;
    s32 index : 26;
    s8 bit2 : 1;
    s8 bit3 : 1;
    s8 bit4 : 1;
    s8 bit5 : 1;
    s8 bit6 : 1;
    s8 bit7 : 1;
} Node_001D82C0;

typedef struct State_001D82C0 {
    u8 pad0[0x6CC];
    Item_001D82C0 *selected;
    u8 pad6D0[0x65C];
    s8 slots[0x1E];
    u8 mode;
    u8 padD4B[3];
    u8 count;
    u8 current;
} State_001D82C0;

static inline Model_001D82C0 *get_model_001D82C0(void) {
    extern void SzDuel_ClearDrawCard();
    Model_001D82C0 *model = (Model_001D82C0 *)gSzDuel_aCardRequest_bytes;
    s32 count = 0;
    do {
        if (model->owner == NULL) {
            SzDuel_ClearDrawCard(model);
            return model;
        }
        count++;
        model++;
    } while (count < 0x20);
    return NULL;
}

s32 lSzDuel_InitHandCombo(s32 arg0) {
    extern void *SzDuel_GetCardInfo();
    extern void SzDuel_ClearDrawCard();
    State_001D82C0 *state = (State_001D82C0 *)data_00317490.b;
    s32 next;
    Item_001D82C0 *copy;
    Node_001D82C0 *primary;
    Model_001D82C0 *model;
    s32 i;
    s32 slot;
    s32 x;
    s32 y;
    Node_001D82C0 *node;
    s32 baseX;
    s32 initial;

    if (data_003181de > 1) {
        initial = state->count;
    } else {
        initial = 1;
    }
    next = initial + 1;
    state->slots[0] = 6;
    if (state->selected != NULL) {
        if (state->selected->model != NULL) {
            primary = (Node_001D82C0 *)gSzDuel_aCardControl_bytes;
            copy = SzDuel_GetCardInfo(2, 0);
            if (primary != NULL && copy != NULL) {
                *copy = *state->selected;
                copy->unk53 = 0;
                primary->model = get_model_001D82C0();
                model = primary->model;
                if (model != NULL) {
                    copy->model = model;
                    copy->model->owner = copy;
                    *(Copy90_001D82C0 *)model = *(Copy90_001D82C0 *)state->selected->model;
                    model->color[3] = 0x80;
                    model->color[0] = 0x80;
                    model->color[1] = 0x80;
                    model->color[2] = 0x80;
                    model->flags83.bit7 = 1;
                    model->flags81.bit5 = 1;
                    primary->bit3 = 1;
                    primary->position = model->position;
                    Ex3d_GetPointOnScreen(&primary->target, -125.0f, -8.0f);
                    primary->bit4 = 1;
                    primary->rotation = model->rotation;
                    primary->values40[0] = 0;
                    primary->values40[1] = 0x10000;
                    primary->values40[2] = 0x10000;
                    primary->values40[3] = 0;
                    primary->bit5 = 1;
                    primary->values50[0] = 0;
                    primary->values50[1] = 0;
                    primary->values50[2] = 0;
                    primary->values50[3] = 0;
                    Ex3d_GetLookVector((s32)&primary->values60);
                    primary->bit6 = 1;
                    primary->scale = model->scale;
                    primary->values80[0] = 0x3EF00000;
                    primary->values80[1] = 0x3EFA6F4E;
                    primary->values80[2] = 0x3F800000;
                    primary->values80[3] = 0;
                    primary->speed = 0x80;
                    primary->unk9A = 0;
                    SzDuel_DoCardControl0(primary);
                    state->slots[0] = primary->index = 0;
                }
            }
            state->selected->flags58.bit0 = 1;
            state->selected->flags57.bit7 = 1;
            state->selected->flags58.bit2 = 1;
            state->selected->model->flags83.bit7 = 1;
        }
    }
    baseX = -250;
    i = 0;
    do {
        s32 index = i + 1;
        if (index < 0 || index >= 6) {
            node = NULL;
        } else {
            node = (Node_001D82C0 *)(index * 0xA0 + gSzDuel_aCardControl_bytes);
        }
        if (node != NULL && node->model != NULL) {
            s32 result = SzDuel_InqHand(arg0, i);
            if (result >= 0 && (result & 0xFFF) != 999) {
                slot = (result & 0xF000) >> 12;
                y = 448;
                x = baseX;
                if (state->count == 0 && i == state->current) {
                    slot = 1;
                }
                switch (state->mode) {
                case 1:
                    if (slot == 1) {
                        y = -8;
                        node->bit4 = 1;
                        x = 0;
                    }
                    break;
                default:
                    if (slot != 0) {
                        if (state->selected != NULL) {
                            x = (slot - 1) * 16 + 125;
                            y = -8;
                        } else if (slot == 1) {
                            x = -125;
                            y = -8;
                        } else {
                            x = (slot - 2) * 16 + 125;
                            y = -8;
                        }
                        node->bit4 = 1;
                    }
                    break;
                }
                node->bit2 = 0;
                Ex3d_GetPointOnScreen(&node->position, (f32)baseX, -8.0f);
                Ex3d_GetPointOnScreen(&node->target, (f32)x, (f32)y);
                node->rotation.x = 0;
                node->rotation.y = 0;
                node->rotation.z = 0;
                node->rotation.w = 0;
                node->values40[0] = 0;
                node->values40[1] = 0x10000;
                node->values40[2] = 0;
                node->values40[3] = 0;
                node->speed = 0x80;
                node->unk9A = 0;
                SzDuel_DoCardControl0(node);
                if (slot == 0) {
                    state->slots[next++] = node->index;
                } else {
                    state->slots[slot] = node->index;
                }
            }
        }
        i++;
        baseX += 125;
    } while (i < 5);
    return 1;
}

typedef struct Model_001D8880 {
    u8 pad0[0x40];
    u8 r;
    u8 g;
    u8 b;
    u8 pad43[0x4D];
    void *unk90;
} Model_001D8880;

typedef struct Slot_001D8880 {
    Model_001D8880 *model;
} Slot_001D8880;

static inline Slot_001D8880 *lookup_001D8880(s32 index) {
    if (index < 0 || index >= 6) {
        return NULL;
    }
    return (Slot_001D8880 *) (gSzDuel_aCardControl_bytes + index * 0xA0);
}

s32 lSzDuel_InitHandDark(s32 arg0, s32 arg1) {
    extern s32 SzDuel_GetPlayer();
    s32 i;
    Slot_001D8880 *slot;
    Model_001D8880 *model;
    s32 level;
    s32 limit;
    s32 other;
    s32 bits;
    s32 flag;

    bits = SzGame_GetMenuNum(0x1D);
    flag = (bits & (1 << arg0)) ? 1 : 0;
    other = arg0 ^ 1;
    if (flag) {
        if (SzDuel_GetPlayer(arg0) < 0x20 && SzDuel_GetPlayer(other) >= 0x64) {
            arg1 = 0;
        }
    } else if ((u16) SzPad_Check(arg0, 1, 2) == 0) {
        arg1 = 0;
    }

    for (i = 0; i < 6; i++) {
        slot = lookup_001D8880(i);
        if (slot != NULL && slot->model != NULL) {
            model = slot->model;
            level = SzDuel_GetSummonPower(arg0);
            limit = SzDuel_GetUnitLevel2(slot->model->unk90, 0);
            if (arg1 != 0 && level < limit) {
                model->r = 0x40;
                model->g = 0x40;
                model->b = 0x40;
            } else {
                model->r = 0x80;
                model->g = 0x80;
                model->b = 0x80;
            }
        }
    }
    return 1;
}

typedef struct Flags81_001D89F0 {
    s8 : 4;
    s8 bit4 : 1;
    s8 bit5 : 1;
    s8 bit6 : 1;
    s8 : 1;
} Flags81_001D89F0;

typedef struct FlagsHigh_001D89F0 {
    s8 : 7;
    s8 enabled : 1;
} FlagsHigh_001D89F0;

typedef struct Flags58_001D89F0 {
    s8 bit0 : 1;
    s8 : 1;
    s8 bit2 : 1;
    s8 : 5;
} Flags58_001D89F0;

typedef struct Model_001D89F0 {
    u8 pad00[0x10];
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
    u8 pad40[0x41];
    Flags81_001D89F0 flags81;
    u8 pad82;
    FlagsHigh_001D89F0 flags83;
    u8 pad84[0xC];
    void *owner;
    u8 pad94[0xC];
} Model_001D89F0;

typedef struct Entry_001D89F0 {
    u8 pad00[4];
    Model_001D89F0 *model;
    u8 pad08[0x34];
    u16 id;
    u8 pad3E[2];
    u8 field40;
    u8 field41;
    u8 pad42[2];
    s8 player;
    s8 slot;
    u8 pad46[2];
    s8 field48;
    u8 pad49[0xE];
    FlagsHigh_001D89F0 flags57;
    Flags58_001D89F0 flags58;
} Entry_001D89F0;

typedef union Status_001D89F0 {
    struct {
        u32 slot : 26;
        u32 : 6;
    } index;
    struct {
        u8 pad[3];
        s8 : 2;
        s8 bit2 : 1;
        s8 bit3 : 1;
        s8 : 4;
    } flags;
} Status_001D89F0;

typedef struct Slot_001D89F0 {
    Model_001D89F0 *model;
    u8 pad04[0x98];
    Status_001D89F0 status;
} Slot_001D89F0;

static inline Slot_001D89F0 *lookupSlot_001D89F0(s32 index) {
    if (index < 0 || index >= 6) {
        return NULL;
    }
    return (Slot_001D89F0 *)(gSzDuel_aCardControl_bytes + index * 0xA0);
}

static inline Model_001D89F0 *allocateModel_001D89F0(void) {
    extern void SzDuel_ClearDrawCard();
    Model_001D89F0 *candidate;
    s32 i;

    candidate = (Model_001D89F0 *)gSzDuel_aCardRequest_bytes;
    i = 0;
    do {
        if (candidate->owner == NULL) {
            SzDuel_ClearDrawCard(candidate);
            return candidate;
        }
        i++;
        candidate++;
    } while (i < 32);
    return NULL;
}

s32 lSzDuel_InitHandInfo(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    extern void *SzDuel_GetCardInfo();
    u8 *player;
    Entry_001D89F0 *entry;
    Slot_001D89F0 *slot;
    Model_001D89F0 *model;

    player = data_00317490.b + arg0 * 0x350;
    entry = SzDuel_GetCardInfo(2, arg1);
    if (entry == NULL) {
        return 0;
    }
    SzDuel_InitCardInfo(entry, arg2);
    entry->player = arg0;
    entry->slot = arg1;
    entry->field40 = player[0x33D];
    entry->field41 = player[0x33E];
    if (entry->id != 999) {
        slot = lookupSlot_001D89F0(arg1);
        entry->field48 = Ta_GetSummonRev(arg0, entry->id);
        if (slot != NULL) {
            if (slot->model == NULL) {
                slot->model = allocateModel_001D89F0();
            }
            model = slot->model;
            if (model != NULL) {
                entry->model = model;
                entry->model->owner = entry;
                model->scale[0] = 0.46875f;
                model->scale[1] = 0.489130437f;
                model->scale[2] = 1.0f;
                model->scale[3] = 0.0f;
                model->position[0] = 0.0f;
                model->position[1] = 0.0f;
                model->position[2] = 0.0f;
                model->position[3] = 0.0f;
                model->flags83.enabled = 1;
                model->flags81.bit4 = 0;
                model->flags81.bit6 = 0;
                model->flags81.bit5 = 1;
                Ex3d_GetLookVector((s32)model->rotation);
                slot->status.flags.bit2 = 1;
                slot->status.flags.bit3 = 1;
                lSzDuel_SetCardCtl(slot, arg3, arg4, arg5, arg6, 0, 0x80);
                slot->status.index.slot = arg1;
            }
        }
    }
    entry->flags57.enabled = 1;
    entry->flags58.bit0 = 1;
    entry->flags58.bit2 = 1;
    return 1;
}

typedef struct Flags_001D8CD0 {
    s8 bit0 : 1;
    s8 bit1 : 1;
    s8 bit2 : 1;
    s8 bit3 : 1;
    s8 bit4 : 1;
    s8 bit5 : 1;
    s8 bit6 : 1;
    s8 bit7 : 1;
} Flags_001D8CD0;

typedef struct Index_001D8CD0 {
    u32 index : 26;
    u32 flags : 6;
} Index_001D8CD0;

static inline u8 *allocate_001D8CD0(void) {
    extern void SzDuel_ClearDrawCard();
    u8 *p = gSzDuel_aCardRequest_bytes;
    s32 i = 0;

    do {
        if (*(s32 *)(p + 0x90) == 0) {
            SzDuel_ClearDrawCard(p);
            return p;
        }
        i++;
        p += 0xA0;
    } while (i < 32);
    return NULL;
}

s32 lSzDuel_InitHandInfo2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    extern void *SzDuel_GetCardInfo();
    u8 *player;
    u8 *object;
    u8 *entry;
    u8 *resource;

    player = data_00317490.b + arg0 * 0x350;
    object = SzDuel_GetCardInfo(2, arg1);
    if (object == NULL) {
        return 0;
    }
    SzDuel_InitCardInfo(object, arg2);
    *(s8 *)(object + 0x44) = arg0;
    *(s8 *)(object + 0x45) = arg1;
    *(u8 *)(object + 0x40) = *(u8 *)(player + 0x33D);
    *(u8 *)(object + 0x41) = *(u8 *)(player + 0x33E);
    if (*(u16 *)(object + 0x3C) != 999) {
        if (arg1 < 0 || arg1 >= 6) {
            entry = NULL;
        } else {
            entry = gSzDuel_aCardControl_bytes + arg1 * 0xA0;
        }
        if (entry != NULL) {
            if (*(u8 **)entry == NULL) {
                *(u8 **)entry = allocate_001D8CD0();
            }
            resource = *(u8 **)entry;
            if (resource != NULL) {
                *(u8 **)(object + 4) = resource;
                *(u8 **)(*(u8 **)(object + 4) + 0x90) = object;
                *(s32 *)(resource + 0x30) = 0x3EF00000;
                *(s32 *)(resource + 0x34) = 0x3EFA6F4E;
                *(s32 *)(resource + 0x38) = 0x3F800000;
                *(s32 *)(resource + 0x3C) = 0;
                *(s32 *)(resource + 0x10) = 0;
                *(s32 *)(resource + 0x14) = 0;
                *(s32 *)(resource + 0x18) = 0;
                *(s32 *)(resource + 0x1C) = 0;
                ((Flags_001D8CD0 *)(resource + 0x83))->bit6 = 1;
                ((Flags_001D8CD0 *)(resource + 0x83))->bit7 = 1;
                ((Flags_001D8CD0 *)(resource + 0x81))->bit4 = 1;
                ((Flags_001D8CD0 *)(resource + 0x81))->bit6 = 1;
                ((Flags_001D8CD0 *)(resource + 0x81))->bit5 = 1;
                Ex3d_GetLookVector((s32)(resource + 0x20));
                ((Flags_001D8CD0 *)(entry + 0x9F))->bit2 = 0;
                ((Flags_001D8CD0 *)(entry + 0x9F))->bit3 = 1;
                lSzDuel_SetCardCtl2(entry, arg3, arg4, arg3, arg4, 128, 128);
                ((Index_001D8CD0 *)(entry + 0x9C))->index = arg1;
            }
        }
        *(s8 *)(object + 0x48) = Ta_GetSummonRev(arg0, *(u16 *)(object + 0x3C));
    }
    ((Flags_001D8CD0 *)(object + 0x57))->bit7 = 1;
    ((Flags_001D8CD0 *)(object + 0x58))->bit0 = 1;
    ((Flags_001D8CD0 *)(object + 0x58))->bit2 = 1;
    return 1;
}

s32 lSzDuel_SetCardCtl(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (arg0 == NULL) {
        return 0;
    }
    *(f32 *)((u8 *)arg0 + 0x10) = (f32) arg1;
    *(f32 *)((u8 *)arg0 + 0x14) = (f32) arg2;
    *(s32 *)((u8 *)arg0 + 0x18) = 0;
    *(s32 *)((u8 *)arg0 + 0x1C) = 0;
    *(f32 *)((u8 *)arg0 + 0x20) = (f32) arg3;
    *(f32 *)((u8 *)arg0 + 0x24) = (f32) arg4;
    *(s32 *)((u8 *)arg0 + 0x28) = 0;
    *(s32 *)((u8 *)arg0 + 0x2C) = 0;
    *(s16 *)((u8 *)arg0 + 0x98) = (s16) arg6;
    *(s16 *)((u8 *)arg0 + 0x9A) = (s16) arg5;
    SzDuel_DoCardControl0(arg0);
    return 1;
}

s32 lSzDuel_SetCardCtl2(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (arg0 == NULL) {
        return 0;
    }
    Ex3d_GetPointOnScreen((u8 *) arg0 + 0x10, (f32) arg1, (f32) arg2);
    Ex3d_GetPointOnScreen((u8 *) arg0 + 0x20, (f32) arg3, (f32) arg4);
    *(s16 *)((u8 *) arg0 + 0x98) = (s16) arg6;
    *(s16 *)((u8 *) arg0 + 0x9A) = (s16) arg5;
    SzDuel_DoCardControl0(arg0);
    return 1;
}

void lSzDuel_LockCamera(s32 arg0, s32 arg1, s32 arg2) {
    f32 sp50[4];
    f32 sp60[4];
    u8 *temp_s3;

    temp_s3 = data_00317490.b + (turnOwner * 0x350);
    SzView_IV2V(temp_s3 + 0x1D0, sp50);
    sp60[0] = (f32) arg1;
    sp60[2] = (f32) arg2;
    sp60[1] = (f32) *(s8 *)(temp_s3 + 0x345);
    SzView_ReqCamera(0, 0, 3, (u16)arg0, 0, sp60);
    SzView_ReqCamera(0, 0, 4, arg0 & 0xFFFF, 0, sp50);
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4_001D91B0;

void lSzDuel_LockCamera2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Vec4_001D91B0 saved;
    f32 pos[4];
    f32 camera[4];
    u8 *p;
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
    s32 angle;
    u8 *cur;

    p = data_00317490.b + turnOwner * 0x350;
    saved = *(Vec4_001D91B0 *)(p + 0x1D0);
    x0 = (s32)(f32)((arg1 - 3) * 50);
    z0 = (s32)(f32)((arg2 - 3) * 50);
    x1 = (s32)(f32)((arg3 - 3) * 50);
    z1 = (s32)(f32)((arg4 - 3) * 50);
    angle = (s32)(65536.0f * (func_001a6c18((f32)(z1 - z0), (f32)(x1 - x0)) / 6.2831855f));
    *(s32 *)(p + 0x1D0) = 0x96;
    *(s32 *)(p + 0x1D4) = (((angle + 0x80) >> 8) << 8) + 0x4000;
    *(s32 *)(p + 0x1D8) = 0x4000;
    cur = data_00317490.b + turnOwner * 0x350;
    SzView_IV2V(cur + 0x1D0, camera);
    pos[0] = (f32)((x0 + x1) >> 1);
    pos[2] = (f32)((z0 + z1) >> 1);
    pos[1] = (f32)*(s8 *)(cur + 0x345);
    SzView_ReqCamera(0, 0, 3, (u16)arg0, 0, pos);
    SzView_ReqCamera(0, 0, 4, arg0 & 0xFFFF, 0, camera);
    *(Vec4_001D91B0 *)(p + 0x1D0) = saved;
}

typedef struct {
    u8 pad0[0x40];
    u8 x;
    u8 y;
    u8 pad42[2];
    u8 owner;
    u8 slot;
    u8 pad46[0x11];
    s8 pad57 : 4;
    s8 flag : 1;
    s8 pad57b : 3;
} Card_001D9420;

s32 lSzDuel_IsUnitHide(void *arg0) {
    u8 *duel = data_00317490.b;
    s32 result;
    void **cell;
    s32 me;
    s32 other;
    s32 flag;
    Card_001D9420 *card = arg0;
    s32 target;
    s32 bits;
    extern s32 SzDuel_GetPlayer();

    result = 0;
    if (card != NULL) {
        cell = SzDuel_GetMapInfo(card->x, card->y);
        bits = SzGame_GetMenuNum(0x1D);
        flag = (bits & (1 << card->owner)) ? 1 : 0;
        me = duel[0xD3C];
        other = duel[0xD3C] ^ 1;
        if (card->owner >= 2) {
            return 0;
        }
        if (cell == NULL || *cell != card) {
            if (flag) {
                if (SzDuel_GetPlayer(me) < 0x20 && SzDuel_GetPlayer(other) >= 0x64) {
                    result = 1;
                }
            } else if ((u16) SzPad_Check(duel[0xD3C], 1, 2) == 0) {
                result = 1;
            }
        } else {
            result = Ta_IsUnitHide((s32) card, duel[0xD3C]);
            if (result == 0 && card->flag == 0) {
                target = Ta_IsUnitControl(card->owner, card->slot);
                if (flag) {
                    if (SzDuel_GetPlayer(me) < 0x20 && SzDuel_GetPlayer(other) >= 0x64 && SzDuel_GetPlayer(target) < 0x20) {
                        result = 1;
                    }
                } else if (duel[0xD3C] == target) {
                    if ((u16) SzPad_Check(duel[0xD3C], 1, 2) == 0) {
                        result = 1;
                    }
                } else {
                    result = 1;
                }
            }
        }
    }
    return result;
}

void SzDuel_ActCardTest(void) {
    u8 *p = gSzDuel_dt_bytes;
    if (SzPad_Check(0, 2, 0x1000) != 0) {
        *(s32 *)(p + 0x5C) = (*(s32 *)(p + 0x5C) + 0x10400) % 0x10000;
    }
    if (SzPad_Check(0, 2, 0x4000) != 0) {
        *(s32 *)(p + 0x5C) = (*(s32 *)(p + 0x5C) + 0xFC00) % 0x10000;
    }
    if (SzPad_Check(0, 2, 0x2000) != 0) {
        *(s32 *)(p + 0x60) = (*(s32 *)(p + 0x60) + 0x10400) % 0x10000;
    }
    if (SzPad_Check(0, 2, 0x8000) != 0) {
        *(s32 *)(p + 0x60) = (*(s32 *)(p + 0x60) + 0xFC00) % 0x10000;
    }
    if (SzPad_Check(0, 2, 8) != 0) {
        *(s32 *)(p + 0x64) = (*(s32 *)(p + 0x64) + 0x10400) % 0x10000;
    }
    if (SzPad_Check(0, 2, 4) != 0) {
        *(s32 *)(p + 0x64) = (*(s32 *)(p + 0x64) + 0xFC00) % 0x10000;
    }
    if (SzPad_Check(0, 2, 2) != 0) {
        *(s32 *)(p + 0x70) = (*(s32 *)(p + 0x70) + 0x357) % 854;
    }
    if (SzPad_Check(0, 2, 1) != 0) {
        *(s32 *)(p + 0x70) = (*(s32 *)(p + 0x70) + 0x355) % 854;
    }
    if (SzPad_Check(0, 0, 0x10) != 0) {
        SzDebug_OutputFrameBuffer();
    }
}

typedef struct {
    u8 pad0[0x1D0];
    u8 f1D0[0x16D];
    u8 x;
    u8 z;
    u8 pad33F[6];
    s8 y;
    s8 active : 1;
    u8 pad347[9];
} Player_001D9820;

typedef struct {
    Player_001D9820 players[4];
    u8 padD40[0x26];
    s8 : 7;
    s8 flagD66 : 1;
    s8 flagD67 : 1;
} State_001D9820;

void SzDuel_InitTurn(s32 arg0, s32 arg1) {
    extern s32 func_001a22c0(f32);
    State_001D9820 *state = (State_001D9820 *)data_00317490.b;
    Player_001D9820 *p;
    s32 x;
    s32 z;
    Player_001D9820 *cur;
    f32 pos[4];
    f32 saved[4];

    turnOwner = arg1;
    p = (Player_001D9820 *)(data_00317490.b + arg1 * 0x350);
    z = func_001a22c0((f32)((p->z - 3) * 50));
    x = func_001a22c0((f32)((p->x - 3) * 50));
    cur = (Player_001D9820 *)(data_00317490.b + turnOwner * 0x350);
    SzView_IV2V(cur->f1D0, saved);
    pos[0] = (f32)x;
    pos[2] = (f32)z;
    pos[1] = (f32)cur->y;
    SzView_ReqCamera(0, 0, 3, 0x20, 0, pos);
    SzView_ReqCamera(0, 0, 4, 0x20, 0, saved);
    p->active = 1;
    if (TaTuto_IsTuto(arg1) != 0) {
        p = state->players;
    }
    TaLogic_AdjustLogicData(p);
    state->flagD66 = 1;
    state->flagD67 = state->flagD66;
}

void SzDuel_StartCardSession(void) {
    RhBase_SetVifPacketArea(SzDuel_StartCardSession_Pipe(RhBase_GetVifPacketArea()));
}

s32 SzDuel_StartCardSession_Pipe(s32 arg0) {
    return SzDuel_StartCardSessionWithPalette_Pipe(arg0, 2);
}

typedef struct {
    s8 low : 5;
    s8 bit5 : 1;
    s8 high : 2;
} Flags_001D9A30;


s32 SzDuel_StartCardSessionWithPalette_Pipe(s32 arg0, s32 arg1) {
    extern s32 SzText_InitClutManage();
    extern s32 SzText_LoadRequest();
    extern s32 *SzMsg_GetInfo(s32);
    extern s32 RhBase_EnableRenderFrame();
    extern s32 RhBase_ClearRenderFrame();
    extern s32 RhBase_DisableRenderFrame();
    u8 *base = data_00317490.b;
    s32 sprite;
    s32 length;
    s32 i;
    s32 result;

    RhBase_AssignRenderFrame(3, 2, 0x32);
    arg0 = RhBase_EnableRenderFrame(arg0);
    arg0 = RhBase_ClearRenderFrame(arg0, 0);
    sprite = RhBase_DisableRenderFrame(arg0);
    arg0 = RhBase_GetUserGsAddress();
    arg0 += SzText_InitClutManage(arg0, 2);
    sprite = SzMsg_Init_Pipe(sprite, arg1, arg0, &length);
    i = 0;
    arg0 += length;
    do {
        *(s32 *)(base + i * 4 + 0x6A0) = 0;
        i++;
    } while (i < 9);
    *(s32 *)(base + 0x6B0) = SzText_LoadRequest(3, 4);
    *(s32 *)(base + 0x6B4) = SzText_LoadRequest(3, 5);
    *(s32 *)(base + 0x6B8) = SzText_LoadRequest(3, 6);
    sprite = SzText_DoRequest_Pipe(sprite, 3, arg0, &length);
    arg0 += length;
    SzText_FreeRequest(3, -1);
    *(s32 *)(base + 0xC80) = arg0;
    arg0 += SzText_InitClutManage(arg0, 1);
    *(s32 *)(base + 0x6A0) = SzText_LoadRequest(3, 0);
    sprite = SzText_DoRequest_Pipe(sprite, 3, arg0, &length);
    arg0 += length;
    SzText_FreeRequest(3, 0);
    {
        s32 *info;

        sprite = SzMsg_LoadFont_Pipe(sprite, 0, 0x15, 0, 0);
        info = SzMsg_GetInfo(0);
        info[3] = 0x16;
        sprite = SzMsg_LoadFont_Pipe(sprite, 1, 0xA1, 0, 0x16);
        result = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(sprite, 2, 0xA2, 0, 0x2C));
    }
    *(s32 *)(base + 0xC7C) = arg0;
    *(s32 *)(base + 0xC54) = 0;
    ((Flags_001D9A30 *)(base + 0xD64))->bit5 = 0;
    return result;
}

typedef struct {
    u8 pad000[0x6A0];
    s32 slots[8];
    s32 count;
} State_001D9C40;

s32 lSzDuel_SetupCardTexture(s32 arg0, s32 arg1) {
    extern s32 SzText_InitClutManage();
    extern s32 SzText_LoadRequest();
    extern s32 data_00317b30[];
    extern s32 data_00318110;
    State_001D9C40 *state;
    s32 value;
    s32 i;

    state = (State_001D9C40 *)data_00317490.b;
    value = data_00318110;
    if (data_00317b30[arg1] != 0) {
        return arg0;
    }
    if (arg1 >= 0 && arg1 < 4) {
        i = 0;
        while (1) {
            state->slots[i] = 0;
            i++;
            if (i > 3) {
                break;
            }
        }
        state->count = 0;
        value += SzText_InitClutManage(value, 1);
        state->slots[arg1] = SzText_LoadRequest(3, arg1);
        arg0 = SzText_DoRequest_Pipe(arg0, 3, value, NULL);
        SzText_FreeRequest(3, arg1);
        arg0 = RhText_SetTextureFlush(arg0);
    } else if (arg1 == 8) {
        i = 0;
        while (1) {
            state->slots[i] = 0;
            i++;
            if (i > 3) {
                break;
            }
        }
        value += SzText_InitClutManage(value, 1);
        state->slots[arg1] = SzText_LoadRequest(9, 0);
        arg0 = SzText_DoRequest_Pipe(arg0, 9, value, NULL);
        SzText_FreeRequest(9, 0);
        arg0 = RhText_SetTextureFlush(arg0);
    }
    return arg0;
}

void SzDuel_FinishCardSession(void) {
    RhBase_SetVifPacketArea(SzDuel_FinishCardSession_Pipe(RhBase_GetVifPacketArea()));
}

s32 SzDuel_FinishCardSession_Pipe(s32 arg0) {
    extern s32 ExClampMode_Pipe();
    s32 temp_s0;
    s32 temp_s0_2;

    RhBase_FreeRenderFrame();
    temp_s0_2 = RhBase_SetZBufferMode(arg0, 0);
    temp_s0 = ExClampMode_Pipe(temp_s0_2, ExGetRegVal(0, 4));
    SzMsg_Free(0);
    SzMsg_Free(1);
    SzMsg_Free(2);
    return temp_s0;
}

s32 SzDuel_FinishCardSessionX_Pipe(s32 arg0) {
    extern s32 ExClampMode_Pipe();
    s32 temp_s0;

    RhBase_FreeRenderFrame();
    temp_s0 = ExClampMode_Pipe(arg0, ExGetRegVal(0, 4));
    SzMsg_Free(0);
    SzMsg_Free(1);
    SzMsg_Free(2);
    return temp_s0;
}

void SzDuel_SetCardDefPrioZ(s32 arg0) {
    data_003180e4 = arg0;
}

void SzDuel_SetWindowDefPrioZ(s32 arg0) {
    data_003180e8 = arg0;
}

s32 SzDuel_GetUserGsAddress(void) {
    return data_0031810c;
}

typedef union {
    __int128 q;
    s32 i[4];
    f32 f[4];
} Vec_001D9F00;

typedef struct {
    s16 value : 12;
    s16 flags : 4;
} Count_001D9F00;

typedef struct {
    s8 unused : 4;
    s8 mode : 1;
    s8 split : 1;
    s8 rotation : 1;
    s8 flag7 : 1;
} Flags_001D9F00;

typedef struct {
    u64 lower : 32;
    u64 color : 24;
    u64 upper : 8;
} Packet_001D9F00;

typedef struct {
    u8 pad00[0x10];
    s32 position[4];
    u8 rotation[0x10];
    u8 orientation[0x10];
    u8 pad40[8];
    s16 field48;
    s16 field4A;
    s32 field4C;
    s64 field50;
    u8 pad58[0x20];
    u8 *vertices;
    s32 field7C;
    union {
        Count_001D9F00 count;
        struct {
            u8 low;
            Flags_001D9F00 flags;
        } bytes;
    } control80;
    Count_001D9F00 count82;
} Object_001D9F00;

extern Vec_001D9F00 data_00291540;
extern Vec_001D9F00 data_00291550;
extern Vec_001D9F00 data_00291560;
extern Vec_001D9F00 data_00291570;
extern Vec_001D9F00 data_00291580;


s32 SzDuel_GetCardNode(void *arg0, s32 arg1) {
    Vec_001D9F00 point;
    Vec_001D9F00 transformed;
    __int128 matrix[4];
    __int128 projection[4];
    u8 *state = data_00317490.b;
    s32 mode;
    Object_001D9F00 *object = arg0;
    s32 i;
    s32 offset;
    extern s64 SzDuel_GetCardNodeUnit();
    extern s64 Ex3d_NodeL2V(void *, s32, void *, s32, s32, s32, s32);

    point = data_00291540;
    mode = arg1 ? 15 : -1;
    RhGeom_PushMatrix(0);
    if (object->control80.bytes.flags.split) {
        __int128 rotationMatrix[4];
        __int128 translationMatrix[4];
        Vec_001D9F00 position;
        Vec_001D9F00 height;
        void *orientation;

        position = data_00291550;
        position.i[0] = object->position[0];
        position.i[2] = object->position[2];
        height = data_00291560;
        height.i[1] = object->position[1];
        orientation = object->control80.bytes.flags.rotation ? object->orientation : NULL;
        RhGeom_GetScaleRotXYZTransMatrix(rotationMatrix, NULL, object->rotation, object);
        RhGeom_GetRotXYZMatrix(translationMatrix, &height);
        RhGeom_MulMatrixRev((s32)translationMatrix, (s32)rotationMatrix);
        RhGeom_GetScaleRotXYZTransMatrix(matrix, orientation, &position, NULL);
        RhGeom_MulMatrixRev((s32)matrix, (s32)translationMatrix);
    } else {
        __int128 rotationMatrix[4];
        Vec_001D9F00 position;
        Vec_001D9F00 height;
        void *orientation;

        position = data_00291570;
        position.i[0] = object->position[0];
        position.i[2] = object->position[2];
        height = data_00291580;
        height.i[1] = object->position[1];
        orientation = object->control80.bytes.flags.rotation ? object->orientation : NULL;
        RhGeom_GetScaleRotXYZTransMatrix(rotationMatrix, NULL, &height, object);
        RhGeom_GetScaleRotXYZTransMatrix(matrix, orientation, &position, NULL);
        RhGeom_MulMatrixRev((s32)matrix, (s32)rotationMatrix);
    }
    RhView_GetWorldViewMatrix(projection);
    RhGeom_MulMatrix((s32)projection, matrix);
    RhGeom_MulMatrixVector(&point, projection, &point);
    object->field4A = 0x35;
    object->field48 = 0;
    object->field50 = 0;
    object->field4C = 0;
    ExAssignAlpha(&object->field48, 0x8000000044LL);
    if (object->control80.bytes.flags.mode) {
        ExAssignClamp(&object->field48, 0x5C00040000ALL);
    } else {
        ExAssignClamp(&object->field48, 0x2D0001E000ALL);
    }
    object->count82.value = (s16)SzDuel_GetCardNodeUnit(object->control80.bytes.flags.mode, object->vertices);
    object->count82.value = (s16)Ex3d_NodeL2V(projection, object->count82.value, object->vertices, object->control80.count.value, object->field7C, mode, 0);
    Ex3d_GetViewVectorForNode(NULL, object->count82.value, object->vertices, transformed.f);
    Ex3d_NodeV2S(NULL, object->count82.value, object->vertices);
    if (*(s32 *)(state + 0xC54) != 0) {
        for (i = 0, offset = 0; i < object->count82.value; i++, offset += 0x30) {
            ((Packet_001D9F00 *)(object->vertices + offset + 0x10))->color = *(u32 *)(state + 0xC54);
        }
    }
    RhGeom_PopMatrix();
    return RhGeom_InnerProduct(transformed.f, point.f) <= 0.0f ? 0 : 1;
}

typedef struct {
    s8 reserved0 : 4;
    s8 flag4 : 1;
    s8 reserved5 : 1;
    s8 flag6 : 1;
    s8 reserved7 : 1;
} FlagByte_001DA2B0;

typedef struct {
    s16 count : 12;
    s16 reserved : 4;
} Count_001DA2B0;

typedef union {
    s16 value;
    Count_001DA2B0 bits;
    struct {
        u8 low;
        FlagByte_001DA2B0 flags;
    } bytes;
} Flags_001DA2B0;

typedef struct {
    u8 pad0[0x48];
    s16 field48;
    s16 field4A;
    s32 field4C;
    s64 field50;
    u8 pad58[0x20];
    void *vertices;
    s32 field7C;
    Flags_001DA2B0 field80;
    Count_001DA2B0 field82;
} Object_001DA2B0;

typedef struct {
    u8 pad0[0x10];
    u64 lower : 32;
    u64 color : 24;
    u64 upper : 8;
    u8 pad18[0x18];
} Vertex_001DA2B0;

s32 SzDuel_GetCardNodeOnScreen(void *arg0, s32 arg1, f32 fparg0, f32 fparg1) {
    Object_001DA2B0 *object;
    void *temp_s1;
    u8 *global;

    object = arg0;
    global = data_00317490.b;
    temp_s1 = (object->field80.bytes.flags.flag6 == 0) ? NULL : ((u8 *) object + 0x30);
    object->field4A = 0x35;
    object->field48 = 0;
    object->field50 = 0;
    object->field4C = 0;
    ExAssignAlpha(&object->field48, ((s64) 0x80 << 32) | 0x44);
    if (object->field80.bytes.flags.flag4 != 0) {
        ExAssignClamp(&object->field48, ((s64) 0x5C0 << 32) | 0x40000A);
    } else {
        ExAssignClamp(&object->field48, ((s64) 0x2D0 << 32) | 0x1E000A);
    }
    object->field82.count = (s16) SzDuel_GetCardNodeUnitOnScreen(
            object->field80.bytes.flags.flag4,
            fparg0,
            fparg1,
            object->vertices,
            temp_s1);
    if (arg1 != 0) {
        extern s32 Ex3d_ViewClip();
        object->field82.count = (s16) Ex3d_ViewClip(
                object->field82.count,
                object->vertices,
                object->field80.bits.count,
                object->field7C,
                0xF);
    }
    {
        extern void Ex3d_NodeV2S();
        Ex3d_NodeV2S(NULL,
            object->field82.count,
            object->vertices);
    }
    if (*(s32 *) (global + 0xC54) != 0) {
        s32 i;
        for (i = 0; i < object->field82.count; i++) {
            ((Vertex_001DA2B0 *) object->vertices)[i].color = *(s32 *) (global + 0xC54);
        }
    }
    return 1;
}

s32 SzDuel_GetCardNodeUnit(s32 mode, void *dst) {
    f32 negativeHalfWidth;
    f32 negativeHalfHeight;
    f32 halfHeight;
    f32 uMax;
    f32 vMax;
    f32 halfWidth;
    f32 width;
    f32 height;

    width = 120.0f;
    height = 180.0f;
    if (mode != 0) {
        width = 256.0f;
        height = 368.0f;
    }
    halfWidth = width / 2.0f;
    halfHeight = height / 2.0f;
    vMax = height / 512.0f;
    uMax = width / 512.0f;
    *(u8 *)((u8 *)dst + 0x18) = 0x80;
    *(u8 *)((u8 *)dst + 0x19) = 0x80;
    *(u8 *)((u8 *)dst + 0x1A) = 0x80;
    *(u8 *)((u8 *)dst + 0x1B) = 0x80;
    *(s32 *)((u8 *)dst + 0x1C) = 0x3F800000;
    negativeHalfWidth = -halfWidth;
    *(f32 *)((u8 *)dst + 0x0) = negativeHalfWidth;
    *(f32 *)((u8 *)dst + 0x4) = halfHeight;
    *(s32 *)((u8 *)dst + 0x8) = 0;
    *(s32 *)((u8 *)dst + 0xC) = 0x3F800000;
    *(s32 *)((u8 *)dst + 0x20) = 0;
    *(s32 *)((u8 *)dst + 0x24) = 0;

    *(u8 *)((u8 *)dst + 0x48) = 0x80;
    *(u8 *)((u8 *)dst + 0x49) = 0x80;
    *(u8 *)((u8 *)dst + 0x4A) = 0x80;
    *(u8 *)((u8 *)dst + 0x4B) = 0x80;
    *(s32 *)((u8 *)dst + 0x4C) = 0x3F800000;
    *(f32 *)((u8 *)dst + 0x30) = negativeHalfWidth;
    negativeHalfHeight = -halfHeight;
    *(f32 *)((u8 *)dst + 0x34) = negativeHalfHeight;
    *(s32 *)((u8 *)dst + 0x38) = 0;
    *(s32 *)((u8 *)dst + 0x3C) = 0x3F800000;
    *(s32 *)((u8 *)dst + 0x50) = 0;
    *(f32 *)((u8 *)dst + 0x54) = vMax;

    *(u8 *)((u8 *)dst + 0x78) = 0x80;
    *(u8 *)((u8 *)dst + 0x79) = 0x80;
    *(u8 *)((u8 *)dst + 0x7A) = 0x80;
    *(u8 *)((u8 *)dst + 0x7B) = 0x80;
    *(s32 *)((u8 *)dst + 0x7C) = 0x3F800000;
    *(f32 *)((u8 *)dst + 0x60) = halfWidth;
    *(f32 *)((u8 *)dst + 0x64) = negativeHalfHeight;
    *(s32 *)((u8 *)dst + 0x68) = 0;
    *(s32 *)((u8 *)dst + 0x6C) = 0x3F800000;
    *(f32 *)((u8 *)dst + 0x80) = uMax;
    *(f32 *)((u8 *)dst + 0x84) = vMax;

    *(u8 *)((u8 *)dst + 0xA8) = 0x80;
    *(u8 *)((u8 *)dst + 0xA9) = 0x80;
    *(u8 *)((u8 *)dst + 0xAA) = 0x80;
    *(u8 *)((u8 *)dst + 0xAB) = 0x80;
    *(s32 *)((u8 *)dst + 0xAC) = 0x3F800000;
    *(f32 *)((u8 *)dst + 0x90) = halfWidth;
    *(f32 *)((u8 *)dst + 0x94) = halfHeight;
    *(s32 *)((u8 *)dst + 0x98) = 0;
    *(s32 *)((u8 *)dst + 0x9C) = 0x3F800000;
    *(f32 *)((u8 *)dst + 0xB0) = uMax;
    *(s32 *)((u8 *)dst + 0xB4) = 0;
    return 4;
}

typedef struct Vertex_001DA5E0 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    u8 pad10[8];
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 q;
    f32 u;
    f32 v;
    u8 pad28[8];
} Vertex_001DA5E0;

s32 SzDuel_GetCardNodeUnitOnScreen(s32 arg0, f32 x, f32 y, void *arg1, void *arg2) {
    f32 *scale = arg2;
    f32 width;
    f32 height;
    f32 halfWidth;
    f32 halfHeight;
    f32 z;
    Vertex_001DA5E0 *vertex;

    width = arg0 ? 256 : 120;
    height = arg0 ? 368 : 180;
    vertex = arg1;
    halfWidth = (scale ? width * scale[0] : width) / 2.0f;
    halfHeight = (scale ? height * scale[1] : height) / 2.0f;
    z = SzView_GetScreenDistance(0);

    vertex[0].r = 0x80;
    vertex[0].g = 0x80;
    vertex[0].b = 0x80;
    vertex[0].a = 0x80;
    vertex[0].q = 1.0f;
    vertex[0].x = x - halfWidth;
    vertex[0].y = y - halfHeight;
    vertex[0].z = z;
    vertex[0].w = 1.0f;
    vertex[0].u = 0.0f;
    vertex[0].v = 0.0f;

    vertex[1].r = 0x80;
    vertex[1].g = 0x80;
    vertex[1].b = 0x80;
    vertex[1].a = 0x80;
    vertex[1].q = 1.0f;
    vertex[1].x = x - halfWidth;
    vertex[1].y = y + halfHeight;
    vertex[1].z = z;
    vertex[1].w = 1.0f;
    vertex[1].u = 0.0f;
    vertex[1].v = height / 512.0f;

    vertex[2].r = 0x80;
    vertex[2].g = 0x80;
    vertex[2].b = 0x80;
    vertex[2].a = 0x80;
    vertex[2].q = 1.0f;
    vertex[2].x = x + halfWidth;
    vertex[2].y = y + halfHeight;
    vertex[2].z = z;
    vertex[2].w = 1.0f;
    vertex[2].u = width / 512.0f;
    vertex[2].v = height / 512.0f;

    vertex[3].r = 0x80;
    vertex[3].g = 0x80;
    vertex[3].b = 0x80;
    vertex[3].a = 0x80;
    vertex[3].q = 1.0f;
    vertex[3].x = x + halfWidth;
    vertex[3].y = y - halfHeight;
    vertex[3].z = z;
    vertex[3].w = 1.0f;
    vertex[3].u = width / 512.0f;
    vertex[3].v = 0.0f;
    return 4;
}

void SzDuel_SetupCard(s32 arg0, s32 arg1) {
    RhBase_SetVifPacketArea(SzDuel_SetupCard_Pipe(RhBase_GetVifPacketArea(), arg0, (Draw_001DA7F0 *)arg1));
}

#endif /* __MWERKS__: compiler context */

typedef struct {
    u16 count;
    u16 prim;
    u32 flags;
    u64 regs;
} Tag_001DA7F0;

typedef struct {
    u64 x : 16;
    u64 y : 16;
    u64 z : 24;
    u64 f : 8;
} XYZ_001DA7F0;

typedef struct {
    u64 tag[2];
    u8 r, g, b, a;
    f32 q;
    u64 uv0;
    XYZ_001DA7F0 xyz0;
    u64 uv1;
    XYZ_001DA7F0 xyz1;
    u64 pad[4];
} Sprite_001DA7F0;

typedef struct {
    u64 tag[2];
    u8 r, g, b, a;
    f32 q;
    XYZ_001DA7F0 xyz0;
    XYZ_001DA7F0 xyz1;
    u64 end;
} Solid_001DA7F0;

typedef struct {
    u16 u : 14;
    u16 pad0 : 2;
    u16 v : 14;
    u16 pad1 : 2;
} UV_001DA7F0;

typedef struct {
    u8 tex;
    u8 pad;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
} Entry_001DA7F0;

typedef struct {
    u16 x;
    u16 y;
} Pos_001DA7F0;

typedef struct {
    Pos_001DA7F0 name;
    Pos_001DA7F0 attr;
    Pos_001DA7F0 stars;
    Pos_001DA7F0 pic;
    Pos_001DA7F0 atkLabel;
    Pos_001DA7F0 atk;
    Pos_001DA7F0 defLabel;
    Pos_001DA7F0 def;
} Layout_001DA7F0;

typedef struct {
    s8 unused0 : 5;
    s8 card : 1;
    s8 unused6 : 2;
} GameFlags_001DA7F0;

typedef struct {
    u8 pad0[0x6A0];
    s32 slots[9];
    u8 pad6C4[0xD64 - 0x6C4];
    GameFlags_001DA7F0 flags;
} Game_001DA7F0;


typedef struct {
    s8 unused0 : 6;
    s8 alt : 1;
    s8 unused7 : 1;
} CardFlags_001DA7F0;

typedef struct {
    u8 pad0[0x3C];
    u16 id;
    u8 kind;
    u8 attr;
    u8 atk0;
    u8 def0;
    u8 atk1;
    u8 def1;
    u8 owner;
    u8 pad45[0x57 - 0x45];
    CardFlags_001DA7F0 flags;
} Card_001DA7F0;

extern Entry_001DA7F0 data_0028ff30[2][10];
extern Entry_001DA7F0 data_00290020[];
extern Entry_001DA7F0 gSzDuel_Icon_bytes[];
extern Layout_001DA7F0 data_002bed50[];
extern u16 data_002bed52[][16];
extern u16 data_002bed54[][16];
extern u16 data_002bed56[][16];
extern u16 data_002bed5c[][16];
extern u16 data_002bed5e[][16];
extern u16 data_002bed60[][16];
extern u16 data_002bed62[][16];
extern u16 data_002bed68[][16];
extern u16 data_002bed6a[][16];
extern char gSzDuel_aicolMsg_bytes[];

static inline void SetSprite_001DA7F0(Sprite_001DA7F0 *p, Entry_001DA7F0 *e, s32 x, s32 y, s32 w, s32 h) {
    ((UV_001DA7F0 *)&p->uv0)->u = (e->u0 << 4) + 8;
    ((UV_001DA7F0 *)&p->uv0)->v = (e->v0 << 4) + 8;
    ((UV_001DA7F0 *)&p->uv1)->u = (e->u1 << 4) + 8;
    ((UV_001DA7F0 *)&p->uv1)->v = (e->v1 << 4) + 8;
    p->xyz0.x = x << 4;
    p->xyz0.y = y << 4;
    p->xyz0.z = 0xFFFFFF;
    p->xyz0.f = 0;
    p->xyz1.x = (x + w) << 4;
    p->xyz1.y = (y + h) << 4;
    p->xyz1.z = 0xFFFFFF;
    p->xyz1.f = 0;
}

struct Draw_001DA7F0_s {
    u8 pad0[0x44];
    u8 b, g, r, a;
    u8 pad48[0x81 - 0x48];
    s8 unused81 : 4;
    s8 large : 1;
    s8 unused81b : 3;
    u8 pad82;
    u8 unused83 : 4;
    u8 blend : 2;
    u8 unused83b : 2;
};

s32 SzDuel_SetupCard_Pipe(s32 arg0, s32 unit, Draw_001DA7F0 *arg2) {
    extern s32 SzGame_GetMode();
    extern s32 *SzMsg_GetInfo(s32);
    extern s32 ExSortPrim_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
    Game_001DA7F0 *game;
    s32 sx;
    s32 sy;
    s32 mode;
    s32 picSize;
    s32 width;
    s32 fade;
    s32 kind;
    Entry_001DA7F0 *frameEntry;
    Entry_001DA7F0 *attrEntry;
    s32 level;
    s32 cardId;
    u16 starY;
    s32 large;
    s32 starW;
    s32 frame;
    s32 back;
    s32 hide;
    s32 digitW;
    s32 height;
    Entry_001DA7F0 *entry;
    s32 out;
    s32 tex;
    s32 attr;
    s32 p1;
    u8 p2;
    s8 alt;
    s32 altw;
    s32 atk;
    s32 def;
    s32 i;
    s32 n;
    s32 exch;
    s32 w;
    f32 scale;
    s32 *digits;
    s32 div;
    Layout_001DA7F0 *lay;
    Sprite_001DA7F0 sprite __attribute__((aligned(8)));
    Solid_001DA7F0 solid __attribute__((aligned(8)));

    game = (Game_001DA7F0 *)data_00317490.b;
    sx = 0x6C0;
    sy = 0x720;
    mode = SzGame_GetMode(1);
    if (unit != 0) {
        cardId = ((Card_001DA7F0 *)unit)->id;
    } else {
        cardId = -1;
    }
    picSize = 0x60;
    width = 0x78;
    frame = 0x20;
    digitW = 0x14;
    starW = 8;
    height = 0xB4;
    large = 0;
    fade = 0;
    back = 0;
    hide = 0;
    if (!arg2) {
        return (s32)arg2;
    }
    if (arg2->large) {
        picSize = 0xC0;
        large = 1;
        frame = 0x1F;
        digitW = 0x16;
        starW = 0x10;
        height = 0x170;
        width = 0x100;
    }
    if (arg2->blend != 0) {
        if (arg2->a != 0) {
            fade = 1;
        }
    }
    ((Tag_001DA7F0 *)sprite.tag)->prim = 0x26;
    ((Tag_001DA7F0 *)sprite.tag)->count = 5;
    ((Tag_001DA7F0 *)sprite.tag)->regs = 0x42421;
    ((Tag_001DA7F0 *)sprite.tag)->flags = 0;
    ((Tag_001DA7F0 *)sprite.tag)->prim |= 0x800;
    ExAssignAlpha(sprite.tag, 0x44 | ((s64)0x80 << 32));
    sprite.r = 0x80;
    sprite.g = 0x80;
    sprite.b = 0x80;
    sprite.a = 0x80;
    sprite.q = 1.0f;
    arg0 = (s32)RhBase_EnableRenderFrame((void *)arg0);
    if (game->flags.card != arg2->large) {
        arg0 = (s32)RhBase_ClearRenderFrame((u8 *)arg0, 0);
        game->flags.card = arg2->large;
    }
    if (cardId >= 0x356) {
        if (((Card_001DA7F0 *)unit)->owner <= 1) {
            back = 1;
        } else {
            cardId = -1;
        }
    }
    if (cardId == 0x29F) {
        cardId = -1;
    }
    if (SzGame_GetMenuNum(0x1E) != 0 && mode == 1 && unit != 0) {
        hide = lSzDuel_IsUnitHide((void *)unit);
    }
    if (back) {
        entry = &data_0028ff30[large][((Card_001DA7F0 *)unit)->owner + 8];
        arg0 = lSzDuel_SetupCardTexture(arg0, 8);
        ExAssignTexture(&sprite, (void *)game->slots[8], 0);
        SetSprite_001DA7F0(&sprite, entry, sx, sy, width, height);
        out = ExSortPrim_Pipe(arg0, (s32)&sprite, 0, 0);
    } else if (hide) {
        entry = &data_0028ff30[large][((Card_001DA7F0 *)unit)->owner + 8];
        tex = lSzDuel_SetupCardTexture(arg0, entry->tex);
        ExAssignTexture(&sprite, (void *)game->slots[entry->tex], 0);
        SetSprite_001DA7F0(&sprite, entry, sx, sy, width, height);
        out = ExSortPrim_Pipe(tex, (s32)&sprite, 0, 0);
    } else if (cardId >= 0) {
        kind = ((Card_001DA7F0 *)unit)->kind >> 5;
        attr = SzDuel_Kind2Attr(kind, ((Card_001DA7F0 *)unit)->attr);
        entry = &data_0028ff30[large][kind];
        frameEntry = &gSzDuel_Icon_bytes[frame];
        attrEntry = &data_00290020[attr];
        level = SzDuel_GetUnitLevel2((void *)unit, 0);
        altw = ((Card_001DA7F0 *)unit)->flags.alt;
        alt = altw;
        p1 = (u8)(altw ? ((Card_001DA7F0 *)unit)->atk1 : ((Card_001DA7F0 *)unit)->atk0);
        p2 = alt ? ((Card_001DA7F0 *)unit)->def1 : ((Card_001DA7F0 *)unit)->def0;
        atk = Ta_GetParamOfs(unit, p1, p2, 1);
        def = Ta_GetParamOfs(unit, p1, p2, 0);
        atk += SzDuel_GetUnitAP2((void *)unit, 0);
        def += SzDuel_GetUnitDP2((void *)unit, 0);
        atk = ((atk > 0 ? atk : 0) < 9999) ? (atk > 0 ? atk : 0) : 9999;
        def = ((def > 0 ? def : 0) < 9999) ? (def > 0 ? def : 0) : 9999;
        if (kind == 0 && SzDuel_GetUnitConstEffect2((void *)unit, 0) != NULL) {
            entry = &data_0028ff30[large][6];
        }
        if (arg2->large) {
            arg0 = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 9, SzDuel_GetMsgName(cardId), 0, 0xC6));
        }
        arg0 = lSzDuel_SetupCardTexture(arg0, entry->tex);
        ExAssignTexture(&sprite, (void *)game->slots[entry->tex], 0);
        ((UV_001DA7F0 *)&sprite.uv0)->u = (entry->u0 << 4) + 8;
        ((UV_001DA7F0 *)&sprite.uv0)->v = (entry->v0 << 4) + 8;
        ((UV_001DA7F0 *)&sprite.uv1)->u = (entry->u1 << 4) + 8;
        ((UV_001DA7F0 *)&sprite.uv1)->v = (entry->v1 << 4) + 8;
        sprite.xyz0.x = sx << 4;
        sprite.xyz0.y = sy << 4;
        sprite.xyz0.z = 0xFFFFFF;
        sprite.xyz0.f = 0;
        sprite.xyz1.x = (sx + width) << 4;
        sprite.xyz1.y = (sy + height) << 4;
        sprite.xyz1.z = 0xFFFFFF;
        sprite.xyz1.f = 0;
        arg0 = ExSortPrim_Pipe(arg0, (s32)&sprite, 0, 0);
        ExAssignTexture(&sprite, (void *)game->slots[frameEntry->tex], 0);
        ((UV_001DA7F0 *)&sprite.uv0)->u = (frameEntry->u0 << 4) + 8;
        ((UV_001DA7F0 *)&sprite.uv0)->v = (frameEntry->v0 << 4) + 8;
        ((UV_001DA7F0 *)&sprite.uv1)->u = (frameEntry->u1 << 4) + 8;
        ((UV_001DA7F0 *)&sprite.uv1)->v = (frameEntry->v1 << 4) + 8;
        i = 0;
        if (i < level) {
            do {
                starY = sy + starW;
                n = 11 - i;
                sprite.xyz0.x = (sx + starW * n + data_002bed50[large].stars.x + (u16)(large * n)) << 4;
                sprite.xyz0.y = (sy + data_002bed50[large].stars.y) << 4;
                sprite.xyz0.z = 0xFFFFFF;
                sprite.xyz0.f = 0;
                sprite.xyz1.x = (sx + starW * (n + 1) + data_002bed50[large].stars.x + (u16)(large * n)) << 4;
                sprite.xyz1.y = (starY + data_002bed50[large].stars.y) << 4;
                sprite.xyz1.z = 0xFFFFFF;
                sprite.xyz1.f = 0;
                arg0 = ExSortPrim_Pipe(arg0, (s32)&sprite, 0, 0);
                i++;
            } while (i < level);
        }
        exch = SzData_ExchSpecial(((Card_001DA7F0 *)unit)->owner, cardId);
        if (exch != cardId && SzText_GetReadyItem(1, exch) == NULL) {
            exch = cardId;
        }
        out = lSzDuel_DrawWindow_PicItem_Pipe(arg0, exch, data_002bed5c[large][0], data_002bed5e[large][0], picSize, picSize, 0xFFFFFF);
        if (arg2->large) {
            ExAssignTexture(&sprite, (void *)game->slots[attrEntry->tex], 0);
            ((UV_001DA7F0 *)&sprite.uv0)->u = (attrEntry->u0 << 4) + 8;
            ((UV_001DA7F0 *)&sprite.uv0)->v = (attrEntry->v0 << 4) + 8;
            ((UV_001DA7F0 *)&sprite.uv1)->u = (attrEntry->u1 << 4) + 8;
            ((UV_001DA7F0 *)&sprite.uv1)->v = (attrEntry->v1 << 4) + 8;
            sprite.xyz0.x = (sx + data_002bed54[large][0]) << 4;
            sprite.xyz0.y = (sy + data_002bed56[large][0]) << 4;
            sprite.xyz0.z = 0xFFFFFF;
            sprite.xyz0.f = 0;
            sprite.xyz1.x = (sx + 0x18 + data_002bed54[large][0]) << 4;
            sprite.xyz1.y = (sy + 0x18 + data_002bed56[large][0]) << 4;
            sprite.xyz1.z = 0xFFFFFF;
            sprite.xyz1.f = 0;
            out = ExSortPrim_Pipe(out, (s32)&sprite, 0, 0);
            w = SzMsg_GetInfo(9)[3];
            if ((s32)(w >= 0xC1) != 0) {
                scale = 192.0f / (f32)w;
            } else {
                scale = 1.0f;
            }
            out = SzMsg_DrawRatioX_Pipe(out, 9, data_002bed50[large].name.x, data_002bed52[large][0], scale, 1.0f, (s32)gSzDuel_aicolMsg_bytes, (void (*)(void))SzMsg_DrawRect_Pipe);
            SzMsg_Free(9);
        }
        if (kind == 0) {
            extern s32 Ta_GetParamOfs(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
            extern void *SzText_GetReadyItem(s32 arg0, s32 arg1);
            w = SzMsg_GetInfo(1)[3];
            if (w > 22) {
                scale = 22.0f / (f32)w;
            } else {
                scale = 1.0f;
            }
            out = SzMsg_DrawRatioX_Pipe(out, 1, data_002bed60[large][0], data_002bed62[large][0], scale, 1.0f, (s32)gSzDuel_aicolMsg_bytes, (void (*)(void))SzMsg_DrawRect_Pipe);
            digits = SzMsg_GetInfo(0);
            div = 1;
            lay = &data_002bed50[large];
            i = 0;
            do {
                digits[1] = (atk / div % 10) * 22;
                out = SzMsg_DrawX_Pipe(out, 0, lay->atk.x + digitW * (4 - i), lay->atk.y, (s32)gSzDuel_aicolMsg_bytes, (s32)SzMsg_DrawRect_Pipe);
                div *= 10;
                if (atk < div) {
                    break;
                }
                i++;
            } while (i < 4);
            w = SzMsg_GetInfo(2)[3];
            if (w > 22) {
                scale = 22.0f / (f32)w;
            } else {
                scale = 1.0f;
            }
            out = SzMsg_DrawRatioX_Pipe(out, 2, data_002bed68[large][0], data_002bed6a[large][0], scale, 1.0f, (s32)gSzDuel_aicolMsg_bytes, (void (*)(void))SzMsg_DrawRect_Pipe);
            digits = SzMsg_GetInfo(0);
            div = 1;
            i = 0;
            do {
                digits[1] = (def / div % 10) * 22;
                out = SzMsg_DrawX_Pipe(out, 0, lay->def.x + digitW * (4 - i), lay->def.y, (s32)gSzDuel_aicolMsg_bytes, (s32)SzMsg_DrawRect_Pipe);
                div *= 10;
                if (def < div) {
                    break;
                }
                i++;
            } while (i < 4);
        }
    } else {
        entry = &data_0028ff30[large][7];
        tex = lSzDuel_SetupCardTexture(arg0, entry->tex);
        ExAssignTexture(&sprite, (void *)game->slots[entry->tex], 0);
        SetSprite_001DA7F0(&sprite, entry, sx, sy, width, height);
        out = ExSortPrim_Pipe(tex, (s32)&sprite, 0, 0);
    }
    if (fade) {
        ((Tag_001DA7F0 *)solid.tag)->prim = 6;
        ((Tag_001DA7F0 *)solid.tag)->count = 3;
        ((Tag_001DA7F0 *)solid.tag)->regs = 0x441;
        ((Tag_001DA7F0 *)solid.tag)->flags = 0;
        ExAssignAlpha(solid.tag, ExGetAlphaVal(arg2->blend));
        solid.r = arg2->r;
        solid.g = arg2->g;
        solid.b = arg2->b;
        solid.a = arg2->a;
        solid.q = 1.0f;
        solid.xyz0.x = 0x6C00;
        solid.xyz0.y = 0x7200;
        solid.xyz0.z = 0xFFFFFF;
        solid.xyz0.f = 0;
        solid.xyz1.x = 0x8000;
        solid.xyz1.y = 0x8E00;
        solid.xyz1.z = 0xFFFFFF;
        solid.xyz1.f = 0;
        out = (s32)RhBase_MaskFBuffer((void *)ExSortPrim_Pipe((s32)RhBase_MaskFBuffer((void *)RhBase_SetZBufferMode(out, 0), 0xFF000000), (s32)&solid, 0, 0), 0);
    }
    RhBase_GetTextureInfoForRender((u8 *)arg2 + 0x60, (u8 *)arg2 + 0x68);
    return RhText_SetTextureFlush((s32)RhBase_DisableRenderFrame((void *)out));
}
