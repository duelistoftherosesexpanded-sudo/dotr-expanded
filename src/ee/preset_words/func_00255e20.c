#include "dotr/ee/preset_words.h"

void Ta_GetFlatCompleteView(int *out)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = 500;
        out[1] = 8192;
        out[2] = 1024;
        out[3] = 0;
    } else {
        out[0] = 500;
        out[1] = 0xa000;
        out[2] = 1024;
        out[3] = 0;
    }
}
