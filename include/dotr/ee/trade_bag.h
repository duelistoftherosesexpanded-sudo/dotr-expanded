#ifndef DOTR_EE_TRADE_BAG_H
#define DOTR_EE_TRADE_BAG_H

/* The trade screen's bag panel drawer lSzDuel_DrawTrade_Bag_Pipe, which draws one side's card list,
 * counters, sort tabs and page numbers from the duel control block at 0x317490.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrTradeBagWord;
#else
#include <stdint.h>
typedef intptr_t DotrTradeBagWord;
#endif

/* The duel control block at 0x317490 as bytes: the two sides' card-list pointers at +0x6d8/+0x6dc
 * (halfword lists) and count-array pointers at +0x6e0/+0x6e4 (854 bytes each), the packet depth
 * word at +0xc58, the list tops +0xc8a / +0xc8e, the list lengths +0xc8c / +0xc90, the sort bytes
 * from +0xcb6 / +0xcb9, the selected label byte per side from +0xcc0 and the cursor rows +0xd5e /
 * +0xd5f. MATCHING DEVICE: on the target the pointers are words of the byte block; on the host
 * (wider pointers) DOTR_TRADE_BAG_PTR reads them from a separate fixture indexed by the same
 * offsets. Assumed: these meanings, from this function's use. */
#ifdef __MWERKS__
extern unsigned char data_00317490_tradebag[];
#define DOTR_TRADE_BAG_PTR(type, data, offset) (*(type *)((data) + (offset)))
#else
union DotrTradeBagState {
    unsigned char b[0xd70];
    long long align;
};
extern union DotrTradeBagState dotr_trade_bag_state;
extern void *dotr_trade_bag_ptrs[4]; /* +0x6d8, +0x6dc, +0x6e0, +0x6e4 */
#define data_00317490_tradebag (dotr_trade_bag_state.b)
#define DOTR_TRADE_BAG_PTR(type, data, offset) ((void)(data), (type)dotr_trade_bag_ptrs[((offset) - 0x6D8) / 4])
#endif

/* The GS packet this function sorts in (ExSortPrim_Pipe): a header of type 3, count 6 and
 * register word 0x441, a colour, q and two 64-bit XYZF vertices. The work block holds a copy
 * of the three label pointers at 0x291490 and the packet. Assumed: the field meanings. */
struct DotrTradeBagHeader {
    unsigned short type;
    unsigned short count;
    unsigned int pad;
    unsigned long long registers;
};
struct DotrTradeBagVertex {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
};
struct DotrTradeBagPacket {
    struct DotrTradeBagHeader header;
    unsigned char r, g, b, a;
    float q;
    struct DotrTradeBagVertex first;
    struct DotrTradeBagVertex second;
};
struct DotrTradeBagLabels {
    signed char *text[3];
};
struct DotrTradeBagWork {
    struct DotrTradeBagLabels labels;
    struct DotrTradeBagPacket packet;
};

/* Data (each symbol links by address): icon
 * definitions (10 bytes each) in the table at 0x290080 and eleven single entries in it, the ten
 * sort names at 0x290fd0, the three label pointers at 0x291490, the colour bytes 0x2be8a4 and
 * 0x2be944, the title strings 0x2bf021 (side 1) and 0x2bf024 (side 0; +3 the subtitle), the
 * description 0x2bef98 and the per-side bytes at 0x31814e (1: the side may pick a sort with
 * button 0x100). MATCHING DEVICE: the single icon entries are separate symbols on the
 * target; on the host they point into one fixture of the table. */
#ifdef __MWERKS__
extern unsigned char gSzDuel_Icon_tradebag[];
extern unsigned char data_00290828_tradebag[];
extern unsigned char data_00290832_tradebag[];
extern unsigned char data_0029086e_tradebag[];
extern unsigned char data_00290896_tradebag[];
extern unsigned char data_002908a0_tradebag[];
extern unsigned char data_00290a6c_tradebag[];
extern unsigned char data_00290aa8_tradebag[];
extern unsigned char data_00290ab2_tradebag[];
extern unsigned char data_00290abc_tradebag[];
extern unsigned char data_00290ac6_tradebag[];
#else
extern unsigned char gSzDuel_Icon_tradebag[0xa50];
#define data_00290828_tradebag (gSzDuel_Icon_tradebag + 0x7a8)
#define data_00290832_tradebag (gSzDuel_Icon_tradebag + 0x7b2)
#define data_0029086e_tradebag (gSzDuel_Icon_tradebag + 0x7ee)
#define data_00290896_tradebag (gSzDuel_Icon_tradebag + 0x816)
#define data_002908a0_tradebag (gSzDuel_Icon_tradebag + 0x820)
#define data_00290a6c_tradebag (gSzDuel_Icon_tradebag + 0x9ec)
#define data_00290aa8_tradebag (gSzDuel_Icon_tradebag + 0xa28)
#define data_00290ab2_tradebag (gSzDuel_Icon_tradebag + 0xa32)
#define data_00290abc_tradebag (gSzDuel_Icon_tradebag + 0xa3c)
#define data_00290ac6_tradebag (gSzDuel_Icon_tradebag + 0xa46)
#endif
extern signed char *data_00290fd0_tradebag[];
extern struct DotrTradeBagLabels data_00291490_tradebag;
extern unsigned char data_002be8a4_tradebag[];
extern char data_002be944_tradebag[];
extern char data_002bef98_tradebag[];
extern char data_002bf021_tradebag[];
extern char data_002bf024_tradebag[];
extern unsigned char data_0031814e_tradebag[];

/* Callees (views; they link by address). Assumed: these
 * types. 0x1b4780 lSzDuel_DrawWindow_Frame_Pipe takes its ninth argument as 64-bit (as
 * deck_list_pipe.h has it); 0x1c4b00 lSzDuel_DrawTrade_Card_Pipe is in trade_panes. */
extern int SzPad_Check(int port, int kind, int mask);
extern int func_0019bef0(const char *s);
extern DotrTradeBagWord func_001b4780_tradebag(DotrTradeBagWord cursor, int a1, int a2, int a3, int x, int y, int w,
                                               int h, long long a8);
extern DotrTradeBagWord func_001b3cc0_tradebag(DotrTradeBagWord cursor, unsigned char *src, int x, int y, int background,
                                               int image, int border, int shrink, int flip_x, int flip_y, int bright,
                                               unsigned char *rgb, int blend, unsigned char alpha);
extern DotrTradeBagWord func_001b65f0_tradebag(DotrTradeBagWord cursor, int colour, int x, int y, int w, int h,
                                               int shadow, int style);
extern DotrTradeBagWord func_001b6ea0_tradebag(DotrTradeBagWord cursor, int x, int y, int a, int flags, int width,
                                               int number);
extern DotrTradeBagWord func_001b6f70_tradebag(DotrTradeBagWord cursor, int x, int y, int font, int flags, int count,
                                               signed char *text, unsigned char *color);
extern DotrTradeBagWord func_001c4b00_tradebag(DotrTradeBagWord cursor, int side, int id, int x, int y, int a5);
extern DotrTradeBagWord func_00207650_tradebag(DotrTradeBagWord cursor, DotrTradeBagWord packet, int a2, int a3);
extern int SzData_GetBagCountAll(int side);

/* 0x1c3e20 lSzDuel_DrawTrade_Bag_Pipe (name from the Japanese build's symbol table): draws
 * side arg1's bag panel of the trade screen at (arg2, arg3), mirrored for side 1. Assumed:
 * four word parameters and the cursor result. */
DotrTradeBagWord lSzDuel_DrawTrade_Bag_Pipe(DotrTradeBagWord arg0, int arg1, int arg2, int arg3);

#endif
