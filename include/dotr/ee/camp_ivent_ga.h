#ifndef DOTR_EE_CAMP_IVENT_GA_H
#define DOTR_EE_CAMP_IVENT_GA_H

/* The campaign map's event screen overlays Taku_Camp_Ivent_Ga: fade frames, bars, lamps and page
 * pieces for the current camp event, drawn from the camp work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Taku_Camp_Ivent_Ga reads the camp work area at the literal address 0x1ed0000 and the camp state
 * at 0x299e10 as bytes (the event number at +0xc2, also read as the separate symbol 0x299ed2).
 * MATCHING DEVICE: under the target compiler the two camp-state symbols are separate externs and
 * AT(a) is the literal address; on the host both symbols name one fixture and AT(a) indexes a
 * fixture of the work area. Assumed: these meanings, from use. */
#ifdef __MWERKS__
extern signed char data_00299e10_iventga;
extern short CutSceneNumber_iventga;
#define DOTR_IVENT_GA_AT(a) (a)
#else
#include <stdint.h>
union DotrIventGaBoard {
    signed char bytes[0x100];
    short align;
};
extern union DotrIventGaBoard dotr_camp_ivent_ga_board;
#define data_00299e10_iventga (dotr_camp_ivent_ga_board.bytes[0])
#define CutSceneNumber_iventga (*(short *)(dotr_camp_ivent_ga_board.bytes + 0xc2))
union DotrIventGaWork {
    unsigned char bytes[0x5500];
    double align;
};
extern union DotrIventGaWork dotr_camp_ivent_ga_work;
#define DOTR_IVENT_GA_AT(a) ((intptr_t)dotr_camp_ivent_ga_work.bytes + ((a) - 0x01ED0000))
#endif

/* Views: a primitive template (eight coordinates, four colour
 * quads, a texture code), groups of four and five, texture-table entries (tex_uv_data, 0x2950a0)
 * and a size pair; templates at 0x29abf0..0x29aea8. Assumed: names from use. */
typedef struct DotrIventGaObj {
    short w[8];
    unsigned char col[4][4];
    short pad20[2];
    unsigned short code;
} DotrIventGaObj;

typedef struct DotrIventGaObj4 {
    DotrIventGaObj o[4];
} DotrIventGaObj4;

typedef struct DotrIventGaObj5 {
    DotrIventGaObj o[5];
} DotrIventGaObj5;

typedef struct DotrIventGaTex {
    short f[10];
} DotrIventGaTex;

typedef struct DotrIventGaPair {
    short x;
    short y;
} DotrIventGaPair;

extern DotrIventGaTex tex_uv_data[];
extern DotrIventGaObj data_0029abf0_iventga;
extern DotrIventGaObj4 data_0029ac20_iventga;
extern DotrIventGaObj4 data_0029acc0_iventga;
extern DotrIventGaObj5 data_0029ad60_iventga;
extern DotrIventGaObj data_0029ae20_iventga;
extern DotrIventGaObj data_0029ae50_iventga;
extern DotrIventGaObj data_0029ae80_iventga;
extern DotrIventGaPair data_0029aea8_iventga;

/* Callees Taku_G4_Set, Taku_GT4_Set and Taku_Obj_Set as views of this caller (quad_offsets.h and
 * small_state.h declare them with other views). Cursors are 32-bit words (int). */
extern int Taku_G4_Set(int cursor, void *src, short x, short y);
extern int Taku_GT4_Set(int cursor, void *src, short x, short y, short dx, short dy);
extern int Taku_Obj_Set(int cursor, void *src, short dx, short dy);

static inline int dotr_camp_ivent_ga_obj(int cursor, void *src, int x, int y) {
    return Taku_Obj_Set(cursor, src, x, y);
}

/* 0x22f3c0: draws the current event's overlays; returns the cursor. */
int Taku_Camp_Ivent_Ga(int cursor);

#endif
