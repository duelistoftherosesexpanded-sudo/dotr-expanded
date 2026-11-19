#include "dotr/ee/summon_pieces.h"

/* TaLogic006_GetSummonPiece (0x128d40): -1 for a null block. When the top three bits of the
 * block byte +0x3e are 0 and `packed` is not negative: among the eight preferred places of
 * `side` (fields other than 8, and other than 9 with 0x24fe40 == 1, no unit), the one with the
 * lowest route cost below 255 (0x27a130 with mode 5 when 0x278510 reports a route, else 0x2792c0
 * with 5) whose list is non-empty, else -1. With `packed` negative: the free place nearest to
 * (1, 3) on rows 0-2 or to (2, 5) below (distance under 14). With level bits set: the last free
 * preferred place above row 8, else -1. */
int TaLogic006_GetSummonPiece(int arg0, unsigned char arg1, struct DotrFreeBlock *arg2, int arg3)
{
    int best;
    int bestCost;
    int candidate;
    struct DotrFreeNibbles flags[12];
    int i;
    struct DotrFreeNibbles *p;
    unsigned char kind;

    (void)arg1; /* not read */
    if (arg2 == 0) {
        return -1;
    }
    p = flags;
    i = 0;
    do {
        p->low = p->high = 15;
        p++;
        i++;
    } while (i < 12);

    kind = arg2->kind >> 5;
    if (kind == 0) {
        if (arg3 >= 0) {
            int y;
            int x;
            int index;
            int originY;
            int originX;
            int origin;
            int cost;

            bestCost = 255;
            best = 65535;
            origin = SzDuel_GetUnitLoc(arg3 >> 8, arg3 & 255);
            index = 0;
            originY = origin >> 8;
            originX = origin & 255;
            do {
                candidate = Ta_GetSommonPiecePreference(arg0, index);
                if (candidate >= 0) {
                    x = candidate & 255;
                    y = candidate >> 8;
                    if (SzDuel_GetField(x, y) != 8 &&
                        (SzDuel_GetField(x, y) != 9 ||
                         func_0024fe40_free(arg2, x, y) != 1) &&
                        SzDuel_GetMapUnit(x, y) == 0) {
                        if (func_00278510_free(arg2, arg3, TaLogic_GetWarnHideLvl__0012b1b0(arg0),
                                         originX, originY, 0) > 0) {
                            cost = func_0027a130_piece(arg2, flags, x, y,
                                                originX, originY, arg3, 0,
                                                5, (unsigned short)TaLogic_GetWarnHideLvl__0012b1b0(arg0));
                        } else {
                            cost = func_002792c0_piece6(arg2, flags, x, y,
                                                originX, originY, arg3, 5);
                        }
                        if (func_0027afc0_free(flags) > 0 && cost < bestCost) {
                            bestCost = cost;
                            best = candidate;
                        }
                    }
                }
                index++;
            } while (index < 8);
            if (best != 65535) {
                return best;
            }
            best = -1;
            return best;
        }
        {
            int maxY;
            int maxX;
            int y;
            int x;
            int minY;
            int minX;
            int origin;

            bestCost = 14;
            best = -1;
            i = 0;
            do {
                candidate = Ta_GetSommonPiecePreference(arg0, i);
                if (candidate >= 0) {
                    x = candidate & 255;
                    y = candidate >> 8;
                    if (SzDuel_GetField(x, y) != 8 &&
                        (SzDuel_GetField(x, y) != 9 ||
                         func_0024fe40_free(arg2, x, y) != 1) &&
                        SzDuel_GetMapUnitID(x, y) < 0) {
                        if (y < 3) {
                            minY = y < 3 ? y : 3;
                            maxY = y > 3 ? y : 3;
                            minX = x < 1 ? x : 1;
                            maxX = x > 1 ? x : 1;
                            (void)maxX; /* keeps maxX's temp, so that y and x take s0/s1 as in the original */
                            origin = (maxX - minX) + (maxY - minY);
                        } else {
                            minY = y < 5 ? y : 5;
                            maxY = y > 5 ? y : 5;
                            minX = x < 2 ? x : 2;
                            maxX = x > 2 ? x : 2;
                            (void)maxX; /* keeps maxX's temp, so that y and x take s0/s1 as in the original */
                            origin = (maxX - minX) + (maxY - minY);
                        }
                        if (origin < bestCost && origin >= 0) {
                            best = candidate;
                            bestCost = origin;
                        }
                    }
                }
                i++;
            } while (i < 8);
            if (bestCost < 14) {
                return best;
            }
        }
    } else {
        int y;
        int x;
        int index;
        int last;
        int candidate;

        last = -1;
        index = 0;
        do {
            candidate = Ta_GetSommonPiecePreference(arg0, index);
            if (candidate >= 0) {
                x = candidate & 255;
                y = candidate >> 8;
                if (SzDuel_GetField(x, y) != 8 &&
                    (SzDuel_GetField(x, y) != 9 ||
                     func_0024fe40_free(arg2, x, y) != 1) &&
                    SzDuel_GetMapUnitID(x, y) < 0 && y < 8) {
                    last = candidate;
                }
            }
            index++;
        } while (index < 8);
        if (last >= 0) {
            return last;
        }
    }
    return -1;
}

