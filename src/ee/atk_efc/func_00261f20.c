#include "dotr/ee/atk_efc.h"

unsigned int Manage_AtkEfc(int nSide_1, int ID_1, int nSide_2, int ID_2, struct jp_570b5 *pDat) {
    struct jp_5730a *temp;
    int eFlag1;
    int eFlag2;
    int Ret1;
    int Ret2;

    eFlag2 = 0;
    eFlag1 = 0;
    Ret2 = 0;
    Ret1 = 0;
    pDat->AttrAdv = ChkAttrAdv(SzDuel_GetUnitAttr(nSide_1, ID_1), SzDuel_GetUnitAttr(nSide_2, ID_2));
    temp = (struct jp_5730a *)func_00250740_word(nSide_1, ID_1, 0);
    if (temp != 0) {
        Ret1 = Do_AtkEfc(temp->dwID, temp->dwParam, pDat, nSide_1, ID_1, nSide_2, ID_2, 0);
        pDat->fLand1 = (Ret1 >> 24) & 1;
        eFlag1 = 1;
    }
    temp = (struct jp_5730a *)func_00250740_word(nSide_2, ID_2, 0);
    if (temp != 0) {
        Ret2 = Do_AtkEfc(temp->dwID, temp->dwParam, pDat, nSide_2, ID_2, nSide_1, ID_1, 1);
        pDat->fLand2 = (Ret2 >> 24) & 1;
        eFlag2 = 1;
    }
    if (!eFlag1 && !eFlag2 && pDat->AttrAdv == 0) {
        return 0;
    }
    if ((Ret1 & 0xff) == 1 && (Ret2 & 0xff) == 1) {
        return 0x1010001;
    }
    if ((Ret1 & 0xff) == 1) {
        return 0x1000001;
    }
    if ((Ret2 & 0xff) == 1) {
        return 0x10001;
    }
    if (((Ret1 >> 16) & 1) == 1 && ((Ret2 >> 16) & 1) == 1) {
        return 0x1010000;
    }
    if (((Ret1 >> 16) & 1) == 0 && ((Ret2 >> 16) & 1) == 1) {
        return 0x10000;
    }
    if (((Ret1 >> 16) & 1) == 1 && ((Ret2 >> 16) & 1) == 0) {
        return 0x1000000;
    }
    return 0;
}
