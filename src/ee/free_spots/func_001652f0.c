#include "dotr/ee/free_spots.h"
/* The packed spot tried on each of the five rounds of the last stage. */
static inline unsigned short round_spot(unsigned char round)
{
    switch (round) {
    case 0:
        return 0x203;
    case 1:
        return 0x301;
    case 2:
        return 0x305;
    case 3:
        return 0x202;
    case 4:
        return 0x204;
    }
    return 0x203;
}
/* The stage used after the scan around the packed spot.
   MATCHING DEVICE: the corner choice (the body of TaLogic020_GetNextGrassPoint, 0x163f90) is pasted here with
   gotos; as a nested inline helper the compiler calls it out of line. */
static inline int near_low_spot(int side, struct DotrFreeBlock *unit, struct DotrFreeNibbles *bytes)
{
    int free_spot;
    int near;
    int j;
    int to_x;
    int fx;
    int fy;
    int near_spot;
    int to;
    int to_y;
    int id;
    int cost;
    near = 14;
    near_spot = -1;
    for (j = 0; j < 8; j++) {
        free_spot = Ta_GetSommonPiecePreference(side, j);
        if (free_spot >= 0) {
            fx = free_spot & 0xff;
            fy = free_spot >> 8;
            if (SzDuel_GetField(fx, fy) != 8) {
                if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                    if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                        if (fy == 0) {
                            if (fx >= 3) {
                                to = 0x106;
                                goto found;
                            }
                            if (fx <= 2) {
                                to = 0x100;
                                goto found;
                            }
                        }
                        if (fy <= 5) {
                            if (fx >= 3) {
                                to = 0x605;
                                goto found;
                            }
                            if (fx <= 2) {
                                to = 0x601;
                                goto found;
                            }
                        }
                        if (fx >= 3) {
                            to = 0x601;
                            goto found;
                        }
                        to = 0x605;
                    found:
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
int TaLogic020_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId)
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
    if (kind == 0 || kind == 5) {
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
                                if (func_00278510_free(unit, ownerId, TaLogic_GetWarnHideLvl__001676d0(side), here_x, here_y, 1) > 0) {
                                    cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 1, 5, TaLogic_GetWarnHideLvl__001676d0(side));
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
        /* Levels other than 0 and 5: five fixed places in turn. MATCHING DEVICE:
           written in place so that these locals get their stack slots before the
           inline stage's. */
        int round;
        int j;
        int fy;
        int fx;
        int near;
        int free_spot;
        int near_spot;
        int to;
        int to_y;
        int to_x;
        int id;
        int cost;
        for (round = 0; round < 5; round++) {
            to = round_spot(round);
            near_spot = -1;
            near = 14;
            if (to >= 0) {
                for (j = 0, to_y = to >> 8, to_x = to & 0xff; j < 8; j++) {
                    free_spot = Ta_GetSommonPiecePreference(side, j);
                    if (free_spot >= 0) {
                        fx = free_spot & 0xff;
                        fy = free_spot >> 8;
                        if (SzDuel_GetField(fx, fy) != 8) {
                            if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                                if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                                    id = SzDuel_GetMapUnitID(fx, fy);
                                    if (id < 0) {
                                        id = 0xffff;
                                    }
                                    cost = func_002792c0_free(unit, bytes, fx, fy, to_x, to_y, id, 7);
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
            }
        }
        return -1;
    }
}
