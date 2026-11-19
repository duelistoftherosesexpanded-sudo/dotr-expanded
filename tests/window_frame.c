/* Synthetic host tests for the window_frame group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/window_frame.h"

union DotrFrameControl data_00317490_frame;
DotrFrameTexture gSzDuel_Icon_frame[0x100];
unsigned char gSzDuel_aaicolRgba_frame[16 * 8];
unsigned char aabFrameOfs_frame[16 * 4];
unsigned char abDecoOfs_frame[16];

static int sort_calls;
static int alpha_calls;
static long long alpha_last;
static int texture_calls;
static int icon_calls;
static unsigned short *icon_src[4];
static int icon_x[4];

int ExSortPrim_Pipe(int packet, void *prim, int a, int b)
{
    (void)prim;
    assert(a == 0 && b == 0);
    sort_calls++;
    return packet + 1;
}
int ExAssignAlpha(void *arg0, long long arg1)
{
    (void)arg0;
    alpha_calls++;
    alpha_last = arg1;
    return 0;
}
int ExAssignTexture(void *arg0, void *arg1, int arg2)
{
    (void)arg0; (void)arg1; (void)arg2;
    texture_calls++;
    return 0;
}
long long ExGetAlphaVal(int arg0) { return arg0; }
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet, unsigned short *src, int x, int y, int background, int image, int border,
                                   int shrink, int flipX, int flipY, int bright, unsigned char *rgb, int blend,
                                   unsigned char alpha)
{
    (void)y; (void)background; (void)image; (void)border; (void)shrink; (void)flipX; (void)flipY; (void)bright;
    (void)rgb; (void)blend; (void)alpha;
    icon_src[icon_calls] = src;
    icon_x[icon_calls++] = x;
    return packet + 1;
}

/* lSzDuel_DrawWindow_Frame_Pipe: style 0 with 8 by 8 tiles, no colour row and no icons; one
 * column and one row draw the eight outer tiles (the middle one is skipped). */
static void check_frame(void)
{
    int i;

    for (i = 0; i < 9; i++) {
        gSzDuel_Icon_frame[0x3b + i].x0 = 0;
        gSzDuel_Icon_frame[0x3b + i].y0 = 0;
        gSzDuel_Icon_frame[0x3b + i].x1 = 8;
        gSzDuel_Icon_frame[0x3b + i].y1 = 8;
    }
    assert(lSzDuel_DrawWindow_Frame_Pipe(100, -1, -1, 0, 10, 20, 1, 1, 1) == 108);
    assert(sort_calls == 8 && texture_calls == 8 && alpha_calls == 1 && alpha_last == 0x8000000044LL);
}

/* lSzDuel_DrawWindow_Emblem_Pipe: count 1 draws the emblem row 0xc6 and one mark. */
static void check_emblem(void)
{
    assert(lSzDuel_DrawWindow_Emblem_Pipe(200, 10, 20, 1, 0, 0) == 202);
    assert(icon_calls == 2 && icon_src[0] == (unsigned short *)&gSzDuel_Icon_frame[0xc6] && icon_x[0] == 10 + 0x6c0);
    assert(icon_src[1] == data_00290864_emblem && icon_x[1] == 10 + 0x6c0 + 0x10);
}

int main(void)
{
    check_frame();
    check_emblem();
    return 0;
}
