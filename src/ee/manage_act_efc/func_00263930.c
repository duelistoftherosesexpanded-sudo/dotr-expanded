#include "dotr/ee/manage_act_efc.h"

/* 0x263930. Compiled with `#pragma conditional_move on`, set directly before the function and
 * reset after it. Two callees are declared without a prototype inside the function body. */
#ifndef __MWERKS__
/* Host build only: the code calls two functions without a prototype and leaves some locals
 * unset on some paths, which the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#ifdef __MWERKS__
#pragma conditional_move on
#endif
int Ta_Manage_ActEfc(DotrActEfcInfo *eInfo, int nSide, int nID, int eSide, int eID, int nX, int nZ,
                     DotrActEfcEffect *Effect, DotrActEfcKind *pInfo, unsigned char bHeap)
{
    extern int SzDuel_SetUnitDelayCount();
    extern int SzDuel_GetUnitCardNo();
    int i;
    int temp;
    int Num;
    int LOC;
    int eObj;
    int Unit;
    int Unit2;
    int EFC;
    int NUM;
    int fN;
    int fE;
    int pKindP;
    int pKindS;
    DotrActEfcInfo *eTemp;

    eObj = 0;
    pKindP = 0;
    i = 0;
    do {
        i++;
    } while (i < 0x31);
    pInfo->bType = 0;
    pInfo->bKind = 0;
    fE = 0;
    fN = 0;
    switch ((unsigned int)(Unit = eInfo->dwID) >> 8) {
    case 2: /* terrain change */
        switch ((unsigned short)eInfo->dwParam) {
        case 8:
            Effect->eNO = 2;
            Effect->dwFrom = (nZ << 8) | nX;
            Effect->dwTo = eInfo->dwParam >> 16;
            Effect->dwPar = (unsigned short)eInfo->dwParam;
            break;
        default:
            Effect->eNO = 2;
            Effect->dwFrom = (nZ << 8) | nX;
            Effect->dwTo = eInfo->dwParam >> 16;
            Effect->dwPar = (unsigned short)eInfo->dwParam;
            break;
        }
        return 1;
    case 0x12: /* 18: AP/DP buff */
        if ((eInfo->dwParam >> 16) == 0x16) {
            NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, Unit & 0x3F, 0x16);
        } else {
            NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3,
                                    ((eInfo->dwParam >> 16) << 16) | (Unit & 0x3F), 0xE);
        }
        if (NUM == 0) {
            return 0;
        }
        Effect->eNO = 0x1D;
        Effect->dwFrom = 1;
        Effect->dwTo = NUM;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        return 1;
    case 0x13: /* 19: AP/DP reduction */
        if (eID > 0 && eSide >= 0 && (bHeap & 0xFF) != 2) {
            eObj = ConvUnitIDtoFlagList(0, eSide, eID);
        } else {
            return 0;
        }
        Effect->eNO = 0x39;
        Effect->dwFrom = 0;
        Effect->dwTo = eObj;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        return eObj == 0 ? 0 : 1;
    case 0x33: /* 51: Time Wizard */
        NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3,
                                ((eInfo->dwParam >> 16) << 16) | (0x3F & Unit), 0xE);
        Effect->eNO = 0x3F;
        Effect->dwFrom = 0;
        Effect->dwTo = NUM;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        return NUM == 0 ? 0 : 1;
    case 0x17: /* 23: spellbind */
        if (0x1A == (pKindP = Unit & 0x3F)) {
            if (eID > 0 && eSide >= 0 && (bHeap & 0xFF) != 2) {
                NUM = ConvUnitIDtoFlagList(0, eSide, eID);
            } else {
                return 0;
            }
        } else {
            NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, pKindP, 0xE);
        }
        Effect->eNO = 0;
        Effect->dwFrom = 0;
        Effect->dwTo = NUM;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        if (0 == NUM) {
            return 0;
        }
        return 1;
    case 0xD: /* 13: alter LP */
        Effect->eNO = 8;
        Effect->dwFrom = 0;
        Effect->dwTo = Ta_GetEOwnerSide(Ta_GetCurrentSide(nSide, nID), (eInfo->dwID >> 6) & 3);
        Effect->dwPar = ((eInfo->dwParam >> 16) << 16) | (eInfo->dwParam & 0xFFFF);
        return 1;
    case 0x14: /* 20: revive */
        if ((Unit = Ta_IsUnitControl(nSide, nID)) != SzDuel_GetTurnOwner()) {
            return 0;
        }
        if ((Unit = Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 0x4C)) >= 0) {
            if (Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 0x2E) >= 0) {
                Effect->eNO = 1;
                Effect->dwFrom = Ta_GetCurrentSide(nSide, nID) | 0x20000;
                Effect->dwTo = Ta_GetSummonablePiece(Ta_GetCurrentSide(nSide, nID), 1);
                Effect->dwPar = 0;
                switch ((eInfo->dwID >> 6) & 3) {
                case 3:
                    LOC = 2;
                    break;
                case 1:
                    LOC = Ta_GetCurrentSide(nSide, nID) != 0 ? 1 : 0;
                    break;
                }
                Num = 0;
                switch (eInfo->dwID & 0x3F) {
                case 0x1E:
                    if (LOC == 2) {
                        Num += TaUtil_GetGraveCardNum(0, 1);
                        Num += TaUtil_GetGraveCardNum(1, 1);
                        Num += TaUtil_GetGraveCardNum(0, 2);
                        Num += TaUtil_GetGraveCardNum(1, 2);
                        Num += TaUtil_GetGraveCardNum(0, 3);
                        Num += TaUtil_GetGraveCardNum(1, 3);
                        Num += TaUtil_GetGraveCardNum(0, 4);
                        Num += TaUtil_GetGraveCardNum(1, 4);
                        Num += TaUtil_GetGraveCardNum(0, 5);
                        Num += TaUtil_GetGraveCardNum(1, 5);
                    } else {
                        Num += TaUtil_GetGraveCardNum(LOC, 1);
                        Num += TaUtil_GetGraveCardNum(LOC, 2);
                        Num += TaUtil_GetGraveCardNum(LOC, 3);
                        Num += TaUtil_GetGraveCardNum(LOC, 4);
                        Num += TaUtil_GetGraveCardNum(LOC, 5);
                    }
                    if (Num == 0) {
                        return 0;
                    }
                    pKindS = 2;
                    break;
                default:
                    return 0;
                }
                pInfo->bType = 0;
                pInfo->bKind = (LOC << 4) | pKindS;
                return 4;
            } else {
                Effect->eNO = 0x66;
                Effect->dwFrom = 1;
                Effect->dwTo = Unit;
                Effect->dwPar = 0;
                return 1;
            }
        } else {
            if (Ta_GetSummonablePiece(Ta_GetCurrentSide(nSide, nID), 1) < 0) {
                return 0;
            }
            Effect->eNO = 1;
            Effect->dwFrom = Ta_GetCurrentSide(nSide, nID) | 0x20000;
            Effect->dwTo = Ta_GetSummonablePiece(Ta_GetCurrentSide(nSide, nID), 1);
            Effect->dwPar = 0;
            switch ((eInfo->dwID >> 6) & 3) {
            case 3:
                LOC = 2;
                break;
            case 1:
                LOC = Ta_GetCurrentSide(nSide, nID) != 0 ? 1 : 0;
                break;
            }
            Num = 0;
            switch (eInfo->dwID & 0x3F) {
            case 0x1E:
                if (LOC == 2) {
                    Num += TaUtil_GetGraveCardNum(0, 1);
                    Num += TaUtil_GetGraveCardNum(1, 1);
                    Num += TaUtil_GetGraveCardNum(0, 2);
                    Num += TaUtil_GetGraveCardNum(1, 2);
                    Num += TaUtil_GetGraveCardNum(0, 3);
                    Num += TaUtil_GetGraveCardNum(1, 3);
                    Num += TaUtil_GetGraveCardNum(0, 4);
                    Num += TaUtil_GetGraveCardNum(1, 4);
                    Num += TaUtil_GetGraveCardNum(0, 5);
                    Num += TaUtil_GetGraveCardNum(1, 5);
                } else {
                    Num += TaUtil_GetGraveCardNum(LOC, 1);
                    Num += TaUtil_GetGraveCardNum(LOC, 2);
                    Num += TaUtil_GetGraveCardNum(LOC, 3);
                    Num += TaUtil_GetGraveCardNum(LOC, 4);
                    Num += TaUtil_GetGraveCardNum(LOC, 5);
                }
                if (Num == 0) {
                    return 0;
                }
                pKindS = 2;
                break;
            default:
                return 0;
            }
            pInfo->bType = 0;
            pInfo->bKind = (LOC << 4) | pKindS;
            return pKindS;
        }
    case 0x23: /* 35: shift to attack */
        NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, Unit & 0x3F, 0xE);
        NUM = ConvUnitIDtoFlagList_OFF(NUM, nSide, nID);
        if (eID > 0 && eSide >= 0 && (bHeap & 0xFF) == 2) {
            NUM = ConvUnitIDtoFlagList_OFF(NUM, eSide, eID);
        }
        Effect->eNO = 0xD;
        Effect->dwFrom = 5;
        Effect->dwTo = NUM;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        return NUM == 0 ? 0 : 1;
    case 0x18: /* 24: flip */
        NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3,
                                ((eInfo->dwParam >> 16) << 16) | (0x3F & Unit), 0xE);
        NUM = ConvUnitIDtoFlagList_OFF(NUM, nSide, nID);
        if ((eInfo->dwID & 0x3F) == 0x2D) {
            Unit2 = Ta_GetMaximumOrderUnitFromFlagList(NUM);
            NUM = ConvUnitIDtoFlagList(0, Unit2 >> 8, Unit2 & 0xFF);
        }
        Effect->eNO = 0xD;
        Effect->dwFrom = 4;
        Effect->dwTo = NUM;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        if (NUM == 0) {
            return 0;
        }
        return 1;
    case 4: /* destruction */
        if (0x1A == (pKindP = 0x3F & Unit)) {
            if (eID <= 0 || eSide < 0 || (bHeap & 0xFF) == 2) {
                return 0;
            }
            switch (eInfo->dwParam >> 16) {
            case 2:
                if ((SzDuel_GetUnitKind(eSide, eID) & 0x1F) == 2) {
                    eObj = ConvUnitIDtoFlagList(0, eSide, eID);
                }
                break;
            default:
                eObj = ConvUnitIDtoFlagList(0, eSide, eID);
                break;
            }
        } else if (pKindP == 0x30) {
            Unit2 = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), 3, 0x1F, 0xE);
            Unit2 = ConvUnitIDtoFlagList_OFF(Unit2, nSide, nID);
            Unit2 = Ta_GetRandomUnit(Unit2);
            if (Unit2 < 0) {
                return 0;
            }
            eObj = ConvUnitIDtoFlagList(0, Unit2 >> 8, 0xFF & Unit2);
        } else if ((eInfo->dwParam >> 16) != 0) {
            temp = SzDuel_GetUnitLoc(nSide, nID);
            Unit = eInfo->dwID;
            eObj = SearchUnitByRange(temp & 0xFF, temp >> 8, Ta_GetCurrentSide(nSide, nID),
                                     ((unsigned int)Unit >> 6) & 3, Unit & 0x3F, eInfo->dwParam >> 16);
        } else {
            eObj = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, pKindP, 0xE);
        }
        Effect->eNO = 0x15;
        Effect->dwFrom = 0;
        Effect->dwTo = eObj;
        Effect->dwPar = 0;
        if (0 == eObj) {
            return 0;
        }
        if (eID > 0 && eSide >= 0) {
            if (Ta_GetAgreeOwnerIDon22bitFlagLst((eSide << 8) | eID, eObj) != 0) {
                if ((bHeap & 0xFF) > 0) {
                    return 0x201;
                }
                return 1;
            }
            if (Ta_GetAgreeOwnerIDon22bitFlagLst((nSide << 8) | nID, eObj) != 0) {
                if ((0xFF & bHeap) > 0) {
                    return 0x401;
                }
                return 1;
            }
        }
        return 1;
    case 0x38: /* 56: summon a face-up copy */
        switch (eInfo->dwParam >> 16) {
        case 0x1F:
            if ((Unit = Ta_GetSummonablePiece(Ta_IsUnitControl(nSide, nID), 1)) > 0 &&
                Ta_GetUnitNum(Ta_IsUnitControl(nSide, nID), 4) < 5) {
                if (SzDuel_GetUnitCardNo(nSide, nID) != 0x129 && SzDuel_GetUnitCardNo(nSide, nID) != 0x112) {
                    Effect->eNO = 1;
                    Effect->dwFrom = Ta_GetCurrentSide(nSide, nID) | 0x10000;
                    Effect->dwTo = Unit;
                    Effect->dwPar = (unsigned short)eInfo->dwParam;
                } else {
                    Effect->eNO = 1;
                    Effect->dwFrom = Ta_GetCurrentSide(nSide, nID);
                    Effect->dwTo = Unit;
                    Effect->dwPar = (unsigned short)eInfo->dwParam;
                }
            } else {
                return 0;
            }
            break;
        default:
            return 0;
        }
        return 1;
    case 0x16: /* 22: grow stats to value */
        if ((Unit = eInfo->dwParam >> 16) == 0x1A) {
            if (eID > 0 && eSide >= 0 && (bHeap & 0xFF) != 2) {
                NUM = ConvUnitIDtoFlagList(0, eSide, eID);
            } else {
                return 0;
            }
        } else {
            NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), 3, Unit, 0xE);
        }
        Effect->eNO = 0x40;
        Effect->dwFrom = NUM;
        Effect->dwTo = (nSide << 8) | nID;
        Effect->dwPar = 0;
        if (NUM == 0) {
            return 0;
        }
        return 1;
    case 0x1A: /* 26: set summoning power */
        Effect->eNO = 0x41;
        Effect->dwFrom = 0;
        Effect->dwTo = Ta_GetCurrentSide(nSide, nID);
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        return 1;
    case 0x26: /* 38: summon again */
        Effect->eNO = 0x21;
        Effect->dwFrom = 0;
        Effect->dwTo = Ta_GetCurrentSide(nSide, nID);
        Effect->dwPar = 0;
        return 1;
    case 0x35: /* 53: attack buff from graveyard cards */
        Unit = ConvUnitIDtoFlagList(0, nSide, nID);
        if (Unit == 0) {
            return 0;
        }
        switch ((EFC = eInfo->dwID) & 0x3F) {
        case 0x22:
            Effect->eNO = 0x1D;
            Effect->dwFrom = 1;
            Effect->dwTo = Unit;
            Effect->dwPar = (unsigned short)eInfo->dwParam;
            return (eInfo->dwParam >> 16) == SzDuel_GetUnitCardNo(Ta_GetCurrentSide(nSide, nID), 0) ? 1 : 0;
        case 0x38:
            Effect->eNO = 0x1D;
            Effect->dwFrom = 1;
            Effect->dwTo = Unit;
            Effect->dwPar = (unsigned short)eInfo->dwParam;
            return (eInfo->dwParam >> 16) == (SzDuel_GetUnitKind(Ta_GetCurrentSide(nSide, nID), 0) & 0x1F) ? 1 : 0;
        case 0x2E:
            Unit2 = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), 3, eInfo->dwParam >> 16, 0xE);
            NUM = (unsigned short)eInfo->dwParam * Ta_GetUnitNumFrom22bitFlagList(Unit2);
            Effect->eNO = 0x1D;
            Effect->dwFrom = 1;
            Effect->dwTo = Unit;
            Effect->dwPar = NUM;
            return NUM != 0 ? 1U : 0U;
        case 0x2F:
            Unit = ConvUnitIDtoFlagList(Unit, nSide, nID);
            if ((eInfo->dwParam >> 16) == 0x1D) {
                temp = (unsigned short)eInfo->dwParam * TaUtil_GetGraveCardNum(Ta_IsUnitControl(nSide, nID), 0);
            } else if ((eInfo->dwParam >> 16) == 0x1C) {
                temp = (unsigned short)eInfo->dwParam * (TaLogic_GetGraveCardCount(nSide, 3, 0x3C) +
                                              TaLogic_GetGraveCardCount(nSide, 3, 0x57));
            }
            if (temp <= 0) {
                return 0;
            }
            Effect->eNO = 0x1D;
            Effect->dwFrom = 1;
            Effect->dwTo = Unit;
            Effect->dwPar = temp;
            return 1;
        default:
            return 0;
        }
    case 0x1E: /* 30: reveal types */
        NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, Unit & 0x3F, 0xE);
        NUM = ConvUnitIDtoFlagList_OFF(NUM, nSide, nID);
        Effect->eNO = 0xF;
        Effect->dwFrom = 0;
        Effect->dwTo = NUM;
        Effect->dwPar = 0;
        return NUM == 0 ? 0 : 1;
    case 0x36: /* 54: transform card kind */
    case 0x37: /* 55: transform card attribute */
        if ((pKindP = Unit & 0x3F) == 0x1A) {
            if (eID > 0 && eSide >= 0 && (bHeap & 0xFF) != 2) {
                NUM = ConvUnitIDtoFlagList(0, eSide, eID);
            } else {
                return 0;
            }
        } else {
            NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, pKindP, 0xE);
        }
        NUM = ConvUnitIDtoFlagList_OFF(NUM, nSide, nID);
        Effect->eNO = 0x42;
        Effect->dwFrom = eInfo->dwID >> 8;
        Effect->dwTo = NUM;
        Effect->dwPar = (unsigned short)eInfo->dwParam;
        if (NUM == 0) {
            return 0;
        }
        return 1;
    case 0x24: /* 36: return to deck */
        if ((Unit & 0x3F) == 0x1A) {
            if (eID > 0 && eSide >= 0 && (0xFF & bHeap) != 2) {
                eObj = ConvUnitIDtoFlagList(0, eSide, eID);
                Effect->eNO = 0x1E;
                Effect->dwFrom = 0;
                Effect->dwTo = eObj;
                Effect->dwPar = Ta_GetCurrentSide(eSide, nID);
            } else {
                return 0;
            }
        } else if ((Unit & 0x3F) == 0x1B) {
            eObj = ConvUnitIDtoFlagList(0, nSide, nID);
            Effect->eNO = 0x1E;
            Effect->dwFrom = 0;
            Effect->dwTo = eObj;
            Effect->dwPar = Ta_GetCurrentSide(nSide, nID);
        }
        if (eID > 0 && eSide >= 0 && (0xFF & bHeap) != 2 &&
            Ta_GetAgreeOwnerIDon22bitFlagLst((eSide << 8) | eID, eObj) != 0) {
            fE = 1;
        }
        if (nSide >= 0 && nID >= 0 &&
            Ta_GetAgreeOwnerIDon22bitFlagLst((nSide << 8) | nID, eObj) != 0) {
            fN = 1;
        }
        if ((0xFF & bHeap) == 1) {
            return (fN << 10) | ((fE << 9) | 1);
        }
        return 1;
    case 0x1F: /* 31: revolt/steal */
        if (Ta_SearchUncTypeNaturalEffectUserNum(0x39) > 0) {
            return 0;
        }
        Unit = eInfo->dwID;
        switch (pKindP = Unit & 0x3F) {
        case 0x1A:
            if (eID <= 0 || eSide < 0 || (bHeap & 0xFF) == 2) {
                return 0;
            }
            switch (eInfo->dwParam >> 16) {
            case 0xE:
                if ((SzDuel_GetUnitKind(Ta_GetCurrentSide(eSide, eID), eID) & 0x1F) == 0xE) {
                    NUM = ConvUnitIDtoFlagList(0, eSide, eID);
                } else {
                    return 0;
                }
                break;
            default:
                NUM = ConvUnitIDtoFlagList(0, eSide, eID);
                break;
            }
            Effect->eNO = 0x1A;
            Effect->dwFrom = Ta_GetCurrentSide(nSide, nID);
            Effect->dwTo = NUM;
            Effect->dwPar = 0xFFFE;
            if (NUM == 0) {
                return 0;
            }
            if ((bHeap & 0xFF) == 1) {
                return 0x101;
            }
            return 1;
        default:
            NUM = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Unit >> 6) & 3, pKindP, 0xE);
            Effect->eNO = 0x1A;
            Effect->dwFrom = Ta_GetCurrentSide(nSide, nID);
            Effect->dwTo = NUM;
            Effect->dwPar = 0xFFFE;
            if (NUM == 0) {
                return 0;
            }
            if (Ta_GetAgreeOwnerIDon22bitFlagLst((eSide << 8) | eID, NUM) == 1) {
                if ((bHeap & 0xFF) == 1) {
                    return 0x101;
                }
                return 1;
            }
            return 1;
        }
    case 0x25: /* 37: teleport */
        switch (eInfo->dwParam >> 16) {
        case 0x1E:
            if ((NUM = Ta_GetSummonablePiece(Ta_GetCurrentSide(nSide, nID) == 0 ? 1 : 0, 1)) >= 0) {
                Effect->eNO = 0x1F;
                Effect->dwFrom = (nSide << 8) | nID;
                Effect->dwTo = 0;
                Effect->dwPar = NUM;
                if ((0xFF & bHeap) == 1) {
                    return 0x801;
                }
                return 1;
            }
            break;
        case 0x1F:
            if ((NUM = Ta_GetSummonablePiece(Ta_GetCurrentSide(nSide, nID), 1)) >= 0) {
                Effect->eNO = 0x1F;
                Effect->dwFrom = (nSide << 8) | nID;
                Effect->dwTo = 0;
                Effect->dwPar = NUM;
                if ((bHeap & 0xFF) == 1) {
                    return 0x801;
                }
                return 1;
            }
            break;
        case 0x20:
            break;
        }
        return 0;
    case 0x3C: /* 60: teleport (Jigen Bakudan) */
        Unit = SzDuel_GetUnitLoc(nSide, nID);
        NUM = SearchUnitByRange(0xFF & Unit, Unit >> 8, Ta_GetCurrentSide(nSide, nID), 3,
                                 ((unsigned short)eInfo->dwParam << 16) | (eInfo->dwID & 0x3F), 1);
        if (NUM > 0 && (Unit = Ta_GetSummonablePiece(Ta_GetCurrentSide(nSide, nID) == 0 ? 1 : 0, 1)) >= 0) {
            Unit2 = Ta_GetMinimumOrderUnitFromFlagList(NUM);
            Effect->eNO = 0x1F;
            Effect->dwFrom = Unit2;
            Effect->dwTo = 0;
            Effect->dwPar = Unit;
            if (Unit2 == ((eSide << 8) | eID)) {
                if ((0xFF & bHeap) == 2) {
                    return 0x201;
                }
                return 1;
            }
            return 1;
        }
        return 0;
    case 0x46: /* 70: Jigen Bakudan */
    case 0xC: /* 12: transform */
        NUM = ConvUnitIDtoFlagList(0, nSide, nID);
        Effect->eNO = 0x43;
        Effect->dwFrom = 0;
        Effect->dwTo = NUM;
        Effect->dwPar = 0;
        SzDuel_SetUnitDelayCount(nSide, nID, (eInfo->dwParam >> 16) + 1);
        return NUM != 0 ? 1U : 0U;
    case 0x57: /* 87: discard hands */
        Effect->eNO = 0xE;
        Effect->dwFrom = 0;
        Effect->dwTo = Ta_GetCurrentSide(nSide, nID) == 0 ? 1 : 0;
        Effect->dwPar = 1;
        return 1;
    case 0x51: /* 81: Jowls of Dark Demise */
        if (Ta_SearchUncTypeNaturalEffectUserNum(0x39) > 0) {
            return 0;
        }
        if (eID > 0 && eSide >= 0 && (bHeap & 0xFF) != 2) {
            Effect->eNO = 0x43;
            Effect->dwFrom = (nSide << 8) | nID;
            Effect->dwTo = (eSide << 8) | eID;
            Effect->dwPar = 1;
            eTemp = SzDuel_GetUnitEffect(eSide, eID, 2);
            if (eTemp != NULL) {
                eTemp->dwID = 0x51FD;
                eTemp->dwParam = 0;
            }
            eTemp = SzDuel_GetUnitEffect(eSide, eID, 4);
            if (eTemp != NULL) {
                eTemp->dwID = 0x515C;
                eTemp->dwParam = (((Ta_IsUnitControl(nSide, nID) << 8) | 0x1F) << 16) | 0x2A0;
            }
            TaUtil_DoOpen(eSide, eID, 1);
            return ((bHeap & 0xFF) == 1) ? 0x101 : 1;
        }
        break;
    default:
        return 0;
    }
    return 0;
}

#ifdef __MWERKS__
#pragma conditional_move reset
#endif
