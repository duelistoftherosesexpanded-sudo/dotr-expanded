#include "dotr/ee/small_misc.h"

/* Follows the links at +0x34: returns the first byte of the object whose
 * successor has no successor, or 255 when the object has no successor or
 * depth reaches 7. */
int TaLogic_GetNextPiece(struct DotrMiscChain *tree, unsigned char depth)
{
    struct DotrMiscChain *next;
    if (depth >= 7) {
        return 255;
    }
    next = tree->parent;
    if (next == 0) {
        return 255;
    }
    if (next->parent == 0) {
        return tree->loc;
    }
    return TaLogic_GetNextPiece(next, depth + 1);
}
