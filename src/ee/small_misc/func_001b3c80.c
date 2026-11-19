#include "dotr/ee/small_misc.h"

/* Forwards its eight register arguments and three stack arguments to
 * lSzDuel_DrawWindow_ExIcon_Pipe with two zero stack arguments before them and 128 after. */
void lSzDuel_DrawWindow_Icon_Pipe(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k)
{
    lSzDuel_DrawWindow_ExIcon_Pipe(a, b, c, d, e, f, g, h, 0, 0, i, j, k, 128);
}
