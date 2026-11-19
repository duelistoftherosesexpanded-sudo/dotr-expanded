#include "dotr/ee/driver_scans.h"

struct TreeTag *TaLogic_GetFusionTree(int nSide, struct TreeTag *p_fTree, unsigned char nOrder, unsigned short pRoot, unsigned char *pCombo, unsigned short *wSize, unsigned short *wCount)
{
    int arNextHand[5];
    int i;
    int j;
    int Res;
    int nCardNo;

    TaHand_GetNextHand(nSide, arNextHand);
    if (pCombo != 0) {
        nCardNo = arNextHand[nOrder];
        if (TaHand_IsCardSummon2(nSide, nCardNo) == 0) {
            return 0;
        }
        Res = (unsigned short)TaLogic_GetHandFusion(pRoot, nCardNo);
        if (Res == 999) {
            return 0;
        }
        if ((p_fTree = TaLogic_TreeAloc(wSize, wCount)) == 0) {
            return 0;
        }
        TaLogic_InitTree(p_fTree, Res, pCombo, nOrder);
    } else {
        if ((p_fTree = TaLogic_TreeAloc(wSize, wCount)) == 0) {
            return 0;
        }
        Res = pRoot;
        TaLogic_InitTree(p_fTree, Res, pCombo, nOrder);
    }
    if ((unsigned char)TaLogic_GetArrayLength(p_fTree->nComboLink) == 5) {
        return p_fTree;
    }
    i = j = 0;
    while (i < 5) {
        if (TaLogic_IsUseHandInCombo(i, p_fTree->nComboLink) != 0 || p_fTree->nRoot == 999) {
            i++;
            continue;
        }
        if ((p_fTree->pNextLink[j] = TaLogic_GetFusionTree(nSide, p_fTree->pNextLink[j], i, Res, p_fTree->nComboLink, wSize, wCount)) != 0) {
            j++;
        }
        i++;
    }
    return p_fTree;
}
