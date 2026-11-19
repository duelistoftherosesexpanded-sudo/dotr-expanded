#include "dotr/ee/preset_words.h"

void Ta_GetTopView(int *out)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = 250;
        out[1] = 8192;
        out[2] = -6144;
        out[3] = 0;
    } else {
        out[0] = 250;
        out[1] = 0xa000;
        out[2] = -6144;
        out[3] = 0;
    }
}
