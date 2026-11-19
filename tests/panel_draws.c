/* Synthetic host tests for the panel_draws group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/panel_draws.h"

unsigned char data_00317490_bytes[0xd70];
char data_002901ca[1];
char data_002909b8[1];
void SzMsg_DrawRect_Pipe(void) {}
static struct DotrSpriteEntry entry;

/* Every stub appends its id and arguments; the checks compare whole call lists. */
static long seen[16][12];
static int seen_count;
static int counts[2];
static int alternate;

static DotrSpriteWord put(long id, long a, long b, long c, long d, long e, long f, long g, long h, long i, long j, long k)
{
    long row[12];
    row[0] = id; row[1] = a; row[2] = b; row[3] = c; row[4] = d; row[5] = e;
    row[6] = f; row[7] = g; row[8] = h; row[9] = i; row[10] = j; row[11] = k;
    assert(seen_count < 16);
    memcpy(seen[seen_count++], row, sizeof(row));
    return 0x1000 * (seen_count + 1);
}
static void was(int at, long id, long a, long b, long c, long d, long e, long f, long g, long h, long i, long j, long k)
{
    long row[12];
    row[0] = id; row[1] = a; row[2] = b; row[3] = c; row[4] = d; row[5] = e;
    row[6] = f; row[7] = g; row[8] = h; row[9] = i; row[10] = j; row[11] = k;
    assert(at < seen_count && memcmp(seen[at], row, sizeof(row)) == 0);
}

int SzDuel_GetPlayer(int index) { put(0x1de4e0, index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return counts[index]; }
DotrSpriteWord lSzDuel_DrawWindow_Frame_Pipe(DotrSpriteWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    return put(0x1b4780, cursor, a1, a2, a3, x, y, a6, a7, a8, 0, 0);
}
DotrSpriteWord func_001b3c80_cursor(DotrSpriteWord cursor, char *b, int c, int d, int e, int f, int g, int h,
                                    long i, long j, long k)
{
    return put(0x1b3c80, cursor, b == data_002901ca ? 1 : b == data_002909b8 ? 2 : 0, c, d, e, f, g, h, i, j, k);
}
int lSzDuel_DrawWindow_Board_Pipe(int a, int b, int c, int d, int e, int f, int g) { return put(0x1b65e0, a, b, c, d, e, f, g, 0, 0, 0, 0); }
int SzDuel_DrawWindow_Digit_Pipe(int a, int b, int c, int d, int e, int f) { return put(0x1b6c90, a, b, c, d, e, f, 0, 0, 0, 0, 0); }
int SzDuel_GetUnitLevel(int a, int b, int flag) { put(0x1e08f0, a, b, flag, 0, 0, 0, 0, 0, 0, 0, 0); return 777; }
DotrSpriteWord SzMsg_LoadFont_Pipe(DotrSpriteWord cursor, int a0, int a1, int a2, int a3)
{
    return put(0x1fd680, cursor, a0, a1, a2, a3, 0, 0, 0, 0, 0, 0);
}
DotrSpriteWord RhText_SetTextureFlush(DotrSpriteWord cursor) { return put(0x20f290, cursor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
struct DotrSpriteEntry *SzMsg_GetInfo(int index) { put(0x1feab0, index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return &entry; }
DotrSpriteWord func_001fe7d0_panel(DotrSpriteWord cursor, int a0, int a1, int a2, int a3, void (*function)(void))
{
    return put(0x1fe7d0, cursor, a0, a1, a2, a3, function == SzMsg_DrawRect_Pipe, entry.unknown_0c, ((int *)&entry)[1], 0, 0, 0);
}
int Ta_GetSummonCost(void) { put(0x26e950, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); return 888; }
void SzMsg_Free(int index) { put(0x1feaf0, index, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }

static DotrSpriteWord run(DotrSpriteWord (*function)(DotrSpriteWord, int, int), int side, int own, int other)
{
    memset(data_00317490_bytes, 0, sizeof(data_00317490_bytes));
    memset(&entry, 0, sizeof(entry));
    data_00317490_bytes[0xd3c] = (unsigned char)side;
    data_00317490_bytes[0xd54] = 9;
    data_00317490_bytes[0xd4f] = 6;
    data_00317490_bytes[0xd4e] = (unsigned char)alternate;
    counts[side] = own;
    counts[side ^ 1] = other;
    seen_count = 0;
    return function(0x100, 200, 50);
}

/* Branch coverage. Returned cursors are 0x1000 * (position of the call + 1). */
static void check_first(void)
{
    /* Own count 32: one query, the number. */
    assert(run(lSzDuel_DrawWindow_Level_Pipe, 1, 32, 100) == 0x7000 && seen_count == 6);
    was(0, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(1, 0x1b4780, 0x100, 9, -1, 6, 200, 50, 8, 3, 1, 0, 0);
    was(2, 0x1b3c80, 0x3000, 1, 200 + 1735, 50 + 1830, 0, 1, 0, 0, 1, 0, -1);
    was(3, 0x1b65e0, 0x4000, 9, 233, 54, 3, 1, -1, 0, 0, 0, 0);
    was(4, 0x1e08f0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(5, 0x1b6c90, 0x5000, 236, 56, 2, 777, 0, 0, 0, 0, 0, 0);
    /* Own count 31, other 99: two queries (the second for side ^ 1), still the number. */
    assert(run(lSzDuel_DrawWindow_Level_Pipe, 0, 31, 99) == 0x8000 && seen_count == 7);
    was(0, 0x1de4e0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(1, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(4, 0x1b65e0, 0x5000, 9, 233, 54, 3, 1, -1, 0, 0, 0, 0);
    was(6, 0x1b6c90, 0x6000, 236, 56, 2, 777, 0, 0, 0, 0, 0, 0);
    /* Own count 31, other 100: the slot image; the entry word is 44 before 0x1fe7d0. */
    assert(run(lSzDuel_DrawWindow_Level_Pipe, 1, 31, 100) == 0xa000 && seen_count == 10 && entry.unknown_0c == 44);
    was(1, 0x1de4e0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(2, 0x1b4780, 0x100, 9, -1, 6, 200, 50, 8, 3, 1, 0, 0);
    was(3, 0x1b3c80, 0x4000, 1, 200 + 1735, 50 + 1830, 0, 1, 0, 0, 1, 0, -1);
    was(4, 0x1b65e0, 0x5000, 9, 233, 54, 3, 1, -1, 0, 0, 0, 0);
    was(5, 0x1fd680, 0x6000, 1, 28, 0, 22, 0, 0, 0, 0, 0, 0);
    was(6, 0x20f290, 0x7000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(7, 0x1feab0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(8, 0x1fe7d0, 0x8000, 1, 236, 56, 0, 1, 44, 0, 0, 0, 0);
    was(9, 0x1feaf0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    /* Control byte +0xd4e is not read by this one. */
    alternate = 1;
    assert(run(lSzDuel_DrawWindow_Level_Pipe, 1, 32, 100) == 0x7000 && seen_count == 6);
    was(5, 0x1b6c90, 0x5000, 236, 56, 2, 777, 0, 0, 0, 0, 0, 0);
    alternate = 0;
}

/* Branch coverage, as check_first plus the two outcomes of control byte +0xd4e. */
static void check_second(void)
{
    assert(run(lSzDuel_DrawWindow_SummonCost_Pipe, 1, 32, 100) == 0x7000 && seen_count == 6);
    was(0, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(1, 0x1b4780, 0x100, 9, -1, 6, 200, 50, 8, 3, 1, 0, 0);
    was(2, 0x1b3c80, 0x3000, 2, 200 + 1735, 50 + 1830, 0, 1, 0, 0, 1, 0, -1);
    was(3, 0x1b65e0, 0x4000, 9, 233, 54, 3, 1, -1, 0, 0, 0, 0);
    was(4, 0x1e08f0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(5, 0x1b6c90, 0x5000, 236, 56, 2, 777, 0, 0, 0, 0, 0, 0);
    alternate = 1;
    assert(run(lSzDuel_DrawWindow_SummonCost_Pipe, 0, 31, 99) == 0x9000 && seen_count == 8);
    was(1, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(5, 0x1e08f0, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(6, 0x26e950, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(7, 0x1b6c90, 0x6000, 236, 56, 2, 888, 0, 0, 0, 0, 0, 0);
    /* The image path does not read the byte. */
    assert(run(lSzDuel_DrawWindow_SummonCost_Pipe, 1, 31, 100) == 0xa000 && seen_count == 10 && entry.unknown_0c == 44);
    was(3, 0x1b3c80, 0x4000, 2, 200 + 1735, 50 + 1830, 0, 1, 0, 0, 1, 0, -1);
    was(5, 0x1fd680, 0x6000, 1, 28, 0, 22, 0, 0, 0, 0, 0, 0);
    was(8, 0x1fe7d0, 0x8000, 1, 236, 56, 0, 1, 44, 0, 0, 0, 0);
    was(9, 0x1feaf0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    alternate = 0;
}

/* Branch coverage for 0x1b6ca0. Rows: 0x1fe7d0 cursor, 0, x, y, a7, function is 0x1fdfe0, entry
 * +0xc, entry +4. */
static DotrSpriteWord digits_run(int digits, int value, int narrow, int all)
{
    memset(&entry, 0, sizeof(entry));
    seen_count = 0;
    return SzDuel_DrawWindow_ExDigit_Pipe(0x100, 200, 50, digits, value, narrow, all, 9);
}
static void check_digits(void)
{
    /* The marker value: every place gets the piece 264; step 20. */
    assert(digits_run(2, 0x7fffffff, 0, 0) == 0x4000 && seen_count == 3);
    was(0, 0x1feab0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(1, 0x1fe7d0, 0x100, 0, 220, 50, 9, 1, 0, 264, 0, 0, 0);
    was(2, 0x1fe7d0, 0x3000, 0, 200, 50, 9, 1, 0, 264, 0, 0, 0);
    /* The marker value with no places: the cursor comes back unchanged. */
    assert(digits_run(0, 0x7fffffff, 0, 0) == 0x100 && seen_count == 1);
    /* No places, ordinary value. */
    assert(digits_run(0, 5, 0, 0) == 0x100 && seen_count == 1);
    /* 305 in five narrow places: three digits, then the power 1000 exceeds it. */
    assert(digits_run(5, 305, 1, 0) == 0x5000 && seen_count == 4);
    was(1, 0x1fe7d0, 0x100, 0, 264, 50, 9, 1, 0, 110, 0, 0, 0);
    was(2, 0x1fe7d0, 0x3000, 0, 248, 50, 9, 1, 0, 0, 0, 0, 0);
    was(3, 0x1fe7d0, 0x4000, 0, 232, 50, 9, 1, 0, 66, 0, 0, 0);
    /* A value equal to the next power does not stop: 10 draws two digits. */
    assert(digits_run(3, 10, 0, 0) == 0x4000 && seen_count == 3);
    was(1, 0x1fe7d0, 0x100, 0, 240, 50, 9, 1, 0, 0, 0, 0, 0);
    was(2, 0x1fe7d0, 0x3000, 0, 220, 50, 9, 1, 0, 22, 0, 0, 0);
    /* `all` set: every place is drawn, leading zero included. */
    assert(digits_run(2, 7, 0, 1) == 0x4000 && seen_count == 3);
    was(1, 0x1fe7d0, 0x100, 0, 220, 50, 9, 1, 0, 154, 0, 0, 0);
    was(2, 0x1fe7d0, 0x3000, 0, 200, 50, 9, 1, 0, 0, 0, 0, 0);
    /* Negative with a place left: the digit word is negative, then the piece 242. */
    assert(digits_run(3, -3, 0, 0) == 0x4000 && seen_count == 3);
    was(1, 0x1fe7d0, 0x100, 0, 240, 50, 9, 1, 0, -66, 0, 0, 0);
    was(2, 0x1fe7d0, 0x3000, 0, 220, 50, 9, 1, 0, 242, 0, 0, 0);
    /* Negative with no place left: no extra piece. */
    assert(digits_run(1, -3, 0, 1) == 0x3000 && seen_count == 2);
    was(1, 0x1fe7d0, 0x100, 0, 200, 50, 9, 1, 0, -66, 0, 0, 0);
}

/* 0x27f830 branch coverage: both sides of each bound, with the call lists. */
static int range_run(int count)
{
    counts[1] = count;
    seen_count = 0;
    return TaTuto_IsTuto(1);
}
static void check_range(void)
{
    /* Below the lower bound: one call only. */
    assert(range_run(31) == 0 && seen_count == 1);
    was(0, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    /* At the lower bound: the second call gets the same index. */
    assert(range_run(32) == 1 && seen_count == 2);
    was(0, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    was(1, 0x1de4e0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    assert(range_run(99) == 1 && seen_count == 2);
    assert(range_run(100) == 0 && seen_count == 2);
}

int main(void)
{
    check_first();
    check_second();
    check_digits();
    check_range();
    return 0;
}
