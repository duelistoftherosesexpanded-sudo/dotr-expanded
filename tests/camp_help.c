/* Synthetic host tests for the camp_help group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_help.h"

struct DotrHelpWindows data_0029b020;
signed char data_00299e10[1];
char data_0030f113[4];
signed char dotr_help_block_01ed0000[0x5470];

static struct {
    int n;
    DotrHelpWord cursor[4];
    struct DotrHelpWindow window[4];
    int flag[4], x[4], y[4], last[4];
} win;
static struct {
    int n;
    DotrHelpWord cursor;
    int y;
} font;
static struct {
    int n;
    DotrHelpWord cursor[5];
    int no[5], y[5];
    unsigned char col[5][4];
} text;

DotrHelpWord Taku_Window_Set(DotrHelpWord cursor, struct DotrHelpWindow *window,
                             unsigned short flag, int a3, unsigned short x, unsigned short y,
                             int a6, int a7, int a8)
{
    assert(win.n < 4 && a3 == 0 && a6 == 0 && a7 == 128);
    win.cursor[win.n] = cursor;
    win.window[win.n] = *window;
    win.flag[win.n] = flag;
    win.x[win.n] = x;
    win.y[win.n] = y;
    win.last[win.n++] = a8;
    return cursor + 1;
}

DotrHelpWord SzDuel_DrawWindow_Font_Pipe(DotrHelpWord cursor, int x, int y, int a, int flags,
                                         int n, const char *text_, int c)
{
    assert(x == 296 && a == 2 && flags == 1 && n == 4 && text_ == data_0030f113 && c == 0);
    font.cursor = cursor;
    font.y = y;
    font.n++;
    return cursor + 1;
}

DotrHelpWord Taku_Bunsyou_Set(DotrHelpWord cursor, int index, int no, int x, int y, int unused,
                              unsigned char *col, float sx, float sy)
{
    assert(text.n < 5 && index == 1 && x == 176 && unused == 0 && sx == 1.0f && sy == 1.0f);
    text.cursor[text.n] = cursor;
    text.no[text.n] = no;
    text.y[text.n] = y;
    memcpy(text.col[text.n++], col, 4);
    return cursor + 1;
}

static void reset(int open, int step, int pick)
{
    int i;

    memset(&win, 0, sizeof win);
    memset(&font, 0, sizeof font);
    memset(&text, 0, sizeof text);
    memset(dotr_help_block_01ed0000, 0, sizeof dotr_help_block_01ed0000);
    for (i = 0; i < 4; i++) {
        data_0029b020.w[i].x = (short)(10 + i);
        data_0029b020.w[i].y = (short)(20 + i);
        data_0029b020.w[i].w = (short)(30 + i);
        data_0029b020.w[i].h = (short)(40 + i);
    }
    data_00299e10[0] = 1;
    DOTR_HELP_OPEN = (signed char)open;
    DOTR_HELP_STEP = (signed char)step;
    DOTR_HELP_PICK = (signed char)pick;
}

/* Branch coverage: closed (no call), open with each selectable line, every switch
 * arm (the loop runs lines 0-4), both colour outcomes, the five-line bound. */
static void check_help_window(void)
{
    static const int order[4] = { 0, 1, 3, 2 };
    int pick, i;

    reset(0, 4, 2);
    assert(Taku_Camp_Help_Window(1000) == 1000);
    assert(win.n == 0 && font.n == 0 && text.n == 0);

    for (pick = 0; pick < 5; pick++) {
        reset(1, 4, pick);
        /* step 4: y = 512 - ((4 << 9) >> 3) = 256 */
        assert(Taku_Camp_Help_Window(1000) == 1010);
        assert(win.n == 4 && font.n == 1 && text.n == 5);
        for (i = 0; i < 4; i++) {
            assert(win.window[i].x == 10 + order[i] && win.window[i].h == 40 + order[i]);
        }
        assert(win.cursor[0] == 1000 && win.flag[0] == 2 && win.x[0] == 128 && win.y[0] == 336 && win.last[0] == 101);
        assert(win.cursor[1] == 1001 && win.flag[1] == 2 && win.x[1] == 128 && win.y[1] == 336 && win.last[1] == 101);
        assert(win.cursor[2] == 1002 && win.flag[2] == 11 && win.x[2] == 282 && win.y[2] == 350 && win.last[2] == 128);
        assert(font.cursor == 1003 && font.y == 358);
        assert(win.cursor[3] == 1004 && win.flag[3] == 2 && win.x[3] == 160 && win.y[3] == 400 + pick * 36 && win.last[3] == 0);
        for (i = 0; i < 5; i++) {
            int c = i == pick ? 255 : 64;

            assert(text.cursor[i] == 1005 + i && text.no[i] == 3049 + i);
            assert(text.y[i] == 402 + i * 36);
            assert(text.col[i][0] == c && text.col[i][1] == c && text.col[i][2] == c && text.col[i][3] == 128);
        }
    }

    /* The step byte moves every y: step 0 gives y = 512, step 8 gives y = 0. */
    reset(1, 0, 0);
    assert(Taku_Camp_Help_Window(0) == 10);
    assert(win.y[0] == 592 && font.y == 614 && text.y[4] == 802);
    reset(1, 8, 0);
    assert(Taku_Camp_Help_Window(0) == 10);
    assert(win.y[0] == 80 && win.y[2] == 94 && win.y[3] == 144 && text.y[0] == 146);
}

int main(void)
{
    check_help_window();
    return 0;
}
