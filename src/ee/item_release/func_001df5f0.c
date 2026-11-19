#include "dotr/ee/item_release.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

static inline struct DotrReleaseObject *free_object(void)
{
    struct DotrReleaseObject *p = gSzDuel_aCardRequest;
    int i;

    for (i = 0; i < 32; i++, p++) {
        if (p->unknown_90 == 0) {
            SzDuel_ClearDrawCard((struct DotrOwnerDrawBlock *)p);
            return p;
        }
    }
    return 0;
}

static inline int card_index(int id)
{
    unsigned short *p = AlternateCardIds;
    int i;

    if (id == 671) {
        id = 999;
    }
    if (id < 0 || id >= 854) {
        return 999;
    }
    for (i = 0; i < 854; i++, p++) {
        if (*p == id) {
            return i;
        }
    }
    return 999;
}

static inline void copy_item(struct DotrStateItem *to, struct DotrStateItem *from)
{
    int *d = (int *)to;
    int *s = (int *)from;
    int n;

    for (n = 23; n > 0; n--) {
        *d++ = *s++;
    }
}

/* Places a copy of the item `src` on cell (x, y) for `side` and returns the new unit, or 0
 * when src is null, the cell is out of range or occupied, side is not 0 or 1, the card id
 * (item +0x3c; 671 reads as 999) is not below 854, or no unit slot is free. With `leader`
 * the id must be below 683 and only slot 0 is tried; otherwise slots 1..5 (id below 683)
 * or 6..10 are tried in order, a slot being free when its byte +0x45 is 11 or more.
 * The unit takes the 23 words of src, Ta_GetSummonRev(side, id) at +0x48, x, y, side and
 * slot at +0x40 / +0x41 / +0x44 / +0x45, and the cell's word points to it. A leader gets
 * its five float pairs from EffIdx_L[id]; any other unit gets a free card request
 * (first of the 32 whose +0x90 is zero, cleared with SzDuel_ClearDrawCard) when it has
 * none, the request's +0x90 and position (SzDuel_CalcUnitPos), the side's running number
 * at +0x47 and SzData_SetLookFlag(side, id, 1). For slots 0..5 a model is set up: number
 * side + 30 for the leader, else side * 5 + 20 + (slot - 1); preset with the id's index
 * in AlternateCardIds (999 when absent), animation 0, direction 1, speed 1, position
 * ((x - 3) * 50, y of data_002915d0, (y - 3) * 50), rotation data_002915e0 with y 0x8000
 * for side 0 and 0 otherwise, centre rotation data_002915f0, SzModel_SetRgba(model, 0, 0),
 * SzModel_SetFilter(model, 1, 0), then lSzDuel_DoUnitRequest(cell, unit).
 * The helpers above are this source's reading of inlined code (the cell and request
 * lookups have the same shape in SzDuel_MoveUnit and lSzDuel_InitHandInfo).
 * The signature is the one the callers declare; the result is the unit's
 * address (a 32-bit word on the target, truncated on a 64-bit host). */
int SzDuel_AddUnit2(int side, int x, int y, struct DotrStateBlock *src, int leader)
{
    struct DotrReleaseSide *rec = &data_00317490_sides[side];
    struct DotrStateCell *c = cell(x, y);
    int count = 1;
    int first = 0;
    int id;
    int i;
    int n;
    struct DotrStateItem *unit;
    int k;
    int model;

    if (src == 0) {
        return 0;
    }
    if (c == 0) {
        return 0;
    }
    if (c->unit != 0) {
        return 0;
    }
    id = ((struct DotrStateItem *)src)->cardNo;
    if (id == 671) {
        id = 999;
    }
    if (side < 0 || side >= 2 || id < 0 || id >= 854) {
        return 0;
    }
    if (leader != 0) {
        if (id >= 683) {
            return 0;
        }
    } else {
        count = 5;
        first = 1;
        if (id >= 683) {
            first = 6;
        }
    }
    for (i = 0; i < count; i++) {
        n = first + i;
        unit = &gSzDuel_aaVarInfo[side][n];
        if (unit->slot >= 11) {
            copy_item(unit, (struct DotrStateItem *)src);
            unit->lvRev = Ta_GetSummonRev(side, id);
            unit->locCol = x;
            unit->locRow = y;
            unit->team = side;
            unit->slot = n;
            c->unit = (DotrStateWord)unit;
            if (leader != 0) {
                for (k = 0; k < 5; k++) {
                    ((struct DotrReleaseItemPairs *)unit)->unknown_08[k] =
                        ((struct DotrReleasePair *)EffIdx_L[id])[k];
                }
            } else {
                if (unit->cardModel == 0) {
                    unit->cardModel = (struct DotrStateItemObject *)free_object();
                }
                if (unit->cardModel != 0) {
                    ((struct DotrReleaseObject *)unit->cardModel)->unknown_90 =
                        (struct DotrReleaseCurrent *)unit;
                    SzDuel_CalcUnitPos(x, y, 0, (struct DotrStateVector *)unit->cardModel);
                }
                unit->summonOrder = rec->numberOfUnitsSummoned++;
                SzData_SetLookFlag(side, id, 1);
            }
            k = card_index(id);
            if (first == 0 || first == 1) {
                struct DotrStateVector pos = data_002915d0;
                struct DotrReleaseAngles rot;
                struct DotrStateVector rotc;

                pos.x = (float)((x - 3) * 50);
                pos.z = (float)((y - 3) * 50);
                rot = data_002915e0;
                rot.y = (side != 0) ? 0 : 0x8000;
                rotc = data_002915f0;
                if (first == 0) {
                    model = side + 30;
                } else {
                    model = i + (side * count + 20);
                }
                unit->modelId = model;
                SzModel_Preset(model, -1, k);
                SzModel_SetAnimDirect(model, 0);
                SzModel_SetAnimDir(model, 1);
                SzModel_SetAnimSpeed(model, 1.0f);
                SzModel_SetPos(model, &pos);
                SzModel_SetRot(model, &rot);
                SzModel_SetRotC(model, &rotc);
                SzModel_SetRgba(model, 0, 0);
                SzModel_SetFilter(model, 1, 0);
                lSzDuel_DoUnitRequest(c, (DotrStateWord)unit);
            }
            return (int)(DotrStateWord)unit;
        }
    }
    return 0;
}
