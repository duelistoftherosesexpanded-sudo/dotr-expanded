#include "dotr/ee/atk_efc_do.h"

unsigned int Do_AtkEfc(unsigned int eID, unsigned int eParam, struct jp_570b5 *pDat, int nSide_Do, int ID_Do, int nSide_Done, int ID_Done, unsigned int fAtkSide)
{
    int i;
    int j;
    int Efc;
    int nX;
    int nZ;

    switch (eID >> 8) {
    case 4: /* destruction */
        if (Match_AtkEfcObj((eID >> 6) & 3, eID & 0x3f, nSide_Do, ID_Do, nSide_Done, ID_Done) == 1) {
            if (ID_Done != 0 && ID_Do != 0) {
                pDat->Do_APDP = 0;
                if (fAtkSide == 1) {
                    pDat->fDestroy_1 = 1;
                } else {
                    pDat->fDestroy_2 = 1;
                }
                return 0x10001;
            }
        }
        return 0;
    case 2: /* terrain change */
        if (fAtkSide == 0) {
            nX = SzDuel_GetUnitLoc(nSide_Do, ID_Do) & 0xff;
            nZ = SzDuel_GetUnitLoc(nSide_Do, ID_Do) >> 8;
        } else {
            nX = SzDuel_GetUnitLoc(nSide_Done, ID_Done) & 0xff;
            nZ = SzDuel_GetUnitLoc(nSide_Done, ID_Done) >> 8;
        }
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                if ((unsigned int)(((i < nX ? nX : i) - (nX < i ? nX : i)) + ((j < nZ ? nZ : j) - (nZ < j ? nZ : j))) <= eParam >> 16) {
                    if (SzDuel_GetField(i, j) != 8) {
                        SzDuel_SetField(i, j, eParam & 0xffff);
                    }
                }
            }
        }
        return 0x10000;
    case 27: /* bonus against a monster kind */
        if (Match_AtkEfcObj(2, eParam >> 16, nSide_Do, ID_Do, nSide_Done, ID_Done) == 1) {
            if (ID_Done != 0 && ID_Do != 0) {
                if (fAtkSide == 0) {
                    pDat->POfs1 += eParam & 0xffff;
                } else {
                    pDat->POfs2 += eParam & 0xffff;
                }
                return 0x10000;
            }
        }
        return 0;
    case 5: /* no combat damage */
        if (ID_Done != 0 && ID_Do != 0) {
            if (fAtkSide == 0) {
                pDat->fGuard1 = 1;
            } else {
                pDat->fGuard2 = 1;
            }
            return 0x10000;
        }
        break;
    case 13: /* alter LP */
        Efc = eParam & 0xffff;
        eParam >>= 16;
        switch (eParam) {
        case 0:
            Efc = Ta_GetLPRevByNaturalEffect(43, Efc);
            break;
        case 1:
            Efc = Ta_GetLPRevByNaturalEffect(44, Efc);
            break;
        }
        SzDuel_CalcLP(nSide_Do, eParam, Efc);
        return 0x10000;
    case 58: /* no revival after combat death */
        if (ID_Done != 0 && ID_Do != 0) {
            pDat->fRemove = 1;
            return 0x10000;
        }
        break;
    case 1: /* cancel power-ups */
        if (ID_Done != 0 && ID_Do != 0) {
            if (fAtkSide == 0) {
                pDat->fNoEq2 = 1;
            } else {
                pDat->fNoEq1 = 1;
            }
            return 0x10000;
        }
        break;
    case 63: /* random AP shift (Goddess of Whim) */
        if (fAtkSide == 0) {
            Efc = (eParam & 0xffff) / 100 + 1;
            pDat->POfs1 = ((unsigned int)func_0019a5c8() % (unsigned int)Efc - (unsigned int)func_0019a5c8() % (unsigned int)Efc) * 100;
        } else {
            Efc = (eParam & 0xffff) / 100 + 1;
            pDat->POfs2 = ((unsigned int)func_0019a5c8() % (unsigned int)Efc - (unsigned int)func_0019a5c8() % (unsigned int)Efc) * 100;
        }
        return 0x10000;
    default:
        return 0;
    }
    return 0;
}
