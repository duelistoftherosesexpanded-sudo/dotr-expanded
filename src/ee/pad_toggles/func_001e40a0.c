#include "dotr/ee/pad_toggles.h"

int SzDuel_ActOptionPopup(int pad)
{
    struct DotrStateControl *control = data_00317490_control;
    int old;
    int i;

    if (SzPad_Check(pad, 2, 0x5000) != 0) {
        old = control->unknown_d35;
        if (SzPad_Check(pad, 2, 0x1000) != 0) {
            control->unknown_d35 = (control->unknown_d35 + 1) % 2;
        } else {
            control->unknown_d35 = (control->unknown_d35 + 3) % 2;
        }
        if (control->unknown_d35 != old) {
            SzSound_PlaySE(0, 3);
        }
        return 0;
    }
    if (SzPad_Check(pad, 0, 0xa000) != 0) {
        old = control->unknown_d36;
        if (SzPad_Check(pad, 0, 0x8000) != 0) {
            control->unknown_d36 |= 1 << (control->unknown_d35 + 1);
        } else {
            control->unknown_d36 &= ~(1 << (control->unknown_d35 + 1));
        }
        if (control->unknown_d36 != old) {
            SzSound_PlaySE(0, 3);
        }
        return 0;
    }
    if (SzPad_Check(pad, 0, 0xc0) != 0) {
        for (i = 0; i < 3; i++) {
            SzData_SetOption(0, i, control->unknown_d36 & (1 << i));
            SzData_SetOption(1, i, control->unknown_d36 & (1 << i));
        }
        SzSound_PlaySE(0, 4);
        return 1;
    }
    if (SzPad_Check(pad, 0, 0x20) != 0) {
        SzSound_PlaySE(0, 5);
        return 2;
    }
    return 0;
}
