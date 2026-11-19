#include "dotr/ee/slot_table.h"

void lSzMsg_GetTexInfo(struct DotrGsTex0 *tex0, struct DotrGsTex1 *tex1)
{
    int *p = gSzMsg_ml.words;

    if (tex0 != 0) {
        tex0->tbp0 = p[5];
        tex0->tbw = p[0] >> 6;
        tex0->psm = 20;
        tex0->tw = p[2];
        tex0->th = p[3];
        tex0->tcc = 1;
        tex0->tfx = 0;
        tex0->cbp = p[4] + p[6];
        tex0->cpsm = 0;
        tex0->csm = 0;
        tex0->csa = 0;
        tex0->cld = 1;
    }
    if (tex1 != 0) {
        tex1->lcm = 0;
        tex1->mxl = 0;
        tex1->mmag = 0;
        tex1->mmin = 0;
        tex1->mtba = 0;
        tex1->l = 0;
        tex1->k = 0;
    }
}
