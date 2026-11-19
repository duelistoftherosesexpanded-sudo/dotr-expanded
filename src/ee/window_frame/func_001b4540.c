#include "dotr/ee/window_frame.h"

/* lSzDuel_DrawWindow_Emblem_Pipe (0x1b4540): for a count above 0 draws the emblem (tile row
 * 0xc6 + (count - 1) / 3) at (x + 0x6c0, y + 0x720) with lSzDuel_DrawWindow_ExIcon_Pipe, then
 * by (count - 1) % 3 + 1 one mark (bit 0) and two marks (bit 1) of the row at 0x290864, offset
 * by 0xc or 0x10 (shrink flag) and mirrored by the last argument. Returns the cursor.
 * Compiled under #pragma opt_propagation off (reset after). */
#ifdef __MWERKS__
#pragma opt_propagation off
#endif
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    int result;

    result = arg0;
    if (arg3 > 0) {
        int parts;
        int px;
        int py;
        int width;
        int quotient;
        unsigned short *src;

        parts = arg4 ? 0xC : 0x10;
        if (arg3 > 0) {
            width = ((arg3 - 1) % 3) + 1;
        } else {
            width = 0;
        }
        quotient = 0;
        if (arg3 > 0) {
            quotient = (arg3 - 1) / 3;
        }
        src = (unsigned short *)(gSzDuel_Icon_emblem + (quotient + 0xC6) * 10);
        result = lSzDuel_DrawWindow_ExIcon_Pipe(arg0, src, arg1 + 0x6C0, arg2 + 0x720,
                               0, 1, 0, arg4, (arg5 != 0) ? 1 : 0, 0, 1, 0, -1, 0x80);
        if (width & 1) {
            px = arg1 + 0x6C0 + ((arg5 != 0) ? 0 : parts);
            result = lSzDuel_DrawWindow_ExIcon_Pipe(result, data_00290864_emblem, px, arg2 + 0x720,
                                   0, 1, 0, arg4, 0, 0, 1, 0, -1, 0x80);
        }
        if (width & 2) {
            px = arg1 + 0x6C0 + ((arg5 != 0) ? parts : 0);
            result = lSzDuel_DrawWindow_ExIcon_Pipe(result, data_00290864_emblem, px, arg2 + 0x720, 0, 1, 0, arg4, 0, 0, 1, 0, -1, 0x80);
            px = arg1 + 0x6C0 + ((arg5 != 0) ? 0 : parts);
            py = arg2 + 0x720 + parts;
            result = lSzDuel_DrawWindow_ExIcon_Pipe(result, data_00290864_emblem, px, py, 0, 1, 0, arg4, 0, 0, 1, 0, -1, 0x80);
        }
    }
    return result;
}
#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
