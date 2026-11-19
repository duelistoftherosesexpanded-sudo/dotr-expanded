#ifndef DOTR_EE_BLEND_SLOTS_H
#define DOTR_EE_BLEND_SLOTS_H

/* 0x1bcce0: walks the eight 4-byte records behind the pointer at 0x317b60 and passes each live one
 * to 0x1bd8a0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* One of eight records behind data_00317b60. The last byte is read as a 5-bit
 * unsigned field and three sign-extended single bits. */
struct DotrBlendSlot {
    unsigned short id;      /* 0x0 */
    unsigned char level;    /* 0x2: stepped down by 16 per call, floor 0 */
    unsigned char n : 5;    /* 0x3 */
    signed char b5 : 1;
    signed char b6 : 1;
    signed char b7 : 1;
};

extern struct DotrBlendSlot *data_00317b60;

/* lSzDuel_DrawBuildDeck_GhostCard_Pipe (title_draws) as this caller uses it. The last parameter is unsigned
 * char because the caller passes its byte without masking it again; the rest are assumed int. */
extern int lSzDuel_DrawBuildDeck_GhostCard_Pipe(int a, int id, int c, int d, int e, unsigned char f);

/* lSzDuel_DrawBuildDeck_Ghost_Pipe: base and out are used only in address arithmetic (base + 8,
 * base + 328, out + 112 + n * 54), so they are spelled int. */
int lSzDuel_DrawBuildDeck_Ghost_Pipe(int a, int base, int out);

/* lSzDuel_ReqGhostCard (0x1ba520; name from the Japanese build's symbol table): claims the
 * first record whose level is 0, stores the id, the low five bits of n and bit 0 of each of
 * the three flags, sets the level to 0x80 and returns 1; returns 0 when all eight are live.
 * The flags are narrowed to a signed byte before their bit is taken (the code sign-extends
 * them), so they are spelled int with a cast; their original types are not known. */
int lSzDuel_ReqGhostCard(short id, unsigned char n, int b5, int b6, int b7);

#endif
