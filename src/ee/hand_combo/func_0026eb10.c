#include "dotr/ee/hand_combo.h"

/* The locals are the Japanese build's; Combo is declared before nVarInfo (the reverse of the
 * debug-information order) because that fixes their stack slots. Which local carried each
 * arm's result (here temp) is an assumption. */
int Ta_GetComboResult(int nSide, int nCardNo, int nCardNo2)
{
    int Ret;
    int Num;
    int Combo[6];
    struct jp_86d82 nVarInfo;
    int temp;
    int bKind1;
    int bKind0;
    int cBuf;
    int Hand;
    int i;

    Hand = SzDuel_GetHandCount(nSide);
    for (i = 0; i < 6; i++) {
        Combo[i] = 999;
    }
    if (nCardNo != 999) {
        Combo[0] = nCardNo;
    }
    if (nCardNo != 999 && nCardNo2 != 999) {
        SzDuel_InitCardInfo(&nVarInfo, nCardNo);
        bKind0 = nVarInfo.bVarKind >> 5;
        SzDuel_InitCardInfo(&nVarInfo, nCardNo2);
        bKind1 = nVarInfo.bVarKind >> 5;
        if (bKind0 == 0 && bKind1 == 0) {
            Num = Fusion(nCardNo, nCardNo2);
            if (Num != 999) {
                temp = Num;
            } else {
                temp = nCardNo2;
            }
        } else if (bKind0 == 2 && bKind1 == 0) {
            Num = Do_Chk_Enc(nCardNo2, nCardNo);
            if (Num != 999) {
                if ((Num >> 30) & 1) {
                    temp = (Num >> 14) & 0x3FFF;
                } else {
                    temp = nCardNo2;
                }
            } else {
                temp = nCardNo2;
            }
        } else if (bKind1 == 2 && bKind0 == 0) {
            Num = Do_Chk_Enc(nCardNo, nCardNo2);
            if (Num != 999) {
                if ((Num >> 30) & 1) {
                    temp = (Num >> 14) & 0x3FFF;
                } else {
                    temp = nCardNo;
                }
            } else {
                temp = nCardNo2;
            }
        } else {
            temp = nCardNo2;
        }
        return temp;
    } else {
    for (i = 0; i < Hand; i++) {
        cBuf = SzDuel_InqHand(nSide, i);
        temp = (cBuf & 0xF000) >> 12;
        if (temp > 0) {
            Combo[temp] = cBuf & 0xFFF;
        }
    }
    Ret = Combo[0];
    for (i = 0; i < 5; i++) {
        if (Ret != 999) {
            SzDuel_InitCardInfo(&nVarInfo, Ret);
            bKind0 = nVarInfo.bVarKind >> 5;
        } else {
            bKind0 = -1;
        }
        if (Combo[i + 1] != 999) {
            SzDuel_InitCardInfo(&nVarInfo, Combo[i + 1]);
            bKind1 = nVarInfo.bVarKind >> 5;
        } else {
            return Ret;
        }
        if (bKind0 == 0 && bKind1 == 0) {
            Num = Fusion(Ret, Combo[i + 1]);
            if (Num != 999) {
                temp = Num;
            } else {
                temp = Combo[i + 1];
            }
        } else if (bKind0 == 2 && bKind1 == 0) {
            Num = Do_Chk_Enc(Combo[i + 1], Ret);
            if (Num != 999) {
                if ((Num >> 30) & 1) {
                    temp = (Num >> 14) & 0x3FFF;
                } else {
                    temp = Combo[i + 1];
                }
            } else {
                temp = Combo[i + 1];
            }
        } else if (bKind1 == 2 && bKind0 == 0) {
            Num = Do_Chk_Enc(Ret, Combo[i + 1]);
            if (Num != 999) {
                if ((Num >> 30) & 1) {
                    temp = (Num >> 14) & 0x3FFF;
                } else {
                    temp = Ret;
                }
            } else {
                temp = Combo[i + 1];
            }
        } else {
            temp = Combo[i + 1];
        }
        Ret = temp;
    }
    }
    return Ret;
}
