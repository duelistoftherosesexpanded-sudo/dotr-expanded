#include "dotr/ee/preset_words.h"

void Ta_GetStdTopDownView(int *out)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = 500;
        out[1] = 16383;
        out[2] = 16384;
        out[3] = 0;
    } else {
        out[0] = 700;
        out[1] = 0xbfff;
        out[2] = 16384;
        out[3] = 0;
    }
}
