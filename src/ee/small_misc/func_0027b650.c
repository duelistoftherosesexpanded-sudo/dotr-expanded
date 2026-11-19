#include "dotr/ee/small_misc.h"
int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray)
{
    int i;
    int currentBestAtk;
    int currentBestUnit;
    unsigned short *currentUnit;
    int unitLoc;
    int atk;
    currentBestAtk = -1;
    currentBestUnit = currentBestAtk;
    i = 1;
    if (TaLogic_GetUnitCtlLstLen(ownerIdArray) > 1) {
        currentUnit = ownerIdArray + 1;
        do {
            unitLoc = SzDuel_GetUnitLoc(*currentUnit >> 8, *currentUnit & 0xff);
            if (SzDuel_GetUnitStatus(*currentUnit >> 8, *currentUnit & 0xff, 4)) {
                atk = TaUtil_GetUnitAP(*currentUnit >> 8, *currentUnit & 0xff, unitLoc & 0xff, unitLoc >> 8);
                if (currentBestAtk < atk) {
                    currentBestUnit = *currentUnit;
                    currentBestAtk = atk;
                }
            }
            currentUnit++;
            i++;
        } while (i < TaLogic_GetUnitCtlLstLen(ownerIdArray));
    }
    return currentBestUnit;
}
