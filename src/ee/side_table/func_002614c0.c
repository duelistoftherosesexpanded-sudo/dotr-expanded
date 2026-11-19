#include "dotr/ee/preset_words.h"

/* After SzView_FreeRequest(0, 0), builds the vector of the packed cell through
 * SzDuel_CalcUnitPos(low byte, upper bits, 0), converts value through
 * Ta_GetStdFocusView and SzView_IV2V, and submits the vector with kind 3 and
 * the converted floats with kind 4, both tagged b. */
void TaView_ReqFocusCamera2(unsigned char b, int cell, int value)
{
    struct DotrPresetVector v;
    int words[4];
    float out[4];
    SzView_FreeRequest(0, 0);
    SzDuel_CalcUnitPos(cell & 0xff, cell >> 8, 0, &v);
    Ta_GetStdFocusView(words, value);
    SzView_IV2V(words, out);
    func_001fa8a0_tag(0, 0, 3, b, 0, &v);
    func_001fa8a0_tag(0, 0, 4, b, 0, out);
}
