#include "dotr/ee/free_spots.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int TaLogic005_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit, int ownerId)
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
            if (ownerId >= 0) {
                best = 0xff;
                best_spot = 0xffff;
                here = SzDuel_GetUnitLoc(ownerId >> 8, ownerId & 0xff);
                for (i = 0, here_y = here >> 8, here_x = here & 0xff; i < 8; i++) {
                    if ((spot = TaLogic_GetSommonPiecePreference__00124f50(side, i)) >= 0) {
                        x = spot & 0xff;
                        y = spot >> 8;
                        if (SzDuel_GetField(x, y) != 8) {
                            if (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(unit, x, y) != 1) {
                                if (SzDuel_GetMapUnit(x, y) == 0) {
                                    if (func_00278510_free(unit, ownerId, func_001254a0_free(side), here_x, here_y, 0) > 0) {
                                        cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 0, 3, (unsigned short)func_001254a0_free(side));
                                    } else {
                                        cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, (unsigned short)ownerId, 3);
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
            {
            /* MATCHING DEVICE: these locals are declared in the order of the registers
             * the original gives them (s0 upwards). */
            int fy;
            int fx;
            int j;
            int near;
            int free_spot;
            int result;
            int d;
            struct DotrFreeSpotBest out;
            near = 14;
            result = -1;
            for (j = 0; j < 8; j++) {
                if ((free_spot = TaLogic_GetSommonPiecePreference__00124f50(side, j)) >= 0) {
                    fx = free_spot & 0xff;
                    fy = free_spot >> 8;
                    if (SzDuel_GetField(fx, fy) != 8) {
                        if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                            if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                                func_00123500_free(unit, fx, fy, &out);
                                d = out.cost;
                                if (d < near && d >= 0) {
                                    result = free_spot;
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
            {
            /* MATCHING DEVICE: the second pass has its own block and locals, declared
             * in register order; sharing the first pass's locals moves four registers. */
            int fy;
            int fx;
            int j;
            int other;
            for (j = 0; j < 8; j++) {
                if ((other = TaLogic_GetSommonPiecePreference__00124f50(side, j)) >= 0) {
                    fx = other & 0xff;
                    fy = other >> 8;
                    if (SzDuel_GetField(fx, fy) != 8) {
                        if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                            if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                                if (SzDuel_GetField(fx, fy) == 4) {
                                    return other;
                                }
                            }
                        }
                    }
                }
            }
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
                if ((free_spot = TaLogic_GetSommonPiecePreference__00124f50(side, j)) >= 0) {
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
        }
    }
    return -1;
}

