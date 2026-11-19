#include "dotr/ee/free_spots.h"

/* The second stage, which the function holds twice (once per +0x3e outcome):
 * written once as an inline so that both copies come out alike. */
static inline int last_spot(int side, struct DotrFreeBlock *unit)
{
    int free_spot;
    int last;
    int j;
    int fx;
    int fy;

    last = -1;
    for (j = 0; j < 8; j++) {
        free_spot = Ta_GetSommonPiecePreference(side, j);
        if (free_spot >= 0) {
            fx = free_spot & 0xff;
            fy = free_spot >> 8;
            if (SzDuel_GetField(fx, fy) != 8) {
                if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                    if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                        last = free_spot;
                    }
                }
            }
        }
    }
    return last;
}

int TaLogic009_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId)
{
    struct DotrFreeNibbles bytes[12];
    int best_spot;
    int best;
    int i;
    struct DotrFreeNibbles *p;
    int spot;
    int cost;
    int here;
    int y;
    int x;
    int here_y;
    int here_x;

    (void)kind;
    if (unit == 0) {
        return -1;
    }
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    if ((unsigned char)(unit->kind >> 5) == 0) {
        if (ownerId >= 0) {
            best = 0xff;
            best_spot = 0xffff;
            here = SzDuel_GetUnitLoc(ownerId >> 8, ownerId & 0xff);
            for (i = 0, here_y = here >> 8, here_x = here & 0xff; i < 8; i++) {
                spot = Ta_GetSommonPiecePreference(side, i);
                if (spot >= 0) {
                    x = spot & 0xff;
                    y = spot >> 8;
                    if (SzDuel_GetField(x, y) != 8) {
                        if (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(unit, x, y) != 1) {
                            if (SzDuel_GetMapUnit(x, y) == 0) {
                                if (func_00278510_free(unit, ownerId, 0, here_x, here_y, 1) > 0) {
                                    cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 1, 2, 0);
                                } else {
                                    cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, ownerId, 2);
                                }
                                if (func_0027afc0_free(bytes) > 0 && cost < best) {
                                    best = cost;
                                    best_spot = spot;
                                }
                            }
                        }
                    }
                }
            }
            if (best_spot != 0xffff) {
                return best_spot;
            }
        }
        return last_spot(side, unit);
    } else {
        return last_spot(side, unit);
    }
}
