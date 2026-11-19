#include "dotr/ee/glide_steps.h"

/* Resets the camp block at 0x1ed0000: clears it, sets the surface, the head and the
 * camera vectors, clears the per-side tables, restores the save block when the byte at
 * 0x34f6f8 asks for it and derives three 5-bit picks from the board's twelve halfwords. */
void Taku_Camp_Data_Init(void)
{
    float a[4];
    float b[4];
    float c[4];
    struct DotrCampInitBlock *bank = DOTR_CAMP_INIT;
    struct DotrGlideBoard *board = &data_00299e10_glide;
    struct DotrCampInitBlock *b1, *b2, *b3, *b4;
    int i, j, k;
    short s;
    int sum;
    unsigned short t;

    func_00195108(bank, 21680);
    a[0] = 0.0f;
    a[1] = 8.65f;
    a[2] = 100.0f;
    b[0] = 0.0f;
    b[1] = 8.65f;
    b[2] = 0.0f;
    c[0] = 100.0f;
    c[1] = 8.65f;
    c[2] = 0.0f;
    Ex3d_GetSurfaceC(a, b, c, bank->surface);
    for (i = 0; i < 12; i++) {
        bank->unknown_53cf[i] = 0;
        bank->unknown_53db[i] = 6;
        bank->unknown_53e7[i] = 1;
    }
    DOTR_CAMP_INIT->unknown_547c = 128;
    DOTR_CAMP_INIT->width = 320;
    DOTR_CAMP_INIT->height = 190;
    DOTR_CAMP_INIT->unknown_04 = 0;
    DOTR_CAMP_INIT->unknown_1a = 0;
    DOTR_CAMP_INIT->unknown_1c = 0;
    DOTR_CAMP_INIT->unknown_1e = 0;
    DOTR_CAMP_INIT->unknown_536e = board->ticks;
    DOTR_CAMP_INIT->limit = 0;
    DOTR_CAMP_INIT->unknown_5372 = 0;
    DOTR_CAMP_INIT->unknown_6e = 0;
    DOTR_CAMP_INIT->to.x = 0.0f;
    DOTR_CAMP_INIT->to.y = 0.0f;
    DOTR_CAMP_INIT->to.z = 0.0f;
    DOTR_CAMP_INIT->to.w = 0.0f;
    DOTR_CAMP_INIT->from.x = DOTR_CAMP_INIT->to.x;
    DOTR_CAMP_INIT->from.y = 96.0f + DOTR_CAMP_INIT->to.y;
    DOTR_CAMP_INIT->from.z = 96.0f + DOTR_CAMP_INIT->to.z;
    DOTR_CAMP_INIT->from.w = DOTR_CAMP_INIT->to.w;
    SzView_SetCamera(0, &DOTR_CAMP_INIT->from, &DOTR_CAMP_INIT->to);
    b1 = DOTR_CAMP_INIT;
    for (i = 0; i < 32; i++) {
        b1->unknown_8c[i] = 0;
        b1->unknown_10c[i] = 0;
    }
    DOTR_CAMP_INIT->unknown_1a6 = 0;
    DOTR_CAMP_INIT->unknown_1ae = 0;
    DOTR_CAMP_INIT->unknown_1b0 = 0;
    DOTR_CAMP_INIT->unknown_1b2 = 0;
    DOTR_CAMP_INIT->unknown_224 = 128;
    DOTR_CAMP_INIT->unknown_226 = 128;
    DOTR_CAMP_INIT->unknown_228 = 128;
    b2 = DOTR_CAMP_INIT;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            b2->records[i][j].unknown_202 = 0;
            b2->unknown_42ac[i][j] = 0;
            for (k = 0; k < 64; k++) {
                b2->unknown_42ec[i][j][k] = 0;
            }
        }
    }
    b3 = DOTR_CAMP_INIT;
    for (i = 0; i < 32; i++) {
        b3->unknown_52ec[i] = 0;
        b3->unknown_532c[i] = 0;
    }
    if ((signed char)HELP_flg[0] != 0) {
        *(struct DotrCampSave *)SzData_GetData(0) = *DOTR_CAMP_SAVE;
        HELP_flg[0] = 0;
    }
    for (s = 0, sum = 0; s < 12; s++) {
        sum += ((unsigned short *)board)[18 + s] % 16;
    }
    DOTR_CAMP_INIT->unknown_53f6 = sum & 0xf;
    b4 = DOTR_CAMP_INIT;
    for (t = 0; t < 3; t++) {
        b4->unknown_53f8[t] = (DeckTab_halves[bank->unknown_53f6] >> (t * 5)) & 0x1f;
        b4->unknown_53fe[t] = data_00299d00[b4->unknown_53f8[t]].unknown_00 & 0x3ff;
    }
}
