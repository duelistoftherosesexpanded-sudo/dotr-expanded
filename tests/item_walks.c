/* Synthetic host tests for the item_walks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/item_walks.h"

struct DotrWalkPad dotr_walk_pad;
static unsigned words[24];
static int calls, kinds[4];
static unsigned *items[4], *nexts[4];

static int take(int kind, int cursor, unsigned *item)
{
    kinds[calls] = kind;
    items[calls] = item;
    dotr_walk_pad.next = nexts[calls];
    calls++;
    return cursor + kind;
}
int lRhModelCpu_CalcIndepStrip(int cursor, unsigned *item) { return take(1, cursor, item); }
int lRhModelCpu_CalcSharedStrip(int cursor, unsigned *item) { return take(0x100, cursor, item); }

/* lRhModelCpu_TraceStaticBlock (branch and loop coverage): empty range; each callee; the extra
 * word on bit 15 of the second word; zero words skipped (none, one, two);
 * the walk stops at the end bound. */
static void check_walk(void)
{
    calls = 0;
    assert(lRhModelCpu_TraceStaticBlock(7, words, 0) == 7 && calls == 0);

    memset(words, 0, sizeof(words));
    words[0] = 1;            /* record A: plain, next taken as given */
    words[4] = 1;            /* record B at +4: fourth word flagged, second word flagged */
    words[5] = 0x8000;
    words[7] = 0x8000;
    words[11] = 1;           /* record C at +11: reached over one extra word and two zero words */
    words[16] = 1;           /* past the end bound */
    nexts[0] = &words[4];
    nexts[1] = &words[8];    /* +1 for the flag = 9, then zero words 9 and 10 */
    nexts[2] = &words[15];   /* one zero word, lands on 16 = the end */
    calls = 0;
    assert(lRhModelCpu_TraceStaticBlock(0x1000, words, 16 * (int)sizeof(unsigned)) == 0x1102 && calls == 3);
    assert(kinds[0] == 1 && kinds[1] == 0x100 && kinds[2] == 1);
    assert(items[0] == &words[0] && items[1] == &words[4] && items[2] == &words[11]);
}

struct DotrWalkSortPad dotr_walk_sort_pad;
static int copies;
void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    copies++;
    memmove(destination, source, (size_t)count * 16);
    return destination;
}

/* lRhModelCpu_SortPolygon (branch and loop coverage): empty and one-record ranges make no copy; two
 * records in both orders; three records (heap build with one child and with two); eight
 * records in rising, falling and mixed order with equal keys and a key above 0x7fffffff
 * (unsigned compare). Each run checks ascending keys, that every record's first word
 * travelled with its key, and that the record after the range is untouched. */
static void check_sort(void)
{
    static const unsigned keys[7][8] = {
        {5, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 0, 0, 0, 0, 0, 0}, {2, 1, 0, 0, 0, 0, 0, 0},
        {3, 1, 2, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 6, 7, 8}, {8, 7, 6, 5, 4, 3, 2, 1},
        {4, 0x80000000u, 4, 0, 9, 1, 0xffffffffu, 2}
    };
    static const int counts[7] = {1, 2, 2, 3, 8, 8, 8};
    static struct DotrWalkPolygon records[9];
    int run, i;

    copies = 0;
    lRhModelCpu_SortPolygon(records, records);
    lRhModelCpu_SortPolygon(records, records + 1);
    assert(copies == 0);
    for (run = 0; run < 7; run++) {
        int n = counts[run];

        memset(records, 0, sizeof(records));
        for (i = 0; i < n; i++) {
            records[i].key = keys[run][i];
            records[i].unknown_00[0] = (int)(keys[run][i] ^ 0x5a5a);
            records[i].unknown_00[30] = i;
        }
        records[n].key = 0;
        records[n].unknown_00[0] = 0x777;
        copies = 0;
        lRhModelCpu_SortPolygon(records, records + n);
        assert((n > 1) == (copies != 0));
        for (i = 0; i < n; i++) {
            assert(i == 0 || records[i - 1].key <= records[i].key);
            assert(records[i].unknown_00[0] == (int)(records[i].key ^ 0x5a5a));
            assert(keys[run][records[i].unknown_00[30]] == records[i].key);
        }
        assert(records[n].unknown_00[0] == 0x777 && records[n].key == 0);
    }
}

int main(void)
{
    check_walk();
    check_sort();
    puts("item_walks: ok");
    return 0;
}
