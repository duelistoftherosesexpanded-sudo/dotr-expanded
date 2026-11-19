/* Host smoke test for SzDuel_SetupCard_Pipe (0x1da7f0), the card set-up draw pipe.
 * Fixtures are host data, not game data; stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* Built without -Werror: the C converts 32-bit EE words to pointers, which warns on a
 * 64-bit host. The path below executes none of those conversions. */
#include <assert.h>
#include "dotr/ee/setup_card.h"

/* Data the function reads (host storage; the target links them at their addresses). The tables
 * whose types the unit defines next to the function are plain aligned bytes here. */
Duel_00317490 data_00317490;
char gSzDuel_aicolMsg_bytes[256];
unsigned char data_0028ff30[256] __attribute__((aligned(16)));
unsigned char data_00290020[1024] __attribute__((aligned(16)));
unsigned char gSzDuel_Icon_bytes[1024] __attribute__((aligned(16)));
unsigned char data_002bed50[1024] __attribute__((aligned(16)));
unsigned char data_002bed52[1024] __attribute__((aligned(16)));
unsigned char data_002bed54[1024] __attribute__((aligned(16)));
unsigned char data_002bed56[1024] __attribute__((aligned(16)));
unsigned char data_002bed5c[1024] __attribute__((aligned(16)));
unsigned char data_002bed5e[1024] __attribute__((aligned(16)));
unsigned char data_002bed60[1024] __attribute__((aligned(16)));
unsigned char data_002bed62[1024] __attribute__((aligned(16)));
unsigned char data_002bed68[1024] __attribute__((aligned(16)));
unsigned char data_002bed6a[1024] __attribute__((aligned(16)));

/* Call log. */
static const char *calls[64];
static int call_count;
static void note(const char *name)
{
    if (call_count < 64) {
        calls[call_count] = name;
    }
    call_count++;
}
static int same(const char *a, const char *b)
{
    while (*a != 0 && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}
static int called(const char *name)
{
    int i, n = 0;
    for (i = 0; i < call_count && i < 64; i++) {
        n += same(calls[i], name);
    }
    return n;
}

/* Callee on the tested path (declared without a prototype in the function's block scope). */
static s32 get_mode_arg = -1;
s32 SzGame_GetMode(s32 arg0)
{
    note("SzGame_GetMode");
    get_mode_arg = arg0;
    return 0;
}

/* Every other callee, with the prototype the unit sees (the header, or the function's
 * block-scope declarations where they differ): records the call, returns 0. */
s32 ExAssignAlpha(void *arg0, s64 arg1) { note("ExAssignAlpha"); (void)arg0; (void)arg1; return 0; }
s32 ExAssignTexture(void *arg0, void *arg1, s32 arg2) { note("ExAssignTexture"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s64 ExGetAlphaVal(s32 arg0) { note("ExGetAlphaVal"); (void)arg0; return 0; }
s32 ExSortPrim_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { note("ExSortPrim_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }
s32 lSzDuel_DrawWindow_PicItem_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s64 arg6) { note("lSzDuel_DrawWindow_PicItem_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return 0; }
s32 lSzDuel_IsUnitHide(void *arg0) { note("lSzDuel_IsUnitHide"); (void)arg0; return 0; }
s32 lSzDuel_SetupCardTexture(s32 arg0, s32 arg1) { note("lSzDuel_SetupCardTexture"); (void)arg0; (void)arg1; return 0; }
void * RhBase_ClearRenderFrame(u8 *arg0, u8 *arg1) { note("RhBase_ClearRenderFrame"); (void)arg0; (void)arg1; return 0; }
void * RhBase_DisableRenderFrame(void *arg0) { note("RhBase_DisableRenderFrame"); (void)arg0; return 0; }
void * RhBase_EnableRenderFrame(void *arg0) { note("RhBase_EnableRenderFrame"); (void)arg0; return 0; }
s32 RhBase_GetTextureInfoForRender(void *arg0, void *arg1) { note("RhBase_GetTextureInfoForRender"); (void)arg0; (void)arg1; return 0; }
void * RhBase_MaskFBuffer(void *arg0, u32 arg1) { note("RhBase_MaskFBuffer"); (void)arg0; (void)arg1; return 0; }
s32 RhBase_SetZBufferMode(s32 arg0, s32 arg1) { note("RhBase_SetZBufferMode"); (void)arg0; (void)arg1; return 0; }
s32 RhText_SetTextureFlush(s32 arg0) { note("RhText_SetTextureFlush"); (void)arg0; return 0; }
s32 SzData_ExchSpecial(s32 arg0, s32 arg1) { note("SzData_ExchSpecial"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetMsgName(s32 arg0) { note("SzDuel_GetMsgName"); (void)arg0; return 0; }
s32 SzDuel_GetUnitAP2(void *arg0, s32 arg1) { note("SzDuel_GetUnitAP2"); (void)arg0; (void)arg1; return 0; }
void * SzDuel_GetUnitConstEffect2(void *arg0, s32 arg1) { note("SzDuel_GetUnitConstEffect2"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetUnitDP2(void *arg0, s32 arg1) { note("SzDuel_GetUnitDP2"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetUnitLevel2(void *arg0, s32 arg1) { note("SzDuel_GetUnitLevel2"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_Kind2Attr(s32 arg0, s32 arg1) { note("SzDuel_Kind2Attr"); (void)arg0; (void)arg1; return 0; }
s32 SzGame_GetMenuNum(s32 arg0) { note("SzGame_GetMenuNum"); (void)arg0; return 0; }
s32 SzMsg_DrawRatioX_Pipe(s32 arg0, s32 arg1, s64 arg2, s64 arg3, f32 arg4, f32 arg5, s32 arg6, void (*arg7)(void)) { note("SzMsg_DrawRatioX_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; (void)arg7; return 0; }
s32 SzMsg_DrawRect_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, u8 *arg5) { note("SzMsg_DrawRect_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; return 0; }
s32 SzMsg_DrawX_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) { note("SzMsg_DrawX_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; return 0; }
void SzMsg_Free(s32 arg0) { note("SzMsg_Free"); (void)arg0; }
s32 * SzMsg_GetInfo(s32 arg0) { note("SzMsg_GetInfo"); (void)arg0; return 0; }
s32 SzMsg_LoadFont_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) { note("SzMsg_LoadFont_Pipe"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; return 0; }
void * SzText_GetReadyItem(s32 arg0, s32 arg1) { note("SzText_GetReadyItem"); (void)arg0; (void)arg1; return 0; }
s32 Ta_GetParamOfs(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { note("Ta_GetParamOfs"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }

int main(void)
{
    /* No draw descriptor: the pipe queries the game mode (argument 1) and returns the descriptor (0)
     * without building a packet. */
    assert(SzDuel_SetupCard_Pipe(0x1234, 0, 0) == 0);
    assert(called("SzGame_GetMode") == 1 && get_mode_arg == 1);
    assert(call_count == 1);
    return 0;
}
