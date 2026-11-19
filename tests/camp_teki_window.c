/* Synthetic host tests for the camp_teki_window group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_teki_window.h"

union DotrTekiWindowWork dotr_camp_teki_window_work;
struct DotrTekiWindowBoard data_00299e10_tekiwindow;
struct DotrTekiWindowRec data_00298ba0_tekiwindow[32];
struct DotrTekiWindowNames data_0029aef0_tekiwindow;
struct DotrTekiWindowPair data_0029af50_tekiwindow;
struct DotrTekiWindowWindow camp_window_Teki_data_tekiwindow[16];
struct DotrTekiWindowWindow data_00297bca_tekiwindow;
struct DotrTekiWindowItem tex_bara_data2_tekiwindow[2];
struct DotrTekiWindowItem data_002983a0_tekiwindow[4];
struct DotrTekiWindowItem data_00298320_tekiwindow[3];
struct DotrTekiWindowItemU teki_syurui_data_tekiwindow;
struct DotrTekiWindowItem tex_batti_data2_tekiwindow[8];
struct DotrTekiWindowItem tex_hosi_data2_tekiwindow[3];
struct DotrTekiWindowItem data_00298b1c_tekiwindow;
unsigned short teki_syurui_tex_no_tekiwindow;
unsigned char data_002999cc_tekiwindow[32][4];
char data_0030f102_tekiwindow[] = "S102";
char data_0030f1e8_tekiwindow[] = "S1E8";
char data_0030f1ef_tekiwindow[] = "S1EF";

#define WB(o) (*(signed char *)(dotr_camp_teki_window_work.bytes + (o)))
#define WH(o) (*(short *)(dotr_camp_teki_window_work.bytes + (o)))
#define WU(o) (*(unsigned short *)(dotr_camp_teki_window_work.bytes + (o)))
#define FACE_AT ((struct DotrTekiWindowFace *)(void *)(dotr_camp_teki_window_work.bytes + 0x5424))
#define board data_00299e10_tekiwindow
#define windows camp_window_Teki_data_tekiwindow

static char names_text[22][8];

/* What was called, one word per call, in order. */
static char seen[2048];
/* Every cursor-passing callee must get the previous one's result. */
static int cur;
static int widths[4], width_n, cost, leader, rank, card_kind, length, use2;
static struct DotrTekiWindowInfo info_entry;
static struct DotrTekiWindowCard card;

static void said(const char *format, ...)
{
    va_list list;
    size_t used = strlen(seen);

    va_start(list, format);
    vsnprintf(seen + used, sizeof seen - used, format, list);
    va_end(list);
    assert(strlen(seen) + 1 < sizeof seen);
}
static int next(int cursor)
{
    assert(cursor == cur);
    cur = cursor + 7;
    return cur;
}
static int window_no(struct DotrTekiWindowWindow *w)
{
    if (w == &data_00297bca_tekiwindow) {
        return 99;
    }
    assert(w >= windows && w < windows + 16);
    return (int)(w - windows);
}

int Taku_Window_Set(int cursor, struct DotrTekiWindowWindow *window, unsigned short flag, int a3,
                    unsigned short x, unsigned short y, int a6, int a7, int a8)
{
    said("win%d(%u,%d,%u,%u,%d,%d,%d) ", window_no(window), flag, a3, x, y, a6, a7, a8);
    return next(cursor);
}
int Taku_Obj_Set(int cursor, struct DotrTekiWindowItem *item, short x, short y)
{
    said("obj%d(%d,%d) ", item->code, x, y);
    return next(cursor);
}
int Taku_FT4_Set(int cursor, struct DotrTekiWindowItem *item, short x, short y, int a4)
{
    said("ft4%d(%d,%d,%d) ", item->code, x, y, a4);
    return next(cursor);
}
int Taku_G4_Set(int cursor, struct DotrTekiWindowItem *item, short x, short y)
{
    said("g4%d(%d,%d) ", item->code, x, y);
    return next(cursor);
}
int SzMsg_LoadFont_Pipe(int cursor, int a0, int a1, int a2, int a3)
{
    said("font(%d,%d,%d,%d) ", a0, a1, a2, a3);
    return next(cursor);
}
int RhText_SetTextureFlush(int cursor)
{
    said("flush ");
    return next(cursor);
}
struct DotrTekiWindowInfo *SzMsg_GetInfo(int index)
{
    assert(index == 1 && width_n < 4);
    said("info ");
    info_entry.w = widths[width_n++];
    return &info_entry;
}
void SzMsg_Free(int index)
{
    assert(index == 1);
    said("free ");
}
void SzMsg_DrawRect_Pipe(void)
{
    assert(0);
}
int SzMsg_DrawRatioX_Pipe(int cursor, int index, int x, int y, float sx, float sy, void *e, void (*function)(void))
{
    static const unsigned char grey[4] = { 128, 128, 128, 128 };

    assert(index == 1 && function == SzMsg_DrawRect_Pipe && memcmp(e, grey, 4) == 0 && sy == 1.0f);
    said("ratio(%d,%d,%.6f) ", x, y, (double)sx);
    return next(cursor);
}
int ShFace_Request(struct DotrTekiWindowFace *req)
{
    assert(req == FACE_AT);
    said("face(%u,%u,%u,%u,%u,%u) ", req->id, req->no, req->kind, req->mode, req->x, req->y);
    return 0;
}
void ShFace_Close(struct DotrTekiWindowFace *req)
{
    assert(req == FACE_AT);
    said("close ");
}
void SzData_SetupFromConstDeck(int side, int deck)
{
    assert(side == 1);
    said("setup(%d) ", deck);
}
int SzData_GetDeckCost(int side, int list)
{
    assert(side == 1 && list == 0);
    said("cost ");
    return cost;
}
int SzData_GetLeader(int side, int deck)
{
    assert(side == 1 && deck == 0);
    said("leader ");
    return leader;
}
int SzData_GetRank(int side, int id)
{
    assert(side == 1 && id == leader);
    said("rank ");
    return rank;
}
int SzDuel_DrawWindow_FontDigit_Pipe(int cursor, int x, int y, int a, int flags, int width, int value)
{
    said("digit(%d,%d,%d,%d,%d,%d) ", x, y, a, flags, width, value);
    return next(cursor);
}
int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int a, int flags, int n, const char *text, int c)
{
    said("text(%d,%d,%d,%d,%d,%s,%d) ", x, y, a, flags, n, text, c);
    return next(cursor);
}
struct DotrTekiWindowCard *SzDuel_GetCardConstInfo(int id)
{
    assert(id == leader);
    said("card ");
    card.b0 = (unsigned char)card_kind;
    return &card;
}
int SzDuel_GetMsgName(int index)
{
    assert(index == leader);
    said("msgname ");
    return 300;
}
int SzEffect_IsUse2(int a, int b)
{
    assert(a == -1 && b == -1);
    said("use2 ");
    return use2;
}
int func_0019bef0(const char *string)
{
    assert(string == data_0029aef0_tekiwindow.s[board.b198]);
    said("len ");
    return length;
}

static void reset(void)
{
    int i;

    memset(&dotr_camp_teki_window_work, 0, sizeof dotr_camp_teki_window_work);
    memset(&board, 0, sizeof board);
    for (i = 0; i < 32; i++) {
        data_00298ba0_tekiwindow[i].h10 = 8;
    }
    for (i = 0; i < 22; i++) {
        snprintf(names_text[i], sizeof names_text[i], "N%d", i);
        data_0029aef0_tekiwindow.s[i] = names_text[i];
    }
    for (i = 0; i < 16; i++) {
        windows[i].x = (unsigned short)(10 * i + 1);
        windows[i].y = (unsigned short)(10 * i + 2);
        windows[i].w = (unsigned short)(10 * i + 3);
        windows[i].h = (unsigned short)(10 * i + 4);
    }
    for (i = 0; i < 2; i++) {
        tex_bara_data2_tekiwindow[i].code = (short)(1200 + i);
        data_0029af50_tekiwindow.it[i].code = (short)(5000 + i);
    }
    for (i = 0; i < 4; i++) {
        data_002983a0_tekiwindow[i].code = (short)(3400 + i);
    }
    for (i = 0; i < 3; i++) {
        data_00298320_tekiwindow[i].code = (short)(3200 + i);
        tex_hosi_data2_tekiwindow[i].code = (short)(7300 + i);
    }
    for (i = 0; i < 8; i++) {
        tex_batti_data2_tekiwindow[i].code = (short)(7000 + i);
    }
    for (i = 0; i < 32; i++) {
        data_002999cc_tekiwindow[i][0] = (unsigned char)(i);
        data_002999cc_tekiwindow[i][1] = (unsigned char)(100 + i);
    }
    data_00298b1c_tekiwindow.code = 2100;
    data_00298b1c_tekiwindow.w[0] = 60;
    data_00298b1c_tekiwindow.w[1] = 70;
    data_00298b1c_tekiwindow.w[2] = 200;
    teki_syurui_data_tekiwindow.code = 0;
    teki_syurui_tex_no_tekiwindow = 0xfedc;
    widths[0] = widths[1] = widths[2] = widths[3] = 100;
    width_n = 0;
    cost = 1234;
    leader = 555;
    rank = 0;
    card_kind = 0;
    length = 5;
    use2 = 0;
}

static int run(void)
{
    seen[0] = 0;
    cur = 1000;
    width_n = 0;
    return Taku_Camp_TEKI_WINDOW(1000);
}
static int has(const char *text)
{
    return strstr(seen, text) != NULL;
}

/* The tooltip's conditions, all met. */
static void tooltip_on(void)
{
    WB(0x5374) = 0;
    WU(4) = 2;
    WB(0x72) = 0;
    WB(0x547a) = 0;
    WB(0x6e) = 1;
    WB(0x5481) = 1;
    use2 = 0;
    WB(0x5376) = 1;
    WB(0x5482) = 0;
}

static void check_names(void)
{
    static const short names[23][3] = {
        { 0, 215, 237 }, { 1, 196, 240 }, { 2, 198, 220 }, { 3, 205, 227 }, { 4, 204, 226 }, { 5, 200, 222 },
        { 6, 203, 225 }, { 7, 206, 228 }, { 8, 199, 221 }, { 9, 202, 224 }, { 10, 201, 223 }, { 11, 197, 219 },
        { 12, 207, 229 }, { 13, 208, 230 }, { 14, 209, 231 }, { 15, 196, 241 }, { 16, 210, 232 }, { 17, 196, 242 },
        { 18, 211, 233 }, { 19, 212, 234 }, { 20, 213, 235 }, { 21, 214, 236 }, { 25, 196, 218 },
    };
    /* Location 11: byte 0x1ed018c, byte 0x1ed53c2, side, location byte 11 -> name, message. */
    static const short eleven[][6] = {
        { 1, 2, 0, 0, 217, 239 }, { 1, 2, 1, 5, 217, 239 }, { 1, 1, 0, 0, 216, 238 }, { 1, 0, 1, 0, 197, 219 },
        { 1, 3, 0, 0, 197, 219 }, { -1, 1, 1, 0, 216, 238 },
        { 0, 1, 0, 3, 216, 238 }, { 0, 1, 0, 9, 216, 238 }, { 0, 0, 0, 3, 197, 219 }, { 0, 2, 0, 3, 197, 219 },
        { 0, 1, 0, 2, 197, 219 }, { 0, 2, 0, 0, 197, 219 },
        { 0, 1, 1, 3, 217, 239 }, { 0, 0, 1, 0, 217, 239 },
    };
    size_t i;
    char want[48];

    for (i = 0; i < sizeof names / sizeof names[0]; i++) {
        reset();
        board.b198 = (signed char)names[i][0];
        board.h196 = 77;
        /* Panel hidden, no tooltip: only the name number is stored. */
        assert(run() == 1000 && seen[0] == 0 && board.h196 == names[i][1] - 196);
        /* The message number shows in the tooltip (not for a location past 21: the name
         * pointers would be read out of their 22). */
        if (names[i][0] > 21) {
            continue;
        }
        tooltip_on();
        run();
        snprintf(want, sizeof want, "use2 len font(1,%d,0,22) ", names[i][2]);
        assert(strncmp(seen, want, strlen(want)) == 0 && board.h196 == names[i][1] - 196);
    }
    for (i = 0; i < sizeof eleven / sizeof eleven[0]; i++) {
        reset();
        tooltip_on();
        board.b198 = 11;
        WB(0x18c) = (signed char)eleven[i][0];
        WB(0x53c2) = (signed char)eleven[i][1];
        board.b0 = (signed char)eleven[i][2];
        board.lv[11] = (signed char)eleven[i][3];
        run();
        snprintf(want, sizeof want, "use2 len font(1,%d,0,22) ", eleven[i][5]);
        assert(strncmp(seen, want, strlen(want)) == 0 && board.h196 == eleven[i][4] - 196);
    }
}

/* Runs with the panel shown (y 44, slide 16) and says what was drawn. */
static void panel(int *disp, int *sel)
{
    WH(0x6c) = 300;
    WH(0x68) = 1;
    WB(0x5488) = 0;
    run();
    *disp = has("obj120");
    *sel = has("leader ");
    assert(*sel == (WB(0x5488) == 1));
}

static void check_shown(void)
{
    /* b0, 0x1ed018c, location, its byte, 0x1ed53ab, 0x1ed53c2, 0x1ed537a, 0x1ed537b,
     * 0x1ed5440 -> drawn, selected. */
    static const signed char rows[][11] = {
        /* Both sides done: all but 1, 15 and 17 are drawn and selected. */
        { 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0 }, { 1, 1, 15, 3, 0, 0, 0, 0, 0, 0, 0 }, { 0, 1, 17, 3, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 2, 0, 1, 0, 0, 0, 0, 1, 1 }, { 1, 1, 11, -1, 0, 0, 0, 0, 0, 1, 1 }, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
        /* Side 0. */
        { 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 11, -1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 2, 1, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 1 }, { 0, 0, 2, 3, 0, 0, 0, 0, 0, 1, 1 }, { 0, 0, 2, 4, 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 2, 5, 0, 0, 0, 0, 0, 1, 1 }, { 0, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 2, -1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 2, 5, 1, 0, 0, 0, 0, 1, 0 }, { 0, 0, 2, 2, -1, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 11, 1, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 11, 3, 0, 1, 0, 0, 0, 1, 0 }, { 0, 0, 11, 4, 0, 1, 0, 0, 0, 1, 1 }, { 0, 0, 11, 3, 0, 2, 0, 0, 0, 1, 1 },
        { 0, 0, 11, 3, 1, 0, 0, 0, 0, 1, 0 }, { 0, 0, 5, 3, 0, 1, 0, 0, 0, 1, 1 },
        /* Side 1. */
        { 1, 0, 15, 3, 0, 0, 0, 0, 0, 0, 0 }, { 1, 0, 17, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0 }, { 1, 0, 1, 2, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0 }, { 1, 0, 11, 2, 0, 1, 0, 0, 0, 1, 1 },
        { 1, 0, 3, 2, 0, 0, 0, 0, 0, 1, 1 }, { 1, 0, 3, 3, 0, 0, 0, 0, 0, 1, 1 }, { 1, 0, 3, 3, 1, 0, 0, 0, 0, 1, 0 },
        { 1, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0 }, { 1, 0, 3, 5, 0, 0, 0, 0, 0, 0, 0 }, { 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* The two overrides. */
        { 0, 0, 0, 0, 0, 0, 0, 1, 100, 1, 1 }, { 1, 1, 15, 0, 0, 0, 0, -1, 100, 1, 1 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 99, 0, 0 }, { 0, 0, 0, 0, 0, 0, 1, 1, 99, 1, 1 }, { 0, 0, 0, 0, 0, 0, 1, 1, -5, 1, 1 },
        { 0, 0, 0, 0, 0, 0, 1, 0, 50, 0, 0 }, { 0, 0, 0, 0, 0, 0, 1, 1, 101, 0, 0 }, { 0, 0, 0, 0, 0, 0, 1, 0, 100, 0, 0 },
    };
    size_t i;
    int disp, sel;

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        reset();
        board.b0 = rows[i][0];
        WB(0x18c) = rows[i][1];
        board.b198 = rows[i][2];
        board.lv[(int)rows[i][2]] = rows[i][3];
        WB(0x53ab) = rows[i][4];
        WB(0x53c2) = rows[i][5];
        WB(0x537a) = rows[i][6];
        WB(0x537b) = rows[i][7];
        WB(0x5440) = rows[i][8];
        WB(0x5406) = 0;
        panel(&disp, &sel);
        assert(disp == rows[i][9] && sel == rows[i][10]);
        /* The deck state: -1 unless selected (then 1 from 0); 1 with 0x537a and 100 %. */
        if (rows[i][6] != 0 && rows[i][8] == 100) {
            assert(WB(0x5406) == 1);
        } else {
            assert(WB(0x5406) == (sel ? 1 : -1));
        }
    }

    /* Hidden panel (halfword 0x6c zero): nothing of it, the deck state untouched. */
    reset();
    board.b198 = 2;
    board.lv[2] = 3;
    WB(0x537b) = 1;
    WB(0x5440) = 100;
    WB(0x5406) = 0;
    assert(run() == 1000 && seen[0] == 0 && WB(0x5406) == 0);
}

/* The panel's side: location record +16 == 8 gives side 0; location 11 the other side's. */
static void check_side(void)
{
    /* 0x1ed018c, 0x1ed53c2, b0, location byte 11 -> side */
    static const signed char eleven[][5] = {
        { 1, 2, 0, 0, 0 }, { 1, 2, 1, 0, 0 }, { 1, 1, 0, 0, 1 }, { 1, 0, 0, 0, 1 }, { 1, 1, 1, 0, 1 },
        { 0, 0, 0, 3, 1 }, { 0, 2, 0, 3, 1 }, { 0, 0, 1, 2, 0 }, { 0, 2, 1, 2, 0 },
    };
    size_t i;
    int disp, sel;

    reset();
    board.b198 = 4;
    board.lv[4] = 1;
    panel(&disp, &sel);
    assert(has("obj1200(") && has("ft43400(") && has("win0(0,"));
    reset();
    board.b198 = 4;
    board.lv[4] = 1;
    data_00298ba0_tekiwindow[4].h10 = 9;
    panel(&disp, &sel);
    assert(has("obj1201(") && has("ft43401(") && has("win0(1,"));
    /* With both sides done, the record still decides for other locations. */
    reset();
    WB(0x18c) = 1;
    board.b198 = 4;
    data_00298ba0_tekiwindow[4].h10 = 0;
    panel(&disp, &sel);
    assert(has("obj1201(") && has("win1(1,") && has("win99(1,"));
    for (i = 0; i < sizeof eleven / sizeof eleven[0]; i++) {
        char want[32];

        reset();
        WB(0x18c) = eleven[i][0];
        WB(0x53c2) = eleven[i][1];
        board.b0 = eleven[i][2];
        board.b198 = 11;
        board.lv[11] = eleven[i][3];
        data_00298ba0_tekiwindow[11].h10 = (short)(eleven[i][4] ? 8 : 9);    /* not consulted */
        panel(&disp, &sel);
        assert(disp);
        snprintf(want, sizeof want, "obj%d(", 1200 + eleven[i][4]);
        assert(has(want));
    }
}

static void check_drawn(void)
{
    static const struct {
        int state;
        const char *face;
        int after;
    } faces[] = {
        { 0, "", 0 }, { 3, "", 3 }, { -1, "", -1 },
        { 1, "face(255,3,103,1,31,60) ", 2 }, { 2, "face(0,3,103,1,31,60) ", 2 },
        { 10, "face(0,3,103,3,31,60) ", 2 }, { 100, "close ", 0 },
    };
    size_t i;
    char want[600];

    /* Side 0, location 2 (name 198, number 2), drawn but not selected. */
    for (i = 0; i < sizeof faces / sizeof faces[0]; i++) {
        reset();
        WH(0x6c) = 300;
        WH(0x68) = 1;
        WH(0x224) = 77;
        board.b198 = 2;
        board.lv[2] = 1;
        WB(0x540a) = (signed char)faces[i].state;
        WB(0x5468) = 1;
        WB(0x5406) = 0;
        assert(run() == cur);
        snprintf(want, sizeof want, "obj1200(0,44) win0(0,0,0,44,0,128,77) win2(0,0,0,44,0,128,77) "
                 "win3(0,0,0,28,1,128,0) %sft43402(0,28,0) ft43403(0,28,0) ft43400(0,28,0) setup(28) cost "
                 "obj3200(0,44) digit(410,70,0,0,4,1234) font(1,198,0,22) flush info ratio(53,65,1.000000) free ",
                 faces[i].face);
        assert(strcmp(seen, want) == 0);
        assert(WB(0x540a) == faces[i].after && WH(0x5444) == 1234 && WB(0x5406) == -1 && WB(0x5488) == 0);
        assert(board.h196 == 2);
    }
    /* The face request: name 217 uses the name number itself, the others number + 1; the
     * kind comes from the row's column 0x1ed5468; the y follows the panel. */
    reset();
    WH(0x6c) = 256 + 100;
    WH(0x68) = 2;
    WB(0x18c) = 1;
    WB(0x53c2) = 2;
    board.b198 = 11;
    WB(0x540a) = 2;
    WB(0x5468) = 0;
    run();
    assert(board.h196 == 21 && has("face(0,21,21,1,31,100) "));
    WB(0x540a) = 10;
    WB(0x5468) = 1;
    WB(0x53c2) = 1;
    run();
    assert(board.h196 == 20 && has("face(0,21,121,3,31,100) ") && WB(0x540a) == 2);
    assert(FACE_AT->no == 21 && FACE_AT->kind == 121 && FACE_AT->mode == 3 && FACE_AT->x == 31 && FACE_AT->y == 100);

    /* A deck already set up is not set up again; a cost that does not fit 16 bits is cut. */
    reset();
    WH(0x6c) = 300;
    board.b198 = 2;
    board.lv[2] = 1;
    WB(0x5406) = 1;
    cost = 0x12345;
    run();
    assert(!has("setup") && has("cost obj3200(0,44) digit(410,70,0,0,4,9029) ") && WH(0x5444) == 0x2345);
    assert(WB(0x5406) == -1);
    /* A name wider than 324 dots is squeezed to it. */
    reset();
    WH(0x6c) = 300;
    board.b198 = 2;
    board.lv[2] = 1;
    widths[0] = 323;
    run();
    assert(has("ratio(53,65,1.000000) "));
    widths[0] = 324;
    run();
    assert(has("ratio(53,65,1.000000) "));
    widths[0] = 648;
    run();
    assert(has("ratio(53,65,0.500000) "));
}

static void check_selected(void)
{
    static const short codes[33] = {
        0, 78, 88, 93, 79, 83, 85, 86, 87, 81, 82, 89, 84, 90, 91, 92, 80, 94, 95, 96, 97, 101,
        -292, -292, -292, -292, -292, -292, -292, -292, -292, -292, -292,
    };
    static const struct {
        int rank;
        const char *marks;
    } ranks[] = {
        { 0, "" },
        { 1, "obj7000(-64,44) obj7302(-64,44) " },
        { 2, "obj7000(-64,44) obj7300(-64,44) obj7301(-64,44) " },
        { 3, "obj7000(-64,44) obj7300(-64,44) obj7301(-64,44) obj7302(-64,44) " },
        { 4, "obj7001(-64,44) obj7302(-64,44) " },
        { 6, "obj7001(-64,44) obj7300(-64,44) obj7301(-64,44) obj7302(-64,44) " },
        { 8, "obj7002(-64,44) obj7300(-64,44) obj7301(-64,44) " },
        { 22, "obj7007(-64,44) obj7302(-64,44) " },
    };
    int kind;
    size_t i;
    char want[900];

    /* Side 1, location 3 (name 205, number 9), selected; slide 10 % (x = -64). */
    for (i = 0; i < sizeof ranks / sizeof ranks[0]; i++) {
        reset();
        WH(0x6c) = 300;
        WH(0x68) = 1;
        WH(0x224) = 77;
        board.b0 = 1;
        board.b198 = 3;
        board.lv[3] = 2;
        board.h66[9] = 11;
        board.h130[9] = 22;
        data_00298ba0_tekiwindow[3].h10 = 5;
        WB(0x5440) = 10;
        WB(0x5406) = 5;
        card_kind = 0xe3;
        rank = ranks[i].rank;
        widths[0] = 400;
        assert(run() == cur);
        snprintf(want, sizeof want, "obj1201(0,44) win0(1,0,0,44,0,128,77) win2(1,0,0,44,0,128,77) "
                 "win3(1,0,0,28,1,128,0) ft43402(0,28,0) ft43403(0,28,0) ft43401(0,28,0) cost "
                 "obj3200(0,44) digit(410,70,0,0,4,1234) font(1,205,0,22) flush info ratio(53,65,0.810000) free "
                 "leader win1(1,0,65472,44,0,128,77) win4(1,0,65472,44,0,128,77) win99(1,0,65472,44,0,128,77) "
                 "text(88,107,1,1,4,S102,0) text(128,107,1,1,6,S1E8,0) card obj79(-64,44) rank %s"
                 "obj3201(-64,44) obj3202(-64,44) digit(260,117,1,0,4,11) digit(357,117,1,0,4,22) msgname "
                 "font(1,300,0,22) flush info ratio(-19,88,1.000000) free ", ranks[i].marks);
        assert(strcmp(seen, want) == 0);
        assert(WB(0x5488) == 1 && WB(0x5406) == 5 && teki_syurui_data_tekiwindow.code == 79);
    }
    /* The kind icon: low five bits + 1 through the table, the default from 0x297ea0. */
    for (kind = 0; kind < 32; kind++) {
        char tok[24];

        reset();
        WH(0x6c) = 300;
        WB(0x18c) = 1;
        board.b198 = 2;
        card_kind = kind | (kind & 1 ? 0x20 : 0x80);
        teki_syurui_data_tekiwindow.code = 1;
        run();
        snprintf(tok, sizeof tok, "card obj%d(0,44) ", codes[kind + 1]);
        assert(has(tok) && teki_syurui_data_tekiwindow.code == (unsigned short)codes[kind + 1]);
    }
    /* The leader's name squeezed to 246 dots. */
    reset();
    WH(0x6c) = 300;
    WB(0x18c) = 1;
    board.b198 = 2;
    widths[1] = 245;
    run();
    assert(has("msgname font(1,300,0,22) flush info ratio(45,88,1.000000) free "));
    widths[1] = 246;
    run();
    assert(has("info ratio(45,88,1.000000) free "));
    widths[1] = 492;
    run();
    assert(has("info ratio(45,88,0.500000) free "));
}

static void check_name_box(void)
{
    /* length, message width -> box width */
    static const int rows[][3] = {
        { 5, 40, 66 }, { 5, 49, 66 }, { 5, 50, 66 }, { 5, 51, 67 }, { 5, 100, 116 }, { 5, 101, 117 },
        { 5, 0x10064, 116 }, { 4000, 100, 116 }, { 0, 0, 16 }, { 30, 10, 316 },
    };
    size_t i;
    char want[200];

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        int wd = rows[i][2];
        int tx = 320 - (wd >> 1);

        reset();
        tooltip_on();
        WB(0x5472) = 1;
        board.b198 = 6;
        length = rows[i][0];
        widths[0] = rows[i][1];
        windows[7].w = 9;
        assert(run() == cur);
        snprintf(want, sizeof want, "len font(1,258,0,22) flush info win7(2,0,%d,224,0,128,64) ratio(%d,228,1.000000) free ",
                 tx, tx + 8);
        /* The box replaces the tooltip. */
        assert(strcmp(seen, want) == 0 && windows[7].w == wd);
    }
}

static void check_tooltip(void)
{
    /* cursor x, cursor y -> box x, box y (message width 84: box width 100) */
    static const int rows[][4] = {
        { 200, 150, 150, 182 }, { 30, 150, 0, 208 }, { 50, 150, 0, 208 }, { 51, 300, 1, 290 },
        { 430, 150, 380, 182 }, { 429, 150, 379, 182 }, { 600, 0, 380, 120 },
        { 100, 100, 50, 208 }, { 100, 176, 50, 208 }, { 100, 177, 50, 209 }, { 177, 0, 127, 208 },
        { 178, 0, 128, 120 }, { 300, 50, 250, 120 }, { 300, 89, 250, 121 }, { 300, 88, 250, 120 },
        { 300, 258, 250, 290 }, { 300, 257, 250, 289 }, { 100, 400, 50, 290 }, { 300, -100, 250, 120 },
    };
    size_t i;
    char want[200];

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        reset();
        tooltip_on();
        board.b198 = 7;
        WH(0) = (short)rows[i][0];
        WH(2) = (short)rows[i][1];
        widths[0] = 84;
        assert(run() == cur);
        snprintf(want, sizeof want, "use2 len font(1,228,0,22) flush info win5(2,0,%d,%d,0,128,64) ratio(%d,%d,1.000000) free ",
                 rows[i][2], rows[i][3], rows[i][2] + 8, rows[i][3] + 4);
        assert(strcmp(seen, want) == 0 && windows[5].w == 100);
    }
    /* An odd box width; the low half of a wide message width. */
    reset();
    tooltip_on();
    board.b198 = 7;
    WH(0) = 200;
    WH(2) = 150;
    widths[0] = 0x30055;
    run();
    assert(has("win5(2,0,150,182,0,128,64) ") && windows[5].w == 101);
    /* Once message 249 instead, and the request is cleared. */
    WB(0x5446) = 3;
    run();
    assert(has("len font(1,249,0,22) ") && WB(0x5446) == 0);
    run();
    assert(has("len font(1,228,0,22) "));

    /* Each condition alone keeps it off. */
    for (i = 0; i < 9; i++) {
        reset();
        tooltip_on();
        board.b198 = 7;
        switch (i) {
        case 0: WB(0x5374) = 1; break;
        case 1: WU(4) = 3; break;
        case 2: WB(0x72) = -1; break;
        case 3: WB(0x547a) = 1; break;
        case 4: WB(0x6e) = 0; break;
        case 5: WB(0x5481) = 0; break;
        case 6: use2 = 1; break;
        case 7: WB(0x5376) = 0; break;
        default: WB(0x5482) = 1; break;
        }
        assert(run() == 1000);
        /* The conditions are asked in order; SzEffect_IsUse2 is the seventh. */
        assert(strcmp(seen, i >= 6 ? "use2 " : "") == 0);
    }
}

static void check_extra_box(void)
{
    /* b0, location byte 11, 0x1ed018c, location -> drawn */
    static const signed char rows[][5] = {
        { 0, 3, 0, 11, 1 }, { 0, 9, 0, 11, 1 }, { 0, 2, 0, 11, 0 }, { 1, 3, 0, 11, 0 },
        { 1, 0, 1, 11, 1 }, { 0, 0, -1, 11, 1 }, { 0, 3, 0, 10, 0 }, { 1, 0, 1, 12, 0 },
    };
    size_t i;

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        reset();
        board.b0 = rows[i][0];
        board.lv[11] = rows[i][1];
        WB(0x18c) = rows[i][2];
        board.b198 = rows[i][3];
        WH(0x6c) = 0;
        WH(0x68) = 1;
        run();
        /* y = -256 with the panel hidden; slide 16. */
        assert(strcmp(seen, rows[i][4] ? "g42100(0,-272) text(100,-178,1,1,6,S1EF,0) obj5000(64,-178) obj5001(174,-178) " : "") == 0);
    }
    /* With the panel shown it comes last. */
    reset();
    board.lv[11] = 3;
    board.b198 = 11;
    WH(0x6c) = 300;
    WH(0x68) = 1;
    run();
    assert(has("free g42100(0,28) text(100,122,1,1,6,S1EF,0) obj5000(64,122) obj5001(174,122) "));
    assert(strcmp(seen + strlen(seen) - strlen("obj5001(174,122) "), "obj5001(174,122) ") == 0);
}

int main(void)
{
    assert(sizeof(struct DotrTekiWindowItem) == 38 && offsetof(struct DotrTekiWindowItem, code) == 36);
    assert(sizeof(struct DotrTekiWindowWindow) == 10 && sizeof(struct DotrTekiWindowFace) == 8);
    assert(offsetof(struct DotrTekiWindowBoard, h196) == 196 && offsetof(struct DotrTekiWindowBoard, b198) == 198);
    assert(sizeof(struct DotrTekiWindowRec) == 52 && offsetof(struct DotrTekiWindowRec, h10) == 16);
    assert(offsetof(struct DotrTekiWindowInfo, w) == 12);
    check_names();
    check_shown();
    check_side();
    check_drawn();
    check_selected();
    check_name_box();
    check_tooltip();
    check_extra_box();
    puts("camp_teki_window: ok");
    return 0;
}
