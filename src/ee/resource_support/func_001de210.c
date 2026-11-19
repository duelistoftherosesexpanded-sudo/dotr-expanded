#include "dotr/ee/resource_support.h"

int SzDuel_GetMsgName(int cardNo)
{
    SzData_GetOption(0, 0);
    if (cardNo < 0 || cardNo >= 854) {
        return -1;
    }
    return cardNo + 320;
}
