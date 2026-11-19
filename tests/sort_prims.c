/* Synthetic host tests for the sort_prims group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/sort_prims.h"

#include <stdio.h>
#include <sys/mman.h>

/* gExPrim_aaRegs (0x293d80) as the byte view the C declares; 0x70 bytes per context
 * (seven 16-byte pairs on host and target). */
unsigned char gExPrim_aaRegs_sortprim[3 * 0x70] __attribute__((aligned(16)));
#define gExPrim_aaRegs ((struct DotrDrawPair (*)[7])gExPrim_aaRegs_sortprim)
static int zbuf_calls;
/* Stub: fills the two fields the real routine writes. */
void func_0020a280_sortprim(void *arg0)
{
    struct DotrDrawFields *fields = arg0;

    zbuf_calls++;
    fields->low_9 = 0x123;
    fields->low_4 = 5;
}

static unsigned char *low_page;
static unsigned long *buf;
static struct DotrSortHeader *header;
static DotrDrawChain *chain;
static DotrDrawSlot slots[16];
/* The C addresses slot i's head as a pointer at byte i * 8 + 4 (pointer-sized on host). */
static void *slot_head(int i)
{
    void *p;

    memcpy(&p, (char *)slots + i * 8 + 4, sizeof(p));
    return p;
}
static void set_slot(int i, void *head)
{
    memcpy((char *)slots + i * 8 + 4, &head, sizeof(head));
}

static int run(int type, int count, unsigned long regs, int linked, int z)
{
    int i;
    DotrDrawWord end;

    for (i = 0; i < 48; i++) {
        buf[i] = 0xa5a5a5a5a5a5a5a5UL;
    }
    header->count = (unsigned short)count;
    header->type = (unsigned short)type;
    header->regs = regs;
    for (i = 0; i < 8; i++) {
        header->data[i] = 0xd0 + i;
    }
    zbuf_calls = 0;
    end = ExSortPrim_Pipe((DotrDrawWord)buf, (int)(DotrDrawWord)header, linked ? (int)(DotrDrawWord)chain : 0, z);
    return (int)((unsigned long *)end - buf);
}

static void assert_tag(int words, unsigned int next)
{
    struct DotrSortTag *tag = (struct DotrSortTag *)buf;
    int size = words / 2 - 1;

    assert(tag->size == size && tag->unknown_02 == 0 && tag->kind == 0x10 && (unsigned int)tag->next == next);
    assert(tag->unknown_08 == 0x11000000 && tag->unknown_0c == (0x51000000 | size));
    assert(buf[2] == (0x1000000000008000UL | (unsigned long)(size - 1)) && buf[3] == 0xe && buf[5] == 0);
    assert(buf[words] == 0xa5a5a5a5a5a5a5a5UL);
}

/* ExSortPrim_Pipe (full test). One case per branch outcome: type 7; no
 * chain, a chain with the last-slot bit and the slot bound at size - 2 (below, equal, above);
 * every flag bit of the primitive word; the optional pairs (0x200, 0x20, 0x4000 with and
 * without 0x20, 0x8000); list lengths 0, 1 and 3; register 2 kept and replaced under 0x800; the
 * closing pairs and the tag fields. Header, chain and buffer sit in a page below 4 GiB because
 * the prototype passes two of them as ints. */
static void check_sort_prim(void)
{
    DotrDrawNode old = {0, 0};
    struct DotrSortZBuf zbuf;
    int i;

    low_page = mmap(0, 8192, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    assert(low_page != MAP_FAILED);
    buf = (unsigned long *)low_page;
    header = (struct DotrSortHeader *)(low_page + 4096);
    chain = (DotrDrawChain *)(low_page + 6144);
    chain->slots = slots;
    chain->shift = 3;
    for (i = 0; i < 7; i++) {
        gExPrim_aaRegs[0][i].unknown_00 = 0x8000 + i;
        gExPrim_aaRegs[0][i].unknown_08 = 0x8100 + i;
        gExPrim_aaRegs[1][i].unknown_00 = 0x9000 + i;
        gExPrim_aaRegs[1][i].unknown_08 = 0x9100 + i;
    }

    assert(run(0x8207, 2, 0x21, 1, 1) == 0 && buf[0] == 0xa5a5a5a5a5a5a5a5UL && slot_head(7) == 0 && zbuf_calls == 0);

    /* Empty list, no flags: tag, register tag, primitive word and the fixed pair only. */
    assert(run(0, 0, 0x21, 0, 0) == 8);
    assert_tag(8, 0);
    assert(buf[4] == 0 && buf[6] == 0x5000c && buf[7] == 0x47);
    /* Three list words with register numbers 1, 2 and 5 from the packed word. */
    assert(run(0, 3, 0x521, 0, 0) == 14);
    assert_tag(14, 0);
    assert(buf[8] == 0xd0 && buf[9] == 1 && buf[10] == 0xd1 && buf[11] == 2 && buf[12] == 0xd2 && buf[13] == 5);
    /* Bit 0x800 turns register 2 into 3 and leaves the others. */
    assert(run(0x0800, 3, 0x521, 0, 0) == 14 && buf[4] == 0x100);
    assert(buf[9] == 1 && buf[11] == 3 && buf[13] == 5);

    /* Each flag bit of the primitive word on its own (one list word). */
    assert(run(0x0005, 1, 1, 0, 0) == 10 && buf[4] == 5);
    assert(run(0x0010, 1, 1, 0, 0) == 10 && buf[4] == 0x008);
    assert(run(0x0020, 1, 1, 0, 0) == 14 && buf[4] == 0x010);
    assert(buf[8] == 0xd2 && buf[9] == 6 && buf[10] == 0xd3 && buf[11] == 0x14 && buf[12] == 0xd0 && buf[13] == 1);
    assert(run(0x0100, 1, 1, 0, 0) == 10 && buf[4] == 0x020);
    assert(run(0x0200, 1, 1, 0, 0) == 12 && buf[4] == 0x040 && buf[6] == 0x5000d);
    assert(buf[8] == 0xd1 && buf[9] == 0x42 && buf[10] == 0xd0 && buf[11] == 1);
    assert(run(0x0400, 1, 1, 0, 0) == 10 && buf[4] == 0x080);
    assert(run(0x1000, 1, 1, 0, 0) == 10 && buf[4] == 0x200 && buf[7] == 0x48);
    assert(run(0x2000, 1, 1, 0, 0) == 10 && buf[4] == 0x400);
    assert(run(0x4000, 1, 1, 0, 0) == 10 && buf[4] == 0);

    /* Slot bound: below, at and above size - 2 = 6; the old head goes into the tag. */
    set_slot(2, &old);
    assert(run(0, 1, 1, 1, 2) == 10 && slot_head(2) == (void *)buf);
    assert_tag(10, (unsigned int)(DotrDrawWord)&old);
    assert(run(0, 1, 1, 1, 6) == 10 && slot_head(6) == (void *)buf);
    assert_tag(10, 0);
    set_slot(6, &old);
    assert(run(0, 1, 1, 1, 7) == 10 && slot_head(6) == (void *)buf && slot_head(7) == 0);
    assert_tag(10, (unsigned int)(DotrDrawWord)&old);
    assert(run(0, 1, 1, 1, -1) == 10 && slot_head(6) == (void *)buf && slot_head(7) == 0);

    /* Everything but 0x800, context 1, last slot through bit 0x8000, two list words. */
    set_slot(7, &old);
    assert(run(0xf733, 2, 0x21, 1, 1) == 26 && slot_head(7) == (void *)buf && slot_head(1) == 0);
    assert_tag(26, (unsigned int)(DotrDrawWord)&old);
    assert(buf[4] == 0x6fb && buf[6] == 0x5000d && buf[7] == 0x48);
    assert(buf[8] == 0xd2 && buf[9] == 0x43 && buf[10] == 0xd3 && buf[11] == 7);
    assert(buf[12] == 0xd4 && buf[13] == 0x15 && buf[14] == 0xd5 && buf[15] == 9);
    memcpy(&zbuf, &buf[16], sizeof(zbuf));
    assert(zbuf.fields.low_9 == 0x123 && zbuf.fields.low_4 == 5 && zbuf.mask == 0 && buf[17] == 0x4f);
    assert(buf[18] == 0xd0 && buf[19] == 1 && buf[20] == 0xd1 && buf[21] == 2);
    assert(buf[22] == 0x9004 && buf[23] == 0x9104);
    memcpy(&zbuf, &buf[24], sizeof(zbuf));
    assert(zbuf.fields.low_9 == 0x123 && zbuf.fields.low_4 == 5 && zbuf.mask == 1 && buf[25] == 0x4f);
    assert(zbuf_calls == 1);
    /* Context 0 takes the other row and the lower register numbers. */
    assert(run(0x4020, 1, 1, 0, 0) == 18 && buf[12] == 0xd4 && buf[13] == 8);
    assert(buf[14] == 0xd0 && buf[15] == 1 && buf[16] == 0x8004 && buf[17] == 0x8104);
    assert(run(0x8000, 1, 1, 0, 0) == 14 && buf[9] == 0x4e && buf[13] == 0x4e);
}

int main(void)
{
    check_sort_prim();
    puts("sort_prims: ok");
    return 0;
}
