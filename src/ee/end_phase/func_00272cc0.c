#include "dotr/ee/end_phase.h"

/* Awards experience at the end of a duel: units 1 to 5 of nSide get 6 when
 * status bit 4 is set and 4 otherwise; then, by bCond (0 to 3), unit 0 of
 * nSide gains and unit 0 of the other side loses an amount that steps down
 * with each side's graveyard count (below 5, 10, 15, or more). Returns 1,
 * or -1 for any other bCond. Parameter and local names are from the
 * Japanese build's debug information; the declaration order below is the
 * reverse of the recorded one and reproduces the saved registers. */
int Ta_Do_DuelEnd(int nSide, unsigned char bCond) {
    int i;
    int gCount_1;
    int gCount_2;

    for (i = 1; i <= 5; i++) {
        if (SzDuel_GetUnitStatus(nSide, i, 4)) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, i), 0, 6);
        } else {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, i), 0, 4);
        }
    }
    gCount_1 = SzDuel_GetGraveCount(nSide);
    gCount_2 = SzDuel_GetGraveCount(nSide == 0 ? 1 : 0);
    switch (bCond) {
    case 0:
        if (gCount_1 < 5) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 40);
        } else if (gCount_1 < 10) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 32);
        } else if (gCount_1 < 15) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 24);
        } else {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 16);
        }
        if (gCount_2 < 5) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -32);
        } else if (gCount_2 < 10) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -24);
        } else if (gCount_2 < 15) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -16);
        } else {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -8);
        }
        break;
    case 1:
        if (gCount_1 < 5) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 32);
        } else if (gCount_1 < 10) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 24);
        } else if (gCount_1 < 15) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 16);
        } else {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 8);
        }
        if (gCount_2 < 5) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -28);
        } else if (gCount_2 < 10) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -20);
        } else if (gCount_2 < 15) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -12);
        } else {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -4);
        }
        break;
    case 2:
        if (gCount_1 < 5) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 44);
        } else if (gCount_1 < 10) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 36);
        } else if (gCount_1 < 15) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 28);
        } else {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 20);
        }
        if (gCount_2 < 5) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -36);
        } else if (gCount_2 < 10) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -28);
        } else if (gCount_2 < 15) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -20);
        } else {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -12);
        }
        break;
    case 3:
        if (gCount_1 < 5) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 48);
        } else if (gCount_1 < 10) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 40);
        } else if (gCount_1 < 15) {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 32);
        } else {
            SzData_CalcExp(nSide, SzDuel_GetUnitCardNo(nSide, 0), 0, 24);
        }
        if (gCount_2 < 5) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -40);
        } else if (gCount_2 < 10) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -32);
        } else if (gCount_2 < 15) {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -24);
        } else {
            SzData_CalcExp(nSide == 0 ? 1 : 0, SzDuel_GetUnitCardNo(nSide == 0 ? 1 : 0, 0), 0, -16);
        }
        break;
    default:
        return -1;
    }
    return 1;
}
