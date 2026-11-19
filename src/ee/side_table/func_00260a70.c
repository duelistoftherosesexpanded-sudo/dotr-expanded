#include "dotr/ee/preset_words.h"

/* After SzView_FreeRequest(0, 0), submits the four words at +0xe0 with kind 3
 * and those at +0xf0 with kind 4, both tagged b, then clears the halfword
 * at +0x102. */
void TaView_RestoreOriginalView(struct DotrPresetPair *p, unsigned char b)
{
    SzView_FreeRequest(0, 0);
    func_001fa8a0_tag(0, 0, 3, b, 0, p->unknown_0e0);
    func_001fa8a0_tag(0, 0, 4, b, 0, p->unknown_0f0);
    p->unknown_102 = 0;
}
