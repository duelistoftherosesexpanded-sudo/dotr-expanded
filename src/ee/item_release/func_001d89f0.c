#include "dotr/ee/item_release.h"

static inline struct DotrReleaseSlot *slot(int index)
{
    if (index < 0 || index >= 6) {
        return 0;
    }
    return &gSzDuel_aCardControl_release[index];
}

static inline struct DotrReleaseObject *take(void)
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

static inline void flags(struct DotrReleaseObject *o, int v)
{
    o->unknown_83_bit_7 = -1;
    o->unknown_81_bit_4 = v;
    o->unknown_81_bit_6 = v;
    o->unknown_81_bit_5 = -1;
}

/* Fills item (2, fieldSlot) for side a, and unless its id is 999 gives slot fieldSlot an object (a free
 * one of the 32 when it has none), sets it up and places the slot with lSzDuel_SetCardCtl. */
int lSzDuel_InitHandInfo(int a, int fieldSlot, int cardId, int d, int e, int f, int g)
{
    struct DotrReleaseSide *side = &data_00317490_sides[a];
    struct DotrReleaseCurrent *item = (struct DotrReleaseCurrent *)SzDuel_GetCardInfo(2, fieldSlot);
    struct DotrReleaseSlot *s;
    struct DotrReleaseObject *o;
    int id;

    if (item == 0) {
        return 0;
    }
    SzDuel_InitCardInfo((struct DotrStateBlock *)item, cardId);
    item->team = a;
    item->slot = fieldSlot;
    item->locCol = side->selectedCol;
    item->locRow = side->selectedRow;
    id = item->cardNo;
    if (id != 999) {
        s = slot(fieldSlot);
        item->lvRev = Ta_GetSummonRev(a, id);
        if (s != 0) {
            if (s->object == 0) {
                s->object = take();
            }
            o = s->object;
            if (o != 0) {
                item->object = o;
                item->object->unknown_90 = item;
                o->unknown_30 = 0.46875f;
                o->unknown_34 = 45.0f / 92.0f;
                o->unknown_38 = 1.0f;
                o->unknown_3c = 0;
                o->unknown_10[0] = 0;
                o->unknown_10[1] = 0;
                o->unknown_10[2] = 0;
                o->unknown_10[3] = 0;
                flags(o, 0);
                func_00205c40_owner(o->unknown_20);
                s->unknown_9f_bit_2 = -1;
                s->unknown_9f_bit_3 = -1;
                lSzDuel_SetCardCtl(s, d, e, f, g, 0, 128);
                s->unknown_9c_bits_0_25 = fieldSlot;
            }
        }
    }
    item->unknown_57_bit_7 = -1;
    item->unknown_58_bit_0 = -1;
    item->unknown_58_bit_2 = -1;
    return 1;
}
