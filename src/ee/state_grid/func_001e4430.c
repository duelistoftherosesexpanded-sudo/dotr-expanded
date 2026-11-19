#include "dotr/ee/state_grid.h"

int SzDuel_ActMsgPopup(int unknown)
{
    struct DotrStateControl *p = data_00317490_control;
    if (SzPad_Check(unknown, 0, 0xa000)) {
        int old = p->unknown_d35;
        if (SzPad_Check(unknown, 0, 0x8000)) {
            p->unknown_d35 = 1;
        } else {
            p->unknown_d35 = 0;
        }
        if (p->unknown_d35 != old) {
            SzSound_PlaySE(0, 3);
        }
        return 0;
    }
    if (SzPad_Check(unknown, 0, 192)) {
        SzSound_PlaySE(0, 4);
        return p->unknown_d35 ? 1 : 2;
    }
    if (SzPad_Check(unknown, 0, 32)) {
        SzSound_PlaySE(0, 5);
        return 2;
    }
    return 0;
}
