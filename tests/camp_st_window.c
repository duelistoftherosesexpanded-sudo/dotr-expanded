/* Synthetic host tests for the camp_st_window group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_st_window.h"

union DotrStWindowWork dotr_camp_st_window_work;
struct DotrStWindowBoard data_00299e10_stwindow;
struct DotrStWindowRec data_00298ba0_stwindow[22];
struct DotrStWindowWindow camp_window_My_data_stwindow[17];
struct DotrStWindowItem tex_bara_data_stwindow[2];
struct DotrStWindowItem tex_deck_data_stwindow[4];
struct DotrStWindowItem tex_batti_data_stwindow[4];
struct DotrStWindowItem tex_hosi_data_stwindow[3];
struct DotrStWindowItem camp_card_stwindow[2];
struct DotrStWindowItem camp_window_data_stwindow[4];
char data_0030f0f7_stwindow[] = "F0F7";
char data_0030f0fc_stwindow[] = "F0FC";

#define WB(o) (*(signed char *)(dotr_camp_st_window_work.bytes + (o)))
#define WH(o) (*(short *)(dotr_camp_st_window_work.bytes + (o)))
#define board data_00299e10_stwindow
#define windows camp_window_My_data_stwindow

/* What was called, one word per call, in order. */
static char seen[4096];
/* Every cursor-passing callee must get the previous one's result. */
static int cur;
static int deck, cost, leader, rank, width;
static struct DotrStWindowInfo info_entry;

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

int Taku_Window_Set(int cursor, struct DotrStWindowWindow *window, unsigned short flag, int a3,
                    unsigned short x, unsigned short a5, int a6, int a7, int a8)
{
    assert(window >= windows && window < windows + 17);
    said("win%d(%u,%d,%u,%u,%d,%d,%d) ", (int)(window - windows), flag, a3, x, a5, a6, a7, a8);
    return next(cursor);
}
int Taku_Obj_Set(int cursor, struct DotrStWindowItem *item, short x, short y)
{
    said("obj%d(%d,%d) ", item->code, x, y);
    return next(cursor);
}
int SzData_GetCurrentDeck(int side)
{
    assert(side == 0);
    said("deck ");
    return deck;
}
int SzData_GetDeckCost(int side, int list)
{
    assert(side == 0 && list == deck);
    said("cost ");
    return cost;
}
int SzData_GetLeader(int side, int d)
{
    assert(side == 0 && d == deck);
    said("leader ");
    return leader;
}
int SzData_GetRank(int side, int id)
{
    assert(side == 0 && id == leader);
    said("rank ");
    return rank;
}
int SzDuel_GetMsgName(int index)
{
    assert(index == leader);
    said("msgname ");
    return 300;
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
struct DotrStWindowInfo *SzMsg_GetInfo(int index)
{
    assert(index == 1);
    said("info ");
    info_entry.w = width;
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
int SzDuel_DrawWindow_FontDigit_Pipe(int cursor, int x, int y, int a, int flags, int w, int value)
{
    said("digit(%d,%d,%d,%d,%d,%d) ", x, y, a, flags, w, value);
    return next(cursor);
}
int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int a, int flags, int n, const char *text, int c)
{
    said("text(%d,%d,%d,%d,%d,%s,%d) ", x, y, a, flags, n, text, c);
    return next(cursor);
}

static void reset(void)
{
    int i;

    memset(&dotr_camp_st_window_work, 0, sizeof dotr_camp_st_window_work);
    memset(&board, 0, sizeof board);
    for (i = 0; i < 22; i++) {
        data_00298ba0_stwindow[i].kind = 8;
    }
    for (i = 0; i < 17; i++) {
        windows[i].x = (short)(10 * i + 1);
        windows[i].y = (short)(10 * i + 2);
    }
    for (i = 0; i < 2; i++) {
        tex_bara_data_stwindow[i].code = (short)(100 + i);
        camp_card_stwindow[i].code = (short)(500 + i);
    }
    for (i = 0; i < 4; i++) {
        tex_deck_data_stwindow[i].code = (short)(200 + i);
        tex_batti_data_stwindow[i].code = (short)(300 + i);
        camp_window_data_stwindow[i].code = (short)(600 + i);
    }
    for (i = 0; i < 3; i++) {
        tex_hosi_data_stwindow[i].code = (short)(400 + i);
    }
    WH(0x226) = 33;
    board.wins[0] = 41;
    board.wins[1] = 42;
    deck = 2;
    cost = 1234;
    leader = 555;
    rank = 0;
    width = 100;
}

static int run(void)
{
    seen[0] = 0;
    cur = 1000;
    return Taku_Camp_ST_WINDOW(1000);
}
static int has(const char *text)
{
    return strstr(seen, text) != NULL;
}
/* Number of non-overlapping occurrences. */
static int count(const char *text)
{
    const char *p = seen;
    int n = 0;

    while ((p = strstr(p, text)) != NULL) {
        n++;
        p += strlen(text);
    }
    return n;
}
/* The three side windows as drawn with flag `flag` at y. */
static void sides(char *out, size_t size, int side, int flag, int y)
{
    snprintf(out, size, "obj%d(0,%d) win0(%d,0,0,%d,0,128,33) win1(%d,0,0,%d,0,128,33) win2(%d,0,0,%d,0,128,33) ",
             100 + side, y, flag, y, flag, y, flag, y);
}

/* The whole sequence once, exactly: switch off (0x1ed018c clear), side 0, rank 0, no
 * location counted, slide 0 (y 128), leader name narrower than 248. */
static void check_sequence(void)
{
    char want[2048];
    int n, y = 128;

    reset();
    assert(run() == 1000 + 7 * 29);
    sides(want, sizeof want, 0, 0, y);
    n = (int)strlen(want);
    n += snprintf(want + n, sizeof want - n, "win16(0,0,0,%d,0,128,33) deck obj202(0,%d) cost leader rank ", y, y);
    /* j 0 counts nothing; j 1 draws eight marks 16 apart. */
    n += snprintf(want + n, sizeof want - n,
                  "obj501(0,%d) obj501(16,%d) obj501(32,%d) obj501(48,%d) obj501(64,%d) obj501(80,%d) obj501(96,%d) obj501(112,%d) ",
                  y, y, y, y, y, y, y, y);
    n += snprintf(want + n, sizeof want - n, "obj600(0,%d) obj601(0,%d) obj602(0,%d) obj603(0,%d) ", y, y, y, y);
    n += snprintf(want + n, sizeof want - n,
                  "digit(157,%d,1,0,4,1234) digit(253,%d,1,0,4,41) digit(349,%d,1,0,4,42) "
                  "text(411,%d,1,1,4,F0F7,0) text(450,%d,1,1,5,F0FC,0) ",
                  y + 384, y + 384, y + 384, y + 375, y + 375);
    n += snprintf(want + n, sizeof want - n,
                  "font(1,3,0,22) flush ratio(54,%d,1.000000) free msgname font(1,300,0,22) flush info "
                  "ratio(25,%d,1.000000) free ",
                  y + 348, 22 + 2 + y);
    assert(strcmp(seen, want) == 0);
    assert(WH(0x5442) == 1234 && board.mode == 0);
    /* The cost is stored as a halfword. */
    reset();
    cost = 0x12345;
    run();
    assert(WH(0x5442) == 0x2345);
}

/* y = 128 - (halfword 0x1ed0070 * 128 >> 8), arithmetic shift. */
static void check_slide(void)
{
    static const short slides[][2] = { { 256, 0 }, { 64, 96 }, { -64, 160 }, { -1, 129 }, { 1, 128 } };
    char want[256];
    size_t i;

    for (i = 0; i < sizeof slides / sizeof slides[0]; i++) {
        reset();
        WH(0x70) = slides[i][0];
        run();
        sides(want, sizeof want, 0, 0, slides[i][1]);
        assert(strncmp(seen, want, strlen(want)) == 0);
        snprintf(want, sizeof want, "digit(157,%d,", slides[i][1] + 384);
        assert(has(want));
    }
}

/* With 0x1ed018c set the side windows take flag side + 10 when the side equals t, and the
 * side flips afterwards when the halfword 0x1ed006c is 256. t: location 11 -> 1 when the
 * byte 0x1ed53c2 is below 2, else 0; other locations -> 0 when their record's halfword at
 * +16 is 8, else 1. */
static void check_side(void)
{
    /* 0x1ed018c, location, record kind, 0x1ed53c2, side, 0x1ed006c -> flag, side after. */
    static const short cases[][8] = {
        { 0, 11, 8, 0, 1, 256, 1, 1 },     /* switch off: plain, no flip */
        { 0, 3, 7, 0, 1, 256, 1, 1 },
        { 1, 11, 8, 1, 1, 256, 11, 0 },    /* t 1 (0x1ed53c2 1), match, flip */
        { 1, 11, 8, -5, 1, 0, 11, 1 },     /* t 1, match, no flip (not 256) */
        { 1, 11, 8, 2, 1, 256, 1, 1 },     /* t 0 at the boundary: no match */
        { 1, 11, 8, 2, 0, 256, 10, 1 },    /* t 0, side 0: match, flip */
        { 1, 3, 8, 0, 0, 255, 10, 0 },     /* kind 8: t 0, match, 255 does not flip */
        { 1, 3, 8, 0, 0, 257, 10, 0 },
        { 1, 3, 8, 0, 1, 256, 1, 1 },      /* kind 8, side 1: no match */
        { 1, 4, 7, 0, 0, 256, 0, 0 },      /* kind 7: t 1, side 0: no match */
        { 1, 4, 9, 5, 1, 256, 11, 0 },     /* kind 9: t 1, match, flip (0x1ed53c2 ignored) */
        { -1, 11, 8, 5, 0, 256, 10, 1 },   /* any non-zero switch byte */
    };
    char want[256];
    size_t i;
    int after;

    for (i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        reset();
        WB(0x18c) = (signed char)cases[i][0];
        board.kind = (signed char)cases[i][1];
        data_00298ba0_stwindow[cases[i][1]].kind = cases[i][2];
        WB(0x53c2) = (signed char)cases[i][3];
        board.mode = (signed char)cases[i][4];
        WH(0x6c) = cases[i][5];
        run();
        sides(want, sizeof want, cases[i][4], cases[i][6], 128);
        assert(strncmp(seen, want, strlen(want)) == 0);
        after = cases[i][7];
        assert(board.mode == after);
        /* Everything after the side windows uses the side as it is after the flip. */
        snprintf(want, sizeof want, "win2(%d,0,0,128,0,128,33) win16(%d,0,0,128,0,128,33) ", cases[i][6], after);
        assert(has(want));
        assert(count(after ? "obj500(" : "obj501(") == 8);
    }
}

/* Rank 0 draws no rank marks; else template (rank - 1) / 3 and (rank - 1) % 3 + 1 marks:
 * one (mark 2), two (marks 0, 1) or three (marks 0, 1, 2). */
static void check_rank(void)
{
    static const char *const marks[] = {
        NULL,
        "rank obj300(0,128) obj402(0,128) obj501(",
        "rank obj300(0,128) obj400(0,128) obj401(0,128) obj501(",
        "rank obj300(0,128) obj400(0,128) obj401(0,128) obj402(0,128) obj501(",
        "rank obj301(0,128) obj402(0,128) obj501(",
        "rank obj301(0,128) obj400(0,128) obj401(0,128) obj501(",
        "rank obj301(0,128) obj400(0,128) obj401(0,128) obj402(0,128) obj501(",
        "rank obj302(0,128) obj402(0,128) obj501(",
        "rank obj302(0,128) obj400(0,128) obj401(0,128) obj501(",
        "rank obj302(0,128) obj400(0,128) obj401(0,128) obj402(0,128) obj501(",
        "rank obj303(0,128) obj402(0,128) obj501(",
    };
    int i;

    reset();
    run();
    assert(has("rank obj501(") && !has("obj30") && !has("obj40"));
    for (i = 1; i <= 10; i++) {
        reset();
        rank = i;
        run();
        assert(has(marks[i]));
    }
}

/* Side 0: location bytes 2, 4, 5, 6, 8, 9, 10 and 11 count when at least 3 (mark 0 per
 * count); side 1: bytes 0, 12, 13, 14, 18, 19, 20 and 21 count when exactly 3 (mark 1 per
 * count). The other row always gets eight marks. Bytes 22 and above are never read. */
static void check_counts(void)
{
    static const int side0[] = { 2, 4, 5, 6, 8, 9, 10, 11 };
    static const int side1[] = { 0, 12, 13, 14, 18, 19, 20, 21 };
    char want[64];
    int i, k;

    /* Side 0: every listed byte at 3, the others at 3 too: eight. */
    reset();
    memset(board.seen, 3, sizeof board.seen);
    run();
    assert(count("obj500(") == 8 && count("obj501(") == 8);
    assert(has("obj500(112,128) obj501(0,128)"));
    /* Only listed bytes count: unlisted ones at 3 give none. */
    reset();
    memset(board.seen, 3, sizeof board.seen);
    for (i = 0; i < 8; i++) {
        board.seen[side0[i]] = 0;
    }
    run();
    assert(count("obj500(") == 0 && count("obj501(") == 8);
    /* Each listed byte alone, at 2 (no), 3 (yes), 127 (yes) and -1 (no). */
    for (i = 0; i < 8; i++) {
        static const signed char values[4] = { 2, 3, 127, -1 };
        static const int counted[4] = { 0, 1, 1, 0 };
        for (k = 0; k < 4; k++) {
            reset();
            board.seen[side0[i]] = values[k];
            run();
            assert(count("obj500(") == counted[k]);
        }
    }
    /* Five counted: x 0..64. */
    reset();
    board.seen[2] = 3;
    board.seen[5] = 4;
    board.seen[9] = 9;
    board.seen[10] = 3;
    board.seen[11] = 100;
    run();
    assert(has("obj600") && count("obj500(") == 5 && has("obj500(64,128) obj501(0,128)"));

    /* Side 1: the first row has eight marks, the second counts. */
    reset();
    board.mode = 1;
    memset(board.seen, 3, sizeof board.seen);
    run();
    assert(count("obj500(") == 8 && count("obj501(") == 8);
    assert(has("obj500(112,128) obj501(0,128)"));
    reset();
    board.mode = 1;
    memset(board.seen, 3, sizeof board.seen);
    for (i = 0; i < 8; i++) {
        board.seen[side1[i]] = 0;
    }
    run();
    assert(count("obj501(") == 0 && count("obj500(") == 8);
    for (i = 0; i < 8; i++) {
        static const signed char values[3] = { 2, 3, 4 };
        static const int counted[3] = { 0, 1, 0 };
        for (k = 0; k < 3; k++) {
            reset();
            board.mode = 1;
            board.seen[side1[i]] = values[k];
            run();
            assert(count("obj501(") == counted[k]);
        }
    }
    /* Three counted (byte 15 at 3 is not listed): x 0..32. */
    reset();
    board.mode = 1;
    board.seen[0] = 3;
    board.seen[14] = 3;
    board.seen[21] = 3;
    board.seen[15] = 3;
    run();
    snprintf(want, sizeof want, "obj501(32,128) obj600(0,128)");
    assert(count("obj501(") == 3 && has(want));
}

/* The leader name: full width below 248 dots, else squeezed to 248 / width (248 itself
 * takes the second branch, whose ratio is then also 1). Placed at window 2's x + 4 and
 * y + 2 + panel y. */
static void check_name(void)
{
    static const int widths[][2] = { { 0, 1000000 }, { 247, 1000000 }, { 248, 1000000 }, { 496, 500000 }, { 310, 800000 }, { -5, 1000000 } };
    char want[64];
    size_t i;

    for (i = 0; i < sizeof widths / sizeof widths[0]; i++) {
        reset();
        windows[2].x = -10;
        windows[2].y = 70;
        WH(0x70) = 64;
        width = widths[i][0];
        run();
        snprintf(want, sizeof want, "info ratio(-6,168,%d.%06d) free ", widths[i][1] / 1000000, widths[i][1] % 1000000);
        assert(has(want));
        assert(strcmp(seen + strlen(seen) - strlen(want), want) == 0);
    }
}

int main(void)
{
    check_sequence();
    check_slide();
    check_side();
    check_rank();
    check_counts();
    check_name();
    puts("camp_st_window: ok");
    return 0;
}
