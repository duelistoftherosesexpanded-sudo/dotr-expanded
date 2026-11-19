#include "dotr/ee/small_misc.h"
int Ta_IsFinishEffect(DotrMiscByteList *list)
{
    int i;
    for (i = 0; i < list->count; i++) {
        if (SzEffect_IsUse(list->entries[i])) {
            return -1;
        }
    }
    return 1;
}
