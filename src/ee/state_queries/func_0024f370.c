#include "dotr/ee/state_queries.h"

/* Removes unit (side, pos): adds -4 to its card's experience (SzData_CalcExp
 * operation 0), sends the card to the side's graveyard (SzDuel_AddGrave) and
 * deletes the unit (SzDuel_DelUnit). When effect slot 4 exists and its first
 * word is 81 above the low byte, the card packed in the second word (side in
 * the top byte, card in the low halfword) also goes to that side's graveyard
 * with the same experience change. Returns 0. The assignment inside the test
 * reproduces the branch delay slot. */
int Ta_DelUnit(int side, int pos)
{
    struct DotrStateEntry8 *effect;
    SzData_CalcExp(side, SzDuel_GetUnitCardNo(side, pos), 0, -4);
    SzDuel_AddGrave(side, SzDuel_GetUnitCardNo(side, pos));
    SzDuel_DelUnit(side, pos);
    if ((effect = Ta_GetUnitEffect(side, pos, 4)) != 0 && (unsigned)effect->unknown_words[0] >> 8 == 81) {
        SzDuel_AddGrave((unsigned)effect->unknown_words[1] >> 24, effect->unknown_words[1] & 0xffff);
        SzData_CalcExp((unsigned)effect->unknown_words[1] >> 24, effect->unknown_words[1] & 0xffff, 0, -4);
    }
    return 0;
}
