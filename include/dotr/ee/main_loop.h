#ifndef DOTR_EE_MAIN_LOOP_H
#define DOTR_EE_MAIN_LOOP_H

/* The Main_* frame functions at 0x1000d0-0x100b00 (Japanese build names; same name prefix and
 * adjacent addresses), which only sequence calls into other modules.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/main.md. */

#ifdef __MWERKS__
typedef int DotrMainWord;
#else
#include <stdint.h>
typedef intptr_t DotrMainWord;
#endif

/* 0x100850 Main_Draw (original name; the Japanese body differs, so only the name is
 * taken from that build). Every callee is declared here with the argument registers this
 * function sets; other group headers declare the same functions with their own types.
 * Assumed: one word for the packet cursor passed down the *_Pipe chain, int results,
 * and the four bytes at 0x28b840 as a constant colour copied to the stack before its
 * last byte is replaced. */
struct DotrMainColor {
    unsigned char r, g, b, a;
};
extern struct DotrMainColor data_0028b840;
extern DotrMainWord RhBase_GetVifPacketArea(void);
extern void RhBase_SetVifPacketArea(DotrMainWord cursor);
extern DotrMainWord RhBase_SetZBufferMode(DotrMainWord cursor, int a);
extern DotrMainWord ExDitherMode_Pipe(DotrMainWord cursor, int value);
extern DotrMainWord ExDitherMatrixEz_Pipe(DotrMainWord cursor, int index);
extern DotrMainWord SzView_Set_Pipe(DotrMainWord cursor, int index);
extern DotrMainWord SzBattle_DrawStage_Pipe(DotrMainWord cursor);
extern DotrMainWord SzDuel_DrawStage_Pipe(DotrMainWord cursor);
extern DotrMainWord RhZSort_Finish_Pipe(DotrMainWord cursor);
extern int SzText_InitClutManage(int base, int size);
extern int SzText_DoRequest(int a0, int a1);
extern int SzText_FreeRequest(int textureFlags, int textureId);
extern void SzModel_SetDrawCache(int v);
extern void SzModel_DrawMotionBlur(int a);
extern int SzModel_SetMaxOT(int index, int value);
extern int SzEffect_IsUse2(int a, int b);
extern void SzEffect_Draw(int a);
extern void ExDitherMatrixEz(int a);
extern void ExZBufferMode(int a);
extern void ExFlushTexture(void);
extern void ExInitPrim(void);
extern void ExTermPrim(void);
extern long ExGetAlphaVal(int index);
extern void ExSortDispBlur(int a0, int a1, int a2, long a3, struct DotrMainColor *color);
extern void SzGame_ModeDraw(void);
extern void SzGame_ModeDrawAfter(void);
extern void SzGame_ModeDrawLast(void);
extern int SzGame_InqAutoBlur(void);
extern void SzDuel_DrawStageAfter(void);
extern void SzDuel_DrawUnit(void);
extern void SzDuel_DrawAllCard(void);
extern void SzFade_DispAll(void);
void Main_Draw(void);

#endif
