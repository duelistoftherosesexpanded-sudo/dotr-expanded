#include "dotr/ee/preset_words.h"

/* Fetches the vector for (3, 3, 1000), converts the 0x255fe0 preset through
 * 0x1fc950, and submits both through 0x1fa8a0 as kinds 3 and 4. */
void TaView_ReqTopView(void)
{
    struct DotrPresetVector position;
    int words[4];
    float converted[4];
    SzView_FreeRequest(0, 0);
    SzDuel_CalcUnitPos(3, 3, 1000, &position);
    Ta_GetTopView(words);
    SzView_IV2V(words, converted);
    SzView_ReqCamera(0, 0, 3, 0, 0, &position);
    SzView_ReqCamera(0, 0, 4, 0, 0, converted);
}
