#ifndef DOTR_EE_CARD_CONTROL_H
#define DOTR_EE_CARD_CONTROL_H

/* SzDuel card control interpolation (0x1cdeb0): reads one gSzDuel_aCardControl element and writes
 * the card it points to.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* 0x1a22c0 is in the library block; assumed float to int, as in the other groups. */
extern int func_001a22c0(float value);
/* The interpolated object: offsets from the stores; member names are assumed (four floats, two
 * sets of four 16-bit angles kept in words, four floats, four bytes written in the order 2, 1,
 * 0, 3). */
struct DotrCardControlOut {
    float pos[4];
    int rot[4];
    int rot2[4];
    float scale[4];
    unsigned char color[4];
};
/* One gSzDuel_aCardControl element (0xa0 bytes; small_misc.h views the same storage as
 * DotrMiscSlot160 / DotrMiscRect): start and end values per channel, a step count over a total
 * and one-bit channel flags in the byte at +0x9f. Member names are assumed. */
struct DotrCardControl {
    struct DotrCardControlOut *out;
    int unknown_04[3];
    float pos[2][4];
    int rot[2][4];
    int rot2[2][4];
    float scale[2][4];
    unsigned char color[2][4];
    unsigned short total;
    unsigned short count;
    unsigned char unknown_9c[3];
    signed char unknown_bits_0_2 : 3;
    signed char do_pos : 1;
    signed char do_rot : 1;
    signed char do_rot2 : 1;
    signed char do_scale : 1;
    signed char do_color : 1;
};
int SzDuel_DoCardControl0(struct DotrCardControl *c);

#endif
