#include "dotr/ee/pick_copies.h"

int TaLogic_SetComboQue__0011ef60(int side, unsigned char *minTask, struct DotrPickBlock *unit)
{
    struct DotrPickNode *nodes[5];
    int ids[5];
    short first;
    short second;
    int i;
    int id;
    int best;
    int score;
    int top;

    if (unit == 0) {
        return -1;
    }
    TaHand_GetNextHand(side, ids);
    best = -1;
    top = best;
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            second = 0;
            first = 0;
            id = ids[i];
            nodes[i] = 0;
            if ((TaHand_GetCardKind(id) >> 5) == 0 && id != 342) {
                nodes[i] = TaLogic_GetFusionTree(side, nodes[i], i, id, 0, &first, &second);
                nodes[i] = TaLogic_GetFusionRes(nodes[i]);
                score = TaLogic_GetEnchantQue(side, nodes[i], 0, 0);
                if (TaHand_IsCardSummon2(side, id) == 1) {
                    if (best < score) {
                        TaLogic_ConvEmptyCombo__0011f150(side, (unsigned short *)nodes[i], ids);
                        TaLogic_CopyArray(nodes[i]->slots, minTask + 6);
                        SzDuel_InitCardInfo((struct DotrStateBlock *)unit, nodes[i]->id);
                        unit->apBoost = (score >> 16) - (unit->target->apWithFlags & 0x1fff);
                        unit->dpBoost = (score & 0xffff) - (unit->target->dpWithFlags & 0x1fff);
                        best = score;
                        if (top < score) {
                            top = score;
                        }
                    }
                } else if (top < score) {
                    top = score;
                }
            }
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return top;
}
