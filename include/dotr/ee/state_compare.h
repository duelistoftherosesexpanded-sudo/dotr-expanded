#ifndef DOTR_EE_STATE_COMPARE_H
#define DOTR_EE_STATE_COMPARE_H

/* View of only the unit fields read here; the object and flag meanings are unknown.
 * Evidence: docs/modules/duel.md. */
struct DotrStateCompareFlags {
    unsigned char unknown_bits_0_2 : 3;
    signed char unknown_bit_3 : 1;
    unsigned char unknown_bits_4_7 : 4;
};

struct DotrStateCompareReference {
    unsigned char unknown_00[0x57];
    struct DotrStateCompareFlags unknown_57;
};

typedef char DotrStateComparePrefixMustBe58[
    (sizeof(struct DotrStateCompareReference) == 0x58) ? 1 : -1];

extern struct DotrStateCompareReference *SzDuel_GetUnit(int first, int second);
extern int Ta_GetParamOfs(struct DotrStateCompareReference *reference,
                       int first, int second, int mode);
extern int SzDuel_GetUnitAP2(struct DotrStateCompareReference *reference, int ignoreBoosts);
extern int SzDuel_GetUnitDP2(struct DotrStateCompareReference *reference, int ignoreBoosts);
extern int SzDuel_GetTurnOwner(void);

int SzDuel_GetPlayMotion(int a0, int a1, int a2, int a3, int a4, int a5, int a6);

#endif
