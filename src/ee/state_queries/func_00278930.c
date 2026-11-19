#include "dotr/ee/state_queries.h"

/* Initializes a 24-byte node: id, five bytes of 255, four null links, then
 * either `handIndex` in the first byte or a copy of `source` with `handIndex`
 * written at its first free position. */
void TaLogic_InitTree(struct DotrStateNode *node, unsigned short id, unsigned char *source, int handIndex)
{
    int i;
    node->result = id;
    for (i = 0; i < 5; i++) {
        node->combo[i] = 255;
    }
    for (i = 0; i < 4; i++) {
        node->children[i] = 0;
    }
    if (source == 0) {
        node->combo[0] = handIndex;
    } else {
        TaLogic_CopyArray(source, node->combo);
        node->combo[(unsigned char)TaLogic_GetArrayLength(source)] = handIndex;
    }
}
