#include "dotr/ee/fx_calc.h"

void ShFx_BlackBG(struct DotrFxBlackSpan *span, int frame)
{
    int darken;
    int darken_step;
    int lighten;
    int lighten_step;

    /* MATCHING DEVICE: each step is masked to 16 bits twice through a second
     * int; an unsigned short local folds the two masks into one. */
    darken = ((frame - span->darken_frame) + 1) & 0xffff;
    if (darken != 0) {
        darken_step = darken & 0xffff;
        if (darken_step < 6) {
            SzEffect_SetBlackOutForBattle((0x80 - darken_step * 0x10) & 0xff);
        }
    }
    lighten = ((frame - span->lighten_frame) + 1) & 0xffff;
    if (lighten != 0) {
        lighten_step = lighten & 0xffff;
        if (lighten_step < 6) {
            SzEffect_SetBlackOutForBattle((lighten_step * 0x10 + 0x30) & 0xff);
        }
    }
}
