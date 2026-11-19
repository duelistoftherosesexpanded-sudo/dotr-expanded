#include "dotr/ee/small_misc.h"

static inline int distance(int value)
{
    if (value >= 0) {
        return value;
    }
    return -value;
}

/* Walks the links at +0x34 for at most remainingSteps objects. The byte at +0 packs
 * a column (low nibble) and a row (high nibble). Returns 1 when an object
 * sits on (col, row), or, for (col, row) and (col2, row2) two steps apart, between them
 * or on one of the two corner cells of a diagonal step; 0 when the chain or
 * the depth runs out. */
int TaLogic_IsRootPast2(struct DotrMiscChain *tree, int col, int row, int col2, int row2, unsigned char remainingSteps)
{
    int minRow;
    int maxRow;
    int minCol;
    int maxCol;

    if (remainingSteps <= 0) {
        return 0;
    }
    if (tree == 0) {
        return 0;
    }
    if (col == (tree->loc & 0xf) && row == tree->loc >> 4) {
        return 1;
    }
    minRow = row < row2 ? row : row2;
    maxRow = row > row2 ? row : row2;
    minCol = col < col2 ? col : col2;
    maxCol = col > col2 ? col : col2;
    /* Two steps apart: is the tree on a cell between the two? */
    if ((maxCol - minCol) + (maxRow - minRow) == 2) {
        switch (distance(col - col2)) {
        case 0:
            if (col == (tree->loc & 0xf) && tree->loc >> 4 == (row + row2) >> 1) {
                return 1;
            }
            break;
        case 1:
            if (col2 == (tree->loc & 0xf) || row2 == tree->loc >> 4) {
                return 1;
            }
            break;
        case 2:
            if ((tree->loc & 0xf) == (col + col2) >> 1 && row == tree->loc >> 4) {
                return 1;
            }
            break;
        }
    }
    return TaLogic_IsRootPast2(tree->parent, col, row, col2, row2, remainingSteps - 1);
}
