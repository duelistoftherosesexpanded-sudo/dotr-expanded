/* Synthetic host tests for the title_pipes group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/title_pipes.h"

unsigned char data_0028ff28_title[8];
union DotrTitleStateFixture dotr_title_state;

static DotrTitleTexSprite sorted;
static int sort_calls;
static int pipe_sorts;
static int request_args[3];
static int frees[2];

int ExAssignAlpha(void *arg0, long long arg1) { (void)arg0; (void)arg1; return 0; }
int ExAssignTexture(void *arg0, void *arg1, int arg2) { (void)arg0; (void)arg1; (void)arg2; return 0; }
long long ExGetAlphaVal(int arg0) { return arg0; }
void ExFlushTexture(void) {}
int RhBase_GetUserGsAddress(void) { return 0x1000; }
int SzText_InitClutManage(int base, int size) { (void)base; (void)size; return 0x40; }
void SzText_Load(DotrTitleWord arg0, int arg1, int arg2) { (void)arg0; (void)arg1; (void)arg2; }
void ExSortPrim(DotrTitleWord arg0, int arg1, int arg2)
{
    assert(arg1 == 0 && arg2 == 0);
    memcpy(&sorted, (void *)arg0, sizeof(sorted));
    sort_calls++;
}
int SzText_LoadRequest(int group, int id) { (void)group; return 0x100 + id; }
int ExSortPrim_Pipe(int packet, void *prim, int a, int b) { (void)prim; (void)a; (void)b; pipe_sorts++; return packet + 1; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int SzText_DoRequest_Pipe(int arg0, int arg1, int arg2, int *arg3)
{
    request_args[0] = arg0;
    request_args[1] = arg1;
    request_args[2] = arg2;
    *arg3 = 0;
    return arg0 + 1;
}
int SzText_FreeRequest(int arg0, int arg1) { frees[0] = arg0; frees[1] = arg1; return 0; }
int lSzGame_DrawTitle_Rose_Pipe(int packet, int id, int x, int y, int width, int height, float depth)
{
    (void)id; (void)x; (void)y; (void)width; (void)height; (void)depth;
    return packet + 1;
}

/* lSzGame_TEXTURE_TEST: a 100 by 50 texture at (10, 20), normal size: one sprite centred on
 * (0x80a, 0x814), texture coordinates over the 128 by 64 power-of-two area. */
static void check_texture_test(void)
{
    unsigned char record[0x60];
    unsigned short w = 100;
    unsigned short h = 50;

    memset(record, 0, sizeof(record));
    memcpy(record + 0x54, &w, 2);
    memcpy(record + 0x56, &h, 2);
    lSzGame_TEXTURE_TEST(record, 10, 20, 0);
    assert(sort_calls == 1);
    assert(sorted.s1 == 100.0f / 128.0f && sorted.t1 == 50.0f / 64.0f);
    assert((int)sorted.xyz0.x == ((0x80a - 50) << 4) - 8 && (int)sorted.xyz1.y == ((0x814 + 25) << 4) - 8);
}

/* lSzGame_DrawTitle_Pipe: alpha below 0x80 with the hidden flag set returns after the logo and
 * its fading overlay (two sorted sprites). */
static void check_draw_title(void)
{
    dotr_title_state.state.alpha = 0x40;
    dotr_title_state.state.flags.hidden = -1;
    assert(lSzGame_DrawTitle_Pipe(100) == 104);
    assert(request_args[0] == 100 && request_args[1] == 9 && request_args[2] == 0x37c0 + 0x40);
    assert(frees[0] == 9 && frees[1] == -1 && pipe_sorts == 2);
}

int main(void)
{
    check_texture_test();
    check_draw_title();
    return 0;
}
