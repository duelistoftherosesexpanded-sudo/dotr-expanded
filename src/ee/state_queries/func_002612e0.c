#include "dotr/ee/state_queries.h"

/* Adds `add` to the first word produced by 0x1f6160, the wrapped distance
 * from 0x47ff to the item word at +0x30 to the second, and 8192 to the third.
 * The loaded word needs its own local declared before the difference. */
void TaView_SetModelFrontView(int *out, int packed, int add)
{
    struct DotrStateItem *item = SzDuel_GetUnit(packed >> 8, packed & 0xff);
    int word;
    int difference;
    func_001f6160_words(packed, out);
    out[0] += add;
    word = item->unknown_30;
    difference = 0x47ff - word;
    if (difference < 0) {
        difference = 0x47ff + 0xffff - word;
    }
    out[1] += difference;
    out[2] += 8192;
}
