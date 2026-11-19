#include "dotr/ee/logic014_summon.h"

/* The search of 0x14f3a0, expanded in place: the first of lines 0..2 whose cell holds no
 * unit gives the turn owner's preference 0, 3 or 4. MATCHING DEVICE: the compare written
 * as `(... == 0 ? 1 : 0) == 1` and the switch with a default the loop never reaches. */
static inline int find_spot(void)
{
    int i;
    int spot;
    int r;
    int side;

    for (i = 0; i < 3; i++) {
        spot = TaLogic014_GetLine(i);
        if ((SzDuel_GetMapUnit(spot & 0xff, spot >> 8) == 0 ? 1 : 0) == 1) {
            side = SzDuel_GetTurnOwner();
            switch (i) {
            case 0:
                r = Ta_GetSommonPiecePreference(side, 0);
                break;
            case 1:
                r = Ta_GetSommonPiecePreference(side, 3);
                break;
            case 2:
                r = Ta_GetSommonPiecePreference(side, 4);
                break;
            default:
                r = -1;
                break;
            }
            return r;
        }
    }
    return -1;
}

/* The stage for any other level: the last free preference cell (not field 8, not field 9
 * with the virus flag, no unit) whose row is below 8, else -1. MATCHING DEVICE: written as
 * an inline helper, the form that reproduces the register assignment. */
static inline int last_low_spot(int side, struct DotrFreeBlock *block)
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
                if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(block, fx, fy) != 1) {
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

int TaLogic014_GetSummonPiece(int side, int kind, struct DotrFreeBlock *block, int packed)
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
    unsigned char mode;

    if (block == 0) {
        return -1;
    }
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    /* MATCHING DEVICE: the level is stored in the second parameter, then copied to a byte. */
    kind = (unsigned char)(block->kind >> 5);
    mode = kind;
    if (mode == 0 || SzDuel_Kind2Type(mode) == 2) {
        if (packed >= 0) {
            best = 0xff;
            best_spot = 0xffff;
            here = SzDuel_GetUnitLoc(packed >> 8, packed & 0xff);
            for (i = 0, here_y = here >> 8, here_x = here & 0xff; i < 8; i++) {
                spot = Ta_GetSommonPiecePreference(side, i);
                if (spot >= 0) {
                    x = spot & 0xff;
                    y = spot >> 8;
                    if (SzDuel_GetField(x, y) != 8) {
                        if (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(block, x, y) != 1) {
                            if (SzDuel_GetMapUnit(x, y) == 0) {
                                if (func_00278510_free(block, packed, TaLogic_GetWarnHideLvl__0014f9b0(side), here_x, here_y, 0) > 0) {
                                    cost = func_0027a130_free(block, bytes, x, y, here_x, here_y, packed, 0, 3, TaLogic_GetWarnHideLvl__0014f9b0(side));
                                } else {
                                    cost = func_002792c0_free(block, bytes, x, y, here_x, here_y, packed, 3);
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
        /* MATCHING DEVICE: the fallback's locals in their own block, in this order. */
        {
            int fy;
            int fx;
            int to_y;
            int to_x;
            int j;
            int free_spot;
            int near;
            int near_spot;
            int to;
            int cost;

            near = 14;
            near_spot = -1;
            for (j = 0; j < 8; j++) {
                free_spot = Ta_GetSommonPiecePreference(side, j);
                if (free_spot >= 0) {
                    fx = free_spot & 0xff;
                    fy = free_spot >> 8;
                    if (SzDuel_GetField(fx, fy) != 8) {
                        if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(block, fx, fy) != 1) {
                            if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                                to = find_spot();
                                to_x = to & 0xff;
                                to_y = to >> 8;
                                /* the answer is not used (as in the original) */
                                SzDuel_GetMapUnitID(to_x, to_y);
                                cost = func_002792c0_free(block, bytes, fx, fy, to_x, to_y, 0, 7);
                                if (cost < near && cost >= 0) {
                                    near_spot = free_spot;
                                    near = cost;
                                }
                            }
                        }
                    }
                }
            }
            if (near < 14) {
                return near_spot;
            }
            near_spot = -1;
            return near_spot;
        }
    } else {
        return last_low_spot(side, block);
    }
}
