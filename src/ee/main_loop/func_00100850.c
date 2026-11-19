#include "dotr/ee/main_loop.h"

/* Main_Draw: one frame of drawing. The stage is sent down the packet chain, text
 * requests 4, 5, 8 and 6 are freed when the effect slots that use them are idle and
 * then redone from offset 0x3bc0, and the automatic blur is drawn when its level is
 * above zero. p is first the packet cursor, then the text offset (one register). */
void Main_Draw(void)
{
    DotrMainWord p;
    int i;
    int blur;
    struct DotrMainColor col;

    p = RhBase_GetVifPacketArea();
    SzText_InitClutManage(0, 0);
    SzModel_SetDrawCache(0);
    p = RhBase_SetZBufferMode(p, 1);
    p = ExDitherMode_Pipe(p, 1);
    p = ExDitherMatrixEz_Pipe(p, 1);
    p = SzView_Set_Pipe(p, 0);
    p = SzBattle_DrawStage_Pipe(p);
    p = SzDuel_DrawStage_Pipe(p);
    p = RhZSort_Finish_Pipe(p);
    RhBase_SetVifPacketArea(p);
    ExDitherMatrixEz(2);
    SzGame_ModeDraw();
    SzModel_SetDrawCache(0);
    ExZBufferMode(0);
    ExDitherMatrixEz(3);
    if (SzEffect_IsUse2(0, -1) == 0 && SzEffect_IsUse2(1, -1) == 0 && SzEffect_IsUse2(2, -1) == 0) {
        SzText_FreeRequest(4, -1);
    }
    if (SzEffect_IsUse2(0, -1) == 0 && SzEffect_IsUse2(1, -1) == 0) {
        SzText_FreeRequest(5, -1);
        SzText_FreeRequest(8, -1);
    }
    if (SzEffect_IsUse2(1, -1) == 0 && SzEffect_IsUse2(2, -1) == 0) {
        SzText_FreeRequest(6, -1);
    }
    p = 0x3bc0;
    p += SzText_InitClutManage(p, 2);
    p += SzText_DoRequest(4, p);
    p += SzText_DoRequest(5, p);
    p += SzText_DoRequest(6, p);
    p += SzText_DoRequest(8, p);
    ExFlushTexture();
    ExInitPrim();
    SzDuel_DrawStageAfter();
    SzModel_DrawMotionBlur(-1);
    SzDuel_DrawUnit();
    for (i = 0; i < 84; i++) {
        SzModel_SetMaxOT(i, 0);
    }
    SzEffect_Draw(0);
    SzEffect_Draw(1);
    SzEffect_Draw(2);
    SzDuel_DrawAllCard();
    ExTermPrim();
    ExZBufferMode(1);
    SzGame_ModeDrawAfter();
    ExDitherMatrixEz(2);
    if ((blur = SzGame_InqAutoBlur()) > 0) {
        long alpha = ExGetAlphaVal(0);
        col = data_0028b840;
        col.a = blur << 4;
        ExSortDispBlur(0, 0, 1, alpha, &col);
    }
    SzFade_DispAll();
    ExDitherMatrixEz(3);
    SzGame_ModeDrawLast();
}
