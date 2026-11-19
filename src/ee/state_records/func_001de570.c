#include "dotr/ee/state_records.h"

void SzDuel_SetTurnCount(int value)
{
    struct DotrStateControl *p = data_00317490_control;
    p->unknown_d53 = ((value > 0 ? value : 0) < 255)
        ? (value > 0 ? value : 0) : 255;
}
