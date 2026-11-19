#include "dotr/ee/small_misc.h"

/* As TakuSave_card_format with func_00194e90 and kind 17; here both outcomes set
 * the halfword at +0x30 to 14 and clear those at +0x40 and +0x34 (12 for a
 * zero word at +0x24, else 13). */
int TakuSave_card_unformat(void)
{
    struct DotrMiscLink *g = &data_0034fb00_view;
    data_0034fb00.unknown_00 = func_00194e90(data_0034fb00.unknown_08, data_0034fb00.unknown_0c);
    data_0034fb00.unknown_04 = func_00194248(1, &data_0034fb00.unknown_20, &data_0034fb00.unknown_24);
    if (data_0034fb00.unknown_04 == 1) {
        if (g->unknown_20 == 17) {
            if (g->unknown_24 == 0) {
                g->unknown_30 = 14;
                g->unknown_40 = 0;
                g->unknown_34 = 0;
                return 12;
            }
            g->unknown_30 = 14;
            g->unknown_40 = 0;
            g->unknown_34 = 0;
            return 13;
        }
    }
    return 0;
}
