#include "dotr/ee/logic017_summon.h"

/* MATCHING DEVICE: the inline helper with its ternary test and second GetLine call, the
 * macros writing the distance twice, the declaration order of the locals (it decides the
 * registers) and the ending that stores -1 again before returning it. */
#define MAXI(a, b) ((a) < (b) ? (b) : (a))
#define MINI(a, b) ((b) < (a) ? (b) : (a))

static inline int empty_line(int c)
{
    int i;
    for (i = 0; i < 7; i++) {
        int pos = TaLogic017_GetLine(i, c);
        if ((SzDuel_GetMapUnit(pos & 0xFF, pos >> 8) == 0 ? 1 : 0) == 1) {
            return TaLogic017_GetLine(i, c);
        }
    }
    return -1;
}

int TaLogic017_GetSummonPiece(int side, int unused, struct DotrLogic017SummonBlock *block)
{
    int loc;
    int row;
    int i;
    int best;
    int spot;
    int ty;
    int best_spot;
    unsigned char far;
    int tx;
    int y;
    int x;
    int pos;

    (void)unused;
    loc = SzDuel_GetUnitLoc(side, 0);
    if (block == 0) {
        return -1;
    }
    far = block->unknown_3e >> 5;
    if (TaLogic017_IsExzParts(block->unknown_3c) == 1) {
        if (loc == 0x403) {
            return func_00156ea0_summon(side);
        }
        return -1;
    }
    if (block->unknown_3c == 821 || block->unknown_3c == 824) {
        if (loc == 0x403) {
            return func_00156f70_summon(side);
        }
        return -1;
    }
    best = 7;
    best_spot = -1;
    row = SzDuel_GetUnitLoc(side, 0) >> 8;
    pos = empty_line(far);
    if (pos < 0) {
        if (far != 0) {
            pos = empty_line(0);
            if (pos < 0) {
                pos = 0x503;
            }
        } else {
            pos = empty_line(far + 1);
            if (pos < 0) {
                pos = 0x503;
            }
        }
    }
    for (i = 0, ty = pos >> 8, tx = pos & 0xff; i < 8; i++) {
        spot = Ta_GetSommonPiecePreference(side, i);
        if (spot >= 0) {
            x = spot & 0xff;
            y = spot >> 8;
            if (SzDuel_GetField(x, y) != 8 && SzDuel_GetField(x, y) != 9 && SzDuel_GetMapUnitID(x, y) < 0) {
                if ((MAXI(tx, x) - MINI(tx, x)) + (MAXI(ty, y) - MINI(ty, y)) <= best && y >= row) {
                    best_spot = spot;
                    best = (MAXI(tx, x) - MINI(tx, x)) + (MAXI(ty, y) - MINI(ty, y));
                    row = y;
                }
            }
        }
    }
    if (best_spot >= 0) {
        return best_spot;
    }
    best_spot = -1;
    return best_spot;
}
