#include "dotr/ee/logic_choices.h"

/* TaLogic011_IsUseLifeHourGlass (0x142040): 1 when the side's life points are at least 1200,
 * at least three graveyard monsters of both sides with attack above 2000 and kind bits 0, at
 * least three of the eight summon orders answer a place, and at most two own monsters are on
 * the board; else 0.
 * Compiled with #pragma opt_lifetimes off (docs/DECOMPILATION.md). */

#ifdef __MWERKS__
#pragma opt_lifetimes off
#endif

int TaLogic011_IsUseLifeHourGlass(int arg0)
{
    unsigned char slots;
    unsigned int side;
    int i;
    unsigned char count;

    side = arg0;
    count = 0;
    slots = 0;
    if (SzDuel_GetLP(side) < 0x4B0) {
        return 0;
    }
    for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
        if ((int)(TaHand_GetCardAPDP(SzDuel_InqGrave(side, i)) >> 16) > 2000) {
            if ((TaHand_GetCardKind(SzDuel_InqGrave(side, i)) >> 5) == 0) {
                count++;
            }
        }
    }
    for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
        if ((int)(TaHand_GetCardAPDP(SzDuel_InqGrave(side == 0 ? 1 : 0, i)) >> 16) > 2000) {
            if ((TaHand_GetCardKind(SzDuel_InqGrave(side == 0 ? 1 : 0, i)) >> 5) == 0) {
                count++;
            }
        }
    }
    if (count >= 3) {
        for (i = 0; i < 8; i++) {
            if (Ta_GetSummonablePieceWithOrder(side, i, 1) >= 0) {
                slots++;
            }
        }
        if (slots >= 3 && 5 - SzDuel_GetUnitMonsterCount(side) >= 3) {
            return 1;
        }
    }
    return 0;
}

#ifdef __MWERKS__
#pragma opt_lifetimes reset
#endif
