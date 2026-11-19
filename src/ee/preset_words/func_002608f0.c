#include "dotr/ee/preset_words.h"

void TaView_SetDestinyCamera(unsigned char flag)
{
    struct DotrPresetVector v;
    int words[4];
    float out[4];

    SzView_FreeRequest(0, 0);
    switch (flag) {
    case 0:
        SzDuel_CalcUnitPos(3, 3, 400, &v);
        Ta_GetDestinyView(words);
        SzView_IV2V(words, out);
        SzView_ReqCamera(0, 0, 3, 0, 0, &v);
        SzView_ReqCamera(0, 0, 4, 0, 0, out);
        break;
    }
}
