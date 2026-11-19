#ifndef DOTR_EE_WINDOW_EX_ICON_H
#define DOTR_EE_WINDOW_EX_ICON_H

/* The window icon drawer: one icon sprite with optional dark background, border and blended tint
 * into the packet cursor.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The packet cursor is a 32-bit word in the game; a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrExIconWord;
#else
typedef long DotrExIconWord;
#endif

/* The packet header ExSortPrim_Pipe takes:
 * count, flags, a reserved word and the GS register list. Assumed: field meanings from the
 * values stored. */
struct DotrExIconTag {
    unsigned int count : 16;
    unsigned int flags : 16;
    unsigned int reserved;
    unsigned long long registers;
};

struct DotrExIconColor {
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;
    float q;
};

/* A 64-bit uv word (14-bit u and v in 12.4 fixed point). */
struct DotrExIconUv {
    unsigned short u : 14;
    unsigned short pad0 : 2;
    unsigned short v : 14;
    unsigned short pad1 : 2;
    unsigned int reserved;
};

/* A 64-bit GS vertex word: x and y (12.4 fixed point), z and fog. The game's code uses 64-bit
 * bit-fields; on the host the same little-endian layout is spelled with 32-bit units. */
#ifdef __MWERKS__
typedef unsigned long long DotrExIconBits;
#else
typedef unsigned int DotrExIconBits;
#endif
struct DotrExIconXyz {
    DotrExIconBits x : 16;
    DotrExIconBits y : 16;
    DotrExIconBits z : 24;
    DotrExIconBits fog : 8;
};

/* The textured sprite packet (header, colour, two uv/xyz pairs, texture words) and the solid
 * rectangle packet (header, colour, two vertices), both built on the stack. */
struct DotrExIconSprite {
    unsigned long long tag[2];
    struct DotrExIconColor color;
    unsigned long long uv0;
    struct DotrExIconXyz xyz0;
    unsigned long long uv1;
    struct DotrExIconXyz xyz1;
    unsigned long long texture[4];
};

struct DotrExIconSolid {
    unsigned long long tag[2];
    struct DotrExIconColor color;
    struct DotrExIconXyz xyz0;
    struct DotrExIconXyz xyz1;
    unsigned long long unused;
};

/* The duel control block at 0x317490 as this drawer reads it: texture addresses at +0x6a0
 * (indexed by the icon's first byte), the draw depth word at +0xc58. Assumed: field meanings
 * from their use here; the host layout differs (wider pointers). */
struct DotrExIconState {
    unsigned char pad000[0x6A0];
    void *textures[16];
    unsigned char pad6E0[0x578];
    int depth;
};
extern struct DotrExIconState data_00317490_exicon;
/* The border icon definition drawn over an icon (an icon record like src). */
extern unsigned char data_002909f4_exicon[];
/* A halfword whose high byte is the alpha of the blended tint. Assumed: a fade level. */
extern unsigned short data_00318118_exicon;

/* Callees (names from the Japanese build's symbols). */
extern int ExAssignAlpha(void *packet, long long alpha);
extern long long ExGetAlphaVal(int index);
extern int ExAssignTexture(void *packet, void *texture, int a);
extern DotrExIconWord ExSortPrim_Pipe(DotrExIconWord cursor, DotrExIconWord packet, int a2, int a3);

/* 0x1b3cc0 lSzDuel_DrawWindow_ExIcon_Pipe (name from the Japanese build's symbols): src is an
 * icon record (texture index byte, then u0, v0, u1, v1 halfwords). Draws at (x, y), optionally
 * at 3/4 size (shrink): a black background rectangle, the icon (flipped by flipX/flipY, 0x80
 * or 0x30 intensity by bright, alpha), the border icon, and a tint rectangle blended by
 * ExGetAlphaVal(blend) when blend >= 0 (colour rgb in b, g, r order or 0x80 grey). Returns the
 * new cursor. */
DotrExIconWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrExIconWord packet, unsigned short *src, int x, int y,
                                              int background, int image, int border, int shrink, int flipX,
                                              int flipY, int bright, unsigned char *rgb, int blend,
                                              unsigned char alpha);

#endif
