#include "dotr/ee/free_spots.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int TaLogic007_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId)
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
    int mode;

    (void)kind;
    if (unit == 0) {
        return -1;
    }
    {
        for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
        mode = (unsigned char)(unit->kind >> 5);
        if (mode == 0 || mode == 3) {
            goto first;
        }
        if (mode == 2) {
        first:
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
                                    if (func_00278510_free(unit, ownerId, TaLogic_GetWarnHideLvl__00130780(side), here_x, here_y, 0) > 0) {
                                        cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 0, 5, (unsigned short)TaLogic_GetWarnHideLvl__00130780(side));
                                    } else {
                                        cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, (unsigned short)ownerId, 2);
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
                best_spot = -1;
                return best_spot;
            }
            /* MATCHING DEVICE: this stage keeps its position in `spot` and its column in
             * `here_x`, the first stage's locals; with locals of its own the row and the
             * position trade saved registers. */
            {
            int ty;
            int tx;
            int fy;
            int j;
            int near;
            int result;
            int there;
            int d;
            near = 14;
            result = -1;
            there = SzDuel_GetUnitLoc(side == 0 ? 1 : 0, 0);
            for (j = 0, ty = there >> 8, tx = there & 0xff; j < 8; j++) {
                spot = Ta_GetSommonPiecePreference(side, j);
                if (spot >= 0) {
                    here_x = spot & 0xff;
                    fy = spot >> 8;
                    if (SzDuel_GetField(here_x, fy) != 8) {
                        if (SzDuel_GetField(here_x, fy) != 9 || func_0024fe40_free(unit, here_x, fy) != 1) {
                            if (SzDuel_GetMapUnitID(here_x, fy) < 0) {
                                d = (MAX(here_x, tx) - MIN(here_x, tx)) + (MAX(fy, ty) - MIN(fy, ty));
                                if (d < near && d >= 0) {
                                    result = spot;
                                    near = d;
                                }
                            }
                        }
                    }
                }
            }
            if (near < 14) {
                return result;
            }
            }
        } else {
            int fy;
            int fx;
            int j;
            int last;
            int free_spot;
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
        }
    }
    return -1;
}
