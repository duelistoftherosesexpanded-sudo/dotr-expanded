/* Synthetic host tests for the taku_window group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/taku_window.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

union DotrTakuWindowWork dotr_taku_window_work;
DotrTakuWindowTex tex_uv_data[0x400];
unsigned short window_tex_no[2][0x100];
static int quads, first_x0 = -1;

DotrTakuWindowWord Taku_Prim_Set_SP1(DotrTakuWindowWord p, short x0, short y0, short x1, short y1, short u,
                                     short v, short width, short height, unsigned char r, unsigned char g,
                                     unsigned char b, unsigned char a, unsigned char mode, unsigned short texture,
                                     unsigned char flag, short depth)
{
    return p + 1;
}

DotrTakuWindowWord Taku_Prim_Set_G4(DotrTakuWindowWord p, short x0, short y0, short x1, short y1, short x2,
                                    short y2, short x3, short y3, unsigned char r0, unsigned char g0,
                                    unsigned char b0, unsigned char a0, unsigned char r1, unsigned char g1,
                                    unsigned char b1, unsigned char a1, unsigned char r2, unsigned char g2,
                                    unsigned char b2, unsigned char a2, unsigned char r3, unsigned char g3,
                                    unsigned char b3, unsigned char a3, unsigned short mode, short depth)
{
    if (quads++ == 0) {
        first_x0 = x0;
    }
    return p + 1;
}

void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip)
{
    return (char *)packet + 1;
}

int main(void)
{
    DotrTakuWindowRect rect = { 10, 20, 110, 80, 0 };

    /* Style 2 (black), kind 0: the window body is one G4 quad inset by 2 and moved by dx. */
    assert(Taku_Window_Set(0x1000, &rect, 2, 0, 5, 0, 7, 0, 0x80) > 0x1000);
    assert(quads >= 1 && first_x0 == 10 + 2 + 5);
    return 0;
}
