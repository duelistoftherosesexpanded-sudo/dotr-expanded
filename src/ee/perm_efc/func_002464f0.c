#include "dotr/ee/perm_efc.h"

/* The locals are the Japanese build's, declared in the reverse of the debug-information order
 * (that order fixes the saved registers). Which of them held the packed cell (here Land) is an
 * assumption. */
unsigned int GetMoveRev(struct jp_64b6d *vInfo, int nX, int nZ)
{
    struct jp_65648 *Temp;
    int i;
    int temp;
    int Land;
    int AGREE;
    int MoveRev;

    if (vInfo == 0) {
        return 0;
    }
    MoveRev = 0;
    if (vInfo->bID == 0) {
        if (Ta_IsLDAbl(4, vInfo->bOwner)) {
            MoveRev = 2;
        } else {
            MoveRev = 1;
        }
        return MoveRev;
    }
    if (!vInfo->fOpen) {
        MoveRev = 1;
        Temp = &vInfo->aVarEffIdx[1];
        if (Temp != 0) {
            temp = Temp->dwID >> 8;
            /* 8-10: the lab-movement effects */
            if (temp == 8 || (unsigned)(temp - 9) < 2) {
                MoveRev |= 0x10;
            }
            /* 79: cannot move */
            if (temp == 79) {
                return 0;
            }
        }
        return MoveRev;
    }
    Land = (nZ << 8) | nX;
    AGREE = CheckLandAgreement(SzDuel_GetField(Land & 0xFF, Land >> 8), vInfo);
    /* 0: no change, 1: powered up, 2: powered down */
    switch (AGREE) {
    case 0:
    case 2:
        MoveRev = 1;
        break;
    case 1:
        MoveRev = 2;
        break;
    }
    Temp = &vInfo->aVarEffIdx[1];
    if (Temp != 0) {
        temp = Temp->dwID >> 8;
        /* 6: double movement; 8-10: lab movement; 79: cannot move */
        if (temp == 6) {
            MoveRev = (unsigned short)Temp->dwParam;
        } else if (temp == 8 || temp == 9 || temp == 10) {
            MoveRev |= 0x10;
        } else if (temp == 79) {
            return 0;
        }
    }
    for (i = 0; i < 11; i++) {
        if (SzDuel_GetUnitStatus(0, i, 4) == 1 && SzDuel_GetUnitStatus(0, i, 5) == 1 && (Temp = Ta_GetUnitEffect(0, i, 2)) != 0) {
            if (Temp->dwID >> 8 == 6) {
                switch (Temp->dwID & 0x3F) {
                case 15:
                    if ((vInfo->bVarKind & 0x1F) == 15 && (vInfo->bVarKind >> 5) == 0) {
                        MoveRev = ((MoveRev >> 4) << 4) | (unsigned short)Temp->dwParam;
                    }
                    break;
                default:
                    MoveRev = ((MoveRev >> 4) << 4) | (unsigned short)Temp->dwParam;
                    break;
                }
            }
        }
        if (SzDuel_GetUnitStatus(1, i, 4) == 1 && SzDuel_GetUnitStatus(1, i, 5) == 1 && (Temp = Ta_GetUnitEffect(1, i, 2)) != 0) {
            if (Temp->dwID >> 8 == 6) {
                switch (Temp->dwID & 0x3F) {
                case 15:
                    if ((vInfo->bVarKind & 0x1F) == 15 && (vInfo->bVarKind >> 5) == 0) {
                        MoveRev = ((MoveRev >> 4) << 4) | (unsigned short)Temp->dwParam;
                    }
                    break;
                default:
                    MoveRev = ((MoveRev >> 4) << 4) | (unsigned short)Temp->dwParam;
                    break;
                }
            }
        }
    }
    /* 67: cancel movement bonus */
    if (Ta_SearchUncTypeNaturalEffectUser(67, 0) >= 0) {
        MoveRev = ((MoveRev >> 4) << 4) | 1;
    }
    if (Ta_IsLDRangeAvilityFromLoc(Land, vInfo->bOwner, vInfo->bVarKind & 0x1F, 12) == 1) {
        MoveRev = ((MoveRev >> 4) << 4) | 2;
    }
    return MoveRev;
}
