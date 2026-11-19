#ifndef DOTR_EE_BATTLE_INIT_H
#define DOTR_EE_BATTLE_INIT_H

/* SzBattle_Init (0x1e4f00): the battle scene set-up that reads the per-stage parameter table
 * gSzBattle_aStageParam and requests the stage camera file.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* The load record of SzFile_ReadRequestParam's callback; not examined here. */
struct DotrPoolLoadRecord;
#include "dotr/ee/duel_init.h"

struct DotrBattleVec3 {
    float x, y, z;
};

/* One stage's entry of gSzBattle_aStageParam (11 entries of 0xc0 bytes; data name from the
 * Japanese build). Member names are from the setter each address is handed to; the
 * contents of light and fog_color are not examined here. */
struct DotrBattleStageParam {
    struct DotrBattleVec3 intensity;  /* 0x00: SzEffect_SetIntensityForBattle */
    struct DotrBattleVec3 intensity2; /* 0x0c: SzEffect_SetIntensityForMonster */
    unsigned char unknown_18[8];
    unsigned char light[0x90];        /* 0x20 */
    float fog_y[2];                   /* 0xb0: equal values mean no vertical fog */
    unsigned char fog_color[8];       /* 0xb8 */
};
extern struct DotrBattleStageParam gSzBattle_aStageParam[11];
extern int gSzBattle_nSave;
extern int anOfs_STAGECAM[11];

/* gSzBattle_bl is declared as words in cell_drops.h / state_grid.h; its third word is the
 * camera file's address, read here through this view. */
struct DotrBattleBlock {
    int unknown_0;
    int unknown_4;
    void *file;
};
extern int gSzBattle_bl[];

/* Vectors copied whole for stage 4 (the first then gets the stage's intensity x and y). */
extern struct DotrBattleVec3 data_002921a0;
extern struct DotrBattleVec3 data_002921b0;
/* Format and label strings; contents not examined. */
extern char data_002cf730[];
extern char data_002cf746[];

/* Callees as this caller passes arguments (index_fanout.h declares the setters with int). */
extern void SzEffect_SetFogColorForBattle(DotrInitWord value);
extern void SzEffect_SetFogParamZForBattle(DotrInitWord value);
extern void SzEffect_SetFogParamYForBattle(DotrInitWord value);
extern void SzEffect_SetStraightLightForBattle(DotrInitWord pointLightParameters);
extern void SzEffect_SetPointLightForBattle(DotrInitWord pointLightParameters);
extern void SzEffect_SetIntensityForBattle(DotrInitWord value);
extern void SzEffect_SetBlackOutForBattle(DotrInitWord blackOutValue);
extern int SzModel_SetFogFlag(int index, int value);
/* object_table.h has struct DotrObjectVector3 here; same three floats. */
extern int SzModel_SetIntensity(int modelId, struct DotrBattleVec3 *intensity);
extern void SzMem_Free(void *pointer);
extern char *SzFile_GetEnv(const char *label);
extern int func_0019b648(char *destination, const char *format, ...);
/* As in pool_table.h. */
extern void *SzFile_ReadRequestParam(int fileType, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
                                     int unknown_010, void (*callback)(struct DotrPoolLoadRecord *record, int completed),
                                     int unknown_01c);
/* Defined in state_grid (0x1e5210), which declares the record by its own view of the
 * load record; only the address is used here, so the parameter is spelled as
 * SzFile_ReadRequestParam's callback type. */
void lSzBattle_CameraCB(struct DotrPoolLoadRecord *record, int completed);

/* 0x1e4f00 SzBattle_Init (name from the Japanese build's symbol table): sets the battle scene
 * up for `stage`. Hides all 84 models, shows 8..19 and 7, hands the stage's fog, light and
 * intensity entries to the Monster / Battle setters (vertical fog only when the two fog_y
 * values differ), fogs models 8..19 except on stage 4, applies the stage 4 and stage 2
 * exceptions, records the stage, frees a previous camera file and requests the stage's
 * part of the camera file (offsets from anOfs_STAGECAM) with lSzBattle_CameraCB. */
void SzBattle_Init(int stage);

#endif
