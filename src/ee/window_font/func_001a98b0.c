#include "dotr/ee/window_font.h"

/* Sorts the window tile once for every cell from *first up to `end` (35 cells per row of
 * 18 x 24, rows from the ninth on 48 lower) and leaves the next cell number in *first.
 * MATCHING DEVICE: the two loops of SzGame_ModeDrawLast reproduce the original's code only
 * when written as copies of one static inline helper with these locals in this order;
 * whether the original had such a helper is unknown. */
static inline DotrDrawWord Cells(DotrDrawWord cursor, struct DotrModeLastPacket *packet, int *first, int end)
{
    int i;
    for (i = *first; i < end; i++) {
        int cy;
        int row;
        int cx;
        cx = i % 35 * 18 + 5;
        row = i / 35;
        cy = row * 24 + 4;
        if (row >= 8) {
            cy += 48;
        }
        packet->vertex[0].xyz.x = (cx + 1728) << 4;
        packet->vertex[0].xyz.y = (cy + 1824) << 4;
        packet->vertex[1].xyz.x = (cx + 1746) << 4;
        packet->vertex[1].xyz.y = (cy + 1848) << 4;
        cursor = ExSortPrim_Pipe(cursor, packet, 0, 0);
    }
    *first = i;
    return cursor;
}

/* SzGame_ModeDrawLast: draws the last game mode's screen. Does nothing unless the mode
 * field is 3 or the fade byte is set, and unless the enable bit is set. Inside a window
 * session (default priority 0xffffff) with window texture 6: the frame counter selects one
 * of five 18 x 24 texture tiles (counter / 560 % 5) and how many of the 560 screen cells
 * (counter % 560) show it; once the counter has passed 560 the remaining cells show the
 * previous tile. The tile alpha is the fade byte, or the counter capped at 128. The text at
 * 0x2be815 is drawn centred at (320, 212) in font 2 with the colour at 0x28ff20 and an
 * alpha that swings by the signed step * 4 per frame, the step being negated when the
 * alpha leaves 1..127. The fade byte drops by 4 per frame while set and the counter
 * advances. The session is finished and passed through ExSortSoftFocus_Pipe to
 * RhBase_SetVifPacketArea whether or not the texture was found. */
void SzGame_ModeDrawLast(void)
{
    struct DotrModeLastView *view = &gSzGame_gl_lastview;
    struct DotrModeLastColour colour;
    int page;
    unsigned int count;
    struct DotrModeLastPacket packet;
    int n;
    int *texture;
    int x;
    int a;
    int y;
    int i;
    DotrDrawWord cursor;
    const char *text;
    unsigned char alpha;

    SzGame_GetMode(1);
    if (gSzGame_gl_last.mode < 3 && view->busy == 0) {
        return;
    }
    if (view->enabled) {
        cursor = SzDuel_StartWindowSessionWithPalette_Pipe(RhBase_GetVifPacketArea(), 0);
        SzDuel_SetWindowDefPrioZ(0xffffff);
        texture = SzDuel_GetWindowTexture(6);
        if (texture) {
            count = view->counter;
            page = count / 560 % 5;
            n = count % 560;
            x = (page < 3 ? 200 : 80) + page % 3 * 18;
            y = page < 3 ? 136 : 128;
            text = data_002be815;
            alpha = view->busy ? view->busy : (count < 128 ? count : 128);
            colour = data_0028ff20;
            a = view->alpha + 64;
            a = a < 128 ? a : 128;
            colour.b[3] = (alpha * a) >> 7;
            ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 38;
            ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 5;
            ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x42421;
            ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
            ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 |= 0x800;
            ExAssignTexture((struct DotrCursorPacketHeader *)packet.header, texture, 0);
            ExAssignAlpha((struct DotrCursorPacketHeader *)packet.header, ExGetAlphaVal(0));
            ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = (x << 4) + 8;
            ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = (y << 4) + 8;
            ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = ((x + 18) << 4) + 8;
            ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = ((y + 24) << 4) + 8;
            packet.colour[0] = 128;
            packet.colour[1] = 128;
            packet.colour[2] = 128;
            packet.colour[3] = alpha;
            packet.unknown_14 = 1.0f;
            packet.vertex[0].xyz.z = 0xffffff;
            packet.vertex[0].xyz.f = 0;
            packet.vertex[1].xyz.z = 0xffffff;
            packet.vertex[1].xyz.f = 0;
            i = 0;
            cursor = Cells(cursor, &packet, &i, n);
            if ((unsigned int)n < view->counter) {
                page = (page + 4) % 5;
                x = (page < 3 ? 200 : 80) + page % 3 * 18;
                y = page < 3 ? 136 : 128;
                ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->u = (x << 4) + 8;
                ((struct DotrCursorSlotUv *)&packet.vertex[0].uv)->v = (y << 4) + 8;
                ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->u = ((x + 18) << 4) + 8;
                ((struct DotrCursorSlotUv *)&packet.vertex[1].uv)->v = ((y + 24) << 4) + 8;
                cursor = Cells(cursor, &packet, &i, 560);
            }
            cursor = SzDuel_DrawWindow_Font_Pipe(cursor, 320, 212, 2, 3, func_0019bef0(text), text, colour.b);
            view->alpha += view->step * 4;
            if (view->alpha <= 0 || view->alpha >= 128) {
                view->step = -view->step;
            }
            if (view->busy) {
                view->busy -= 4;
            }
            view->counter++;
        }
        RhBase_SetVifPacketArea(ExSortSoftFocus_Pipe(SzDuel_FinishWindowSession_Pipe(cursor), 0, 0, 0, 0, 12, 0));
        /* MATCHING DEVICE: the explicit return ending the last conditional block makes the
         * compiler fill branch delay slots throughout the function with useful instructions
         * instead of nops, and drop the originals of instructions it copies into them. */
        return;
    }
}
