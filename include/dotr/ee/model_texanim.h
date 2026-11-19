#ifndef DOTR_EE_MODEL_TEXANIM_H
#define DOTR_EE_MODEL_TEXANIM_H

/* Texture-animation packet builder SzModel_LoadTexAnim_Pipe and the entry layout it reads
 * (24-byte entries with bit-fields).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* A word that holds an address: `int` on the target (part_lists.h declares the cursor and
 * the result of SzModel_LoadTexAnim_Pipe as int). */
#ifdef __MWERKS__
typedef int DotrTexAnimWord;
#else
#include <stdint.h>
typedef intptr_t DotrTexAnimWord;
#endif
/* 64-bit packet word (`long` is 64 bits for the target compiler). */
typedef unsigned long DotrTexAnimLong;

/* Texture description as read here: pixel format at 0x40 and width at 0x44; other bytes
 * are not touched. It sits 16 bytes into the block the record's first word points to. */
struct DotrTexAnimTexture {
    char unknown_00[0x40];
    unsigned short psm;         /* 0x40: placed in both format fields of BITBLTBUF */
    unsigned short unknown_42;
    unsigned short width;       /* 0x44: buffer width in pixels; /64 is the BITBLTBUF width */
};
struct DotrTexAnimHead {
    char unknown_00[16];
    struct DotrTexAnimTexture tex;
};

/* One 24-byte animation entry. Member names are assumed from use; byte 3 and the
 * word at 4 are read through a different layout for each `type`. */
struct DotrTexAnimEntry {
    unsigned char last : 1;     /* 0x00 bit 0: last entry of the list */
    unsigned char count : 1;    /* bit 1: use `counter` instead of the animation frame */
    unsigned char type : 3;     /* bits 2-4: switch selector */
    unsigned char mode : 3;     /* bits 5-7: 2 and 3 negate the cell */
    unsigned char mask;         /* 0x01: one bit per animation number */
    unsigned char cols : 4;     /* 0x02 */
    unsigned char rows : 4;
    union {
        signed char cell;       /* types 0, 1 */
        struct {
            signed char dir : 2;
            signed char pos : 6;
        } swing;                /* types 2, 3 */
        struct {
            unsigned char on : 1;
        } gate;                 /* types 5, 6 */
    } u3;                       /* 0x03 */
    union {
        struct {
            unsigned short a, b;
        } h;
        struct {
            unsigned short a;
            unsigned char limit, time;
        } b;
    } u4;                       /* 0x04 */
    unsigned short w, h;        /* 0x08: cell size */
    unsigned short sx, sy;      /* 0x0c: source origin */
    unsigned short dx, dy;      /* 0x10: destination */
    int counter;                /* 0x14 */
};

/* The record words used here, as addresses (part_lists.h views the same record as int
 * words; +4 is not touched). */
struct DotrTexAnimRecord {
    struct DotrTexAnimHead *head;
    int unknown_4;
    struct DotrTexAnimEntry *entries;
};

extern int SzModel_GetPauseFlag(int modelId);
extern int SzModel_GetAnim(int modelId);
extern float SzModel_GetAnimFrame(int modelId);
/* Float-to-integer helper in the library block, as block_table.h and ex3d_clip.h declare it. */
extern int func_001a23b0(float value);

/* 0x1ea410: appends one 12-doubleword GS transfer (BITBLTBUF, TRXPOS, TRXREG, TRXDIR) per
 * active entry and, when any was written, fills the first packet's tag words. Parameter
 * names are assumed (the Japanese build gives only the function name). */
DotrTexAnimWord SzModel_LoadTexAnim_Pipe(DotrTexAnimWord cursor, int index, int position, struct DotrTexAnimRecord *record);

#endif
