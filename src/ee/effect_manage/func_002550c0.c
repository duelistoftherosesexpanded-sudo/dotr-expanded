#include "dotr/ee/effect_manage.h"

/* Runs the sequence function chosen by the effect number (low 15 bits of eInfo->eNO) with the
 * same three arguments. When the sequence returns 0 (also for number 999 and for a number with
 * no sequence), clears status bit 0 and the busy flag of every type-2 unit 1..10 of both sides
 * that has status bit 4. Name, prototype, parameter and local names from the Japanese build's
 * debug information (Effect.c); the two locals are declared in the reverse of that order. */
int Ta_Manage_Effect(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID)
{
    int i;
    int Ret;

    if (eInfo == 0 || pTemp == 0) {
        return 0;
    }
    switch (eInfo->eNO & 0x7FFF) {
    case 0:
        Ret = Ta_Eff_000(eInfo, pTemp, ID);
        break;
    case 1:
        Ret = Ta_Eff_001(eInfo, pTemp, ID);
        break;
    case 2:
        Ret = Ta_Eff_002(eInfo, pTemp, ID);
        break;
    case 3:
        Ret = Ta_Eff_003B(eInfo, pTemp, ID);
        break;
    case 4:
        Ret = Ta_Eff_004(eInfo, pTemp, ID);
        break;
    case 5:
        Ret = Ta_Eff_005(eInfo, pTemp, ID);
        break;
    case 6:
        Ret = Ta_Eff_006(eInfo, pTemp, ID);
        break;
    case 7:
        Ret = Ta_Eff_007(eInfo, pTemp, ID);
        break;
    case 8:
        Ret = Ta_Eff_008(eInfo, pTemp, ID);
        break;
    case 9:
        Ret = Ta_Eff_009(eInfo, pTemp, ID);
        break;
    case 12:
        Ret = Ta_Eff_012(eInfo, pTemp, ID);
        break;
    case 13:
        Ret = Ta_Eff_013(eInfo, pTemp, ID);
        break;
    case 14:
        Ret = Ta_Eff_014(eInfo, pTemp, ID);
        break;
    case 15:
        Ret = Ta_Eff_015(eInfo, pTemp, ID);
        break;
    case 16:
        Ret = Ta_Eff_016(eInfo, pTemp, ID);
        break;
    case 17:
        Ret = Ta_Eff_017_A(eInfo, pTemp, ID);
        break;
    case 18:
        Ret = Ta_Eff_018(eInfo, pTemp, ID);
        break;
    case 19:
        Ret = Ta_Eff_019(eInfo, pTemp, ID);
        break;
    case 20:
        Ret = Ta_Eff_020(eInfo, pTemp, ID);
        break;
    case 21:
        Ret = Ta_Eff_021(eInfo, pTemp, ID);
        break;
    case 23:
        Ret = Ta_Eff_023(eInfo, pTemp, ID);
        break;
    case 25:
        Ret = Ta_Eff_025(eInfo, pTemp, ID);
        break;
    case 26:
        Ret = Ta_Eff_026(eInfo, pTemp, ID);
        break;
    case 28:
        Ret = Ta_Eff_028(eInfo, pTemp, ID);
        break;
    case 29:
        Ret = Ta_Eff_029(eInfo, pTemp, ID);
        break;
    case 91:
        Ret = Ta_Eff_029_CNG(eInfo, pTemp, ID);
        break;
    case 30:
        Ret = Ta_Eff_030(eInfo, pTemp, ID);
        break;
    case 31:
        Ret = Ta_Eff_031(eInfo, pTemp, ID);
        break;
    case 32:
        Ret = Ta_Eff_032(eInfo, pTemp, ID);
        break;
    case 33:
        Ret = Ta_Eff_033(eInfo, pTemp, ID);
        break;
    case 34:
        Ret = Ta_Eff_034(eInfo, pTemp, ID);
        break;
    case 35:
        Ret = Ta_Eff_035(eInfo, pTemp, ID);
        break;
    case 90:
        Ret = Ta_Eff_017_MEGUMI(eInfo, pTemp, ID);
        break;
    case 92:
        Ret = Ta_Eff_006_WIND(eInfo, pTemp, ID);
        break;
    case 93:
        Ret = Ta_Eff_BTL_000(eInfo, pTemp, ID);
        break;
    case 36:
        Ret = Ta_Eff_036(eInfo, pTemp, ID);
        break;
    case 37:
        Ret = Ta_Eff_037(eInfo, pTemp, ID);
        break;
    case 94:
        Ret = Ta_Eff_BTL_001(eInfo, pTemp, ID);
        break;
    case 95:
        Ret = Ta_Eff_BTL_002(eInfo, pTemp, ID);
        break;
    case 43:
        Ret = Ta_Eff_100(eInfo, pTemp, ID);
        break;
    case 44:
        Ret = Ta_Eff_044(eInfo, pTemp, ID);
        break;
    case 45:
        Ret = Ta_Eff_045(eInfo, pTemp, ID);
        break;
    case 46:
        Ret = Ta_Eff_046(eInfo, pTemp, ID);
        break;
    case 51:
        Ret = Ta_Eff_051(eInfo, pTemp, ID);
        break;
    case 54:
        Ret = Ta_Eff_054(eInfo, pTemp, ID);
        break;
    case 56:
        Ret = Ta_Eff_056(eInfo, pTemp, ID);
        break;
    case 57:
        Ret = Ta_Eff_057(eInfo, pTemp, ID);
        break;
    case 58:
        Ret = Ta_Eff_058(eInfo, pTemp, ID);
        break;
    case 59:
        Ret = Ta_Eff_059(eInfo, pTemp, ID);
        break;
    case 60:
        Ret = Ta_Eff_060(eInfo, pTemp, ID);
        break;
    case 63:
        Ret = Ta_Eff_063(eInfo, pTemp, ID);
        break;
    case 64:
        Ret = Ta_Eff_064(eInfo, pTemp, ID);
        break;
    case 65:
        Ret = Ta_Eff_065(eInfo, pTemp, ID);
        break;
    case 66:
        Ret = Ta_Eff_066(eInfo, pTemp, ID);
        break;
    case 67:
        Ret = Ta_Eff_067(eInfo, pTemp, ID);
        break;
    case 70:
        Ret = Ta_Eff_070(eInfo, pTemp, ID);
        break;
    case 71:
        Ret = Ta_Eff_071(eInfo, pTemp, ID);
        break;
    case 72:
        Ret = Ta_Eff_072(eInfo, pTemp, ID);
        break;
    case 100:
        Ret = Ta_Eff_100(eInfo, pTemp, ID);
        break;
    case 101:
        Ret = Ta_Eff_101(eInfo, pTemp, ID);
        break;
    case 102:
        Ret = Ta_Eff_102(eInfo, pTemp, ID);
        break;
    case 109:
        Ret = Ta_Eff_109(eInfo, pTemp, ID);
        break;
    case 112:
        Ret = Ta_Eff_112(eInfo, pTemp, ID);
        break;
    case 113:
        Ret = Ta_Eff_113(eInfo, pTemp, ID);
        break;
    case 114:
        Ret = Ta_Eff_114(eInfo, pTemp, ID);
        break;
    case 150:
        Ret = Ta_Eff_Hand_Dest(eInfo, pTemp, ID);
        break;
    case 255:
        Ret = Ta_Eff_ETC(eInfo, pTemp, ID);
        break;
    case 254:
        Ret = Ta_Eff_WAIT(eInfo, pTemp, ID);
        break;
    case 999:
        Ret = 0;
        break;
    default:
        Ret = 0;
        break;
    }
    if (Ret == 0) {
        for (i = 1; i < 11; i++) {
            if (SzDuel_Kind2Type(SzDuel_GetUnitKind(0, i) >> 5) == 2 && SzDuel_GetUnitStatus(0, i, 4) != 0) {
                SzEffect_SetBusyFlag2(i, 0);
                SzDuel_SetUnitStatus(0, i, 0, 1);
            }
            if (SzDuel_Kind2Type(SzDuel_GetUnitKind(1, i) >> 5) == 2 && SzDuel_GetUnitStatus(1, i, 4) != 0) {
                SzDuel_SetUnitStatus(1, i, 0, 1);
                SzEffect_SetBusyFlag2(i | 0x100, 0);
            }
        }
    }
    return Ret;
}
