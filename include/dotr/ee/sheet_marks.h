#ifndef DOTR_EE_SHEET_MARKS_H
#define DOTR_EE_SHEET_MARKS_H

/* Functions that copy a 38-byte sheet-item template to the stack, pick a position and row from
 * bytes at fixed addresses near 0x1ed5370 and draw it through 0x217bd0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* The 38-byte item 0x217bd0 draws (small_state.h has the same layout as DotrSmallSheetItem);
 * only `row` is written here. */
struct DotrMarkItem {
    short x0, y0, x1, y1;
    unsigned char unknown_08[8];
    unsigned char color[4];
    unsigned char unknown_14[12];
    unsigned char unknown_20;
    unsigned char unknown_21;
    short unknown_22;
    unsigned short row;
};
/* View of 0x299e10 (glide_steps.h and movie_operations.h have others): byte +0, short +0xc2. */
struct DotrMarkBoard {
    signed char mode;
    signed char unknown_01[193];
    short ticks;
};
/* Bytes at fixed addresses from 0x1ed537b on (glide_steps.h views the same storage from
 * 0x1ed5370). 0x22f1c0 addresses them as literal addresses (each access has its own `lui`,
 * also in branch delay slots), so the target build uses an integer constant; the host build
 * points the name at an object the test defines. Member names are assumed. */
struct DotrMarkFlags {
    signed char state;         /* 0x1ed537b */
    unsigned char unknown_01[2];
    signed char corner;        /* 0x1ed537e */
    unsigned char unknown_04[0x2d];
    signed char stage;         /* 0x1ed53ac */
    unsigned char unknown_32[5];
    unsigned char frame;       /* 0x1ed53b2 */
};
#ifdef __MWERKS__
#define DOTR_MARK_FLAGS ((struct DotrMarkFlags *)0x1ed537b)
#else
extern struct DotrMarkFlags dotr_mark_flags_01ed537b;
#define DOTR_MARK_FLAGS (&dotr_mark_flags_01ed537b)
#endif

/* The initialised 38 bytes at 0x29abc0 that 0x22f1c0 copies to its local. Whether the original
 * had a named object or a compiler-made initialiser template is not known. */
extern struct DotrMarkItem data_0029abc0;
extern struct DotrMarkBoard data_00299e10_mark;

/* 0x217bd0 is defined in small_state, which declares it void; it ends in a tail call, and
 * 0x22f1c0 keeps its v0 as the new first argument, so this view returns int. */
extern int func_00217bd0_mark(int a0, struct DotrMarkItem *item, short x, short y);

int Taku_Camp_BOTAN_MATI(int a0);

#endif
