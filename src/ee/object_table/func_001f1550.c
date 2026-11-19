#include "dotr/ee/object_table.h"

int SzModel_SetAnimIntr(int modelId, int animIndex)
{
    return SzModel_SetAnimIntrParam(modelId, animIndex, 6);
}
