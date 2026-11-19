#ifndef DOTR_EE_CARD_SCALE_ON_H
#define DOTR_EE_CARD_SCALE_ON_H

/* CARD_SCL_ON_ effect steps that grow a standing-by card's scale vector towards a fixed limit.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */
/* A four-float vector (packed_item.h: DotrStateVector). */
struct DotrCardScaleVec {
    float x, y, z, w;
};
/* SzEffect_IsCardStandby2 (0x1f5af0) and SzEffect_SetCardScale2 (0x1f5d10), declared in
 * packed_item.h with these parameters (the vector type is that group's view). */
extern int func_001f5af0_card(int packed);
extern int func_001f5d10_card(int packed, struct DotrCardScaleVec *v);

/* 0x107510 CARD_SCL_ON_1: when the card is standing by, calls SzEffect_SetCardScale2 with
 * the local vector BEFORE anything is stored in it (the call target is 0x1f5d10 both times;
 * SzEffect_GetCardScale2 is at 0x1f5d70), then adds step to x, y and z, clamping each to 0.1
 * (compared as double through the runtime routines fptodp 0x1a2370 and _dpfgt 0x17b3f0, which
 * the compiler calls; stored as 0.1f), and calls SzEffect_SetCardScale2 again with the vector;
 * w is never written. Returns 0. Assumed: int card and result. */
int CARD_SCL_ON_1(int card, float step);

#endif
