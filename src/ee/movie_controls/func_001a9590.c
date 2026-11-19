#include "dotr/ee/movie_controls.h"

/* Draws the current mode (SzGame_GetMode(1)) unless the 2-bit level at +0x22c is 3 or
 * hidden_228 is set. Mode 17 sorts a frame-buffer rectangle in the stored colour and calls
 * lSzGame_TEXTURE_TEST for each non-zero texture word. */
void SzGame_ModeDraw(void)
{
    struct DotrMovieControlPrefix *p = &gSzGame_gl;
    struct DotrModeDrawPacket packet;
    int mode = SzGame_GetMode(1);

    if (gSzGame_gl.bits_22c.level < 3) {
        /* MATCHING DEVICE: a one-case switch, not `if (...) return;`, keeps the separate
         * branch to the epilogue the original has. */
        switch (p->hidden_228) {
        case 0:
            break;
        default:
            return;
        }
        switch (mode) {
        case -1:
            SzGame_DrawMenu();
            break;
        case 1:
            SzDuel_DrawWindow();
            break;
        case 6:
            SzDuel_DrawBuildDeck();
            break;
        case 7:
            SzDuel_Draw3DView();
            break;
        case 8:
            SzDuel_DrawTrade();
            break;
        case 9:
            SzDuel_DrawCustomDuel();
            break;
        case 10:
            lSzGame_DrawTitle();
            break;
        case 11:
            ExDitherMatrixEz(1);
            TkCamp_Draw();
            break;
        case 12:
            ExDitherMatrixEz(1);
            SzEvent_Draw();
            break;
        case 13:
            SzDuel_DrawMapEdit();
            break;
        case 18:
            SzDuel_DrawCardTest();
            break;
        case 14:
            ShFace_Draw();
            break;
        case 15:
            ShSlot_Draw();
            break;
        case 17:
            ((struct DotrModeDrawHeader *)packet.header)->unknown_02 = 6;
            ((struct DotrModeDrawHeader *)packet.header)->unknown_00 = 3;
            ((struct DotrModeDrawHeader *)packet.header)->unknown_08 = 1089;
            ((struct DotrModeDrawHeader *)packet.header)->unknown_04 = 0;
            packet.colour[0] = p->colour_20c[2];
            packet.colour[1] = p->colour_20c[1];
            packet.colour[2] = p->colour_20c[0];
            packet.colour[3] = p->colour_20c[3];
            packet.unknown_14 = 1.0f;
            packet.vertex[0].x = 27640;
            packet.vertex[0].y = 29176;
            packet.vertex[0].z = 0xfffff8;
            packet.vertex[0].f = 0;
            packet.vertex[1].x = 0x93f8;
            packet.vertex[1].y = 0x8df8;
            packet.vertex[1].z = 0xfffff8;
            packet.vertex[1].f = 0;
            ExSortPrim(&packet, 0, 0);
            if (p->texture_208 != 0) {
                lSzGame_TEXTURE_TEST(p->texture_208, 0, 0, 0);
            }
            if (p->texture_204 != 0) {
                lSzGame_TEXTURE_TEST(p->texture_204, p->word_214, p->word_218, p->bits_22f.flag);
            }
            break;
        }
    }
}
