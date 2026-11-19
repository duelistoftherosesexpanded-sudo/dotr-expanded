#include "dotr/ee/state_grid.h"

void SzDuel_OpenMsgPopup(int operation, int a, int b, int value)
{
    struct DotrStateControl *p = data_00317490_control;
    switch (operation) {
    case 0: p->unknown_cb5 = 19; break;
    case 1: p->unknown_cb5 = 15; break;
    case 2:
        p->unknown_cb5 = 12;
        if (SzGame_GetMenuNum(15) == 0) {
            SzData_AddBag(p->unknown_d50, 0, value);
        }
        break;
    case 3:
        p->unknown_cb5 = 13;
        if (SzGame_GetMenuNum(15) == 0) {
            SzData_AddBag(p->unknown_d50, 0, value);
            SzData_RollSpecial(p->unknown_d50, value);
        }
        break;
    case 5: p->unknown_cb5 = 20; break;
    case 6: p->unknown_cb5 = 21; break;
    case 7:
        p->unknown_cb5 = 22;
        if (value >= 0) {
            p->unknown_d35 = (value & 0x8000) ? 1 : 0;
            value &= 0x7fff;
        }
        break;
    case 8: p->unknown_cb5 = 16; break;
    default: p->unknown_cb5 = 24; return;
    }
    if (value >= 0) {
        p->unknown_cb0 = value;
    }
    p->unknown_ca6 = a;
    p->unknown_ca8 = b;
}
