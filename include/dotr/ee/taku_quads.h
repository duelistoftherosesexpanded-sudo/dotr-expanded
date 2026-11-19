#ifndef DOTR_EE_TAKU_QUADS_H
#define DOTR_EE_TAKU_QUADS_H

/* Taku_Prim_Set_FT4 and Taku_Prim_Set_GT4 quad packet builders: each fills a quad packet on its
 * stack, places the corners with a TAKU_*_XYWH helper and hands the packet to ExAssignTexture,
 * ExAssignAlpha and ExSortPrim_Pipe.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* The code passes the packet cursor and the packet address as 32-bit words: int on the target, pointer-sized
 * on the host. */
#ifdef __MWERKS__
typedef int DotrQuadWord;
#else
#include <stdint.h>
typedef intptr_t DotrQuadWord;
#endif

/* Packet views. Assumed: the meanings. */
/* Taku_Prim_Set_FT4: texel u and v as 14-bit fields of two halfwords, each vertex 16 bytes. */
typedef struct {
    unsigned short u : 14;
    unsigned short reserved_u : 2;
    unsigned short v : 14;
    unsigned short reserved_v : 2;
    unsigned char pad[12];
} DotrQuadFt4Vertex;

typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int pad;
    unsigned long long registers;
} DotrQuadFt4Header;

/* The textured quad packet local (header bytes, colour, q, four vertices, four state words);
 * the function gives it 8-byte alignment. */
typedef struct {
    unsigned char tag[0x10];
    unsigned char r, g, b, a;
    float q;
    unsigned char vertices[0x40];
    unsigned char state[0x20];
} DotrQuadFt4Packet;

/* Taku_Prim_Set_GT4: four vertices of colour, q, s, t and the packed xyz word. */
typedef struct {
    unsigned char r, g, b, a;
    float q;
    float s, t;
    long long xyz;
} DotrQuadGt4Vertex;

typedef struct {
    short count;
    short flags;
    int zero;
    long long regs;
    DotrQuadGt4Vertex vertex[4];
    unsigned char padding[0x20];
} DotrQuadGt4Packet;

/* A static inline header initialiser for the GT4 packet (12 registers, flags 0x34). */
static inline void dotr_taku_quads_init_gt4(void *packet) {
    *(short *)((unsigned char *)packet + 2) = 0x34;
    *(short *)packet = 0xC;
    *(long long *)((unsigned char *)packet + 8) = 0x421421421421LL;
    *(int *)((unsigned char *)packet + 4) = 0;
}

/* Callees. TAKU_FT4_XYWH and tex_data are declared inside the functions. TAKU_GT4_XYWH
 * (0x2194b0) is defined in draw_state with int parameters; this view has narrower ones. */
extern unsigned char *ExGetMyOT(void);
extern int ExAssignTexture(void *packet, void *texture, int a);
extern int ExAssignAlpha(void *packet, long long value);
extern long long ExGetAlphaVal(int index);
extern DotrQuadWord ExSortPrim_Pipe(DotrQuadWord cursor, DotrQuadWord packet, int a2, int a3);
extern void TAKU_GT4_XYWH(void *packet, short x0, short y0, int z0, unsigned char fog0, short x1, short y1,
                          int z1, unsigned char fog1, short x2, short y2, int z2, unsigned char fog2,
                          short x3, short y3, int z3, unsigned char fog3);

/* 0x218340: one textured quad, texel coordinates as (n * 16) + 8, 0x800 set in the header flags
 * after the corners are placed; returns the cursor ExSortPrim_Pipe returns. mode and flag are
 * not read. */
void *Taku_Prim_Set_FT4(void *arg0, short x0, short y0, short x1, short y1,
                        short x2, short y2, short x3, short y3,
                        short u0, short v0, short u1, short v1,
                        short u2, short v2, short u3, short v3,
                        unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned char mode,
                        unsigned short texture, unsigned char flag, short depth);

/* 0x219180: one Gouraud-shaded textured quad with normalised texture coordinates (texel + 0.5
 * over 256 or 512). The two unused arguments are not read. */
void Taku_Prim_Set_GT4(void *arg0, short x0, short y0, short x1, short y1,
                       short x2, short y2, short x3, short y3,
                       short u0, short v0, short u1, short v1,
                       short u2, short v2, short u3, short v3,
                       unsigned char r0, unsigned char g0, unsigned char b0, unsigned char a0,
                       unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
                       unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2,
                       unsigned char r3, unsigned char g3, unsigned char b3, unsigned char a3,
                       unsigned char unused0, unsigned short texture, unsigned char unused1, short z);

#endif
