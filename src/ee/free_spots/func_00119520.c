#include "dotr/ee/free_spots.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int TaLogic003_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *unit)
{
    struct DotrFreeNibbles bytes[12];
    int best_spot;
    int best;
    int i;
    struct DotrFreeNibbles *p;
    int spot;
    int ownerId;
    int corner;
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
        if (mode == 0) {
            corner = SzDuel_GetUnitLoc(side, 0);
            ownerId = func_00276f10_free(unit, corner & 0xff, corner >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_0011aa10_key(side));
            if (ownerId >= 0) {
                best = 0xff;
                best_spot = 0xffff;
                here = SzDuel_GetUnitLoc(ownerId >> 8, ownerId & 0xff);
                for (i = 0, here_y = here >> 8, here_x = here & 0xff; i < 8; i++) {
                    if ((spot = TaLogic_GetSommonPiecePreference__0011a4c0(side, i)) >= 0) {
                        x = spot & 0xff;
                        y = spot >> 8;
                        if (SzDuel_GetField(x, y) != 8) {
                            if (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(unit, x, y) != 1) {
                                if (SzDuel_GetMapUnit(x, y) == 0) {
                                    if (func_00278510_free(unit, ownerId, func_0011aa10_key(side), here_x, here_y, 0) > 0) {
                                        cost = func_0027a130_free(unit, bytes, x, y, here_x, here_y, ownerId, 0, 3, (unsigned short)func_0011aa10_key(side));
                                    } else {
                                        cost = func_002792c0_free(unit, bytes, x, y, here_x, here_y, (unsigned short)ownerId, 3);
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
            {
            int fy;
            int fx;
            int j;
            int near;
            int free_spot;
            int ty;
            int tx;
            int k;
            int target;
            int result;
            int d;
            int id;
            /* the body of TaLogic003_GetEmptyAtkLineStart (0x118dd0), inlined in the
             * original: the first of four keys TaLogic003_IsEmptyAtkLine accepts picks
             * the place to cost towards */
            for (k = 0; k < 4; k++) {
                if (TaLogic003_IsEmptyAtkLine(side, (unsigned char)k) != 0) {
                    switch (k) {
                    case 0: target = 0x102; goto found;
                    case 1: target = 0x104; goto found;
                    case 2: target = 0x100; goto found;
                    default:
                    case 3: target = 0x106; goto found;
                    }
                }
            }
            target = -1;
found:
            near = 14;
            result = -1;
            for (j = 0, ty = target >> 8, tx = target & 0xff; j < 8; j++) {
                if ((free_spot = TaLogic_GetSommonPiecePreference__0011a4c0(side, j)) >= 0) {
                    fx = free_spot & 0xff;
                    fy = free_spot >> 8;
                    if (SzDuel_GetField(fx, fy) != 8) {
                        if (SzDuel_GetField(fx, fy) != 9 || func_0024fe40_free(unit, fx, fy) != 1) {
                            if (SzDuel_GetMapUnitID(fx, fy) < 0) {
                                id = SzDuel_GetMapUnitID(fx, fy);
                                if (id < 0) id = 0xffff;
                                d = func_002792c0_free(unit, bytes, fx, fy, tx, ty, (unsigned short)id, 7);
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
            }
        } else {
            int fy;
            int fx;
            int j;
            int last;
            int free_spot;
            last = -1;
            for (j = 0; j < 8; j++) {
                if ((free_spot = TaLogic_GetSommonPiecePreference__0011a4c0(side, j)) >= 0) {
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

