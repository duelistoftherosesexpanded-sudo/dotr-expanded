#ifndef DOTR_EE_PRIM_SP1_H
#define DOTR_EE_PRIM_SP1_H

/* Taku_Prim_Set_SP1 (0x217d70): sprite packet builder.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* The code passes the packet cursor and the packet address as 32-bit words: int on the target,
 * pointer-sized on the host. */
#ifdef __MWERKS__
typedef int DotrSp1Word;
#else
#include <stdint.h>
typedef intptr_t DotrSp1Word;
#endif

/* Packet views. Assumed: the meanings. The first vertex's
 * texel u and v are 14-bit fields of two halfwords, each vertex 16 bytes; the header holds
 * a count, flags, a zero word and the GS register list. */
typedef struct {
    unsigned short u : 14;
    unsigned short reserved_u : 2;
    unsigned short v : 14;
    unsigned short reserved_v : 2;
    unsigned char pad[12];
} DotrSp1Vertex;

typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int pad;
    unsigned long long registers;
} DotrSp1Header;

/* The 88-byte sprite packet local: header bytes, colour, q, two vertices and four state
 * words. The function gives the local 8-byte alignment. */
typedef struct {
    unsigned char tag[0x10];
    unsigned char r, g, b, a;
    float q;
    unsigned char vertices[0x20];
    unsigned char state[0x20];
} DotrSp1Packet;

/* Callees; TAKU_SP_XYWH and tex_data are declared inside the function. */
extern unsigned char *ExGetMyOT(void);
extern int ExAssignTexture(void *packet, void *texture, int a);
extern int ExAssignAlpha(void *packet, long long value);
extern DotrSp1Word ExSortPrim_Pipe(DotrSp1Word cursor, DotrSp1Word packet, int a2, int a3);

/* Seventeen arguments, nine on the stack; mode and flag are not read. small_state.h declares
 * this function for its caller with int words. */
void Taku_Prim_Set_SP1(DotrSp1Word arg0, short x0, short y0, short x1, short y1,
                       short u, short v, short width, short height,
                       unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned char mode,
                       unsigned short texture, unsigned char flag, short depth);

#endif
