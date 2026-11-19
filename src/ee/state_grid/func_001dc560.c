#include "dotr/ee/state_grid.h"

/* Returns the cursor unchanged when SzModel_GetDrawFlag(0) is 0. Otherwise walks the 7x7 map
 * row by row: copies the cell's second and fourth colour quads, scales bytes 2, 1 and 0 of each
 * by the cell's byte +26 over 128, and at ((x - 3) * 50, height, (y - 3) * 50) draws a 24 by 24
 * cursor outline for each quad whose fourth byte is non-zero: the first at height -0.2 with the
 * cell's byte +25 as alpha and the single-pass flag, the second at -0.1 with alpha 1 and no flag. */
int SzDuel_DrawLine_Pipe(int cursor)
{
    int x;
    int y;
    struct DotrStateCell *cell;
    float pos[4];
    float dx;
    float dz;
    struct DotrStateCellQuad c0;
    struct DotrStateCellQuad c1;

    if (SzModel_GetDrawFlag(0) == 0) {
        return cursor;
    }
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            cell = SzDuel_GetMapInfo(x, y);
            c0 = cell->unknown_04[1];
            c1 = cell->unknown_04[3];
            c0.unknown_2 = (c0.unknown_2 * cell->mapAlpha) >> 7;
            c0.unknown_1 = (c0.unknown_1 * cell->mapAlpha) >> 7;
            c0.unknown_0 = (c0.unknown_0 * cell->mapAlpha) >> 7;
            c1.unknown_2 = (c1.unknown_2 * cell->mapAlpha) >> 7;
            c1.unknown_1 = (c1.unknown_1 * cell->mapAlpha) >> 7;
            c1.unknown_0 = (c1.unknown_0 * cell->mapAlpha) >> 7;
            pos[0] = (x - 3) * 50;
            pos[2] = (y - 3) * 50;
            pos[1] = -0.2f;
            if (c0.unknown_3 != 0) {
                dz = dx = 24.0f;
                cursor = SzDuel_DrawCursor_Pipe(cursor, pos, dx, dz, cell->selectionHighlightType, &c0.unknown_0, 1);
            }
            pos[1] = -0.1f;
            if (c1.unknown_3 != 0) {
                dz = dx = 24.0f;
                cursor = SzDuel_DrawCursor_Pipe(cursor, pos, dx, dz, 1, &c1.unknown_0, 0);
            }
        }
    }
    return cursor;
}
