#ifndef DOTR_EE_CELL_SENDS_H
#define DOTR_EE_CELL_SENDS_H

/* Adjacent functions 0x260af0-0x2610d0 that, after SzView_FreeRequest(0, 0), submit a cell's
 * vector and converted words through SzView_ReqCamera with a tag.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* For struct DotrPresetVector and the shared callees SzView_FreeRequest,
 * SzDuel_CalcUnitPos, SzView_IV2V, Ta_GetStdFocusView and the func_001fa8a0_tag view. */
#include "dotr/ee/preset_words.h"

/* 0x260eb0: after SzView_FreeRequest(0, 0). When SzEffect_IsModelStandby2(packed) is non-zero and,
 * for the cell (packed >> 8, packed & 0xff), SzDuel_GetUnitStatus(.., 4) is non-zero or the
 * low byte is 0, and 0x1e1d10 is positive: converts the TaView_SetExzModelFrontView(packed, add)
 * words, adds SzEffect_GetRadius2(packed) to the first float, and submits kind 7 with the
 * 0x1e1360 answer and kind 4 with the floats, both tagged `who`. Otherwise
 * submits the 0x1e10f0 vector of the 0x1e1090 cell (kind 3) and the converted
 * Ta_GetStdFocusView(value) words (kind 4); when 0x1f5ec0 was non-zero this is done a
 * second time. Assumed: argument types follow the callees' declarations; the
 * tag is a byte. */
extern int SzEffect_IsModelStandby2(int packed);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitCtlRateM(int side, int pos);
extern void TaView_SetExzModelFrontView(int *out, int packed, int add);
extern float SzEffect_GetRadius2(int packed);
extern int SzDuel_GetUnitModelID(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
void TaView_ReqExzFocusCamera(unsigned char who, int packed, int add, int value);

/* 0x260af0: as 0x260eb0 with the words of TaView_SetModelFrontView (the cell pair is also
 * submitted twice). 0x260d10: with 0x2612e0 and 0x255f70, and the cell pair
 * submitted once in every other case. 0x2610d0: with 0x261420, without the
 * 0x1e1d10 question, and the cell pair submitted once. Assumed: as 0x260eb0. */
extern void TaView_SetModelFrontView(int *out, int packed, int add);
extern void TaView_SetModelBackView(int *out, int packed, int add);
void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value);
void TaView_ReqFocusCamera3(unsigned char who, int packed, int add, int value);
void TaView_ReqBackFocusCamera(unsigned char who, int packed, int add, int value);

#endif
