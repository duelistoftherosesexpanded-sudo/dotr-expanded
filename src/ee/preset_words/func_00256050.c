#include "dotr/ee/preset_words.h"

void Ta_GetSkyView(int *out)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = 500;
        out[1] = 24576;
        out[2] = -2048;
        out[3] = 0;
    } else {
        out[0] = 500;
        out[1] = 0xe000;
        out[2] = -2048;
        out[3] = 0;
    }
}
