#ifndef DOTR_EE_TASK_FINDERS_H
#define DOTR_EE_TASK_FINDERS_H

/* The 21 file-local copies of TaLogic_GetMinimumTask (one per TaLogicNNN unit): each picks the
 * next active unit, then runs the order's first and second purpose through two switches of
 * mission calls.
 * The purpose codes the switches test (each byte of wPurpose): 2 attack, 3 confirm unit
 * (move, maybe to defend), 5 run, 6 move to a location, 9 enchant, 10 summon, 11 trap,
 * 12 revive, 13 revolt, 14 do nothing.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The 16-byte order record and the 20-byte result record. Member names and offsets are those
 * the Japanese build's debug information gives for the parameters of
 * TaLogic_GetMinimumTask_EncMission (driver.c), which these functions call with the same two
 * pointers; assumed to hold for this build. Other groups view the same storage as struct
 * DotrRuleOrder / DotrRuleEntry and struct DotrRuleStep / DotrRuleResult. */
struct DotrFinderUnit {
    unsigned short wUnit;
    unsigned short wOrder;
    unsigned short wPurpose;
    unsigned int dwGoal;
    unsigned char bCond : 2;
};
struct DotrFinderTask {
    unsigned short wUnit;
    unsigned char bPurpose;
    unsigned short wGoal;
    unsigned char bHand[5];
    int bOpen;
    unsigned char bDefense;
};
/* The list as the copies that use the 4-bit member at +417 need it (struct DotrRuleSet in
 * lower_rules.h: 22 entries, 32 ids from +352, the bit-field byte at +416, this byte at
 * +417). The other copies only index the records and take the list as a pointer to the
 * first one. */
struct DotrFinderSet {
    struct DotrFinderUnit unit[26];
    unsigned char pad;
    unsigned char m : 4;
};

/* Callees shared by every copy, as these callers pass their arguments: SzDuel_GetTurnOwner,
 * TaLogic_GetActionUnitFromOwnerID (two views of its list argument), TaLogic_InitRogicControl,
 * SzDuel_GetUnitStatus, TaLogic_GetMinimumTask_EncMission and TaLogic_GetMinimumTask_DoNone. */
extern int func_001de540_fnd(void);
extern int func_00276c80_fnd(struct DotrFinderUnit *list, int id);
extern void func_0027b1b0_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_001e2100_fnd(int a, int b, int bit);
extern int func_0027d020_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int view, int lvl, int g, int h);
extern void func_00276ec0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task);
extern int func_00276c80_fnd2(struct DotrFinderSet *list, int id);

/* Each copy's own callees (its unit's GetNextActiveUnit, CommandUnit, the Run / Atk / Move
 * and other missions, the step finder, GetWarnHideLvl, IsActiveMode and its InitRogicControl
 * copy where it has one). All results and the `first`, `e`, `g`, `m` parameters are int
 * unless a caller shows otherwise: 0x12f930's last parameter is unsigned char (with int the
 * last two arguments are loaded in the other order). */
extern int func_0010d080_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_0010ce20_fnd(struct DotrFinderUnit *list, struct DotrFinderUnit *unit);
extern int func_0010fa60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0010fff0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001106f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0010dd50_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0010eda0_fnd(int side);
extern int func_001170a0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00116b60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00113370_fnd(struct DotrFinderSet *list, int side, int mode, struct DotrFinderTask *task);
extern int func_00117340_fnd(void);
extern void func_00113140_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_00115850_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00115e20_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001162f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00114080_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00117c30_fnd(int side);
extern int func_0011b0e0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern void func_0011c3e0_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_00118510_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00118330_fnd(struct DotrFinderUnit *unit);
extern int func_0011a4d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0011aa70_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0011b680_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00119300_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0011aa10_fnd(int side);
extern int func_00120150_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern void func_001215e0_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_0011cd30_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_0011cb20_fnd(struct DotrFinderUnit *unit);
extern int func_0011f540_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0011fae0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001206f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0011dfb0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0011fa80_fnd(int side);
extern int func_00121e90_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00126940_fnd(struct DotrFinderUnit *unit, int mode);
extern void func_00121ce0_fnd(struct DotrFinderUnit *unit);
extern int func_00124f60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00125500_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00125b70_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00122d30_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_001254a0_fnd(int side);
extern int func_00129700_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern void func_0012b830_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_0012aac0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001273e0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_001271f0_fnd(struct DotrFinderUnit *unit);
extern int func_001291c0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00129d00_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0012a2e0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00127e30_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0012b1b0_fnd(int side);
extern int func_0012ed70_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m);
extern int func_00130120_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m);
extern int func_0012c3f0_fnd(struct DotrFinderSet *list, int side, struct DotrFinderTask *task);
extern int func_00130cb0_fnd(void);
extern void func_0012c1a0_fnd(struct DotrFinderUnit *unit, int m);
extern int func_0012e820_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m);
extern int func_0012f350_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int m);
extern int func_0012f930_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, unsigned char m);
extern int func_0012d270_fnd(int pos, int side, struct DotrFinderTask *task, int m);
extern int func_00130780_fnd(int side);
extern int func_00131750_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00131540_fnd(struct DotrFinderUnit *unit);
extern int func_001336a0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00133be0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001341c0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00132460_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00134ae0_fnd(int side);
extern int func_00138780_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00138950_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00135970_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00135790_fnd(struct DotrFinderUnit *unit);
extern int func_001374d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001379f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00137fb0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00136210_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0014f9b0_fnd(int side);
extern int func_0013a010_fnd(struct DotrFinderSet *list, int side, int mode, struct DotrFinderTask *task);
extern int func_0013d280_fnd(void);
extern void func_00139d90_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_0013be60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0013c430_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0013ca10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0013ab70_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0013da70_fnd(int side);
extern int func_001415f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0013e3d0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_0013e1d0_fnd(struct DotrFinderUnit *unit);
extern int func_001402f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00140830_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00140e10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0013eee0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00141b30_fnd(int side);
extern int func_00142b70_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_001429b0_fnd(struct DotrFinderUnit *unit);
extern int func_00144ec0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00145400_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001459e0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00143ba0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_001461c0_fnd(int side);
extern int func_0014aab0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00147100_fnd(struct DotrFinderSet *list, int side, int mode, struct DotrFinderTask *task);
extern int func_0014aca0_fnd(void);
extern void func_00146ec0_fnd(struct DotrFinderUnit *unit, int mode);
extern int func_00149690_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00149c60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0014a240_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00148020_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0014b330_fnd(int side);
extern int func_0014bfd0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_0014bde0_fnd(struct DotrFinderUnit *unit);
extern int func_0014df80_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0014e550_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0014eb30_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0014cba0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00153550_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00150470_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00150270_fnd(struct DotrFinderUnit *unit);
extern int func_00152100_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001526d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00152cd0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_00150c40_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00153ed0_fnd(int side);
extern int func_00154ad0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_001547a0_fnd(struct DotrFinderUnit *unit);
extern int func_00157480_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00156fc0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00157a50_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, int g);
extern int func_00155770_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00158780_fnd(int side);
extern int func_0015c670_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00159380_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_001591b0_fnd(struct DotrFinderUnit *unit);
extern int func_0015b250_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0015b820_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0015be00_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0015a360_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0015d120_fnd(int side);
extern int func_00161ea0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00161bf0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0015daf0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_0015d8e0_fnd(struct DotrFinderUnit *unit);
extern int func_001607d0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00160da0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00161380_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0015f300_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00162580_fnd(int side);
extern int func_00166d10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00163870_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00163660_fnd(struct DotrFinderUnit *unit);
extern int func_001658f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00165ec0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001664a0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_001643f0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_001676d0_fnd(int side);
extern int func_00167e80_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_00167d30_fnd(struct DotrFinderUnit *unit);
extern int func_00169a10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00169fe0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0016a5c0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_001687a0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_0016b0e0_fnd(int side);
extern int func_00172000_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00171d60_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_0016d8c0_fnd(struct DotrFinderUnit *list, int side, struct DotrFinderTask *task);
extern void func_0016d6a0_fnd(struct DotrFinderUnit *unit);
extern int func_00170940_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_00170f10_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first);
extern int func_001714f0_fnd(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e);
extern int func_0016f4d0_fnd(int pos, int side, struct DotrFinderTask *task);
extern int func_00172310_fnd(int side);

/* The finders. The callers (lower_rules.h) pass the side as a third argument that none of
 * them reads. The list is the first record, or the DotrFinderSet view in the copies that use
 * the member at +417. */
int TaLogic_GetMinimumTask__0010c940(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00112b90(struct DotrFinderSet *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00117e60(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0011c600(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00121800(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00126c80(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0012bb70(struct DotrFinderSet *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00131060(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00135230(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00139890(struct DotrFinderSet *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0013dca0(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__001424b0(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00146960(struct DotrFinderSet *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0014b900(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0014fd40(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00154220(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00158c80(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0015d350(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__00163130(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__001678b0(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);
int TaLogic_GetMinimumTask__0016d140(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner);

#endif
