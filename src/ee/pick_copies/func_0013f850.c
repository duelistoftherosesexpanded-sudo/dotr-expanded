#include "dotr/ee/pick_copies.h"

int TaLogic011_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block)
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

    if (block == 0) {
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
            if ((TaHand_GetCardKind(id) >> 5) == 0) {
                if (Taku_Get_WinCount() < 3) {
                    nodes[i] = func_00278c80_pick(side, nodes[i], i, id, 0, &first, &second);
                } else {
                    nodes[i] = TaLogic_GetFusionTree(side, nodes[i], i, id, 0, &first, &second);
                }
                nodes[i] = TaLogic_GetFusionRes(nodes[i]);
                score = TaLogic_GetEnchantQue(side, nodes[i], 0, 0);
                if (TaHand_IsCardSummon2(side, id) == 1) {
                    if (best < score) {
                        if (3 < Taku_Get_WinCount()) {
                            TaLogic_ConvEmptyCombo__0013fa90(side, (unsigned short *)nodes[i], ids);
                        }
                        TaLogic_CopyArray(nodes[i]->slots, item + 6);
                        SzDuel_InitCardInfo((struct DotrStateBlock *)block, nodes[i]->id);
                        block->apBoost = (score >> 16) - (block->target->apWithFlags & 0x1fff);
                        block->dpBoost = (score & 0xffff) - (block->target->dpWithFlags & 0x1fff);
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
