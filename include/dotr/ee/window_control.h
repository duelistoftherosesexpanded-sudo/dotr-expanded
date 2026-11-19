#ifndef DOTR_EE_WINDOW_CONTROL_H
#define DOTR_EE_WINDOW_CONTROL_H

/* Views of the duel control block at 0x317490 and of the unit record, as far as the
 * information-window functions read them (window_font.h, window_panels.h). Structure layouts
 * and prototypes are inferred, not the original declarations. The assumptions are listed with
 * the functions that use each member. */
struct DotrWindowUnit {
    unsigned char unknown_00[64];
    unsigned char x;
    unsigned char y;
    unsigned char unknown_42[2];
    unsigned char side;
    unsigned char place;
    unsigned char unknown_46[17];
    signed char unknown_57_bit_0 : 1;
    signed char unknown_57_bits_1_3 : 3;
    signed char unknown_57_bit_4 : 1;
    signed char unknown_57_bit_5 : 1;
    signed char unknown_57_bit_6 : 1;
    signed char unknown_57_bit_7 : 1;
};
struct DotrWindowSide {
    unsigned char unknown_000[826];
    unsigned short points;
    unsigned char player;
    unsigned char x;
    unsigned char y;
    unsigned char unknown_33f[2];
    unsigned char hand;
    unsigned char unknown_342[4];
    signed char unknown_346_bit_0 : 1;
    signed char unknown_346_high : 7;
    unsigned char unknown_347[9];
};
struct DotrWindowControl {
    struct DotrWindowSide sides[2];
    unsigned char unknown_6a0[40];
    struct DotrWindowUnit *unit_6c8;
    struct DotrWindowUnit *unit_6cc;
    unsigned char unknown_6d0[1416];
    unsigned int unknown_c58;
    unsigned char unknown_c5c[217];
    unsigned char unknown_d35;
    unsigned char unknown_d36[6];
    unsigned char side;
    unsigned char unknown_d3d;
    unsigned char slide;
    unsigned char slide2;
    unsigned char x;
    unsigned char y;
    unsigned char x2;
    unsigned char y2;
    unsigned char unknown_d44;
    unsigned char unknown_d45;
    unsigned char unknown_d46;
    unsigned char unknown_d47;
    unsigned char unknown_d48;
    unsigned char unknown_d49[6];
    unsigned char unknown_d4f;
    unsigned char unknown_d50[3];
    unsigned char unknown_d53;
    unsigned char unknown_d54;
    unsigned char unknown_d55[2];
    unsigned char unknown_d57;
    unsigned char unknown_d58[13];
    signed char unknown_d65_low : 4;
    signed char unknown_d65_bit_4 : 1;
    signed char unknown_d65_high : 3;
    signed char unknown_d66_low : 3;
    signed char unknown_d66_bit_3 : 1;
    signed char unknown_d66_bits_4_6 : 3;
    signed char unknown_d66_bit_7 : 1;
    signed char unknown_d67_bit_0 : 1;
    signed char unknown_d67_bit_1 : 1;
    signed char unknown_d67_bit_2 : 1;
    signed char unknown_d67_bit_3 : 1;
    signed char unknown_d67_bit_4 : 1;
    signed char unknown_d67_high : 3;
};
extern struct DotrWindowControl data_00317490_window;

#endif
