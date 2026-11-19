#include "dotr/ee/state_records.h"

void SzDuel_SetWindowBusy(int value)
{
    struct DotrStateControl *p = data_00317490_control;
    p->unknown_d67.unknown_bit_1 = (signed char)(value ? 1 : 0);
}
