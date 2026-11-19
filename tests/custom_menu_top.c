/* Synthetic host tests for the custom_menu_top group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/custom_menu_top.h"

unsigned char dotr_menu_top_block[0x90];
unsigned char SzDuel_CustomDuelMenu_menutop[2][10] = {
    { 0, 1, 2, 3, 4, 7, 0xff, 0xff, 0xff, 0xff },
    { 0, 1, 2, 7, 0xff, 3, 4, 5, 6, 0xff },
};
int data_00290a58_menutop;
int data_00290a62_menutop;
int data_00290a6c_menutop;
int data_00290a76_menutop;
signed char data_002bf05b_menutop[] = "ab";
signed char data_002bef98_menutop[] = "cde";
signed char data_002bf03a_menutop[] = "fghi";
char data_002be944_menutop[4];

enum { F_LOAD = 1, F_FLUSH, F_FRAME, F_RATIO, F_ICON, F_FONT, F_BOARD, F_FREE };

struct call {
    int fn;
    long a[8];
    float s[2];
    const void *p;
};

static struct call trace[96];
static int ntrace, map_flag, widths[16];
static DotrMenuTopWord next_cursor;

static struct call *add(int fn, long a0, long a1, long a2, long a3, long a4, long a5, long a6, const void *p)
{
    struct call *t;

    assert(ntrace < 96);
    t = &trace[ntrace++];
    memset(t, 0, sizeof(*t));
    t->fn = fn;
    t->a[0] = a0;
    t->a[1] = a1;
    t->a[2] = a2;
    t->a[3] = a3;
    t->a[4] = a4;
    t->a[5] = a5;
    t->a[6] = a6;
    t->p = p;
    return t;
}

static DotrMenuTopWord step(DotrMenuTopWord cursor)
{
    assert(cursor == next_cursor);
    return ++next_cursor;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    assert(0);
    return 0;
}

int func_0019bef0(const char *s) { return (int)strlen(s); }

int *SzMsg_GetInfo(int index)
{
    static int info[4];

    info[3] = widths[index];
    return info;
}

void SzMsg_Free(int index) { add(F_FREE, index, 0, 0, 0, 0, 0, 0, 0); }

void SzMsg_DrawRect_Pipe(void) {}

int SzData_GetMapEditFlag(int side)
{
    assert(side == 1);
    return map_flag;
}

DotrMenuTopWord SzMsg_LoadFont_Pipe(DotrMenuTopWord cursor, int a1, int msg, int a3, int a4)
{
    add(F_LOAD, a1, msg, a3, a4, 0, 0, 0, 0);
    return step(cursor);
}

DotrMenuTopWord RhText_SetTextureFlush(DotrMenuTopWord cursor)
{
    add(F_FLUSH, 0, 0, 0, 0, 0, 0, 0, 0);
    return step(cursor);
}

DotrMenuTopWord SzMsg_DrawRatioX_Pipe(DotrMenuTopWord cursor, int index, int x, int y, float sx, float sy, int a6,
                                      void (*function)(void))
{
    struct call *t;

    assert(a6 == 0 && function == SzMsg_DrawRect_Pipe);
    t = add(F_RATIO, index, x, y, 0, 0, 0, 0, 0);
    t->s[0] = sx;
    t->s[1] = sy;
    return step(cursor);
}

DotrMenuTopWord lSzDuel_DrawWindow_Frame_Pipe(DotrMenuTopWord cursor, int a1, int a2, int a3, int x, int y, int w,
                                              int h, int a8)
{
    assert(a8 == 1);
    add(F_FRAME, a1, a2, a3, x, y, w, h, 0);
    return step(cursor);
}

DotrMenuTopWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrMenuTopWord cursor, void *def, int x, int y, int a4, int a5,
                                               int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
    assert(a4 == 0 && a5 == 1 && a6 == 0 && a7 == 0 && a8 == 0 && a9 == 0 && a10 == 1 && a11 == 0 && a12 == -1);
    assert(a13 == 0x80);
    add(F_ICON, x, y, 0, 0, 0, 0, 0, def);
    return step(cursor);
}

DotrMenuTopWord SzDuel_DrawWindow_Font_Pipe(DotrMenuTopWord cursor, int x, int y, int a, int flags, int n,
                                            signed char *text, DotrMenuTopWord color)
{
    assert(a == 1 && flags == 2 && color == 0);
    add(F_FONT, x, y, n, 0, 0, 0, 0, text);
    return step(cursor);
}

DotrMenuTopWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrMenuTopWord cursor, int colour, int x, int y, int w, int h,
                                                int a6, int a7)
{
    assert(a6 == -1 && a7 == 0);
    add(F_BOARD, colour, x, y, w, h, 0, 0, 0);
    return step(cursor);
}

static void reset(int wide, int menu)
{
    int i;

    ntrace = 0;
    next_cursor = 300;
    memset(dotr_menu_top_block, 0, sizeof(dotr_menu_top_block));
    DOTR_MENU_TOP_WIDE = (unsigned char)wide;
    dotr_menu_top_block[2 + 1] = (unsigned char)(0x20 | menu);
    dotr_menu_top_block[2] = 0x0f;
    for (i = 0; i < 16; i++) {
        widths[i] = 100;
    }
}

static void test_narrow(void)
{
    DotrMenuTopWord r;
    struct call *c;
    int k, width = 0x12a;

    reset(0, 2);
    map_flag = 0;
    widths[3] = 344; /* item 2: scaled to 172 / 344 */
    r = lSzDuel_DrawCustomDuel_Menu_Top_Pipe(300, (unsigned char *)1, 10, 20);
    assert(r == next_cursor);
    for (k = 0; k < 8; k++) {
        assert(trace[k].fn == F_LOAD && trace[k].a[0] == k + 1 && trace[k].a[1] == 0x81 + k);
        assert(trace[k].a[3] == 0x16 + 0x16 * k);
    }
    assert(trace[8].fn == F_FLUSH);
    c = nth(F_FRAME, 0);
    assert(c->a[0] == 1 && c->a[1] == 1 && c->a[2] == 0 && c->a[3] == 10 && c->a[5] == -(width - 0x14) && c->a[6] == -0xae);
    /* Row 0 of the narrow list: items 0..4 drawn, item 7 (map) disabled; row 1 empty. */
    assert(count(F_RATIO) == 5 && count(F_FREE) == 6);
    c = nth(F_RATIO, 0);
    assert(c->a[0] == 1 && c->a[1] == 10 + (width >> 1) - 50 && c->a[2] == 20 + 0x1a);
    assert(c->s[0] == 1.0f && c->s[1] == 0.90909094f);
    c = nth(F_RATIO, 2);
    assert(c->a[0] == 3 && c->s[0] == 0.5f && c->a[1] == 10 + (width >> 1) - 86 && c->a[2] == 20 + 0x1a + 2 * 0x14);
    /* Item 2 is the cursor: the narrow highlight frame after its text. */
    assert(count(F_FRAME) == 2);
    c = nth(F_FRAME, 1);
    assert(c[-1].fn == F_RATIO && c[-1].a[0] == 3 && c[1].fn == F_FREE && c[1].a[0] == 3);
    assert(c->a[0] == -1 && c->a[1] == -1 && c->a[2] == 6 && c->a[3] == 10 + (width >> 1) - 0x62);
    assert(c->a[4] == 20 + 0x16 + 2 * 0x14 && c->a[5] == -0xbe && c->a[6] == -0x18);
    assert(nth(F_FREE, 5)->a[0] == 8);
    /* Board and three hints at quarters of width - 0x3a. */
    c = nth(F_BOARD, 0);
    assert(c->a[0] == 1 && c->a[1] == 10 + 0x20 && c->a[2] == 20 + 0x85 && c->a[3] == -(width - 0x3a));
    assert(nth(F_ICON, 0)->p == &data_00290a58_menutop && nth(F_ICON, 0)->a[0] == 10 + 0x6d0 + ((width - 0x3a) >> 2));
    assert(nth(F_ICON, 3)->p == &data_00290a76_menutop && nth(F_ICON, 3)->a[0] == 10 + 0x6d8 + (((width - 0x3a) * 3) >> 2));
    assert(nth(F_FONT, 0)->p == data_002bf05b_menutop && nth(F_FONT, 0)->a[2] == 2);
    assert(nth(F_FONT, 1)->p == data_002bef98_menutop && nth(F_FONT, 1)->a[0] == 10 + 0x20 + (((width - 0x3a) * 2) >> 2));
    assert(nth(F_FONT, 2)->p == data_002bf03a_menutop && nth(F_FONT, 2)->a[1] == 20 + 0xa7);
}

static void test_wide(void)
{
    struct call *c;
    int width = 0x25a;

    reset(1, 7);
    map_flag = 1;
    widths[8] = 172; /* item 7: exactly 172, not scaled */
    lSzDuel_DrawCustomDuel_Menu_Top_Pipe(300, (unsigned char *)1, 0, 0);
    /* Wide list: items 0, 1, 2, 7 (map enabled) in column 1, 3..6 in column 2. */
    assert(count(F_RATIO) == 8 && count(F_FREE) == 8);
    c = nth(F_RATIO, 3);
    assert(c->a[0] == 8 && c->s[0] == 1.0f && c->s[1] == 1.0f);
    assert(c->a[1] == (width / 3) - 86 && c->a[2] == 0x20 + 3 * 0x20);
    c = nth(F_RATIO, 4);
    assert(c->a[0] == 4 && c->a[1] == (width / 3) * 2 - 50 && c->a[2] == 0x20);
    /* The cursor on item 7: the wide highlight frame. */
    c = nth(F_FRAME, 1);
    assert(c->a[2] == 6 && c->a[3] == (width / 3) - 0x62 && c->a[4] == 0x1c + 3 * 0x20 && c->a[6] == -0x1a);
    assert(nth(F_FRAME, 0)->a[5] == -(width - 0x14) && nth(F_BOARD, 0)->a[3] == -(width - 0x3a));
}

int main(void)
{
    test_narrow();
    test_wide();
    return 0;
}
