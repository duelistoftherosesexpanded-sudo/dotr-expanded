#ifndef DOTR_EE_TITLE_PIPES_H
#define DOTR_EE_TITLE_PIPES_H

/* Two game-mode screen drawers, the texture test viewer and the title screen, that build GS sprite
 * packets through the Ex packet helpers.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrTitleWord;
#else
#include <stdint.h>
typedef intptr_t DotrTitleWord;
#endif

/* Packet helpers both functions call (SzText_InitClutManage, SzText_LoadRequest and
 * ExSortPrim_Pipe are declared inside the function bodies). Assumed: these types. */
int ExAssignAlpha(void *arg0, long long arg1);
int ExAssignTexture(void *arg0, void *arg1, int arg2);
long long ExGetAlphaVal(int arg0);

/* ---- lSzGame_TEXTURE_TEST (0x1a9f50), views: the GS tag, a
 * 64-bit XYZ vertex and the sprite packet with float texture coordinates. Assumed: these
 * field meanings. */
typedef struct {
    unsigned short loops;
    unsigned short primitive;
    unsigned int flags;
    unsigned long long registers;
} DotrTitleTexTag;

typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrTitleTexVertex;

typedef struct {
    DotrTitleTexTag tag;
    unsigned char r, g, b, a;
    float q;
    float s0, t0;
    DotrTitleTexVertex xyz0;
    float s1, t1;
    DotrTitleTexVertex xyz1;
    unsigned long long extra[4];
} DotrTitleTexSprite;

void ExFlushTexture(void);
int RhBase_GetUserGsAddress(void);
void SzText_Load(DotrTitleWord arg0, int arg1, int arg2);
void ExSortPrim(DotrTitleWord arg0, int arg1, int arg2);

/* 0x1a9f50. Assumed: a texture record (width and height halfwords at +0x54 / +0x56), a
 * position and a double-size flag. */
void lSzGame_TEXTURE_TEST(void *arg0, int arg1, int arg2, int arg3);

/* ---- lSzGame_DrawTitle_Pipe (0x1acc80), views: GS tag, colour,
 * UV and XYZ bit-fields, the flat rectangle and textured sprite packets, the title state
 * (intensity, selection, alpha, shade bytes from +0x224 and a hidden flag at +0x22f) and the
 * eight row cut bytes. Assumed: these field meanings. */
typedef struct {
    unsigned short count;
    unsigned short flags;
    unsigned int reserved;
    unsigned long long registers;
} DotrTitleTag;

typedef struct {
    unsigned char r, g, b, a;
    float q;
} DotrTitleColor;

typedef struct {
    unsigned long long u : 14;
    unsigned long long reserved0 : 2;
    unsigned long long v : 14;
    unsigned long long reserved1 : 34;
} DotrTitleUV;

typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long f : 8;
} DotrTitleXYZ;

typedef struct {
    unsigned long long uv;
    DotrTitleXYZ xyz;
} DotrTitleVertex;

typedef struct {
    unsigned long long tag[2];
    DotrTitleColor color;
    DotrTitleXYZ xyz[2];
    unsigned long long reserved;
} DotrTitleRect;

typedef struct {
    unsigned long long tag[2];
    DotrTitleColor color;
    DotrTitleVertex vertex[4];
} DotrTitleTexture;

typedef struct {
    signed char reserved : 1;
    signed char hidden : 1;
    signed char rest : 6;
} DotrTitleFlags;

typedef struct {
    unsigned char reserved0[0x224];
    unsigned char intensity;
    unsigned char selected;
    unsigned char alpha;
    unsigned char shade;
    unsigned char reserved1[7];
    DotrTitleFlags flags;
} DotrTitleState;

typedef union {
    long long value;
    unsigned char edge[8];
} DotrTitleCuts;

/* Data (each symbol links by address): the row cut bytes at 0x28ff28 and the title state at
 * 0x317250, declared as a word. On the host the
 * state word is the first member of a fixture large enough for the whole state. */
extern unsigned char data_0028ff28_title[];
#ifdef __MWERKS__
extern int gSzGame_gl_title;
#else
union DotrTitleStateFixture {
    int word;
    DotrTitleState state;
};
extern union DotrTitleStateFixture dotr_title_state;
#define gSzGame_gl_title (dotr_title_state.word)
#endif

int RhText_SetTextureFlush(int arg0);
int SzText_DoRequest_Pipe(int arg0, int arg1, int arg2, int *arg3);
int SzText_FreeRequest(int textureFlags, int textureId);
int lSzGame_DrawTitle_Rose_Pipe(int packet, int id, int x, int y, int width, int height, float depth);

/* 0x1acc80. Assumed: the packet cursor; returns the advanced cursor. */
int lSzGame_DrawTitle_Pipe(int arg0);

#endif
