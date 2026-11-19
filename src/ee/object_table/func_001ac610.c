#include "dotr/ee/object_table.h"


void lSzGame_InitTitle(int mode)
{
    struct DotrInitView *p = &gSzGame_gl_init;
    int i;
    for (i = 0; i < 84; i++) {
        SzModel_SetDrawFlag(i, 0);
    }
    for (i = 0; i < 40; i++) {
        SzModel_SetDrawFlag(i + 34, 1);
    }
    if (mode < 2) {
        data_003173b0[0] = 2;
        p->byte_224 = 0;
        p->byte_225 = 0;
        p->byte_226 = 0;
        p->byte_227 = 128;
        p->bits_22c.lo = 1;
        p->bits_22f.bit1 = -1;
        p->word_008 = 0;
    }
}
