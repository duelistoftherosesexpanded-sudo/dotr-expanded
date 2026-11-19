#include "dotr/ee/object_table.h"

void SzModel_CancelMotionBlur(int value)
{
    int i;
    for (i = 1; i < 32; i++) {
        int current = gSzModel_aBlur[i].unknown_68_bits_0_26;
        if (current >= 0) {
            if (value < 0 || current == value) {
                gSzModel_aBlur[i].unknown_68_bits_0_26 = -1;
            }
        }
    }
}
