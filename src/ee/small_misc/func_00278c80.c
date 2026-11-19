#include "dotr/ee/small_misc.h"

/* The second argument is not read, and the 32 bytes filled by TaHand_GetNextHand are not
 * used afterwards. The record assignment sits inside its null test. */
struct DotrMiscRecord24 *TaLogic_GetFusionTree2(int a, int unused, int c, int d, int e,
                                       unsigned short *used, unsigned short *count)
{
    struct DotrMiscScratch32 scratch;
    struct DotrMiscRecord24 *record;
    (void)unused;
    TaHand_GetNextHand(a, &scratch);
    if (e != 0) {
        return 0;
    }
    if ((record = TaLogic_TreeAloc(used, count)) == 0) {
        return 0;
    }
    TaLogic_InitTree(record, d, e, c);
    return record;
}
