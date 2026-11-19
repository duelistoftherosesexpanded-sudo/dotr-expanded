#ifndef DOTR_EE_EX_BOARD_H
#define DOTR_EE_EX_BOARD_H

/* lSzDuel_DrawWindow_ExBoard_Pipe (0x1b65f0), the nine-tile window frame drawn from the tile
 * table at gSzDuel_Icon_bytes, and lSzDuel_DrawWindow_Base_Pipe (0x1b62f0), the tiled backing.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#ifdef __MWERKS__
typedef int DotrDrawWord;
#else
#include <stdint.h>
typedef intptr_t DotrDrawWord;
#endif

/* Packet header as the code writes it: two halfwords, a word and a 64-bit word. */
struct DotrExBoardHeader {
    short unknown_00;
    unsigned short unknown_02;
    int unknown_04;
    long unknown_08;
};
/* Two 14-bit texel coordinates in separate halfwords of an 8-byte slot. */
struct DotrExBoardUv {
    unsigned short u : 14;
    unsigned short v : 14;
};
/* The same slot as 64-bit bit-fields (u at bit 0, v at bit 16), as lSzDuel_DrawWindow_ExBoard_Pipe
 * writes it. */
struct DotrExBoardUv64 {
    unsigned long u : 14;
    unsigned long unknown_14 : 2;
    unsigned long v : 14;
    unsigned long unknown_30 : 34;
};
struct DotrExBoardXyz {
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long f : 8;
};
struct DotrExBoardVertex {
    long uv;
    struct DotrExBoardXyz xyz;
};
/* The textured two-vertex packet, 88 bytes: header, colour and q, two uv / xyz pairs and four
 * 8-byte GS register words that these functions never write. The trailing member names and
 * the size follow the Japanese build's debug information for the packet family (exPacket,
 * gsRgbaq, ..., gsAlpha, gsTex0, gsTex1, gsClamp); the 8-byte alignment attribute is
 * inferred: with it the compiler places the local end-aligned at the top of the
 * frame, where the original has it. */
typedef struct {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrExBoardVertex vertex[2];
    long gsAlpha;
    long gsTex0;
    long gsTex1;
    long gsClamp;
} DotrExBoardSprite __attribute__((aligned(8)));
/* The untextured two-vertex packet, 48 bytes (40 written, then gsAlpha); same attribute. */
typedef struct {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrExBoardXyz xyz[2];
    long gsAlpha;
} DotrExBoardFlat __attribute__((aligned(8)));
/* One 10-byte row of the table at gSzDuel_Icon_bytes: texture index and the texel rectangle. */
struct DotrExBoardTile {
    unsigned char texture;
    unsigned short u0, v0, u1, v1;
};
/* The words of the control block at 0x317490 this function reads (cursor_packets.h has the
 * fuller view of the same object under another tag). */
struct DotrExBoardControl {
    unsigned char unknown_000[0x6a0];
    int textures[366];
    int z;
};
extern struct DotrExBoardControl data_00317490;
extern char gSzDuel_Icon_bytes[];
/* gSzDuel_aaicolRgba (original name, 128 bytes) as rows of eight bytes; bytes 4-7 are read. */
extern unsigned char gSzDuel_aaicolRgba[][8];
/* One row of gSzDuel_aaicolRgba as lSzDuel_DrawWindow_ExBoard_Pipe reads it: bytes 4-7 are
 * blue, green, red, alpha (assumed order: they are stored into the packet as r, g, b, a from
 * +6, +5, +4, +7). */
struct DotrExBoardColour {
    unsigned char unknown_0[4];
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};
/* gSzDuel_aaicolRgba (0x2be840) under a view symbol of that row type. MATCHING DEVICE: the table
 * is declared with the row type because a cast of the byte view allocates registers differently.
 * The host build points the name at the byte view. */
#ifdef __MWERKS__
extern struct DotrExBoardColour gSzDuel_aaicolRgba_exboard[];
#else
#define gSzDuel_aaicolRgba_exboard ((struct DotrExBoardColour *)gSzDuel_aaicolRgba)
#endif

/* Callees, assumed prototypes from these call sites. */
long ExGetAlphaVal(int index);
int ExAssignAlpha(struct DotrExBoardHeader *packet, long value);
int ExAssignTexture(struct DotrExBoardHeader *packet, int *texture, int a);
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2);

/* 0x1b65f0. Assumed: eight int-sized arguments as the callers pass them; colour and back are
 * rows of gSzDuel_aaicolRgba (negative colour selects row 2, negative back draws no backing
 * rectangle); w and h are repeat counts of the middle tiles when positive and pixel sizes
 * when negative; kind selects a set of nine tiles. Parameter names are descriptive. */
DotrDrawWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrDrawWord cursor, int colour, int x, int y, int w, int h, int back,
                                             int kind);

/* lSzDuel_DrawWindow_Base_Pipe (0x1b62f0). Its own views: the 10-byte tile record (texture index,
 * left, top, right, bottom), the 64-bit UV and XYZ bit-fields, the header (count, flags, padding,
 * register word) and the 88-byte sprite packet. Assumed: these field meanings, from this function's
 * use. */
typedef struct {
    unsigned char texture;
    unsigned char padding;
    unsigned short left;
    unsigned short top;
    unsigned short right;
    unsigned short bottom;
} DotrExBoardBaseTexture;

typedef struct {
    unsigned long long u : 14;
    unsigned long long pad0 : 2;
    unsigned long long v : 14;
    unsigned long long pad1 : 34;
} DotrExBoardBaseUV;

typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long padding : 8;
} DotrExBoardBaseXYZ;

typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
} DotrExBoardBaseHeader;

typedef struct {
    unsigned long long header[2];
    unsigned char r, g, b, a;
    float q;
    unsigned long long uv0;
    DotrExBoardBaseXYZ xyz0;
    unsigned long long uv1;
    DotrExBoardBaseXYZ xyz1;
    unsigned long long primitive;
    unsigned long long tex0;
    unsigned long long tex1;
    unsigned long long clamp;
} DotrExBoardBasePacket;

/* Data (each symbol links by address): row 189 of the tile table
 * (0x2907e2), the texture pointer table (0x317b30 = control block + 0x6a0) and the depth word
 * (0x3180e8 = control block + 0xc58). MATCHING DEVICE: separate symbols on the target; on the
 * host the row and the depth point into the fixtures above and the texture pointers are a
 * host-only fixture (pointers are wider there). */
#ifdef __MWERKS__
extern unsigned char data_002907e2_exboard[];
extern void *data_00317b30_exboard[];
extern int data_003180e8_exboard;
#else
#define data_002907e2_exboard ((unsigned char *)gSzDuel_Icon_bytes + 0x762)
extern void *data_00317b30_exboard[];
#define data_003180e8_exboard (data_00317490.z)
#endif

/* Callees, declared as views (they link by address). Assumed: these types. */
int func_002086d0_base(void *packet, long long alpha);
int func_00208700_base(void *packet, void *texture, int a);
DotrDrawWord func_00207650_base(DotrDrawWord cursor, DotrDrawWord packet, int a2, int a3);

/* 0x1b62f0. Assumed: five int-sized arguments; the tile is row 189 of gSzDuel_Icon_bytes (address
 * 0x2907e2), repeated columns by rows times. */
DotrDrawWord lSzDuel_DrawWindow_Base_Pipe(DotrDrawWord packet, int x, int y, int columns, int rows);

#endif
