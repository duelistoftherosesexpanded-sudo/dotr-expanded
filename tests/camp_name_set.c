/* Synthetic host tests for the camp_name_set group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_name_set.h"

#define TEXTS 20
#define TEXT_LEN 40

struct DotrCampNameWork dotr_camp_name_set_work;
/* The function reaches the board through a cast of this byte array (see the header), so the
 * host object is a byte array too, large enough for the struct and aligned for its halfwords. */
#ifdef __GNUC__
__attribute__((aligned(8)))
#endif
signed char data_00299e10[sizeof(struct DotrCampNameBoard) + 4];
int StringOffsetTable_nameset[TEXTS];
unsigned short StringBlob_nameset[TEXTS * TEXT_LEN];
unsigned short DeckTab_nameset[16];
struct DotrCampNameDeck data_00299d00_nameset[32];

/* Every text character carries three top bits, so the two masks give different results. */
static unsigned short raw(int text, int column)
{
    return (unsigned short)(0xe000 | text << 8 | column);
}
static unsigned short *text(int n)
{
    return &StringBlob_nameset[n * TEXT_LEN];
}
static short board_name(int i)
{
    short v;

    memcpy(&v, data_00299e10 + 36 + i * 2, sizeof v);
    return v;
}
static void reset(void)
{
    int n, c, i;

    memset(&dotr_camp_name_set_work, 0, sizeof dotr_camp_name_set_work);
    memset(data_00299e10, 0x55, sizeof data_00299e10);
    for (n = 0; n < TEXTS; n++) {
        StringOffsetTable_nameset[n] = n * TEXT_LEN;
        for (c = 0; c < TEXT_LEN; c++) {
            text(n)[c] = raw(n, c);
        }
    }
    for (i = 0; i < 16; i++) {
        /* bit 15 set: it must not reach the third 5-bit number */
        DeckTab_nameset[i] = (unsigned short)(0x8000 | (i + 3) << 10 | (31 - i) << 5 | i);
    }
    for (i = 0; i < 32; i++) {
        memset(&data_00299d00_nameset[i], 0xee, sizeof data_00299d00_nameset[i]);
        /* six top bits set: only the low ten are kept */
        data_00299d00_nameset[i].id = (unsigned short)(0xfc00 | (i * 29 + 5));
    }
}

/* What slot i must hold, from the description of the function (not from its code). */
static unsigned short expect_char(int i)
{
    struct DotrCampNameWork *w = &dotr_camp_name_set_work;
    int row = w->code[i] / 5;
    int col = w->code[i] % 5 + w->page[i] * 6;

    if (w->mode[i] == 0) {
        return (unsigned short)((row + 11) << 8 | col);
    }
    if (w->mode[i] == 1) {
        return (unsigned short)((row + 14) << 8 | col);
    }
    return (unsigned short)(0x2000 | (row + 8) << 8 | col);
}

static void run(int end, int mode0)
{
    static const signed char modes[12] = { 0, 1, 2, 0, 1, -1, 0, 1, 2, 0, 1, 3 };
    static const signed char codes[12] = { 7, 0, 14, 4, 5, 9, 10, 13, 1, 2, 12, 6 };
    static const signed char pages[12] = { 2, 0, 1, 3, 0, 2, 1, 0, 3, 2, 1, 0 };
    struct DotrCampNameWork *w = &dotr_camp_name_set_work;
    unsigned short want[12];
    int i, j, sum = 0;

    reset();
    memcpy(w->mode, modes, 12);
    memcpy(w->code, codes, 12);
    memcpy(w->page, pages, 12);
    w->mode[0] = (signed char)mode0;
    w->end = (signed char)end;
    w->hash = 0xffff;
    for (i = 0; i < 12; i++) {
        want[i] = expect_char(i);
    }
    Taku_Camp_Ivent_Name_Set();

    /* Slot 0 is also written to the text's first halfword, without the end mark; the second
     * halfword is not written at all. */
    assert(text(2)[0] == want[0]);
    assert(text(2)[1] == raw(2, 1));
    for (i = 0; i < 12; i++) {
        unsigned short v = (unsigned short)(want[i] | (i == end ? 0x8000 : 0));

        assert(text(2)[i + 2] == v);
        assert((unsigned short)board_name(i) == v);
        sum += v & 15;
    }
    assert(text(2)[14] == raw(2, 14));
    /* nothing before or after the twelve name halfwords of the board */
    assert(data_00299e10[35] == 0x55 && data_00299e10[60] == 0x55);
    /* The marked slot's halfword is negative as a short; the hash still adds its low four bits. */
    if (end >= 0 && end < 12) {
        assert(board_name(end) < 0);
    }
    assert(w->hash == (sum & 15));
    for (j = 0; j < 3; j++) {
        int pick = j == 0 ? w->hash : j == 1 ? 31 - w->hash : w->hash + 3;

        assert(w->pick[j] == pick);
        assert(w->deck[j] == ((pick * 29 + 5) & 0x3ff));
    }
    /* the inputs are left alone */
    assert(w->end == end && w->mode[0] == mode0 && memcmp(w->code, codes, 12) == 0 && memcmp(w->page, pages, 12) == 0);
}

int main(void)
{
    assert(offsetof(struct DotrCampNameBoard, name) == 36);
    assert(offsetof(struct DotrCampNameWork, end) == 21454 && offsetof(struct DotrCampNameWork, mode) == 21455);
    assert(offsetof(struct DotrCampNameWork, code) == 21467 && offsetof(struct DotrCampNameWork, page) == 21479);
    assert(offsetof(struct DotrCampNameWork, hash) == 21494 && offsetof(struct DotrCampNameWork, pick) == 21496);
    assert(offsetof(struct DotrCampNameWork, deck) == 21502 && sizeof(struct DotrCampNameDeck) == 12);

    /* each mode on slot 0 (later slots carry modes 0, 1 and three other values); end marks
     * inside the name (first, middle, last slot) and outside it */
    run(0, 0);
    run(0, 1);
    run(0, 2);
    run(5, 0);
    run(11, 1);
    run(12, 2);
    run(-1, 0);
    puts("camp_name_set: ok");
    return 0;
}
