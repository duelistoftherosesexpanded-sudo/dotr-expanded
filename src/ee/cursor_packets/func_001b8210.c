#include "dotr/ee/cursor_packets.h"

/* Returns the cursor unchanged while the control block's byte at 0xd3a is zero; otherwise draws
 * the full-screen rectangle (0, 0, 640, 448) through lSzDuel_Draw_Fade_Pipe with that byte as
 * alpha and returns its result. */
DotrDrawWord SzDuel_DrawJoinFade_Pipe(DotrDrawWord cursor)
{
    struct DotrCursorPicControl *control = &data_00317490;

    /* MATCHING DEVICE: the forward goto keeps the early return as its own block (bnez over
     * `b end; move v0,a0`); an if/else or a single return coalesces the result into a0. */
    if (data_00317490.join_fade != 0) goto draw;
    return cursor;
draw:
    return lSzDuel_Draw_Fade_Pipe(cursor, 0, 0, 640, 448, control->join_fade);
}
