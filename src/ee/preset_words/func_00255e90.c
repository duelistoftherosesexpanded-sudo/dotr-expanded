#include "dotr/ee/preset_words.h"

void Ta_GetDestinyView(int *out)
{
    if (SzDuel_GetTurnOwner() == 0) {
        out[0] = 122;
        out[1] = 20480;
        out[2] = -4096;
        out[3] = 0;
    } else {
        out[0] = 122;
        out[1] = 0xd000;
        out[2] = -4096;
        out[3] = 0;
    }
}
