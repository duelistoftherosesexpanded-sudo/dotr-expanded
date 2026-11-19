#ifndef DOTR_EE_SOUND_GATE_H
#define DOTR_EE_SOUND_GATE_H

#include "dotr/ee/sound_control.h"

/* Offset view of twelve bytes at EE 0x34f6e0: four signed halfwords and a
 * flag byte (field layout assumed). small_state.h declares the flag byte alone as
 * data_0034f6e8. Evidence: docs/modules/engine.md. */
struct DotrSoundGateBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bits_2_7 : 6;
};

struct DotrSoundGate {
    short unknown_00[4];
    struct DotrSoundGateBits unknown_08;
    unsigned char unknown_09[3];
};

typedef char DotrSoundGateMustBe0c[(sizeof(struct DotrSoundGate) == 0x0c) ? 1 : -1];

extern struct DotrSoundGate data_0034f6e0_gate[];
extern char data_002d0930[];
/* Four format-string addresses at EE 0x295090. */
extern char *apchSEGrp[];

void SzSound_Init(void);
void SzSound_Term(void);
int SzSound_TransferSEGrp(int index, int id, int force);
int SzSound_IsPlaySE(void);
int SzSound_PlaySE(int index, int soundEffectNumber);
int SzSound_StereoPlaySE(int index, int a, int b);
int SzSound_StopSE(int index, int value);
int Ta_Eff_036_GetSound(unsigned int *p); /* word 3 >> 14 selects the 0x217710 value */
/* 0x269ec0: post-increment the step counter at +0xd0 of the second object
 * and, by its old value, send SzSound_PlaySE(1, 56) at 0, (1, 57) at 20, (1, 76) at
 * 30 and call Ta_Eff_036_GetSound(p) at 36; any other old value sends (1, 60) when the
 * new count equals 64, 66, 64 or 72 for p[3] >> 14 of 536, 540, 541 or 542 and
 * 48 otherwise. Unless the whole word p[3] is 999, an old value of 37 instead
 * sends (1, 59) for 541 and 542 and nothing for the rest. Only the counter of
 * the second object is known. */
struct DotrSoundStep {
    char unknown_00[0xd0];
    int count_d0;
};
void Ta_Eff_036_ManageSound(unsigned int *p, struct DotrSoundStep *step);
int SzSound_IsLoadingBGM(void);
int SzSound_StopBGM(int value);
int SzSound_SetOutput(int flag);

/* Dependency signatures inferred from the call sites; 0x197548 is a library routine. */
extern void RhMem_FillMemory(void *destination, int value, int size);
extern char *func_00197548(const char *label);

#endif
