#include "dotr/ee/small_misc.h"

void SzView_TermCamera(void)
{
    if (gSzView_pCamera[0]) {
        func_00217b50_word(gSzView_pCamera[0]);
        gSzView_pCamera[0] = 0;
    }
}
