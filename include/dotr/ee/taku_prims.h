#ifndef DOTR_EE_TAKU_PRIMS_H
#define DOTR_EE_TAKU_PRIMS_H

/* Taku_Prim_Set_* quad builders (first member Taku_Prim_Set_FT4_ST at 0x2185e0): each fills one
 * packet on its stack, places the four corners with TAKU_FT4_XYWH and hands the packet to the
 * Ex* assigners and ExSortPrim_Pipe.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#ifdef __MWERKS__
typedef int DotrPrimWord;
#else
#include <stdint.h>
typedef intptr_t DotrPrimWord;
#endif

/* Views assumed from the stores (the layouts cursor_packets.h and draw_state.h use for the
 * same packets): a 16-byte header, a 64-bit vertex word of 16-bit x and y, a 24-bit depth
 * and a byte, and the textured packet's four colour bytes, a float and four vertices of
 * two texture floats and a vertex word. The packet local starts eight bytes below the
 * header and extends past the last vertex (frame size); those parts are not accessed. */
struct DotrPrimHeader {
    unsigned short unknown_00;
    unsigned short unknown_02;
    int unknown_04;
    long unknown_08;
};
struct DotrPrimVertex {
    unsigned long unknown_x : 16;
    unsigned long unknown_y : 16;
    unsigned long unknown_z : 24;
    unsigned long unknown_f : 8;
};
struct DotrPrimStVertex {
    float s;
    float t;
    struct DotrPrimVertex xyz;
};
/* Textured quad packet, 120 bytes. Size, the four trailing members and their names come
 * from the Japanese build's debug information (the `FT4` locals of ShSlot.c); the 8-byte
 * alignment attribute is inferred: with it the pinned compiler places the local where the
 * original has it (end-aligned at the top of the frame), without it 8 bytes lower. */
typedef struct {
    long header[2];
    unsigned char colour[4];
    float q;
    struct DotrPrimStVertex v[4];
    long gsAlpha;
    long gsTex0;
    long gsTex1;
    long gsClamp;
} DotrPrimStPacket __attribute__((aligned(8)));
extern int *tex_data[];

/* Callees, signatures assumed from this call site. ExAssignTexture (0x208700) and
 * ExSortPrim_Pipe (0x207650) are in ex_texture and sort_prims. */
extern int *ExGetMyOT(void);
extern void TAKU_FT4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1,
                          int f1, int x2, int y2, int z2, int f2, int x3, int y3, int z3,
                          int f3);
extern int ExAssignTexture(struct DotrPrimHeader *packet, int *texture, int a);
extern int ExAssignAlpha(struct DotrPrimHeader *packet, long value);
extern DotrPrimWord ExSortPrim_Pipe(DotrPrimWord cursor, void *packet, int a1, int a2);

/* Taku_Prim_Set_FT4_ST (0x2185e0): one textured quad with explicit texture
 * coordinates. Parameter widths are those the body narrows to; the two unnamed ones are
 * never read. */
DotrPrimWord Taku_Prim_Set_FT4_ST(DotrPrimWord cursor, int x0, int y0, int x1, int y1, int x2,
                                  int y2, int x3, short y3, short s0, short t0, short s1,
                                  short t1, short s2, short t2, short s3, short t3,
                                  unsigned char r, unsigned char g, unsigned char b,
                                  unsigned char a, int unknown_68, unsigned short texture,
                                  int unknown_78, short z);

/* Taku_Prim_Set_FT4 (0x218340) is defined in taku_quads. */

#endif
