#include "dotr/ee/object_table.h"

int SzModel_SetAnimFrame(int modelId, float animFrame)
{
    return SzModel_SetAnimFrameParam(modelId, 0, animFrame);
}
