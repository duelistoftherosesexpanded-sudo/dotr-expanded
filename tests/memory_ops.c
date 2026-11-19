/* Host tests of the six copy/fill loops. long is 64-bit on the target and on
 * the LP64 development host; other hosts are not covered. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/memory_ops.h"

/* Scratch for 0x20b840; 16 bytes cover the element sizes used here. */
unsigned char data_003261f0[16];

static int order_calls, scratch_first;

static int by_word(unsigned char *a, unsigned char *b)
{
    int x, y;
    order_calls++;
    if (a == data_003261f0) scratch_first++;
    memcpy(&x, a, sizeof(x));
    memcpy(&y, b, sizeof(y));
    return x < y ? -1 : x > y;
}

/* Twelve-byte elements ordered by their last byte only. */
static int by_last_byte(unsigned char *a, unsigned char *b)
{
    order_calls++;
    return (int)a[11] - (int)b[11];
}

/* RhMath_HeapSortStruct (loop coverage: counts -1, 0 and 1 leave the data and make no
 * comparison; 2 in both orders; every count to 33 with ascending, descending,
 * equal and mixed keys, checked for order and for the same multiset; a wider
 * element whose other bytes must travel with the key). 0x20b300 is real. */
static void check_sort(void)
{
    int words[35], count, pattern, i, sum, want;
    unsigned char wide[10][12];

    for (count = -1; count <= 1; count++) {
        words[0] = 7; words[1] = 3; words[2] = 5;
        order_calls = 0;
        RhMath_HeapSortStruct((unsigned char *)&words[1], count, 4, by_word);
        assert(order_calls == 0 && words[0] == 7 && words[1] == 3 && words[2] == 5);
    }
    words[0] = 9; words[1] = 4;
    RhMath_HeapSortStruct((unsigned char *)words, 2, 4, by_word);
    assert(words[0] == 4 && words[1] == 9);
    RhMath_HeapSortStruct((unsigned char *)words, 2, 4, by_word);
    assert(words[0] == 4 && words[1] == 9);
    for (count = 2; count <= 33; count++) {
        for (pattern = 0; pattern < 4; pattern++) {
            want = 0;
            words[0] = -1000;
            words[count + 1] = 1000;
            for (i = 1; i <= count; i++) {
                words[i] = pattern == 0 ? i : pattern == 1 ? -i : pattern == 2 ? 6 : (i * 37 + count) % 11 - 5;
                want += words[i];
            }
            order_calls = scratch_first = 0;
            RhMath_HeapSortStruct((unsigned char *)&words[1], count, 4, by_word);
            sum = words[1];
            for (i = 2; i <= count; i++) {
                assert(words[i - 1] <= words[i]);
                sum += words[i];
            }
            /* neighbours untouched, same total, comparisons were made */
            assert(sum == want && words[0] == -1000 && words[count + 1] == 1000);
            assert(order_calls > 0 && scratch_first > 0);
        }
    }
    /* element 0 is a guard: the function forms base - size */
    memset(wide[0], 0xee, 12);
    for (i = 0; i < 9; i++) {
        memset(wide[i + 1], 0x40 + i, 11);
        wide[i + 1][11] = (unsigned char)((i * 5) % 9);
    }
    RhMath_HeapSortStruct(wide[1], 9, 12, by_last_byte);
    assert(wide[0][0] == 0xee && wide[0][11] == 0xee);
    for (i = 0; i < 9; i++) {
        /* key k came from element (k * 2) % 9 since 5 * 2 = 1 mod 9 */
        assert(wide[i + 1][11] == i && wide[i + 1][0] == 0x40 + (i * 2) % 9 && wide[i + 1][10] == wide[i + 1][0]);
    }
}

int main(void)
{
    unsigned char bytes[8], source_bytes[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int words[6], source_words[6] = {10, -20, 30, -40, 50, -60};
    long longs[4];
    DotrWord128 wide[3], source_wide[3];
    int count, i;
    assert(sizeof(long) == 8 && sizeof(DotrWord128) == 16);
    check_sort();
    for (count = -2; count <= 6; count++) {
        int n = count > 0 ? count : 0;
        /* Non-positive counts write nothing and return the destination. */
        memset(bytes, 0xee, sizeof(bytes));
        assert(RhMem_CopyMemory(bytes + 1, source_bytes, count) == bytes + 1 + n);
        assert(bytes[0] == 0xee && bytes[1 + n] == 0xee);
        for (i = 0; i < n; i++) assert(bytes[1 + i] == source_bytes[i]);
        memset(bytes, 0xee, sizeof(bytes));
        assert(RhMem_FillMemory(bytes + 1, 0x1a5, count) == bytes + 1 + n);
        assert(bytes[0] == 0xee && bytes[1 + n] == 0xee);
        for (i = 0; i < n; i++) assert(bytes[1 + i] == 0xa5);
    }
    for (count = -1; count <= 4; count++) {
        int n = count > 0 ? count : 0;
        for (i = 0; i < 6; i++) words[i] = 777;
        assert(RhMem_CopyMemory4(words + 1, source_words, count) == words + 1 + n);
        assert(words[0] == 777 && words[1 + n] == 777);
        for (i = 0; i < n; i++) assert(words[1 + i] == source_words[i]);
        for (i = 0; i < 6; i++) words[i] = 777;
        assert(RhMem_FillMemory4(words + 1, -9, count) == words + 1 + n);
        assert(words[0] == 777 && words[1 + n] == 777);
        for (i = 0; i < n; i++) assert(words[1 + i] == -9);
    }
    for (i = 0; i < 4; i++) longs[i] = 5;
    assert(RhMem_FillMemory8(longs + 1, 0x123456789abcdef0L, 2) == longs + 3);
    assert(longs[0] == 5 && longs[1] == 0x123456789abcdef0L && longs[2] == longs[1] && longs[3] == 5);
    assert(RhMem_FillMemory8(longs, 1, 0) == longs && longs[0] == 5);
    memset(wide, 0x11, sizeof(wide));
    memset(source_wide, 0xc3, sizeof(source_wide));
    assert(RhMem_CopyMemory16(wide, source_wide, 2) == wide + 2);
    assert(memcmp(wide, source_wide, 2 * sizeof(wide[0])) == 0);
    assert(((unsigned char *)&wide[2])[0] == 0x11 && ((unsigned char *)&wide[2])[15] == 0x11);
    puts("memory_ops: ok");
    return 0;
}
