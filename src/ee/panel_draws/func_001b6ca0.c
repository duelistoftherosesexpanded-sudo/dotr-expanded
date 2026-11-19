#include "dotr/ee/panel_draws.h"

DotrSpriteWord SzDuel_DrawWindow_ExDigit_Pipe(DotrSpriteWord cursor, int x, int y, int digits, int value, int narrow, int all, int a7)
{
    struct DotrSpriteEntry *entry;
    int step;
    int power;
    int i;

    entry = SzMsg_GetInfo(0);
    step = 20 - (narrow ? 4 : 0);
    power = 1;
    if (value == 0x7fffffff) {
        ((int *)entry)[1] = 264;
        for (i = 0; i < digits; i++) {
            cursor = func_001fe7d0_panel(cursor, 0, x + (digits - i - 1) * step, y, a7, SzMsg_DrawRect_Pipe);
        }
        return cursor;
    }
    for (i = 0; i < digits;) {
        ((int *)entry)[1] = ((value / power) % 10) * 22;
        cursor = func_001fe7d0_panel(cursor, 0, x + (digits - i - 1) * step, y, a7, SzMsg_DrawRect_Pipe);
        power *= 10;
        i++;
        if (all == 0 && value < power) {
            break;
        }
    }
    if (i < digits && value < 0) {
        ((int *)entry)[1] = 242;
        cursor = func_001fe7d0_panel(cursor, 0, x + (digits - i - 1) * step, y, a7, SzMsg_DrawRect_Pipe);
    }
    return cursor;
}
