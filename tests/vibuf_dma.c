/* Synthetic host tests for the vibuf_dma group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. The leave stub stands in for the hardware: it
 * loads the channel-4 control and address words the routine then reads. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/vibuf_dma.h"

volatile int dotr_vibuf_host_regs[0x1000];
#define REG(a) dotr_vibuf_host_regs[((a) & 0xffff) >> 4]
char data_0030f890[] = "<not active>";

static char order[32];
static int calls, err_calls, err_arg, sema_wait, sema_signal, leaves;
static int hw_ctrl, hw_madr, written_ctrl[2], enable_during[2];

static void said(char c)
{
    assert(calls < (int)sizeof order - 1);
    order[calls++] = c;
    order[calls] = 0;
}

void ErrMessage(int a)
{
    said('E');
    err_calls++;
    err_arg = a;
}
void func_0017b920(int sema)
{
    said('W');
    sema_wait = sema;
}
void func_0017b900(int sema)
{
    said('S');
    sema_signal = sema;
}
void func_001843e0(void)
{
    said('(');
}
void func_00184428(void)
{
    said(')');
    assert(leaves < 2);
    written_ctrl[leaves] = REG(0x1000b400);
    enable_during[leaves] = REG(0x1000f590);
    leaves++;
    REG(0x1000b400) = hw_ctrl;
    REG(0x1000b410) = hw_madr;
}

static DotrViBufQuad area[8 * 128];
static DotrViBufQuad tags[10];
static struct DotrViBufDma s;

static unsigned word(const void *p)
{
    return (unsigned)(uintptr_t)p;
}

static void reset(int start, int count, int read)
{
    memset((void *)dotr_vibuf_host_regs, 0, sizeof dotr_vibuf_host_regs);
    memset(tags, 0x55, sizeof tags);
    memset(&s, 0, sizeof s);
    s.data = area;
    s.tag = tags;
    s.n = 8;
    s.dmaStart = start;
    s.dmaN = count;
    s.readBytes = read;
    s.sema = 77;
    s.isActive = 1;
    REG(0x1000f520) = 0x1201;
    calls = err_calls = err_arg = sema_wait = sema_signal = leaves = 0;
    written_ctrl[0] = written_ctrl[1] = enable_during[0] = enable_during[1] = 0;
    order[0] = 0;
}

static unsigned long tag_word(int i)
{
    unsigned long v;
    memcpy(&v, &tags[i], sizeof v);
    return v;
}
static int tag_untouched(int i)
{
    static unsigned char fill[16];
    memset(fill, 0x55, sizeof fill);
    return memcmp(&tags[i], fill, 16) == 0;
}
static int high_untouched(int i)
{
    return ((unsigned char *)&tags[i])[8] == 0x55 && ((unsigned char *)&tags[i])[15] == 0x55;
}
static unsigned long elem(int i)
{
    return (unsigned long)(word(area) + ((unsigned)i << 11)) << 32;
}
static int madr_of(int i)
{
    return (int)(word(area) + ((unsigned)i << 11));
}

static void check_inactive(void)
{
    reset(0, 0, 0);
    s.isActive = 0;
    assert(viBufAddDMA(&s) == 0);
    /* The semaphore is not signalled on this path. */
    assert(strcmp(order, "WE") == 0 && sema_wait == 77);
    assert(err_arg == (int)word(data_0030f890));
    assert(REG(0x1000b400) == 0 && tag_untouched(0));
}

static void check_end_tag(void)
{
    /* Address at the end tag after the last element: position 0. 5 + 3 consumed -> start 0,
     * count 0; nothing read, so no restart. */
    reset(5, 3, 100);
    hw_ctrl = 0x10000145;
    hw_madr = (int)(word(tags + 9) & 0x0fffffff);
    assert(viBufAddDMA(&s) == 1);
    assert(strcmp(order, "W()S") == 0 && sema_signal == 77);
    assert(written_ctrl[0] == 5 && enable_during[0] == 0x1201);
    assert(REG(0x1000f590) == 0x1201);
    assert(s.dmaStart == 0 && s.dmaN == 0 && s.readBytes == 100);
    {
        int i;
        for (i = 0; i < 10; i++)
            assert(tag_untouched(i));
    }
}

static void check_wrap(void)
{
    /* Start 6, two queued, address in element 7: one consumed -> start 7, count 1, write
     * index 0. Three new elements (6244 bytes) wrap from 0. */
    reset(6, 2, 3 * 2048 + 100);
    hw_ctrl = 0x10000044;
    hw_madr = madr_of(7) + 0x40;
    assert(viBufAddDMA(&s) == 1);
    assert(strcmp(order, "W()()S") == 0);
    assert(s.dmaStart == 7 && s.dmaN == 4 && s.readBytes == 100);
    assert(tag_word(7) == (elem(7) | 0x30000080) && high_untouched(7));
    assert(tag_word(0) == (elem(0) | 0x30000080) && high_untouched(0));
    assert(tag_word(1) == (elem(1) | 0x30000080));
    assert(tag_word(2) == (elem(2) | 0x80));
    assert(tag_untouched(3) && tag_untouched(6) && tag_untouched(8));
    /* Restart with tag id forced to 3 and bit 8. */
    assert(written_ctrl[1] == 0x30000144 && REG(0x1000b400) == hw_ctrl);
    assert(enable_during[1] == 0x1201 && REG(0x1000f590) == 0x1201);
}

static void check_single(void)
{
    /* One new element after an empty queue: start 2 consumed up to element 3; the previous
     * last tag (index 2) becomes REF, element 3 REFE. */
    reset(2, 1, 2048);
    hw_ctrl = (int)0xe0000001;
    hw_madr = madr_of(3);
    assert(viBufAddDMA(&s) == 1);
    assert(s.dmaStart == 3 && s.dmaN == 1 && s.readBytes == 0);
    assert(tag_word(2) == (elem(2) | 0x30000080));
    assert(tag_word(3) == (elem(3) | 0x80));
    assert(tag_untouched(4));
    assert(written_ctrl[1] == 0x30000101);
}

static void check_no_new_data(void)
{
    /* Nothing read but elements remain: restart keeps the channel's own tag id. */
    reset(1, 5, 2047);
    hw_ctrl = 0x10000045;
    hw_madr = madr_of(3);
    assert(viBufAddDMA(&s) == 1);
    assert(strcmp(order, "W()()S") == 0);
    assert(s.dmaStart == 3 && s.dmaN == 3 && s.readBytes == 2047);
    assert(written_ctrl[1] == 0x10000145);
    assert(tag_untouched(0) && tag_untouched(7));
}

/* Smoke test only (docs/DECOMPILATION.md, Tests). */
static void check_get_ts(void)
{
    /* viBufGetTs: one time stamp covering the decode position (channel 4 address 0x1000 with
     * the FIFO empty, data area at 0, bit pointer 0) is copied out and marked used. */
    static DotrViBufTimeStamp stamps[2];
    DotrViBufTs f;
    DotrViBufTimeStamp out;

    memset((void *)dotr_vibuf_host_regs, 0, sizeof dotr_vibuf_host_regs);
    memset(&f, 0, sizeof f);
    REG(0x1000b410) = 0x1000;
    f.n = 8;
    f.sema = 5;
    f.ts = stamps;
    f.n_ts = 2;
    f.count_ts = 1;
    f.wt_ts = 1;
    stamps[0].pts = 1234;
    stamps[0].dts = 1200;
    stamps[0].pos = 0x0f00;
    stamps[0].len = 0x200;
    assert(viBufGetTs(&f, &out) == 1);
    assert(out.pts == 1234 && out.dts == 1200 && stamps[0].pts == -1 && f.count_ts == 0);
}

int main(void)
{
    check_inactive();
    check_end_tag();
    check_wrap();
    check_single();
    check_no_new_data();
    check_get_ts();
    puts("vibuf_dma: ok");
    return 0;
}
