#include "dotr/ee/screen_vars.h"

/* 0x20ad10 lRhBase_InitScreenVariables (name from the Japanese build's symbols). */
void lRhBase_InitScreenVariables(struct DotrScreenVars *s, int mode, int psm, int zpsm)
{
    s->mode = mode;
    s->width = DOTR_RH_SYS_ASD[mode].width;
    s->height = DOTR_RH_SYS_ASD[mode].height;
    s->unknown_244 = DOTR_RH_SYS_ASD[mode].unknown_14;
    s->unknown_248 = DOTR_RH_SYS_ASD[mode].unknown_18;
    s->unknown_24c = DOTR_RH_SYS_ASD[mode].unknown_1c;
    s->psm = psm;
    switch (psm) {
    case 0:
    case 1:
        s->psm_index = 1;
        break;
    case 2:
    case 10:
        s->psm_index = 0;
        break;
    }
    s->zpsm = zpsm;
    switch (zpsm) {
    case 48:
    case 49:
        s->zpsm_index = 1;
        break;
    case 50:
    case 58:
        s->zpsm_index = 0;
        break;
    }
    s->unknown_260 = 2048;
    s->unknown_264 = 2048;
    s->unknown_268 = DOTR_RH_SYS_ASD[mode].pages[s->zpsm_index] + DOTR_RH_SYS_ASD[mode].pages[s->psm_index] * 2;
    {
        /* MATCHING DEVICE: the temporary keeps the two right shifts apart (`sra 2`, `sra 6`);
         * written as one expression the compiler folds them into `sra 8`. */
        int t = s->unknown_268 << 13;
        t >>= 2;
        s->unknown_268 = t >> 6;
    }
    func_001851e8((int *)s, (short)psm, (short)DOTR_RH_SYS_ASD[mode].width, (short)DOTR_RH_SYS_ASD[mode].height, 3,
                  (short)zpsm, 1);
    if (s->db.disp[0].dispfb.FBP == s->db.disp[1].dispfb.FBP) {
        s->db.disp[0].dispfb.FBP = 0;
        s->db.disp[1].dispfb.FBP = DOTR_RH_SYS_ASD[mode].pages[s->psm_index];
        s->db.draw0.frame.page = s->db.disp[1].dispfb.FBP;
        s->db.draw1.frame.page = s->db.disp[0].dispfb.FBP;
        s->db.draw0.zbuf.page = DOTR_RH_SYS_ASD[mode].pages[s->psm_index] * 2;
        s->db.draw1.zbuf.page = s->db.draw0.zbuf.page;
        s->db.disp[0].display.DH = s->db.disp[1].display.DH = DOTR_RH_SYS_ASD[mode].height - 1;
    }
    s->db.clear0.r = 0;
    s->db.clear0.g = 0;
    s->db.clear0.b = 128;
    s->db.clear0.a = 255;
    s->db.clear0.q = 0.0f;
    s->db.clear1.r = 0;
    s->db.clear1.g = 0;
    s->db.clear1.b = 128;
    s->db.clear1.a = 255;
    s->db.clear1.q = 0.0f;
    s->unknown_26c = 0;
}
