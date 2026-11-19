#include "dotr/ee/small_misc.h"
int Ta_Eff_033(DotrMiscJob *in, DotrMiscJobQueue *q, int mode)
{
    DotrMiscJob req;
    DotrMiscJob *p = &req;
    req.unknown_00 = 999;
    req.first = 0;
    req.second = 0;
    req.value = 0;
    if (mode >= 0) {
        InitEffectTemp(q);
        TaEffect_LoadModule(q, 533);
        return 1;
    }
    if (TaEffect_IsLoadModule(q) == 1) {
        if (q->count == 0) {
            p->unknown_00 = 533;
            p->first = 0;
            p->second = 0;
            p->value = 0;
            q->slot = Ta_Do_Effect(p, q);
            q->count = q->count + 1;
        } else {
            if (SzEffect_IsUse(q->slot) == 1) {
                return 1;
            }
            SzDuel_SetHandPhase(in->second, 1);
            SzDuel_CloseMsgPopup();
            return 0;
        }
    }
    return 1;
}
