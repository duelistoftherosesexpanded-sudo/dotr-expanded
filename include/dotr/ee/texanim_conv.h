#ifndef DOTR_EE_TEXANIM_CONV_H
#define DOTR_EE_TEXANIM_CONV_H

/* lSzModel_TexAnimConv, which parses a texture-animation text description into a newly
 * allocated binary table.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Types, macros, data and callee prototypes: the 24-byte table entry with its bit fields (the
 * layout SzModel_LoadTexAnim_Pipe reads, model_texanim.h) and the texture header's width and
 * height at +0x44. Field meanings are assumed. */

#ifndef NULL
#define NULL 0
#endif

/* The texture file address arrives as a 32-bit word: int for the target, pointer-sized on the
 * host. */
#ifdef __MWERKS__
typedef int DotrTexAnimWord;
#else
typedef long DotrTexAnimWord;
#endif

union DotrTexAnimB3 {
    unsigned char raw;
    struct {
        signed char a : 2;
        signed char b : 6;
    } s;
    struct {
        unsigned char c : 1;
        unsigned char : 7;
    } u;
};

union DotrTexAnimV6 {
    unsigned short w;
    struct {
        unsigned char lo;
        unsigned char hi;
    } b;
};

struct DotrTexAnimEntry {
    unsigned char last : 1;
    unsigned char plain : 1;
    unsigned char kind : 3;
    unsigned char align : 3;
    unsigned char mask;
    unsigned char lo : 4;
    unsigned char hi : 4;
    union DotrTexAnimB3 b3;
    unsigned short v4;
    union DotrTexAnimV6 v6;
    unsigned short x8;
    unsigned short xA;
    unsigned short xC;
    unsigned short xE;
    unsigned short x10;
    unsigned short x12;
    int x14;
};

struct DotrTexAnimHeader {
    unsigned char pad[0x44];
    unsigned short w;
    unsigned short h;
};

#define DOTR_TEXANIM_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_TEXANIM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define DOTR_TEXANIM_CLAMP(x, lo, hi) DOTR_TEXANIM_MIN(DOTR_TEXANIM_MAX(x, lo), hi)

/* The keyword strings (0x292f40, seven entries) and the format, separator and message strings
 * at 0x2cfa72-0x2cfb4d. */
extern char *apchMode[];
extern char data_002cfa72[];
extern char data_002cfa75[];
extern char data_002cfaa2[];
extern char data_002cfac2[];
extern char data_002cfaf2[];
extern char data_002cfb16[];
extern char data_002cfb18[];
extern char data_002cfb32[];
extern char data_002cfb34[];
extern char data_002cfb3a[];
extern char data_002cfb3c[];
extern char data_002cfb40[];
extern char data_002cfb45[];
extern char data_002cfb4b[];
extern char data_002cfb4d[];

/* Callees with their prototypes. The func_ ones are C library routines in the library block
 * (unnamed here); their roles (tokeniser, copy, length, compare, scan, message) are assumed from
 * the prototypes. */
int func_0017da98(const char *arg0, ...);
int func_001950a0(const char *s);
int func_0019b6e0(const char *str, const char *fmt, ...);
int func_0019b8f8(const char *s1, const char *s2);
int func_0019bc90(const char *a, const char *b);
char *func_0019bdd8(char *dst, const char *src);
int func_0019bef0(const char *s);
int func_0019c028(const char *s1, const char *s2, unsigned int n);
char *func_0019d430(char *s, const char *delim);
DotrTexAnimWord SzMem_Alloc(int arg0);

/* 0x1e9340: arg0 the texture file, arg1 the description text (tokenised in place). */
unsigned char *lSzModel_TexAnimConv(DotrTexAnimWord arg0, unsigned char *arg1);

#endif
