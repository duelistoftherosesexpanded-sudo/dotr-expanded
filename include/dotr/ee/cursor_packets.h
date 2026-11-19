#ifndef DOTR_EE_CURSOR_PACKETS_H
#define DOTR_EE_CURSOR_PACKETS_H

/* Routines that take a draw cursor, build a packet in locals and pass it on through the cursor
 * helpers 0x20a0b0 / 0x207650.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#ifdef __MWERKS__
typedef int DotrDrawWord;
#else
#include <stdint.h>
typedef intptr_t DotrDrawWord;
#endif

/* The first 16 bytes of a packet as lSzDuel_Draw_Fade_Pipe fills them. */
struct DotrCursorPacketHeader {
    unsigned short unknown_00;
    unsigned short unknown_02;
    int unknown_04;
    long unknown_08;
};

/* One 64-bit vertex word: 16-bit x and y, a 24-bit value and a byte (the layout
 * draw_state.h calls DotrDrawVertex). */
struct DotrCursorVertex {
    unsigned long unknown_x : 16;
    unsigned long unknown_y : 16;
    unsigned long unknown_z : 24;
    unsigned long unknown_f : 8;
};

/* The 40-byte local of lSzDuel_Draw_Fade_Pipe: header words, four colour bytes, a float and two
 * vertex words. */
struct DotrCursorRectPacket {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorVertex vertex[2];
};

/* Callees. Assumed: ExSortPrim_Pipe's second argument is the packet address (lSzDuel_Draw_Fade_Pipe
 * passes the address of its local; draw_state.h carries it as an untyped int). */
long ExGetAlphaVal(int index);
int ExAssignAlpha(struct DotrCursorPacketHeader *packet, long value);
DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a);
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2);

/* 0x1b8260. Assumed: int arguments, the cursor result of the last call is returned. */
DotrDrawWord lSzDuel_Draw_Fade_Pipe(DotrDrawWord cursor, int x, int y, int width, int height, int alpha);

/* Storage. gSzMsg_ml is the state block slot_table.h types as DotrSlotTable (byte view
 * as in small_misc.h; the word at +628 is read as the 24-bit vertex value). gSzMsg_defRect
 * is the four-word default rectangle (same layout as in slot_table.h, which cannot be
 * included here: it declares the callees differently). icolDef_266 holds four colour bytes. */
extern unsigned char gSzMsg_ml[];
struct DotrSlotTableWords {
    int words[4];
};
extern struct DotrSlotTableWords gSzMsg_defRect[];
extern unsigned char icolDef_266[];

/* 0x1fcf90. Assumed: index is ExGetAlphaVal's argument, colour points at four bytes read
 * in the order 2, 1, 0, 3, z is unsigned (no sign extension before the 24-bit store). */
DotrDrawWord SzFade_Draw_Pipe(DotrDrawWord cursor, int index, unsigned char *colour, unsigned int z);
/* 0x1fe8f0. Assumed: rect holds x, y, width, height; null arguments select the defaults. */
DotrDrawWord SzMsg_DrawGlass_Pipe(DotrDrawWord cursor, int x, int y, struct DotrSlotTableWords *rect,
                           unsigned char *colour);

/* SzMsg_DrawRect_Pipe (0x1fdfe0). The slot entry as slot_table.h lays it out (flag in bit 31
 * of the first word, then u, v, width, height); the two words lSzMsg_GetTexInfo fills are
 * carried untyped here. iColDef_210 holds the four default colour bytes. */
struct DotrCursorSlotEntry {
    unsigned int unknown_000 : 31;
    signed int flag : 1;
    int unknown_004[4];
};
struct DotrCursorTex {
    long word;
};
extern struct DotrCursorSlotEntry *SzMsg_GetInfo(int index);
extern void lSzMsg_GetTexInfo(struct DotrCursorTex *tex0, struct DotrCursorTex *tex1);
extern unsigned char iColDef_210[];

/* Two 14-bit texel coordinates in separate halfwords, stored into the 8-byte `uv` slot. */
struct DotrCursorSlotUv {
    unsigned short u : 14;
    unsigned short v : 14;
};
struct DotrCursorSlotXyz {
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long f : 8;
};
struct DotrCursorSlotRectVertex {
    long uv;
    struct DotrCursorSlotXyz xyz;
};
/* The 88-byte local of SzMsg_DrawRect_Pipe. unknown_m8 and unknown_38 are never written by
 * it; the callees receive the address of `header`. */
struct DotrCursorSlotRectPacket {
    long unknown_m8;
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorSlotRectVertex vertex[2];
    long unknown_38;
    struct DotrCursorTex tex0;
    struct DotrCursorTex tex1;
};
/* Assumed: rect holds x, y, width, height; null rect and colour select the defaults. */
DotrDrawWord SzMsg_DrawRect_Pipe(DotrDrawWord cursor, int index, int x, int y, struct DotrSlotTableWords *rect,
                                 unsigned char *colour);

/* lSzDuel_DrawWindow_PicMini_Pipe (0x1b7f90). Views assumed from the uses: the ready item
 * starts with its texture pointer, whose first word is compared with 0x324852; the control
 * block (typed DotrOwnerDrawControl in owner_draws.h) is read only at +0xc7c. Callee
 * signatures are assumed from this call site; other headers carry them with int words. */
struct DotrCursorPicEntry {
    int *texture;
};
struct DotrCursorPicPlayer {
    unsigned char unknown_000[0x1d4];
    int unknown_1d4;            /* read by lSzDuel_DrawWindow_DualShock2_Pipe as an angle */
    unsigned char unknown_1d8[0x350 - 0x1d8];
};
struct DotrCursorPicControl {
    struct DotrCursorPicPlayer players[2];
    int textures[366];          /* 0x6a0: texture addresses, kept as words */
    int z;                      /* 0xc58 */
    unsigned char unknown_c5c[0xc7c - 0xc5c];
    int clut;
    unsigned char unknown_c80[0xd3a - 0xc80];
    unsigned char join_fade;    /* 0xd3a: tested, then passed as alpha by SzDuel_DrawJoinFade_Pipe */
    unsigned char unknown_d3b;
    unsigned char side;         /* 0xd3c: index into players */
};
extern struct DotrCursorPicControl data_00317490;
/* Assumed: one cursor argument, as the only argument register read. */
DotrDrawWord SzDuel_DrawJoinFade_Pipe(DotrDrawWord cursor);
extern struct DotrCursorPicEntry *SzText_GetReadyItem(int group, int id);
extern int SzText_InitClutManage(int base, int size);
extern DotrDrawWord SzText_Load_Pipe(DotrDrawWord cursor, int *texture, int a1, int a2);
extern DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor);
extern int ExAssignTexture(struct DotrCursorPacketHeader *packet, int *texture, int a);
/* Assumed: z is unsigned (no sign extension before the 24-bit store); bright is a flag. */
DotrDrawWord lSzDuel_DrawWindow_PicMini_Pipe(DotrDrawWord cursor, int id, int x, int y, int width, int height,
                                             unsigned int z, int bright);
/* Assumed: same parameters as the mini picture without the brightness flag. */
DotrDrawWord lSzDuel_DrawWindow_PicItem_Pipe(DotrDrawWord cursor, int id, int x, int y, int width, int height, unsigned int z);

/* lSzDuel_Draw*_Base_Pipe (0x1bb260, 0x1c38e0, 0x1c7790). Assumed from the stores: four
 * vertices of colour bytes, a float and a position word after the header; the local starts
 * 8 bytes before the header. The colour tables are read as eight bytes each; the depth
 * word is read as an int. */
struct DotrBasePipeVertex {
    unsigned char colour[4];
    float q;
    struct DotrCursorVertex xyz;
};
struct DotrBasePipePacket {
    long unknown_m8;
    long header[2];
    struct DotrBasePipeVertex v[4];
};
extern unsigned char data_002be850[8];
extern unsigned char data_002be858[8];
extern unsigned char data_002be860[8];
extern int data_003180e8;
DotrDrawWord lSzDuel_DrawBuildDeck_Base_Pipe(DotrDrawWord cursor);
DotrDrawWord lSzDuel_DrawTrade_Base_Pipe(DotrDrawWord cursor);
DotrDrawWord lSzDuel_DrawCustomDuel_Base_Pipe(DotrDrawWord cursor);

/* lSzModel_Draw_DefFunc (0x1ee570). Assumed from the stores: two vertices of two floats and a
 * position word; the local starts 8 bytes before the header and ends with the two words
 * ExFrameBuffer2Texture_Pipe receives the addresses of (never read by this function).
 * Assumed here: the cursor is a word for RhBase_MaskFBuffer (draw_state.h declares it with
 * long pointers) and the last two arguments of ExFrameBuffer2Texture_Pipe are addresses. */
struct DotrModelShotVertex {
    float u;
    float v;
    struct DotrCursorSlotXyz xyz;
};
struct DotrModelShotPacket {
    long unknown_m8;
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrModelShotVertex vertex[2];
    long unknown_38;
    struct DotrCursorTex tex0;
    struct DotrCursorTex tex1;
};
extern DotrDrawWord ExFrameBuffer2Texture_Pipe(DotrDrawWord cursor, int a, int b, struct DotrCursorTex *c,
                                               struct DotrCursorTex *d);
extern DotrDrawWord RhBase_MaskFBuffer(DotrDrawWord cursor, unsigned int mask);
DotrDrawWord lSzModel_Draw_DefFunc(DotrDrawWord cursor);

/* SzModel_ShotTextureWithFunc_Pipe (0x1efdc0). Assumed from the frame and the stores: the same
 * local shape as lSzModel_Draw_DefFunc (8 bytes before the header, two words at +72 and +80
 * that the draw call is taken to fill; this function never writes the rest); a 1-bit and a
 * 3-bit field at bits 5 and 6 of those words. Assumed here, differing from other headers:
 * the sixth argument of the two draw functions is the packet address (object_frees.h passes 0
 * as an int), ExClampMode_Pipe takes and returns the cursor as a word (draw_state.h: packet
 * pointers), cursors of the model functions are ints. gSzModel_pfhCache is declared as an int. */
struct DotrModelShotTex {
    unsigned long unknown_0 : 5;
    unsigned long flag : 1;
    unsigned long mode : 3;
};
struct DotrModelShotWork {
    long unknown_m8;
    long header[2];
    unsigned char unknown_18[40];
    long unknown_38;
    struct DotrModelShotTex tex0;
    struct DotrModelShotTex tex1;
};
extern int gSzModel_pfhCache;
extern int SzModel_GetCalcFlag(int modelId);
extern int SzModel_SetCalcFlag(int modelId, int flagValue);
extern DotrDrawWord ExDitherMatrixEz_Pipe(DotrDrawWord cursor, int index);
extern long ExGetRegVal(int a, int b);
extern DotrDrawWord ExClampMode_Pipe(DotrDrawWord cursor, long value);
extern int SzModel_DrawWithFunc_Pipe(int cursor, int index, int translationRelated, void *rotationRelated,
                                     int scaleRelated, void *f, int zbufferPtr, void (*callback)(void),
                                     int postRenderCallback);
extern int SzModel_Draw_Pipe(int cursor, int modelId, int c, void *d, int e, void *f, int g);
DotrDrawWord SzModel_ShotTextureWithFunc_Pipe(DotrDrawWord cursor, int modelId, int mode, struct DotrModelShotTex *out0,
                                              struct DotrModelShotTex *out1, void (*callback)(void), int arg);

/* Taku_Prim_Set_G4 (0x218c10). The local is the four-vertex DotrBasePipePacket. Assumed: the
 * seven register coordinates are ints (the definition moves them without sign extension;
 * short parameters would need sign extensions), y3 and z are shorts and mode an unsigned short
 * (lh/lhu from the stack). quad_offsets.h declares it for its caller with short coordinates;
 * which prototype is the original is unresolved. Returns ExSortPrim_Pipe's result. */
extern int *ExGetMyOT(void);
void TAKU_G4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                  int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3);
DotrDrawWord Taku_Prim_Set_G4(DotrDrawWord cursor, int x0, int y0, int x1, int y1, int x2, int y2, int x3,
                              short y3, unsigned char r0, unsigned char g0, unsigned char b0,
                              unsigned char a0, unsigned char r1, unsigned char g1, unsigned char b1,
                              unsigned char a1, unsigned char r2, unsigned char g2, unsigned char b2,
                              unsigned char a2, unsigned char r3, unsigned char g3, unsigned char b3,
                              unsigned char a3, unsigned short mode, short z);

/* ExSortDispBlur_Pipe (0x208110). The packet of lSzModel_Draw_DefFunc without the leading
 * doubleword: header (5, 38, 0, 0x42421), four colour bytes, 1.0 and two textured vertices;
 * the local is 88 bytes, of which this function writes the first 56 (the tail is assumed to
 * belong to the callees). A null colour pointer selects a 4-byte copy of the object at
 * 0x293ed0, taken as a 4-byte structure because it is copied whole. Assumed: the fourth
 * argument is the int ExAssignFrameBuffer takes, the fifth the long ExAssignAlpha takes.
 * draw_state.h declares this function for its caller with int arguments throughout. */
struct DotrBlurColour {
    unsigned char c[4];
};
struct DotrBlurPacket {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrModelShotVertex vertex[2];
    long unknown_38[4];
};
extern struct DotrBlurColour data_00293ed0;
extern int ExAssignFrameBuffer(unsigned short *p, int b);
DotrDrawWord ExSortDispBlur_Pipe(DotrDrawWord cursor, int a1, int a2, int buffer, long alpha,
                                 unsigned char *colour);

/* ExSortSoftFocus_Pipe (0x208280). The ExSortDispBlur_Pipe packet with its tail named: one
 * untouched doubleword, then the two words ExFrameBuffer2Texture_Pipe receives the addresses
 * of (80 bytes, no leading doubleword). ASSUMED: the first of them is read here as a 34-bit
 * field and a flag at bit 34 (the shape draw_state.h calls DotrFakeTex); the function sets
 * only that bit. data_00293ed8 is the four default colour bytes, copied whole like
 * data_00293ed0. func_00208870_soft is ExFrameBuffer2Texture_Pipe seen with this packet's
 * word type (a distinct view of the callee declared above). Assumed: the GS address comes
 * from RhBase_GetUserGsAddress as an int; a1 and a2 are passed through to ExSortPrim_Pipe,
 * a3 to the frame-buffer call. window_font.h declares this function for its caller with
 * seven int-like arguments (its only caller passes cursor, 0, 0, 0, 0, 12, 0). */
struct DotrSoftFocusTex {
    unsigned long low : 34;
    unsigned long flag : 1;
};
struct DotrSoftFocusPacket {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrModelShotVertex vertex[2];
    long unknown_38;
    struct DotrSoftFocusTex tex0;
    struct DotrCursorTex tex1;
};
extern struct DotrBlurColour data_00293ed8;
extern int RhBase_GetUserGsAddress(void);
extern DotrDrawWord func_00208870_soft(DotrDrawWord cursor, int a, int b, struct DotrSoftFocusTex *c,
                                       struct DotrCursorTex *d);
DotrDrawWord ExSortSoftFocus_Pipe(DotrDrawWord cursor, int a1, int a2, int a3, int x, int y,
                                  unsigned char *colour);

/* SzMsg_DrawRectG_Pipe (0x1fe290, original name). SzMsg_DrawRect_Pipe with four vertices
 * that each carry their own colour: header (12, 52 | 0x800, 0, 0x421421421421), then four
 * 24-byte vertices (colour, 1.0, texel, position), one untouched doubleword and the two
 * words lSzMsg_GetTexInfo fills. The local is 144 bytes and starts at the header (no
 * leading doubleword here); the last 8 bytes are never written by this function. A null
 * colour selects 16 bytes at 0x293080. Open: that range runs over 0x293088, which this
 * header also declares as a separate 4-byte object for SzMsg_DrawGlass_Pipe; the object
 * boundaries there are not established. */
struct DotrCursorRectGVertex {
    unsigned char colour[4];
    float unknown_04;
    long uv;
    struct DotrCursorSlotXyz xyz;
};
struct DotrCursorRectGPacket {
    long header[2];
    struct DotrCursorRectGVertex vertex[4];
    long unknown_70;
    struct DotrCursorTex tex0;
    struct DotrCursorTex tex1;
    long unknown_88;
};
extern unsigned char icolDef_226[];
DotrDrawWord SzMsg_DrawRectG_Pipe(DotrDrawWord cursor, int index, int x, int y, struct DotrSlotTableWords *rect,
                                  unsigned char *colour);

/* SzMsg_DrawRatioX_Pipe (0x1fe660, compiled warm; see its build entry): draws slot `index` with
 * its width and height scaled by sx and sy. Copies the default rectangle, replaces its width and
 * height by the slot entry's (words +0xc and +0x10) times the factors converted to int (casts; the
 * compiler calls the helper 0x1a22c0), and calls `function` with the cursor, index, x, y,
 * the rectangle and `e`; returns its result, or the cursor unchanged when the slot has no
 * entry. Assumed: the rectangle is passed by value (the callee receives its address, as
 * SzMsg_DrawRect_Pipe expects), the two floats are declared before `e` and `function` (they
 * travel in f12/f13 either way). MATCHING DEVICE: gSzMsg_defRect_rect is a four-member view of
 * gSzMsg_defRect[0]; copying through the array declaration or a cast would give four word
 * copies instead of the original's single 16-byte copy. */
struct DotrCursorRatioRect {
    int unknown_0;
    int unknown_4;
    int width;
    int height;
};
typedef DotrDrawWord (*DotrCursorRatioDraw)(DotrDrawWord cursor, int index, int x, int y, struct DotrCursorRatioRect rect, int e);
extern struct DotrCursorRatioRect gSzMsg_defRect_rect;
extern int func_001a22c0(float value);
DotrDrawWord SzMsg_DrawRatioX_Pipe(DotrDrawWord cursor, int index, int x, int y, float sx, float sy, int e,
                                   DotrCursorRatioDraw function);

/* lSzDuel_DrawWindow_DualShock2_Pipe (0x1b7820). A 10-byte icon entry (texture index, then two
 * texel corners) as element 1 of an array at 0x290800; the packet type below is the four-vertex
 * form of DotrCursorSlotRectPacket, kept for the host test's view of what is sorted. The
 * geometry callees are declared in cursor_pad_geom.h (view_state.h, which some includers of
 * this header also include, declares RhGeom_PushMatrix with an int argument). The function's
 * own views follow the lSzDuel_OpenHandCursor_Pipe ones below. */
struct DotrCursorPadIcon {
    unsigned char texture;
    unsigned short u0, v0, u1, v1;
};
struct DotrCursorPadPacket {
    long unknown_m8;
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorSlotRectVertex vertex[4];
    struct DotrCursorTex tex0;
    struct DotrCursorTex tex1;
    long unknown_70;
};
extern struct DotrCursorPadIcon data_00290800[];
#ifdef __MWERKS__
#define DOTR_CURSOR_PAD_SCRATCH ((float (*)[4])0x70000000)
#else
extern float dotr_cursor_pad_scratch_70000000[8][4];
#define DOTR_CURSOR_PAD_SCRATCH (dotr_cursor_pad_scratch_70000000)
#endif

/* lSzDuel_OpenHandCursor_Pipe (0x1e4570) views (their sizes and the absence of extra alignment
 * decide the stack layout): eight corner vectors at 0x291600 (copied as 128 bytes), four rotation
 * words at 0x291680, a 4x4 matrix local, the 120-byte packet (header, colour and q, four uv/xyz
 * corners, four more words) and the packet header. The two icon entries are reached at their own
 * addresses 0x290814 and 0x29081e (entries 2 and 3 of data_00290800); on the host they are those
 * entries of the fixture. Assumed: angle is a 16-bit rotation, x and y are screen coordinates of
 * the centre. */
struct DotrCursorHandQuad {
    float v[8][4];
};
struct DotrCursorHandRotation {
    int v[4];
};
struct DotrCursorHandMatrix {
    float m[4][4];
};
struct DotrCursorHandTag {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
};
/* 64-bit uv and xyz words; the game's code uses 64-bit bit-fields, the host the same
 * little-endian layout in 32-bit units. */
#ifdef __MWERKS__
struct DotrCursorHandUv {
    unsigned long long u : 14;
    unsigned long long pad0 : 2;
    unsigned long long v : 14;
    unsigned long long pad1 : 34;
};
struct DotrCursorHandXyz {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
};
#else
struct DotrCursorHandUv {
    unsigned int u : 14;
    unsigned int pad0 : 2;
    unsigned int v : 14;
    unsigned int pad1 : 2;
    unsigned int pad2;
};
struct DotrCursorHandXyz {
    unsigned int x : 16;
    unsigned int y : 16;
    unsigned int z : 24;
    unsigned int fog : 8;
};
#endif
struct DotrCursorHandCorner {
    unsigned long long uv;
    struct DotrCursorHandXyz xyz;
};
struct DotrCursorHandPacket {
    unsigned long long tag[2];
    unsigned char r, g, b, a;
    float q;
    struct DotrCursorHandCorner corners[4];
    unsigned long long extra[4];
};
extern struct DotrCursorHandQuad data_00291600_hand;
extern struct DotrCursorHandRotation data_00291680_hand;
#ifdef __MWERKS__
extern unsigned char data_00290814_hand[];
extern unsigned char data_0029081e_hand[];
#else
#define data_00290814_hand ((unsigned char *)&data_00290800[2])
#define data_0029081e_hand ((unsigned char *)&data_00290800[3])
#endif
DotrDrawWord lSzDuel_OpenHandCursor_Pipe(DotrDrawWord cursor, int angle, int x, int y);

/* lSzDuel_DrawWindow_DualShock2_Pipe (0x1b7820) views (no extra alignment; with it the stack layout
 * differs): the corner template of four vectors at 0x2912e0, the four rotation words at 0x291320,
 * the 4x4 matrix and the 120-byte packet of lSzDuel_OpenHandCursor_Pipe; the icon entry at its own
 * address 0x29080a (entry 1 of data_00290800) and the side byte at 0x3181cc (the `side` member of
 * data_00317490) under their own symbols. Assumed: x, y are screen coordinates of the centre, alpha
 * is the fourth colour byte. */
struct DotrCursorDs2Quad {
    float v[4][4];
};
extern struct DotrCursorDs2Quad data_002912e0;
extern struct DotrCursorHandRotation data_00291320;
#ifdef __MWERKS__
extern unsigned char data_0029080a_ds2[];
extern unsigned char WhoGoesFirst_ds2;
#else
#define data_0029080a_ds2 ((unsigned char *)&data_00290800[1])
#define WhoGoesFirst_ds2 (data_00317490.side)
#endif
DotrDrawWord lSzDuel_DrawWindow_DualShock2_Pipe(DotrDrawWord cursor, int x, int y, int alpha);

#endif
