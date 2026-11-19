#include "dotr/ee/free_spots.h"

/* The body of 0x113b60. */
static inline int far_corner(int a, int b)
{
    if (b >= 5) {
        return 0x602;
    }
    if (a >= 5) {
        return 0x206;
    }
    if (a >= b) {
        return 0x105;
    }
    return 0x501;
}

/* The stage used when the level is not 0 or the +0x3c word is 83. */
static inline int last_low_spot(int side)
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
                if (SzDuel_GetField(fx, fy) != 9) {
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

/* The stage used after the scan around the packed spot. */
static inline int near_low_spot(int side, struct DotrFreeBlock *unit, struct DotrFreeNibbles *bytes)
{
    int free_spot;
    int near;
    int j;
    int fx;
    int fy;
    int near_spot;
    int to;
    int to_x;
    int to_y;
    int id;
    int cost;

    near = 14;
    near_spot = 0;
    for (j = 0; j < 8; j++) {
        free_spot = Ta_GetSommonPiecePreference(side, j);
        if (free_spot >= 0) {
            fx = free_spot & 0xff;
            fy = free_spot >> 8;
            if (SzDuel_GetField(fx, fy) != 8) {
                if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                    if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                        if (CheckLandAgreement(3, (struct DotrStateItem *)unit) == 1) {
                            to = far_corner(fx, fy);
                            to_x = to & 0xff;
                            to_y = to >> 8;
                            id = SzDuel_GetMapUnitID(to_x, to_y);
                            if (id < 0) {
                                id = 0xffff;
                            }
                            cost = func_002792c0_free(unit, bytes, fx, fy, to_x, to_y, id, 7);
                            if (cost < near && cost >= 0) {
                                near_spot = free_spot;
                                near = cost;
                            }
                        } else {
                            id = SzDuel_GetMapUnitID(3, 3);
                            if (id < 0) {
                                id = 0xffff;
                            }
                            cost = func_002792c0_free(unit, bytes, fx, fy, 3, 3, id, 7);
                            if (cost < near && cost >= 0) {
                                near_spot = free_spot;
                                near = cost;
                            }
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

int TaLogic001_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId)
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

    if (unit == 0) {
        return -1;
    }
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    kind = unit->kind >> 5;
    if (kind == 0 && unit->cardNo != 83) { /* a monster other than Lord of D. (card 83) */
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
                                if (func_00278510_free(unit, ownerId, TaLogic_GetWarnHideLvl__00117c30(side), here_x, here_y, 0) > 0) {
                                    cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 0, 5, TaLogic_GetWarnHideLvl__00117c30(side));
                                } else {
                                    cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, ownerId, 5);
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
        return near_low_spot(side, unit, bytes);
    } else {
        return last_low_spot(side);
    }
}
