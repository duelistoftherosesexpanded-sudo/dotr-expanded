#ifndef DOTR_EE_EFFECT_MANAGE_H
#define DOTR_EE_EFFECT_MANAGE_H

/* Effect.c dispatcher Ta_Manage_Effect (0x2550c0): selects one Ta_Eff_* sequence function by
 * the effect number in eInfo->eNO and passes its three arguments on.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Ta_Manage_Effect (0x2550c0): prototype and the locals Ret and i from the Japanese build's debug
 * information (Effect.c). struct jp_11dc9 has that build's member names; struct jp_11e6e is only
 * passed on and stays opaque here. The sequence functions are declared as this caller passes the
 * arguments (the three incoming argument registers unchanged); other groups declare several of
 * them with their own structure views. Do not include together with effect_seqs.h, preset_waits.h
 * or small_misc.h. */
struct jp_11dc9 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
struct jp_11e6e;
extern int Ta_Eff_000(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_001(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_002(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_003B(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_004(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_005(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_006(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_007(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_008(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_009(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_012(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_013(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_014(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_015(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_016(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_017_A(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_018(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_019(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_020(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_021(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_023(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_025(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_026(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_028(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_029(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_029_CNG(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_030(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_031(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_032(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_033(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_034(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_035(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_017_MEGUMI(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_006_WIND(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_BTL_000(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_036(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_037(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_BTL_001(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_BTL_002(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_100(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_044(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_045(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_046(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_051(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_054(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_056(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_057(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_058(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_059(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_060(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_063(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_064(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_065(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_066(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_067(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_070(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_071(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_072(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_101(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_102(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_109(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_112(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_113(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_114(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_Hand_Dest(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_ETC(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int Ta_Eff_WAIT(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_Kind2Type(int kind);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzEffect_SetBusyFlag2(int packed, int flag);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);

int Ta_Manage_Effect(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID);

#endif
