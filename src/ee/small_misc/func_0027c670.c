#include "dotr/ee/small_misc.h"

/* Counts the halfwords below 0xffff among the first 22. The read index is
 * separate from the loop counter and is tested on its own. */
int TaLogic_GetUnitCtlLstLen(unsigned short *units)
{
    int i;
    int length = 0;
    int j = 0;
    for (i = 0; i < 22; i++) {
        if (j >= 22) {
            return length;
        }
        if (units[j++] < 0xffff) {
            length++;
        }
    }
    return length;
}
