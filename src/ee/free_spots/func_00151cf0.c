#include "dotr/ee/free_spots.h"

/* The stage used when the level is not 0 and its type is not 2. */
static inline int last_low_spot(int side, struct DotrFreeBlock *unit)
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
                        if (fy < 8) {
                            last = free_spot;
                        }
                    }
                }
            }
        }
    }
    if (last >= 0) {
        return last;
    }
    last = -1;
    return last;
}

int TaLogic015_GetSummonPiece(int side, long kind, struct DotrFreeBlock *unit, int ownerId)
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
    int fy;
    int fx;
    int j;
    int free_spot;
    unsigned char k;

    if (unit == 0) {
        return -1;
    }
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    /* MATCHING DEVICE: a 64-bit `kind` copied into a byte local; with a narrower
     * parameter the compiler tests and passes one register instead of the
     * original's a1 and a0 copy. */
    kind = (unsigned char)(unit->kind >> 5);
    k = kind;
    if (k == 0 || SzDuel_Kind2Type(k) == 2) {
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
                                if (func_00278510_free(unit, ownerId, TaLogic_GetWarnHideLvl__00153ed0(side), here_x, here_y, 0) > 0) {
                                    cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 0, 3, TaLogic_GetWarnHideLvl__00153ed0(side));
                                } else {
                                    cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, ownerId, 3);
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
        SzDuel_GetUnitLoc(ownerId >> 8, ownerId & 0xff);
        for (j = 0; j < 8; j++) {
            free_spot = Ta_GetSommonPiecePreference(side, j);
            if (free_spot >= 0) {
                fx = free_spot & 0xff;
                fy = free_spot >> 8;
                if (SzDuel_GetField(fx, fy) != 8) {
                    if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                        if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                            return free_spot;
                        }
                    }
                }
            }
        }
        return -1;
    } else {
        return last_low_spot(side, unit);
    }
}
