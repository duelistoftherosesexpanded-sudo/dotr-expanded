/* Synthetic host tests for the model_bind group. Fixtures are host data, not game data. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/model_bind.h"

struct DotrModelPad dotr_model_pad;

static union {
    int words[256];
    struct DotrPatchHead head;
} block;
static union {
    DotrModelAddress slots[8];
    int words[32];
    unsigned char bytes[128];
} first, second;

/* Branch coverage: both list selections; list words tagged 0x4, 0x2 and other; slot words
 * tagged 0x4, 0x2 and other; the -1 terminator. */
static void check_bind(int flag)
{
    struct DotrModelHead *head = DOTR_MODEL_BLOCK_HEAD(&block);
    int *list = &block.words[flag ? 80 : 64];
    int *other = &block.words[flag ? 64 : 80];
    DotrModelAddress s = sizeof(DotrModelAddress);

    memset(&block, 0, sizeof(block));
    memset(&second, 0, sizeof(second));
    assert((unsigned char *)head == (unsigned char *)&block + 16);
    assert(offsetof(struct DotrModelHead, bind_list) == 0x30 && offsetof(struct DotrModelHead, bind_list_second) == 0x38);
    head->bind_list = 64 * 4;
    head->bind_list_second = 80 * 4;
    other[0] = 0x40000000 | (int)(7 * s);
    other[1] = -1;
    list[0] = 0x40000000 | (int)(0 * s);
    list[1] = 0x20000000 | (int)(1 * s);
    list[2] = 0x10000000 | (int)(2 * s);
    list[3] = (int)(3 * s);
    list[4] = 0x40000000 | (int)(4 * s);
    list[5] = 0x20000000 | (int)(5 * s);
    list[6] = -1;
    list[7] = 0x40000000 | (int)(6 * s);
    second.slots[0] = 0x40000000 | 0x30;
    second.slots[1] = 0x20000000 | 0x44;
    second.slots[2] = 0x40000000 | 0x50;
    second.slots[3] = 0x20000000 | 0x60;
    second.slots[4] = 0x10000000 | 0x70;
    second.slots[5] = 0x78;
    second.slots[6] = 0x40000000 | 0x10;
    second.slots[7] = 0x40000000 | 0x14;
    lRhModel_DynamicAddressBind(&block.head, first.bytes, second.bytes, flag);
    assert(second.slots[0] == (DotrModelAddress)(first.bytes + 0x30));
    assert(second.slots[1] == (DotrModelAddress)(first.bytes + 0x44));
    assert(second.slots[2] == (0x40000000 | 0x50) && second.slots[3] == (0x20000000 | 0x60));
    assert(second.slots[4] == (0x10000000 | 0x70) && second.slots[5] == 0x78);
    assert(second.slots[6] == (0x40000000 | 0x10) && second.slots[7] == (0x40000000 | 0x14));
}

static int cleared(const int *w)
{
    return w[0] == 0x10000000 && w[1] == 0 && w[2] == 0 && w[3] == 0;
}

static int untouched(const int *w)
{
    return w[0] == 0x5a5a5a5a && w[1] == 0x5a5a5a5a && w[2] == 0x5a5a5a5a && w[3] == 0x5a5a5a5a;
}

/* Branch coverage: absent table, disabled, mask from the argument and from the table, a
 * masked list, a zero offset, lists 0 and 31, tags 0x2 / 0x4 / other, a null second copy. */
static void check_option(int has_table, int enabled, int flag, int with_second)
{
    struct DotrModelHead *head = DOTR_MODEL_BLOCK_HEAD(&block);
    struct DotrModelOptionTable *table = (struct DotrModelOptionTable *)&block.words[100];
    int *lists = &block.words[140];
    int ran = has_table && enabled;

    memset(&block, 0, sizeof(block));
    memset(&first, 0x5a, sizeof(first));
    memset(&second, 0x5a, sizeof(second));
    assert(offsetof(struct DotrModelHead, option_table) == 0x48 && offsetof(struct DotrModelHead, option_enabled) == 0x4c);
    assert(offsetof(struct DotrModelOptionTable, mask) == 0x80);
    head->option_table = has_table ? 100 * 4 : 0;
    head->option_enabled = enabled;
    /* list 0: tag 2 at first+0, other tag, tag 4 at second+16 */
    table->list[0] = 40 * 4;
    lists[0] = 0x20000000 | 0;
    lists[1] = 0x30000000 | 64;
    lists[2] = 0x40000000 | 16;
    lists[3] = 0;
    lists[4] = 0x20000000 | 96;
    /* list 5: masked either way */
    table->list[5] = 48 * 4;
    lists[8] = 0x20000000 | 32;
    lists[9] = 0;
    /* list 6: masked by the argument only */
    table->list[6] = 52 * 4;
    lists[12] = 0x20000000 | 48;
    lists[13] = 0;
    /* list 7: masked by the table only */
    table->list[7] = 56 * 4;
    lists[16] = 0x40000000 | 48;
    lists[17] = 0;
    /* list 31 */
    table->list[31] = 60 * 4;
    lists[20] = 0x40000000 | 80;
    lists[21] = 0x20000000 | 80;
    lists[22] = 0;
    table->mask = (1 << 5) | (1 << 7);
    lRhModel_SetupOptionObject(&block.head, first.bytes, with_second ? second.bytes : NULL, flag, (1 << 5) | (1 << 6));
    assert(ran ? cleared(&first.words[0]) : untouched(&first.words[0]));
    assert(untouched(&first.words[16]) && untouched(&first.words[24]) && untouched(&first.words[8]));
    assert(ran && with_second ? cleared(&second.words[4]) : untouched(&second.words[4]));
    assert(ran && flag ? cleared(&first.words[12]) : untouched(&first.words[12]));
    assert(ran && !flag && with_second ? cleared(&second.words[12]) : untouched(&second.words[12]));
    assert(ran && with_second ? cleared(&second.words[20]) : untouched(&second.words[20]));
    assert(ran ? cleared(&first.words[20]) : untouched(&first.words[20]));
    assert(untouched(&second.words[0]) && untouched(&second.words[8]) && untouched(&second.words[16]));
}

int main(void)
{
    check_bind(0);
    check_bind(1);
    check_option(0, 1, 0, 1);
    check_option(1, 0, 0, 1);
    check_option(1, 1, 0, 1);
    check_option(1, 1, 1, 1);
    check_option(1, 1, 0, 0);
    check_option(1, 1, 1, 0);
    puts("model_bind: ok");
    return 0;
}
