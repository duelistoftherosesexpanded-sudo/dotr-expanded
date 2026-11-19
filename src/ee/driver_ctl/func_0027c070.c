#include "dotr/ee/driver_ctl.h"

void TaLogic_GetUnitCtlLst(int nSide, unsigned short *wUnitLst, int sKind, unsigned char fView)
{
    struct jp_9a533 *pVarInfo;
    int i;
    int sStart;
    int sEnd;
    unsigned char bNum;

    bNum = 0;
    switch (sKind) {
    case 0x22: /* 34: the leader only */
        sStart = 0;
        sEnd = 1;
        break;
    case 0x1f: /* 31: all cards */
        sStart = 0;
        sEnd = 11;
        break;
    case 0x34: /* 52: all but the leader */
        sStart = 1;
        sEnd = 11;
        break;
    default:
        sStart = 1;
        sEnd = 11;
        break;
    }
    for (i = sStart; i < sEnd; i++) {
        if (bNum >= 22) return;
        if ((pVarInfo = SzDuel_GetUnit(nSide, i)) != 0 && nSide == Ta_IsUnitControl2(pVarInfo)) {
            if (TaLogic_IsJudgeUnitDanger(nSide, i, fView) || TaLogic_IsUnitCategory(nSide, i, sKind) == 1) {
                wUnitLst[bNum++] = (nSide << 8) | i;
            }
        }
    }
    for (i = sStart; i < sEnd; i++) {
        if (bNum >= 22) return;
        if ((pVarInfo = SzDuel_GetUnit((nSide == 0 ? 1 : 0), i)) != 0 && nSide == Ta_IsUnitControl2(pVarInfo)) {
            if (TaLogic_IsJudgeUnitDanger((nSide == 0 ? 1 : 0), i, fView) || TaLogic_IsUnitCategory((nSide == 0 ? 1 : 0), i, sKind) == 1) {
                wUnitLst[bNum++] = (((nSide == 0 ? 1 : 0)) << 8) | i;
            }
        }
    }
}
