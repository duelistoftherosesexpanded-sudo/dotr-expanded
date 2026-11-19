#ifndef DOTR_EE_DUEL_STAGE_DRAW_H
#define DOTR_EE_DUEL_STAGE_DRAW_H

/* lSzDuel_DrawStage_Pipe (0x1ae5f0): the duel board's per-frame stage pass over the 7x7 map
 * (tint, cursor cell, terrain models).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* One side's record inside the duel block at 0x317490; only the cursor cell is used here. */
struct DotrStageSide {
    unsigned char unknown_000[0x33d];
    unsigned char x;
    unsigned char y;
    unsigned char unknown_33f[0x11];
};
struct DotrStageDuel {
    struct DotrStageSide side[2];
    unsigned char unknown_6a0[0x5e6];
    unsigned short unknown_c86;
    unsigned char unknown_c88[0x42];
    unsigned char mark_x[49];
    unsigned char mark_y[49];
    unsigned char unknown_d2c[0x28];
    unsigned char turn;
    unsigned char unknown_d55[2];
    unsigned char step;
    unsigned char unknown_d58;
    unsigned char mark_count;
    unsigned char reach;
    unsigned char unknown_d5b[0x15];
};
struct DotrStageUnit {
    unsigned char unknown_00[0x44];
    unsigned char owner;
};
struct DotrStageCell {
    unsigned char unknown_00[0x14];
    short height;
    unsigned char model;
    unsigned char unknown_17[3];
    unsigned char alpha;
};
struct DotrStageAngles {
    int v[4];
};
/* gSzDuel_dl in the Japanese build (size 0xd70 there); this view names only the members read here. */
extern struct DotrStageDuel data_00317490;
/* Initial values copied to the stack: six 4-byte colours 8 bytes apart and one 4-word angle set. */
struct DotrStageColour {
    unsigned char b[4];
};
extern struct DotrStageColour data_002910e0, data_002910e8, data_002910f0, data_002910f8, data_00291100, data_00291108;
extern struct DotrStageAngles data_00291110;
/* Callees with the assumed prototypes this function needs. */
extern int SzGame_GetMode(int);
extern int SzEffect_GetBlackOutForDuel(void);
extern void SzEffect_SetIntensityOnly(int kind, float value);
extern int SzDuel_SetMapAlpha(int x, int y, int alpha);
extern void SzDuel_InitMapColor(int a, unsigned char *b, int c);
extern struct DotrStageUnit *SzDuel_GetMapUnit(int x, int y);
extern void SzDuel_SetMapColor(int col, int row, int c, unsigned char *colour, int e);
extern int SzDuel_GetMapMovable(int x, int y);
extern int SzDuel_CalcMapColor(int x, int y);
extern struct DotrStageCell *SzDuel_GetMapInfo(int x, int y);
extern int SzModel_IsModelDisp(int modelId);
extern void SzDuel_CalcUnitPos(int x, int y, int z, float *v);
extern int SzDuel_DrawBoxCursor_Pipe(int a, float *centre, unsigned char *colour, int single);
extern int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, float *c, void *d, int e, int f, int g);
extern int SzModel_SetFilter(int index, int value, void *in);

int lSzDuel_DrawStage_Pipe(int cursor);

#endif
