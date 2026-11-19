#include "dotr/ee/camp_sinema_cut.h"

DotrTikeiWord Taku_Camp_SINEMA_CUT(DotrTikeiWord cursor, unsigned short fade)
{
    struct DotrSinemaWork *b = DOTR_SINEMA_BLOCK;
    struct DotrSinemaQuad q = data_0029aec0;
    int i;
    int j;
    int n;

    if (fade == 0) {
        if (DOTR_SINEMA_STATE == 1 && DOTR_SINEMA_HOLD == 0) {
            n = 0;
            if (DOTR_SINEMA_MODE == 0) {
                if (0 <= DOTR_SINEMA_CUT0) {
                    DOTR_SINEMA_CUT0 -= 8;
                    if (DOTR_SINEMA_CUT0 <= 0) {
                        DOTR_SINEMA_CUT0 = 0;
                        n++;
                    }
                }
                if (0 <= DOTR_SINEMA_CUT1) {
                    DOTR_SINEMA_CUT1 -= 8;
                    if (DOTR_SINEMA_CUT1 <= 0) {
                        DOTR_SINEMA_CUT1 = 0;
                        n++;
                    }
                }
                if (0 <= DOTR_SINEMA_CUT2) {
                    DOTR_SINEMA_CUT2 -= 8;
                    if (DOTR_SINEMA_CUT2 <= 0) {
                        DOTR_SINEMA_CUT2 = 0;
                        n++;
                    }
                }
            } else if (DOTR_SINEMA_MODE == 1) {
                if (DOTR_SINEMA_CUT0 <= 64) {
                    DOTR_SINEMA_CUT0 += 8;
                    if (DOTR_SINEMA_CUT0 >= 64) {
                        DOTR_SINEMA_CUT0 = 64;
                        n++;
                    }
                }
                if (0 <= DOTR_SINEMA_CUT1) {
                    DOTR_SINEMA_CUT1 -= 8;
                    if (DOTR_SINEMA_CUT1 <= 0) {
                        DOTR_SINEMA_CUT1 = 0;
                        n++;
                    }
                }
                if (0 <= DOTR_SINEMA_CUT2) {
                    DOTR_SINEMA_CUT2 -= 8;
                    if (DOTR_SINEMA_CUT2 <= 0) {
                        DOTR_SINEMA_CUT2 = 0;
                        n++;
                    }
                }
            } else if (DOTR_SINEMA_MODE == 2) {
                if (DOTR_SINEMA_CUT0 < 64) {
                    DOTR_SINEMA_CUT0 += 8;
                    if (DOTR_SINEMA_CUT0 >= 64) {
                        DOTR_SINEMA_CUT0 = 64;
                        n++;
                    }
                } else {
                    DOTR_SINEMA_CUT0 = 64;
                    n++;
                    if (DOTR_SINEMA_CUT1 <= 64) {
                        DOTR_SINEMA_CUT1 += 8;
                        if (DOTR_SINEMA_CUT1 >= 64) {
                            DOTR_SINEMA_CUT1 = 64;
                            n++;
                        }
                    }
                }
                if (0 <= DOTR_SINEMA_CUT2) {
                    DOTR_SINEMA_CUT2 -= 8;
                    if (DOTR_SINEMA_CUT2 <= 0) {
                        DOTR_SINEMA_CUT2 = 0;
                        n++;
                    }
                }
            } else if (DOTR_SINEMA_MODE == 3) {
                if (DOTR_SINEMA_CUT0 < 64) {
                    DOTR_SINEMA_CUT0 += 8;
                    if (DOTR_SINEMA_CUT0 >= 64) {
                        DOTR_SINEMA_CUT0 = 64;
                        n++;
                    }
                } else if (DOTR_SINEMA_CUT1 < 64) {
                    DOTR_SINEMA_CUT0 = 64;
                    DOTR_SINEMA_CUT1 += 8;
                    n++;
                    if (DOTR_SINEMA_CUT1 >= 64) {
                        DOTR_SINEMA_CUT1 = 64;
                        n++;
                    }
                } else {
                    DOTR_SINEMA_CUT0 = 64;
                    DOTR_SINEMA_CUT1 = 64;
                    DOTR_SINEMA_CUT2 += 8;
                    n += 2;
                    if (DOTR_SINEMA_CUT2 >= 116) {
                        DOTR_SINEMA_CUT2 = 116;
                        n++;
                    }
                }
            }
            if (n >= 3) {
                DOTR_SINEMA_STATE = 2;
            }
        }
        for (i = 0; i < 2; i++) {
            q = tex_sinema_brack[i];
            /* MATCHING DEVICE: the DOTR_SINEMA_C* terms are scaled spellings of the counters. */
            if (i == 0) {
                q.w[5] = DOTR_SINEMA_C2 + (tex_sinema_brack[i].w[1] + DOTR_SINEMA_C0 + DOTR_SINEMA_C1);
                q.w[7] = DOTR_SINEMA_C2 + (tex_sinema_brack[i].w[1] + DOTR_SINEMA_C0 + DOTR_SINEMA_C1);
            } else {
                q.w[1] = tex_sinema_brack[i].w[1] - DOTR_SINEMA_C0 - DOTR_SINEMA_C1 - DOTR_SINEMA_C2;
                q.w[3] = tex_sinema_brack[i].w[1] - DOTR_SINEMA_C0 - DOTR_SINEMA_C1 - DOTR_SINEMA_C2;
            }
            for (j = 0; j < 16; j++) {
                if (j % 4 == 3) {
                    q.col[j] = 128;
                } else {
                    q.col[j] = 0;
                }
            }
            q.flag = 0;
            cursor = Taku_G4_Set(cursor, (struct DotrTikeiItem *)&q, 0, 0);
        }
    } else {
        if (DOTR_SINEMA_STATE == 1) {
            if (DOTR_SINEMA_MODE == 0) {
                if (DOTR_SINEMA_CUT0 < 255) {
                    DOTR_SINEMA_CUT0 += 32;
                    if (DOTR_SINEMA_CUT0 >= 255) {
                        DOTR_SINEMA_CUT0 = 255;
                        DOTR_SINEMA_MODE = 1;
                        DOTR_SINEMA_STATE = 0;
                    }
                } else {
                    DOTR_SINEMA_MODE = 1;
                    DOTR_SINEMA_CUT0 = 255;
                    DOTR_SINEMA_STATE = 0;
                }
            } else {
                if (0 < DOTR_SINEMA_CUT0) {
                    DOTR_SINEMA_CUT0 -= 32;
                    if (DOTR_SINEMA_CUT0 <= 0) {
                        DOTR_SINEMA_CUT0 = 0;
                        DOTR_SINEMA_MODE = 0;
                        DOTR_SINEMA_STATE = 0;
                    }
                } else {
                    DOTR_SINEMA_MODE = 0;
                    DOTR_SINEMA_CUT0 = 0;
                    DOTR_SINEMA_STATE = 0;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 8; j++) {
                q.w[j] = tex_sinema_brack[i].w[j];
            }
            q.col[3] = q.col[7] = q.col[11] = q.col[15] = b->cut0;
            cursor = Taku_G4_Set(cursor, (struct DotrTikeiItem *)&q, 0, 0);
        }
    }
    return cursor;
}
