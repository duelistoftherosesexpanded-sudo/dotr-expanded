#include "dotr/ee/state_records.h"

int SzDuel_InqHandPhase(int side)
{
    return data_00317490_records[side].unknown_346.unknown_bit_0 ? 1 : 0;
}
