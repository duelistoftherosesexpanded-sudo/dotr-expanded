#ifndef DOTR_EE_MAP_EDIT_GRAPH_H
#define DOTR_EE_MAP_EDIT_GRAPH_H

/* The map-edit screen's field-count graph: one bar per field kind drawn from the map cells,
 * with its icon, frame and count.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A cursor or packet address kept as an int on the target (the packet's address is passed as
 * an int); widened on the host so that the address survives. */
#ifdef __MWERKS__
typedef int DotrMapGraphWord;
#else
#include <stdint.h>
typedef intptr_t DotrMapGraphWord;
#endif

/* The map-edit cursor bytes at 0xa9c08f (column) and 0xa9c090 (row), read with literal
 * addresses (map_edit.h has the same pair). MATCHING DEVICE: the target build reads them in
 * the `*(unsigned char *)address` form; the host build reads a host array. */
#ifdef __MWERKS__
#define DOTR_MAPGRAPH_CURSOR_X (*(unsigned char *)0xA9C08F)
#define DOTR_MAPGRAPH_CURSOR_Y (*(unsigned char *)0xA9C090)
#else
extern unsigned char dotr_mapgraph_cursor[2];
#define DOTR_MAPGRAPH_CURSOR_X (dotr_mapgraph_cursor[0])
#define DOTR_MAPGRAPH_CURSOR_Y (dotr_mapgraph_cursor[1])
#endif

/* The 16-byte GIF tag at the start of the packet (count, flags, a padding word, the register
 * list). */
struct DotrMapGraphTag {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long registers;
};
/* One gouraud vertex: RGBA bytes and q, then the 64-bit xyz word (16-bit x and y, 24-bit z,
 * fog byte). */
struct DotrMapGraphColor {
    unsigned char r, g, b, a;
    float q;
};
struct DotrMapGraphXyz {
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long fog : 8;
};
struct DotrMapGraphPoint {
    struct DotrMapGraphColor color;
    struct DotrMapGraphXyz xyz;
};
/* The 88-byte packet local: tag, four points and a trailing word that is not written. */
struct DotrMapGraphPacket {
    unsigned long tag[2];
    struct DotrMapGraphPoint points[4];
    unsigned long end;
};
/* Colour entries at 0x2be8c0, stored b, g, r, a; three per field kind (top, middle, bottom of
 * the two-band bar). Assumed: the meaning of the three entries. */
struct DotrMapGraphSource {
    unsigned char b, g, r, a;
};

/* 0x1ccea0 lSzDuel_DrawMapEdit_Graph_Pipe (original name). Assumed: the duel control block at
 * 0x317490 read as bytes (the picture depth word at +0xc58); the icon definitions at 0x290080
 * as 10-byte rows (rows 270..279 are the ten field kinds); the colour entries at 0x2be8c0;
 * SzDuel_GetField returning the field kind (0..9) of a cell of the 7 x 7 map. Callee
 * signatures as this caller uses them (the frame drawer's last argument is a 64-bit word). */
extern unsigned char data_00317490_mapgraph[];
extern unsigned char gSzDuel_Icon_mapgraph[];
extern unsigned char gSzDuel_aaicolField_mapgraph[];
extern int SzDuel_GetField(int x, int y);
extern DotrMapGraphWord lSzDuel_DrawWindow_Frame_Pipe(DotrMapGraphWord cursor, int a1, int a2, int a3, int x, int y,
                                                      int w, int h, long a8);
extern DotrMapGraphWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrMapGraphWord cursor, void *def, int x, int y, int a4,
                                                       int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                                                       int a12, int a13);
extern DotrMapGraphWord ExSortPrim_Pipe(DotrMapGraphWord cursor, DotrMapGraphWord packet, int a2, int a3);
extern DotrMapGraphWord SzDuel_DrawWindow_FontDigit_Pipe(DotrMapGraphWord cursor, int x, int y, int a, int flags,
                                                         int width, int value);
DotrMapGraphWord lSzDuel_DrawMapEdit_Graph_Pipe(DotrMapGraphWord cursor, int x, int y);

#endif
