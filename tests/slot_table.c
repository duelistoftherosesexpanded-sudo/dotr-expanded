/* Synthetic host checks for the 30-entry slot table helpers. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/slot_table.h"

struct DotrSlotTable gSzMsg_ml;
struct DotrSlotTableWords gSzMsg_defRect[1] = {{{0x6c0, 0x720, 0x16, 0x16}}};

static void fill(void)
{
    int i;
    memset(&gSzMsg_ml, 0x7e, sizeof(gSzMsg_ml));
    for (i = 0; i < 30; i++) {
        gSzMsg_ml.entries[i].unknown_000 = 0x1234567 + i;
        gSzMsg_ml.entries[i].flag = -1;
    }
}

static void check_entry(int index, int flag)
{
    struct DotrSlotTableEntry *entry = &gSzMsg_ml.entries[index];
    assert(entry->flag == flag && entry->unknown_000 == 0x1234567u + (unsigned int)index);
    assert(entry->unknown_004[0] == 0x7e7e7e7e && entry->unknown_004[3] == 0x7e7e7e7e);
}

static int draw_calls, draw_args[4];
static DotrDrawWord draw_cursor;
static struct DotrSlotTableWords draw_words;

static DotrDrawWord draw_callback(DotrDrawWord cursor, int a0, int a1, int a2, struct DotrSlotTableWords words, int a3)
{
    draw_calls++;
    draw_cursor = cursor;
    draw_args[0] = a0;
    draw_args[1] = a1;
    draw_args[2] = a2;
    draw_args[3] = a3;
    draw_words = words;
    return 0x4321;
}

int StringOffsetTable;
unsigned short StringBlob[16];
static int count_calls, count_results[2], base_calls, base_args[2];
static int step_calls, step_index[3], step_slot[3];

int SzText_GetClutGsAddr(int count)
{
    assert(count == 3 && count_calls < 2);
    return count_results[count_calls++];
}

int SzText_InitClutManage(int base, int size)
{
    base_calls++;
    base_args[0] = base;
    base_args[1] = size;
    return 0x40;
}

DotrDrawWord SzMsg_LoadClut_Pipe(DotrDrawWord cursor, int index, int slot)
{
    assert(step_calls < 3 && cursor == 1000 + step_calls);
    step_index[step_calls] = index;
    step_slot[step_calls] = slot;
    step_calls++;
    return cursor + 1;
}

/* 0x1fd320: one assertion set per outcome of the base-index test and of the
 * null test on the out pointer; the three-step loop is checked by its calls. */
static void check_setup(void)
{
    int *words = (int *)&gSzMsg_ml;
    int out = -1;
    int i;

    assert(offsetof(struct DotrSlotTable, unknown_274) == 628);
    assert(offsetof(struct DotrSlotTable, unknown_278) == 632);
    memset(&gSzMsg_ml, 0x7e, sizeof(gSzMsg_ml));
    StringOffsetTable = 2;
    StringBlob[2] = 0xe123;
    count_calls = base_calls = step_calls = 0;
    count_results[0] = 7;
    assert(SzMsg_Init_Pipe(1000, 0x55, 0x100, &out) == 1003);
    assert(words[0] == 640 && words[1] == 384 && words[2] == 10 && words[3] == 9);
    assert(words[4] == 7 && words[5] == 0x100 && words[6] == 0x55);
    assert(count_calls == 1 && base_calls == 0 && step_calls == 3 && out == 480);
    for (i = 0; i < 3; i++) {
        assert(step_index[i] == i && step_slot[i] == 7 + i);
        assert(gSzMsg_ml.entries[i * 10].flag == 0);
    }
    assert(gSzMsg_ml.unknown_274 == 0xfffffb && gSzMsg_ml.unknown_278 == 0x0123);

    count_calls = base_calls = step_calls = 0;
    count_results[0] = -1;
    count_results[1] = 12;
    gSzMsg_ml.unknown_278 = 0;
    assert(SzMsg_Init_Pipe(1000, 0x66, 0x100, 0) == 1003);
    assert(count_calls == 2 && base_calls == 1 && base_args[0] == 0x100 && base_args[1] == 1);
    assert(words[4] == 12 && words[5] == 0x140 && words[6] == 0x66 && step_slot[2] == 14);
    assert(gSzMsg_ml.unknown_278 == 0x0123);
    out = -1;
    count_calls = base_calls = step_calls = 0;
    assert(SzMsg_Init_Pipe(1000, 0x66, 0x100, &out) == 1003 && out == 0x40 + 480);
}

/* lSzMsg_GetTexInfo (branch coverage: each pointer null and not null). */
static void check_tex(void)
{
    struct DotrGsTex0 tex0;
    struct DotrGsTex1 tex1;
    unsigned long raw;
    int *words = gSzMsg_ml.words;

    assert(sizeof(tex0) == 8 && sizeof(tex1) == 8);
    words[0] = 640 | 0x7000; words[1] = 384; words[2] = 0x1a; words[3] = 0x29;
    words[4] = 0x3000; words[5] = 0x7abc; words[6] = 0x1234;
    lSzMsg_GetTexInfo(0, 0);

    memset(&tex0, 0xff, sizeof(tex0));
    memset(&tex1, 0xff, sizeof(tex1));
    lSzMsg_GetTexInfo(&tex0, 0);
    assert(tex0.tbp0 == 0x3abc && tex0.tbw == ((640 | 0x7000) >> 6 & 0x3f) && tex0.psm == 20);
    assert(tex0.tw == 0xa && tex0.th == 9 && tex0.tcc == 1 && tex0.tfx == 0);
    assert(tex0.cbp == ((0x3000 + 0x1234) & 0x3fff) && tex0.cpsm == 0 && tex0.csm == 0);
    assert(tex0.csa == 0 && tex0.cld == 1);
    memcpy(&raw, &tex1, 8);
    assert(raw == ~0ul);

    memset(&tex0, 0xff, sizeof(tex0));
    lSzMsg_GetTexInfo(0, &tex1);
    memcpy(&raw, &tex0, 8);
    assert(raw == ~0ul);
    assert(tex1.lcm == 0 && tex1.mxl == 0 && tex1.mmag == 0 && tex1.mmin == 0);
    assert(tex1.mtba == 0 && tex1.l == 0 && tex1.k == 0);
    /* padding bits keep their value */
    assert(tex1.pad_1 == 1 && tex1.pad_10 == 0x1ff && tex1.pad_21 == 0x7ff && tex1.pad_44 == 0xfffff);

    memset(&tex0, 0, sizeof(tex0));
    memset(&tex1, 0xff, sizeof(tex1));
    lSzMsg_GetTexInfo(&tex0, &tex1);
    assert(tex0.tbp0 == 0x3abc && tex0.cld == 1 && tex0.tcc == 1 && tex1.k == 0 && tex1.pad_1 == 1);
}

/* lSzMsg_FontTrace (branch and loop coverage): wide and narrow characters, the newline
 * character, a reference with a count of 0, one that runs out its count (with a nested
 * reference counting as 2) and one stopped by bit 15. Host fixtures; the word table has
 * one entry here. */
static void check_font_trace(void)
{
    static char half[3][53];
    static short code[3][53];
    unsigned short cols[3], widths[3], line, text[2];
    unsigned short *t = StringBlob;

    memset(half, 0x55, sizeof(half)); memset(code, 0x55, sizeof(code));
    memset(t, 0, sizeof(StringBlob));
    cols[0] = cols[1] = cols[2] = 0; widths[0] = widths[1] = widths[2] = 0; line = 1;
    gSzMsg_ml.unknown_278 = 0x1abc;
    text[0] = 0x0123; text[1] = 0x7777;
    assert(lSzMsg_FontTrace(text, half, code, cols, widths, &line) == 1);
    assert(widths[1] == 22 && half[1][0] == 11 && code[1][0] == 0x123 && cols[1] == 1 && line == 1);
    text[0] = 0x2000 | 0x0045;
    assert(lSzMsg_FontTrace(text, half, code, cols, widths, &line) == 1);
    assert(widths[1] == 34 && half[1][1] == 6 && code[1][1] == 0x45 && cols[1] == 2 && line == 1);
    assert(cols[0] == 0 && cols[2] == 0 && half[1][2] == 0x55 && half[0][0] == 0x55);
    /* The newline character only moves the line (bit 13 and 15 are not part of it). */
    text[0] = 0x8000 | 0x2000 | 0x1abc;
    assert(lSzMsg_FontTrace(text, half, code, cols, widths, &line) == 1);
    assert(line == 2 && cols[1] == 2 && widths[1] == 34 && widths[2] == 0);
    /* Reference, count 0: nothing traced, 2. */
    StringOffsetTable = 3;
    text[0] = 0x4000 | (1 << 6) | 0; text[1] = 0x8000;
    assert(lSzMsg_FontTrace(text, half, code, cols, widths, &line) == 2);
    assert(cols[2] == 0 && line == 2);
    /* Count 4 from table[3 + 1]: a character, a nested reference (2 words, to table[3 + 9],
     * count 1) and a character. */
    t[4] = 0x0011; t[5] = 0x4000 | (9 << 6) | 1; t[6] = 0x0000; t[7] = 0x2012; t[8] = 0x0099;
    t[12] = 0x0033; t[13] = 0x0044;
    text[0] = 0x4000 | (1 << 6) | 4; text[1] = 0xc000;
    assert(lSzMsg_FontTrace(text, half, code, cols, widths, &line) == 2);
    assert(cols[2] == 3 && code[2][0] == 0x11 && code[2][1] == 0x33 && code[2][2] == 0x12);
    assert(widths[2] == 22 + 22 + 12 && half[2][2] == 6 && code[2][3] == 0x5555);
    /* Bit 15 on the first traced word stops the reference though the count is 3. */
    cols[2] = 0; widths[2] = 0;
    t[4] = 0x8000 | 0x0021;
    text[0] = 0x4000 | (1 << 6) | 3;
    assert(lSzMsg_FontTrace(text, half, code, cols, widths, &line) == 2);
    assert(cols[2] == 1 && code[2][0] == 0x21 && widths[2] == 22);
    memset(t, 0, sizeof(StringBlob));
    StringOffsetTable = 0;
    gSzMsg_ml.unknown_278 = 0;
}

unsigned char gSzMsgImage[4][242];
/* Views SzMsg_LoadFont_Pipe reads (slot_table.h): the message index table as an int
 * array, the glyph table as bytes and the texture context at 0x31c070; the test fills them with
 * the same values as the scalar, 2-D and gSzMsg_ml forms above. */
int StringOffsetTable_slotfont[1];
unsigned char gSzMsgImage_slotfont[4 * 242];
struct DotrSlotFontContext gSzMsg_ml_slotfont;

/* SzMsg_LoadFont_Pipe: no entry, an entry already loaded, and a two-line text (wide and
 * narrow glyph, newline, wide glyph with bit 15): line 0 is the widest (34, two columns),
 * line 1 (22) is padded; both size loops double twice; the packet end needs four bytes of
 * alignment. Host fixtures: one word-table entry, four glyphs. */
static void check_load_font(void)
{
    static unsigned long packet[160];
    unsigned char *image = (unsigned char *)&packet[14];
    struct DotrSlotFontEntry *entry = (struct DotrSlotFontEntry *)&gSzMsg_ml.entries[3];
    unsigned short *t = StringBlob;
    int c, b, r, k;

    for (c = 0; c < 4; c++) {
        for (b = 0; b < 242; b++) {
            gSzMsgImage[c][b] = (unsigned char)(c * 64 + b % 61 + 1);
        }
    }
    memset(t, 0, sizeof(StringBlob));
    StringOffsetTable = 2;
    gSzMsg_ml.unknown_278 = 0x1abc;
    gSzMsg_ml.words[0] = 640;
    gSzMsg_ml.words[5] = 0x2345;
    memcpy(gSzMsgImage_slotfont, gSzMsgImage, sizeof(gSzMsgImage_slotfont));
    StringOffsetTable_slotfont[0] = 2;
    gSzMsg_ml_slotfont.width = 640;
    gSzMsg_ml_slotfont.tbp = 0x2345;
    t[2] = 0x0001; t[3] = 0x2000 | 0x0002; t[4] = 0x1abc; t[5] = 0x8000 | 0x0003;

    memset(packet, 0x5a, sizeof(packet));
    assert(SzMsg_LoadFont_Pipe((DotrDrawWord)(intptr_t)packet, 30, 0, 7, 9) == (DotrDrawWord)(intptr_t)packet);
    memset(entry, 0, sizeof(*entry));
    entry->flag = -1;
    assert(SzMsg_LoadFont_Pipe((DotrDrawWord)(intptr_t)packet, 3, 0, 7, 9) == (DotrDrawWord)(intptr_t)packet);
    assert(packet[0] == 0x5a5a5a5a5a5a5a5aUL && entry->cols == 0 && entry->w == 0);

    entry->flag = 0;
    assert(SzMsg_LoadFont_Pipe((DotrDrawWord)(intptr_t)packet, 3, 0, 7, 9) ==
           (DotrDrawWord)(intptr_t)((unsigned char *)packet + 864));
    assert(entry->flag == -1 && entry->cols == 2 && entry->tw == 64 && entry->th == 64);
    assert(entry->x == 7 && entry->y == 9 && entry->w == 34 && entry->h == 44);
    assert(packet[0] == 0x10000035UL && packet[1] == 0x5100003511000000UL);
    assert(packet[2] == 0x1000000000000004UL && packet[3] == 14);
    assert(packet[4] == (0x2345UL << 32 | 10UL << 48 | 0x14UL << 56) && packet[5] == 80);
    assert(packet[6] == (7UL << 32 | 9UL << 48) && packet[7] == 81);
    assert(packet[8] == (34UL | 44UL << 32) && packet[9] == 82);
    assert(packet[10] == 0 && packet[11] == 83);
    assert(packet[12] == (47UL | 0x8000UL | 2UL << 58) && packet[13] == 0);
    for (r = 0; r < 22; r++) {
        for (k = 0; k < 11; k++) {
            assert(image[r * 17 + k] == (unsigned char)(1 * 64 + (r * 11 + k) % 61 + 1));
            assert(image[(22 + r) * 17 + k] == (unsigned char)(3 * 64 + (r * 11 + k) % 61 + 1));
        }
        for (k = 0; k < 6; k++) {
            assert(image[r * 17 + 11 + k] == (unsigned char)(2 * 64 + (r * 11 + k) % 61 + 1));
            assert(image[(22 + r) * 17 + 11 + k] == 0);
        }
    }
    /* The four alignment bytes are skipped, not written. */
    assert(image[748] == 0x5a && image[751] == 0x5a && image[752] == 0x5a);

    memset(t, 0, sizeof(StringBlob));
    StringOffsetTable = 0;
    gSzMsg_ml.unknown_278 = 0;
}

int main(void)
{
    struct DotrSlotTableWords copy[3];
    int i;

    assert(offsetof(struct DotrSlotTable, entries) == 28);
    check_tex();
    check_font_trace();
    check_load_font();

    /* 0x1fea80 copies exactly the four words. */
    memset(copy, 0x55, sizeof(copy));
    SzMsg_GetDefRect(&copy[1]);
    assert(memcmp(&copy[1], gSzMsg_defRect, 16) == 0);
    assert(copy[0].words[3] == 0x55555555 && copy[2].words[0] == 0x55555555);

    /* 0x1feab0: entries 0..29, null outside. */
    assert(SzMsg_GetInfo(-1) == 0 && SzMsg_GetInfo(30) == 0 && SzMsg_GetInfo(0x7fffffff) == 0);
    assert(SzMsg_GetInfo(0) == &gSzMsg_ml.entries[0]);
    assert(SzMsg_GetInfo(29) == &gSzMsg_ml.entries[29]);
    assert((char *)SzMsg_GetInfo(7) - (char *)&gSzMsg_ml == 28 + 20 * 7);

    /* 0x1feaf0: clears only the flag of an in-range entry. */
    fill();
    SzMsg_Free(-1);
    SzMsg_Free(30);
    for (i = 0; i < 30; i++) {
        check_entry(i, -1);
    }
    SzMsg_Free(0);
    SzMsg_Free(29);
    check_entry(0, 0);
    check_entry(29, 0);
    check_entry(1, -1);
    check_entry(28, -1);
    assert(((unsigned char *)&gSzMsg_ml)[27] == 0x7e);

    /* 0x1feb40: clears every flag and nothing else. */
    fill();
    SzMsg_FreeAll();
    for (i = 0; i < 30; i++) {
        check_entry(i, 0);
    }
    assert(((unsigned char *)&gSzMsg_ml)[0] == 0x7e && ((unsigned char *)&gSzMsg_ml)[27] == 0x7e);

    /* 0x1fe7d0: missing entry returns the cursor without calling a4. */
    fill();
    assert(SzMsg_DrawX_Pipe(77, 30, 1, 2, 3, (int)(intptr_t)draw_callback) == 77);
    assert(SzMsg_DrawX_Pipe(78, -1, 1, 2, 3, (int)(intptr_t)draw_callback) == 78 && draw_calls == 0);
    /* Present entry: words 0,1 from the table, 2,3 from the entry. */
    gSzMsg_ml.entries[5].unknown_004[2] = 0xaaaa;
    gSzMsg_ml.entries[5].unknown_004[3] = 0xbbbb;
    assert(SzMsg_DrawX_Pipe(99, 5, 11, 12, 13, (int)(intptr_t)draw_callback) == 0x4321);
    assert(draw_calls == 1 && draw_cursor == 99);
    assert(draw_args[0] == 5 && draw_args[1] == 11 && draw_args[2] == 12 && draw_args[3] == 13);
    assert(draw_words.words[0] == 0x6c0 && draw_words.words[1] == 0x720);
    assert(draw_words.words[2] == 0xaaaa && draw_words.words[3] == 0xbbbb);

    check_setup();
    puts("slot_table: ok");
    return 0;
}
