#include "dotr/ee/hand_combo.h"

/* Prototype and local names are the Japanese build's (util.c); the locals are declared in the
 * reverse of the debug-information order because that fixes their saved registers. */
int Ta_GetUnitAgreeWithVarInfo(struct jp_86d82 *vInfo, int eOwner, int cSide, int cKey)
{
    int Kind;
    int fSideMatch;
    int fObjMatch;

    Kind = vInfo->bVarKind;
    if ((Kind >> 5) != 0) {
        return 0;
    }
    fSideMatch = Ta_SideMatch(vInfo->fRevolt == 0 ? vInfo->bOwner : (vInfo->bOwner == 0 ? 1 : 0), eOwner, cSide);
    switch (cKey) { /* the search type */
    case 21: /* DARK */
        if (vInfo->bVarAttr == 1) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 20: /* LIGHT */
        fObjMatch = vInfo->bVarAttr == 0 ? 1 : 0;
        break;
    case 29: /* MONSTER */
        fObjMatch = 1;
        break;
    case 49: /* FEMALE */
        if (Do_Chk_Enc(vInfo->wCardNo, 767) != 999) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 14: /* MACHINE */
        if ((Kind & 0x1f) == 14) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 15: /* THUNDER */
        if ((Kind & 0x1f) == 15) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 2: /* ZOMBIE */
        if ((Kind & 0x1f) == 2) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 6: /* WINGED BEAST */
        if ((Kind & 0x1f) == 6) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 7: /* FIEND */
        if ((Kind & 0x1f) == 7) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 25: /* WIND */
        if (vInfo->bVarAttr == 5) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 16: /* AQUA */
        if ((Kind & 0x1f) == 16) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 17: /* PYRO */
        if ((Kind & 0x1f) == 17) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    case 19: /* PLANT */
        if ((Kind & 0x1f) == 19) {
            fObjMatch = 1;
        } else {
            fObjMatch = 0;
        }
        break;
    default:
        fObjMatch = 0;
        break;
    }
    if (fSideMatch != 0 && fObjMatch != 0) {
        return 1;
    }
    return 0;
}
