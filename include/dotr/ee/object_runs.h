#ifndef DOTR_EE_OBJECT_RUNS_H
#define DOTR_EE_OBJECT_RUNS_H

/* Functions that submit a run of gSzModel_ahModel objects through
 * SzModel_DrawWithOutCalc_Pipe behind the SzModel_GetDrawFlag gate, as in 0x1e78c0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* A word that holds an address: 32 bits on the target (SzModel_DrawWithOutCalc_Pipe's third
 * argument is `int` in object_table.h, the same type there). */
#ifdef __MWERKS__
typedef int DotrRunWord;
#else
#include <stdint.h>
typedef intptr_t DotrRunWord;
#endif

/* 0x2cf750: 12-byte entries, two signed halfwords and two floats; the list ends
 * at an entry whose second halfword is negative. Extent unknown. */
struct DotrRunPair {
    short first;
    short second;
    float x;
    float z;
};
extern struct DotrRunPair CastleData[];
/* 0x292470: sixteen bytes copied whole (lq/sq) to a local whose words +0 and +8
 * are then replaced by an entry's floats. Assumed: four floats. */
struct DotrRunVector {
    float x, y, z, w;
};
extern struct DotrRunVector data_00292470;
/* 0x292468 (object_table.h declares the same array): the mode word. */
extern int gSzEvent_nSave[];

extern int SzModel_GetDrawFlag(int index);
extern int SzModel_IsModelDisp(int modelId);
extern int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, DotrRunWord c, void *d, int e, int f, int g);

/* 0x1e78c0. Returns the cursor unchanged when SzModel_GetDrawFlag(34) is 0. In mode 2
 * it walks CastleData: for each entry a local copy of data_00292470 gets the
 * entry's two floats, and each non-negative halfword h with SzModel_IsModelDisp(h + 34)
 * set is submitted as SzModel_DrawWithOutCalc_Pipe(cursor, h + 34, &local, 0, 0, 0, 0). In any
 * other mode it submits each of 34..73 that SzModel_IsModelDisp accepts with a zero
 * third argument. Assumed: int cursor in and out. */
int SzEvent_Draw_Pipe(int cursor);

#endif
