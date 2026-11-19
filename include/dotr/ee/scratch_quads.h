#ifndef DOTR_EE_SCRATCH_QUADS_H
#define DOTR_EE_SCRATCH_QUADS_H

/* Functions that build vertices in the scratchpad at the literal address 0x70002000 and pass them
 * to the view and draw callees, as 0x1c1fe0 does.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* One 48-byte scratchpad vertex as lSzDuel_DrawFadeBoard_Pipe fills it: three floats, four bytes at +0x18
 * and a float at +0x1c. Which of the unwritten bytes belong to the record is not known. */
struct DotrScratchVertex {
    float position[3];
    unsigned char unknown_0c[12];
    unsigned char colour[4];
    float unknown_1c;
    unsigned char unknown_20[16];
};

/* The original addresses the scratchpad as a literal (one `lui $at` per store, lui/ori for
 * the pointer arguments); host tests use an object instead. */
#ifdef __MWERKS__
#define DOTR_SCRATCH_VERTICES ((struct DotrScratchVertex *)0x70002000)
#define DOTR_SCRATCH_BASE ((char *)0x70000000)
#define DOTR_SCRATCH_BASE_ARG(p) ((int)(p))
#else
/* Host: the base pointer is not used as an address; the argument keeps the literal value. */
#define DOTR_SCRATCH_BASE ((char *)0)
#define DOTR_SCRATCH_BASE_ARG(p) ((void)(p), 0x70000000)
extern struct DotrScratchVertex dotr_scratch_vertices_70002000[5];
#define DOTR_SCRATCH_VERTICES (dotr_scratch_vertices_70002000)
/* Callees defined in draw_state (0x207fa0, 0x208030), small_state (0x207500) and
 * view_state (0x205d10); block, node and packet pointers are passed on as plain words here. */
extern int ExSetAnchor_Pipe(int block);
extern int *ExGetMyOT(void);
extern int Ex3d_GetSampleZ(float *out, float dz);
extern void ExSortAnchor_Pipe(int packet, int node, int *chain, int z);

/* `a` is the draw block or node passed on; `colour` may be null; assumed from the uses. */
extern void SzDuel_DrawCursor_Pipe(int a, float *centre, float dx, float dz, int alpha, unsigned char *colour,
                                   int single);

#endif

/* 16-byte header at the start of lSzDuel_DrawFadeBoard_Pipe's 48-byte local; draw_state holds another
 * view of the same header (struct DotrDrawPacket). */
struct DotrScratchPacket {
    short unknown_00;
    short unknown_02;
    int unknown_04;
    long unknown_08;
};

/* Callees defined in view_state (0x2070f0, 0x207230) and draw_state (0x208d00, 0x2086d0,
 * 0x207aa0). Pointer parameters are narrowed to this group's views. */
extern int func_002070f0_quad(void *m, int count, struct DotrScratchVertex *v, int a, int b, int c, float *average);
extern void Ex3d_NodeV2S(int a, int count, struct DotrScratchVertex *v);
extern long ExGetAlphaVal(int index);
extern int func_002086d0_quad(struct DotrScratchPacket *packet, long value);
extern int func_00207aa0_quad(int a, struct DotrScratchPacket *packet, int count, struct DotrScratchVertex *v, int e,
                              int f);

/* The first argument is only passed on; alpha is stored as one byte. */
extern void lSzDuel_DrawFadeBoard_Pipe(int a, float *centre, int alpha, float dx, float dz);

/* Callees defined in draw_state (0x207fa0, 0x208030), small_state (0x207500) and
 * view_state (0x205d10); block, node and packet pointers are passed on as plain words here. */
extern int ExSetAnchor_Pipe(int block);
extern int *ExGetMyOT(void);
extern int Ex3d_GetSampleZ(float *out, float dz);
extern void ExSortAnchor_Pipe(int packet, int node, int *chain, int z);

/* `a` is the draw block or node passed on; `colour` may be null; assumed from the uses. */
extern void SzDuel_DrawCursor_Pipe(int a, float *centre, float dx, float dz, int alpha, unsigned char *colour,
                                   int single);

/* `a` is the draw cursor passed on and returned; `colour` may be null; assumed from the uses. */
extern int SzDuel_DrawBoxCursor_Pipe(int a, float *centre, unsigned char *colour, int single);

#endif
