#include "dotr/ee/shslot.h"

/* ShSlot_IsFlashStop (0x2817c0): returns 0 while any of the five line timers or the nine
 * cell timers of the flash state at 0x351210 is above 1, else 1.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
int ShSlot_IsFlashStop(void)
{
    int i;
    int j;

    for (i = 0; i < 5; i++) {
        if (data_00351210_shslot.line_timer[i] > 1) {
            return 0;
        }
    }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            if (data_00351210_shslot.cell_timer[j][i] > 1) {
                return 0;
            }
        }
    }
    return 1;
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif
