#ifndef DOTR_EE_CAMP_IVENT_NAME_H
#define DOTR_EE_CAMP_IVENT_NAME_H

/* The campaign map's event name-entry screen Taku_Camp_Ivent_Name, which draws the name window,
 * the character grid, the cursor and the entered name from the camp work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Taku_Camp_Ivent_Name reads the camp work area at the literal address 0x1ed0000 through a state
 * view. The target build keeps the literal; the host build points it at a test object. Assumed:
 * these meanings, from use. */
#ifdef __MWERKS__
#define DOTR_IVENT_NAME_AT(a) (a)
#else
#include <stdint.h>
union DotrIventNameWork {
    unsigned char bytes[0x5500];
    double align;
};
extern union DotrIventNameWork dotr_camp_ivent_name_work;
#define DOTR_IVENT_NAME_AT(a) ((intptr_t)dotr_camp_ivent_name_work.bytes + ((a) - 0x01ED0000))
#endif

/* Views: sprite templates (position, colour and alpha bytes),
 * the texture rectangle data, palette and image records at 0x29a7e0..0x29a9e0, the character
 * table at 0x2999a0, the window title text at 0x30f0b0 and the work-area state (mode, slide,
 * cursor, selection, scroll and blink bytes from 0x1ed53a6, three twelve-entry name lists from
 * 0x1ed53cf). Assumed: names from use. */
typedef struct {
    short x, y;
    unsigned char pad04[12];
    unsigned char r, g, b, a;
    unsigned char pad14[3], alpha1;
    unsigned char pad18[3], alpha2;
    unsigned char pad1C[3], alpha3;
    unsigned char pad20[6];
} DotrIventNameSprite;
typedef struct { int value[4]; } DotrIventNameUv;
typedef struct { short rgb[10][3]; } DotrIventNamePalette;
typedef struct { DotrIventNameSprite item[6]; } DotrIventNameSix;
typedef struct { DotrIventNameSprite item[3]; } DotrIventNameThree;
typedef struct { short u, v, w, h, page; } DotrIventNameImage;
typedef struct { int x, y, w, h; } DotrIventNameRect;
typedef struct { int flags; DotrIventNameRect rect; } DotrIventNameTexture;

typedef struct {
    unsigned char pad0[0x53A6];
    signed char mode;
    signed char slide;
    unsigned char pad53A8[0x1B];
    signed char cursorX;
    signed char cursorY;
    signed char selCol;
    signed char selRow;
    signed char order;
    signed char scrolling;
    signed char scroll;
    signed char selected;
    signed char selMove;
    signed char blink;
    signed char count;
    unsigned char pad53CE;
    signed char listKind[12];
    signed char listCell[12];
    signed char listRow[12];
} DotrIventNameState;

extern DotrIventNameUv data_0029a7e0_iventname;
extern DotrIventNamePalette data_0029a7f0_iventname;
extern DotrIventNameImage data_0029a830_iventname;
extern DotrIventNameSix data_0029a840_iventname;
extern DotrIventNameSprite data_0029a930_iventname;
extern DotrIventNameThree data_0029a960_iventname;
extern DotrIventNameSprite data_0029a9e0_iventname;
extern short data_002999a0_iventname[][3];
extern signed char data_0030f0b0_iventname[];

/* Callees with file-scope prototypes. The others are declared inside the function body.
 * quad_offsets.h and taku_window.h declare Taku_FT4_Set and Taku_Window_Set with other views.
 * Cursors are 32-bit words (int). */
extern int SzMsg_LoadFont_Pipe(int arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4);
extern void SzMsg_Free(int arg0);
extern int RhText_SetTextureFlush(int arg0);
extern void *Taku_FT4_Set(void *packet, void *src, short x, short y, unsigned short flip);
extern int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx,
                           unsigned short dy, unsigned short depth, int unused, short alpha);

/* 0x223600: draws the name-entry screen; returns the cursor. */
int Taku_Camp_Ivent_Name(int packet);

#endif
