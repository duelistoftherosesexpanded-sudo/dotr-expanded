#include "dotr/ee/draw_state.h"

/* Builds a chained packet for `count` 48-byte vertices and returns the cursor after it; with
 * primitive type 7 or no vertices the cursor is returned untouched. The 16-byte tag is linked
 * into the chain slot `z` (last slot with header bit 0x8000, else at most the one before it),
 * followed by a one-register tag (0x1000000000008000, 0xe), the primitive word built from the
 * header's flag bits, (0x5000c | textured, context + 0x47) and the optional pairs: header word
 * 0 (bit 0x200), words 1 and 2 (bit 0x20), word 3 (bits 0x20 and 0x4000) and the Z-buffer word
 * with its mask bit clear (bit 0x8000). Each vertex gives its colour word with register 1 (the
 * first vertex always, later ones with bit 0x10, which also stores 1/q in the upper float), its
 * texture word with register 2 or 3 (bit 0x20; divided by q when bit 0x10 is set and 0x800 is
 * clear) and its position word with register 4. The row pair of gExPrim_aaRegs and the Z-buffer
 * word with the mask bit set close the list; the tag then gets the quadword count. */
DotrDrawWord ExSortNode_Pipe(DotrDrawWord cursor, struct DotrSortHeader *header, int count, struct DotrSortVertex *vertices,
                             DotrDrawChain *chain, int z)
{
    unsigned long *p = (unsigned long *)cursor;
    int type = header->type;
    struct DotrSortVertex *vertex = vertices;
    int textured = (type & 0x200) ? 1 : 0;
    int context = (type & 0x1000) ? 1 : 0;
    unsigned long *start = p;
    struct DotrSortZBuf zbuf;
    int depth;
    int i;
    int size;
    unsigned long *q;
    struct DotrDrawPair *row;

    if ((type & 0xf) == 7) {
        return cursor;
    }
    if (count <= 0) {
        return cursor;
    }
    p[1] = 0;
    p[0] = 0;
    ((struct DotrSortTag *)p)->kind = 0x10;
    if (chain) {
        size = 1 << chain->shift;
        if (type & 0x8000) {
            z = size - 1;
        } else if ((unsigned int)(size - 2) < (unsigned int)z) {
            z = size - 2;
        }
        /* Slots are addressed by byte offset: eight bytes each on the target. */
        z <<= 3;
        ((struct DotrSortTag *)p)->next = (int)(DotrDrawWord)((DotrDrawSlot *)((DotrDrawWord)chain->slots + z))->head;
        ((DotrDrawSlot *)((DotrDrawWord)chain->slots + z))->head = (DotrDrawNode *)p;
    }
    p[2] = 0x1000000000008000;
    p[3] = 0xe;
    p[4] = (unsigned int)(type & 0xf) | ((long)(int)((type & 0x10) ? 1 : 0) << 3) | ((long)(int)((type & 0x20) ? 1 : 0) << 4)
        | ((long)(int)((type & 0x100) ? 1 : 0) << 5) | ((long)(int)((type & 0x200) ? 1 : 0) << 6) | ((long)(int)((type & 0x400) ? 1 : 0) << 7)
        | ((long)(int)((type & 0x800) ? 1 : 0) << 8) | ((long)(int)((type & 0x1000) ? 1 : 0) << 9) | ((long)(int)((type & 0x2000) ? 1 : 0) << 10);
    p[5] = 0;
    p[6] = (long)(int)(textured ? 1 : 0) | 0x5000c;
    p[7] = context + 0x47;
    p += 8;
    if (type & 0x200) {
        p[0] = header->data[0];
        p[1] = context + 0x42;
        p += 2;
    }
    if (type & 0x20) {
        p[0] = header->data[1];
        p[1] = context + 6;
        p[2] = header->data[2];
        p[3] = context + 0x14;
        p += 4;
        if (type & 0x4000) {
            p[0] = header->data[3];
            p[1] = context + 8;
            p += 2;
        }
    }
    depth = type & 0x8000;
    if (depth) {
        RhBase_GetZBufInfo(&zbuf.fields);
        zbuf.mask = 0;
        p[0] = *(unsigned long *)&zbuf;
        p[1] = context + 0x4e;
        p += 2;
    }
    for (i = 0; i < count; i++, vertex++) {
        if (i == 0 || (type & 0x10)) {
            q = p;
            p[0] = vertex->colour;
            p[1] = 1;
            p += 2;
            if (type & 0x10) {
                ((float *)q)[1] = 1.0f / vertex->q;
            }
        }
        if (type & 0x20) {
            q = p;
            p[0] = vertex->uv;
            p[1] = (type & 0x800) ? 3 : 2;
            p += 2;
            if (!(type & 0x800) && (type & 0x10)) {
                ((float *)q)[0] /= vertex->q;
                ((float *)q)[1] /= vertex->q;
            }
        }
        p[0] = vertex->xyz;
        p[1] = 4;
        p += 2;
    }
    if ((type & 0x20) && (type & 0x4000)) {
        row = gExPrim_aaRegs[context];
        p[0] = row[4].unknown_00;
        p[1] = row[4].unknown_08;
        p += 2;
    }
    if (depth) {
        zbuf.mask = 1;
        p[0] = *(unsigned long *)&zbuf;
        p[1] = context + 0x4e;
        p += 2;
    }
    size = ((p - start) >> 1) - 1;
    ((struct DotrSortTag *)start)->size = size;
    ((struct DotrSortTag *)start)->unknown_08 = 0x11000000;
    ((struct DotrSortTag *)start)->unknown_0c = (size & 0xffff) | 0x51000000;
    ((struct DotrSortTag *)start)->loops = (size & 0xffff) - 1;
    return (DotrDrawWord)p;
}
