#include "dotr/ee/item_release.h"

static inline struct DotrReleaseSlot *slot(int index)
{
    if (index < 0 || index >= 6) {
        return 0;
    }
    return &gSzDuel_aCardControl_release[index];
}

static inline void release(struct DotrReleaseObject *object)
{
    if (object != 0) {
        SzDuel_ClearDrawCard((struct DotrOwnerDrawBlock *)object);
        object->unknown_90 = 0;
    }
}

static inline void drop(struct DotrReleaseSlot *s)
{
    struct DotrReleaseObject *object;

    if (s != 0) {
        object = s->object;
        if (object != 0) {
            release(object);
        }
        SzDuel_ClearCardControl((struct DotrMiscSlot160 *)s);
    }
}

static inline void clear(struct DotrReleaseControl *c, int v)
{
    c->current->unknown_57_bit_7 = v;
    c->current->unknown_58_bit_0 = v;
    c->current->unknown_58_bit_2 = v;
    c->current->object->unknown_83_bit_7 = v;
}

/* Gives back slot 0 when the current element has an object, then places slots 1-5
 * for the positions SzDuel_InqHand reports as used and numbers the byte list. */
int lSzDuel_InitHandCancel(int index)
{
    struct DotrReleaseControl *c = &data_00317490_release;
    int i;
    int x;
    struct DotrReleaseSlot *s;

    if (data_00317b5c_release != 0) {
        if (c->current->object != 0) {
            s = slot(0);
            if (s != 0 && s->object != 0) {
                drop(s);
            }
            clear(c, 0);
        }
    }
    x = -250;
    for (i = 0; i < 5; i++, x += 125) {
        int v = SzDuel_InqHand(index, i);

        if (v < 0) {
            continue;
        }
        if ((v & 0xfff) == 999) {
            continue;
        }
        s = slot(i + 1);
        if (s != 0 && s->object != 0) {
            s->unknown_9f_bit_2 = -1;
            s->unknown_9f_bit_4 = 0;
            lSzDuel_SetCardCtl(s, x, 448, x, -8, 128, 128);
        }
    }
    c->unknown_d4a = SzDuel_GetHandCount(index);
    for (i = 0; i < c->unknown_d4a; i++) {
        c->unknown_d2c[i] = i + 1;
    }
    return 1;
}
