#include "dotr/ee/resource_support.h"

int SzDuel_Xax2New(int index)
{
    if (index < 0 || index >= 854) {
        return 999;
    }
    index = AlternateCardIds[index];
    if (index == 671) {
        index = 999;
    }
    if (index < 0 || index >= 854) {
        return 999;
    }
    return index;
}
