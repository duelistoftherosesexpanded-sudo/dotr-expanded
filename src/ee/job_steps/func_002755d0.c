#include "dotr/ee/job_steps.h"

int Ta_Eff_BTL_002(DotrJobStepRequest *in, DotrJobStepQueue *q, int mode)
{
    DotrJobStepLocal req;
    DotrJobStepLocal *p = &req;
    int side;
    int cell;

    p->unknown_00 = 999;
    p->first = 0;
    p->second = 0;
    p->value = 0;
    if (in == 0 || q == 0 || p == 0) {
        SzDuel_CloseMsgPopup();
        return 0;
    }
    if (mode >= 0) {
        InitEffectTemp(q);
        if ((in->flags & 1) == 1) {
            q->first = in->second;
            q->second = in->first;
        } else {
            q->first = in->first;
            q->second = in->second;
        }
        if (in->flags >> 1) {
            side = Ta_GetCurrentSide(q->second >> 8, q->second & 0xff);
            cell = SzDuel_GetUnitLoc(side, 0);
            Ta_SetRangeVisual(cell & 0xff, cell >> 8, 0, Ta_IsLDAbl(9, side), 1, 1, 11);
        }
        Ta_UnitFade(q->first >> 8, q->first & 0xff, -1, 1, 3);
        return 1;
    }
    if (q->count++ < 16) {
        return 1;
    }
    Ta_DelUnit(q->first >> 8, q->first & 0xff);
    SzDuel_CloseMsgPopup();
    return 0;
}
