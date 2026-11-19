#include "dotr/ee/kind_applies.h"

/* Prototype from the Japanese build's debug information. -1 for kind bytes
 * 2 and 3, and for kind 4 when the halfword table value is 9; otherwise 1. */
int TaEnc_IsEncDo(int eNO)
{
    switch (EnchantId[eNO]) {
    case 2: /* Cursebreaker */
    case 3: /* Paralyzing Potion */
        return -1;
    case 4: /* fusion equip */
        if (gAnmPat_Bakura[eNO] == 9) { /* Insect Imitation */
            return -1;
        }
        break;
    }
    return 1;
}
