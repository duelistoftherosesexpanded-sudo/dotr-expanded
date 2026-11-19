#include "dotr/ee/cursor_packets.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* SzMsg_DrawRatioX_Pipe (0x1fe660). Compiled warm: the two float-to-int conversions are written
 * as casts instead of calls to 0x1a22c0 (see docs/DECOMPILATION.md, build entries). */
DotrDrawWord SzMsg_DrawRatioX_Pipe(DotrDrawWord cursor, int index, int x, int y, float sx, float sy, int e,
                                   DotrCursorRatioDraw function)
{
    struct DotrCursorSlotEntry *entry = SzMsg_GetInfo(index);
    struct DotrCursorRatioRect rect = gSzMsg_defRect_rect;

    if (entry == 0) {
        return cursor;
    }
    rect.width = (int)((float)entry->unknown_004[2] * sx);
    rect.height = (int)((float)entry->unknown_004[3] * sy);
    return function(cursor, index, x, y, rect, e);
}
