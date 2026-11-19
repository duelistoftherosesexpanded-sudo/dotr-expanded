#include "dotr/ee/unit_warning.h"

#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))

/* Locals as the Japanese build's debug information lists them; Dist is declared
 * before Danger here because that order gives the original's saved registers. */
int TaLogic_GetUnitWarning(int nX, int nZ, int nOwnerID, unsigned char bWarnRange,
                           unsigned short bWarnLvl, unsigned char fView)
{
    struct jp_9a533 *pVarInfo;
    unsigned short wUnitLst[22];
    int Dist;
    int Danger;
    int i;
    int v1;
    int Loc;

    for (i = 0; i < 22; i++) {
        wUnitLst[i] = 0xffff;
    }
    pVarInfo = SzDuel_GetUnit(nOwnerID >> 8, nOwnerID & 0xff);
    TaLogic_GetUnitCtlLst(Ta_IsUnitControl2(pVarInfo) == 0 ? 1 : 0, wUnitLst, 29, fView);
    Danger = 0;
    for (i = 0; i < TaLogic_GetUnitCtlLstLen(wUnitLst); i++) {
        if (SzDuel_GetUnit(wUnitLst[i] >> 8, wUnitLst[i] & 0xff) != 0) {
            Loc = SzDuel_GetUnitLoc(wUnitLst[i] >> 8, wUnitLst[i] & 0xff);
            Dist = (HI(nX, Loc & 0xff) - LO(nX, Loc & 0xff)) + (HI(nZ, Loc >> 8) - LO(nZ, Loc >> 8));
            if (Dist <= bWarnRange && Dist > 0) {
                if (Dist < bWarnRange && Dist > 0) {
                    v1 = TaLogic_IsUnitWin4(pVarInfo, wUnitLst[i], bWarnLvl, Loc & 0xff, Loc >> 8, fView);
                    if (v1 > 0) {
                        v1 = 0;
                    } else {
                        v1 = -v1;
                    }
                    Danger += v1 / Dist;
                }
            }
        }
    }
    return Danger;
}
