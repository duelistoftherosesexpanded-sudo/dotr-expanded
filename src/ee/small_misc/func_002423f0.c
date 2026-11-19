#include "dotr/ee/small_misc.h"

/* Calls func_001854e0(0) until its result differs from a, then clears the
 * word at 0x3505f8, sets the word at 0x350340 to 1 and clears 0x350348. */
void startDisplay(int a)
{
    do {
    } while (func_001854e0(0) == a);
    frd[0] = 0;
    isCountVblank[0] = 1;
    isCountVblank[2] = 0;
}
