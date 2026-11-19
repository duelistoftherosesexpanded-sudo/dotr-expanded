/* Synthetic host tests for the model_merge group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/model_merge.h"

struct DotrMergeTable data_00292ca0_merge;
struct DotrMergeTable data_00292d40_merge;
struct DotrMergeTable data_00292de0_merge;
struct DotrMergeOffsets data_00292e80_merge;
unsigned short gSzModel_awIgnore_merge[8];
struct DotrMergeObject *gSzModel_ahModel_merge[32];

static struct DotrMergeObject objs[32];
static int offsets[4] = {0, 8, 16, 24};
static char data[64];

static char log_[64];
static int calls;

static void said(char c, int index)
{
    assert(calls + 2 < (int)sizeof log_);
    log_[calls++] = c;
    log_[calls++] = (char)('0' + index);
    log_[calls] = 0;
}

static void *bound_addr[8];
static int bound_n;

int lSzModel_Clean(int index)
{
    said('C', index);
    return 0;
}
int lSzModel_BindModelByAddr(int index, void *addr)
{
    said('B', index);
    bound_addr[bound_n++] = addr;
    return 0;
}

static void put(int off, int v)
{
    memcpy(data + off, &v, sizeof v);
}

/* Group 5: slots 3..6 (count 4), rebind limit 3, offsets 0/8/16/24. */
static void reset(void)
{
    int i;
    memset(&data_00292ca0_merge, 0, sizeof data_00292ca0_merge);
    memset(&data_00292d40_merge, 0, sizeof data_00292d40_merge);
    memset(&data_00292de0_merge, 0, sizeof data_00292de0_merge);
    memset(&data_00292e80_merge, 0, sizeof data_00292e80_merge);
    memset(objs, 0, sizeof objs);
    memset(gSzModel_ahModel_merge, 0, sizeof gSzModel_ahModel_merge);
    memset(data, 0, sizeof data);
    data_00292ca0_merge.v[5] = 3;
    data_00292d40_merge.v[5] = 4;
    data_00292de0_merge.v[5] = 3;
    data_00292e80_merge.v[5] = offsets;
    for (i = 0; i < 32; i++) {
        objs[i].kind = 2;
        objs[i].number = 5;
        *(unsigned char *)&objs[i].bits = 0xff;
        gSzModel_ahModel_merge[i] = &objs[i];
    }
    gSzModel_awIgnore_merge[2] = 6;
    for (i = 0; i < 4; i++)
        put(offsets[i], 0x304852);
    calls = 0;
    log_[0] = 0;
    bound_n = 0;
}

static unsigned char bits(int i)
{
    return *(unsigned char *)&objs[i].bits;
}

static struct DotrMergeRecord *request(void)
{
    static struct DotrMergeRecord req;
    memset(&req, 0, sizeof req);
    req.data = data;
    req.user = 5;
    return &req;
}

static void check_failed_read(void)
{
    reset();
    gSzModel_ahModel_merge[4] = NULL;
    lSzModel_LoadMergeCB(request(), 0);
    assert(calls == 0);
    assert(bits(2) == 0xff && bits(7) == 0xff);
    assert(bits(3) == 0xf7 && bits(5) == 0xf7 && bits(6) == 0xf7);
    assert(bits(4) == 0xff); /* the null slot's object is untouched */
}

static void check_all_true(void)
{
    reset();
    lSzModel_LoadMergeCB(request(), 1);
    /* j 0..2 rebound (limit 3); j 3 (slot 6) only loses bit 3. */
    assert(strcmp(log_, "C3B3C4B4C5B5") == 0);
    assert(bound_n == 3 && bound_addr[0] == data && bound_addr[1] == data + 8 && bound_addr[2] == data + 16);
    assert(bits(3) == 0xf6 && bits(4) == 0xf6 && bits(5) == 0xf6);
    assert(bits(6) == 0xf7 && bits(7) == 0xff);
}

static void check_conditions(void)
{
    /* number not below the kind's limit */
    reset();
    objs[3].number = 6;
    lSzModel_LoadMergeCB(request(), 1);
    assert(strcmp(log_, "C4B4C5B5") == 0 && bits(3) == 0xf7);

    /* the kind byte selects the limit */
    reset();
    objs[4].kind = 1; /* limit 0 */
    lSzModel_LoadMergeCB(request(), 1);
    assert(strcmp(log_, "C3B3C5B5") == 0 && bits(4) == 0xf7);

    /* the data word differs */
    reset();
    put(16, 0x304853);
    lSzModel_LoadMergeCB(request(), 1);
    assert(strcmp(log_, "C3B3C4B4") == 0 && bits(5) == 0xf7);

    /* null slot skipped */
    reset();
    gSzModel_ahModel_merge[3] = NULL;
    lSzModel_LoadMergeCB(request(), 1);
    assert(strcmp(log_, "C4B4C5B5") == 0 && bits(3) == 0xff);

    /* rebind limit 4 = count: every slot qualifies */
    reset();
    data_00292de0_merge.v[5] = 4;
    lSzModel_LoadMergeCB(request(), 1);
    assert(strcmp(log_, "C3B3C4B4C5B5C6B6") == 0 && bound_addr[3] == data + 24);

    /* count 0: nothing */
    reset();
    data_00292d40_merge.v[5] = 0;
    lSzModel_LoadMergeCB(request(), 1);
    assert(calls == 0 && bits(3) == 0xff);
}

int main(void)
{
    check_failed_read();
    check_all_true();
    check_conditions();
    puts("model_merge: ok");
    return 0;
}
