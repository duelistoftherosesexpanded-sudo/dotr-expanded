#ifndef DOTR_EE_BATTLE_SEA_H
#define DOTR_EE_BATTLE_SEA_H

/* lSzBattle_DrawSea_Pipe (0x1e5eb0): draws the battle sea model from the per-type location tables
 * apLocData / abLocDataMax.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* Assumed layouts, sized from the accesses of lSzBattle_DrawSea_Pipe only: a placement record
 * is 32 bytes with two angles in degrees at +0x10 / +0x14 and a frame offset at +0x18; the
 * 16-byte rotation is copied whole from data_002922a0 (contents not interpreted) before its first
 * two words are replaced. Member names are descriptive, not recovered. */
struct DotrSeaRot {
    int x;
    int y;
    int z;
    int w;
};
struct DotrSeaPart {
    int unknown_00[4];
    float rx;
    float ry;
    int frame;
    int unknown_1c;
};

extern struct DotrSeaPart *apLocData[];
extern unsigned char abLocDataMax[];
extern struct DotrSeaRot data_002922a0;

/* An address passed as a 32-bit word: int for the target,
 * a pointer-sized integer on the host. */
#ifdef __MWERKS__
typedef int DotrSeaWord;
#else
typedef long DotrSeaWord;
#endif

/* Callee signatures as this caller uses them (parameter types assumed). */
extern int SzModel_SetAnimFrame(int modelId, float animFrame);
extern float SzModel_GetAnimFrame(int modelId);
extern int SzModel_GetAnimFrameMax(int modelId, int animIndex);
extern int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, DotrSeaWord c, DotrSeaWord d, int e, int f, int g);
extern int lSzBattle_DrawFakeMirror(int cursor, int index);

int lSzBattle_DrawSea_Pipe(int cursor, int model, int type);

#endif
