#include "dotr/ee/state_grid.h"

void SzDuel_OpenOptionPopup(int index, int a, int b)
{
    struct DotrStateControl *p = data_00317490_control;
    int i;
    data_00317490_control[0].unknown_cb2 = 7;
    data_00317490_control[0].unknown_caa = 0;
    data_00317490_control[0].unknown_c9a = a;
    data_00317490_control[0].unknown_c9c = b;
    if (index >= 0) {
        p->unknown_d54 = index;
        p->unknown_d35 = 0;
        p->unknown_d36 = 0;
        for (i = 0; i < 3; i++) {
            p->unknown_d36 |= (SzData_GetOption(0, i) ? 1 : 0) << i;
        }
    }
}
