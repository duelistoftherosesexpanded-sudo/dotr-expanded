#include "dotr/ee/small_misc.h"

DotrMiscAddress SzDuel_DrawWindow_FontDigit_Pipe(DotrMiscAddress cursor, int x, int y, int a, int flags, int width, int value)
{
    char buf[16];
    int pad = 0;
    int div = 1;
    int i;

    if (flags & 0x20)
        pad = 1;
    for (i = 0; i < width; i++)
        buf[i] = ' ';
    for (i = 0; i < width;) {
        buf[width - i - 1] = (value / div) % 10 + '0';
        div *= 10;
        i++;
        if (!pad && value < div)
            break;
    }
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, a, flags, width, buf, 0);
}
