#ifndef DOTR_EE_PRESET_WORDS_H
#define DOTR_EE_PRESET_WORDS_H

/* Eight routines that fill four words with fixed values chosen by the result
 * of 0x1de540, and one initializer of an object view. Meanings are
 * unassigned. Evidence: docs/modules/duel-rules.md. */
struct DotrPresetBits {
    signed char unknown_bit_0 : 1;
    unsigned char unknown_bits_1_5 : 5;
    unsigned char unknown_bits_6_7 : 2;
};

/* Prefix view of the object initialized by 0x255cd0. */
struct DotrPresetObject {
    unsigned char unknown_00[0x80];
    struct DotrPresetBits unknown_80;
    unsigned char unknown_81;
    unsigned short unknown_82[24];
    unsigned char unknown_b2[24];
};

typedef char DotrPresetObjectMustBeCa[(sizeof(struct DotrPresetObject) == 0xca) ? 1 : -1];

void TaEffect_ClearEffectTemp(struct DotrPresetObject *visualEffect);
void Ta_GetStdCompleteView(int *out);
void Ta_GetStdTopDownView(int *out);
void Ta_GetFlatCompleteView(int *out);
void Ta_GetDestinyView(int *out);
void Ta_GetStdFocusView(int *out, int value);
void Ta_GetStdFocusView2(int *out, int value);
void Ta_GetTopView(int *out);
void Ta_GetSkyView(int *out);

/* Two callers that convert a preset and submit it. The 16-byte local at
 * the lowest stack offset is written by 0x1e10f0; four floats are assumed. */
struct DotrPresetVector {
    float x, y, z;
    float unknown_w;
};
void TaView_ReqTopView(void);
void TaView_ReqSkyView(void);

/* Defined in the state_records group. */
extern int SzDuel_GetTurnOwner(void);
/* 0x1fa810 and 0x1fc950 are defined in block_table and 0x1e10f0 in
 * state_grid, each with its own pointer types; SzView_ReqCamera (0x1fa8a0,
 * view_request) receives six argument registers here. */
extern int SzView_FreeRequest(int a, int b);
extern void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrPresetVector *v);
extern void SzView_IV2V(int *in, float *out);
extern void SzView_ReqCamera(int a, int b, int kind, int d, int e, void *data);

/* Initializer, id-list append and id-list validation. */
/* 0x255960 initializer view (more of the 0x255cd0 object); the two float
 * sources are assumed to return four-float arrays. */
struct DotrPresetInit {
    unsigned short word_00;
    unsigned short word_02;
    unsigned short word_04;
    unsigned char bytes_06[24];
    int words_20[24];
    unsigned char unknown_80[0x4c];
    int word_cc;
    int word_d0;
    unsigned char byte_d4;
    unsigned char unknown_d5[0x0b];
    float vec_e0[4];
    float vec_f0[4];
    unsigned char byte_100;
    unsigned char unknown_101;
    unsigned short word_102;
};
extern float *func_001f9d30_floats(int index);
extern float *func_001faf60_floats(int index);

/* 0x1f3910 result form, as declared in object_table.h. */
extern int func_001f3910_result(int a, int b, int c);

/* 0x1f3ea0 entry viewed as a byte whose low three bits are a state; pool_table.h
 * declares it with another return type, so do not include both. */
struct DotrJobPool {
    unsigned char state : 3;
    unsigned char rest : 5;
};
extern struct DotrJobPool *SzEffect_GetModuleInfo(int a, int b);

void InitEffectTemp(struct DotrPresetInit *effectAnimationParams);
int TaEffect_LoadModule(struct DotrPresetObject *obj, int moduleId);
int TaEffect_IsLoadModule(struct DotrPresetObject *effectModule);

/* Object whose word at +0xc holds a kind in bits 14 and up (assumed layout). */
struct DotrPresetKindView {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    unsigned unknown_0c;
};
int Ta_Eff_036_LoadEffect(struct DotrPresetKindView *obj, struct DotrPresetObject *target);
/* Defined in view_requests; (block, four words out) as TaView_ReqRotCamera passes them. */
extern void SzView_V2IV(float *block, int *out);
/* Defined in the side_table group. */
void TaView_ReqRotCamera(unsigned char b, int delta);
/* View of SzView_ReqCamera whose fourth parameter is 16 bits wide: with it the
 * callers below (byte tag held in a saved register) reproduce the original
 * argument order; state_records.h has the same view with a float pointer. */
extern void func_001fa8a0_tag(int a, int b, int kind, unsigned short d, int e, void *data);
/* Words at +0xe0 and +0xf0 and the halfword at +0x102 of TaView_RestoreOriginalView's object. */
struct DotrPresetPair {
    unsigned char unknown_000[0xe0];
    int unknown_0e0[4];
    int unknown_0f0[4];
    unsigned char unknown_100[2];
    short unknown_102;
};
/* All four defined in the side_table group (see TaView_ReqRotCamera). */
void TaView_RestoreOriginalView(struct DotrPresetPair *p, unsigned char b);
void TaTuto_RestoreOriginalView(char *a, unsigned char b);
void TaView_ReqHalfFarView(unsigned char b);
void TaView_ReqFocusCamera2(unsigned char b, int cell, int value);
/* Argument assumed to be a byte flag (only its low 8 bits are tested). */
void TaView_SetDestinyCamera(unsigned char flag);
/* Three submitters like 0x261580 that pass their byte argument as the fourth
 * argument of both 0x1fa8a0 calls (through the 16-bit view above). */
void TaView_ReqStdCompleteView(unsigned char flag);
void TaView_ReqStdTopDownView(unsigned char flag);
void TaView_ReqFlatCompleteView(unsigned char flag);

#endif
