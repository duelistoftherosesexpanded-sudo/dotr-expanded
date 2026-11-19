#include "dotr/ee/small_misc.h"

/* Calls 0x27acb0 with a zeroed word, a byte of 255, the first argument
 * repeated and a trailing zero. */
int TaLogic_GetShortRoot(int pathfindingBuffer, int tree, int col, int row)
{
    int word;
    unsigned char byte;
    byte = 255;
    word = 0;
    return TaLogic_DoGetShortRoot(pathfindingBuffer, &word, &byte, tree, col, row, pathfindingBuffer, 0);
}
