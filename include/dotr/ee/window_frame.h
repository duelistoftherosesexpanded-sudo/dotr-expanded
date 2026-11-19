#ifndef DOTR_EE_WINDOW_FRAME_H
#define DOTR_EE_WINDOW_FRAME_H

/* The duel window frame drawer and the emblem drawer, sprite helpers that the window panels call to
 * draw frames and card emblems.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The control block at 0x317490 as bytes, in a union (an int stands for the alignment of a
 * partial field view this code does not use). */
union DotrFrameControl {
    unsigned char b[0xd70];
    int align;
};
extern union DotrFrameControl data_00317490_frame;

/* ---- lSzDuel_DrawWindow_Frame_Pipe (0x1b4780), views. Assumed:
 * the field meanings, from this function's use: GS packet pieces (tag, colour with q, 14-bit
 * UV pairs, 64-bit XYZ bit-fields), the textured sprite, the four-vertex gradient, the
 * textured quad, the 10-byte tile record of the table at 0x290080 and the control block's
 * texture pointers (+0x6a0) and depth (+0xc58). */
typedef struct DotrFrameTag {
    unsigned int count : 16;
    unsigned int flags : 16;
    unsigned int reserved;
    unsigned long long registers;
} DotrFrameTag;

typedef struct DotrFrameColor {
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;
    float q;
} DotrFrameColor;

typedef struct DotrFrameUV {
    unsigned short u : 14;
    unsigned short pad0 : 2;
    unsigned short v : 14;
    unsigned short pad1 : 2;
    unsigned int reserved;
} DotrFrameUV;

typedef struct DotrFrameXYZ {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrFrameXYZ;

typedef struct DotrFrameSprite {
    unsigned long long tag[2];
    DotrFrameColor color;
    unsigned long long uv0;
    DotrFrameXYZ xyz0;
    unsigned long long uv1;
    DotrFrameXYZ xyz1;
    unsigned long long texture[4];
} DotrFrameSprite;

typedef struct DotrFrameVertex {
    DotrFrameColor color;
    DotrFrameXYZ xyz;
} DotrFrameVertex;

typedef struct DotrFrameGradient {
    unsigned long long tag[2];
    DotrFrameVertex v[4];
} DotrFrameGradient;

typedef struct DotrFrameCorner {
    unsigned long long uv;
    DotrFrameXYZ xyz;
} DotrFrameCorner;

typedef struct DotrFrameQuad {
    unsigned long long tag[2];
    DotrFrameColor color;
    DotrFrameCorner c[4];
    unsigned long long texture[4];
} DotrFrameQuad;

typedef struct DotrFrameTexture {
    unsigned char index;
    unsigned char reserved;
    unsigned short x0;
    unsigned short y0;
    unsigned short x1;
    unsigned short y1;
} DotrFrameTexture;

typedef struct DotrFrameState {
    unsigned char pad000[0x6A0];
    void *textures[16];
    unsigned char pad6E0[0x578];
    int depth;
} DotrFrameState;

/* Data (each symbol links by address): the tile table at 0x290080,
 * gSzDuel_aaicolRgba (0x2be840, 8-byte colour rows), the frame margins (0x2be9c0, four bytes
 * per style) and the arrow shifts (0x2be9f8). */
extern DotrFrameTexture gSzDuel_Icon_frame[];
extern unsigned char gSzDuel_aaicolRgba_frame[];
extern unsigned char aabFrameOfs_frame[];
extern unsigned char abDecoOfs_frame[];

/* Helpers for this function. */
static inline void dotr_frame_tag(DotrFrameTag *tag, int count, int flags, unsigned long long registers)
{
    tag->flags = flags;
    tag->count = count;
    tag->registers = registers;
    tag->reserved = 0;
}

static inline int dotr_frame_abs(int value)
{
    if (value >= 0)
{
        return value;
    }
    return -value;
}

static inline int dotr_frame_intensity(int bright)
{
    if (bright != 0)
{
        return 0x80;
    }
    return 0x30;
}

/* Callees (ExSortPrim_Pipe is declared inside the function). Assumed: these types. */
int ExAssignAlpha(void *arg0, long long arg1);
int ExAssignTexture(void *arg0, void *arg1, int arg2);
long long ExGetAlphaVal(int arg0);

/* 0x1b4780. Assumed: cursor, colour row (negative: no gradient backing), icon kind, style
 * (tile set and margins), position, columns and rows (repeat counts when positive, pixel
 * sizes when negative) and a brightness flag (64-bit). Returns the advanced cursor. */
int lSzDuel_DrawWindow_Frame_Pipe(int packet, int colorIndex, int icons, int style, int x, int y, int cols, int rows,
                                  long long bright);

/* ---- lSzDuel_DrawWindow_Emblem_Pipe (0x1b4540). Data: the tile
 * table at 0x290080 and the single row 0x290864 in it (MATCHING DEVICE: separate symbols on
 * the target; on the host the row points into the table fixture). */
#ifdef __MWERKS__
extern unsigned char gSzDuel_Icon_emblem[];
extern unsigned short data_00290864_emblem[];
#else
#define gSzDuel_Icon_emblem ((unsigned char *)gSzDuel_Icon_frame)
#define data_00290864_emblem ((unsigned short *)(gSzDuel_Icon_emblem + 0x7e4))
#endif

/* lSzDuel_DrawWindow_ExIcon_Pipe (0x1b3cc0). Assumed: these types. */
int lSzDuel_DrawWindow_ExIcon_Pipe(int packet, unsigned short *src, int x, int y, int background, int image, int border,
                                   int shrink, int flipX, int flipY, int bright, unsigned char *rgb, int blend,
                                   unsigned char alpha);

/* 0x1b4540. Assumed: cursor, x, y, a count (1..; the emblem row is 0xc6 + (count - 1) / 3 and
 * (count - 1) % 3 + 1 selects the extra marks), a shrink flag (parts 0xc, else 0x10) and a
 * mirror flag. Returns the advanced cursor. */
int lSzDuel_DrawWindow_Emblem_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);

#endif
