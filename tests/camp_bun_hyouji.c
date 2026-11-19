/* Synthetic host tests for the camp_bun_hyouji group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_bun_hyouji.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

signed char dotr_camp_bun_hyouji_board[0x100];
union DotrBunHyoujiWork dotr_camp_bun_hyouji_work;
DotrBunRect data_0029aeb0_bunhyouji;
DotrBunG4 data_00298af6_bunhyouji;
DotrBunWin data_00297b72_bunhyouji;
static DotrBunInfo info = { 1, { 0, 0, 100, 20 } };
static int font_msg = -1, window_x = -1;

int Taku_G4_Set(int p, void *src, short x, short y) { return p + 1; }
int SzMsg_DrawRatioX_Pipe(int p, int a1, int x, int y, float sx, float sy, void *col, void *draw) { return p + 1; }
void SzMsg_DrawRect_Pipe(void) { }
int SzMsg_DrawX_Pipe(int p, int a1, int x, int y, void *col, void *draw) { return p + 1; }
int SzEffect_IsUse2(int arg0, int arg1) { return 0; }
int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4) { font_msg = arg2; return arg0 + 1; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
void *SzMsg_GetInfo(int arg0) { return &info; }
int Taku_Camp_BUN_HANTEI(void) { return 0; }
int SzMsg_DrawRectG_Pipe(int cursor, int index, int x, int y, void *rect_v, unsigned char *colour) { return cursor + 1; }
void SzMsg_Free(int arg0) { }
void ShFace_Close(unsigned char *arg0) { }
int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx,
                    unsigned short dy, unsigned short depth, int unused, short alpha)
{
    window_x = dx;
    return p + 1;
}

int main(void)
{
    DotrBunWork *w = (DotrBunWork *)dotr_camp_bun_hyouji_work.bytes;
    short event = 0x2B70;

    /* No message running (step 0), the caption flag 0x1ed5483 set, event 0x2b70: caption
     * message 0x8ab in a window centred on its text width (0x140 - 100 / 2). */
    memcpy(dotr_camp_bun_hyouji_board + 0xC2, &event, sizeof event);
    w->f5483 = 1;
    Taku_Camp_BUN_HYOUJI(0x1000);
    assert(font_msg == 0x8AB && window_x == 0x140 - 50);
    return 0;
}
