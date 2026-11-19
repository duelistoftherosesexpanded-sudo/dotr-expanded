/* Synthetic host tests for the deck_list_pipe group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/deck_list_pipe.h"

unsigned char data_00317490_decklist[0xd70];
unsigned char DeckBuildingSide_decklist;
unsigned char BuildDeckHorizontalCursorIndex_decklist;
unsigned char gSzDuel_Icon_decklist[0x1000];
unsigned char data_00290828_decklist[1];
unsigned char data_00290832_decklist[1];
unsigned char data_00290896_decklist[1];
unsigned char data_002908a0_decklist[1];
unsigned char data_0029095e_decklist[1];
unsigned char data_00290968_decklist[1];
unsigned char data_00290972_decklist[1];
unsigned char data_00290a6c_decklist[1];
unsigned char data_00290a76_decklist[1];
unsigned char data_00290aa8_decklist[1];
unsigned char data_00290ab2_decklist[1];
unsigned char data_00290abc_decklist[1];
unsigned char data_00290ac6_decklist[1];
static signed char sort0[] = "sort0", sort1[] = "sort-one";
signed char *data_00290fd0_decklist[10] = { sort0, sort1, sort0, sort0, sort0, sort0, sort0, sort0, sort0, sort0 };
unsigned char data_002be8a4_decklist[4];
char data_002bef93_decklist[] = "act-5";
char data_002bef98_decklist[] = "act";
char data_002befab_decklist[] = "deckname";

enum { F_FRAME = 1, F_PIC, F_BOARD, F_ICON, F_FONT, F_DIGIT, F_CARD, F_SORT };

struct call {
    int fn;
    DotrDeckListWord cursor;
    long a[8];
    const void *p[2];
};

static struct call trace[64];
static int ntrace, pad_held, pad_calls, current_deck;
static DotrDeckListWord next_cursor;
static struct DotrDeckListPacket sorted;
static unsigned short cards[16];

static DotrDeckListWord add(int fn, DotrDeckListWord cursor, long a0, long a1, long a2, long a3, long a4, long a5,
                            long a6, long a7, const void *p0, const void *p1)
{
    struct call *t;

    assert(ntrace < 64);
    t = &trace[ntrace++];
    t->fn = fn;
    t->cursor = cursor;
    t->a[0] = a0;
    t->a[1] = a1;
    t->a[2] = a2;
    t->a[3] = a3;
    t->a[4] = a4;
    t->a[5] = a5;
    t->a[6] = a6;
    t->a[7] = a7;
    t->p[0] = p0;
    t->p[1] = p1;
    /* Each draw returns a fresh cursor, so the chaining can be checked. */
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

/* The ExIcon call drawing this definition. */
static struct call *icon(const unsigned char *def)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == F_ICON && trace[i].p[0] == def) {
            return &trace[i];
        }
    }
    return 0;
}

int SzPad_Check(int port, int kind, int mask)
{
    assert(port == 0 && kind == 1 && mask == 0x100);
    pad_calls++;
    return pad_held;
}

int SzData_GetCurrentDeck(int side)
{
    assert(side == DeckBuildingSide_decklist);
    return current_deck;
}

int func_0019bef0(const char *s) { return (int)strlen(s); }

DotrDeckListWord lSzDuel_DrawWindow_Frame_Pipe(DotrDeckListWord cursor, int a1, int a2, int a3, int x, int y,
                                              int w, int h, long long a8)
{
    assert(a1 == -1 && a2 == -1 && a8 == 1);
    return add(F_FRAME, cursor, a3, x, y, w, h, 0, 0, 0, 0, 0);
}

DotrDeckListWord lSzDuel_DrawWindow_PicMini_Pipe(DotrDeckListWord cursor, int id, int x, int y, int w, int h,
                                                long long z, int bright)
{
    return add(F_PIC, cursor, id, x, y, w, h, (long)z, bright, 0, 0, 0);
}

DotrDeckListWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrDeckListWord cursor, int colour, int x, int y, int w,
                                                int h, int back, int kind)
{
    return add(F_BOARD, cursor, colour, x, y, w, h, back, kind, 0, 0, 0);
}

DotrDeckListWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrDeckListWord cursor, unsigned char *def, int x, int y,
                                               int background, int image, int border, int shrink, int flipX,
                                               int flipY, int bright, unsigned char *rgb, int blend,
                                               unsigned char alpha)
{
    assert(background == 0 && image == 1 && border == 0 && shrink == 0 && flipX == 0 && flipY == 0);
    assert(alpha == 0x80);
    return add(F_ICON, cursor, x, y, bright, blend, 0, 0, 0, 0, def, rgb);
}

DotrDeckListWord SzDuel_DrawWindow_Font_Pipe(DotrDeckListWord cursor, int x, int y, int align, int flags,
                                            int n, signed char *text, void *color)
{
    return add(F_FONT, cursor, x, y, align, flags, n, 0, 0, 0, text, color);
}

DotrDeckListWord SzDuel_DrawWindow_FontDigit_Pipe(DotrDeckListWord cursor, int x, int y, int a, int flags,
                                                 int width, int value)
{
    return add(F_DIGIT, cursor, x, y, a, flags, width, value, 0, 0, 0, 0);
}

void *lSzDuel_DrawBuildDeck_Card_Pipe(void *cursor, int card, int x, int y, int flag, int flag2)
{
    return (void *)add(F_CARD, (DotrDeckListWord)cursor, card, x, y, flag, flag2, 0, 0, 0, 0, 0);
}

DotrDeckListWord ExSortPrim_Pipe(DotrDeckListWord cursor, DotrDeckListWord packet, int a2, int a3)
{
    assert(a2 == 0 && a3 == 0);
    memcpy(&sorted, (void *)packet, sizeof(sorted));
    return add(F_SORT, cursor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

static void reset(int mode_ed, int d5d, int held)
{
    unsigned short *list = cards;
    int i;

    memset(data_00317490_decklist, 0, sizeof(data_00317490_decklist));
    memset(&sorted, 0, sizeof(sorted));
    for (i = 0; i < 16; i++) {
        cards[i] = (unsigned short)(100 + i);
    }
    memcpy(data_00317490_decklist + 0x6dc, &list, sizeof(list));
    *(int *)(data_00317490_decklist + 0xc58) = 0x1234;
    data_00317490_decklist[0xd5d] = (unsigned char)d5d;
    DeckBuildingSide_decklist = 1;
    BuildDeckHorizontalCursorIndex_decklist = (unsigned char)mode_ed;
    pad_held = held;
    pad_calls = 0;
    current_deck = 0;
    ntrace = 0;
    next_cursor = 1000;
}

static void set_rows(unsigned short first, unsigned short total, unsigned char page)
{
    *(unsigned short *)(data_00317490_decklist + 0xc8e) = first;
    *(unsigned short *)(data_00317490_decklist + 0xc90) = total;
    data_00317490_decklist[0xd5f] = page;
}

static void test_plain(void)
{
    int i;

    reset(0, 0, 1);
    set_rows(0, 40, 0);
    data_00317490_decklist[0xcb9] = 3;
    current_deck = 1;
    lSzDuel_DrawBuildDeck_Deck_Pipe(1000, 10, 20);
    /* Neither mode byte selects the second set: the pad is not read. */
    assert(pad_calls == 0);
    assert(ntrace == 3 + 4 + 17 + 2 + 5 + 1 + 2);
    assert(count(F_BOARD) == 4);
    assert(count(F_ICON) == 17 && count(F_FONT) == 2 && count(F_CARD) == 5);
    assert(icon(data_00290aa8_decklist) == 0 && icon(data_00290a6c_decklist) == 0);
    /* The frame, leader picture (with the draw depth as 64-bit z) and the inner frame. */
    assert(trace[0].fn == F_FRAME && trace[0].a[0] == 11 && trace[0].a[1] == 0x280 + 10 - 0x50);
    assert(trace[0].a[2] == 20 + 0x10 && trace[0].a[3] == -0x40 && trace[0].a[4] == -0x44);
    assert(trace[1].fn == F_PIC && trace[1].a[0] == 1 && trace[1].a[1] == 0x280 + 10 - 0x4b);
    assert(trace[1].a[3] == 0x36 && trace[1].a[4] == 0x3a && trace[1].a[5] == 0x1234 && trace[1].a[6] == 1);
    assert(trace[2].fn == F_FRAME && trace[2].a[0] == 4);
    /* Sort tabs 0xd1..0xda, 0x10 apart; the selected one (index 3) drawn with style 1, no colour. */
    for (i = 0; i < 10; i++) {
        struct call *t = icon(gSzDuel_Icon_decklist + (0xd1 + i) * 10);
        assert(t != 0);
        assert(t->a[0] == 0x280 + 10 + 0x5a8 + i * 0x10 && t->a[1] == 20 + 0x769);
        assert(t->a[3] == (i == 3 ? 1 : -1) && t->p[1] == 0);
    }
    /* Sort name with flags 3, then the deck name centred. */
    assert(nth(F_FONT, 0)->p[0] == sort0 && nth(F_FONT, 0)->a[3] == 3 && nth(F_FONT, 0)->a[4] == 5);
    assert(nth(F_FONT, 0)->a[0] == 0x280 + 10 - 0xc8 && nth(F_FONT, 0)->a[1] == 20 + 0x5e);
    assert(nth(F_FONT, 1)->p[0] == data_002befab_decklist && nth(F_FONT, 1)->a[2] == 1);
    assert(nth(F_FONT, 1)->a[4] == 8 && nth(F_FONT, 1)->a[3] == 0);
    /* Five card rows from the first shown, 0x36 apart; the first row is marked at the top. */
    for (i = 0; i < 5; i++) {
        assert(nth(F_CARD, i)->a[0] == 100 + i && nth(F_CARD, i)->a[1] == 0x280 + 10 - 0x138);
        assert(nth(F_CARD, i)->a[2] == 20 + 0x70 + i * 0x36 && nth(F_CARD, i)->a[3] == (i == 0) && nth(F_CARD, i)->a[4] == 0);
    }
    /* The grey bar packet. */
    assert(count(F_SORT) == 1);
    assert(sorted.header.type == 3 && sorted.header.count == 6 && sorted.header.pad == 0);
    assert(sorted.header.registers == 0x441);
    assert(sorted.r == 0xa0 && sorted.g == 0xa0 && sorted.b == 0xa0 && sorted.a == 0x80 && sorted.q == 1.0f);
    assert(sorted.first.x == (unsigned)(((0x280 + 10 - 0x138) + 0x6ca) << 4) % 0x10000);
    assert(sorted.first.y == (unsigned)(((20 + 0x17e) + 0x738) << 4) % 0x10000);
    assert(sorted.second.x == (unsigned)(((0x280 + 10 - 0x138) + 0x6f2) << 4) % 0x10000);
    assert(sorted.second.y == (unsigned)(((20 + 0x17e) + 0x739) << 4) % 0x10000);
    assert(sorted.first.z == 0x1234 && sorted.second.z == 0x1234 && sorted.first.fog == 0);
    /* Page and count. */
    assert(nth(F_DIGIT, 0)->a[5] == 1 && nth(F_DIGIT, 1)->a[5] == 40);
    assert(nth(F_DIGIT, 1)->a[1] == nth(F_DIGIT, 0)->a[1] + 0x17);
    /* The current deck's icon is bright. */
    assert(icon(data_0029095e_decklist)->a[2] == 0 && icon(data_00290968_decklist)->a[2] == 1);
    assert(icon(data_00290972_decklist)->a[2] == 0);
    assert(trace[ntrace - 1].p[0] == data_00290972_decklist);
}

static void test_second_set(void)
{
    int i;

    /* Mode byte 4 with button 0x100 held: the second sort index, highlight colour and flags 11. */
    reset(4, 0, 1);
    set_rows(10, 12, 2);
    data_00317490_decklist[0xcb9] = 0;
    data_00317490_decklist[0xcba] = 1;
    current_deck = 2;
    lSzDuel_DrawBuildDeck_Deck_Pipe(1000, 0, 0);
    assert(pad_calls == 1);
    assert(count(F_ICON) == 23 && count(F_FONT) == 3);
    for (i = 0; i < 10; i++) {
        struct call *t = icon(gSzDuel_Icon_decklist + (0xd1 + i) * 10);
        assert(t->a[3] == (i == 1 ? 2 : -1) && t->p[1] == data_002be8a4_decklist);
    }
    assert(nth(F_FONT, 0)->p[0] == sort1 && nth(F_FONT, 0)->a[3] == 11);
    assert(nth(F_FONT, 0)->p[1] == data_002be8a4_decklist);
    /* The action text (mode byte 0xd5d not 5) between the a6c and a76 icons. */
    assert(nth(F_FONT, 1)->p[0] == data_002bef98_decklist && nth(F_FONT, 1)->a[0] == 0x280 - 0x4f);
    assert(nth(F_FONT, 1)->a[1] == 0x19d && nth(F_FONT, 1)->p[1] == 0);
    assert(icon(data_00290aa8_decklist) && icon(data_00290abc_decklist) && icon(data_00290ac6_decklist));
    assert(icon(data_00290ab2_decklist) && icon(data_00290a6c_decklist) && icon(data_00290a76_decklist));
    /* Rows start at card 10 and none is marked. */
    for (i = 0; i < 5; i++) {
        assert(nth(F_CARD, i)->a[0] == 110 + i && nth(F_CARD, i)->a[3] == 0);
    }
    assert(nth(F_DIGIT, 0)->a[5] == 10 + 2 + 1 && nth(F_DIGIT, 1)->a[5] == 12);
    assert(icon(data_00290972_decklist)->a[2] == 1 && icon(data_00290968_decklist)->a[2] == 0);

    /* Mode 5 at 0xd5d also enables the set; not held: first sort index, no colour. */
    reset(0, 5, 0);
    data_00317490_decklist[0xcb9] = 9;
    lSzDuel_DrawBuildDeck_Deck_Pipe(1000, 0, 0);
    assert(pad_calls == 1);
    assert(count(F_ICON) == 23);
    assert(nth(F_FONT, 0)->p[0] == sort0 && nth(F_FONT, 0)->a[3] == 3 && nth(F_FONT, 0)->p[1] == 0);
    assert(icon(gSzDuel_Icon_decklist + (0xd1 + 9) * 10)->a[3] == 1);
    assert(nth(F_FONT, 1)->p[0] == data_002bef93_decklist);
    assert(icon(data_0029095e_decklist)->a[2] == 1);
}

int main(void)
{
    test_plain();
    test_second_set();
    return 0;
}
