#include "dotr/ee/end_phase.h"

/* One step of the start-of-turn sequence for the turn owner; PrNo selects
 * the step and the result is (next step << 16) | effect flag, or 0xff0000
 * when the sequence is over. Step 0 advances the turn count (when the owner
 * moved first or a kind-85 natural effect is in use), clears status bit 1
 * of units 1 to 10 on both sides unless their effect word 2 has id 81, and
 * counts down the delay of the owner's units 1 to 5 that have status bits 4
 * and 5 (by two with a kind-85 effect). Steps 32 to 63 offer one unit per
 * step to the standby natural effects: the owner's units 1 to 5 (status bit
 * 1 clear), the other side's units 1 to 5, then the owner's units again
 * (status bit 1 set), and continue at 64 after sixteen steps. Steps 64 to 95
 * rewrite status bit 0 of both sides' units from control, curse, spell and
 * type, then go on as step 128. Steps up to 159 with a complete Exzodia
 * request effect 102 (a trap of kind 17, or 46, on the other side) or
 * effect 109 with the four part units packed three bits apart. Otherwise
 * the summon charge is added from turn 2, the destiny draw flag is set
 * and the hand phase is opened. Prototype, parameter and local names and
 * the two structures are from the Japanese build's debug information; the
 * declaration order below is the reverse of the recorded one, the unused
 * recorded locals sOfs and Unit2 are left out, and PrNo doubles as the
 * loop counter because the original keeps both in one register. */
int Ta_Do_StandbyPhase(int PrNo, struct jp_6826f *eInfo) {
    int i;
    int Ret;
    int cRest;
    int Unit;
    int fTwice;
    int U_EXZ[4];
    int TurnOwner;
    int eID;
    struct jp_6835c *temp;

    TurnOwner = SzDuel_GetTurnOwner();
    cRest = SzDuel_GetSummonCharge(TurnOwner);
    Unit = -1;
    if (PrNo == 0) {
        fTwice = Ta_SearchUncTypeNaturalEffectUserNum(85);
        if (fTwice > 0 || TurnOwner == SzDuel_GetTurnFirst()) {
            SzDuel_SetTurnCount(SzDuel_GetTurnCount() + 1);
        }
        for (i = 1; i < 11; i++) {
            temp = (struct jp_6835c *)Ta_GetUnitEffect(0, i, 2);
            if (temp == 0 || temp->dwID >> 8 != 81) {
                SzDuel_SetUnitStatus(0, i, 1, 0);
            }
            temp = (struct jp_6835c *)Ta_GetUnitEffect(1, i, 2);
            if (temp == 0 || temp->dwID >> 8 != 81) {
                SzDuel_SetUnitStatus(1, i, 1, 0);
            }
        }
        for (PrNo = 1; PrNo <= 5; PrNo++) {
            if ((cRest = SzDuel_GetUnitDelayCount(TurnOwner, PrNo)) > 0 && SzDuel_GetUnitStatus(TurnOwner, PrNo, 4) && SzDuel_GetUnitStatus(TurnOwner, PrNo, 5)) {
                if (fTwice > 0) {
                    SzDuel_SetUnitDelayCount(TurnOwner, PrNo, GetCap(1, cRest - 2));
                } else {
                    SzDuel_SetUnitDelayCount(TurnOwner, PrNo, GetCap(1, cRest - 1));
                }
            }
        }
        return 0x200000;
    }
    if (PrNo < 64) {
        if (PrNo - 32 < 6) {
            if (SzDuel_GetUnit(TurnOwner, PrNo - 32) && PrNo - 32 != 0) {
                if (SzDuel_GetUnitStatus(TurnOwner, PrNo - 32, 4) && SzDuel_GetUnitStatus(TurnOwner, PrNo - 32, 5) && SzDuel_GetUnitStatus(TurnOwner, PrNo - 32, 1) == 0 && Ta_GetUnitEffect(TurnOwner, PrNo - 32, 2)) {
                    Unit = TurnOwner << 8 | (PrNo - 32);
                }
                if (Unit >= 0) {
                    Ret = Ta_ManageNaturalEffectInStandbyPhase(Unit >> 8, Unit & 0xff, (struct jp_65da1 *)eInfo);
                    return (PrNo + 1) << 16 | Ret;
                }
            }
        }
        if (PrNo - 32 < 11) {
            if (SzDuel_GetUnit((TurnOwner == 0 ? 1 : 0), PrNo - 37) && PrNo - 37 != 0) {
                if (SzDuel_GetUnitStatus((TurnOwner == 0 ? 1 : 0), PrNo - 37, 4) && SzDuel_GetUnitStatus((TurnOwner == 0 ? 1 : 0), PrNo - 37, 5) && SzDuel_GetUnitStatus((TurnOwner == 0 ? 1 : 0), PrNo - 37, 1) == 0 && Ta_GetUnitEffect((TurnOwner == 0 ? 1 : 0), PrNo - 37, 2)) {
                    Unit = (TurnOwner == 0 ? 1 : 0) << 8 | (PrNo - 37);
                }
                if (Unit >= 0) {
                    Ret = Ta_ManageNaturalEffectInStandbyPhase2(Unit >> 8, Unit & 0xff, eInfo);
                    return (PrNo + 1) << 16 | Ret;
                }
            }
        }
        if (SzDuel_GetUnit(TurnOwner, PrNo - 42) && PrNo - 42 != 0) {
            if (SzDuel_GetUnitStatus(TurnOwner, PrNo - 42, 4) && SzDuel_GetUnitStatus(TurnOwner, PrNo - 42, 5) && SzDuel_GetUnitStatus(TurnOwner, PrNo - 42, 1) && Ta_GetUnitEffect(TurnOwner, PrNo - 42, 2)) {
                Unit = TurnOwner << 8 | (PrNo - 42);
            }
            if (Unit >= 0) {
                Ret = Ta_ManageNaturalEffectInStandbyPhase(Unit >> 8, Unit & 0xff, (struct jp_65da1 *)eInfo);
                return (PrNo + 1) << 16 | Ret;
            }
        }
        if (PrNo - 32 < 16) {
            PrNo++;
        } else {
            PrNo = 64;
        }
        return PrNo << 16;
    }
    if (PrNo < 96) {
        for (PrNo = 0; PrNo < 11; PrNo++) {
            if (TurnOwner == Ta_GetCurrentSide(0, PrNo)) {
                SzDuel_SetUnitStatus(0, PrNo, 0, 0);
            } else {
                SzDuel_SetUnitStatus(0, PrNo, 0, 1);
            }
            if (TurnOwner == Ta_GetCurrentSide(1, PrNo)) {
                SzDuel_SetUnitStatus(1, PrNo, 0, 0);
            } else {
                SzDuel_SetUnitStatus(1, PrNo, 0, 1);
            }
        }
        for (PrNo = 1; PrNo < 11; PrNo++) {
            if (SzDuel_GetUnitCurseRest(TurnOwner, PrNo) > 0 || SzDuel_GetUnitSpellRest(TurnOwner, PrNo) > 0 || (SzDuel_Kind2Type(SzDuel_GetUnitKind(TurnOwner, PrNo) >> 5) == 2 && SzDuel_GetUnitStatus(TurnOwner, PrNo, 4))) {
                SzDuel_SetUnitStatus(TurnOwner, PrNo, 0, 1);
            }
        }
        for (PrNo = 1; PrNo < 11; PrNo++) {
            if (SzDuel_GetUnitCurseRest((TurnOwner == 0 ? 1 : 0), PrNo) > 0 || SzDuel_GetUnitSpellRest((TurnOwner == 0 ? 1 : 0), PrNo) > 0 || (SzDuel_Kind2Type(SzDuel_GetUnitKind((TurnOwner == 0 ? 1 : 0), PrNo) >> 5) == 2 && SzDuel_GetUnitStatus((TurnOwner == 0 ? 1 : 0), PrNo, 4))) {
                SzDuel_SetUnitStatus((TurnOwner == 0 ? 1 : 0), PrNo, 0, 1);
            }
        }
        PrNo = 128;
    }
    if (PrNo < 160 && Ta_IsExzodia(TurnOwner) == 4) {
        if (PrNo == 128) {
            if ((eID = Ta_IsActiveTrapB((TurnOwner == 0 ? 1 : 0), 17)) >= 0) {
                if ((Ret = Ta_IsActiveTrapB((TurnOwner == 0 ? 1 : 0), 46)) >= 0) {
                    eInfo->eNO = 102;
                    eInfo->dwFrom = 1;
                    eInfo->dwTo = Ret;
                    eInfo->dwPar = 0;
                    return (PrNo + 1) << 16 | 1;
                }
                eInfo->eNO = 102;
                eInfo->dwFrom = 1;
                eInfo->dwTo = eID;
                eInfo->dwPar = 0;
                return 0xa00001;
            }
        }
        for (PrNo = 0; PrNo < 4; PrNo++) {
            U_EXZ[PrNo] = Ta_GetExzodiaPartsUnit(TurnOwner, PrNo);
        }
        eInfo->eNO = 109;
        eInfo->dwFrom = TurnOwner;
        eInfo->dwTo = (U_EXZ[0] & 0xff) << 9 | ((U_EXZ[1] & 0xff) << 6 | ((U_EXZ[2] & 0xff) << 3 | (U_EXZ[3] & 0xff)));
        eInfo->dwPar = 0;
        return TaTuto_IsTuto(SzDuel_GetTurnOwner()) == 0 ? 0xff0005 : 0xff0001;
    }
    if (SzDuel_GetTurnCount() > 1) {
        if (Ta_SearchUncTypeNaturalEffectUserNum(86) > 0) {
            SzDuel_CalcSummonPower(TurnOwner, 0, cRest + Ta_GetSummonCalcRevision(TurnOwner));
        } else {
            SzDuel_CalcSummonPower(TurnOwner, 0, cRest);
        }
    }
    SzDuel_InqDestinyFlag(TurnOwner);
    if ((SzDuel_InqDestinyFlag(TurnOwner) & 0x80) == 0) {
        if ((Ret = Ta_IsDestinyDraw(TurnOwner)) > 0) {
            SzDuel_SetDestinyFlag(TurnOwner, Ret & 0xff);
        }
    }
    SzDuel_SetHandPhase(TurnOwner, 1);
    return 0xff0000;
}
