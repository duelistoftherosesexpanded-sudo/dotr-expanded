#include "dotr/ee/small_misc.h"
int Ta_Eff_101(DotrMiscJob *in, DotrMiscJobQueue *q, int mode)
{
    DotrMiscJob req;
    DotrMiscJob *p = &req;
    req.unknown_00 = 999;
    req.first = 0;
    req.second = 0;
    req.value = 0;
    if (mode >= 0) {
        InitEffectTemp(q);
        TaEffect_LoadModule(q, 601);
        TaUtil_DoOpen((unsigned)in->second >> 8, in->second & 0xFF, 1);
        return 1;
    }
    if (TaEffect_IsLoadModule(q) == 1) {
        if (SzDuel_GetUnitOpenRate((unsigned)in->second >> 8, in->second & 0xFF) == 0xFF) {
            if (q->count == 0) {
                p->unknown_00 = 601;
                p->first = 0;
                p->second = in->second;
                p->value = in->value;
                q->slot = Ta_Do_Effect(p, q);
                q->count = q->count + 1;
            } else {
                if (SzEffect_IsUse(q->slot) == 1) {
                    return 1;
                }
                SzDuel_CloseMsgPopup();
                return 0;
            }
        }
    }
    return 1;
}
