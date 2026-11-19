#include "dotr/ee/end_phase.h"

/* One step of the end-of-turn sequence for the turn owner; PrNo selects the
 * step and the result is (next step << 16) | effect flag, or 0xff0000 when
 * the sequence is over. Step 0 requests effect 8 when the owner's leader has
 * ability 5; step 1 forwards to Ta_IsVirusAll; step 2 collects, over the 7x7
 * map, the other side's units inside the leader's square range into three
 * flag lists (abilities 16, 15 and 13) and requests effect 0x71 when any is
 * set; step 3 awards experience to units 1 to 5, counts down the curse
 * timers of units 1 to 11 on both sides, clears spell timers outside a
 * cursing leader's range and clears all curse flags; step 4 answers 5 when
 * no preferred summon square is usable; step 5 answers 0xff0008 once the
 * second player has reached turn 100. Prototype, parameter and local names
 * and struct jp_908da are from the Japanese build's debug information; the
 * declaration order below is the reverse of the recorded one. */
int Ta_Do_EndPhase(int PrNo, struct jp_908da *eInfo) {
    int i;
    int j;
    int tSide;
    int eObj;
    int eObj2;
    int eObj3;
    int Unit;
    int Loc;
    int Loc2;
    int cRest;
    int fTwice;

    tSide = SzDuel_GetTurnOwner();
    switch (PrNo) {
    case 0:
        if (Ta_IsLDAbl(5, tSide)) {
            eInfo->eNO = 8;
            eInfo->dwFrom = 0;
            eInfo->dwTo = tSide;
            eInfo->dwPar = 50;
            return (PrNo + 1) << 16 | 1;
        }
        return (PrNo + 1) << 16;
    case 1:
        return (PrNo + 1) << 16 | Ta_IsVirusAll((int *)eInfo);
    case 2:
        Loc = SzDuel_GetUnitLoc(tSide, 0);
        i = 0;
        eObj = 0;
        eObj2 = 0;
        eObj3 = 0;
        for (; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                Unit = SzDuel_GetMapUnitID(i, j);
                if (Unit >= 0) {
                    if (tSide != Ta_GetCurrentSide(Unit >> 8, Unit & 0xff)) {
                        if (Ta_IsSquareRange(Loc & 0xff, Loc >> 8, i, j, Ta_GetLDRange(tSide))) {
                            if (SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) >> 5 == 0) {
                                if (Ta_IsLDAbl(16, tSide)) {
                                    if (Ta_IsLD_DestObj(tSide, SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) & 0x1f) && (Unit & 0xff) != 0) {
                                        eObj = ConvUnitIDtoFlagList(eObj, Unit >> 8, Unit & 0xff);
                                    }
                                }
                                if (Ta_IsLDAbl(15, tSide)) {
                                    if (Ta_IsLD_CurseObj(tSide, SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) & 0x1f) && (Unit & 0xff) != 0 &&
                                        SzDuel_GetUnitSpellRest(Unit >> 8, Unit & 0xff) == 0) {
                                        eObj2 = ConvUnitIDtoFlagList(eObj2, Unit >> 8, Unit & 0xff);
                                    }
                                }
                            }
                            if (Ta_IsLDAbl(13, tSide) && SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xff, 4) == 0 && (Unit & 0xff) != 0) {
                                eObj3 = ConvUnitIDtoFlagList(eObj3, Unit >> 8, Unit & 0xff);
                            }
                        }
                    }
                }
            }
        }
        if (eObj > 0 || eObj2 > 0 || eObj3 > 0) {
            eInfo->eNO = 0x71;
            eInfo->dwFrom = eObj | 0x80000000;
            eInfo->dwTo = eObj2;
            eInfo->dwPar = eObj3;
            return (PrNo + 1) << 16 | 1;
        }
        return (PrNo + 1) << 16;
    case 3:
        for (i = 1; i <= 5; i++) {
            if (SzDuel_GetUnitStatus(tSide, i, 4) == 1) {
                SzData_CalcExp(tSide, SzDuel_GetUnitCardNo(tSide, i), 0, 4);
            } else {
                SzData_CalcExp(tSide, SzDuel_GetUnitCardNo(tSide, i), 0, 2);
            }
        }
        fTwice = Ta_SearchUncTypeNaturalEffectUserNum(85);
        for (j = 1; j <= 11; j++) {
            Loc = SzDuel_GetUnitLoc(tSide, j);
            if (Ta_SearchUncTypeNaturalEffectUserNum(71) <= 0) {
            if ((cRest = SzDuel_GetUnitCurseRest(tSide, j)) > 0 && tSide == Ta_IsUnitControl(tSide, j) && SzDuel_GetUnitCurseFlag(tSide, j) == 0 && cRest <= 254) {
                if (fTwice > 0) {
                    SzDuel_SetUnitCurseRest(tSide, j, GetCap(0, cRest - 2));
                } else {
                    SzDuel_SetUnitCurseRest(tSide, j, cRest - 1);
                }
            }
            if ((cRest = SzDuel_GetUnitCurseRest((tSide == 0 ? 1 : 0), j)) > 0 && tSide == Ta_IsUnitControl((tSide == 0 ? 1 : 0), j) && SzDuel_GetUnitCurseFlag((tSide == 0 ? 1 : 0), j) == 0 && cRest <= 254) {
                if (fTwice > 0) {
                    SzDuel_SetUnitCurseRest((tSide == 0 ? 1 : 0), j, GetCap(0, cRest - 2));
                } else {
                    SzDuel_SetUnitCurseRest((tSide == 0 ? 1 : 0), j, cRest - 1);
                }
            }
            }
            Loc2 = SzDuel_GetUnitLoc((tSide == 0 ? 1 : 0), 0);
        if (SzDuel_GetUnit(tSide, j)) {
            if (!(Ta_IsLDAbl(15, (tSide == 0 ? 1 : 0)) && Ta_IsLD_CurseObj((tSide == 0 ? 1 : 0), SzDuel_GetUnitKind(tSide, j) & 0x1f) &&
                  Ta_IsSquareRange(Loc2 & 0xff, Loc2 >> 8, Loc & 0xff, Loc >> 8, Ta_GetLDRange((tSide == 0 ? 1 : 0))))) {
                if (!TaPerm_IsPermCurseActive(tSide, j)) {
                    SzDuel_SetUnitSpellRest(tSide, j, 0);
                }
            }
        }
            Loc = SzDuel_GetUnitLoc((tSide == 0 ? 1 : 0), j);
            Loc2 = SzDuel_GetUnitLoc(tSide, 0);
        if (SzDuel_GetUnit((tSide == 0 ? 1 : 0), j)) {
            if (!(Ta_IsLDAbl(15, tSide) && Ta_IsLD_CurseObj(tSide, SzDuel_GetUnitKind((tSide == 0 ? 1 : 0), j) & 0x1f) &&
                  Ta_IsSquareRange(Loc2 & 0xff, Loc2 >> 8, Loc & 0xff, Loc >> 8, Ta_GetLDRange(tSide)))) {
                if (!TaPerm_IsPermCurseActive((tSide == 0 ? 1 : 0), j)) {
                    SzDuel_SetUnitSpellRest((tSide == 0 ? 1 : 0), j, 0);
                }
            }
        }
        }
        for (i = 0; i <= 11; i++) {
            SzDuel_SetUnitCurseFlag(tSide, i, 0);
            SzDuel_SetUnitCurseFlag((tSide == 0 ? 1 : 0), i, 0);
        }
        return (PrNo + 1) << 16;
    case 4:
        for (i = 0; i < 8; i++) {
            Loc = Ta_GetSommonPiecePreference(tSide, i);
            if (Loc >= 0) {
                Unit = SzDuel_GetMapUnitID(Loc & 0xff, Loc >> 8);
                if (Unit >= 0) {
                    if (tSide == Ta_IsUnitControl(Unit >> 8, Unit & 0xff)) {
                        if (SzDuel_GetField(Loc & 0xff, Loc >> 8) != 8) {
                            return (PrNo + 1) << 16;
                        }
                    }
                } else {
                    if (SzDuel_GetField(Loc & 0xff, Loc >> 8) != 8) {
                        return (PrNo + 1) << 16;
                    }
                }
            }
        }
        return (PrNo + 1) << 16 | 5;
    case 5:
        if (tSide != SzDuel_GetTurnFirst()) {
            if (SzDuel_GetTurnCount() >= 100) {
                return 0xff0008;
            }
        }
        return 0xff0000;
    }
    return 0xff0000;
}
