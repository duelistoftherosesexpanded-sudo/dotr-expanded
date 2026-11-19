/* Synthetic host tests for the digit_rows group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/digit_rows.h"

/* Every callee is a recording stub. */
struct DotrDigitView gSzGame_gl_digits;
unsigned char DebugMenuOptions[160];

static struct DotrDigitEntry entries[22];
static int entry_calls, close_calls, begin_calls, state_calls;
static struct { int a0, a1, a2, a3; } rows[24];
static int row_count;
static struct { int a0, a1, a2, a3, low, high; } draws[40];
static int draw_count;
static int box_y, box_words[4], box_calls;

void SzMsg_GetDefRect(int *destination)
{
    destination[0] = 1, destination[1] = 2, destination[2] = 3, destination[3] = 4;
}
struct DotrDigitEntry *SzMsg_GetInfo(int index)
{
    assert(index >= 0 && index < 22);
    entry_calls++;
    return &entries[index];
}
int RhBase_GetUserGsAddress(void)
{
    return 77;
}
int SzMsg_Init(int a0, int a1)
{
    assert(a0 == 0 && a1 == 77);
    state_calls++;
    return 0;
}
void SzMsg_LoadFont(int a0, int a1, int a2, int a3)
{
    assert(row_count < 24);
    rows[row_count].a0 = a0, rows[row_count].a1 = a1, rows[row_count].a2 = a2, rows[row_count].a3 = a3;
    row_count++;
}
void ExFlushTexture(void)
{
    /* the two entry stores come before this call */
    assert(entries[20].words[2] == 22 && entries[21].words[2] == 22);
    begin_calls++;
}
void SzMsg_DrawGlass(int a0, int a1, int *words, int a3)
{
    assert(a0 == 0 && a3 == 0);
    box_y = a1;
    memcpy(box_words, words, sizeof(box_words));
    box_calls++;
}
void SzMsg_DrawRect_Pipe(void)
{
}
void SzMsg_DrawX(int a0, int a1, int a2, int a3, void (*draw)(void))
{
    assert(draw == SzMsg_DrawRect_Pipe && draw_count < 40);
    draws[draw_count].a0 = a0, draws[draw_count].a1 = a1, draws[draw_count].a2 = a2, draws[draw_count].a3 = a3;
    draws[draw_count].low = entries[20].words[0], draws[draw_count].high = entries[21].words[0];
    draw_count++;
}
void SzMsg_Free(int index)
{
    /* rows are closed in order, each after its 0x1feab0 call */
    assert(index == close_calls && entry_calls == 2 + index + 1);
    close_calls++;
}

static void fill(int page, int cursor)
{
    int i;

    memset(&gSzGame_gl_digits, 0, sizeof(gSzGame_gl_digits));
    memset(entries, 0, sizeof(entries));
    assert(offsetof(struct DotrDigitView, cursor) == 0x223 && offsetof(struct DotrDigitView, radix) == 0x1cc);
    for (i = 0; i < 160; i++) {
        DebugMenuOptions[i] = (unsigned char)(i % 29 + 1);
    }
    for (i = 0; i < 20; i++) {
        entries[i].words[2] = 100 + i;
    }
    gSzGame_gl_digits.page = (unsigned char)page;
    gSzGame_gl_digits.cursor = (unsigned char)cursor;
    entry_calls = close_calls = begin_calls = state_calls = row_count = draw_count = box_calls = 0;
}

static void set_label(int id, int count, int radix, unsigned int value)
{
    gSzGame_gl_digits.flag[id] = 1;
    gSzGame_gl_digits.count[id] = (unsigned int)count;
    gSzGame_gl_digits.radix[id] = (unsigned char)radix;
    gSzGame_gl_digits.value[id] = value;
}

/* SzGame_DrawMenu (branch and loop coverage). */
static void check_page(void)
{
    int i;

    /* page 0: all twenty rows; row 3 (label 4) selected, flag set, three digits in radix 12 */
    fill(0, 3);
    set_label(4, 3, 12, 1 * 144 + 11 * 12 + 9);
    set_label(6, 1, 16, 10);
    set_label(7, 0, 10, 5);
    SzGame_DrawMenu();
    assert(state_calls == 1 && begin_calls == 1 && entry_calls == 22 && close_calls == 20);
    assert(row_count == 22);
    for (i = 0; i < 20; i++) {
        assert(rows[i].a0 == i && rows[i].a1 == i + 1 + 284);
        assert(rows[i].a2 == (i < 10 ? 0 : 320) && rows[i].a3 == (i % 10 + 2) * 22);
    }
    assert(rows[9].a2 == 0 && rows[10].a2 == 320 && rows[10].a3 == 44 && rows[19].a3 == 242);
    assert(rows[20].a0 == 20 && rows[20].a1 == 21 && rows[20].a2 == 0 && rows[20].a3 == 0);
    assert(rows[21].a0 == 21 && rows[21].a1 == 24 && rows[21].a2 == 0 && rows[21].a3 == 22);
    assert(box_calls == 1 && box_y == 66);
    assert(box_words[0] == 1 && box_words[1] == 2 && box_words[2] == 13 * 22 && box_words[3] == 4);
    /* 20 row draws, three digits for row 3, one for row 5, none for row 6 (count 0) */
    assert(draw_count == 24);
    assert(draws[3].a0 == 3 && draws[3].a1 == 0 && draws[3].a2 == 66 && draws[3].a3 == 0);
    /* digits 1, 11, 9: below 10 through entry 20, the others through entry 21 */
    assert(draws[4].a0 == 20 && draws[4].a1 == 220 && draws[4].a2 == 66 && draws[4].low == 22);
    assert(draws[5].a0 == 21 && draws[5].a1 == 242 && draws[5].a2 == 66 && draws[5].high == 22);
    assert(draws[6].a0 == 20 && draws[6].a1 == 264 && draws[6].low == 9 * 22 && draws[6].a3 == 0);
    assert(draws[7].a0 == 4 && draws[7].a2 == 88);
    /* a single digit equal to 10: entry 21 with offset 0 */
    assert(draws[8].a0 == 5 && draws[9].a0 == 21 && draws[9].a1 == 220 && draws[9].a2 == 110 && draws[9].high == 0);
    assert(draws[10].a0 == 6 && draws[11].a0 == 7 && draws[23].a0 == 19 && draws[23].a2 == 418);

    /* selected row with the flag clear: word 2 comes from the row's entry */
    fill(0, 2);
    SzGame_DrawMenu();
    assert(box_calls == 1 && box_y == 44 && box_words[2] == 102 && box_words[0] == 1);
    assert(draw_count == 20 && row_count == 22);

    /* page 1: rows 0..8 only (20 + 8 = 28 is the last below 29); the selected row 9 is skipped */
    fill(1, 9);
    SzGame_DrawMenu();
    assert(row_count == 11 && rows[8].a0 == 8 && rows[8].a1 == 29 + 284 && rows[9].a0 == 20 && rows[10].a0 == 21);
    assert(draw_count == 9 && draws[8].a0 == 8 && box_calls == 0);
    assert(entry_calls == 22 && close_calls == 20);
    fill(1, 8);
    SzGame_DrawMenu();
    assert(box_calls == 1 && box_y == 176 && box_words[2] == 108);

    /* page 2: no row at all, the two fixed 0x1fd610 calls remain */
    fill(2, 0);
    SzGame_DrawMenu();
    assert(row_count == 2 && draw_count == 0 && box_calls == 0 && close_calls == 20 && begin_calls == 1);
}

int main(void)
{
    check_page();
    return 0;
}
