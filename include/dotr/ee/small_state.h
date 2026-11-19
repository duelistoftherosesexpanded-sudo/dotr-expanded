#ifndef DOTR_EE_SMALL_STATE_H
#define DOTR_EE_SMALL_STATE_H

/* Miscellaneous tiny routines from three address ranges (0x1d9ed0-0x1d9ef0,
 * 0x2031f0-0x207500, 0x2173d0-0x217cb0), grouped for convenience only. Views are offset
 * hypotheses. Evidence: docs/modules/engine.md and the functions' other module entries. */
#ifdef __MWERKS__
typedef int DotrSmallWord;
#else
#include <stdint.h>
typedef intptr_t DotrSmallWord;
#endif

struct DotrSmallBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bit_4 : 1;
    signed char unknown_bit_5 : 1;
    signed char unknown_bit_6 : 1;
    signed char unknown_bit_7 : 1;
};

/* 92-byte entries at +0x200 of the object at EE 0x3259c0. */
struct DotrSmallEntry {
    unsigned short unknown_00;         /* three halfwords rotated by 0x203840 */
    unsigned short unknown_02;
    unsigned short unknown_04;
    unsigned char unknown_06[4];
    unsigned char unknown_0a[12];      /* 0x2038f0 copies twelve bytes here */
    unsigned char unknown_16[2];
    int unknown_18[16];                /* words selected by a mask in 0x203b50 */
    unsigned char unknown_58;
    signed char unknown_59;            /* stored by 0x2032c0; read signed by 0x203b50 */
    struct DotrSmallNibbles {
        signed char unknown_bits_0_3 : 4; /* replaced by 0x2032c0 */
        signed char unknown_bits_4_7 : 4;
    } unknown_5a;
    struct DotrSmallEntryBits {
        signed char unknown_bits_0_5 : 6; /* compared with 5 by 0x203840 */
        signed char unknown_bit_6 : 1;
        signed char unknown_bit_7 : 1;
    } unknown_5b;
};

struct DotrSmallTable {
    unsigned char unknown_000[0x200];
    struct DotrSmallEntry unknown_200[2]; /* 0x2031f0 and 0x203840 use two */
    unsigned char unknown_2b8[8];         /* 0x2031f0 clears 704 bytes in all */
};

typedef char DotrSmallEntryMustBe5c[(sizeof(struct DotrSmallEntry) == 0x5c) ? 1 : -1];

extern struct DotrSmallTable gSzPad_pl;
extern int gSzDebug_dl[];
extern struct DotrSmallBits data_00325cac[];
/* Same storage as gSzDebug_dl[] and data_00325cac[]: the flag byte read by
 * 0x205170 is addressed as a member at +0x2c of an object at 0x325c80. The
 * eleven leading words are filler from the int-array declaration. */
struct DotrSmallFlagsOwner {
    int unknown_00[11];
    struct DotrSmallBits unknown_2c;
};
extern struct DotrSmallFlagsOwner data_00325c80_owner;
int SzDebug_IsHide(void);
extern int gEx3d_aPlane[];
extern int gMyOT[];
extern struct DotrSmallBits data_0034f6e8[];
/* Words inside the control view at 0x317490 (see state_records.h). */
extern int data_003180e4[];
extern int data_0031810c[];
/* Address-only symbol used in a size computation. */
extern char data_01fe0000[];

void SzDuel_SetCardDefPrioZ(int value);
void SzDuel_SetWindowDefPrioZ(int value);
int SzDuel_GetUserGsAddress(void);
void SzPad_Init(void);
int *SzPad_Term(void);
void SzPad_Act(void);
void SzPad_SetParam(int index, int value, int nibble);
int SzPad_ResetCounter(int index, unsigned short mask);
/* 0x203a40: mode 0, 1 or 2 selects which of the entry's halfwords and counters answer the mask;
 * calls itself with mode 0. Other groups declare it with the same three ints. */
int SzPad_Check(int index, int mode, int mask);
/* 0x192bb8 / 0x192b38: routines of the SDK library block. Assumed from 0x2038f0:
 * (entry index, 0) answering a state, and (entry index, 0, buffer) answering non-zero
 * after filling at least 20 bytes. */
extern int func_00192bb8(int port, int slot);
extern int func_00192b38(int port, int slot, unsigned char *out);
/* 0x203320 and five more routines of the SDK library block. Assumed from 0x203320 only:
 * (entry index, 0, term, offset) and (entry index, 0, offset, 3) answering an int, and
 * (entry index, 0) answering a state. No argument or result of SzPad_Status. */
extern int func_00192f98(int port, int slot, int term, int offs);
extern int func_001930d0(int port, int slot, int offs, int lock);
extern int func_00192cd0(int port, int slot);
extern int func_00193488(int port, int slot);
extern int func_001934e8(int port, int slot);
/* One pad entry as SzPad_Status views it (the layout of
 * DotrSmallEntry): three button halfwords, four analog bytes, twelve bytes, sixteen press
 * counters, the mode id at +0x58, the counter base at +0x59, the mode count in the high nibble
 * of +0x5a and the phase, pressed and ready bits at +0x5b. Assumed: the meanings. */
struct DotrSmallPadEntry {
    unsigned short buttons[3];
    unsigned char act[4];
    unsigned char data[12];
    unsigned char pad16[2];
    int press[16];
    signed char mode;
    signed char base;
    signed char low : 4;
    signed char count : 4;
    signed char state : 6;
    signed char pressed : 1;
    signed char ready : 1;
};
void SzPad_Status(void);
/* 0x203cd0 and its storage. data_002d078c is read as one unsigned byte; the three
 * data_002d07f* objects are passed as strings (a format, a label for SzFile_GetEnv and a
 * name), contents not described here. func_0019b648 and func_00197548 are library
 * routines; assumed prototypes as in resource_boot.h. */
extern unsigned char data_002d078c;
extern char data_002d07f4[];
extern char data_002d07fa[];
extern char data_002d07ff[];
extern char *SzFile_GetEnv(const char *label);
extern int func_0019b648(char *destination, const char *format, ...);
extern char *func_00197548(const char *label);
void SzDebug_Init(void);
/* 0x2038f0: always answers 1. */
int SzPad_VSync(void);
int SzPad_IsAnalog(int index);
int SzPad_IsPress(int index);
int SzPad_GetAnalogInfo(int index, int slot);
void SzPad_SetAnalogInfo(int index, int slot, int value);
int SzPad_GetPressInfo(int index, int slot);
int SzDebug_GetActiveMenu(void);
int SzDebug_GetActiveModel(void);
void SzDebug_SetHideFlag(int value);
int SzDebug_IsEffect(void);
int Ex3d_ViewClip(int a, int b, int c, int d, int e);
void ExInitPrim(void);
void ExTermPrim(void);
int *ExGetMyOT(void);
int RhMemCard_Init(void);
int RhMemCard_Term(void);
int RhMemCard_Act(void);
int SzSound_IsStandby(void);
void SzSound_VSync(void);
int SzSound_PlayBGM(void);
void SzMem_Init(void);

/* Dependencies: 0x20b400 is in memory_ops, 0x109dc0 and 0x109e80 in
 * sound_control; 0x20baf0 and 0x20bb50 are VU0 coprocessor routines. The
 * other prototypes are assumed from the call sites. */
/* 0x20b3c0 is the byte fill. 0x192588 and 0x1927e8 are library routines;
 * only the argument registers set by 0x2031f0 are known. */
extern void RhMem_FillMemory(void *destination, int value, int size);
extern void func_00192588(int unknown);
extern void func_001927e8(int a, int b, void *buffer);
extern int *RhMem_FillMemory4(int *destination, int value, int count);
extern int Ex3d_Clipping(int a, int *b, int c, int d, int e, int f, int g);
extern void RhGeom_PushMatrix(int unknown);
extern void RhGeom_PopMatrix(void);
extern void func_00193808(void);
extern void SD_CallEveryVSync(void);
extern void SD_BGMPreLoad(void);
extern DotrSmallWord func_00197bc0(DotrSmallWord size);
extern void func_00197c18(DotrSmallWord value);

/* Views for 0x217bd0 and 0x217cb0, which forward a rectangle, four rows
 * of the 20-byte table at 0x2950a0 and bytes of an item to Taku_Prim_Set_SP1 (seventeen
 * arguments, nine on the stack). Field names follow how the two callers
 * combine them; meanings and the callee's parameter types are assumed. */
struct DotrSmallSheetRow {
    short x;
    short y;
    short width;
    short height;
    unsigned char unknown_08[8];
    unsigned short unknown_10;
    unsigned char unknown_12[2];
};
struct DotrSmallSheetItem {
    short x0;
    short y0;
    short x1;
    short y1;
    unsigned char unknown_08[8];
    unsigned char color[4];
    unsigned char unknown_14[12];
    unsigned char unknown_20;
    unsigned char unknown_21;
    short unknown_22;
    unsigned short row;
};
extern struct DotrSmallSheetRow tex_uv_data[];
extern void Taku_Prim_Set_SP1(int a0, short x0, short y0, short x1, short y1, int u, int v, int width, int height,
                          int c0, int c1, int c2, int c3, int unknown_20, int unknown_10, int unknown_21,
                          int unknown_22);
void Taku_Obj_Set(int a0, struct DotrSmallSheetItem *item, short x, short y);
void Taku_Te_Set(int a0, struct DotrSmallSheetItem *item, short x, short y);

#endif
