#include "dotr/ee/item_release.h"

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
}

/* Lays out the hand for side `side`. The first byte of the list at +0xd2c of the control
 * block becomes 6. When the control block has a current item with an object, item (2, 0)
 * becomes a copy of it with a free request object (a copy of the current one, grey 128,
 * two flags) hung on slot 0, which is placed at (-125, -8) and whose index (0) replaces the 6.
 * Then each of the five hand slots that has an object and a card (SzDuel_InqHand) is placed at
 * x = -250 + 125 * position, moving to a target chosen from the card's bits 12-15, the mode
 * byte at +0xd4a and the bytes at +0xd4e / +0xd4f; its index goes into the list at the
 * card's number, or at the next free place after max(+0xd4e, 1) when the number is 0.
 * MATCHING DEVICE: the bounds test of the slot lookup is written out with `j` (as 0 for the
 * first slot) instead of the inline the neighbouring sources use; the inline gives the same
 * instructions with the slot and object registers exchanged. */
int lSzDuel_InitHandCombo(int side)
{
    struct DotrReleaseControl *ctl = &data_00317490_release;
    int next;
    struct DotrReleaseCurrent *item;
    int i;
    struct DotrReleaseSlot *s;
    int k;
    struct DotrReleaseObject *o;
    int tx;
    int y;
    struct DotrReleaseSlot *c;
    int x;
    int r;
    int j;

    next = (ctl->unknown_d4e > 1 ? ctl->unknown_d4e : 1) + 1;
    ctl->unknown_d2c[0] = 6;
    if (ctl->current != 0 && ctl->current->object != 0) {
        j = 0;
        if (j < 0 || j >= 6) {
            s = 0;
        } else {
            s = &gSzDuel_aCardControl_release[j];
        }
        item = (struct DotrReleaseCurrent *)SzDuel_GetCardInfo(2, 0);
        if (s != 0 && item != 0) {
            *item = *ctl->current;
            item->ctlRateM = 0;
            s->object = take();
            o = s->object;
            if (o != 0) {
                item->object = o;
                item->object->unknown_90 = item;
                *(struct DotrReleaseObjectBody *)o = *(struct DotrReleaseObjectBody *)ctl->current->object;
                o->unknown_40[3] = 128;
                o->unknown_40[0] = 128;
                o->unknown_40[1] = 128;
                o->unknown_40[2] = 128;
                o->unknown_83_bit_7 = -1;
                o->unknown_81_bit_5 = -1;
                s->unknown_9f_bit_3 = -1;
                s->unknown_10 = *(struct DotrStateVector *)o->unknown_00;
                Ex3d_GetPointOnScreen(&s->unknown_20, -125.0f, -8.0f);
                s->unknown_9f_bit_4 = -1;
                s->unknown_30 = *(struct DotrStateVector *)o->unknown_10;
                s->unknown_40[0] = 0;
                s->unknown_40[1] = 0x10000;
                s->unknown_40[2] = 0x10000;
                s->unknown_40[3] = 0;
                s->unknown_9f_bit_5 = -1;
                s->unknown_50[0] = 0;
                s->unknown_50[1] = 0;
                s->unknown_50[2] = 0;
                s->unknown_50[3] = 0;
                func_00205c40_owner(s->unknown_60);
                s->unknown_9f_bit_6 = -1;
                s->unknown_70 = *(struct DotrStateVector *)&o->unknown_30;
                s->unknown_80 = 0.46875f;
                s->unknown_84 = 45.0f / 92.0f;
                s->unknown_88 = 1.0f;
                s->unknown_8c = 0;
                s->unknown_98 = 128;
                s->unknown_9a = 0;
                SzDuel_DoCardControl0(s);
                ctl->unknown_d2c[0] = s->unknown_9c_bits_0_25 = 0;
            }
        }
        ctl->current->unknown_58_bit_0 = -1;
        ctl->current->unknown_57_bit_7 = -1;
        ctl->current->unknown_58_bit_2 = -1;
        ctl->current->object->unknown_83_bit_7 = -1;
    }
    x = -250;
    for (i = 0; i < 5; i++, x += 125) {
        j = i + 1;
        if (j < 0 || j >= 6) {
            c = 0;
        } else {
            c = &gSzDuel_aCardControl_release[j];
        }
        if (c != 0 && c->object != 0) {
            r = SzDuel_InqHand(side, i);
            if (r >= 0 && (r & 0xfff) != 999) {
                k = (r & 0xf000) >> 12;
                y = 448;
                tx = x;
                if (ctl->unknown_d4e == 0 && i == ctl->unknown_d4f) {
                    k = 1;
                }
                switch (ctl->unknown_d4a) {
                case 1:
                    if (k == 1) {
                        tx = 0;
                        y = -8;
                        c->unknown_9f_bit_4 = -1;
                    }
                    break;
                default:
                    if (k != 0) {
                        if (ctl->current != 0) {
                            tx = (k - 1) * 16 + 125;
                            y = -8;
                        } else if (k == 1) {
                            tx = -125;
                            y = -8;
                        } else {
                            tx = (k - 2) * 16 + 125;
                            y = -8;
                        }
                        c->unknown_9f_bit_4 = -1;
                    }
                    break;
                }
                mark(c, 0);
                Ex3d_GetPointOnScreen(&c->unknown_10, (float)x, -8.0f);
                Ex3d_GetPointOnScreen(&c->unknown_20, (float)tx, (float)y);
                c->unknown_30.x = 0;
                c->unknown_30.y = 0;
                c->unknown_30.z = 0;
                c->unknown_30.unknown_w = 0;
                c->unknown_40[0] = 0;
                c->unknown_40[1] = 0x10000;
                c->unknown_40[2] = 0;
                c->unknown_40[3] = 0;
                c->unknown_98 = 128;
                c->unknown_9a = 0;
                SzDuel_DoCardControl0(c);
                if (k == 0) {
                    ctl->unknown_d2c[next++] = c->unknown_9c_bits_0_25;
                } else {
                    ctl->unknown_d2c[k] = c->unknown_9c_bits_0_25;
                }
            }
        }
    }
    return 1;
}
