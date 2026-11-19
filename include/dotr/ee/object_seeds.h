#ifndef DOTR_EE_OBJECT_SEEDS_H
#define DOTR_EE_OBJECT_SEEDS_H

/* Functions that walk a fixed table of per-object records and seed the object slots and grid
 * cells from it (0x100350 walks 84 48-byte records at 0x28a880).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/main.md. */

/* 48-byte record at 0x28a880 (84 of them); offsets observed in 0x100350, names assumed.
 * The two 16-byte blocks are only passed by address to 0x1f22b0 / 0x1f2430. */
struct DotrSeedBlock {
    int words[4];
};
typedef struct DotrSeedRecord {
    int kind;
    int value;
    int unknown_08;
    int unknown_0c;
    struct DotrSeedBlock vector;
    struct DotrSeedBlock words;
} DotrSeedRecord;
extern DotrSeedRecord gMain_ModelData[84];

/* Callees are defined in object_table, resource_support, state_records and state_grid;
 * the block pointer types are assumed for this group. */
extern int SzModel_Create(int modelId);
extern int SzModel_SetPos(int modelId, struct DotrSeedBlock *pos);
extern int SzModel_SetRot(int index, struct DotrSeedBlock *in);
extern int SzModel_Preset(int modelId, int modelType, int modelNumber);
extern int SzDuel_Xax2New(int index);
extern void SzDuel_AddUnit(int side, int col, int row, int cardNo, int e);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int SzDuel_SetUnitOpenRate(int side, int pos, int pair, int openRate);
extern int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
int lMain_InitModel2(void);

#endif
