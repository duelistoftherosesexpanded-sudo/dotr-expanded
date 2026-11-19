#include "dotr/ee/small_misc.h"

int TaLogic_IsUseHandInCombo(unsigned char handSlot, unsigned char *combo)
{
    int i;
    for (i = 0; i < 5; i++) {
        if (handSlot == combo[i]) {
            return 1;
        }
    }
    return 0;
}
