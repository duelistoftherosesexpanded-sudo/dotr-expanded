#include "dotr/ee/blend_slots.h"

int lSzDuel_ReqGhostCard(short id, unsigned char n, int b5, int b6, int b7)
{
    struct DotrBlendSlot *p = data_00317b60;
    int i;

    for (i = 0; i < 8; i++, p++) {
        if (p->level == 0) {
            p->id = id;
            p->n = n;
            p->b5 = (signed char)b5;
            p->b6 = (signed char)b6;
            p->b7 = (signed char)b7;
            p->level = 0x80;
            return 1;
        }
    }
    return 0;
}
