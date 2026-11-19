#include "dotr/ee/small_misc.h"

int Ta_Eff_065(DotrMiscJob *in, DotrMiscJobQueue *q, int mode)
{
    DotrMiscJob req;
    DotrMiscJob *p = &req;
    req.unknown_00 = 999;
    req.first = 0;
    req.second = 0;
    req.value = 0;
    SzDuel_SetWindowDraw(0);
    if (mode >= 0) {
        InitEffectTemp(q);
        TaEffect_LoadModule(q, 534);
        return 1;
    }
    switch (q->count) {
    case 0:
        if (TaEffect_IsLoadModule(q) == 1) {
            if ((unsigned)SzDuel_GetSummonPower(in->second) <= (unsigned)in->value) {
                p->unknown_00 = 534;
                p->first = 0;
                p->second = 0;
                p->value = in->value - SzDuel_GetSummonPower(in->second);
            } else {
                p->unknown_00 = 534;
                p->first = 0;
                p->second = 1;
                p->value = SzDuel_GetSummonPower(in->second) - in->value;
            }
            q->slot = Ta_Do_Effect(p, q);
            q->count = q->count + 1;
        }
        break;
    default:
        if (SzEffect_IsUse(q->slot) == 1) {
            return 1;
        }
        SzDuel_CalcSummonPower(in->second, 4, in->value);
        if (SzDuel_GetWindowDisp() != 0) {
            SzDuel_SetWindowDraw(1);
        }
        SzDuel_CloseMsgPopup();
        return 0;
    }
    return 1;
}
