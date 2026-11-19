#include "dotr/ee/dest_efc.h"

/* Decides what the effect *temp (slot 4) of the unit nSide_1/nID_1 does when the unit is destroyed
 * in a battle with nSide_2/nID_2 at cell nX/nZ, and fills the effect record *eInfo (sequence
 * number eNO and three operands). The effect type is temp->dwID >> 8; bits 6-7 of dwID select the
 * side(s) searched and bits 0-5 the kind; dwParam carries a high and a low 16-bit operand. Returns
 * 0 when there is no unit, no effect, both units are on the same current side, or (for most types)
 * the effect has no object; otherwise 1 with flag bits in the second byte (0x101, 0x103, 0x201,
 * 0x401) and 3 for type 62 under an active trap 76 with trap 46. Types 20 and 62 first ask
 * Ta_IsActiveTrapB for the opponent's trap 76, which turns the effect into sequence 102 unless
 * trap 46 is active.
 * Name, prototype, parameter, local and member names from the Japanese build's debug information
 * (Ta_Manage_DestEfc, EndBattle.c). MATCHING DEVICE: which local holds which value in type 62
 * (see the note there). */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int Ta_Manage_DestEfc(int nSide_1, int nID_1, int nSide_2, int nID_2, int nX, int nZ,
        struct jp_10241 *eInfo, unsigned int fFlag)
        {
    struct jp_102e6 *temp;
    int Num3;
    int Num2;
    int Num1;
    int Unit;
    int eSide;
    int LOC;
    unsigned int EFC;
    int eObj;
    int i;

    if (SzDuel_GetUnit(nSide_1, nID_1) == 0) {
        return 0;
    }
    if ((temp = Ta_GetUnitEffect(nSide_1, nID_1, 4)) == 0) {
        return 0;
    }
    if (Ta_GetCurrentSide(nSide_1, nID_1) == Ta_GetCurrentSide(nSide_2, nID_2)) {
        return 0;
    }
    EFC = temp->dwID;
    switch (EFC >> 8) {
    case 2: /* Terrain Change */
        switch (temp->dwParam & 0xFFFF) {
        case 8:
            eInfo->eNO = 2;
            eInfo->dwFrom = (nZ << 8) | nX;
            eInfo->dwTo = temp->dwParam >> 16;
            eInfo->dwPar = temp->dwParam & 0xFFFF;
            break;
        default:
            eInfo->eNO = 2;
            eInfo->dwFrom = (nZ << 8) | nX;
            eInfo->dwTo = temp->dwParam >> 16;
            eInfo->dwPar = temp->dwParam & 0xFFFF;
            break;
        }
        return 1;
    case 19: /* AP/DP Reduction */
        if ((EFC & 0x3F) == 26) { /* search kind 26: the card that attacked */
            if (nSide_2 >= 0 && nID_2 > 0 && fFlag == 0) {
                Unit = ConvUnitIDtoFlagList(0, nSide_2, nID_2);
            } else {
                return 0;
            }
        } else {
            LOC = SzDuel_GetUnitLoc(nSide_1, nID_1);
            eObj = temp->dwParam >> 16;
            if (eObj == 0) {
                EFC = temp->dwID;
                Unit = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide_1, nID_1), (EFC >> 6) & 3,
                        EFC & 0x3F, 14);
            } else {
                EFC = temp->dwID;
                Unit = SearchUnitByRange(LOC & 0xFF, LOC >> 8, Ta_GetCurrentSide(nSide_1, nID_1),
                        (EFC >> 6) & 3, EFC & 0x3F, eObj);
            }
        }
        Unit = ConvUnitIDtoFlagList_OFF(Unit, nSide_1, nID_1);
        if (fFlag != 0) {
            Unit = ConvUnitIDtoFlagList_OFF(Unit, nSide_2, nID_2);
        }
        if (Unit == 0) {
            return 0;
        }
        eInfo->eNO = 57;
        eInfo->dwFrom = 0;
        eInfo->dwTo = Unit;
        eInfo->dwPar = temp->dwParam & 0xFFFF;
        return 1;
    case 23: /* Spellbind */
        eObj = EFC & 0x3F;
        if (eObj == 26) {
            if (nSide_2 >= 0 && nID_2 > 0 && fFlag == 0) {
                Unit = ConvUnitIDtoFlagList(0, nSide_2, nID_2);
            } else {
                return 0;
            }
        } else {
            Unit = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide_1, nID_1), (EFC >> 6) & 3, eObj,
                    14);
        }
        if (fFlag != 0) {
            Unit = ConvUnitIDtoFlagList_OFF(Unit, nSide_2, nID_2);
        }
        eInfo->eNO = 0;
        eInfo->dwFrom = 0;
        eInfo->dwTo = Unit;
        eInfo->dwPar = temp->dwParam & 0xFFFF;
        if (Unit == 0) {
            return 0;
        }
        return 0x401;
    case 36: /* Return To Deck */
        Unit = ConvUnitIDtoFlagList(0, nSide_1, nID_1);
        eInfo->eNO = 30;
        eInfo->dwFrom = 0;
        eInfo->dwTo = Unit;
        eInfo->dwPar = Ta_GetCurrentSide(nSide_1, nID_1);
        return 1;
    case 13: /* Alter LP (Heal/Damage) */
        Num1 = ((temp->dwParam >> 16) << 16) | (temp->dwParam & 0xFFFF);
        switch ((EFC >> 6) & 3) {
        case 1:
            eInfo->eNO = 8;
            eInfo->dwFrom = 0;
            eInfo->dwTo =
                    Ta_GetEOwnerSide(Ta_GetCurrentSide(nSide_1, nID_1), (temp->dwID >> 6) & 3);
            eInfo->dwPar = Num1;
            return 1;
        case 2:
            eInfo->eNO = 9;
            eInfo->dwFrom = SzDuel_GetUnitLoc(nSide_1, nID_1);
            eInfo->dwTo =
                    Ta_GetEOwnerSide(Ta_GetCurrentSide(nSide_1, nID_1), (temp->dwID >> 6) & 3);
            eInfo->dwPar = Num1;
            return 1;
        default:
            return 0;
        }
        break;
    case 20: /* Revive */
        if ((i = Ta_IsActiveTrapB(nSide_1 == 0 ? 1 : 0, 76)) >= 0) {
            if (Ta_IsActiveTrapB(0, 46) < 0) {
                eInfo->eNO = 102;
                eInfo->dwFrom = 1;
                eInfo->dwTo = i;
                eInfo->dwPar = 0;
                return 1;
            }
            if ((Num1 = Ta_GetSummonablePiece(nSide_1, 1)) >= 0) {
                Unit = ConvUnitIDtoFlagList(0, nSide_1, nID_1);
                eInfo->eNO = 31;
                eInfo->dwFrom = ((nSide_1 << 8) | nID_1) | 0x10000;
                eInfo->dwTo = Unit;
                eInfo->dwPar = Num1;
                return 0x103;
            }
            return 0;
        }
        if ((Num1 = Ta_GetSummonablePiece(nSide_1, 1)) >= 0) {
            Unit = ConvUnitIDtoFlagList(0, nSide_1, nID_1);
            eInfo->eNO = 31;
            eInfo->dwFrom = ((nSide_1 << 8) | nID_1) | 0x10000;
            eInfo->dwTo = Unit;
            eInfo->dwPar = Num1;
            return 0x101;
        }
        return 0;
    case 62: /* Revive highest attack for 1k LP */
        /* MATCHING DEVICE: the side is held in Num3 (first block) and Unit (second block) and
         * the counts in Unit / Num3, Num2; with eSide and Num1..Num3 in their natural roles the
         * same code is produced with other saved registers. */
        if ((eSide = Ta_IsActiveTrapB(nSide_1 == 0 ? 1 : 0, 76)) >= 0) {
            if (Ta_IsActiveTrapB(0, 46) >= 0) {
                Num3 = Ta_GetCurrentSide(nSide_1, nID_1);
                if (SzDuel_GetLP(nSide_1) > Ta_GetLPRevByNaturalEffect(44, temp->dwParam & 0xFFFF)) {
                    if (Ta_GetSummonablePiece(Num3, 1) >= 0) {
                        Num1 = 5 - Ta_GetUnitNum(Num3, 4);
                        Num2 = TaUtil_GetGraveCardNum(Num3, 0) + TaUtil_GetGraveCardNum(Num3 == 0 ? 1 : 0, 0);
                        for (i = 0, Unit = 0; i < 8; i++) {
                            if (Ta_GetSummonablePieceWithOrder(Num3, i, 1) < 0) {
                                continue;
                            }
                            Unit++;
                        }
                        if (Num1 <= 0 || Num2 == 0 || Unit == 0) {
                            return 0;
                        }
                        eInfo->eNO = 58;
                        if (Ta_GetLPRevByNaturalEffect(44, temp->dwParam & 0xFFFF) >= 0) {
                            eInfo->dwFrom =
                                    Ta_GetLPRevByNaturalEffect(44, temp->dwParam & 0xFFFF);
                        } else {
                            eInfo->dwFrom =
                                    -Ta_GetLPRevByNaturalEffect(44, temp->dwParam & 0xFFFF);
                        }
                        eInfo->dwTo = Num3;
                        eInfo->dwPar = MIN(MIN(Num1, Num2), Unit);
                        return 3;
                    }
                }
                return 0;
            } else {
                eInfo->eNO = 102;
                eInfo->dwFrom = 1;
                eInfo->dwTo = eSide;
                eInfo->dwPar = 0;
                return 1;
            }
        }
        Unit = Ta_GetCurrentSide(nSide_1, nID_1);
        if (SzDuel_GetLP(nSide_1) > Ta_GetLPRevByNaturalEffect(44, temp->dwParam & 0xFFFF)) {
            if (Ta_GetSummonablePiece(Unit, 1) >= 0) {
                Num1 = 5 - Ta_GetUnitNum(Unit, 4);
                Num3 = TaUtil_GetGraveCardNum(Unit, 0) + TaUtil_GetGraveCardNum(Unit == 0 ? 1 : 0, 0);
                for (i = 0, Num2 = 0; i < 8; i++) {
                    if (Ta_GetSummonablePieceWithOrder(Unit, i, 1) < 0) {
                        continue;
                    }
                    Num2++;
                }
                if (Num1 <= 0 || Num3 == 0 || Num2 == 0) {
                    return 0;
                }
                eInfo->eNO = 58;
                eInfo->dwFrom = Ta_GetLPRevByNaturalEffect(44, temp->dwParam & 0xFFFF);
                eInfo->dwTo = Unit;
                eInfo->dwPar = MIN(MIN(Num1, Num3), Num2);
                return 1;
            }
        }
        return 0;
    case 38: /* Summon Again */
        eInfo->eNO = 33;
        eInfo->dwFrom = 0;
        eInfo->dwTo = Ta_GetCurrentSide(nSide_1, nID_1);
        eInfo->dwPar = 0;
        return 1;
    case 65: /* Buff Monsters by 1k for 1k LP */
        Num1 = SzDuel_GetLP(nSide_1);
        if ((temp->dwParam >> 16) < (unsigned int)Num1) {
            eSide = Ta_GetCurrentSide(nSide_1, nID_1);
            Unit = ConvUnitIDtoFlagList_OFF(SearchUnitByRange(0, 0, eSide, (temp->dwID >> 6) & 3,
                    temp->dwID & 0x3F, 14),
                    nSide_1, nID_1);
            if (Unit == 0) {
                return 0;
            }
            eInfo->eNO = 59;
            eInfo->dwFrom = eSide;
            eInfo->dwTo = Unit;
            eInfo->dwPar = temp->dwParam;
            return 1;
        }
        return 0;
    case 56: /* Summon Copy Faceup (in summoning area) */
        eSide = Ta_GetCurrentSide(nSide_1, nID_1);
        switch (temp->dwParam >> 16) {
        case 31:
            Unit = Ta_GetSummonablePiece(eSide, 1);
            if (Unit >= 0) {
                eInfo->eNO = 1;
                eInfo->dwFrom = eSide;
                eInfo->dwTo = Unit;
                eInfo->dwPar = temp->dwParam;
                return 1;
            }
            return 0;
        case 30:
            Unit = Ta_GetSummonablePiece(eSide == 0 ? 1 : 0, 1);
            if (Unit >= 0) {
                eInfo->eNO = 1;
                eInfo->dwFrom = eSide;
                eInfo->dwTo = Unit;
                eInfo->dwPar = temp->dwParam;
                return 1;
            }
            return 0;
        default:
            return 0;
        }
        break;
    case 81: /* Jowls of Dark Demise */
        eSide = Ta_GetCurrentSide(nSide_1, nID_1);
        switch ((temp->dwParam >> 16) & 0xFF) {
        case 31:
            Unit = Ta_GetSummonablePiece(eSide, 1);
            if (Unit >= 0) {
                eInfo->eNO = 1;
                eInfo->dwFrom = eSide | 0x10000;
                eInfo->dwTo = Unit;
                eInfo->dwPar = temp->dwParam;
                temp->dwID = 0xFFFFFFFF;
                temp->dwParam = 0xFFFFFFFF;
                return 1;
            }
            return 0;
        case 30:
            Unit = Ta_GetSummonablePiece(eSide == 0 ? 1 : 0, 1);
            if (Unit >= 0) {
                eInfo->eNO = 1;
                eInfo->dwFrom = eSide | 0x10000;
                eInfo->dwTo = Unit;
                eInfo->dwPar = temp->dwParam;
                temp->dwID = 0xFFFFFFFF;
                temp->dwParam = 0xFFFFFFFF;
                return 1;
            }
            return 0;
        default:
            return 0;
        }
        break;
    case 26: /* Set Summoning Power */
        eInfo->eNO = 65;
        eInfo->dwFrom = 0;
        eInfo->dwTo = Ta_GetCurrentSide(nSide_1, nID_1) == 0 ? 1 : 0;
        eInfo->dwPar = temp->dwParam & 0xFFFF;
        return 1;
    case 4: /* Destruction */
        switch (EFC & 0x3F) {
        case 26:
            if (nSide_2 >= 0) {
                if (nID_2 > 0 && fFlag == 0) {
                    Unit = ConvUnitIDtoFlagList(0, nSide_2, nID_2);
                    eInfo->eNO = 21;
                    eInfo->dwFrom = 0;
                    eInfo->dwTo = Unit;
                    eInfo->dwPar = 0;
                    return 0x201;
                }
            }
            return 0;
        case 30:
            Unit = SearchUnitByRange(nX, nZ, Ta_GetCurrentSide(nSide_1, nID_1), (EFC >> 6) & 3, 30,
                    temp->dwParam >> 16);
            eInfo->eNO = 21;
            eInfo->dwFrom = 0;
            eInfo->dwTo = Unit;
            eInfo->dwPar = 0;
            if (SzDuel_GetUnitCardNo(nSide_1, nID_1) == 617) {
                eInfo->eNO = 23;
            }
            eInfo->dwFrom = 0;
            eInfo->dwTo = Unit;
            eInfo->dwPar = 0;
            return Unit != 0 ? 1 : 0;
        case 31:
            Unit = ConvUnitIDtoFlagList_OFF(SearchUnitByRange(nX, nZ,
                    Ta_GetCurrentSide(nSide_1, nID_1),
                    (EFC >> 6) & 3, 31,
                    temp->dwParam >> 16),
                    nSide_1, nID_1);
            if (fFlag != 0) {
                Unit = ConvUnitIDtoFlagList_OFF(Unit, nSide_2, nID_2);
            }
            eInfo->eNO = 21;
            eInfo->dwFrom = 0;
            eInfo->dwTo = Unit;
            eInfo->dwPar = 0;
            if (SzDuel_GetUnitCardNo(nSide_1, nID_1) == 509 ||
                    SzDuel_GetUnitCardNo(nSide_1, nID_1) == 506) {
                eInfo->eNO = 46;
            }
            eInfo->dwFrom = (nSide_1 << 8) | nID_1;
            eInfo->dwTo = 0;
            eInfo->dwPar = 1;
            return 0x201;
        default:
            return 0;
        }
        break;
    default:
        return 0;
    }
    return 0;
}
