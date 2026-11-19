#include "dotr/ee/small_misc.h"

/* Inserts a byte at the front of a five-byte list unless TaLogic_GetArrayLength reports
 * five entries. */
void TaLogic_AddEmptyCombo(unsigned char *combo, int newCard)
{
    int i;
    if ((unsigned char)TaLogic_GetArrayLength(combo) != 5) {
        for (i = 3; i >= 0; i--) {
            combo[i + 1] = combo[i];
        }
        combo[0] = newCard;
    }
}
