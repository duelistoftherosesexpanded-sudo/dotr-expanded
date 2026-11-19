#include "dotr/ee/state_queries.h"

/* As TaView_SetModelFrontView (0x2612e0), with 0xc7ff in place of 0x47ff. */
void TaView_SetModelBackView(int *out, int packed, int add)
{
    struct DotrStateItem *item = SzDuel_GetUnit(packed >> 8, packed & 0xff);
    int word;
    int difference;
    func_001f6160_words(packed, out);
    out[0] += add;
    word = item->unknown_30;
    difference = 0xc7ff - word;
    if (difference < 0) {
        difference = 0xc7ff + 0xffff - word;
    }
    out[1] += difference;
    out[2] += 8192;
}
