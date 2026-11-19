#include "dotr/ee/preset_words.h"

void Ta_GetStdFocusView(int *out, int value)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = value;
        out[1] = 16383;
        out[2] = 4096;
        out[3] = 0;
    } else {
        out[0] = value;
        out[1] = 0xbfff;
        out[2] = 4096;
        out[3] = 0;
    }
}
