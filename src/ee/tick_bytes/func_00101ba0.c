#include "dotr/ee/tick_bytes.h"

void ShFx_AdjustPoint(struct DotrTickSource *source, struct DotrObjectVector4 *v)
{
    switch (source->mode) {
    case 0:
        break;
    case 1:
        SzEffect_GetRef2(SzEffect_GetFrom(), source->value, v);
        break;
    case 2:
        SzEffect_GetRef2(SzEffect_GetTarget(), source->value, v);
        break;
    }
}
