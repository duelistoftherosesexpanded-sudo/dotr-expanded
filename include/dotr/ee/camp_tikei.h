#ifndef DOTR_EE_CAMP_TIKEI_H
#define DOTR_EE_CAMP_TIKEI_H

/* Taku_Camp_TIKEI_* terrain-window drawers near 0x236c50: they share the camp state at 0x299e10,
 * the literal-addressed block at 0x1ed0040 and the Taku_*_Set packet builders.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#ifdef __MWERKS__
typedef int DotrTikeiWord;
#else
#include <stdint.h>
typedef intptr_t DotrTikeiWord;
#endif

/* Views assumed from the access widths and strides: 38-byte drawing templates
 * (camp_window_tikei_partu, _gejiwaku, _iro_ue, _iro_sita in the Japanese build),
 * 52-byte model_data records whose short at +0x10 is compared with 8, and 10-byte
 * window records (camp_window_Map_data). 0x236f90 copies a template whole (two
 * 16-byte loads, a word, a halfword), writes the halfword at +36 and reads or writes
 * the first eight halfwords; the window record's first four halfwords are read. */
#ifndef DOTR_TIKEI_ITEM_DEFINED
#define DOTR_TIKEI_ITEM_DEFINED
struct DotrTikeiItem {
    short w[8];
    short unknown_10[10];
    short code;
};
#endif
struct DotrTikeiModel {
    char unknown_00[16];
    short kind;
    char unknown_12[34];
};
struct DotrTikeiWindow {
    short x, y, w, h;
    char unknown_08[2];
};
extern struct DotrTikeiItem camp_window_tikei_partu[];
extern struct DotrTikeiItem camp_window_tikei_partu_gejiwaku[];
extern struct DotrTikeiItem camp_window_tikei_iro_ue[];
extern struct DotrTikeiItem camp_window_tikei_iro_sita[];
extern struct DotrTikeiModel data_00298ba0_tikei[];
extern struct DotrTikeiWindow camp_window_Map_data[];

/* Camp state: byte 0 and byte 198 of the block at 0x299e10 are read through one
 * pointer; the short at 0x299ed4 (inside the same block) is read by its own address. */
extern signed char data_00299e10[];
extern short SelectedBattleArena[];

/* The 0x1ed0040 block is addressed with literal addresses (a `lui at` repeated in
 * branch delay slots), so the target build uses integer constants; the host build
 * points the same names at objects the test defines. */
#ifdef __MWERKS__
#define DOTR_TIKEI_OPEN (*(signed char *)0x1ed0066)
#define DOTR_TIKEI_STEP (*(short *)0x1ed0068)
#define DOTR_TIKEI_MODE (*(signed char *)0x1ed018c)
#define DOTR_TIKEI_DEPTH (*(short *)0x1ed0228)
#else
extern signed char dotr_tikei_open_01ed0066;
extern short dotr_tikei_step_01ed0068;
extern signed char dotr_tikei_mode_01ed018c;
extern short dotr_tikei_depth_01ed0228;
#define DOTR_TIKEI_OPEN dotr_tikei_open_01ed0066
#define DOTR_TIKEI_STEP dotr_tikei_step_01ed0068
#define DOTR_TIKEI_MODE dotr_tikei_mode_01ed018c
#define DOTR_TIKEI_DEPTH dotr_tikei_depth_01ed0228
#endif

/* Callees. Taku_Window_Set (taku_window), Taku_Camp_TIKEI_Map_Hyouji (camp_tikei_map) and Taku_FT4_Set
 * (quad_offsets): argument counts and widths assumed from this call site (0x2195e0 takes a ninth argument
 * on the stack). small_state.h declares Taku_Obj_Set (0x217bd0) as returning nothing; here
 * its v0 is passed on as the next cursor, as sheet_marks.h also assumes. The tests stub
 * all four. */
extern DotrTikeiWord Taku_Window_Set(DotrTikeiWord cursor, struct DotrTikeiWindow *window,
                                     unsigned short flag, int a3, unsigned short x, int a5,
                                     int a6, int a7, int a8);
DotrTikeiWord Taku_Camp_TIKEI_Map_Hyouji(DotrTikeiWord cursor, unsigned short mapIndex,
                                         unsigned short x, unsigned short y);
extern DotrTikeiWord Taku_Obj_Set(DotrTikeiWord cursor, struct DotrTikeiItem *item, short x,
                                  short y);
extern DotrTikeiWord Taku_FT4_Set(DotrTikeiWord cursor, struct DotrTikeiItem *item, short x,
                                  short y, int a4);

DotrTikeiWord Taku_Camp_TIKEI_WINDOW(DotrTikeiWord cursor);

/* 0x236f90. Two 38-byte templates copied to the stack, the 7 x 7 byte maps at
 * 0x2cee5c (small_misc.h has the same array), ten rows of two templates at 0x2986b0
 * and ten halfword counters at 0x1ed0006, addressed like the block above. */
struct DotrTikeiCounts {
    short unknown_00[3];
    short count[10];
};
#ifdef __MWERKS__
#define DOTR_TIKEI_COUNTS ((struct DotrTikeiCounts *)0x1ed0000)
#else
extern struct DotrTikeiCounts dotr_tikei_counts_01ed0000;
#define DOTR_TIKEI_COUNTS (&dotr_tikei_counts_01ed0000)
#endif
extern struct DotrTikeiItem data_0029afa0;
extern struct DotrTikeiItem data_0029afd0;
extern unsigned char gSzDuel_MapData[46][7][7];
extern struct DotrTikeiItem data_002986b0[10][2];

/* Callees of 0x236f90: quad_offsets.h declares 0x218970 and 0x218ab0 with a
 * quad-record pointer and halfword sizes; 0x236f90 itself calls 0x218ab0 through the
 * halfword view in camp_tikei_map.h. The word-sized Taku_G4_Set2 view and 0x1a22c0
 * (library code, assumed float to int, as hand_up.h has it) are declared here but called
 * by none of its includers. 0x1b6ea0 as in scaled_sprite.h. */
extern DotrTikeiWord Taku_G4_Set(DotrTikeiWord cursor, struct DotrTikeiItem *quad, short dx,
                                 short dy);
extern DotrTikeiWord Taku_G4_Set2(DotrTikeiWord cursor, struct DotrTikeiItem *quad, short dx,
                                  short dy, int width, int height);
extern int func_001a22c0(float value);
extern DotrTikeiWord SzDuel_DrawWindow_FontDigit_Pipe(DotrTikeiWord cursor, int x, int y, int a,
                                                      int flags, int width, int value);

/* Taku_Camp_SYS_ICON (0x2351a0, a neighbour that shares the builders): the loop reads
 * the 0x1ed0000 block through one pointer kept in a register (byte +126, halfword
 * +128, four halfwords from +130) while the halfword at 0x1ed0070 and the byte at
 * 0x1ed007e outside the loop are literal addresses; the host build maps all of them
 * onto one object. Four 38-byte templates at 0x298260, one at 0x2982f8, two at
 * 0x298b50, and five texts in .rodata passed by address (lengths 4, 4, 6, 4, 6 are
 * the sixth argument of 0x1b6f70; prototype as in small_misc.h). */
struct DotrTikeiMenu {
    char unknown_00[126];
    signed char menu;
    short pick;
    short grow[4];
};
#ifdef __MWERKS__
#define DOTR_TIKEI_MENU_BLOCK ((struct DotrTikeiMenu *)0x1ed0000)
#define DOTR_TIKEI_SLIDE (*(short *)0x1ed0070)
#define DOTR_TIKEI_MENU (*(signed char *)0x1ed007e)
#else
extern struct DotrTikeiMenu dotr_tikei_menu_01ed0000;
#define DOTR_TIKEI_MENU_BLOCK (&dotr_tikei_menu_01ed0000)
#define DOTR_TIKEI_SLIDE (*(short *)&dotr_tikei_menu_01ed0000.unknown_00[112])
#define DOTR_TIKEI_MENU (dotr_tikei_menu_01ed0000.menu)
#endif
extern struct DotrTikeiItem data_002982f8;
extern struct DotrTikeiItem camp_sys_icon[];
extern struct DotrTikeiItem camp_sys_icon_botan[];
extern char data_0030f102[];
extern char data_0030f107[];
extern char data_0030f10c[];
extern char data_0030f113[];
extern char data_0030f118[];
extern DotrTikeiWord SzDuel_DrawWindow_Font_Pipe(DotrTikeiWord cursor, int x, int y, int a,
                                                 int flags, int n, const char *text, int c);

DotrTikeiWord Taku_Camp_SYS_ICON(DotrTikeiWord cursor);

/* Taku_Camp_Kumo (0x226120, the full-screen cloud/fade quad). The 38-byte template at
 * 0x29ab80 is copied to the stack and edited by member: eight halfword coordinates,
 * four colour quadruples written a byte at a time, and a halfword at +34 set to -10
 * for three stage numbers. Member names are assumed from the stores; the camp state's
 * halfword at +194 (0x299ed2) is read through the block pointer; the first read is
 * folded to the absolute address only when the block is a structure object, so this
 * is a structure view of 0x299e10 (a cast of the byte array above compiles differently).
 * The mode and fade halfwords at 0x1ed53a2 / 0x1ed53a4 and the limit at 0x1ed536c
 * (glide_steps.h has the same object) are literal addresses. 0x21bb20 and the frame are
 * declared as in glide_steps.h. */
struct DotrTikeiQuad {
    short w[8];
    struct {
        unsigned char r, g, b, a;
    } c[4];
    short unknown_20;
    short z;
    short code;
};
struct DotrTikeiCamp {
    char unknown_00[194];
    short stage;
};
#ifdef __MWERKS__
#define DOTR_TIKEI_LIMIT (*(short *)0x1ed536c)
#define DOTR_TIKEI_KUMO_MODE (*(short *)0x1ed53a2)
#define DOTR_TIKEI_KUMO_FADE (*(short *)0x1ed53a4)
#else
extern short dotr_glide_limit_01ed536c;
extern short dotr_tikei_kumo_mode_01ed53a2;
extern short dotr_tikei_kumo_fade_01ed53a4;
#define DOTR_TIKEI_LIMIT dotr_glide_limit_01ed536c
#define DOTR_TIKEI_KUMO_MODE dotr_tikei_kumo_mode_01ed53a2
#define DOTR_TIKEI_KUMO_FADE dotr_tikei_kumo_fade_01ed53a4
#endif
extern struct DotrTikeiQuad data_0029ab80;
extern struct DotrTikeiCamp data_00299e10_camp;
struct DotrGlideFrame;
extern struct DotrGlideFrame prim_data_glide;
extern void Taku_Coord_Set(struct DotrGlideFrame *frame);

DotrTikeiWord Taku_Camp_Kumo(DotrTikeiWord cursor);

/* TkCamp_Draw_After (0x21daa0, the camp screen's late 2D pass). Scalars of the 0x1ed0000
 * block are literal addresses in the target: the scroll halfword at 0x1ed006c, the leader
 * number at 0x1ed5404, the leader model state byte at 0x1ed5406, its alpha at 0x1ed543a,
 * the slide byte at 0x1ed5440, the card flag / value / position at 0x1ed5472 / 0x1ed5474 /
 * 0x1ed5478 and two flag bytes at 0x1ed5481 / 0x1ed5488; the host build gives each an
 * object (the scroll lies inside the menu block above). Two 16-byte aligned vectors at
 * 0x29a7b0 / 0x29a7c0 are copied to the stack for the position and rotation setters
 * (object_table.h has the real prototypes with its own vector type). The template at
 * 0x298ad0 is read as the 38-byte item. Callee prototypes are assumed from the argument
 * registers. */
struct DotrTikeiVector {
    float v[4];
} __attribute__((aligned(16)));
#ifdef __MWERKS__
#define DOTR_TIKEI_SCROLL (*(short *)0x1ed006c)
#define DOTR_TIKEI_LEADER (*(short *)0x1ed5404)
#define DOTR_TIKEI_LEADER_STATE (*(signed char *)0x1ed5406)
#define DOTR_TIKEI_LEADER_ALPHA (*(short *)0x1ed543a)
#define DOTR_TIKEI_LEADER_SLIDE (*(signed char *)0x1ed5440)
#define DOTR_TIKEI_CARD_ON (*(signed char *)0x1ed5472)
#define DOTR_TIKEI_CARD (*(int *)0x1ed5474)
#define DOTR_TIKEI_CARD_X (*(short *)0x1ed5478)
#define DOTR_TIKEI_FLAG_5481 (*(signed char *)0x1ed5481)
#define DOTR_TIKEI_QUAD_ON (*(signed char *)0x1ed5488)
#else
extern short dotr_tikei_leader_01ed5404;
extern signed char dotr_tikei_leader_state_01ed5406;
extern short dotr_tikei_leader_alpha_01ed543a;
extern signed char dotr_tikei_leader_slide_01ed5440;
extern signed char dotr_tikei_card_on_01ed5472;
extern int dotr_tikei_card_01ed5474;
extern short dotr_tikei_card_x_01ed5478;
extern signed char dotr_tikei_flag_01ed5481;
extern signed char dotr_tikei_quad_on_01ed5488;
#define DOTR_TIKEI_SCROLL (*(short *)&dotr_tikei_menu_01ed0000.unknown_00[108])
#define DOTR_TIKEI_LEADER dotr_tikei_leader_01ed5404
#define DOTR_TIKEI_LEADER_STATE dotr_tikei_leader_state_01ed5406
#define DOTR_TIKEI_LEADER_ALPHA dotr_tikei_leader_alpha_01ed543a
#define DOTR_TIKEI_LEADER_SLIDE dotr_tikei_leader_slide_01ed5440
#define DOTR_TIKEI_CARD_ON dotr_tikei_card_on_01ed5472
#define DOTR_TIKEI_CARD dotr_tikei_card_01ed5474
#define DOTR_TIKEI_CARD_X dotr_tikei_card_x_01ed5478
#define DOTR_TIKEI_FLAG_5481 dotr_tikei_flag_01ed5481
#define DOTR_TIKEI_QUAD_ON dotr_tikei_quad_on_01ed5488
#endif
extern struct DotrTikeiItem teki_a_seru;
extern struct DotrTikeiVector data_0029a7b0;
extern struct DotrTikeiVector data_0029a7c0;
extern signed char Move_END_flg;
extern DotrTikeiWord RhBase_GetVifPacketArea(void);
extern void RhBase_SetVifPacketArea(DotrTikeiWord cursor);
extern DotrTikeiWord Taku_Camp_2D(DotrTikeiWord cursor);
extern int SzModel_IsModelLoading(int modelId);
extern int SzModel_IsModelStandby(int index);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzModel_SetPos(int modelId, struct DotrTikeiVector *pos);
extern int SzModel_SetRot(int index, struct DotrTikeiVector *in);
extern int SzDuel_New2Xax(int cardNo);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int d);
extern int SzDuel_GetCamDist(int cardId);
extern DotrTikeiWord Taku_Camp_Teki_Model_Disp(DotrTikeiWord cursor, int modelId, int x, int y,
                                               int w, int h, float dist, int alpha);
extern DotrTikeiWord Taku_Camp_Card_Get(DotrTikeiWord cursor, int kind, float x, float y,
                                        int value);

void TkCamp_Draw_After(void);

#endif
