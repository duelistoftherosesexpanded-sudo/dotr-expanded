#include "dotr/ee/state_queries.h"

/* Returns the node of the tree whose id has the largest high half of
 * 0x26fbb0. The index is declared before the child pointer; other
 * declaration orders assign different saved registers. */
struct DotrStateNode *TaLogic_GetFusionRes(struct DotrStateNode *node)
{
    int i;
    struct DotrStateNode *child;
    unsigned short max;
    struct DotrStateNode *best = node;
    if (node != 0) {
        max = (unsigned)TaHand_GetCardAPDP(node->result) >> 16;
        for (i = 0; i < 4; i++) {
            child = TaLogic_GetFusionRes(node->children[i]);
            if (child != 0) {
                if (max < (unsigned)TaHand_GetCardAPDP(child->result) >> 16) {
                    max = (unsigned)TaHand_GetCardAPDP(child->result) >> 16;
                    best = child;
                }
            }
        }
    } else {
        return 0;
    }
    return best;
}
