#include "dotr/ee/object_table.h"

int SzModel_SetAnimSpeed(int index, float value)
{
    return SzModel_SetAnimSpeedParam(index, 0, value);
}
