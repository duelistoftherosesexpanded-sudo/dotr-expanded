#include "dotr/ee/pick_copies.h"

/* The hand walk of TaLogic001_GetSpecCardPlay without the summon test, written out six times in the
 * original with a constant card number; each expansion has its own index and cursor. */
#define SPELL_TRY(I, P, ID) \
    I = 0; \
    if (0 < TaHand_GetNextHandNum(side)) { \
        P = ids; \
        do { \
            if (*P == ID) { \
                minTask[6] = I; \
                SzDuel_InitCardInfo(unit, ids[I]); \
                return TaHand_GetCardKind(ids[I]) >> 5; \
            } \
            P++; \
            I++; \
        } while (I < TaHand_GetNextHandNum(side)); \
    }

int TaLogic005_GetSpecialSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit)
{
    int ids[5];
    /* One index, cursor and counter per expansion. MATCHING DEVICE: this declaration order gives
     * the original registers. */
    int *p2;
    int c2;
    int c1;
    int waterCount;
    int *p1;
    int i1;
    int j1;
    int *p5;
    int i0;
    int *p3;
    int i5;
    int *p4;
    int i3;
    int j2;
    int j0;
    int i4;
    int i2;
    int *p0;

    waterCount = 0;
    TaHand_GetNextHand(side, ids);
    SPELL_TRY(i0, p0, 829) /* Mirror Wall */
    if (TaLogic005_IsDoCerem(side, 844) == 1) {
        SPELL_TRY(i1, p1, 844) /* Turtle Oath */
    }
    SPELL_TRY(i2, p2, 818) /* Shadow of Eyes */
    for (j0 = 0; j0 <= 5; j0++) {
        if (side == func_00250360_pair(side, j0) && SzDuel_GetUnitAttr(side, j0) == 4) {
            waterCount++;
        }
        if (side == func_00250360_pair(side == 0 ? 1 : 0, j0) && SzDuel_GetUnitAttr(side == 0 ? 1 : 0, j0) == 4) {
            waterCount++;
        }
    }
    if (waterCount >= 3) {
        SPELL_TRY(i3, p3, 742) /* Aqua Chorus, with three water monsters */
    }
    c1 = 0;
    for (j1 = 0; j1 < 11; j1++) {
        if (side == func_00250360_pair(side, j1) && SzDuel_GetUnitCurseRest(side, j1) == 255) {
            c1++;
        }
        if (side == func_00250360_pair(side == 0 ? 1 : 0, j1) && SzDuel_GetUnitCurseRest(side == 0 ? 1 : 0, j1) == 255) {
            c1++;
        }
    }
    if (c1 > 0) {
        SPELL_TRY(i4, p4, 796) /* Cursebreaker */
    }
    c2 = 0;
    for (j2 = 6; j2 < 11; j2++) {
        if (side == func_00250360_pair(side, j2) && SzDuel_GetUnitCardNo(side, j2) == 829) {
            c2++;
        }
        if (side == func_00250360_pair(side == 0 ? 1 : 0, j2) && SzDuel_GetUnitCardNo(side == 0 ? 1 : 0, j2) == 829) {
            c2++;
        }
    }
    if (c2 > 0) {
        SPELL_TRY(i5, p5, 820) /* Fake Trap */
    }
    return -1;
}
