#include "dotr/ee/item_release.h"

/* The hand slot `base` when the index is one of the six, else none. The original tests the
 * index although the loop below never leaves the range; the helper names are not original. */
static inline struct DotrReleaseSlot *slot(struct DotrReleaseSlot *base, int index)
{
    if (index < 0 || index >= 6) {
        return 0;
    }
    return base;
}

static inline void step(int side, int flag, int i, struct DotrReleaseSlot *base)
{
    struct DotrReleaseSlot *s;
    struct DotrReleaseObject *o;
    int limit;
    int v;

    s = slot(base, i);
    if (s != 0) {
        o = s->object;
        if (o != 0) {
            limit = SzDuel_GetSummonPower(side);
            v = SzDuel_GetUnitLevel2((struct DotrStateItem *)s->object->unknown_90, 0);
            if (flag && limit < v) {
                o->unknown_40[0] = 64;
                o->unknown_40[1] = 64;
                o->unknown_40[2] = 64;
            } else {
                o->unknown_40[0] = 128;
                o->unknown_40[1] = 128;
                o->unknown_40[2] = 128;
            }
        }
    }
}

int lSzDuel_InitHandDark(int side, int flag)
{
    int t = SzGame_GetMenuNum(29);
    int on = (t & (1 << side)) ? 1 : 0;
    int other = side ^ 1;
    int i;
    struct DotrReleaseSlot *base;

    if (on) {
        if (SzDuel_GetPlayer(side) < 32 && SzDuel_GetPlayer(other) >= 100) {
            flag = 0;
        }
    } else if ((SzPad_Check(side, 1, 2) & 0xffff) == 0) {
        flag = 0;
    }
    for (i = 0, base = gSzDuel_aCardControl_release; i < 6; i++, base++) {
        step(side, flag, i, base);
    }
    return 1;
}
