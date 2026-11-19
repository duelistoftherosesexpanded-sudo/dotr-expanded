/* Synthetic host tests for the patch_lists group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/patch_lists.h"

/* The only callee is a stub that records its arguments. */
static struct { void *destination, *source; } copies[8];
static int copy_count;

void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    assert(count == 2 && copy_count < 8);
    copies[copy_count].destination = destination;
    copies[copy_count].source = source;
    copy_count++;
    return destination;
}

static struct {
    struct DotrPatchHead head;
    struct DotrPatchList list;
    struct DotrPatchItem items[3];
    struct DotrPatchRun runs[3];
    unsigned int words[6];
    struct DotrPatchPick pick;
} block;
static struct DotrPatchObject objects[3];
static unsigned char first[64], second[64];

static void *source_of(int object)
{
    return (unsigned char *)&objects[object] + 0x30;
}

static void run(int index)
{
    copy_count = 0;
    lRhModel_SetupTexture(&block.head, first, second, index);
}

static void fill(void)
{
    int i;

    memset(&block, 0, sizeof(block));
    assert(offsetof(struct DotrPatchHead, sub.has_list) == 0x3c);
    assert((unsigned char *)block.items == (unsigned char *)(&block.list + 1));
    block.head.sub.list_offset = (int)offsetof(__typeof__(block), list);
    block.head.sub.has_list = 1;
    block.list.count = 3;
    block.list.runs_offset = (int)((unsigned char *)block.runs - (unsigned char *)&block.list);
    for (i = 0; i < 3; i++) {
        block.items[i].object = &objects[i];
    }
    /* item 0: tags 2, 4 and an ignored tag; item 1: no words; item 2: tag 4 */
    block.words[0] = 0x20000010;
    block.words[1] = 0x40000020;
    block.words[2] = 0x10000030;
    block.words[3] = 0x40000008;
    block.runs[0].count = 3;
    block.runs[0].words_offset = (int)((unsigned char *)&block.words[0] - (unsigned char *)&block.list);
    block.runs[1].count = 0;
    block.runs[1].words_offset = (int)((unsigned char *)&block.words[3] - (unsigned char *)&block.list);
    block.runs[2].count = 1;
    block.runs[2].words_offset = (int)((unsigned char *)&block.words[3] - (unsigned char *)&block.list);
}

static void set_pick(int key, int choice)
{
    block.head.sub.pick_offset = (int)offsetof(__typeof__(block), pick);
    block.head.sub.has_pick = 1;
    block.pick.key = key;
    block.pick.choice[5] = choice;
}

static void expect_plain(int last_object)
{
    assert(copy_count == 3);
    assert(copies[0].destination == first + 0x10 && copies[0].source == source_of(0));
    assert(copies[1].destination == second + 0x20 && copies[1].source == source_of(0));
    assert(copies[2].destination == second + 8 && copies[2].source == source_of(last_object));
}

/* lRhModel_SetupTexture (branch and loop coverage). */
static void check_patch(void)
{
    /* header word zero: nothing */
    fill();
    block.head.sub.has_list = 0;
    run(5);
    assert(copy_count == 0);
    /* empty list: nothing */
    fill();
    block.list.count = 0;
    run(5);
    assert(copy_count == 0);
    /* no selection record: each item in turn; tag 2, tag 4, other tag, empty run */
    fill();
    run(5);
    expect_plain(2);
    /* either selection word zero: the same */
    set_pick(2, 0);
    block.head.sub.has_pick = 0;
    run(5);
    expect_plain(2);
    set_pick(2, 0);
    block.head.sub.pick_offset = 0;
    run(5);
    expect_plain(2);
    /* selection applies to item 2 only: choice 0, and the upper bound count - 1 */
    set_pick(2, 0);
    run(5);
    expect_plain(0);
    set_pick(2, 1);
    run(5);
    expect_plain(1);
    set_pick(2, 2);
    run(5);
    expect_plain(2);
    /* choice equal to the count, or negative: the item itself */
    block.items[2].object = &objects[1];
    set_pick(2, 3);
    run(5);
    expect_plain(1);
    set_pick(2, -1);
    run(5);
    expect_plain(1);
    /* negative key, negative index: the item itself */
    set_pick(-1, 0);
    run(5);
    expect_plain(1);
    set_pick(2, 0);
    run(-1);
    expect_plain(1);
    /* key 0: item 0 takes the choice, item 2 does not; key boundary 0 is accepted */
    block.items[2].object = &objects[2];
    set_pick(0, 1);
    run(5);
    assert(copy_count == 3 && copies[0].source == source_of(1) && copies[1].source == source_of(1));
    assert(copies[0].destination == first + 0x10 && copies[2].source == source_of(2));
}

int main(void)
{
    check_patch();
    return 0;
}
