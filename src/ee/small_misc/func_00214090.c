#include "dotr/ee/small_misc.h"
int NjaReloc_Adjust_M3DMOT(DotrMiscRebaseHead *head)
{
    if (head->magic != 0x1a544f4d) {
        return 0;
    }
    head->first = (DotrMiscAddress)head + head->first;
    head->second = (DotrMiscAddress)head + head->second;
    lNjaReloc_AdjustMotion((DotrMiscFixupHead *)head->first, (DotrMiscAddress)head, head->count, head->flags);
    return 1;
}
