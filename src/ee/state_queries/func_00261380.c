#include "dotr/ee/state_queries.h"

/* TaView_SetModelFrontView (0x2612e0) with 0x3fff and 4096. */
void TaView_SetExzModelFrontView(int *out, int packed, int add)
{
    struct DotrStateItem *item = SzDuel_GetUnit(packed >> 8, packed & 0xff);
    int word;
    int difference;
    func_001f6160_words(packed, out);
    out[0] += add;
    word = item->unknown_30;
    difference = 0x3fff - word;
    if (difference < 0) {
        difference = 0x3fff + 0xffff - word;
    }
    out[1] += difference;
    out[2] += 4096;
}
