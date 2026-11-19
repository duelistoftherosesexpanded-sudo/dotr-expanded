/* Synthetic host tests for the camp_ivent_window group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_ivent_window.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

union DotrIventWinBoard dotr_camp_ivent_window_board;
union DotrIventWinWork dotr_camp_ivent_window_work;
DotrIventWinFrame data_0029aa08_iventwin;
DotrIventWinSprite data_0029aa20_iventwin;
DotrIventWinPair data_0029aa50_iventwin;
DotrIventWinSprite data_0029aaa0_iventwin;
DotrIventWinPair data_0029aad0_iventwin;
DotrIventWinSprite data_0029ab20_iventwin;
DotrIventWinSprite data_0029ab50_iventwin;
DotrIventWinSprite tex_bara_data[0x100];
DotrIventWinTex tex_uv_data[0x400];
DotrIventWinCode data_00299d00_iventwin[0x40];
char data_0030f0bb_iventwin[16];
char data_0030f0c7_iventwin[16];
char data_0030f0d4_iventwin[16];
char data_0030f0e8_iventwin[16];
static int name_calls;

int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text,
                                unsigned char *color) { return packet + 1; }
int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5) { return arg0 + 1; }
void *SzMsg_GetInfo(int arg0) { return NULL; }
int SzDuel_GetMsgName(int arg0) { return 0; }
void SzMsg_Free(int arg0) { }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int Taku_Camp_Kazu_Disp(int arg0, int arg1, int arg2, int arg3) { return arg0 + 1; }
/* Declared inside the function with their own types there. */
int Taku_Camp_Ivent_Name(int packet) { name_calls++; return packet + 0x10; }
int Taku_Window_Set(int p, void *rect, int style, int mode, unsigned short dx, unsigned short dy, int depth,
                    int unused, int alpha) { return p + 1; }
int Taku_Obj_Set(int p, void *src, short x, short y) { return p + 1; }
int Taku_Bunsyou_Set(int p, int a1, int a2, short x, short y, float sx, float sy, int a7, void *c) { return p + 1; }
int SzMsg_LoadFont_Pipe(int a0, int a1, int a2, int a3, int a4) { return a0 + 1; }
int Taku_FT4_Set(int p, void *src, short x, short y, unsigned short flip) { return p + 1; }
int SzMsg_DrawRatioX_Pipe(int p, int a1, int a2, int a3, float sx, float sy, void *e, void (*f)(void)) { return p + 1; }

int main(void)
{
    /* Event 0x2738 with the window open (mode byte 0x1ed53a6 set): the name-entry screen draws
     * it, Taku_Camp_Ivent_Name is called once. */
    CutSceneNumber_iventwin = 0x2738;
    dotr_camp_ivent_window_work.bytes[0x53A6] = 2;
    Taku_Camp_Ivent_Window(0x1000);
    assert(name_calls == 1);
    return 0;
}
