#include "dotr/ee/free_spots.h"

/* MATCHING DEVICE: the order of the local declarations is the one that reproduces the
 * register and stack-slot assignment; it is not taken from the debug information. */
int TaLogic002_GetSummonPiece(int side, int kind, struct DotrFreeBlock *unit, int ownerId)
{
    struct DotrFreeNibbles bytes[12];
    int i;
    int inv;
    int id;
    int last_y;
    int last_x;
    int here;
    int lvl;
    int best_spot;
    int ny;
    struct DotrFreeNibbles *p;
    int cost;
    int best;
    int spot;
    int nx;
    int j;
    int m;
    int mode;
    int y;
    int last;
    int here_y;
    int field;
    int round;
    int near_spot;
    int near;
    int to_y;
    int near_at;
    int here_x;
    int to_x;
    int line;
    int x;
    int free_spot;
    int near_cost;

    if (unit == 0) {
        return -1;
    }
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    SzDuel_GetUnitLoc(side, 0);
    /* MATCHING DEVICE: the three kinds are tested one by one and the two stages are
     * reached by labels; `kind == 0 || kind == 3 || kind == 4` with an else compiles to
     * a range test and gives the last stage its own `return -1`. */
    if ((mode = (kind = (unsigned char)(unit->kind >> 5))) != 0 && kind != 3) {
        if (kind != 4) goto tail;
    }
    {
        if (ownerId >= 0) {
            best = 0xff;
            best_spot = 0xffff;
            here = SzDuel_GetUnitLoc(ownerId >> 8, ownerId & 0xff);
            lvl = TaLogic_GetWarnHideLvl__0010eda0(side);
            here_y = here >> 8;
            here_x = here & 0xff;
            inv = func_00278510_free(unit, ownerId, lvl, here_x, here_y, 0);
            for (i = 0; i < 8; i++) {
                if ((spot = TaLogic_GetSommonPiecePreference__0010ed90(side, i)) >= 0) {
                    x = spot & 0xff;
                    y = spot >> 8;
                    field = SzDuel_GetField(x, y);
                    if (field != 8) {
                        if (SzDuel_GetMapUnit(x, y) == 0) {
                            if (field != 9 || func_0024fe40_free(unit, x, y) != 1) {
                                for (j = 0, p = bytes; j < 12; j++, p++) p->low = p->high = 0xf;
                                if (inv >= 0) {
                                    cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 0, 2, (unsigned short)TaLogic_GetWarnHideLvl__0010eda0(side));
                                } else {
                                    cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, (unsigned short)ownerId, 2);
                                }
                                if (func_0027afc0_free(bytes) > 0 && cost < best && cost >= 0) {
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
            best_spot = -1;
            return best_spot;
        }
        for (round = 0; round < (mode == 0 ? 6 : 4); round++) {
            line = (unsigned short)TaLogic002_GetLinePiece(mode == 0 ? 1 : 2, round);
            near_spot = 0xffff;
            near = 14;
            if (line >= 0) {
                for (j = 0, to_y = line >> 8, to_x = line & 0xff; j < 8; j++) {
                    if ((near_at = TaLogic_GetSommonPiecePreference__0010ed90(side, j)) >= 0) {
                        nx = near_at & 0xff;
                        ny = near_at >> 8;
                        if (SzDuel_GetField(nx, ny) != 8) {
                            if (SzDuel_GetField(nx, ny) != 9) {
                                if (SzDuel_GetMapUnitID(nx, ny) < 0) {
                                    id = SzDuel_GetMapUnitID(nx, ny);
                                    if (id < 0) {
                                        id = 0xffff;
                                    }
                                    near_cost = func_002792c0_free(unit, bytes, nx, ny, to_x, to_y, (unsigned short)id, 7);
                                    if (near_cost < near && near_cost >= 0) {
                                        near_spot = near_at;
                                        near = near_cost;
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
        goto none;
    }
tail:
    last = 0xffff;
    for (m = 0; m < 8; m++) {
        if ((free_spot = TaLogic_GetSommonPiecePreference__0010ed90(side, m)) >= 0) {
            last_x = free_spot & 0xff;
            last_y = free_spot >> 8;
            if (SzDuel_GetField(last_x, last_y) != 8) {
                if (SzDuel_GetField(last_x, last_y) != 9) {
                    if (SzDuel_GetMapUnitID(last_x, last_y) < 0) {
                        if (last_x >= 0) {
                            last = free_spot;
                        }
                    }
                }
            }
        }
    }
    if (last != 0xffff) {
        return last;
    }
none:
    return -1;
}
