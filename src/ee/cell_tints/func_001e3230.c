#include "dotr/ee/cell_tints.h"

static inline struct DotrTintCell *dotr_tint_cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &data_00318e60_tint[y][x];
}

/* Moves one colour byte towards its goal by at most 8. */
#define DOTR_TINT_STEP(cur, goal) \
    if ((cur) > (goal)) { \
        d = (cur) - (goal); \
        d = d < 8 ? d : 8; \
        (cur) -= d; \
    } else { \
        d = (goal) - (cur); \
        d = d < 8 ? d : 8; \
        (cur) += d; \
    }

/* Fades cell (x, y): quad 1 towards quad 0 and quad 3 towards quad 2, or, while the
 * shown kind differs from the wanted one, quad 1's last byte down to zero first. */
int SzDuel_CalcMapColor(int x, int y)
{
    struct DotrTintCell *cell = dotr_tint_cell(x, y);
    int d;
    int v;

    if (cell == 0) {
        return 0;
    }
    if (cell->TargetSelectionHighlight != cell->SelectionHighlightType) {
        v = cell->quads[1].alpha;
        if (v != 0) {
            d = (unsigned char)v;
            d = d < 8 ? d : 8;
            cell->quads[1].alpha -= d;
        } else {
            cell->SelectionHighlightType = cell->TargetSelectionHighlight;
        }
        return 1;
    }
    DOTR_TINT_STEP(cell->quads[1].red, cell->quads[0].red)
    DOTR_TINT_STEP(cell->quads[1].green, cell->quads[0].green)
    DOTR_TINT_STEP(cell->quads[1].blue, cell->quads[0].blue)
    DOTR_TINT_STEP(cell->quads[1].alpha, cell->quads[0].alpha)
    DOTR_TINT_STEP(cell->quads[3].red, cell->quads[2].red)
    DOTR_TINT_STEP(cell->quads[3].green, cell->quads[2].green)
    DOTR_TINT_STEP(cell->quads[3].blue, cell->quads[2].blue)
    DOTR_TINT_STEP(cell->quads[3].alpha, cell->quads[2].alpha)
    return 1;
}
