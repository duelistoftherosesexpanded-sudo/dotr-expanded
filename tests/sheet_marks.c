/* Synthetic host tests for the sheet_marks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/sheet_marks.h"

struct DotrMarkFlags dotr_mark_flags_01ed537b;
struct DotrMarkItem data_0029abc0;
struct DotrMarkBoard data_00299e10_mark;

static int draws, draw_a0, draw_x, draw_y, draw_result;
static struct DotrMarkItem drawn;

int func_00217bd0_mark(int a0, struct DotrMarkItem *item, short x, short y)
{
    draws++;
    draw_a0 = a0;
    drawn = *item;
    draw_x = x;
    draw_y = y;
    return draw_result;
}

static int run(int corner, int frame, int state, int stage, int mode, int ticks)
{
    dotr_mark_flags_01ed537b.corner = (signed char)corner;
    dotr_mark_flags_01ed537b.frame = (unsigned char)frame;
    dotr_mark_flags_01ed537b.state = (signed char)state;
    dotr_mark_flags_01ed537b.stage = (signed char)stage;
    data_00299e10_mark.mode = (signed char)mode;
    data_00299e10_mark.ticks = (short)ticks;
    draws = 0;
    return Taku_Camp_BOTAN_MATI(0x1000);
}

static void check_mark(void)
{
    static const int corner_x[4] = {460, 600, 600, 460};
    static const int corner_y[4] = {104, 408, 408, 104};
    static const struct { int frame, row; } rows[] = {
        {0, 152}, {4, 152}, {5, 153}, {9, 153}, {10, 154}, {14, 154}, {15, 152}, {255, 152},
    };
    static const struct { int mode, ticks, shown; } boards[] = {
        {0, 100, 0}, {1, 105, 0}, {1, 106, 0}, {0, 105, 1}, {0, 106, 1}, {1, 100, 1}, {2, 100, 1},
    };
    size_t i;

    data_0029abc0.x0 = 1;
    data_0029abc0.y1 = 4;
    data_0029abc0.color[3] = 0x80;
    data_0029abc0.unknown_22 = 7;
    data_0029abc0.row = 99;
    draw_result = 0x2000;

    /* Each corner; the template is copied and only its row replaced; the frame byte counts up
     * and the callee's result is returned. */
    for (i = 0; i < 4; i++) {
        assert(run((int)i, 0, 4, 0, 0, 0) == 0x2000 && draws == 1 && draw_a0 == 0x1000);
        assert(draw_x == corner_x[i] && draw_y == corner_y[i]);
        assert(drawn.x0 == 1 && drawn.y1 == 4 && drawn.color[3] == 0x80 && drawn.unknown_22 == 7);
        assert(drawn.row == 152 && dotr_mark_flags_01ed537b.frame == 1);
        assert(data_0029abc0.row == 99);
    }
    /* Row by frame % 15, at each boundary; 255 wraps to 0 after the draw. */
    for (i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
        assert(run(0, rows[i].frame, 4, 0, 0, 0) == 0x2000 && drawn.row == rows[i].row);
        assert(dotr_mark_flags_01ed537b.frame == (unsigned char)(rows[i].frame + 1));
    }
    /* The three (mode, ticks) pairs that hide it, and neighbouring values. */
    for (i = 0; i < sizeof(boards) / sizeof(boards[0]); i++) {
        int got = run(0, 3, 4, 0, boards[i].mode, boards[i].ticks);
        assert(draws == boards[i].shown && got == (boards[i].shown ? 0x2000 : 0x1000));
        assert(dotr_mark_flags_01ed537b.frame == 3 + boards[i].shown);
    }
    /* State 4 or stage 2 is needed. */
    assert(run(0, 3, 3, 2, 0, 0) == 0x2000 && draws == 1);
    assert(run(0, 3, 4, 2, 0, 0) == 0x2000 && draws == 1);
    assert(run(0, 3, 3, 1, 0, 0) == 0x1000 && draws == 0 && dotr_mark_flags_01ed537b.frame == 3);
    /* Corner outside 0-3 with nothing drawn (x and y are unset on that path, so the drawing
     * case is not run on the host). */
    assert(run(4, 3, 0, 0, 0, 0) == 0x1000 && draws == 0);
    assert(run(-1, 3, 0, 0, 0, 0) == 0x1000 && draws == 0);
}

int main(void)
{
    check_mark();
    return 0;
}
