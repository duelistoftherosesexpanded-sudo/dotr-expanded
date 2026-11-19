#include "dotr/ee/draw_state.h"

void SzGame_ModeDrawAfter(void)
{
    struct DotrDrawModeView *view = &gSzGame_gl_modeview;
    int r = SzGame_GetMode(1);

    if (gSzGame_gl_mode.mode < 3) {
        switch (view->busy) {
        case 0:
            switch (r) {
            case 1:
                SzDuel_DrawJoinFade();
                SzDuel_DrawJoinCard();
                break;
            case 11:
                ExDitherMatrixEz(2);
                TkCamp_Draw_After();
                ShFace_Draw();
                break;
            }
            SzDuel_DrawDialog();
            return;
        }
    }
}
