#include "dotr/ee/packed_item.h"

/* SzEffect_GetPos2 (0x1f53c0) on the packed position stored at 0x31be98. */
int SzEffect_GetPos(int kind, struct DotrObjectVector4 *out)
{
    return SzEffect_GetPos2(kind, data_0031be98[0], out);
}
