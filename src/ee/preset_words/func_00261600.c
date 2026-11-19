#include "dotr/ee/preset_words.h"

/* 0x261580 with the 0x256050 preset. */
void TaView_ReqSkyView(void)
{
    struct DotrPresetVector position;
    int words[4];
    float converted[4];
    SzView_FreeRequest(0, 0);
    SzDuel_CalcUnitPos(3, 3, 1000, &position);
    Ta_GetSkyView(words);
    SzView_IV2V(words, converted);
    SzView_ReqCamera(0, 0, 3, 0, 0, &position);
    SzView_ReqCamera(0, 0, 4, 0, 0, converted);
}
