#include "dotr/ee/order_tries.h"

/* TaLogic013_CommandInvadeUnit (0x147830): the flagless order try. Nothing without a unit; with
 * an open root to the other leader, 0x2ff toward it; else up to five TaLogic_GetNearUnit tries
 * (answer 3: 0x6ff, 2 and 0: 0x3ff, each at the unit's position; 1: nothing stored); after
 * five, 0x6ff to the TaLogic013_GetInvadePoint answer.
 * Compiled with #pragma opt_dead_assignments off, set directly before the function and reset
 * after it (docs/DECOMPILATION.md). */

static inline int other_00147830(int side)
{
    return side == 0 ? 1 : 0;
}

#ifdef __MWERKS__
#pragma opt_dead_assignments off
#endif

void TaLogic013_CommandInvadeUnit(struct DotrTryOrder *order)
{
    struct DotrTryInvadeOrder *entry = (struct DotrTryInvadeOrder *)order;
    int side;
    int player;
    int pos;
    int other;
    int playerLeaderLoc;
    int i;
    int selection;
    unsigned char ignoreList[0x70];
    struct DotrTryCell list[22];
    short countA;
    short countB;

    if (func_001df540_word(entry->coordinates >> 8, entry->coordinates & 0xFF) != 0) {
        side = func_0024de70_pair(entry->coordinates >> 8, entry->coordinates & 0xFF);
        other = other_00147830(side) << 8;
        pos = SzDuel_GetUnitLoc(entry->coordinates >> 8, entry->coordinates & 0xFF);
        playerLeaderLoc = SzDuel_GetUnitLoc(other >> 8, other & 0xFF);
        player = func_001df540_word(entry->coordinates >> 8, entry->coordinates & 0xFF);
        func_0027e770_buf(ignoreList);
        for (i = 0; i < 22; i++) {
            list[i].id = 0xFFFF;
            list[i].kind = 0xFF;
        }
        if (TaLogic_IsRootOpen(entry->coordinates >> 8, entry->coordinates & 0xFF, pos & 0xFF, pos >> 8,
                               other >> 8, other & 0xFF, playerLeaderLoc & 0xFF, playerLeaderLoc >> 8, 2) > 0) {
            entry->state = 0x2FF;
            entry->data.word = (other << 16) | entry->data.halves.low;
            return;
        }
        countB = 0;
        countA = 0;
        i = 1;
        while (1) {
            selection = TaLogic_GetWarnHideLvl__0014b330(side);
            switch (TaLogic_GetNearUnit(order, 0, player, pos & 0xFF, pos >> 8,
                                        other_00147830(func_0024de70_pair(entry->coordinates >> 8, entry->coordinates & 0xFF)),
                                        (unsigned char)i, selection, list, 0,
                                        (unsigned short *)&countA, (unsigned short *)&countB, 5, 0, 0x960, ignoreList)) {
            case 3:
                entry->state = 0x6FF;
                entry->data.word = (pos << 16) | entry->data.halves.low;
                return;
            case 2:
                entry->state = 0x3FF;
                entry->data.word = (pos << 16) | entry->data.halves.low;
                return;
            case 1:
                return;
            case 0:
                entry->state = 0x3FF;
                entry->data.word = (pos << 16) | entry->data.halves.low;
                return;
            default:
                break;
            }
            i++;
            if (i > 5) {
                break;
            }
        }
        entry->state = 0x6FF;
        entry->data.word = entry->data.halves.low | (TaLogic013_GetInvadePoint(side, pos & 0xFF, pos >> 8) << 16);
    }
}

#ifdef __MWERKS__
#pragma opt_dead_assignments reset
#endif
