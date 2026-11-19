#include "dotr/ee/ring_setup.h"

void Taku_Camp_3D_Poly_Init(void)
{
    short j;
    int phase = 0;
    struct DotrRing *row;
    int first;
    int middle;
    float z;
    float x;
    int last;
    short i;
    struct DotrRing *ring = &prim_data;
    struct DotrRingCell *cell = prim_data.cells;

    Taku_Coord_Init(&prim_data.head);
    prim_data.unknown_a0 = 0;
    for (i = 0; i < 16; ring = (struct DotrRing *)((unsigned char *)ring + 64), i++) {
        first = (i << 17) >> 5;
        middle = ((i * 2 + 1) << 16) >> 5;
        last = ((i * 2 + 2) << 16) >> 5;
        for (row = ring, j = 0; j < 4; row = (struct DotrRing *)((unsigned char *)row + 16), j++) {
            if (j == 2) {
                row->rows[0][0][0] = 0.0f;
                row->rows[0][0][1] = 8.65f;
                row->rows[0][0][2] = 0.0f;
                row->rows[0][0][3] = 0.0f;
            } else {
                switch (j) {
                case 0:
                    phase = first;
                    break;
                case 1:
                    phase = middle;
                    break;
                case 3:
                    phase = last;
                    break;
                }
                x = 5.0f * RhMath_CosI(phase);
                z = 5.0f * RhMath_SinI(phase);
                row->rows[0][0][0] = x;
                row->rows[0][0][1] = 8.65f;
                row->rows[0][0][2] = z;
                row->rows[0][0][3] = 0.0f;
            }
        }
    }
    for (j = 0; j < 16; j++, cell++) {
        Taku_Vector_Set(&cell->unknown_00, &model_data);
        cell->unknown_20 = 0;
        cell->color[0][2] = 0;
        cell->color[1][2] = 0;
        cell->color[2][2] = 128;
        cell->color[3][2] = 128;
        cell->color[0][0] = 128;
        cell->color[1][0] = 0;
        cell->color[2][0] = 0;
        cell->color[3][0] = 128;
        cell->color[0][1] = 0;
        cell->color[1][1] = 128;
        cell->color[2][1] = 0;
        cell->color[3][1] = 128;
        cell->color[0][3] = 128;
        cell->color[1][3] = 0;
        cell->color[2][3] = 0;
        cell->color[3][3] = 128;
    }
}
