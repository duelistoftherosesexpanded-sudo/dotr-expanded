#include "dotr/ee/small_misc.h"

void TaLogic_SetRootTreeDat(unsigned char *currentTree, int parentTree, int col, int row)
{
    int i;
    currentTree[0] = (row << 4) | col;
    for (i = 0; i < 12; i++) {
        ((int *)currentTree)[1 + i] = 0;
    }
    ((int *)currentTree)[13] = parentTree;
}
