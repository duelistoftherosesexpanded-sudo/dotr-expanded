#include "dotr/ee/preset_words.h"

void Ta_GetStdFocusView2(int *out, int value)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = value;
        out[1] = 20479;
        out[2] = 6144;
        out[3] = 0;
    } else {
        out[0] = value;
        out[1] = 0xcfff;
        out[2] = 6144;
        out[3] = 0;
    }
}
