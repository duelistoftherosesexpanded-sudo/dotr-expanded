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

static inline void mark(struct DotrReleaseSlot *s, int v)
{
    s->unknown_9f_bit_2 = v;
    s->unknown_9f_bit_3 = -1;
}

/* As lSzDuel_InitHandInfo with other flag values and lSzDuel_SetCardCtl2 placing the slot; the
 * Ta_GetSummonRev byte is stored after the slot work. */
int lSzDuel_InitHandInfo2(int a, int unitArrayPos, int c, int d, int e)
{
    struct DotrReleaseSide *side = &data_00317490_sides[a];
    struct DotrReleaseCurrent *item = (struct DotrReleaseCurrent *)SzDuel_GetCardInfo(2, unitArrayPos);
    struct DotrReleaseSlot *s;
    struct DotrReleaseObject *o;

    if (item == 0) {
        return 0;
    }
    SzDuel_InitCardInfo((struct DotrStateBlock *)item, c);
    item->team = a;
    item->slot = unitArrayPos;
    item->locCol = side->selectedCol;
    item->locRow = side->selectedRow;
    if (item->cardNo != 999) {
        s = slot(unitArrayPos);
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
                o->unknown_83_bit_6 = -1;
                o->unknown_83_bit_7 = -1;
                o->unknown_81_bit_4 = -1;
                o->unknown_81_bit_6 = -1;
                o->unknown_81_bit_5 = -1;
                func_00205c40_owner(o->unknown_20);
                mark(s, 0);
                lSzDuel_SetCardCtl2(s, d, e, d, e, 128, 128);
                s->unknown_9c_bits_0_25 = unitArrayPos;
            }
        }
        item->lvRev = Ta_GetSummonRev(a, item->cardNo);
    }
    item->unknown_57_bit_7 = -1;
    item->unknown_58_bit_0 = -1;
    item->unknown_58_bit_2 = -1;
    return 1;
}
