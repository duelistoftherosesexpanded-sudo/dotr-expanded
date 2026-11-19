/* Synthetic host tests for the text_load group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/text_load.h"

#define MAGIC 0x324852

unsigned short picpackArts[8] = {501, 502, 503, 504, 505, 506, 507, 508};

/* One log of every callee call in order. */
struct Call {
    char what; /* F free, C copy, B bind, A add */
    DotrTextLoadWord a;
    int b, c, d;
    void *p, *q;
};
static struct Call calls[32];
static int ncalls;

static struct Call *log_call(char what)
{
    assert(ncalls < 32);
    memset(&calls[ncalls], 0, sizeof calls[ncalls]);
    calls[ncalls].what = what;
    return &calls[ncalls++];
}

void SzMem_Free(DotrTextLoadWord address)
{
    log_call('F')->a = address;
}
void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    struct Call *c = log_call('C');
    c->p = destination;
    c->q = source;
    c->b = count;
    return destination;
}
void RhText_BindAddress(void *header)
{
    log_call('B')->p = header;
}
int SzText_AddFile(int group, int id, const char *name, DotrTextLoadWord value)
{
    struct Call *c = log_call('A');
    c->b = group;
    c->c = id;
    c->q = (void *)name;
    c->a = value;
    return 0;
}

static int buffer[0x10000 / 4];
static struct DotrTextLoadEntry entry;
static struct DotrTextLoadRecord rec;

static void reset(int id, int group, int byte_size)
{
    memset(buffer, 0, sizeof buffer);
    memset(&entry, 0, sizeof entry);
    memset(&rec, 0, sizeof rec);
    entry.id = (short)id;
    entry.group = (short)group;
    rec.data = buffer;
    rec.entry = &entry;
    rec.byte_size = byte_size;
    ncalls = 0;
}

/* A block at byte offset off: magic word and unit word at +8. */
static int *block(int off, int unit)
{
    int *b = (int *)((char *)buffer + off);
    b[0] = MAGIC;
    b[2] = unit;
    return b;
}

static void expect_add(int i, int group, int id, const void *bound)
{
    assert(calls[i].what == 'A');
    assert(calls[i].b == group);
    assert(calls[i].c == id);
    assert(calls[i].q == NULL);
    assert(calls[i].a == (DotrTextLoadWord)bound);
}

static void expect_bind(int i, const void *p)
{
    assert(calls[i].what == 'B');
    assert(calls[i].p == p);
}

static int *copy_at(long offset)
{
    return (int *)(intptr_t)(offset + 0xb1b000L);
}

static void check_failures(void)
{
    /* Not completed, nothing allocated: entry cleared, no call. */
    reset(0, 0, 0x100);
    block(0, 0x100);
    entry.unknown_000 = 77;
    lSzText_LoadFileCB(&rec, 0);
    assert(ncalls == 0 && entry.unknown_000 == 0 && entry.unknown_004 == 0);

    /* Not completed with an allocation at +4: freed and cleared. */
    reset(0, 0, 0x100);
    entry.unknown_004 = 0x1234;
    lSzText_LoadFileCB(&rec, 0);
    assert(ncalls == 1 && calls[0].what == 'F' && calls[0].a == 0x1234);
    assert(entry.unknown_004 == 0 && entry.unknown_000 == 0);

    /* Completed, bad magic, owned buffer: buffer freed, then the failure path. */
    reset(0, 0, 0x100);
    buffer[0] = MAGIC + 1;
    rec.owned = -1;
    entry.unknown_004 = 0x5678;
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 2);
    assert(calls[0].what == 'F' && calls[0].a == (DotrTextLoadWord)buffer);
    assert(calls[1].what == 'F' && calls[1].a == 0x5678);
    assert(entry.unknown_004 == 0 && entry.unknown_000 == 0);

    /* Completed, bad magic, not owned: only the failure path. */
    reset(0, 0, 0x100);
    buffer[0] = 0;
    entry.unknown_000 = 9;
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 0 && entry.unknown_000 == 0);
}

static void check_group(void)
{
    /* Group != 0: data stored and bound whole, no block walk. */
    reset(0x23, 5, 0x100);
    block(0, 0x100);
    lSzText_LoadFileCB(&rec, 1);
    assert(entry.unknown_000 == (DotrTextLoadWord)buffer);
    assert(ncalls == 1);
    expect_bind(0, buffer);
    /* A negative group is also nonzero. */
    reset(0x23, -1, 0x100);
    block(0, 0x100);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 1);
    expect_bind(0, buffer);
}

static void check_kind0(void)
{
    int *b1;

    /* Kind 0, start slot 0: first block bound in place, second copied to 0xb1b000 + unit,
     * step 0x4800, group 1. */
    reset(0x00, 0, 0x9000);
    block(0, 0x200);
    b1 = block(0x4800, 0x999);
    lSzText_LoadFileCB(&rec, 1);
    assert(entry.unknown_000 == (DotrTextLoadWord)buffer);
    assert(ncalls == 5);
    expect_bind(0, buffer);
    expect_add(1, 1, 0, buffer);
    assert(calls[2].what == 'C' && calls[2].p == copy_at(0x200) && calls[2].q == b1 && calls[2].b == 0x20);
    expect_bind(3, copy_at(0x200));
    expect_add(4, 1, 1, copy_at(0x200));

    /* Kind 0 with id >> 4 = 2: slot 200, the first block is already copied to
     * 0xb1b000 + 200 * unit; remain 0x4801 allows exactly two blocks. */
    reset(0x20, 0, 0x4801);
    block(0, 0x40);
    block(0x4800, 0x40);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 6);
    assert(calls[0].what == 'C' && calls[0].p == copy_at(200 * 0x40) && calls[0].q == buffer && calls[0].b == 4);
    expect_bind(1, copy_at(200 * 0x40));
    expect_add(2, 1, 200, copy_at(200 * 0x40));
    assert(calls[3].what == 'C' && calls[3].p == copy_at(201 * 0x40));
    expect_add(5, 1, 201, copy_at(201 * 0x40));
}

static void check_kind1(void)
{
    /* Kind 1: ids from the table by block number, step = unit of the first block; a
     * non-magic block is added unbound under its index. */
    reset(0x01, 0, 0x300);
    block(0, 0x100);
    block(0x200, 0x55);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 5);
    expect_bind(0, buffer);
    expect_add(1, 1, 501, buffer);
    expect_add(2, 1, 1, NULL);
    expect_bind(3, (char *)buffer + 0x200);
    expect_add(4, 1, 503, (char *)buffer + 0x200);
}

static void check_kind2(void)
{
    /* Kind 2: slot starts at 0 whatever the id, step 0x1000, group 2. */
    reset(0x52, 0, 0x2000);
    block(0, 0x80);
    block(0x1000, 0x80);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 5);
    expect_bind(0, buffer);
    expect_add(1, 2, 0, buffer);
    assert(calls[2].what == 'C' && calls[2].p == copy_at(0x80) && calls[2].b == 8);
    expect_add(4, 2, 1, copy_at(0x80));
}

static void check_other_kinds(void)
{
    static const int kinds[] = {3, 4, 5, 6, 7, 8, 9, 10, 15};
    size_t k;

    /* Kind 3: magic blocks step by their own unit word; ids are block numbers. */
    reset(0x03, 0, 0x180);
    block(0, 0x100);
    block(0x100, 0x80);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 4);
    expect_add(1, 3, 0, buffer);
    expect_bind(2, (char *)buffer + 0x100);
    expect_add(3, 3, 1, (char *)buffer + 0x100);

    /* Groups for every other kind (jump table 0x2d06e0): 3 and 4 add under their own number,
     * 5 under 7, 6 under 8, 7-9 under 9; 10-15 add nothing. */
    for (k = 0; k < sizeof kinds / sizeof kinds[0]; k++) {
        int kind = kinds[k];
        reset(kind, 0, 0x100);
        block(0, 0x100);
        lSzText_LoadFileCB(&rec, 1);
        expect_bind(0, buffer);
        if (kind <= 9) {
            assert(ncalls == 2);
            expect_add(1, kind <= 4 ? kind : kind == 5 ? 7 : kind == 6 ? 8 : 9, 0, buffer);
        } else {
            assert(ncalls == 1);
        }
    }
}

static void check_stops(void)
{
    /* A non-magic block with remain < unit stops the walk before any add. */
    reset(0x03, 0, 0x150);
    block(0, 0x100);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 2);
    expect_add(1, 3, 0, buffer);

    /* remain == unit for a non-magic block still adds it; then remain reaches 0. */
    reset(0x04, 0, 0x200);
    block(0, 0x100);
    lSzText_LoadFileCB(&rec, 1);
    assert(ncalls == 3);
    expect_add(2, 4, 1, NULL);
}

int main(void)
{
    check_failures();
    check_group();
    check_kind0();
    check_kind1();
    check_kind2();
    check_other_kinds();
    check_stops();
    puts("text_load: ok");
    return 0;
}
