#include "dotr/ee/packed_item.h"

int SzEffect_IsModelStandby2(int packed)
{
    struct DotrStateItem *p = SzDuel_GetUnit(packed >> 8, packed & 0xff);
    return p ? SzModel_IsModelDisp(p->modelId) : 0;
}
