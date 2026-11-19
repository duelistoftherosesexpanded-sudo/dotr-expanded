#include "dotr/ee/camp_ship_route.h"

int Taku_Camp_3D_FUNE(int cursor, short a, short b, short model)
{
    int vec[3];
    short idx = DOTR_SHIP_WORK->idx;
    float dx2;
    float dz2;
    float dx;
    float dz;
    float px;
    float pz;
    int ang;
    int ang2;

    dx2 = DOTR_SHIP_X(2) - DOTR_SHIP_X(1);
    dz2 = DOTR_SHIP_Z(2) - DOTR_SHIP_Z(1);
    /* The original calls this and does not use the result. */
    func_001a6c18(dx2, dz2);
    dx = DOTR_SHIP_X(1) - DOTR_SHIP_X(0);
    dz = DOTR_SHIP_Z(1) - DOTR_SHIP_Z(0);
    ang = DOTR_SHIP_ANGLE(dx, dz);
    /* MATCHING DEVICE: the point is read into a local before the sum. */
    px = DOTR_SHIP_X(0);
    DOTR_SHIP_WORK->pos[0] = px + dx * (float)DOTR_SHIP_WORK->t / 1024.0f;
    DOTR_SHIP_WORK->pos[1] = data_00298ba4_shiproute;
    pz = DOTR_SHIP_Z(0);
    DOTR_SHIP_WORK->pos[2] = pz + dz * (float)DOTR_SHIP_WORK->t / 1024.0f;
    /* MATCHING DEVICE: the comparisons are written with the count first. */
    if (DOTR_SHIP_N <= idx) {
        ang2 = DOTR_SHIP_ANGLE(DOTR_SHIP_X(0) - DOTR_SHIP_X(-1), DOTR_SHIP_Z(0) - DOTR_SHIP_Z(-1));
        vec[0] = 0;
        vec[1] = ang2;
        vec[2] = 0;
    } else if (DOTR_SHIP_N - 1 <= idx) {
        vec[0] = 0;
        vec[1] = ang;
        vec[2] = 0;
    } else {
        short t = DOTR_SHIP_WORK->t;
        vec[0] = 0;
        vec[1] = DOTR_SHIP_ANGLE(dx * (float)(1024 - t) / 1024.0f + dx2 * (float)t / 1024.0f,
                                 dz * (float)(1024 - t) / 1024.0f + dz2 * (float)t / 1024.0f);
        vec[2] = 0;
    }
    DOTR_SHIP_WORK->angle = vec[1];
    if (DOTR_SHIP_WORK->show) {
        cursor = func_001ee4b0_shiproute(cursor, model + 34, DOTR_SHIP_WORK->pos, vec, 0, 0, 0);
    }
    if (DOTR_SHIP_WORK->state > 0 && DOTR_SHIP_WORK->t < 1024 && DOTR_SHIP_WORK->idx < DOTR_SHIP_N) {
        if (DOTR_SHIP_WORK->state == 1) {
            SzEffect_Entry(0, 403, 0, 0, 0);
            DOTR_SHIP_WORK->state = 2;
        }
        if (DOTR_SHIP_WORK->state == 2) {
            DOTR_SHIP_WORK->t += 256;
            if (DOTR_SHIP_WORK->t >= 1024) {
                DOTR_SHIP_WORK->idx++;
                if (DOTR_SHIP_N <= DOTR_SHIP_WORK->idx) {
                    DOTR_SHIP_WORK->t = 0;
                    DOTR_SHIP_WORK->state = 3;
                    DOTR_SHIP_WORK->idx = DOTR_SHIP_N;
                    DOTR_SHIP_WORK->flag[data_00299066_shiproute[a][b].id] = 1;
                } else {
                    DOTR_SHIP_WORK->t = 0;
                }
            }
        }
    }
    return cursor;
}
