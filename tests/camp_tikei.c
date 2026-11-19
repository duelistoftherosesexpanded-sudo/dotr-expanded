/* Synthetic host tests for the camp_tikei group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_tikei.h"

struct DotrTikeiItem camp_window_tikei_partu[10];
struct DotrTikeiItem camp_window_tikei_partu_gejiwaku[3];
struct DotrTikeiItem camp_window_tikei_iro_ue[2];
struct DotrTikeiItem camp_window_tikei_iro_sita[2];
struct DotrTikeiModel data_00298ba0_tikei[12];
struct DotrTikeiWindow camp_window_Map_data[3];
signed char data_00299e10[200];
short SelectedBattleArena[1];
signed char dotr_tikei_open_01ed0066;
short dotr_tikei_step_01ed0068;
signed char dotr_tikei_mode_01ed018c;
short dotr_tikei_depth_01ed0228;

/* Each stub records its arguments and returns the cursor plus one, so the chain of
 * cursors shows the call order. */
static struct {
    int n;
    struct DotrTikeiWindow *window[3];
    int flag[3], x[3], last[3];
} win;
static struct {
    int n, no, x;
} map;
static struct {
    int n;
    struct DotrTikeiItem *item[40];
    struct DotrTikeiItem copy[40];
    int x[40], y[40];
} obj;
static struct {
    int n;
    struct DotrTikeiItem *item[2];
    int x[2], y[2], mode[2];
} ft4;

DotrTikeiWord Taku_Window_Set(DotrTikeiWord cursor, struct DotrTikeiWindow *window,
                              unsigned short flag, int a3, unsigned short x, int a5, int a6,
                              int a7, int a8)
{
    assert(a3 == 0 && a5 == 0 && a6 == 0 && a7 == 128);
    win.window[win.n] = window;
    win.flag[win.n] = flag;
    win.x[win.n] = x;
    win.last[win.n++] = a8;
    return cursor + 1;
}

DotrTikeiWord Taku_Camp_TIKEI_Map_Hyouji(DotrTikeiWord cursor, unsigned short mapIndex,
                                         unsigned short x, unsigned short y)
{
    assert(y == 0 && cursor == 101);
    map.n++;
    map.no = mapIndex;
    map.x = x;
    return cursor + 1;
}

DotrTikeiWord Taku_Obj_Set(DotrTikeiWord cursor, struct DotrTikeiItem *item, short x, short y)
{
    obj.item[obj.n] = item;
    obj.copy[obj.n] = *item;
    obj.x[obj.n] = x;
    obj.y[obj.n++] = y;
    return cursor + 1;
}

DotrTikeiWord Taku_FT4_Set(DotrTikeiWord cursor, struct DotrTikeiItem *item, short x, short y,
                           int a4)
{
    ft4.item[ft4.n] = item;
    ft4.x[ft4.n] = x;
    ft4.y[ft4.n] = y;
    ft4.mode[ft4.n++] = a4;
    return cursor + 1;
}

/* Stubs and fixtures for 0x2351a0. */
struct DotrTikeiMenu dotr_tikei_menu_01ed0000;
struct DotrTikeiItem data_002982f8;
struct DotrTikeiItem camp_sys_icon[4];
struct DotrTikeiItem camp_sys_icon_botan[2];
char data_0030f102[5], data_0030f107[5], data_0030f10c[7], data_0030f113[5], data_0030f118[7];
static struct {
    int n;
    struct DotrTikeiItem *quad;
    struct DotrTikeiItem copy;
    int x, y;
} g4;
static struct {
    int n;
    DotrTikeiWord cursor[2];
    int y[2], len[2];
    const char *text[2];
} font;

DotrTikeiWord Taku_G4_Set(DotrTikeiWord cursor, struct DotrTikeiItem *quad, short dx, short dy)
{
    g4.n++;
    g4.quad = quad;
    g4.copy = *quad;
    g4.x = dx;
    g4.y = dy;
    return cursor + 1;
}

DotrTikeiWord SzDuel_DrawWindow_Font_Pipe(DotrTikeiWord cursor, int x, int y, int a, int flags,
                                          int n, const char *text, int c)
{
    assert(x == 562 && a == 1 && flags == 1 && c == 0);
    font.cursor[font.n] = cursor;
    font.y[font.n] = y;
    font.len[font.n] = n;
    font.text[font.n++] = text;
    return cursor + 1;
}

static DotrTikeiWord run_icon(int slide, int menu, int pick, int g0, int g1, int g2, int g3)
{
    int i, k;

    memset(&obj, 0, sizeof obj);
    memset(&g4, 0, sizeof g4);
    memset(&font, 0, sizeof font);
    memset(&dotr_tikei_menu_01ed0000, 0, sizeof dotr_tikei_menu_01ed0000);
    DOTR_TIKEI_SLIDE = (short)slide;
    dotr_tikei_menu_01ed0000.menu = (signed char)menu;
    dotr_tikei_menu_01ed0000.pick = (short)pick;
    dotr_tikei_menu_01ed0000.grow[0] = (short)g0;
    dotr_tikei_menu_01ed0000.grow[1] = (short)g1;
    dotr_tikei_menu_01ed0000.grow[2] = (short)g2;
    dotr_tikei_menu_01ed0000.grow[3] = (short)g3;
    for (i = 0; i < 4; i++) {
        for (k = 0; k < 8; k++) {
            camp_sys_icon[i].w[k] = (short)(100 * i + 10 * k + 50);
        }
        camp_sys_icon[i].code = (short)(700 + i);
    }
    return Taku_Camp_SYS_ICON(100);
}

static void check_icon_item(int call, int i, int d)
{
    int k;

    assert(obj.item[call] != &camp_sys_icon[i] && obj.x[call] == 0);
    for (k = 0; k < 8; k++) {
        int want = 100 * i + 10 * k + 50;
        if (k < 2) {
            want -= d;
        } else if (k < 4) {
            want += d;
        }
        assert(obj.copy[call].w[k] == want);
    }
    assert(obj.copy[call].code == 700 + i);
}

static void check_icon(void)
{
    short *grow = dotr_tikei_menu_01ed0000.grow;
    int i;

    /* Menu closed: every positive counter shrinks by 4 with a floor of 0 (4 -> 0,
     * 3 -> 0, 8 -> 4), zero stays; the first tail template and the six-letter text. */
    assert(run_icon(0, 0, 1, 4, 3, 8, 0) == 100 + 1 + 4 + 1 + 1);
    assert(g4.n == 1 && g4.quad == &data_002982f8 && g4.x == 0 && g4.y == 128);
    assert(grow[0] == 0 && grow[1] == 0 && grow[2] == 4 && grow[3] == 0);
    assert(obj.n == 5 && font.n == 1);
    for (i = 0; i < 4; i++) {
        check_icon_item(i, i, 0);
        assert(obj.y[i] == 128);
    }
    assert(obj.item[4] == &camp_sys_icon_botan[0] && obj.x[4] == 0 && obj.y[4] == 128);
    assert(font.cursor[0] == 106 && font.y[0] == 128 + 335 && font.len[0] == 6);
    assert(font.text[0] == data_0030f118);
    /* A negative counter is left alone. */
    assert(run_icon(0, 0, 0, -2, 0, 0, 0) == 107 && grow[0] == -2);
    /* Menu 1, pick 1 growing 4 -> 8: the copy moves by (8 * 6) >> 4 = 3; text 1 of
     * length 4; the others shrink; second tail template and no tail text. slide 256
     * gives y = 0. */
    assert(run_icon(256, 1, 1, 8, 4, 0, 0) == 100 + 1 + 4 + 1 + 1);
    assert(g4.y == 0 && grow[0] == 4 && grow[1] == 8 && obj.n == 5 && font.n == 1);
    check_icon_item(0, 0, 0);
    check_icon_item(1, 1, 3);
    check_icon_item(2, 2, 0);
    assert(font.cursor[0] == 103 && font.y[0] == 335 && font.len[0] == 4);
    assert(font.text[0] == data_0030f107);
    assert(obj.item[4] == &camp_sys_icon_botan[1] && obj.y[4] == 0);
    /* Upper bound: 11 -> 15 (no clamp), 12 -> 16, 16 -> 16; the texts of picks 0, 2, 3. */
    assert(run_icon(0, 1, 0, 11, 0, 0, 0) == 107 && grow[0] == 15);
    check_icon_item(0, 0, 5);
    assert(font.len[0] == 4 && font.text[0] == data_0030f102 && font.cursor[0] == 102);
    assert(run_icon(0, 1, 2, 0, 0, 12, 0) == 107 && grow[2] == 16);
    check_icon_item(2, 2, 6);
    assert(font.len[0] == 6 && font.text[0] == data_0030f10c && font.cursor[0] == 104);
    assert(run_icon(0, 1, 3, 0, 0, 0, 16) == 107 && grow[3] == 16);
    check_icon_item(3, 3, 6);
    assert(font.len[0] == 4 && font.text[0] == data_0030f113 && font.cursor[0] == 105);
    /* Menu neither 0 nor 1: the pick still grows but no text is drawn. */
    assert(run_icon(0, 2, 0, 0, 0, 0, 0) == 106 && grow[0] == 4 && font.n == 0);
    check_icon_item(0, 0, 1);
    assert(obj.item[4] == &camp_sys_icon_botan[1]);
    /* Menu 1 with a pick outside 0..3: nothing grows, no text. */
    assert(run_icon(0, 1, 7, 0, 8, 0, 0) == 106 && grow[1] == 4 && font.n == 0);
}

static DotrTikeiWord run(int open, int step, int mode, int kind_index, int side)
{
    memset(&win, 0, sizeof win);
    memset(&map, 0, sizeof map);
    memset(&obj, 0, sizeof obj);
    memset(&ft4, 0, sizeof ft4);
    dotr_tikei_open_01ed0066 = (signed char)open;
    dotr_tikei_step_01ed0068 = (short)step;
    dotr_tikei_mode_01ed018c = (signed char)mode;
    dotr_tikei_depth_01ed0228 = 77;
    data_00299e10[0] = (signed char)side;
    data_00299e10[198] = (signed char)kind_index;
    SelectedBattleArena[0] = 0x1234;
    return Taku_Camp_TIKEI_WINDOW(100);
}

static void check_window(void)
{
    int i;

    memset(data_00298ba0_tikei, 0, sizeof data_00298ba0_tikei);
    data_00298ba0_tikei[3].kind = 8;

    /* Closed and already at zero: step stays 0 (lower clamp), nothing is drawn. */
    assert(run(0, 0, 0, 3, 0) == 100 && dotr_tikei_step_01ed0068 == 0);
    assert(win.n == 0 && map.n == 0 && obj.n == 0 && ft4.n == 0);
    /* Closed from 1: -1 clamps to 0. Closed from 2: exactly 0, still nothing. */
    assert(run(0, 1, 0, 3, 0) == 100 && dotr_tikei_step_01ed0068 == 0 && win.n == 0);
    assert(run(0, 2, 0, 3, 0) == 100 && dotr_tikei_step_01ed0068 == 0 && win.n == 0);
    /* Open at 16 and at 15: both clamp to 16; x is 256 - 16 * 16 = 0. */
    assert(run(1, 16, 0, 3, 0) == 100 + 3 + 1 + 10 + 30 + 2 && dotr_tikei_step_01ed0068 == 16);
    assert(win.x[0] == 0);
    assert(run(1, 15, 0, 3, 0) == 146 && dotr_tikei_step_01ed0068 == 16);
    /* Open from 4: step 6 (no clamp), x = 256 - 96 = 160; kind 8 gives flag 0. */
    assert(run(1, 4, 0, 3, 1) == 146 && dotr_tikei_step_01ed0068 == 6);
    assert(win.n == 3 && map.n == 1 && obj.n == 40 && ft4.n == 2);
    assert(win.window[0] == &camp_window_Map_data[0] && win.flag[0] == 0 && win.x[0] == 160 && win.last[0] == 77);
    assert(win.window[1] == &camp_window_Map_data[1] && win.flag[1] == 0 && win.x[1] == 160 && win.last[1] == 128);
    assert(win.window[2] == &camp_window_Map_data[2] && win.flag[2] == 0 && win.x[2] == 160 && win.last[2] == 128);
    assert(map.no == 0x1234 && map.x == 160);
    for (i = 0; i < 10; i++) {
        assert(obj.item[i] == &camp_window_tikei_partu[i] && obj.x[i] == 160 && obj.y[i] == 0);
    }
    for (i = 0; i < 30; i++) {
        assert(obj.item[10 + i] == &camp_window_tikei_partu_gejiwaku[i % 3] && obj.x[10 + i] == 160);
        assert(obj.y[10 + i] == 16 * (i / 3));
    }
    assert(ft4.item[0] == &camp_window_tikei_iro_ue[0] && ft4.x[0] == 160 && ft4.y[0] == 0 && ft4.mode[0] == 0);
    assert(ft4.item[1] == &camp_window_tikei_iro_sita[1] && ft4.x[1] == 160 && ft4.y[1] == 0 && ft4.mode[1] == 2);
    /* Closing from 6: step 4, x = 192; a record whose kind is not 8 gives flag 1. */
    assert(run(0, 6, 0, 2, 0) == 146 && dotr_tikei_step_01ed0068 == 4);
    assert(win.flag[0] == 1 && win.flag[1] == 1 && win.flag[2] == 1 && win.x[0] == 192);
    assert(ft4.item[0] == &camp_window_tikei_iro_ue[1] && ft4.item[1] == &camp_window_tikei_iro_sita[0]);
    /* Index 11 with mode 1, and index 11 with another mode: flag is side ^ 1. */
    assert(run(1, 4, 1, 11, 1) == 146 && win.flag[0] == 0 && win.flag[2] == 0);
    assert(ft4.item[0] == &camp_window_tikei_iro_ue[0]);
    assert(run(1, 4, 0, 11, 0) == 146 && win.flag[0] == 1 && win.flag[2] == 1);
    assert(ft4.item[0] == &camp_window_tikei_iro_ue[1] && ft4.item[1] == &camp_window_tikei_iro_sita[0]);
    /* Mode 1 with another index falls through to the record test. */
    assert(run(1, 4, 1, 3, 0) == 146 && win.flag[0] == 0 && win.flag[2] == 1);
}

/* Taku_Camp_Kumo fixtures: the frame is opaque to the function, so the host gives it a body. */
struct DotrGlideFrame {
    int unused;
};
struct DotrGlideFrame prim_data_glide;
struct DotrTikeiQuad data_0029ab80;
struct DotrTikeiCamp data_00299e10_camp;
short dotr_glide_limit_01ed536c;
short dotr_tikei_kumo_mode_01ed53a2;
short dotr_tikei_kumo_fade_01ed53a4;
static struct {
    int n;
    struct DotrGlideFrame *frame;
} coord;

void Taku_Coord_Set(struct DotrGlideFrame *frame)
{
    coord.n++;
    coord.frame = frame;
}

static DotrTikeiWord kumo(int stage, int limit, int mode, int fade)
{
    struct DotrTikeiCamp *camp = &data_00299e10_camp;
    int i;

    for (i = 0; i < 8; i++) {
        data_0029ab80.w[i] = (short)(100 + i);
    }
    for (i = 0; i < 4; i++) {
        data_0029ab80.c[i].r = 1;
        data_0029ab80.c[i].g = 2;
        data_0029ab80.c[i].b = 3;
        data_0029ab80.c[i].a = 4;
    }
    data_0029ab80.unknown_20 = 55;
    data_0029ab80.z = 66;
    data_0029ab80.code = 77;
    camp->stage = (short)stage;
    dotr_glide_limit_01ed536c = (short)limit;
    dotr_tikei_kumo_mode_01ed53a2 = (short)mode;
    dotr_tikei_kumo_fade_01ed53a4 = (short)fade;
    memset(&g4, 0, sizeof g4);
    memset(&coord, 0, sizeof coord);
    return Taku_Camp_Kumo(500);
}

/* The quad the stub saw: colour r, g, b in every corner, alpha a, depth z. */
static int kumo_quad(int r, int g, int b, int a, int z)
{
    struct DotrTikeiQuad q;
    int i;

    memcpy(&q, &g4.copy, sizeof q);
    for (i = 0; i < 4; i++) {
        if (q.c[i].r != r || q.c[i].g != g || q.c[i].b != b || q.c[i].a != a) {
            return 0;
        }
    }
    return g4.n == 1 && g4.x == 0 && g4.y == 0 && q.z == z && q.unknown_20 == 55 && q.code == 77 &&
           q.w[0] == 0 && q.w[4] == 0 && q.w[2] == 640 && q.w[6] == 640 && q.w[1] == 0 && q.w[3] == 0 &&
           q.w[5] == 448 && q.w[7] == 448;
}

#define KUMO_MODE dotr_tikei_kumo_mode_01ed53a2
#define KUMO_FADE dotr_tikei_kumo_fade_01ed53a4
static void check_kumo(void)
{
    /* Mode 0: nothing drawn, the frame is still set up, the cursor comes back. */
    assert(kumo(1, 0, 0, 100) == 500 && g4.n == 0 && coord.n == 1 && coord.frame == &prim_data_glide);
    assert(KUMO_FADE == 100 && data_0029ab80.z == 66);
    /* Modes 2 and 5 (no fade step): fade untouched, template colours, alpha = fade * 128 / 4096. */
    assert(kumo(1, 0, 2, 2048) == 501 && KUMO_MODE == 2 && KUMO_FADE == 2048 && kumo_quad(1, 2, 3, 64, 66));
    assert(coord.n == 1);
    assert(kumo(1, 0, 5, 4096) == 501 && KUMO_FADE == 4096 && kumo_quad(1, 2, 3, 128, 66));
    /* Mode 3: negative fade is left; 17 -> 1 stays; 16 -> 0 ends in mode 0 (still drawn). */
    assert(kumo(1, 0, 3, -1) == 501 && KUMO_FADE == -1 && KUMO_MODE == 3);
    assert(kumo(1, 0, 3, 17) == 501 && KUMO_FADE == 1 && KUMO_MODE == 3);
    assert(kumo(1, 0, 3, 16) == 501 && KUMO_FADE == 0 && KUMO_MODE == 0 && kumo_quad(1, 2, 3, 0, 66));
    assert(kumo(1, 0, 3, 0) == 501 && KUMO_FADE == 0 && KUMO_MODE == 0);
    /* Mode 4: above 4096 is left; 4096 itself and 4064 clamp to 4096; 0 -> 32. */
    assert(kumo(1, 0, 4, 4097) == 501 && KUMO_FADE == 4097 && KUMO_MODE == 4);
    assert(kumo(1, 0, 4, 4096) == 501 && KUMO_FADE == 4096 && KUMO_MODE == 4);
    assert(kumo(1, 0, 4, 4064) == 501 && KUMO_FADE == 4096 && KUMO_MODE == 4);
    assert(kumo(1, 0, 4, 0) == 501 && KUMO_FADE == 32 && kumo_quad(1, 2, 3, 1, 66));
    /* Mode 10: +1024; reaching 4096 clamps and moves to 11. Colour (0, 0, 32) from 10 to 19. */
    assert(kumo(1, 0, 10, 0) == 501 && KUMO_FADE == 1024 && KUMO_MODE == 10 && kumo_quad(0, 0, 32, 32, 66));
    assert(kumo(1, 0, 10, 3072) == 501 && KUMO_FADE == 4096 && KUMO_MODE == 11 && kumo_quad(0, 0, 32, 128, 66));
    assert(kumo(1, 0, 19, 4096) == 501 && kumo_quad(0, 0, 32, 128, 66));
    assert(kumo(1, 0, 9, 4096) == 501 && kumo_quad(1, 2, 3, 128, 66));
    /* Mode 20: -256; reaching 0 moves to 21. Black from 20 to 29. */
    assert(kumo(1, 0, 20, 512) == 501 && KUMO_FADE == 256 && KUMO_MODE == 20 && kumo_quad(0, 0, 0, 8, 66));
    assert(kumo(1, 0, 20, 256) == 501 && KUMO_FADE == 0 && KUMO_MODE == 21 && kumo_quad(0, 0, 0, 0, 66));
    /* Mode 22: +256; reaching 4096 moves to 23. */
    assert(kumo(1, 0, 22, 0) == 501 && KUMO_FADE == 256 && KUMO_MODE == 22);
    assert(kumo(1, 0, 22, 3840) == 501 && KUMO_FADE == 4096 && KUMO_MODE == 23);
    assert(kumo(1, 0, 29, 4096) == 501 && kumo_quad(0, 0, 0, 128, 66));
    /* Mode 30: +1024 to 31. White from 30 up. */
    assert(kumo(1, 0, 30, 0) == 501 && KUMO_FADE == 1024 && KUMO_MODE == 30 && kumo_quad(255, 255, 255, 32, 66));
    assert(kumo(1, 0, 30, 3072) == 501 && KUMO_FADE == 4096 && KUMO_MODE == 31);
    /* Mode 32: -32; reaching 0 moves to 33. */
    assert(kumo(1, 0, 32, 64) == 501 && KUMO_FADE == 32 && KUMO_MODE == 32 && kumo_quad(255, 255, 255, 1, 66));
    assert(kumo(1, 0, 32, 32) == 501 && KUMO_FADE == 0 && KUMO_MODE == 33);
    /* Depth -10: stage 10020 while the limit is below 6, stages 11184 and 12156 always. */
    assert(kumo(10020, 5, 2, 0) == 501 && kumo_quad(1, 2, 3, 0, -10));
    assert(kumo(10020, 6, 2, 0) == 501 && kumo_quad(1, 2, 3, 0, 66));
    assert(kumo(11184, 9, 2, 0) == 501 && kumo_quad(1, 2, 3, 0, -10));
    assert(kumo(12156, 9, 2, 0) == 501 && kumo_quad(1, 2, 3, 0, -10));
    /* Not drawn: stage 10040 from limit 2, stages 11162 and 12132. The fade still steps. */
    assert(kumo(10040, 1, 2, 0) == 501 && g4.n == 1);
    assert(kumo(10040, 2, 22, 0) == 500 && g4.n == 0 && coord.n == 1 && KUMO_FADE == 256);
    assert(kumo(11162, 0, 2, 0) == 500 && g4.n == 0 && coord.n == 1);
    assert(kumo(12132, 0, 2, 0) == 500 && g4.n == 0 && coord.n == 1);
}

/* TkCamp_Draw_After: stubs record the call order in `after.log` (one letter per call). */
struct DotrTikeiItem teki_a_seru;
struct DotrTikeiVector data_0029a7b0;
struct DotrTikeiVector data_0029a7c0;
signed char Move_END_flg;
short dotr_tikei_leader_01ed5404;
signed char dotr_tikei_leader_state_01ed5406;
short dotr_tikei_leader_alpha_01ed543a;
signed char dotr_tikei_leader_slide_01ed5440;
signed char dotr_tikei_card_on_01ed5472;
int dotr_tikei_card_01ed5474;
short dotr_tikei_card_x_01ed5478;
signed char dotr_tikei_flag_01ed5481;
signed char dotr_tikei_quad_on_01ed5488;

static struct {
    char log[32];
    int n;
    int loading, standby;
    DotrTikeiWord set;
    struct DotrTikeiVector pos, rot;
    int load_c;
    int disp[5];
    float dist;
    int kind, value;
    float cx, cy;
} after;
static int after_clear_scroll;

DotrTikeiWord RhBase_GetVifPacketArea(void)
{
    after.log[after.n++] = 'g';
    return 1000;
}

void RhBase_SetVifPacketArea(DotrTikeiWord cursor)
{
    after.log[after.n++] = 's';
    after.set = cursor;
}

DotrTikeiWord Taku_Camp_2D(DotrTikeiWord cursor)
{
    after.log[after.n++] = '2';
    return cursor + 16;
}

int SzModel_IsModelLoading(int index)
{
    assert(index == 31);
    after.log[after.n++] = 'l';
    if (after_clear_scroll) {
        DOTR_TIKEI_SCROLL = 0;
    }
    return after.loading;
}

int SzModel_IsModelStandby(int index)
{
    assert(index == 31);
    after.log[after.n++] = 'b';
    return after.standby;
}

int SzData_GetLeader(int side, int list)
{
    assert(side == 1 && list == 0);
    after.log[after.n++] = 'L';
    return 77;
}

int SzModel_SetPos(int index, struct DotrTikeiVector *in)
{
    assert(index == 31);
    after.log[after.n++] = 'p';
    after.pos = *in;
    return 0;
}

int SzModel_SetRot(int index, struct DotrTikeiVector *in)
{
    assert(index == 31);
    after.log[after.n++] = 'r';
    after.rot = *in;
    return 0;
}

int SzDuel_New2Xax(int value)
{
    after.log[after.n++] = 'x';
    return value + 1000;
}

int SzModel_Load(int a, int b, int c, int d)
{
    assert(a == 31 && b == -1 && d == 1);
    after.log[after.n++] = 'm';
    after.load_c = c;
    return 1;
}

int SzDuel_GetCamDist(int index)
{
    after.log[after.n++] = 'd';
    return index * 2;
}

DotrTikeiWord Taku_Camp_Teki_Model_Disp(DotrTikeiWord cursor, int index, int x, int y, int w,
                                        int h, float dist, int alpha)
{
    assert(index == 31);
    after.log[after.n++] = 'D';
    after.disp[0] = x;
    after.disp[1] = y;
    after.disp[2] = w;
    after.disp[3] = h;
    after.disp[4] = alpha;
    after.dist = dist;
    return cursor + 4;
}

DotrTikeiWord Taku_Camp_Card_Get(DotrTikeiWord cursor, int kind, float x, float y, int value)
{
    after.log[after.n++] = 'c';
    after.kind = kind;
    after.cx = x;
    after.cy = y;
    after.value = value;
    return cursor + 2;
}

static const char *draw_after(int scroll, int quad_on, int loading, int state, int standby,
                              int alpha, int card_on)
{
    memset(&after, 0, sizeof after);
    memset(&g4, 0, sizeof g4);
    memset(&dotr_tikei_menu_01ed0000, 0, sizeof dotr_tikei_menu_01ed0000);
    DOTR_TIKEI_SCROLL = (short)scroll;
    DOTR_TIKEI_QUAD_ON = (signed char)quad_on;
    DOTR_TIKEI_LEADER_STATE = (signed char)state;
    DOTR_TIKEI_LEADER_ALPHA = (short)alpha;
    DOTR_TIKEI_LEADER_SLIDE = 50;
    DOTR_TIKEI_LEADER = 9;
    DOTR_TIKEI_CARD_ON = (signed char)card_on;
    DOTR_TIKEI_CARD = 4321;
    DOTR_TIKEI_CARD_X = 0x1000;
    DOTR_TIKEI_FLAG_5481 = 5;
    after.loading = loading;
    after.standby = standby;
    data_00299e10[0] = 1;
    teki_a_seru.w[0] = 10;
    teki_a_seru.w[1] = 20;
    teki_a_seru.w[2] = 110;
    teki_a_seru.w[5] = 220;
    data_0029a7b0.v[0] = 1.0f;
    data_0029a7b0.v[3] = 4.0f;
    data_0029a7c0.v[0] = 5.0f;
    data_0029a7c0.v[3] = 8.0f;
    TkCamp_Draw_After();
    return after.log;
}

static void check_draw_after(void)
{
    /* Busy flag set: nothing is called and nothing is stored. */
    Move_END_flg = 1;
    assert(strcmp(draw_after(300, 1, 0, 1, 1, 0, 1), "") == 0 && DOTR_TIKEI_FLAG_5481 == 5);
    Move_END_flg = 0;
    /* Scroll 0: only the 2D pass, the flag clear and the cursor store. */
    assert(strcmp(draw_after(0, 1, 0, 1, 1, 0, 0), "g2s") == 0 && after.set == 1016);
    assert(DOTR_TIKEI_FLAG_5481 == 0 && DOTR_TIKEI_QUAD_ON == 1 && g4.n == 0);
    /* Card only: side byte ^ 1, x = (0x1000 << 10 >> 12) - 512, y 0, the value. */
    assert(strcmp(draw_after(0, 0, 0, 0, 0, 0, 1), "g2cs") == 0 && after.set == 1018);
    assert(after.kind == 0 && after.cx == 512.0f && after.cy == 0.0f && after.value == 4321);
    /* Scroll set, quad flag set: template at (slide * -640 / 100, scroll - 256), flag cleared.
     * Model still loading and not standing by: no load, no display. */
    assert(strcmp(draw_after(300, 1, 1, 1, 0, 0, 0), "g2lbs") == 0 && after.set == 1017);
    assert(g4.n == 1 && g4.quad == &teki_a_seru && g4.x == -320 && g4.y == 44);
    assert(DOTR_TIKEI_QUAD_ON == 0 && DOTR_TIKEI_LEADER_STATE == 1);
    /* Not loading but state not 1: no load. Standing by but state not 2: no display. */
    assert(strcmp(draw_after(300, 0, 0, 3, 1, 0, 0), "g2lbs") == 0 && after.set == 1016 && g4.n == 0);
    /* Not loading, state 1, but the scroll was cleared meanwhile: no load. */
    after_clear_scroll = 1;
    assert(strcmp(draw_after(300, 0, 0, 1, 0, 0, 0), "g2lbs") == 0 && DOTR_TIKEI_LEADER_STATE == 1);
    after_clear_scroll = 0;
    /* Not loading, state 1: leader fetched, vectors copied, model loaded, state 2, alpha 0;
     * then standing by in state 2: alpha 0 -> 8 and the display call. */
    assert(strcmp(draw_after(300, 0, 0, 1, 1, 64, 0), "g2lLprxmbdDs") == 0 && after.set == 1020);
    assert(DOTR_TIKEI_LEADER == 77 && DOTR_TIKEI_LEADER_STATE == 2 && after.load_c == 1077);
    assert(after.pos.v[0] == 1.0f && after.pos.v[3] == 4.0f && after.rot.v[0] == 5.0f && after.rot.v[3] == 8.0f);
    assert(DOTR_TIKEI_LEADER_ALPHA == 8 && after.disp[4] == 8 && after.dist == 154.0f);
    assert(after.disp[0] == 10 - 320 && after.disp[1] == 20 + 44 && after.disp[2] == 100 && after.disp[3] == 200);
    /* Same without standby: loaded, not displayed. */
    assert(strcmp(draw_after(300, 0, 0, 1, 0, 64, 0), "g2lLprxmbs") == 0 && DOTR_TIKEI_LEADER_ALPHA == 0);
    /* State 2, standing by: alpha 112 -> 120, 120 -> 128 (clamped), 128 and 200 -> 128. */
    assert(strcmp(draw_after(300, 0, 1, 2, 1, 112, 0), "g2lbdDs") == 0 && DOTR_TIKEI_LEADER_ALPHA == 120);
    assert(after.dist == 18.0f && after.disp[4] == 120);
    assert(strcmp(draw_after(300, 0, 1, 2, 1, 120, 0), "g2lbdDs") == 0 && DOTR_TIKEI_LEADER_ALPHA == 128);
    assert(strcmp(draw_after(300, 0, 1, 2, 1, 128, 0), "g2lbdDs") == 0 && DOTR_TIKEI_LEADER_ALPHA == 128);
    assert(strcmp(draw_after(300, 0, 1, 2, 1, 200, 1), "g2lbdDcs") == 0 && DOTR_TIKEI_LEADER_ALPHA == 128);
    assert(after.set == 1022 && after.disp[4] == 128);
}

int main(void)
{
    check_draw_after();
    check_kumo();
    check_window();
    check_icon();
    return 0;
}
