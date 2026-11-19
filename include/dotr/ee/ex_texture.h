#ifndef DOTR_EE_EX_TEXTURE_H
#define DOTR_EE_EX_TEXTURE_H

/* ExAssignTexture, which copies two texture words and two bit fields into a flagged packet after
 * its header.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/math-prims.md. */

/* One 64-bit word of the packet after the header, with the two bit fields 0x208700 writes:
 * bit 5 of the low byte and bits 6-8 of the low halfword. Field meanings not established. */
struct DotrExTexBits {
    unsigned char lo : 5;
    unsigned char b5 : 1;
    unsigned char hi : 2;
};
struct DotrExTexBits16 {
    unsigned short lo : 6;
    unsigned short f6 : 3;
    unsigned short rest : 7;
};
union DotrExTexWord {
    long l;
    struct DotrExTexBits b;
    struct DotrExTexBits16 h;
};
/* View of the packet header: a halfword count, a halfword of flags (bit 0x20 tested), then
 * 64-bit words from +16. Assumed: the count is the number of words before the two the
 * texture fills. */
struct DotrExTexHdr {
    unsigned short count;
    unsigned short flags;
    int unknown_04;
    long unknown_08;
    union DotrExTexWord w[1];
};
/* View of the texture: the 64-bit words at +48 and +64 are copied. */
struct DotrExTexTex {
    char unknown_00[48];
    long w48;
    char unknown_38[8];
    long w64;
};

/* 0x208700 ExAssignTexture: when the header's flag 0x20 is set and the texture is not null,
 * copies the texture words at +48 and +64 to words count + 1 and count + 2, writes the third
 * argument into bits 6-8 of the second word and that value's low bit into its bit 5, and
 * returns 1; otherwise returns 0. MATCHING DEVICES: the count and the word base read into
 * locals before the tests; the chained assignment of the two bit fields. Assumed: parameter
 * types (a pointer to a texture-like object, an int value). */
int ExAssignTexture(struct DotrExTexHdr *h, struct DotrExTexTex *tex, int a);

#endif
