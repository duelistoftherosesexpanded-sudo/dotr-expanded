#include "dotr/ee/small_misc.h"

int Ta_Eff_Hand_Dest(DotrMiscJob *in, DotrMiscJobQueue *q, int mode)
{
    DotrMiscJob req;
    DotrMiscJob *p = &req;
    req.unknown_00 = 999;
    req.first = 0;
    req.second = 0;
    req.value = 0;
    if (mode >= 0) {
        InitEffectTemp(q);
        TaEffect_LoadModule(q, 603);
        return 1;
    }
    switch (q->count) {
    case 0:
        if (TaEffect_IsLoadModule(q) == 1) {
            p->unknown_00 = 603;
            p->first = 0;
            p->second = in->second;
            p->value = 0;
            q->slot = Ta_Do_Effect(p, q);
            q->count = q->count + 1;
        }
        break;
    default:
        if (SzEffect_IsUse(q->slot) != 0) {
            return 1;
        }
        Ta_DelUnit2((unsigned)in->second >> 8, in->second & 0xff);
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
