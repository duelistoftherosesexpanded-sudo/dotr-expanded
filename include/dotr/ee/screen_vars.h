#ifndef DOTR_EE_SCREEN_VARS_H
#define DOTR_EE_SCREEN_VARS_H

/* lRhBase screen-variable setup around the double-buffer block, which the library's display-buffer
 * setup routine 0x1851e8 fills.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The block lRhBase_InitScreenVariables fills. Only the members it reads or writes are evidenced;
 * the rest is padding. The first 560 bytes are passed to 0x1851e8 (in the library block, not
 * reconstructed), and the member names of that part follow the SDK double-buffer structure its
 * argument pattern suggests: a hypothesis, not a recovered declaration. */
struct DotrScreenDispFb {
    unsigned long FBP : 9;
    unsigned long unknown : 55;
};
struct DotrScreenDisplay {
    unsigned int unknown_lo;
    unsigned int DW : 12;
    unsigned int DH : 11;
    unsigned int unknown_hi : 9;
};
struct DotrScreenDispEnv {
    long unknown_00[2];
    struct DotrScreenDispFb dispfb;
    struct DotrScreenDisplay display;
    long unknown_20;
};
struct DotrScreenPageWord {
    unsigned long page : 9;
    unsigned long unknown : 55;
};
struct DotrScreenDrawEnv {
    struct DotrScreenPageWord frame;
    long unknown_08;
    struct DotrScreenPageWord zbuf;
    long unknown_18[13];
};
struct DotrScreenClear {
    long unknown_00[4];
    unsigned char r, g, b, a;
    float q;
    long unknown_28[7];
};
struct DotrScreenDBuff {
    struct DotrScreenDispEnv disp[2];
    long unknown_50[2];
    struct DotrScreenDrawEnv draw0;
    struct DotrScreenClear clear0;
    long unknown_140[2];
    struct DotrScreenDrawEnv draw1;
    struct DotrScreenClear clear1;
};
struct DotrScreenVars {
    struct DotrScreenDBuff db;
    int unknown_230[2];
    int mode;
    int width;
    int height;
    float unknown_244;
    int unknown_248;
    int unknown_24c;
    int psm;
    int psm_index;
    int zpsm;
    int zpsm_index;
    int unknown_260;
    int unknown_264;
    int unknown_268;
    int unknown_26c;
};

/* One 40-byte row of the table at 0x293f10 (`gRhSys_asd` in the Japanese build's symbols; the
 * member names are this project's). draw_state.h declares the same storage as bytes; a cast of that
 * declaration compiles to one shared base register, while the original forms the address of
 * each member separately, as a structure-typed object does, hence this distinct view symbol. */
struct DotrRhSysAsd {
    int unknown_00[3];
    int width;
    int height;
    float unknown_14;
    int unknown_18;
    int unknown_1c;
    int pages[2];
};
extern struct DotrRhSysAsd gRhSys_asd_view[];
#define DOTR_RH_SYS_ASD gRhSys_asd_view

/* Callee in the library block; prototype as small_misc.h carries it (assumed). */
extern void func_001851e8(int *, int, int, int, int, int, int);

/* 0x20ad10: copies the mode row into the block, records the two pixel-format arguments and a
 * 0/1 index for each, has 0x1851e8 fill the buffer block and, when both display pages come back
 * equal, assigns the pages from the row. Returns nothing (v0 is not set). */
void lRhBase_InitScreenVariables(struct DotrScreenVars *s, int mode, int psm, int zpsm);

#endif
