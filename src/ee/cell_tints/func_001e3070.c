#include "dotr/ee/cell_tints.h"

static inline struct DotrTintCell *dotr_tint_cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &data_00318e60_tint[y][x];
}

int SzDuel_SetMapColor2(int x, int y, int c, struct DotrTintQuad *colour, int e, int f)
{
    struct DotrTintCell *cell = dotr_tint_cell(x, y);
    unsigned char lo;
    unsigned char hi;
    struct DotrTintItem *unit;

    hi = data_00318118_tint[0] >> 8;
    lo = 128 - hi;
    if (cell == 0) {
        return 0;
    }
    cell->TargetSelectionHighlight = c;
    if (colour != 0) {
        cell->quads[0] = *colour;
    } else {
        cell->quads[0].red = 0;
        cell->quads[0].green = 0;
        cell->quads[0].blue = 0;
        cell->quads[0].alpha = 0;
    }
    cell->quads[2].red = 0;
    cell->quads[2].green = 0;
    cell->quads[2].blue = 0;
    cell->quads[2].alpha = 0;
    if (f != 0 && (unit = cell->unit) != 0) {
        if (unit->unknown_57_bit_1) {
            cell->quads[2].red = (hi * 255 >> 7) + (lo * 255 >> 7);
            cell->quads[2].green = lo * 255 >> 7;
            cell->quads[2].blue = lo * 255 >> 7;
        } else if (unit->team == 0) {
            cell->quads[2].red = 255;
            cell->quads[2].green = 0;
            cell->quads[2].blue = 0;
        } else {
            cell->quads[2].red = 255;
            cell->quads[2].green = 255;
            cell->quads[2].blue = 255;
        }
        cell->quads[2].alpha = 128;
    }
    if (e != 0) {
        cell->quads[1] = cell->quads[0];
        cell->quads[3] = cell->quads[2];
    }
    return 1;
}
