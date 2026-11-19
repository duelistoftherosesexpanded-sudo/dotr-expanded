#include "dotr/ee/state_records.h"

void SzDuel_SetWindowDisp(int value)
{
    struct DotrStateControl *p = data_00317490_control;
    p->unknown_d66.unknown_bit_7 = (signed char)(value ? 1 : 0);
}
