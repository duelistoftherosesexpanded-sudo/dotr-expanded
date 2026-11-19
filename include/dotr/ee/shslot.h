#ifndef DOTR_EE_SHSLOT_H
#define DOTR_EE_SHSLOT_H

/* The ShSlot slot-machine minigame (original slotLIB, compiled unoptimised): reel start, line
 * checks, hit requests, flash and reel drawing, set-up and the per-frame act.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md.
 *
 * Where the code reads one storage through two incompatible C types, two symbols are used. */

/* A word that holds a packet cursor or an address kept in an int: int on the target,
 * wide enough for a host pointer in the host tests. */
#ifdef __MWERKS__
typedef int DotrShSlotWord;
#else
typedef long DotrShSlotWord;
#endif

/* Marks a local the code copies but never reads (host builds warn otherwise). */
#ifdef __MWERKS__
#define DOTR_SHSLOT_UNUSED
#else
#define DOTR_SHSLOT_UNUSED __attribute__((unused))
#endif

/* The line/cell flash state at 0x351210. Assumed: active is non-zero while a won line
 * flashes; line_timer[n] counts down the flash of line n (0 centre row, 1 top row,
 * 2 bottom row, 3 and 4 the diagonals); cell_timer[reel][row] the flash of one cell.
 * Values above 1 count down once per drawn frame. */
struct DotrShSlotBoard {
    int active;
    unsigned char line_timer[5];
    unsigned char cell_timer[3][3];
};

/* One reel at 0x351240 (three of them, 0x3a bytes each). state is used only in reel 0
 * (the game phase 0..4); enabled is the reel phase (0 idle, 1 speeding up, 2 spinning,
 * 3 braking, 4 stopped); position is the scroll in 1/112 symbol steps (wrapping at
 * 0x351248), angle the 0..350 background scroll; center the symbol on the centre row
 * when stopped (0xffff none); symbols the reel's strip of card numbers. */
struct DotrShSlotReel {
    unsigned char state;
    unsigned char unknown_01[5];
    unsigned short frame;
    short limit;
    unsigned char enabled;
    unsigned char countdown;
    unsigned char unknown_0c[6];
    short position;
    short angle;
    short velocity;
    unsigned short center;
    unsigned short symbols[16];
};

/* The same reel bytes as ShSlot_Init writes them (it steps a byte pointer to reach each
 * symbol through cards[0]): position, velocity and center under other names. */
struct DotrShSlotReelRow {
    unsigned char unknown_00[0x12];
    short position;
    unsigned char unknown_14[2];
    short velocity;
    unsigned short center;
    unsigned short cards[21];
};

/* The reel table data_00351230 points to (the literal 0x1ed0000 in the demo set-up):
 * count symbols per reel, flags (bit 0: each reel has its own strip; bit 1: random extra
 * brake time), three strips, and five per-line stop cards (0xfffe: draw one from the
 * centre row through TaSlot_GetFeverGain; 0xffff: line not in play). */
struct DotrShSlotDeck {
    unsigned char count;
    unsigned char flags;
    unsigned char unknown_2[6];
    unsigned short cards[3][21];
    unsigned short extra[5];
};

/* One flash/particle object at 0x351180 (three, 0x30 bytes each), set by ShSlot_Start
 * and drawn by ShSlot_Flear. The first four bytes are the ShFx timer (age at +0,
 * duration at +2); flags bit 0x80 draws in front, bit 0x40 keeps the alpha fixed;
 * divisions the triangles per quarter circle (at most 9). */
struct DotrShSlotFlash {
    unsigned char age;
    unsigned char unknown_01;
    unsigned char duration;
    unsigned char unknown_03;
    unsigned char flags;
    unsigned char unknown_05[2];
    unsigned char divisions;
    unsigned char b, g, r, unknown_0b;
    unsigned char center_b, center_g, center_r, alpha;
    float radius;
    float speed;
    unsigned char unknown_18[8];
    float x, y;
    unsigned char unknown_28[8];
};

/* The packet ShSlot_Flear builds on its stack: a 16-byte header (count, primitive word,
 * flags, register list) and three vertices (colour, q, 64-bit position with 16-bit x/y,
 * 24-bit z and the ADC bit), read also as two 16-bit coordinates. */
union DotrShSlotFlearPosition {
    struct {
        unsigned short x, y;
        unsigned int packed;
    } coordinates;
    struct {
        unsigned long long x : 16;
        unsigned long long y : 16;
        unsigned long long z : 24;
        unsigned long long adc : 8;
    } bits;
};

struct DotrShSlotFlearVertex {
    unsigned char r, g, b, a;
    float q;
    union DotrShSlotFlearPosition position;
};

struct DotrShSlotFlearPacket {
    unsigned short count;
    unsigned short primitive;
    unsigned int flags;
    unsigned long long registers;
    struct DotrShSlotFlearVertex vertex[3];
    unsigned long long padding;
};

/* GS packet pieces the drawing functions build on their stacks: the 16-byte header word
 * pair read as a tag (count, flags, reserved, register list), a colour with q, a 64-bit
 * position (16-bit x/y, 24-bit z, 8-bit fog/ADC), a texture coordinate pair (14-bit u
 * and v) and the vertices made of them. */
struct DotrShSlotTag {
    short count;
    unsigned short flags;
    int reserved;
    long long registers;
};

struct DotrShSlotColor {
    unsigned char r, g, b, a;
    float q;
};

struct DotrShSlotXyz {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long f : 8;
};

struct DotrShSlotUv {
    unsigned short u : 14;
    unsigned short : 2;
    unsigned short v : 14;
    unsigned short : 2;
    unsigned int padding;
};

struct DotrShSlotTexVertex {
    unsigned long long texture;
    struct DotrShSlotXyz xyz;
};

struct DotrShSlotColorVertex {
    struct DotrShSlotColor color;
    struct DotrShSlotXyz xyz;
};

/* ShSlot_DrawEye's sprite packet (no leading tag word). */
struct DotrShSlotEyePacket {
    unsigned long long header[2];
    struct DotrShSlotColor color;
    struct DotrShSlotTexVertex v0, v1, v2, v3;
    unsigned long long texture[4];
};

/* The textured sprite packet of ShSlot_FlashBox and ShSlot_DrawReel. */
struct DotrShSlotTexPacket {
    unsigned long long tag;
    unsigned long long header[2];
    struct DotrShSlotColor color;
    struct DotrShSlotTexVertex v0, v1, v2, v3;
    unsigned long long texture[4];
};

/* ShSlot_DrawReel's shaded (Gouraud) rectangle packet. */
struct DotrShSlotGradPacket {
    unsigned long long tag;
    unsigned long long header[2];
    struct DotrShSlotColorVertex v0, v1, v2, v3;
};

/* ShSlot_DrawReel's stack work: the four symbol images, the background texture and the
 * two packets. */
struct DotrShSlotReelWork {
    DotrShSlotWord images[4];
    int *background;
    struct DotrShSlotGradPacket gradient;
    struct DotrShSlotTexPacket textured;
};

/* Data views, linked by address. Scalars that lie inside the structures above are kept
 * as the separate symbols the C uses (0x351241 is reel 0 byte 1, 0x351252 reel 0's
 * position, 0x351258 and 0x351292 the centre symbols of reels 0 and 1). */
extern struct DotrShSlotBoard data_00351210_shslot;
extern unsigned char MiddleStraightLineHighlight_shslot;
extern unsigned char TopStraightLineHighlight_shslot;
extern unsigned char BottomStraightLineHighlight_shslot;
extern unsigned char TopLeftDiagonalHighlight_shslot;
extern unsigned char BottomLeftDiagonalLineHighlight_shslot;
extern DotrShSlotWord data_00351228_shslot; /* the packet cursor while drawing */
extern struct DotrShSlotDeck *data_00351230_shslot;
/* The same pointer read as a byte pointer (*p is the deck's count). */
extern unsigned char *data_00351230_shslot_bytes;
extern struct DotrShSlotReel data_00351240_shslot[];
extern unsigned char data_00351241_shslot; /* lines in play, bits 0..4 */
extern unsigned char CanStillGet3InARow_shslot; /* lines still winning, bits 0..4 */
extern unsigned char data_00351244_shslot; /* demo mode (ShSlot_Init's argument) */
extern unsigned short data_00351246_shslot; /* frame counter */
extern short data_00351248_shslot; /* reel length: count * 0x70 */
extern short SquareSlotReelImageOffset_shslot;
extern unsigned short SquareSlotCardID_shslot;
extern unsigned short data_00351292_shslot;
extern struct DotrShSlotFlash data_00351180_shslot[];
extern unsigned char data_00351178_shslot; /* hit-request phase */
extern unsigned char VisualDelayTimer_shslot; /* hit-request wait */
extern unsigned char nextSlotDropNumber_shslot; /* hit-request step 0..8 */
extern unsigned short nextSlotDrop_shslot; /* the card of the current hit request */

#ifdef __MWERKS__
#define DOTR_SHSLOT_DEMO_DECK ((struct DotrShSlotDeck *)0x01ED0000)
#else
extern struct DotrShSlotDeck dotr_shslot_demo_deck;
#define DOTR_SHSLOT_DEMO_DECK (&dotr_shslot_demo_deck)
#endif

/* Read-only tables. 0x2b8780: the row offset per line for the fever draw (5 words);
 * 0x2b8760: six floats ShSlot_Flear copies and does not use; 0x2b8740: the grey level
 * per cell timer value; 0x2b8e40: a 17-word pulse for the eye. */
extern int SlotTable_shslot[];
struct DotrShSlotFloats {
    float values[6];
};
extern struct DotrShSlotFloats data_002b8760_shslot;
extern unsigned char gSh_Lamp_shslot[];
struct DotrShSlotPulse {
    int values[17];
};
extern struct DotrShSlotPulse data_002b8e40_shslot;

/* Callees (names from the Japanese build's symbols). Assumed:
 * func_001a6898 / func_001a6a88 are sine / cosine of radians; func_001a22c0 and func_001a23b0
 * convert a float to an int; func_0019a5c8 returns the next pseudo-random number.
 * TaSlot_GetFeverGain is called with a second (line) argument here; side_table.h declares one
 * parameter. SzText_GetReadyItem's result is held in an int-sized word. ShFx_initTimer takes int
 * parameters here (low_leaves.h declares char parameters): 0xff passes as
 * 255. */
extern int ShFx_isTimer(void *timer);
extern void ShFx_initTimer(void *timer, int a, int b, int c);
extern int ExAssignAlpha(void *packet, long long value);
extern int ExAssignClamp(void *packet, long long value);
extern int ExAssignTexture(void *packet, void *texture, int a);
extern DotrShSlotWord ExSortPrim_Pipe(DotrShSlotWord cursor, void *packet, int a, int depth);
extern float func_001a6898(float x);
extern float func_001a6a88(float x);
extern int func_001a22c0(float x);
extern int func_001a23b0(float x);
extern int func_0019a5c8(void);
extern int SzSound_PlaySE(int a, int soundEffectNumber);
extern int SzSound_StopSE(int a, int b);
extern int TaSlot_GetFeverGain(unsigned short card, unsigned char line);
extern DotrShSlotWord SzText_LoadRequest(int group, int id);
extern int RhBase_GetUserGsAddress(void);
extern int SzText_InitClutManage(int base, int size);
extern DotrShSlotWord SzText_DoRequest_Pipe(DotrShSlotWord cursor, int a, int b, int *out);
extern DotrShSlotWord RhText_SetTextureFlush(DotrShSlotWord cursor);
extern int SzText_FreeRequest(int textureFlags, int textureId);
extern DotrShSlotWord SzText_GetReadyItem(int kind, int card);
extern DotrShSlotWord SzText_Load_Pipe(DotrShSlotWord cursor, int image, int base, int *size);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern void SzDuel_CloseMsgPopup(void);
extern void SzGame_Pop(void);
extern int SzPad_Check(int pad, int kind, int mask);
extern int SzPad_IsAnalog(int pad);
extern int SzPad_GetAnalogInfo(int pad, int which);
extern void ShSlot_DrawParts(int part);

/* 0x280060 ShSlot_Flear: draws count flash objects as triangle fans; returns 1. */
int ShSlot_Flear(void *objects, int count);
/* 0x280670 ShSlot_Start: starts all three reels and the three start flashes. */
void ShSlot_Start(void);
/* 0x280920 ShSlot_HitCardRequest: finds the next hit card (nextSlotDrop). */
void ShSlot_HitCardRequest(void);
/* 0x280cb0 ShSlot_CheckLine: after reel `reel` stops, clears the lines it breaks. */
void ShSlot_CheckLine(int reel);
/* 0x2817c0 ShSlot_IsFlashStop: 1 when no line or cell is still flashing. */
int ShSlot_IsFlashStop(void);
/* 0x2825f0 ShSlot_DrawEye: draws the button marks over reels not yet stopped. */
void ShSlot_DrawEye(void);
/* 0x2831a0 ShSlot_FlashBox: draws the flashing frames of the cells. */
void ShSlot_FlashBox(void);
/* 0x283880 ShSlot_DrawReel: draws one reel's background, symbols and shading. */
void ShSlot_DrawReel(int reel);
/* 0x284d30 ShSlot_Draw_Pipe: moves the reels and draws the machine; returns the cursor. */
DotrShSlotWord ShSlot_Draw_Pipe(DotrShSlotWord cursor);
/* 0x285880 ShSlot_Init: sets up the reels (demo strips when demo is non-zero). */
void ShSlot_Init(int demo);
/* 0x285f20 ShSlot_Act: the per-frame input and phase step. */
void ShSlot_Act(void);

#endif
