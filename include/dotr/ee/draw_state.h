#ifndef DOTR_EE_DRAW_STATE_H
#define DOTR_EE_DRAW_STATE_H

/* A group of small state accessors and packet helpers, most of them near 0x208460 and
 * 0x209d20. Meanings are unassigned; evidence is in each function's docs/modules entry
 * (mainly render-files.md and math-prims.md).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Several target words hold addresses; the host uses a pointer-sized integer for
 * them, so host sizes differ. */
#ifdef __MWERKS__
typedef int DotrDrawWord;
#else
#include <stdint.h>
typedef intptr_t DotrDrawWord;
#endif

struct DotrDrawState {
    unsigned char unknown_000[0x100];
    unsigned char unknown_100[4];
    int unknown_104;
    unsigned char unknown_108[0xe8];
    unsigned char unknown_1f0[4];
    int unknown_1f4;
    unsigned char unknown_1f8[0x44];
    int unknown_23c;
    int unknown_240;
    float unknown_244;
    int unknown_248;
    unsigned char unknown_24c[0x14];
    int unknown_260;
    int unknown_264;
    int unknown_268;
    int unknown_26c;
    /* Words up to +0x2a0 (the size RhBase_Init clears); +0x278 is the one stored by member. */
    int unknown_270;
    DotrDrawWord unknown_274;
    int unknown_278;
    DotrDrawWord unknown_27c;
    int unknown_280;
    int unknown_284;
    int unknown_288;
    int unknown_28c;
    int unknown_290;
    /* Called by RhBase_FlipBuffer when not null: +0x298 before the vertical-sync wait, +0x294 after it. */
    void (*unknown_294)(void);
    void (*unknown_298)(void);
    int unknown_29c;
};

struct DotrDrawFlag {
    signed char unknown_bit_0 : 1;
    signed char unknown_bits_1_7 : 7;
};

struct DotrDrawSelector {
    unsigned char unknown_00[16];
    int unknown_10;
};

/* 16-byte header followed by 64-bit entries. */
struct DotrDrawPacket {
    unsigned short unknown_count;
    unsigned short unknown_flags;
    unsigned char unknown_04[12];
};

struct DotrDrawPair {
    long unknown_00;
    long unknown_08;
};

struct DotrDrawTag {
    int unknown_00;
    DotrDrawWord unknown_04;
    int unknown_08;
    int unknown_0c;
};

/* Four 64-bit words written by 0x207fa0. */
struct DotrDrawBlock {
    long unknown_words[4];
};

extern struct DotrDrawState gRhBase_dl;
extern struct DotrDrawFlag data_00325f40[];
extern struct DotrDrawSelector gRhBase_rl;
extern DotrDrawWord data_003261d0[];
extern int data_003261e4[];
extern struct DotrDrawPair gExPrim_aaRegs[][7];
extern long gExPrim_aqwAlpha[];
extern long g_aqwExDitherMatrix[];
extern int data_003261a8[];

/* View of 0x325dd0-0x325e2f: a word, two groups of four floats,
 * two four-float vectors, three floats and a word. */
struct DotrDrawVector4 {
    float x, y, z;
    float unknown_w;
};

struct DotrDrawView {
    int unknown_00;
    unsigned char unknown_04[12];
    float unknown_10[4];
    float unknown_20[4];
    struct DotrDrawVector4 unknown_30;
    struct DotrDrawVector4 unknown_40;
    float unknown_50;
    float unknown_54;
    float unknown_58;
    int unknown_5c;
};

typedef char DotrDrawViewMustBe60[(sizeof(struct DotrDrawView) == 0x60) ? 1 : -1];

extern struct DotrDrawView gRhVu1Mac_State;

/* A second view of 0x325f20 used by 0x209d80: a word at +0x18 and two
 * flag bits at +0x20 (the byte data_00325f40 also names). */
struct DotrDrawFlagPair {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bits_2_7 : 6;
};

struct DotrDrawSelectorView {
    unsigned char unknown_00[0x18];
    int unknown_18;
    unsigned char unknown_1c[4];
    struct DotrDrawFlagPair unknown_20;
};

extern struct DotrDrawSelectorView data_00325f20_view[];

void ExFlushTexture(void);
void ExZBufferMode(int a);
void ExDitherMatrixEz(int a);
long ExGetRegVal(int a, int b);
int ExAssignAlpha(struct DotrDrawPacket *packet, long value);
int ExAssignClamp(struct DotrDrawPacket *packet, long value);
void ExFrameBuffer2Texture(int a, int b, int c, int d);
long ExGetAlphaVal(int index);
struct DotrDrawTag *RhVu1Mac_ReserveChangeDrawMethod(struct DotrDrawTag *tag);
struct DotrDrawTag *RhVu1Mac_SetViewScreenMatrix(struct DotrDrawTag *tag, void *source);
struct DotrDrawTag *RhVu1Mac_SetScreenWorldMatrix(struct DotrDrawTag *tag, void *source);
DotrDrawWord RhBase_GetVifPacketArea(void);
void RhBase_SetVifPacketArea(DotrDrawWord value);
void ShSlot_Draw(void);
struct DotrDrawPacket *ExClampMode_Pipe(struct DotrDrawPacket *packet, long value);
struct DotrDrawPacket *ExDitherMode_Pipe(struct DotrDrawPacket *packet, int value);
DotrDrawWord ExDitherMatrixEz_Pipe(DotrDrawWord cursor, int index);
struct DotrDrawTag *RhVu1Mac_ChangeDrawMethod(struct DotrDrawTag *tag, int flags);
int RhVu1Mac_GetPointLight(struct DotrDrawVector4 *a, struct DotrDrawVector4 *b, float *c, float *d, float *e);
struct DotrDrawTag *RhVu1Mac_SetClipParameter(struct DotrDrawTag *tag, int mode, float x, float y, float z);
struct DotrDrawTag *RhVu1Mac_SetFogParameterEx(struct DotrDrawTag *tag, float a, float b, float c, float d);
struct DotrDrawTag *RhVu1Mac_SetFogOnOff(struct DotrDrawTag *tag, int a, int b);
int RhBase_GetZMinMax(int *a, int *b);
int RhBase_GetBufferFormatEx(int a, int *x, int *y, int *z);
struct DotrDrawPacket *RhBase_SetFogColor(struct DotrDrawPacket *packet, unsigned char *bytes);
void RhBase_FreeRenderFrame(void);
/* A third view of 0x325f20 used by 0x209f70: words at +0, +0xc and +0x14 and
 * the two flag bits at +0x20. The four-byte record is what 0x209f70 reads at
 * gRhBase_dl + 96 or + 336; the packet argument is assumed to be a 64-bit
 * cursor and the second argument an unsigned word (zero-extended by the code). */
struct DotrDrawFrameWords {
    int unknown_00;
    unsigned char unknown_04[8];
    int unknown_0c;
    unsigned char unknown_10[4];
    int unknown_14;
    unsigned char unknown_18[8];
    struct DotrDrawFlagPair unknown_20;
};
struct DotrDrawFrameFields {
    unsigned short unknown_00;
    unsigned char unknown_02;
    unsigned char unknown_03;
};
extern struct DotrDrawFrameWords data_00325f20_frame;
long *RhBase_MaskFBuffer(long *p, unsigned int mask);
/* A fourth view of 0x325f20 used by 0x20a610: words at +0, +4, +8, +0xc,
 * +0x10 (0x20a470 only), +0x14, +0x18 and +0x1c and the two flag bits at +0x20. */
struct DotrDrawFrameSetup {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
    int unknown_18;
    int unknown_1c;
    struct DotrDrawFlagPair unknown_20;
};
extern struct DotrDrawFrameSetup data_00325f20_setup;
/* 0x20a610. With flag bit 0 clear answers `p` unchanged. Otherwise writes
 * twelve 64-bit words (tag words 0x10000005 / 0 / 0x11000000 / 0x51000005,
 * 0x1000000000008004, 14, a frame word from +0xc, the 0x293f1c table value
 * of row +0 >> 6 and +0x14, 76, a second one from +0xc + +0x1c and +0x18,
 * 78, 1, 74, (+4 - 1) << 16 | (+8 - 1) << 48, 64), sets flag bit 1 and
 * answers p + 12. Assumed: 64-bit cursor argument and result. */
long *RhBase_EnableRenderFrame(long *p);
/* 0x20a760. With flag bit 0 clear answers `p` unchanged. Otherwise writes
 * twelve 64-bit words (the same four tag words, 0x1000000000008004, 14, the
 * four 64-bit words at 0x325f50 + 336 (word +0x26c odd) or + 96, 0, 74,
 * (+0x23c - 1) << 16 | (+0x240 - 1) << 48, 64), clears flag bit 1 and
 * answers p + 12. Assumed: 64-bit cursor argument and result. */
long *RhBase_DisableRenderFrame(long *p);
/* 0x20a870. With flag bit 0 or bit 1 clear answers `p` unchanged. Otherwise
 * writes six 64-bit words (tag words 0x10000009 / 0 / 0x11000000 /
 * 0x51000009, 0x1000000000008008, 14, 0, 74), lets the SDK-block routine
 * 0x184ff8 write its entries after them with the four colour bytes (the
 * four-byte record at 0x293f00 when `color` is null), advances by twice its
 * result, writes 1 and 74 and answers the position after those. Assumed: the
 * four-byte record type, eleven int arguments for 0x184ff8 (as movie_start
 * calls it) and a 64-bit cursor. */
struct DotrDrawColor { unsigned char c[4]; };
extern struct DotrDrawColor data_00293f00;
extern int func_00184ff8(long *, int, int, int, int, int, int, int, int, int, int);
long *RhBase_ClearRenderFrame(long *p, struct DotrDrawColor *color);
/* 0x20a470. With flag bit 0 already set answers 0. Otherwise stores the
 * index at +0, the words at 0x293f1c and 0x293f20 of the index's 40-byte row
 * at +4 and +8, `mode` at +0x14 and `kind` at +0x18; picks x = 1 for mode 0
 * or 1 (else 0) and y = 1 for kind 48 or 49 (else 0); stores the word +0x268
 * of 0x325f50 >> 5 at +0xc, row word [x] of 0x293f30 at +0x1c and
 * [x] + [y] at +0x10. Answers 0 when +0xc + +0x10 exceeds 512; else sets
 * flag bit 0, clears bit 1 and answers 1 (the stores stay either way).
 * Assumed: three int arguments, int result. */
extern char data_00293f30[];
int RhBase_AssignRenderFrame(int index, int mode, int kind);
/* Four-byte record whose low nine bits and low four bits of byte +3 are
 * written by 0x20a280; the same layout is read at 0x325fc0 (inside the
 * storage gRhBase_dl names, at +0x70). Other bits are not interpreted. */
struct DotrDrawFields {
    unsigned short low_9 : 9;
    unsigned short unknown_bits_9_15 : 7;
    unsigned char unknown_02;
    unsigned char low_4 : 4;
    unsigned char unknown_bits_4_7 : 4;
};
extern struct DotrDrawFields data_00325fc0_fields[4];
/* 0x20a280. With both flag bits of 0x325f40 set, stores (+0xc + +0x1c of
 * 0x325f20) in the record's nine-bit field and +0x18 in its four-bit field;
 * otherwise copies both fields from the record at 0x325fc0.
 * Assumed: record pointer argument, no result. */
void RhBase_GetZBufInfo(struct DotrDrawFields *fields);
/* Two 64-bit words of bit-fields written by 0x20aa30. The widths come from
 * the masks in the code; no register meaning is assigned. Padding members
 * are bits the function does not touch. */
struct DotrDrawBits64A {
    unsigned long unknown_bits_0_13 : 14;
    unsigned long unknown_bits_14_19 : 6;
    unsigned long unknown_bits_20_25 : 6;
    unsigned long unknown_bits_26_29 : 4;
    unsigned long unknown_bits_30_33 : 4;
    unsigned long unknown_bit_34 : 1;
    unsigned long unknown_bits_35_36 : 2;
    unsigned long unknown_bits_37_50 : 14;
    unsigned long unknown_bits_51_54 : 4;
    unsigned long unknown_bit_55 : 1;
    unsigned long unknown_bits_56_60 : 5;
    unsigned long unknown_bits_61_63 : 3;
};
struct DotrDrawBits64B {
    unsigned long unknown_bit_0 : 1;
    unsigned long unknown_bit_1 : 1;
    unsigned long unknown_bits_2_4 : 3;
    unsigned long unknown_bit_5 : 1;
    unsigned long unknown_bits_6_8 : 3;
    unsigned long unknown_bit_9 : 1;
    unsigned long unknown_bits_10_18 : 9;
    unsigned long unknown_bits_19_20 : 2;
    unsigned long unknown_bits_21_31 : 11;
    unsigned long unknown_bits_32_43 : 12;
    unsigned long unknown_bits_44_63 : 20;
};
/* 0x20aa30. With flag bit 0 clear answers 0. Otherwise, for a non-null
 * `a`: bits 0-13 = +0xc of 0x325f20 << 5, bits 14-19 = W >> 6 (W, H = the
 * words at 0x293f1c / 0x293f20 of row +0), bits 20-25 = +0x14, bits 26-29 =
 * 9 when W <= 320 else 10, bits 30-33 = 8 when H <= 224 else 9, bit 34 = 1,
 * bits 35-60 = 0, bits 61-63 = 1. For a non-null `b`: bit 0 = 0, bits 2-4 =
 * 0, bit 5 = 1, bits 6-8 = 1, bit 9 = 0, bits 19-20 = 0, bits 32-43 = 0.
 * Answers 1. Assumed: the two pointer types, int result. */
int RhBase_GetTextureInfoForRender(struct DotrDrawBits64A *a, struct DotrDrawBits64B *b);
/* 0x100610 stores six 64-bit words at fixed addresses 0x12000000-0x120000d0
 * (volatile fixed-address accesses; no peripheral meaning is assigned here).
 * Only whether the argument is zero is used. */
void useSoftFocus(int flag);

/* One 64-bit vertex word: 16-bit x and y, a 24-bit value and a byte. */
struct DotrDrawVertex {
    unsigned long unknown_x : 16;
    unsigned long unknown_y : 16;
    unsigned long unknown_z : 24;
    unsigned long unknown_f : 8;
};

typedef char DotrDrawVertexMustBe8[(sizeof(struct DotrDrawVertex) == 8) ? 1 : -1];

#define DOTR_DRAW_VERTEX(p, offset) ((struct DotrDrawVertex *)((char *)(p) + (offset)))

void SzDuel_SetupCard(int a0, int a1);
void SzFade_Draw(int a0, int a1, int a2);
int SzFade_Disp(int index);
void SzMsg_LoadFont(int a0, int a1, int a2, int a3);
void SzMsg_DrawX(int a0, int a1, int a2, int a3, int a4);
void SzMsg_DrawGlass(int a0, int a1, int a2, int a3);
void SzText_Load(int a0, int a1, int a2);
void ExSortPrim(int a0, int a1, int a2);
void ExSortDispBlur(int a0, int a1, int a2, int a3, int a4);
int SzMsg_Init(int a0, int a1);
int SzText_DoRequest(int a0, int a1);
void TAKU_SP_XYWH(void *packet, int x, int y, int width, int height, int z, int f);
void TAKU_FT4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                   int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3);
void TAKU_G4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                   int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3);
void TAKU_GT4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                   int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3);
void SzModel_ShotTextureWithFunc(int modelId, int b, int c, int d, int e, int f);
float RhBase_GetAspectY(int *out);
int RhBase_GetScreenSize(int *a, int *b);
int RhBase_GetScreenCenter(int *a, int *b);
int RhBase_GetUserGsAddress(void);
int RhBase_SetClearColorEx(int a, int b, int c, int d);
int RhBase_SetVSyncCallback(int value);
void SzDuel_StartCardSession(void);
void SzDuel_FinishCardSession(void);
void lSzGame_DrawTitle(void);
void SzDuel_DrawStageAfter(void);
void SzDuel_DrawDialog(void);
void SzDuel_DrawWindow(void);
void SzDuel_DrawJoinFade(void);
void SzDuel_DrawJoinCard(void);
void SzDuel_DrawBuildDeck(void);
void SzDuel_Draw3DView(void);
void SzDuel_DrawTrade(void);
void SzDuel_DrawCustomDuel(void);
void SzDuel_DrawMapEdit(void);
void SzDuel_DrawAllCard(void);
void SzDuel_DrawUnit(void);
void SzEvent_Draw(void);
void RhZSort_Finish(void);
void SzDuel_DrawCard(int a);
void SzModel_DrawMotionBlur(int a);
void TkCamp_Draw(void);
void ShFace_Draw(void);
DotrDrawWord ExSetAnchor(void);
struct DotrDrawBlock *ExSetAnchor_Pipe(struct DotrDrawBlock *block);

/* Assumed prototypes of callees from packet_builds (RhText_SetTextureFlush,
 * RhBase_SetZBufferMode), shslot (ShSlot_Draw_Pipe) and frame_texture
 * (ExFrameBuffer2Texture_Pipe); ExAssignFrameBuffer is defined in this group. */
extern DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor);
extern DotrDrawWord ShSlot_Draw_Pipe(DotrDrawWord cursor);
extern DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a);
extern DotrDrawWord ExFrameBuffer2Texture_Pipe(DotrDrawWord cursor, int a, int b, int c, int d);
int ExAssignFrameBuffer(unsigned short *p, int b);
/* Callees of RhBase_ResetGsEnv (0x2098b0) with assumed int arguments (only the call order is
 * established); data_00326180 is the draw-state word table, and gRhSys_asd (0x293f10) and
 * data_00293f14..data_00293f20 are fields of a 40-byte-per-row table, each a separate symbol
 * on purpose. */
extern void func_001869c0(int, int, int);
extern void func_00186148(int);
/* RhBase_FlipBuffer tests the result for a negative value, so the return type is int. */
extern int func_00185578(int, int);
extern void func_001849b0(void);
extern void func_001885c8(void);
extern void func_00184810(int, int, int, int);
extern int *lRhBase_InitGsAndVif(int);
extern void func_0017bb60(int);
extern void func_00186470(int, int);
extern int data_00326180[];
extern char gRhSys_asd[];
extern char data_00293f14[];
extern char data_00293f18[];
extern char data_00293f1c[];
extern char data_00293f20[];
void RhBase_ResetGsEnv(int i);
/* Callees of RhBase_CopyFrameBuffer (arguments assumed). It copies the 640x448 frame-buffer
 * region from GS local memory to main memory at its argument `x` (BITBLTBUF source fields,
 * TRXDIR 1, local to host). data_00325e80 is the store-image transfer packet record that
 * func_00185890 builds and func_001859d0 runs, not a pixel buffer; both take it as their
 * first argument. */
extern void func_00185890(void *, int, int, int, int, int, int, int);
extern void func_001859d0(void *, unsigned long);
extern char data_00325e80[];
void RhBase_CopyFrameBuffer(unsigned long x);
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
/* RhMem_CopyMemory16 (above) is defined in memory_ops with a typed prototype and is declared
 * here with untyped pointers; the word copy and 64-bit fill helpers below are also from
 * memory_ops. */
extern int *RhMem_CopyMemory4(int *destination, int *source, int count);
extern long *RhMem_FillMemory8(long *destination, long value, int count);
extern DotrDrawWord SzDuel_StartCardSession_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_FinishCardSession_Pipe(DotrDrawWord cursor);
extern DotrDrawWord ShFace_Draw_Pipe(DotrDrawWord cursor);
/* Routines called by the cursor wrappers; assumed prototypes from the call sites. */
extern DotrDrawWord SzDuel_SetupCard_Pipe(DotrDrawWord cursor, int unit, DotrDrawWord a1);
extern DotrDrawWord SzFade_Draw_Pipe(DotrDrawWord cursor, int a0, int a1, int a2);
extern DotrDrawWord SzMsg_LoadFont_Pipe(DotrDrawWord cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern DotrDrawWord SzMsg_DrawX_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3, int a4);
extern DotrDrawWord SzMsg_DrawGlass_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3);
extern DotrDrawWord SzText_Load_Pipe(DotrDrawWord cursor, int a0, int a1, int a2);
extern DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, int a0, int a1, int a2);
extern DotrDrawWord ExSortDispBlur_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3, int a4);
extern DotrDrawWord SzMsg_Init_Pipe(DotrDrawWord cursor, int a0, int a1, int *out);
extern DotrDrawWord SzText_DoRequest_Pipe(DotrDrawWord cursor, int a0, int a1, int *out);
extern DotrDrawWord SzModel_ShotTextureWithFunc_Pipe(DotrDrawWord cursor, int modelId, int b, int c, int d, int e, int f);
extern DotrDrawWord func_001dbd30_cursor(DotrDrawWord cursor, int a);
extern DotrDrawWord SzModel_DrawMotionBlur_Pipe(DotrDrawWord cursor, int a);
extern DotrDrawWord TkCamp_Draw_Pipe(DotrDrawWord cursor);
extern signed char Move_END_flg[];
extern DotrDrawWord lSzGame_DrawTitle_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawStageAfter_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawDialog_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawWindow_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawJoinFade_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawJoinCard_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawBuildDeck_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_Draw3DView_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawTrade_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawCustomDuel_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawMapEdit_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawAllCard_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_DrawUnit_Pipe(DotrDrawWord cursor);
extern DotrDrawWord SzEvent_Draw_Pipe(DotrDrawWord cursor);
extern DotrDrawWord RhZSort_Finish_Pipe(DotrDrawWord cursor);

/* SzMsg_Free is defined in slot_table; SzDuel_FinishCardSessionX_Pipe in this group. */
void SzMsg_Free(int index);
struct DotrDrawPacket *SzDuel_FinishCardSessionX_Pipe(struct DotrDrawPacket *packet);

/* Chain of 8-byte slots indexed by a clamped value; layouts assumed from the
 * single caller-visible access pattern of ExSortAnchor_Pipe (placeholder names). */
typedef struct DotrDrawSlot {
    int unknown_00;
    struct DotrDrawNode *head;
} DotrDrawSlot;
typedef struct DotrDrawNode {
    int unknown_00;
    struct DotrDrawNode *next;
} DotrDrawNode;
typedef struct DotrDrawChain {
    DotrDrawSlot *slots;
    int unknown_04;
    int unknown_08;
    int shift;
} DotrDrawChain;
void *ExSortAnchor_Pipe(unsigned char *packet, DotrDrawNode *node, DotrDrawChain *chain, unsigned int index);
/* ExSortAnchor runs ExSortAnchor_Pipe on the current VIF packet area and stores the
 * returned end; parameter types are taken from the callee. */
void ExSortAnchor(DotrDrawNode *node, DotrDrawChain *chain, unsigned int index);

/* 0x207aa0 ExSortNode_Pipe. Views taken from its accesses only (placeholder member names):
 * the packet header (count, flag word, 64-bit register words from +0x10), a 48-byte vertex
 * (q at +0xc, position, colour and texture words at +0x10/+0x18/+0x20), the eight-byte Z-buffer
 * local (the record RhBase_GetZBufInfo fills, mask bit at +4) and the 16-byte chain tag followed by
 * the loop count of the next tag. The tag's link is a 32-bit target address: the host stores
 * the low half of the pointer. Assumed: (cursor, header, count, vertices, chain, slot) with the
 * end cursor as the result; other headers keep integer views of the same routine. */
struct DotrSortHeader {
    unsigned short count;
    unsigned short type;
    int unknown_04;
    unsigned long regs;
    unsigned long data[4];
};
struct DotrSortVertex {
    float unknown_00[3];
    float q;
    unsigned long xyz;
    unsigned long colour;
    unsigned long uv;
    unsigned long unknown_28;
};
struct DotrSortZBuf {
    struct DotrDrawFields fields;
    unsigned char mask : 1;
    unsigned char unknown_bits_1_7 : 7;
    unsigned char unknown_05[3];
};
struct DotrSortTag {
    unsigned short size;
    unsigned char unknown_02;
    unsigned char kind;
    int next;
    int unknown_08;
    int unknown_0c;
    unsigned short loops : 15;
    unsigned short end : 1;
};
DotrDrawWord ExSortNode_Pipe(DotrDrawWord cursor, struct DotrSortHeader *header, int count, struct DotrSortVertex *vertices,
                             DotrDrawChain *chain, int z);

/* Packet context filled by ExClearOt: ent is the entry array, w1/w2 are
 * the two packet buffers and shift is the entry-count exponent. Target
 * pointers are 32-bit; the host layout is wider, so host tests seed w1 and
 * allocate entries below 4 GiB. */
typedef struct DotrPktEnt { unsigned short a; unsigned short b; int next; } DotrPktEnt;
typedef struct DotrPktCtx { DotrPktEnt *ent; unsigned short *w1; char *w2; int shift; } DotrPktCtx;
extern void ExClearOt(DotrPktCtx *ctx, DotrPktEnt *ent, int shift);
void ExBindPrim(DotrPktCtx *ctx);

/* RhVu1Mac_SetStraightLightToVu1_Ex (0x208f00). For slot 0-2 with both vectors given, stores
 * floats 1-3 of the light as row `slot` of the colour matrix at 0x325d90 and the direction
 * as column `slot` of the matrix at 0x325d50. With a tag it then copies the colour matrix
 * to a 16-byte aligned local, multiplies it by the direction matrix (RhGeom_MulMatrix,
 * 0x20bc00), writes the words 0x10000004, 0, 0, 0x6c04003f and the product, and
 * returns tag + 80 bytes; with no tag and a negative slot it clears both matrices. Parameter
 * types are those of the caller in block_table.h; the float[16] matrices are assumed.
 * RhGeom_MulMatrix is declared in draw_quad_block.h, which the source includes (view_state.h
 * views its arguments differently). */
struct DotrBlockTriple;
struct DotrBlockVector4;
extern float gRhVu1Mac_matDir[16];
extern float gRhVu1Mac_matCol[16];
struct DotrDrawTag *RhVu1Mac_SetStraightLightToVu1_Ex(struct DotrDrawTag *tag, struct DotrBlockTriple *light,
                                                      struct DotrBlockVector4 *dir, int slot);
/* RhVu1Mac_SetPointLightToVu1 (0x209110): writes a 48-byte packet (header, then two vectors,
 * or zeros when a or b is null) at tag and records the parameters in gRhVu1Mac_State.
 * Returns tag + 48. */
struct DotrDrawTag *RhVu1Mac_SetPointLightToVu1(struct DotrDrawTag *tag, float *a, float *b, float x, float y, float z);

/* Object view for 0x1dbc40: four colour bytes at 0x40, a pointer at 0x78 to
 * 48-byte rows, and a bitfield word at 0x80 (bit 15 and a signed 12-bit count
 * at bits 16-27). Only these fields are evidenced; the third argument's type
 * is assumed from its use and is forwarded to 0x1da7f0 as a word. */
struct DotrDrawRowOwner {
    unsigned char unknown_00[0x40];
    unsigned char color[4];
    unsigned char unknown_44[0x34];
    unsigned char *rows;
    unsigned char unknown_7c[4];
    signed int unknown_bits_0_14 : 15;
    signed int flag : 1;
    signed int count : 12;
};
DotrDrawWord SzDuel_SetupCard2_Pipe(DotrDrawWord cursor, int unit, struct DotrDrawRowOwner *owner);

/* View of the storage at 0x317250 used by 0x1a9800: a byte at +0x228 and a
 * 2-bit field at bits 4-5 of the byte at +0x22c. The function reads the
 * bitfield through the symbol and the byte through a register, hence the
 * second view symbol (same address; the same object on the host). Callee
 * signatures are assumed from this caller only. */
struct DotrDrawModeView {
    unsigned char unknown_000[0x228];
    unsigned char busy;
    unsigned char unknown_229[3];
    unsigned char unknown_bits_0_3 : 4;
    unsigned char mode : 2;
};
extern struct DotrDrawModeView gSzGame_gl_mode;
#ifdef __MWERKS__
extern struct DotrDrawModeView gSzGame_gl_modeview;
#else
#define gSzGame_gl_modeview gSzGame_gl_mode
#endif
extern int SzGame_GetMode(int a);
extern void TkCamp_Draw_After(void);
void SzGame_ModeDrawAfter(void);

/* 0x21c450 and its callees (assumed prototypes). 0x1fdfe0 is passed by address only;
 * the argument order of SzMsg_DrawRatioX_Pipe (six integer registers, two float registers)
 * and all parameter meanings are assumed from this caller. The sixth
 * parameter of Taku_Bunsyou_Set (t1) is never read. */
extern void SzMsg_DrawRect_Pipe(void);
extern DotrDrawWord SzMsg_DrawRatioX_Pipe(DotrDrawWord cursor, int index, short x, short y, int e,
                                  void (*function)(void), float sx, float sy);
DotrDrawWord Taku_Bunsyou_Set(DotrDrawWord cursor, int index, int a2, int x, int y, int unused, int e);

/* 0x1da5e0 fills four 48-byte vertices of a centred quad (120x180, or
 * 256x368 when mode is non-zero), optionally scaled, at the depth returned by
 * SzView_GetScreenDistance(0) (defined in block_table). The vertex layout, field names and
 * the position of the float parameters (cx before out, cy last) are assumed;
 * the parameter order is the one that reproduces the register moves. */
struct DotrDrawQuadVertex {
    float x, y, z, w;
    unsigned char pad_10[8];
    unsigned char color[4];
    float q;
    float u, v;
    unsigned char pad_28[8];
};
struct DotrDrawQuadScale {
    float x, y;
};
extern float SzView_GetScreenDistance(int index);
int SzDuel_GetCardNodeUnitOnScreen(int mode, float cx, struct DotrDrawQuadVertex *out, struct DotrDrawQuadScale *scale,
                  float cy);

/* SzMsg_LoadClut_Pipe (0x1fd4c0): builds a 16-entry colour table in a stack buffer (each of
 * the three low bytes from gSzMsg_abRGB[index][i], the fourth from gSzMsg_abAlpha[index][i]),
 * writes a 14-doubleword header at the cursor with `slot` in the high half of doubleword 4,
 * appends the table with RhMem_CopyMemory4 and closes the leading tag with the doubleword-pair
 * count. Returns the copy's result (v0 is left untouched after the call). Row width 16 is observed; the row counts and the
 * meaning of the two tables are not established. Signature as slot_table.h declares it for its
 * caller (integer cursor in and out). */
extern unsigned char gSzMsg_abRGB[][16];
extern unsigned char gSzMsg_abAlpha[][16];
DotrDrawWord SzMsg_LoadClut_Pipe(DotrDrawWord cursor, int index, int slot);

/* RhBase_Init (0x209510). Seven int parameters are the registers read; the callees below take
 * the registers set at the call as their arguments (return types assumed).
 * The timer-mode bit-field follows the write order and widths seen in the code; its member
 * names are those of the EE timer mode register and are assumed. The 28-byte record is
 * only passed by address, with the short at +6 set. On the host the two timer registers
 * are objects the test defines. */
struct DotrDrawTimerMode {
    unsigned CLKS : 2;
    unsigned GATE : 1;
    unsigned GATS : 1;
    unsigned GATM : 2;
    unsigned ZRET : 1;
    unsigned CUE : 1;
    unsigned CMPE : 1;
    unsigned OVFE : 1;
    unsigned EQUF : 1;
    unsigned OVFF : 1;
    unsigned p0 : 20;
};
struct DotrDrawBit6 {
    unsigned char unknown_bits_0_5 : 6;
    unsigned char unknown_bit_6 : 1;
};
struct DotrDrawEnv {
    short unknown_00[3];
    short unknown_06;
    char unknown_08[20];
};
#ifdef __MWERKS__
#define DOTR_DRAW_T1_COUNT (*(volatile unsigned *)0x10000800)
#define DOTR_DRAW_T1_MODE (*(volatile unsigned *)0x10000810)
#else
extern volatile unsigned dotr_draw_t1_count_10000800;
extern volatile unsigned dotr_draw_t1_mode_10000810;
#define DOTR_DRAW_T1_COUNT dotr_draw_t1_count_10000800
#define DOTR_DRAW_T1_MODE dotr_draw_t1_mode_10000810
#endif
extern void *func_00198948(void *destination, int value, int byte_count);
extern int RhFile_Init(void);
extern void func_00186fc8(void);
extern void func_00186bd0(void);
extern void func_00186410(struct DotrDrawEnv *e);
extern void func_00186238(struct DotrDrawEnv *e);
extern DotrDrawWord func_00186120(int a);
extern void lRhBase_InitScreenVariables(struct DotrDrawState *b, int mode, int a, int c);
extern int func_0017b7d0(void);
extern void func_0017b770(int a, int b);
extern int RhGeom_Init(void);
extern int RhView_Init(void);
extern int RhMemCard_Init(void);
extern void RhZSort_Start(int vifPacketArea, int b);
extern int func_001854e0(int a);

/* RhBase_FlipBuffer (0x2099d0). Hardware words it writes besides the timer count: DMA channel 1
 * at 0x10009000 (control), 0x10009020 (count) and 0x10009030 (tag address) and the word at
 * 0x1000e010; host variables stand in for them as for the timer. Read from the code, not from
 * names: frameSkipCounter is a count of frames to wait for, func_00185578 a path wait whose
 * negative result resets the GS environment, func_001854e0 the vertical-sync wait (called a
 * second time when the buffer height is the mode's interlaced height and the first result is
 * not 0), func_00186060 (env, short, short, short) and func_00185480 (state, frame count) the
 * half-offset and buffer-swap calls, lRhBase_SetRandomDither (0x20b1e0) a cursor filter. All of
 * those are stubbed in the host test (the func_ ones are library code). Returns 2 when the
 * timer count at the end exceeds 540, 270 or 640 (mode kind 2 with height 448, kind 2
 * otherwise, other kinds), else 1. */
#ifdef __MWERKS__
#define DOTR_DRAW_D1_CHCR (*(volatile unsigned *)0x10009000)
#define DOTR_DRAW_D1_QWC (*(volatile unsigned *)0x10009020)
#define DOTR_DRAW_D1_TADR (*(volatile unsigned *)0x10009030)
#define DOTR_DRAW_D_STAT (*(volatile unsigned *)0x1000e010)
#else
extern volatile unsigned dotr_draw_d1_chcr_10009000;
extern volatile unsigned dotr_draw_d1_qwc_10009020;
extern volatile unsigned dotr_draw_d1_tadr_10009030;
extern volatile unsigned dotr_draw_d_stat_1000e010;
#define DOTR_DRAW_D1_CHCR dotr_draw_d1_chcr_10009000
#define DOTR_DRAW_D1_QWC dotr_draw_d1_qwc_10009020
#define DOTR_DRAW_D1_TADR dotr_draw_d1_tadr_10009030
#define DOTR_DRAW_D_STAT dotr_draw_d_stat_1000e010
#endif
extern int frameSkipCounter;
extern void RhFile_VSyncProcess(void);
extern int RhMemCard_Act(void);
extern int SzGame_InqSoftFocus(void);
extern void func_00186060(void *env, short x, short y, short half);
extern void func_00185480(struct DotrDrawState *state, int frame);
extern DotrDrawWord lRhBase_SetRandomDither(DotrDrawWord cursor);
int RhBase_FlipBuffer(void);
extern char data_00286980[];
int RhBase_Init(int mode, int a1, int a2, int a3, int a4, int a5, int a6);

/* clearGsMem (0x241c20). The tag bit-field is the widths and write order seen in the code
 * (member names are those of a GIF tag and are assumed); the block at 0x350410 is that tag
 * followed by 64-bit words handed to func_00184e10, of which only word 4 is cleared here.
 * The 16-byte packet record is filled by the callees; only the word at +4 is read. The
 * packet callees are library-block routines: arguments are the registers set at each call. The
 * quadword member of the tag union gives the host object the alignment the cast store needs. */
#ifdef __MWERKS__
typedef __int128 DotrDrawWord128;
#else
__extension__ typedef __int128 DotrDrawWord128 __attribute__((may_alias));
#endif
struct DotrDrawGifTag {
    unsigned long NLOOP : 15;
    unsigned long EOP : 1;
    unsigned long pad16 : 16;
    unsigned long id : 14;
    unsigned long PRE : 1;
    unsigned long PRIM : 11;
    unsigned long FLG : 2;
    unsigned long NREG : 4;
    unsigned long REGS0 : 4;
    unsigned long REGS1 : 4;
    unsigned long REGS2 : 4;
    unsigned long REGS3 : 4;
    unsigned long REGSx : 48;
};
typedef union DotrDrawQuad {
    DotrDrawWord128 q;
    long l[2];
} DotrDrawQuad;
struct DotrDrawClearEnv {
    union {
        struct DotrDrawGifTag bits;
        DotrDrawWord128 q;
    } giftag;
    struct { unsigned long v; } draw[14];
};
struct DotrDrawGifPacket {
    DotrDrawWord unknown_00;
    DotrDrawWord base;
    DotrDrawWord unknown_08[2];
};
extern struct DotrDrawClearEnv data_00350410;
extern DotrDrawQuad data_0029b2c0;
extern int func_00184e10(void *env, short a, short w, short h, int d, int e);
extern void func_00185100(struct DotrDrawClearEnv *e);
extern void func_001872f8(struct DotrDrawGifPacket *p, DotrDrawWord128 *base);
extern void func_00187308(struct DotrDrawGifPacket *p);
extern void func_00187480(struct DotrDrawGifPacket *p, int a, int b, int c);
extern void func_00187510(struct DotrDrawGifPacket *p, DotrDrawWord128 tag);
extern void func_001875d0(struct DotrDrawGifPacket *p, int reg, long value);
extern void func_00187528(struct DotrDrawGifPacket *p);
extern void func_00187318(struct DotrDrawGifPacket *p);
void clearGsMem(int r, int g, int b, int disp_width, int disp_height);

/* 0x241e80 setImageTag. Tags at 0x29b2d0 and 0x29b2e0 are copied whole to locals as
 * clearGsMem does with 0x29b2c0. Assumed: unsigned address parameters (the packet base is
 * (tags & 0x0fffffff) | 0x20000000, the reference address image & 0x0fffffff), int mode,
 * width and height; 0x187370 with three ints, 0x1874f8 answering the reserved words,
 * 0x1873e8 as (packet, address, size, 0, 0, 0). Those three are SDK-block
 * routines. */
extern DotrDrawQuad data_0029b2d0;
extern DotrDrawQuad data_0029b2e0;
extern void func_00187370(struct DotrDrawGifPacket *p, int a, int b, int c);
extern unsigned long *func_001874f8(struct DotrDrawGifPacket *p, int count);
extern void func_001873e8(struct DotrDrawGifPacket *p, unsigned int address, int size, int a, int b, int c);
void setImageTag(unsigned int tags, unsigned int image, int mode, int width, int height);

#endif
