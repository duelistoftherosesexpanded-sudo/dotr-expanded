#include "dotr/ee/small_misc.h"

/* Stores func_001948a8(word +8, word +0xc) at 0x34fb00 and
 * func_00194248(1, &word +0x20, &word +0x24) at +4. When that is 1 and the
 * word at +0x20 is 16: returns 13 for a non-zero word at +0x24, otherwise
 * sets the halfword at +0x30 to 14, clears those at +0x40 and +0x34 and
 * returns 12. Returns 0 in every other case. */
int TakuSave_card_format(void)
{
    struct DotrMiscLink *g = &data_0034fb00_view;
    data_0034fb00.unknown_00 = func_001948a8(data_0034fb00.unknown_08, data_0034fb00.unknown_0c);
    data_0034fb00.unknown_04 = func_00194248(1, &data_0034fb00.unknown_20, &data_0034fb00.unknown_24);
    if (data_0034fb00.unknown_04 == 1) {
        if (g->unknown_20 == 16) {
            if (g->unknown_24 == 0) {
                g->unknown_30 = 14;
                g->unknown_40 = 0;
                g->unknown_34 = 0;
                return 12;
            }
            return 13;
        }
    }
    return 0;
}
