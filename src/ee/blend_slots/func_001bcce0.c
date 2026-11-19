#include "dotr/ee/blend_slots.h"

int lSzDuel_DrawBuildDeck_Ghost_Pipe(int a, int base, int out)
{
    struct DotrBlendSlot *p = data_00317b60;
    int near = base + 8;
    int far = base + 328;
    int i;
    int from, to;
    int level;
    unsigned char t;

    for (i = 0; i < 8; i++, p++) {
        level = p->level;
        if (level != 0) {
            t = level;
            if (p->b7) {
                t = 128 - level;
            }
            if (p->b5) {
                to = near;
                from = far;
            } else {
                to = far;
                from = near;
            }
            a = lSzDuel_DrawBuildDeck_GhostCard_Pipe(a, p->id, from + (((unsigned char)level * (to - from)) >> 7), out + 112 + p->n * 54, p->b6, t);
            p->level = p->level - 16 > 0 ? p->level - 16 : 0;
        }
    }
    return a;
}
