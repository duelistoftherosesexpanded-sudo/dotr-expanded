#include "dotr/ee/draw_state.h"

void useSoftFocus(int flag)
{
    int width;
    int height;
    int x;
    int y;

    RhBase_GetScreenSize(&width, &height);
    RhBase_GetBufferFormatEx(0, &x, &y, 0);
    *(volatile long *)0x12000000 = (long)(flag ? 0x7f : 0xff) << 8 | 0x23;
    *(volatile long *)0x12000090 = (long)(x >> 5) | (long)(width >> 6) << 9 | (long)y << 15;
    *(volatile long *)0x120000a0 = 0x183227c | (long)(width * 4 - 1) << 32 | (long)(height - 1) << 44;
    *(volatile long *)0x12000070 = (long)(x >> 5) | (long)(width >> 6) << 9 | (long)y << 15;
    *(volatile long *)0x12000080 = 0x183327c | (long)(width * 4 - 1) << 32 | (long)(height - 1) << 44;
    *(volatile long *)0x120000d0 = 0;
}
