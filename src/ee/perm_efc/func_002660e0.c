#include "dotr/ee/perm_efc.h"

/* MATCHING DEVICE: the clamp is an inline helper; spelled in the caller it compiles to another
 * branch form. */
static inline int cap(int v)
{
    return v > 9999 ? 9999 : v;
}

int Ta_GetParamOfs(struct jp_64b6d *vInfo, int nX, int nZ, int fAP)
{
    int i;
    int eSide;
    int Ofs;
    struct jp_65648 *eInfo;
    /* MATCHING DEVICE: the original keeps the effect id word in a register across the
     * Ta_IsUnitControl call and swaps that register with the counter between the two loops.
     * Here the first loop counts in i and keeps the word in k, the second the other way round;
     * the debug information names only i. */
    int k;

    if (vInfo == 0) {
        return 0;
    }
    Ofs = 0;
    eSide = Ta_IsUnitControl2(vInfo);
    /* 675: Shapesnatch */
    if (vInfo->wCardNo == 675) {
        if (fAP == 0) {
            Ofs -= cap(vInfo->sVarDP + vInfo->pConstInfo->wDP);
        } else {
            Ofs -= cap(vInfo->sVarAP + vInfo->pConstInfo->wAP);
        }
        if (vInfo->fDefense) {
            if (fAP == 0) {
                Ofs += SzDuel_GetUnitDP(eSide, 0, 1);
            } else {
                Ofs += SzDuel_GetUnitAP(eSide, 0, 1);
            }
        } else {
            if (fAP == 0) {
                Ofs += SzDuel_GetUnitDP(eSide == 0 ? 1 : 0, 0, 1);
            } else {
                Ofs += SzDuel_GetUnitAP(eSide == 0 ? 1 : 0, 0, 1);
            }
        }
    }
    for (i = 1; i <= 5; i++) {
        if (SzDuel_GetUnit(eSide, i) != 0 && SzDuel_GetUnitStatus(eSide, i, 4) != 0 && SzDuel_GetUnitStatus(eSide, i, 3) != 0 &&
            SzDuel_GetUnitStatus(eSide, i, 5) != 0) {
            if ((eInfo = Ta_GetUnitEffect(eSide, i, 2)) != 0) {
                k = eInfo->dwID;
                if (Ta_GetUnitAgreeWithVarInfo(vInfo, Ta_IsUnitControl(eSide, i), ((unsigned)k >> 6) & 3, k & 0x3f) != 0) {
                    switch (eInfo->dwID >> 8) {
                    case 19: /* AP/DP reduction */
                        Ofs -= (unsigned short)eInfo->dwParam;
                        break;
                    case 18: /* AP/DP buff */
                        Ofs += (unsigned short)eInfo->dwParam;
                        break;
                    case 34: /* DP buff */
                        if (fAP == 0) {
                            Ofs += (unsigned short)eInfo->dwParam;
                        }
                        break;
                    }
                }
            }
        }
    }
    for (k = 1; k <= 5; k++) {
        if (SzDuel_GetUnit(eSide == 0 ? 1 : 0, k) != 0 && SzDuel_GetUnitStatus(eSide == 0 ? 1 : 0, k, 4) != 0 &&
            SzDuel_GetUnitStatus(eSide == 0 ? 1 : 0, k, 3) != 0 && SzDuel_GetUnitStatus(eSide == 0 ? 1 : 0, k, 5) != 0) {
            if ((eInfo = Ta_GetUnitEffect(eSide == 0 ? 1 : 0, k, 2)) != 0) {
                i = eInfo->dwID;
                if (Ta_GetUnitAgreeWithVarInfo(vInfo, Ta_IsUnitControl(eSide == 0 ? 1 : 0, k), ((unsigned)i >> 6) & 3,
                                               i & 0x3f) != 0) {
                    switch (eInfo->dwID >> 8) {
                    case 19: /* AP/DP reduction */
                        Ofs -= (unsigned short)eInfo->dwParam;
                        break;
                    case 18: /* AP/DP buff */
                        Ofs += (unsigned short)eInfo->dwParam;
                        break;
                    case 34: /* DP buff */
                        if (fAP == 0) {
                            Ofs += (unsigned short)eInfo->dwParam;
                        }
                        break;
                    }
                }
            }
        }
    }
    /* 1: powered up, 2: powered down */
    switch (CheckLandAgreement(SzDuel_GetField(nX, nZ), vInfo)) {
    case 1:
        Ofs += 500;
        break;
    case 2:
        Ofs -= 500;
        break;
    }
    Ofs += Ta_GetLDParamOfs2(vInfo, nX, nZ);
    return Ofs;
}
