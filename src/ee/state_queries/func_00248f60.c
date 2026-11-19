#include "dotr/ee/state_queries.h"

/* Reads bit (equipNo - 752) of row cardNo of the 7-byte rows at 0x29d580; when set
 * returns Ta_GetHandEnchant3(cardNo, equipNo), otherwise -1. */
int Ta_GetHandEnchant(int cardNo, int equipNo)
{
    if ((EnchMatch[cardNo * 7 + ((equipNo - 752) >> 3)] >> ((equipNo - 752) % 8)) & 1) {
        return Ta_GetHandEnchant3(cardNo, equipNo);
    }
    return -1;
}
