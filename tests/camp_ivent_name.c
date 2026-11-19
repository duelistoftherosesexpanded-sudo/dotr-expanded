/* Synthetic host tests for the camp_ivent_name group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_ivent_name.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

union DotrIventNameWork dotr_camp_ivent_name_work;
DotrIventNameUv data_0029a7e0_iventname;
DotrIventNamePalette data_0029a7f0_iventname;
DotrIventNameImage data_0029a830_iventname;
DotrIventNameSix data_0029a840_iventname;
DotrIventNameSprite data_0029a930_iventname;
DotrIventNameThree data_0029a960_iventname;
DotrIventNameSprite data_0029a9e0_iventname;
short data_002999a0_iventname[64][3];
signed char data_0030f0b0_iventname[16];
static int windows, last_style;

int SzMsg_LoadFont_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4) { return arg0 + 1; }
void SzMsg_Free(int arg0) { }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip) { return (char *)packet + 1; }
int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx,
                    unsigned short dy, unsigned short depth, int unused, short alpha)
{
    windows++;
    last_style = style;
    return p + 1;
}
/* Declared inside the function (unprototyped or with their own types there). */
int SzDuel_DrawWindow_Font_Pipe(void) { return 0; }
int Taku_Obj_Set(int p, void *src, short x, short y) { return p + 1; }
int Taku_G4_Set(int p, void *src, short x, short y) { return p + 1; }
int SzMsg_DrawRect_Pipe(void) { return 0; }
int SzMsg_GetInfo(void) { return 0; }
int Taku_Bunsyou_Set(int p, int a1, int a2, short x, short y, float sx, float sy, int a7, void *c) { return p + 1; }

int main(void)
{
    /* Screen mode 0 (not open): only the background window is drawn (style 2) and its cursor
     * returned. */
    assert(Taku_Camp_Ivent_Name(0x1000) == 0x1001);
    assert(windows == 1 && last_style == 2);
    return 0;
}
