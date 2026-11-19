#ifndef DOTR_EE_TAKU_WINDOW_H
#define DOTR_EE_TAKU_WINDOW_H

/* The campaign map's window frame builder Taku_Window_Set, which draws a framed window of a
 * given style and kind from G4 quads and SP1 sprites of the window texture table.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* The code passes the packet cursor as a 32-bit word (int on the target, pointer-sized on the host) and
 * reads the camp work area at the literal address 0x1ed0000 (the halfword 0x1ed006c, a fade
 * value for styles 10 and 11). The target build keeps the literal; the host build points it at
 * a test object. Assumed: these meanings, from this use. */
#ifdef __MWERKS__
typedef int DotrTakuWindowWord;
#define DOTR_TAKU_WINDOW_AT(a) (a)
#else
#include <stdint.h>
typedef intptr_t DotrTakuWindowWord;
union DotrTakuWindowWork {
    unsigned char bytes[0x100];
    double align;
};
extern union DotrTakuWindowWork dotr_taku_window_work;
#define DOTR_TAKU_WINDOW_AT(a) ((intptr_t)dotr_taku_window_work.bytes + ((a) - 0x01ED0000))
#endif

/* Views: the window rectangle (corners and a kind), one
 * primitive template (eight corner coordinates, seventeen colour/data bytes, a flag, depth and
 * a texture-table index) and the texture table entries at tex_uv_data (0x2950a0). window_tex_no
 * (0x296520) is declared but not read here. Assumed: names from use. */
typedef struct {
    short x0, y0, x1, y1, x2, y2, x3, y3;
    unsigned char data[17];
    unsigned char flag;
    short depth;
    unsigned short index;
} DotrTakuWindowPrim;

typedef struct {
    short x0, y0, x1, y1;
    short kind;
} DotrTakuWindowRect;

typedef struct {
    short u, v, width, height;
    unsigned char pad08[8];
    unsigned short texture;
    unsigned char pad12[2];
} DotrTakuWindowTex;

extern DotrTakuWindowTex tex_uv_data[];
extern unsigned short window_tex_no[][0x100];


/* Callee: Taku_FT4_Set (0x218020) is defined in quad_offsets with another view. */
extern void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);

/* Static inline helpers: one SP1 sprite from a template and its texture entry (flipped:
 * v - 1), and one G4 quad from a template. Taku_Prim_Set_SP1 and Taku_Prim_Set_G4 are declared
 * inside them (returning the cursor; prim_sp1.h and cursor_packets.h declare them with other
 * views). */
static inline DotrTakuWindowWord dotr_taku_window_sprite(DotrTakuWindowWord p, DotrTakuWindowPrim *s, short dx, short dy) {
    extern DotrTakuWindowWord Taku_Prim_Set_SP1(DotrTakuWindowWord, short, short, short, short, short, short, short, short,
                               unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned short, unsigned char, short);
    DotrTakuWindowTex *e = &tex_uv_data[s->index];

    return Taku_Prim_Set_SP1(p, (short)(s->x0 + dx), (short)(s->y0 + dy),
                         (short)(s->x1 + dx), (short)(s->y1 + dy),
                         e->u, e->v, e->width, e->height,
                         s->data[0], s->data[1], s->data[2], s->data[3],
                         s->data[16], e->texture, s->flag, s->depth);
}

static inline DotrTakuWindowWord dotr_taku_window_flipped(DotrTakuWindowWord p, DotrTakuWindowPrim *s, short dx, short dy) {
    extern DotrTakuWindowWord Taku_Prim_Set_SP1(DotrTakuWindowWord, short, short, short, short, short, short, short, short,
                               unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned short, unsigned char, short);
    DotrTakuWindowTex *e = &tex_uv_data[s->index];

    return Taku_Prim_Set_SP1(p, (short)(s->x0 + dx), (short)(s->y0 + dy),
                         (short)(s->x1 + dx), (short)(s->y1 + dy),
                         e->u, e->v - 1, e->width, e->height,
                         s->data[0], s->data[1], s->data[2], s->data[3],
                         s->data[16], e->texture, s->flag, s->depth);
}

static inline DotrTakuWindowWord dotr_taku_window_quad(DotrTakuWindowWord p, DotrTakuWindowPrim *q, short x, short y) {
    extern DotrTakuWindowWord Taku_Prim_Set_G4(DotrTakuWindowWord, short, short, short, short, short, short, short, short,
                               unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned short,
                               short);

    return Taku_Prim_Set_G4(p,
        q->x0 + x, q->y0 + y,
        q->x1 + x, q->y1 + y,
        q->x2 + x, q->y2 + y,
        q->x3 + x, q->y3 + y,
        q->data[0], q->data[1], q->data[2], q->data[3],
        q->data[4], q->data[5], q->data[6], q->data[7],
        q->data[8], q->data[9], q->data[10], q->data[11],
        q->data[12], q->data[13], q->data[14], q->data[15],
        q->data[16], q->depth);
}

/* 0x2195e0: draws a window of the given style (colour 0..5, 10 and 11 fading by 0x1ed006c) and
 * the rectangle's kind; returns the cursor. */
DotrTakuWindowWord Taku_Window_Set(DotrTakuWindowWord p, void *rect_arg, unsigned short style, unsigned short mode,
                                   unsigned short dx, unsigned short dy, unsigned short depth, int unused, short alpha);

#endif
