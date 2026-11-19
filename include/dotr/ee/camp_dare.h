#ifndef DOTR_EE_CAMP_DARE_H
#define DOTR_EE_CAMP_DARE_H

/* Campaign-map drawers of the two sliding opponent name panels (first member Taku_Camp_BUN_DARE at
 * 0x2337a0): they share the slide counters and face requests in the literal-addressed block at
 * 0x1ed53b6-0x1ed546a, the Taku_*_Set packet builders and the message slot table.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#ifdef __MWERKS__
typedef int DotrDareWord;
#else
#include <stdint.h>
typedef intptr_t DotrDareWord;
#endif

/* Views assumed from the access widths and strides, as in camp_tikei.h: 38-byte drawing
 * templates (tex_bara_data2 and camp_window_teki3 are indexed by a signed byte), 10-byte
 * window records whose first two halfwords are read unsigned (entry 3 of
 * camp_window_Teki_data), a four-byte row per name number at 0x2999cc, the fourth word
 * of a message slot entry (its width) and an eight-byte face request (small_misc.h has
 * ShFace_Request with its own view of the same request; ShFace_Close takes the same
 * address). Member names are assumed from the stores. */
struct DotrDareItem {
    short w[8];
    short unknown_10[10];
    short code;
};
struct DotrDareWindow {
    unsigned short x, y, w, h;
    char unknown_08[2];
};
struct DotrDareInfo {
    int unknown_00[3];
    int w;
};
struct DotrDareFace {
    unsigned char id, no, kind, mode;
    unsigned short x, y;
};
extern struct DotrDareWindow camp_window_Teki_data[];
extern struct DotrDareItem tex_bara_data2[];
extern struct DotrDareItem camp_window_teki3[];
extern unsigned char data_002999cc[][4];

/* The block near 0x1ed53b6 is addressed with literal addresses (`lui at` per access,
 * `lui`/`ori` for the two request pointers), so the target build uses integer constants;
 * the host build points the same names at objects the test defines. Per side (0 left,
 * 1 right): slide counter, name number, side byte, face state, face request, look byte. */
#ifdef __MWERKS__
#define DOTR_DARE_SLIDE0 (*(short *)0x1ed53b6)
#define DOTR_DARE_SLIDE1 (*(short *)0x1ed53b8)
#define DOTR_DARE_NAME0 (*(short *)0x1ed53ba)
#define DOTR_DARE_NAME1 (*(short *)0x1ed53bc)
#define DOTR_DARE_SIDE0 (*(signed char *)0x1ed53be)
#define DOTR_DARE_SIDE1 (*(signed char *)0x1ed53bf)
#define DOTR_DARE_STATE0 (*(signed char *)0x1ed5407)
#define DOTR_DARE_STATE1 (*(signed char *)0x1ed5408)
#define DOTR_DARE_FACE0 ((struct DotrDareFace *)0x1ed540c)
#define DOTR_DARE_FACE1 ((struct DotrDareFace *)0x1ed5414)
#define DOTR_DARE_LOOK1 (*(signed char *)0x1ed5469)
#define DOTR_DARE_LOOK0 (*(signed char *)0x1ed546a)
#else
extern short dotr_dare_slide_01ed53b6[2];
extern short dotr_dare_name_01ed53ba[2];
extern signed char dotr_dare_side_01ed53be[2];
extern signed char dotr_dare_state_01ed5407[2];
extern struct DotrDareFace dotr_dare_face_01ed540c[2];
extern signed char dotr_dare_look_01ed5469[2];
#define DOTR_DARE_SLIDE0 dotr_dare_slide_01ed53b6[0]
#define DOTR_DARE_SLIDE1 dotr_dare_slide_01ed53b6[1]
#define DOTR_DARE_NAME0 dotr_dare_name_01ed53ba[0]
#define DOTR_DARE_NAME1 dotr_dare_name_01ed53ba[1]
#define DOTR_DARE_SIDE0 dotr_dare_side_01ed53be[0]
#define DOTR_DARE_SIDE1 dotr_dare_side_01ed53be[1]
#define DOTR_DARE_STATE0 dotr_dare_state_01ed5407[0]
#define DOTR_DARE_STATE1 dotr_dare_state_01ed5407[1]
#define DOTR_DARE_FACE0 (&dotr_dare_face_01ed540c[0])
#define DOTR_DARE_FACE1 (&dotr_dare_face_01ed540c[1])
#define DOTR_DARE_LOOK1 dotr_dare_look_01ed5469[0]
#define DOTR_DARE_LOOK0 dotr_dare_look_01ed5469[1]
#endif

/* Callees, assumed prototypes from this call site: Taku_Window_Set (0x2195e0, taku_window),
 * SzMsg_LoadFont_Pipe (0x1fd680, slot_table) and SzMsg_DrawRatioX_Pipe (0x1fe660,
 * cursor_packets). Taku_Window_Set takes a ninth argument on the stack; its sixth is
 * narrowed to 16 bits at the two calls for window record 3 (camp_tikei.h has it as int).
 * SzMsg_DrawRect_Pipe (0x1fdfe0) is passed by address only. */
extern DotrDareWord Taku_Window_Set(DotrDareWord cursor, struct DotrDareWindow *window,
                                    unsigned short flag, int a3, unsigned short x, int y,
                                    int a6, int a7, int a8);
extern DotrDareWord Taku_Obj_Set(DotrDareWord cursor, struct DotrDareItem *item, short x,
                                 short y);
extern DotrDareWord Taku_FT4_Set(DotrDareWord cursor, struct DotrDareItem *item, short x,
                                 short y, int a4);
extern DotrDareWord SzMsg_LoadFont_Pipe(DotrDareWord cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern DotrDareWord RhText_SetTextureFlush(DotrDareWord cursor);
extern struct DotrDareInfo *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern DotrDareWord SzMsg_DrawRatioX_Pipe(DotrDareWord cursor, int index, int x, int y,
                                          float sx, float sy, void *e,
                                          void (*function)(void));
extern int ShFace_Request(struct DotrDareFace *req);
extern void ShFace_Close(struct DotrDareFace *req);

DotrDareWord Taku_Camp_BUN_DARE(DotrDareWord cursor);

#endif
