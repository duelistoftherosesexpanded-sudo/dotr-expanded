#include "dotr/ee/camp_3d_spots.h"

int Taku_Camp_3D(int cursor)
{
    /* MATCHING DEVICE: 32 positions (22 are used) give the original's 752-byte frame; the
     * declaration order of the locals below and the `<=` loop bounds are also needed. */
    struct DotrCamp3dVec pos[32];
    struct DotrCamp3dBoard *w = &data_00299e10_camp3d;
    struct DotrCamp3dWork *g = DOTR_CAMP3D_WORK;
    short i;
    short a, b, c;
    short flag;
    int n;
    float h;

    flag = 0;
    cursor = func_001ee4b0_camp3d(cursor, 34, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, 35, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, 36, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, 37, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, 38, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, 39, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[11].m0 + 34, &data_00298ba0_camp3d[11], 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[11].m1 + 34, &data_00298ba0_camp3d[11], 0, 0, 0, 0);
    for (i = 0; i <= 21; i++) {
        h = 12.0f;
        if (i == 16) {
            h = 18.0f;
        }
        if (w->seen[i] != 0) {
            pos[i].x = data_00298ba0_camp3d[i].x;
            pos[i].y = data_00298ba0_camp3d[i].y;
            pos[i].z = data_00298ba0_camp3d[i].z;
        } else if (g->rise[i] != 0) {
            flag = 1;
            pos[i].x = 0.4f * (float)((g->count[i] >> 2) % 2) + data_00298ba0_camp3d[i].x;
            pos[i].y = data_00298ba0_camp3d[i].y - h + h * (float)g->count[i] / 256.0f;
            pos[i].z = data_00298ba0_camp3d[i].z;
            if (g->started[i] == 0 && g->count[i] != 0 && i != 11) {
                SzEffect_Entry(0, 404, i, 0, 0);
                g->started[i] = 1;
            }
        } else {
            pos[i].x = data_00298ba0_camp3d[i].x;
            pos[i].y = data_00298ba0_camp3d[i].y - h;
            pos[i].z = data_00298ba0_camp3d[i].z;
        }
        if (w->mode == 0) {
            if (g->rise[i] != 0) {
                g->count[i] += 4;
                if (g->count[i] >= 256) {
                    g->count[i] = 0;
                    w->seen[i] = 1;
                    g->rise[i] = 0;
                    g->ship_state = 0;
                }
            }
        } else if (i == 16) {
            if (g->rise[i] == 2) {
                g->count[i] += 4;
                if (g->count[i] >= 256) {
                    g->count[i] = 0;
                    w->seen[i] = 1;
                    g->rise[i] = 0;
                    g->ship_state = 0;
                }
            }
        } else if (g->rise[i] != 0) {
            g->count[i] += 4;
            if (g->count[i] >= 256) {
                g->count[i] = 0;
                w->seen[i] = 1;
                g->rise[i] = 0;
                g->ship_state = 0;
            }
        }
    }
    if (DOTR_CAMP3D_ALL) {
        DOTR_CAMP3D_PICK_OK = 1;
    } else if (w->ticks >= 10000) {
        DOTR_CAMP3D_PICK_OK = 0;
    } else if (flag == 0 && DOTR_CAMP3D_5481 && !SzEffect_IsUse2(-1, -1) && DOTR_CAMP3D_5376 && !DOTR_CAMP3D_5482) {
        if (w->mode == 0) {
            if (DOTR_CAMP3D_5372 == 0) {
                switch (w->ticks) {
                case 100:
                case 101:
                    DOTR_CAMP3D_PICK_OK = 0;
                    break;
                default:
                    DOTR_CAMP3D_PICK_OK = 1;
                    break;
                }
            } else {
                DOTR_CAMP3D_PICK_OK = 0;
            }
        } else {
            if (DOTR_CAMP3D_5372 == 0) {
                switch (w->ticks) {
                case 104:
                case 105:
                case 111:
                    DOTR_CAMP3D_PICK_OK = 0;
                    break;
                default:
                    DOTR_CAMP3D_PICK_OK = 1;
                    break;
                }
            } else {
                DOTR_CAMP3D_PICK_OK = 0;
            }
        }
    } else if (w->ticks == 0 && DOTR_CAMP3D_5481 && !SzEffect_IsUse2(-1, -1) && DOTR_CAMP3D_5376 && !DOTR_CAMP3D_5482) {
        DOTR_CAMP3D_PICK_OK = 1;
    } else {
        DOTR_CAMP3D_PICK_OK = 0;
    }
    cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[0].m0 + 34, &pos[0], 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[0].m1 + 34, &pos[0], 0, 0, 0, 0);
    if (DOTR_CAMP3D_ALL) {
        for (n = 1; n <= 10; n++) {
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m0 + 34, &pos[n], 0, 0, 0, 0);
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m1 + 34, &pos[n], 0, 0, 0, 0);
        }
        for (n = 12; n <= 21; n++) {
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m0 + 34, &pos[n], 0, 0, 0, 0);
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m1 + 34, &pos[n], 0, 0, 0, 0);
        }
    } else if (w->mode == 0) {
        for (n = 1; n <= 10; n++) {
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m0 + 34, &pos[n], 0, 0, 0, 0);
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m1 + 34, &pos[n], 0, 0, 0, 0);
        }
    } else if (w->mode == 1) {
        for (n = 12; n <= 21; n++) {
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m0 + 34, &pos[n], 0, 0, 0, 0);
            cursor = func_001ee4b0_camp3d(cursor, data_00298ba0_camp3d[n].m1 + 34, &pos[n], 0, 0, 0, 0);
        }
    }
    cursor = Taku_Camp_3D_BOU(cursor);
    /* No default: other values leave a, b and c uninitialised, as in the original. */
    switch (DOTR_CAMP3D_SHIP_CASE) {
    case 0:
        b = 15;
        a = 0;
        c = 27;
        break;
    case 1:
        a = 0;
        b = 0;
        c = 27;
        break;
    case 2:
        b = 4;
        a = 1;
        c = 30;
        break;
    case 3:
        a = 1;
        b = 5;
        c = 30;
        break;
    case 4:
        a = 1;
        b = 11;
        c = 27;
        break;
    }
    if (c == 27) {
        DOTR_CAMP3D_SHIP_LARGE = 0;
    } else {
        DOTR_CAMP3D_SHIP_LARGE = 1;
    }
    cursor = func_0022cc40_camp3d(cursor, a, b, c);
    cursor = func_001ee4b0_camp3d(cursor, 41, 0, 0, 0, 0, 0);
    cursor = func_001ee4b0_camp3d(cursor, 40, 0, 0, 0, 0, 0);
    return Taku_Camp_3D_LeaderModel(cursor);
}
