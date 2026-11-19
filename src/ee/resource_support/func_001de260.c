#include "dotr/ee/resource_support.h"

int SzDuel_GetCamDist(int cardId)
{
    if (cardId < 0 || cardId >= 854) {
        return -1;
    }
    return gSzDuel_CardDefaultCamDist[cardId];
}
