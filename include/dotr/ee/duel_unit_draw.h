#ifndef DOTR_EE_DUEL_UNIT_DRAW_H
#define DOTR_EE_DUEL_UNIT_DRAW_H

/* Duel draw passes over the duel control block at 0x317490 that tint models by the side to
 * move (SzDuel_DrawUnit_Pipe).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Views of the duel control block at 0x317490 (848-byte per-side records first), the 0x5c-area
 * unit record and the map cell, limited to the members SzDuel_DrawUnit_Pipe reads. Member
 * names are this project's. */
struct DotrUnitDrawSide {
    unsigned char unknown_000[0x33d];
    unsigned char x;
    unsigned char y;
    unsigned char unknown_33f[0x11];
};
struct DotrUnitDrawUnit {
    unsigned char unknown_00[0x30];
    int angle;
    short pos_x;
    short pos_z;
    unsigned char unknown_38[8];
    unsigned char x;
    unsigned char y;
    unsigned char unknown_42[2];
    unsigned char owner;
    unsigned char slot;
    unsigned char model;
    unsigned char unknown_47[2];
    unsigned char unknown_49;
    unsigned char unknown_4a;
    unsigned char unknown_4b[4];
    unsigned char flags;
    unsigned char unknown_50;
    unsigned char unknown_51;
    unsigned char unknown_52;
    unsigned char fade;
    unsigned char unknown_54[3];
    signed char unknown_57_0 : 6;
    signed char unknown_57_6 : 1;
    signed char unknown_57_7 : 1;
    signed char hidden : 1;
};
struct DotrUnitDrawDuel {
    struct DotrUnitDrawSide side[2];
    unsigned char unknown_6a0[0x28];
    struct DotrUnitDrawUnit *active;
    struct DotrUnitDrawUnit *target;
    unsigned char unknown_6d0[0x5b6];
    unsigned short unknown_c86;
    unsigned char unknown_c88[0xb0];
    unsigned char unknown_d38;
    unsigned char unknown_d39[0x1b];
    unsigned char turn;
    unsigned char unknown_d55[2];
    unsigned char step;
};
struct DotrUnitDrawCell {
    unsigned char unknown_00[0x14];
    short height;
};
struct DotrUnitDrawAngles {
    int v[4];
};
struct DotrUnitDrawColour {
    unsigned char b[4];
};
extern struct DotrUnitDrawDuel data_00317490;
/* .data templates the compiler copies into the three local colours and the local rotation
 * before the computed members are stored. */
extern struct DotrUnitDrawColour data_002914e0, data_002914e8, data_002914f0;
extern struct DotrUnitDrawAngles data_00291500;
/* Callees with the assumed prototypes this group needs. SzDuel_GetMapUnit, SzDuel_GetUnit and
 * SzDuel_GetMapInfo return this header's views of the unit and cell records; func_001a23b0 is the
 * library float-to-unsigned conversion, called explicitly as in the other groups. */
extern int SzGame_GetMode(int);
extern int SzEffect_GetBlackOutForMonster(void);
extern void SzEffect_SetIntensityOnly(int kind, float value);
extern struct DotrUnitDrawUnit *SzDuel_GetMapUnit(int x, int y);
extern struct DotrUnitDrawUnit *SzDuel_GetUnit(int side, int pos);
extern struct DotrUnitDrawCell *SzDuel_GetMapInfo(int x, int y);
extern int SzModel_IsModelDisp(int modelId);
extern float SzModel_GetAnimFrame(int modelId);
extern int func_001a23b0(float value);
extern int SzModel_SetAnimDir(int modelId, int animDir);
extern int SzDuel_TrimUnitDir1(int a, int b);
extern int SzModel_SetFilter(int index, int value, void *in);
extern int SzModel_ExSort_Pipe(int cursor, int index, float *v, void *a, int b, int c, int d);

/* 0x1ce430. Assumed: the cursor is an int as for SzModel_ExSort_Pipe. */
int SzDuel_DrawUnit_Pipe(int cursor);

#endif
