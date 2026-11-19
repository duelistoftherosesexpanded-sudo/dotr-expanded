#include "dotr/ee/camp_tikei.h"

DotrTikeiWord Taku_Camp_Kumo(DotrTikeiWord cursor)
{
    struct DotrTikeiCamp *b = &data_00299e10_camp;
    struct DotrGlideFrame *frame = &prim_data_glide;
    struct DotrTikeiQuad q = data_0029ab80;
    short i;

    if (b->stage == 10020 && DOTR_TIKEI_LIMIT < 6) {
        q.z = -10;
    } else if (b->stage == 11184) {
        q.z = -10;
    } else if (b->stage == 12156) {
        q.z = -10;
    }
    if (DOTR_TIKEI_KUMO_MODE != 0) {
        switch ((short)DOTR_TIKEI_KUMO_MODE) {
        case 2:
            break;
        case 3:
            if (0 <= DOTR_TIKEI_KUMO_FADE) {
                DOTR_TIKEI_KUMO_FADE -= 16;
                if (DOTR_TIKEI_KUMO_FADE <= 0) {
                    DOTR_TIKEI_KUMO_FADE = 0;
                    DOTR_TIKEI_KUMO_MODE = 0;
                }
            }
            break;
        case 4:
            if (DOTR_TIKEI_KUMO_FADE <= 4096) {
                DOTR_TIKEI_KUMO_FADE += 32;
                if (DOTR_TIKEI_KUMO_FADE >= 4096) {
                    DOTR_TIKEI_KUMO_FADE = 4096;
                    DOTR_TIKEI_KUMO_MODE = 4;
                }
            }
            break;
        case 10:
            DOTR_TIKEI_KUMO_FADE += 1024;
            if (DOTR_TIKEI_KUMO_FADE >= 4096) {
                DOTR_TIKEI_KUMO_FADE = 4096;
                DOTR_TIKEI_KUMO_MODE = 11;
            }
            break;
        case 20:
            DOTR_TIKEI_KUMO_FADE -= 256;
            if (DOTR_TIKEI_KUMO_FADE <= 0) {
                DOTR_TIKEI_KUMO_FADE = 0;
                DOTR_TIKEI_KUMO_MODE = 21;
            }
            break;
        case 22:
            DOTR_TIKEI_KUMO_FADE += 256;
            if (DOTR_TIKEI_KUMO_FADE >= 4096) {
                DOTR_TIKEI_KUMO_FADE = 4096;
                DOTR_TIKEI_KUMO_MODE = 23;
            }
            break;
        case 30:
            DOTR_TIKEI_KUMO_FADE += 1024;
            if (DOTR_TIKEI_KUMO_FADE >= 4096) {
                DOTR_TIKEI_KUMO_FADE = 4096;
                DOTR_TIKEI_KUMO_MODE = 31;
            }
            break;
        case 32:
            DOTR_TIKEI_KUMO_FADE -= 32;
            if (DOTR_TIKEI_KUMO_FADE <= 0) {
                DOTR_TIKEI_KUMO_FADE = 0;
                DOTR_TIKEI_KUMO_MODE = 33;
            }
            break;
        }
        if (DOTR_TIKEI_KUMO_MODE >= 30) {
            q.c[0].r = q.c[1].r = q.c[2].r = q.c[3].r = 255;
            q.c[0].g = q.c[1].g = q.c[2].g = q.c[3].g = 255;
            q.c[0].b = q.c[1].b = q.c[2].b = q.c[3].b = 255;
        } else if (DOTR_TIKEI_KUMO_MODE >= 20) {
            q.c[0].r = q.c[1].r = q.c[2].r = q.c[3].r = 0;
            q.c[0].g = q.c[1].g = q.c[2].g = q.c[3].g = 0;
            q.c[0].b = q.c[1].b = q.c[2].b = q.c[3].b = 0;
        } else if (DOTR_TIKEI_KUMO_MODE >= 10) {
            q.c[0].r = q.c[1].r = q.c[2].r = q.c[3].r = 0;
            q.c[0].g = q.c[1].g = q.c[2].g = q.c[3].g = 0;
            q.c[0].b = q.c[1].b = q.c[2].b = q.c[3].b = 32;
        }
        q.w[0] = q.w[4] = 0;
        q.w[2] = q.w[6] = 640;
        q.w[1] = q.w[3] = 0;
        q.w[5] = q.w[7] = 448;
        if (b->stage == 10040 && DOTR_TIKEI_LIMIT > 1) {
            q.c[0].a = q.c[1].a = q.c[2].a = q.c[3].a = 0;
        } else if (b->stage == 11162 || b->stage == 12132) {
            q.c[0].a = q.c[1].a = q.c[2].a = q.c[3].a = 0;
        } else {
            q.c[0].a = q.c[1].a = q.c[2].a = q.c[3].a = (DOTR_TIKEI_KUMO_FADE << 7) >> 12;
            cursor = Taku_G4_Set(cursor, (struct DotrTikeiItem *)&q, 0, 0);
        }
    }
    Taku_Coord_Set(frame);
    for (i = 0; i < 16; i++) {
    }
    return cursor;
}
