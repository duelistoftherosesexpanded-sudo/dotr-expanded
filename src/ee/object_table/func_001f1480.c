#include "dotr/ee/object_table.h"

int SzModel_SetAnim(int modelId, int animIndex)
{
    return SzModel_SetAnimParam(modelId, animIndex, 3);
}
