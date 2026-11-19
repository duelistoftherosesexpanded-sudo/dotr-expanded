#include "dotr/ee/small_misc.h"
int TaLogic_GetMaxApUnit2(unsigned short *ownerIdArray, int team)
{
    int i;
    int currentBestAttack;
    int currentBestUnit;
    int unitLocation;
    unsigned short *p;
    int atk;
    currentBestAttack = -1;
    currentBestUnit = currentBestAttack;
    i = 1;
    if (TaLogic_GetUnitCtlLstLen(ownerIdArray) > 1) {
        p = ownerIdArray + 1;
        do {
            unitLocation = SzDuel_GetUnitLoc(*p >> 8, *p & 0xff);
            if (SzDuel_GetUnitStatus(*p >> 8, *p & 0xff, 4)
                || team == func_00250360_pair(*p >> 8, *p & 0xff)) {
                atk = TaUtil_GetUnitAP(*p >> 8, *p & 0xff, unitLocation & 0xff, unitLocation >> 8);
                if (currentBestAttack < atk) {
                    currentBestUnit = *p;
                    currentBestAttack = atk;
                }
            }
            p++;
            i++;
        } while (i < TaLogic_GetUnitCtlLstLen(ownerIdArray));
    }
    return currentBestUnit;
}
