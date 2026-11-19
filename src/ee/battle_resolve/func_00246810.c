#include "dotr/ee/battle_resolve.h"

/* Battle (0x246810): resolves an attack of unit (nSide_1, ID_1) on unit (nSide_2, ID_2) at
 * field cell (nX, nZ) and writes the event into eInfo. Both units of kind group 0: a direct
 * attack (ID_1 == 0) gives event 0x5d; otherwise Manage_AtkEfc, then the AP/DP comparison by
 * the attacker's status bit 3, gives event 0x5e with destroy flags and life-point damage, and
 * experience through SzData_CalcExp. Other kind pairs give 0x5f, or the enchantment events
 * 0x24, 0x25 and 0x2b from Chk_Enc.
 * The callees are declared by the header's prototypes. */
/* Sign-magnitude form: bit 15 the sign, the low bits the magnitude. */
static inline int battle_encode(int value)
{
    return ((value < 0 ? 1 : 0) << 15) | (value >= 0 ? value : -value);
}

int Battle(int nSide_1, int ID_1, int nSide_2, int ID_2, int nX, int nZ, void *eInfo)
{
    struct DotrBattleResolveAtkEfc status;
    struct DotrBattleResolveAtkEfc *p = &status;
    int *out = eInfo;
    int damageDealt;
    int sp138;
    int unit1FinalDP;
    int unit1FinalAP;
    int unit2FinalAP;
    int cardTypeTwo;
    int temp_a0_2;
    int temp_s0;
    int temp_s0_10;
    int temp_s0_11;
    int temp_s0_2;
    int temp_s0_3;
    int temp_s0_4;
    int temp_s0_5;
    int temp_s0_6;
    int temp_s0_7;
    int temp_s0_8;
    int temp_s0_9;
    int cardTypeOne;
    int temp_s1_2;
    int temp_s1_3;
    int temp_s5;
    int temp_s5_2;
    int temp_v0_20;
    int temp_v0_21;
    int attribAdv;
    int atkEfcResult;
    int temp_v0_7;
    int var_fp;
    int var_s7;

    p->POfs1 = 0;
    p->POfs2 = 0;
    p->AttrAdv = 0;
    p->flags.fGuard1 = 0;
    p->flags.fGuard2 = 0;
    p->flags.fRemove = 0;
    p->flags.fNoEq1 = 0;
    p->flags.fNoEq2 = 0;
    p->head.fDestroy_1 = 0;
    p->head.fDestroy_2 = 0;
    p->head.Do_APDP = 1;
    p->flags.fLand1 = 0;
    p->flags.fLand2 = 0;
    /* Card type (kind >> 5): 0 monster, 1 magic, 2 power-up, 3 and 4 trap, 5 ritual. */
    cardTypeOne = SzDuel_GetUnitKind(nSide_1, ID_1) >> 5;
    cardTypeTwo = SzDuel_GetUnitKind(nSide_2, ID_2) >> 5;
    damageDealt = 0;
    var_fp = 0;
    var_s7 = 0;
    if ((cardTypeOne == 0) && (cardTypeTwo == 0)) {
        if (ID_1 == 0) {
            temp_s1_2 = Manage_AtkEfc(nSide_1, ID_1, nSide_2, ID_2, p);
            temp_s5 = Ta_GetParamOfs(SzDuel_GetUnit(nSide_2, ID_2), nX, nZ, 1);
            temp_s5_2 = p->POfs2 + SzDuel_GetUnitAP(nSide_2, ID_2, 0);
            temp_s5_2 = GetParCap(temp_s5_2 + temp_s5);
            SzDuel_CalcUnitAP(nSide_2, ID_2, 0, p->POfs2);
            SzDuel_CalcUnitDP(nSide_2, ID_2, 0, p->POfs2);
            p->POfs2 = battle_encode(p->POfs2);
            if (Ta_IsLDAbl(6, nSide_1) != 0) {
                out[0] = 0x5D;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = nSide_1;
                out[3] = (((p->flags.fLand2 << 1) | ((temp_s1_2 >> 0x18) & 1)) << 0x16) | ((temp_s5_2 >> 1) | (((p->flags.fLand1 << 1) | ((temp_s1_2 >> 0x10) & 1)) << 0x14));
            } else {
                out[0] = 0x5D;
                out[1] = (int) ((nSide_2 << 8) | ID_2);
                out[2] = nSide_1;
                out[3] = (((p->flags.fLand2 << 1) | ((temp_s1_2 >> 0x18) & 1)) << 0x16) | (temp_s5_2 | (((p->flags.fLand1 << 1) | ((temp_s1_2 >> 0x10) & 1)) << 0x14));
            }
            return 1;
        }
        temp_s1_3 = SzDuel_GetUnitAttr(nSide_2, ID_2);
        attribAdv = ChkAttrAdv(temp_s1_3, SzDuel_GetUnitAttr(nSide_1, ID_1));
        switch (attribAdv) {
        case 1:
            if (SzDuel_GetUnitCurseRest(nSide_1, ID_1) == 0) {
                var_fp = 1;
            }
            break;
        case -1:
            if (SzDuel_GetUnitCurseRest(nSide_2, ID_2) == 0) {
                var_s7 = 1;
            }
            break;
        }
        atkEfcResult = Manage_AtkEfc(nSide_1, ID_1, nSide_2, ID_2, p);
        if ((atkEfcResult & 0xFF) == 1) {
            temp_a0_2 = (atkEfcResult >> 0x18) & 1;
            if ((temp_a0_2 == 1) && (((atkEfcResult >> 0x10) & 1) == 1)) {
                out[0] = 0x5E;
                out[1] = (int) ((nSide_2 << 8) | ID_2);
                out[2] = (int) ((nSide_1 << 8) | ID_1);
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | 1) << 0x16) | ((((p->flags.fLand1 << 1) | 1) << 0x14) | 0xCFFFF)))) | 0x10000000);
                temp_s0 = SzDuel_GetUnitCardNo(nSide_1, ID_1);
                                SzData_CalcExp(nSide_1, temp_s0, 0, (SzDuel_GetUnitLevel(nSide_2, ID_2, 0) + 2) * 2);
                temp_s0_2 = SzDuel_GetUnitCardNo(nSide_2, ID_2);
                                SzData_CalcExp(nSide_2, temp_s0_2, 0, (SzDuel_GetUnitLevel(nSide_1, ID_1, 0) + 2) * 2);
                return 1;
            }
            if (temp_a0_2 == 1) {
                out[0] = 0x5E;
                out[1] = (int) ((nSide_2 << 8) | ID_2);
                out[2] = (int) ((nSide_1 << 8) | ID_1);
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | 1) << 0x16) | (((p->flags.fLand1 << 1) << 0x14) | 0x8FFFF)))) | 0x10000000);
                temp_s0_3 = SzDuel_GetUnitCardNo(nSide_1, ID_1);
                                SzData_CalcExp(nSide_1, temp_s0_3, 0, (SzDuel_GetUnitLevel(nSide_2, ID_2, 0) + 2) * 2);
                return 1;
            }
            if (((atkEfcResult >> 0x10) & 1) == 1) {
                out[0] = 0x5E;
                out[1] = (int) ((nSide_2 << 8) | ID_2);
                out[2] = (int) ((nSide_1 << 8) | ID_1);
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((p->flags.fLand2 << 0x17) | ((((p->flags.fLand1 << 1) | 1) << 0x14) | 0x4FFFF)))));
                temp_s0_4 = SzDuel_GetUnitCardNo(nSide_2, ID_2);
                                SzData_CalcExp(nSide_2, temp_s0_4, 0, (SzDuel_GetUnitLevel(nSide_1, ID_1, 0) + 2) * 2);
                return 1;
            }

        }
        temp_v0_7 = SzDuel_GetUnitStatus(nSide_1, ID_1, 3);
        switch (temp_v0_7) {
        case 1:
            if (p->flags.fNoEq1 == 1) {
                SzDuel_CalcUnitAP(nSide_1, ID_1, 4, 0);
                SzDuel_CalcUnitDP(nSide_1, ID_1, 4, 0);
                unit1FinalDP = GetParCap(SzDuel_GetUnitDP(nSide_1, ID_1, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_1, ID_1), nX, nZ, 0));
            } else {
                unit1FinalDP = GetParCap(p->POfs1 + SzDuel_GetUnitDP(nSide_1, ID_1, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_1, ID_1), nX, nZ, 0));
                SzDuel_CalcUnitAP(nSide_1, ID_1, 0, p->POfs1);
                SzDuel_CalcUnitDP(nSide_1, ID_1, 0, p->POfs1);
                p->POfs1 = battle_encode(p->POfs1);
            }
            if (p->flags.fNoEq2 == 1) {
                SzDuel_CalcUnitAP(nSide_2, ID_2, 4, 0);
                SzDuel_CalcUnitDP(nSide_2, ID_2, 4, 0);
                sp138 = GetParCap(SzDuel_GetUnitAP(nSide_2, ID_2, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_2, ID_2), nX, nZ, 1));
            } else {
                sp138 = GetParCap(p->POfs2 + SzDuel_GetUnitAP(nSide_2, ID_2, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_2, ID_2), nX, nZ, 1));
                SzDuel_CalcUnitAP(nSide_2, ID_2, 0, p->POfs2);
                SzDuel_CalcUnitDP(nSide_2, ID_2, 0, p->POfs2);
                p->POfs2 = battle_encode(p->POfs2);
            }
            if (unit1FinalDP > sp138) {

                if (p->flags.fGuard2 == 0) {
                    damageDealt += unit1FinalDP - sp138;

                }
                out[0] = 0x5E;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = (int) ((p->POfs1 << 0x10) | ((nSide_1 << 8) | ID_1));
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | ((atkEfcResult >> 0x18) & 1)) << 0x16) | (damageDealt | 0x20000 | (((p->flags.fLand1 << 1) | ((atkEfcResult >> 0x10) & 1)) << 0x14)) | 0x01000000))) | 0x10000000);
                return 1;
            }
            if (unit1FinalDP == sp138) {
                out[0] = 0x5E;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = (int) ((p->POfs1 << 0x10) | ((nSide_1 << 8) | ID_1));
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | ((atkEfcResult >> 0x18) & 1)) << 0x16) | (((p->flags.fLand1 << 1) | ((atkEfcResult >> 0x10) & 1)) << 0x14) | 0x01000000))));
                return 1;
            }
            if (unit1FinalDP < sp138) {
                out[0] = 0x5E;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = (int) ((p->POfs1 << 0x10) | ((nSide_1 << 8) | ID_1));
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | ((atkEfcResult >> 0x18) & 1)) << 0x16) | ((((p->flags.fLand1 << 1) | ((atkEfcResult >> 0x10) & 1)) << 0x14) | 0x40000)))));
                temp_s0_5 = SzDuel_GetUnitCardNo(nSide_2, ID_2);
                                SzData_CalcExp(nSide_2, temp_s0_5, 0, (SzDuel_GetUnitLevel(nSide_1, ID_1, 0) + 2) * 2);
                return 1;
            }
            break;
        case 0:
            if (p->flags.fNoEq1 == 1) {
                SzDuel_CalcUnitAP(nSide_1, ID_1, 4, 0);
                SzDuel_CalcUnitDP(nSide_1, ID_1, 4, 0);
                unit1FinalAP = GetParCap(SzDuel_GetUnitAP(nSide_1, ID_1, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_1, ID_1), nX, nZ, 1));
            } else {
                unit1FinalAP = GetParCap(p->POfs1 + SzDuel_GetUnitAP(nSide_1, ID_1, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_1, ID_1), nX, nZ, 1));
                SzDuel_CalcUnitAP(nSide_1, ID_1, 0, p->POfs1);
                SzDuel_CalcUnitDP(nSide_1, ID_1, 0, p->POfs1);
                p->POfs1 = battle_encode(p->POfs1);
            }
            if (p->flags.fNoEq2 == 1) {
                SzDuel_CalcUnitAP(nSide_2, ID_2, 4, 0);
                SzDuel_CalcUnitDP(nSide_2, ID_2, 4, 0);
                unit2FinalAP = GetParCap(SzDuel_GetUnitAP(nSide_2, ID_2, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_2, ID_2), nX, nZ, 1));
            } else {
                unit2FinalAP = GetParCap(p->POfs2 + SzDuel_GetUnitAP(nSide_2, ID_2, 0) + Ta_GetParamOfs(SzDuel_GetUnit(nSide_2, ID_2), nX, nZ, 1));
                SzDuel_CalcUnitAP(nSide_2, ID_2, 0, p->POfs2);
                SzDuel_CalcUnitDP(nSide_2, ID_2, 0, p->POfs2);
                p->POfs2 = battle_encode(p->POfs2);
            }
            if (unit1FinalAP > unit2FinalAP) {
                if (p->flags.fGuard2 == 0) {
                    damageDealt = unit1FinalAP - unit2FinalAP;
                }
                out[0] = 0x5E;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = (int) ((p->POfs1 << 0x10) | ((nSide_1 << 8) | ID_1));
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | ((atkEfcResult >> 0x18) & 1)) << 0x16) | (damageDealt | 0xA0000 | (((p->flags.fLand1 << 1) | ((atkEfcResult >> 0x10) & 1)) << 0x14))))) | 0x10000000);
                if (damageDealt <= 0) {
                    temp_s0_6 = SzDuel_GetUnitCardNo(nSide_1, ID_1);
                                        SzData_CalcExp(nSide_1, temp_s0_6, 0, (SzDuel_GetUnitLevel(nSide_2, ID_2, 0) + 2) * 2);
                } else {
                    temp_s0_7 = SzDuel_GetUnitCardNo(nSide_1, ID_1);
                                        SzData_CalcExp(nSide_1, temp_s0_7, 0, SzDuel_GetUnitLevel(nSide_2, ID_2, 0) * 4);
                }
                return 1;
            }
            if (unit1FinalAP == unit2FinalAP) {
                out[0] = 0x5E;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = (int) ((p->POfs1 << 0x10) | ((nSide_1 << 8) | ID_1));
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | ((atkEfcResult >> 0x18) & 1)) << 0x16) | ((((p->flags.fLand1 << 1) | ((atkEfcResult >> 0x10) & 1)) << 0x14) | 0xC0000)))) | 0x10000000);
                temp_s0_8 = SzDuel_GetUnitCardNo(nSide_1, ID_1);
                                SzData_CalcExp(nSide_1, temp_s0_8, 0, (SzDuel_GetUnitLevel(nSide_2, ID_2, 0) + 2) * 2);
                temp_s0_9 = SzDuel_GetUnitCardNo(nSide_2, ID_2);
                                SzData_CalcExp(nSide_2, temp_s0_9, 0, (SzDuel_GetUnitLevel(nSide_1, ID_1, 0) + 2) * 2);
                return 1;
            }
            if (unit1FinalAP < unit2FinalAP) {
                if (!(p->flags.fGuard1)) {
                    damageDealt = unit2FinalAP - unit1FinalAP;
                }
                out[0] = 0x5E;
                out[1] = (int) ((p->POfs2 << 0x10) | ((nSide_2 << 8) | ID_2));
                out[2] = (int) ((p->POfs1 << 0x10) | ((nSide_1 << 8) | ID_1));
                out[3] = (int) ((p->flags.fRemove << 0x1B) | ((var_s7 << 0x1A) | ((var_fp << 0x19) | ((((p->flags.fLand2 << 1) | ((atkEfcResult >> 0x18) & 1)) << 0x16) | (damageDealt | 0x50000 | (((p->flags.fLand1 << 1) | ((atkEfcResult >> 0x10) & 1)) << 0x14))))));
                if (damageDealt <= 0) {
                    temp_s0_10 = SzDuel_GetUnitCardNo(nSide_2, ID_2);
                                        SzData_CalcExp(nSide_2, temp_s0_10, 0, (SzDuel_GetUnitLevel(nSide_1, ID_1, 0) + 2) * 2);
                } else {
                    temp_s0_11 = SzDuel_GetUnitCardNo(nSide_2, ID_2);
                                        SzData_CalcExp(nSide_2, temp_s0_11, 0, SzDuel_GetUnitLevel(nSide_1, ID_1, 0) * 4);
                }
                return 1;
            }
            break;
        }
    } else {
        if ((cardTypeOne == 0) && ((cardTypeTwo == 1) || ((unsigned int)(cardTypeTwo - 3) < 3U))) {
            out[0] = 0x5F;
            out[1] = (int) ((nSide_1 << 8) | ID_1);
            out[2] = (int) ((nSide_2 << 8) | ID_2);
            out[3] = 1;
            return 1;
        }
        if (cardTypeOne == 0) {
            if (cardTypeTwo == 2) {
                if (Ta_IsLDRangeAvility(nSide_1, ID_1, 0xB) == 1) {
                    out[0] = 0x5F;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = 3;
                    return 0x10001;
                }
                if (Ta_IsResistAll(nSide_1, ID_1) != 0) {
                    out[0] = 0x5F;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = 1;
                    return 0x10001;
                }
                temp_v0_20 = Chk_Enc(nSide_1, ID_1, SzDuel_GetUnitCardNo(nSide_2, ID_2));
                if (temp_v0_20 == 0x3E7) {
                    out[0] = 0x5F;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = 1;
                    return 0x10001;
                }
                if (((temp_v0_20 >> 0x1E) & 1) == 1) {
                    SzDuel_SetUnitStatus(nSide_2, ID_2, 2, 1);
                    out[0] = 0x2B;
                    out[1] = (int) ((nSide_2 << 8) | ID_2);
                    out[2] = (int) ((nSide_1 << 8) | ID_1);
                    out[3] = (int) ((temp_v0_20 >> 0xE) & 0x3FFF);
                    return 0x10001;
                }
                if ((temp_v0_20 >> 0xE) == 0x219) {
                    SzDuel_SetUnitStatus(nSide_2, ID_2, 2, 1);
                    out[0] = 0x25;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = (int) ((temp_v0_20 & 0x3FFF) | (Ta_GetCurrentSide(nSide_2, ID_2) << 0x10));
                } else {
                    SzDuel_SetUnitStatus(nSide_2, ID_2, 2, 1);
                    out[0] = 0x24;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = temp_v0_20;
                }
                return 0x10001;
            }
        }

        if (cardTypeOne == 2) {
            if (cardTypeTwo == 0) {
                SzData_CalcExp(nSide_2, SzDuel_GetUnitCardNo(nSide_2, ID_2), 0, 6);
                if (Ta_IsLDRangeAvility(nSide_2, ID_2, 0xB) == 1) {
                    out[0] = 0x5F;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = 2;
                    return 1;
                }
                if (Ta_IsResistAll(nSide_2, ID_2) != 0) {
                    out[0] = 0x5F;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = 0;
                    return 1;
                }
                temp_v0_21 = Chk_Enc(nSide_2, ID_2, SzDuel_GetUnitCardNo(nSide_1, ID_1));
                if (temp_v0_21 == 0x3E7) {
                    out[0] = 0x5F;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = 0;
                    return 1;
                }
                if (((temp_v0_21 >> 0x1E) & 1) == 1) {
                    SzDuel_SetUnitStatus(nSide_1, ID_1, 2, 1);
                    out[0] = 0x2B;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = (int) ((temp_v0_21 >> 0xE) & 0x3FFF);
                    return 1;
                }
                if ((temp_v0_21 >> 0xE) == 0x219) {
                    SzDuel_SetUnitStatus(nSide_1, ID_1, 2, 1);
                    out[0] = 0x25;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = (int) ((temp_v0_21 & 0x3FFF) | (Ta_GetCurrentSide(nSide_1, ID_1) << 0x10));
                } else {
                    SzDuel_SetUnitStatus(nSide_1, ID_1, 2, 1);
                    out[0] = 0x24;
                    out[1] = (int) ((nSide_1 << 8) | ID_1);
                    out[2] = (int) ((nSide_2 << 8) | ID_2);
                    out[3] = temp_v0_21;
                }
                return 1;
            }

        }
        if (((unsigned int)(cardTypeOne - 1) < 5U) && ((unsigned int) cardTypeTwo < 6U)) {
            out[0] = 0x5F;
            out[1] = (int) ((nSide_1 << 8) | ID_1);
            out[2] = (int) ((nSide_2 << 8) | ID_2);
            out[3] = 0;
            if (cardTypeTwo == 0) {
                SzData_CalcExp(nSide_2, SzDuel_GetUnitCardNo(nSide_2, ID_2), 0, 6);
            }
            return 1;
        }
    }
    return 0;
}
