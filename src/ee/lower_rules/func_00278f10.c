#include "dotr/ee/lower_rules.h"
int TaLogic_GetEnchantQue(int nSide, struct TreeTag *p_fTree, struct jp_9c054 **pKeep, unsigned char nKeepDepth)
{
    int arNextHand[5];
    int i;
    int j;
    int Temp;
    int wEncCardNo;
    int Unit;
    int AP;
    int DP;
    AP = (unsigned int)TaHand_GetCardAPDP(p_fTree->nRoot) >> 16;
    DP = TaHand_GetCardAPDP(p_fTree->nRoot) & 0xffff;
    TaHand_GetNextHand(nSide, arNextHand);
    Unit = Ta_IsActiveTrapB(nSide, 43);
    if (Unit >= 0 && SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xff, 4)) {
        return 0;
    }
    for (i = 0; i < TaHand_GetNextHandNum(nSide); i++) {
        if ((unsigned char)TaLogic_IsUseHandInCombo(i, p_fTree->nComboLink)) {
            continue;
        }
        wEncCardNo = arNextHand[i];
        Temp = TaHand_GetCardKind(arNextHand[i]);
        if (Temp >> 5 != 2) {
            continue;
        }
        if (arNextHand[i] == 796) {
            continue;
        }
        if (arNextHand[i] == 795) {
            continue;
        }
        if (Ta_GetHandEnchant(p_fTree->nRoot, wEncCardNo) >> 16 != 1) {
            continue;
        }
        if (TaEnc_IsEncDo(wEncCardNo) != 1) {
            continue;
        }
        if (pKeep) {
            for (j = 0; j < nKeepDepth; j++) {
                TaLogic_IsKeepEnchant(pKeep[j], wEncCardNo, p_fTree->nRoot);
            }
        } else {
            p_fTree->nComboLink[(unsigned char)TaLogic_GetArrayLength(p_fTree->nComboLink)] = i;
            AP += TaEnc_GetEncScore(wEncCardNo);
            DP += TaEnc_GetEncScore(wEncCardNo);
        }
    }
    return AP << 16 | (DP & 0xffff);
}
