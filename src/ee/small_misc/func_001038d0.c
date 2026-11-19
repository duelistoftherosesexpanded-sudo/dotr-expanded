#include "dotr/ee/small_misc.h"

/* Same body as Ta_LkSetCoordinate (0x27f110): resets the 64-byte member at
 * +0x30, builds a 64-byte local from the object and its members at +0x10
 * and +0x20, and combines the two. */
void LkSetCoordinate(struct DotrMiscFrame *frame)
{
    struct DotrMiscMatrix local;

    RhView_GetWorldScreenMatrix(&frame->unknown_30);
    RhGeom_GetScaleRotXYZTransMatrix(&local, frame, frame->unknown_10, frame->unknown_20);
    RhGeom_MulMatrix(&frame->unknown_30, &local);
}
