#include "dotr/ee/point_lists.h"

/* For each of the 2 x 16 lists at 0x299020: converts its points to float pairs
 * in a local array, sets a local weight of 1.0 for each, and calls
 * Taku_spline2 with the matching output record's three addresses, the two
 * local arrays, the count, the halfword at +66 and 4.0. */
void Taku_spline_init(void)
{
    float points[64][2];
    float weights[64];
    int j;
    int i;
    struct DotrPointBlock *block = DOTR_POINT_BLOCK;
    int k;
    int count;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            count = root_no[i][j].count;
            for (k = 0; k < count; k++) {
                points[k][0] = root_no[i][j].points[k][0];
                points[k][1] = root_no[i][j].points[k][1];
                weights[k] = 1.0f;
            }
            Taku_spline2(block->outputs[i][j].unknown_000, block->outputs[i][j].unknown_100,
                          &block->outputs[i][j].unknown_200, points, weights, count,
                          root_no[i][j].unknown_42, 4.0f);
        }
    }
}
