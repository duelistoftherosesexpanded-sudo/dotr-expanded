#include "dotr/ee/preset_words.h"

void TaView_ReqStdCompleteView(unsigned char flag)
{
    struct DotrPresetVector position;
    int words[4];
    float converted[4];

    SzView_FreeRequest(0, 0);
    SzDuel_CalcUnitPos(3, 3, 0, &position);
    Ta_GetStdCompleteView(words);
    SzView_IV2V(words, converted);
    func_001fa8a0_tag(0, 0, 3, flag, 0, &position);
    func_001fa8a0_tag(0, 0, 4, flag, 0, converted);
}
