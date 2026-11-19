/* Synthetic host tests for the trade_scene group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/trade_scene.h"

struct DotrTradeControl data_00317490;
unsigned short data_0031811c;

/* Each stub appends a letter to the trace and returns cursor + 1; argument records are
 * indexed by side where a call can happen once per side. */
static char trace[64];
static int fade_n, fade[4][5], detail_n, detail[2][7], detail_side[2], ready_n, ready_x[2], cursor_row[2], freed;

static DotrTradeWord mark(DotrTradeWord cursor, char id)
{
    size_t n = strlen(trace);
    trace[n] = id;
    trace[n + 1] = 0;
    return cursor + 1;
}
DotrTradeWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrTradeWord cursor, int a)
{
    assert(a == 2);
    return mark(cursor, 'S');
}
DotrTradeWord lSzDuel_DrawTrade_Base_Pipe(DotrTradeWord cursor) { return mark(cursor, 'B'); }
DotrTradeWord lSzDuel_DrawTrade_Title_Pipe(DotrTradeWord cursor, int a, int b)
{
    assert(a == 0 && b == 0);
    return mark(cursor, 'T');
}
DotrTradeWord lSzDuel_DrawTrade_Bag_Pipe(DotrTradeWord cursor, int side, int a, int b)
{
    assert(a == 0 && b == 0);
    return mark(cursor, side ? 'g' : 'G');
}
DotrTradeWord lSzDuel_DrawTrade_Cursor_Pipe(DotrTradeWord cursor, int side, int a, int b, int row)
{
    assert(a == 0 && b == 0);
    cursor_row[side] = row;
    return mark(cursor, side ? 'c' : 'C');
}
DotrTradeWord lSzDuel_Draw_Fade_Pipe(DotrTradeWord cursor, int x, int y, int width, int height, int alpha)
{
    fade[fade_n][0] = x;
    fade[fade_n][1] = y;
    fade[fade_n][2] = width;
    fade[fade_n][3] = height;
    fade[fade_n][4] = alpha;
    fade_n++;
    return mark(cursor, 'F');
}
DotrTradeWord lSzDuel_DrawBuildDeck_Detail_Pipe(DotrTradeWord cursor, int card, int x, int a, int side, int b, int c, int d)
{
    detail[detail_n][0] = card;
    detail[detail_n][1] = x;
    detail[detail_n][2] = a;
    detail[detail_n][3] = side;
    detail[detail_n][4] = b;
    detail[detail_n][5] = c;
    detail[detail_n][6] = d;
    detail_side[detail_n] = data_00317490.side;
    detail_n++;
    return mark(cursor, 'D');
}
DotrTradeWord lSzDuel_DrawTrade_Ready_Pipe(DotrTradeWord cursor, int x, int a)
{
    assert(a == 0);
    ready_x[ready_n++] = x;
    return mark(cursor, 'R');
}
DotrTradeWord lSzDuel_DrawTrade_Exit_Pipe(DotrTradeWord cursor, int a, int b)
{
    assert(a == 0 && b == 0);
    return mark(cursor, 'X');
}
DotrTradeWord lSzDuel_DrawTrade_Do_Pipe(DotrTradeWord cursor, int a, int b)
{
    assert(a == 0 && b == 0);
    return mark(cursor, 'O');
}
DotrTradeWord lSzDuel_DrawTrade_Notice_Pipe(DotrTradeWord cursor, int a, int b)
{
    assert(a == 0 && b == 0);
    return mark(cursor, 'N');
}
void SzMsg_Free(int index)
{
    assert(index == 0);
    freed++;
}

static unsigned short left_list[8] = { 100, 101, 102, 103, 104, 105, 106, 107 };
static unsigned short right_list[8] = { 200, 201, 202, 203, 204, 205, 206, 207 };

static void reset(void)
{
    memset(&data_00317490, 0, sizeof(data_00317490));
    data_00317490.list[0] = left_list;
    data_00317490.list[1] = right_list;
    data_00317490.side = 9;
    data_0031811c = 0;
    trace[0] = 0;
    fade_n = detail_n = ready_n = freed = 0;
}

/* Runs the function and checks the trace, the chained cursor and the single free. */
static void run(const char *expected)
{
    assert(SzDuel_DrawTrade_Pipe(0x1000) == 0x1000 + (DotrTradeWord)strlen(expected));
    assert(strcmp(trace, expected) == 0 && freed == 1);
}

/* Branch coverage: each test in the function is taken both ways; modes 3 to 8 per side. */
static void check_trade(void)
{
    int mode;

    /* Nothing optional. */
    reset();
    run("SBTGg");
    assert(data_00317490.side == 9);

    /* Left cursor through the count symbol, right cursor through the block. */
    reset();
    data_0031811c = 1;
    data_00317490.cursor[0] = 3;
    run("SBTGCg");
    assert(cursor_row[0] == 3);
    reset();
    data_00317490.count1 = 2;
    data_00317490.cursor[1] = 5;
    run("SBTGgc");
    assert(cursor_row[1] == 5);

    /* Left slide: fade over the left half, side 0, card list[0][top0 + cursor]. */
    reset();
    data_00317490.slide[0] = 160;
    data_00317490.top0 = 2;
    data_00317490.cursor[0] = 3;
    run("SBTGFDg");
    assert(fade[0][0] == 0 && fade[0][1] == 0 && fade[0][2] == 320 && fade[0][3] == 448 && fade[0][4] == 32);
    assert(detail[0][0] == 105 && detail[0][1] == 160 && detail[0][2] == 0 && detail[0][3] == 0);
    assert(detail[0][4] == 1 && detail[0][5] == 1 && detail[0][6] == 0 && detail_side[0] == 0);
    /* Negative slide: the quotient is truncated to a byte (-10 * 64 / 320 = -2). */
    reset();
    data_00317490.slide[0] = -10;
    run("SBTGFDg");
    assert(fade[0][4] == 254 && detail[0][0] == 100 && detail[0][1] == -10);

    /* Right slide: fade over the right half, side 1, slide negated. */
    reset();
    data_00317490.slide[1] = 320;
    data_00317490.top1 = 1;
    data_00317490.cursor[1] = 4;
    run("SBTGgFD");
    assert(fade[0][0] == 320 && fade[0][1] == 0 && fade[0][2] == 320 && fade[0][3] == 448 && fade[0][4] == 64);
    assert(detail[0][0] == 205 && detail[0][1] == -320 && detail[0][2] == 0 && detail[0][3] == 1);
    assert(detail[0][4] == 1 && detail[0][5] == 1 && detail[0][6] == 0 && detail_side[0] == 1);
    assert(data_00317490.side == 1);

    /* Modes per side. */
    for (mode = 3; mode <= 8; mode++) {
        static const char *left[6] = { "SBTGg", "SBTGFRg", "SBTGgX", "SBTGFRgO", "SBTGFRg", "SBTGgN" };
        static const char *right[6] = { "SBTGg", "SBTGgFR", "SBTGgX", "SBTGgFRO", "SBTGgFR", "SBTGgN" };
        int ready = mode == 4 || mode == 6 || mode == 7;

        reset();
        data_00317490.mode[0] = mode;
        run(left[mode - 3]);
        assert(ready_n == ready && fade_n == ready);
        if (ready) {
            assert(ready_x[0] == -160 && fade[0][0] == 0 && fade[0][2] == 320 && fade[0][3] == 448 && fade[0][4] == 64);
        }
        reset();
        data_00317490.mode[1] = mode;
        run(right[mode - 3]);
        assert(ready_n == ready && fade_n == ready);
        if (ready) {
            assert(ready_x[0] == 160 && fade[0][0] == 320 && fade[0][2] == 320 && fade[0][3] == 448 && fade[0][4] == 64);
        }
    }
    /* Both sides in the same mode draw the shared prompt once. */
    reset();
    data_00317490.mode[0] = 5;
    data_00317490.mode[1] = 5;
    run("SBTGgX");
}

int main(void)
{
    check_trade();
    puts("trade_scene: ok");
    return 0;
}
