/* Synthetic host tests for the mpeg_video group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/mpeg_video.h"

struct DotrMvDecoder {
    int unknown;
};
struct DotrMvDecoder data_00350680_mv;
char data_0030f5e0_mv[] = "pts buffer overflow\n";

static struct DotrMvReadBuf ring;
static char area0[64], area1[64];

static char order[16];
static int calls;
static void said(char c)
{
    assert(calls < (int)sizeof order - 1);
    order[calls++] = c;
    order[calls] = 0;
}

int func_002450d0_mv(struct DotrMvDecoder *vd, char **pd0, int *d0, char **pd1, int *d1)
{
    said('B');
    assert(vd == &data_00350680_mv);
    *pd0 = area0;
    *d0 = 40;
    *pd1 = area1;
    *d1 = 24;
    return 0;
}

static struct {
    char *pd0, *pd1, *ps0, *ps1;
    int d0, d1, s0, s1;
} copy;
static int copy_result;
int func_00242f20_mv(char *pd0, int d0, char *pd1, int d1, char *ps0, int s0, char *ps1, int s1)
{
    said('C');
    copy.pd0 = pd0;
    copy.d0 = d0;
    copy.pd1 = pd1;
    copy.d1 = d1;
    copy.ps0 = ps0;
    copy.s0 = s0;
    copy.ps1 = ps1;
    copy.s1 = s1;
    return copy_result;
}

static long ts_pts, ts_dts;
static char *ts_pd0;
static int ts_len, ts_result;
int func_00245160_mv(struct DotrMvDecoder *vd, long pts, long dts, char *pd0, int len)
{
    said('T');
    assert(vd == &data_00350680_mv);
    ts_pts = pts;
    ts_dts = dts;
    ts_pd0 = pd0;
    ts_len = len;
    return ts_result;
}

static char *err_text;
void func_00242cf0_mv(char *text)
{
    said('E');
    err_text = text;
}

static int end_len;
int func_002450e0_mv(struct DotrMvDecoder *vd, int len)
{
    said('N');
    assert(vd == &data_00350680_mv);
    end_len = len;
    return 0;
}

static char *uncached(char *p)
{
    return (char *)(((DotrMvAddr)p & 0x0fffffff) | 0x20000000);
}

/* Runs the callback with the data at ring offset `at`, `len` bytes, a ring size of 0x1000. */
static int run(int at, unsigned int len, int copied, int ts_ok)
{
    struct DotrMvCbData cb;

    memset(&cb, 0, sizeof cb);
    memset(&copy, 0, sizeof copy);
    ring.size = 0x1000;
    cb.data = ring.data + at;
    cb.len = len;
    cb.pts = 0x123456789L;
    cb.dts = -5;
    copy_result = copied;
    ts_result = ts_ok;
    ts_len = -99;
    err_text = NULL;
    calls = 0;
    order[0] = 0;
    return videoCallback(NULL, &cb, &ring);
}

static void common(int at)
{
    assert(copy.pd0 == uncached(area0) && copy.d0 == 40 && copy.pd1 == uncached(area1) && copy.d1 == 24);
    assert(copy.ps0 == ring.data + at && copy.ps1 == ring.data);
}

int main(void)
{
    /* No wrap: all 50 bytes from the data, none from the ring start. */
    assert(run(100, 50, 50, 1) == 1);
    assert(strcmp(order, "BCTN") == 0);
    common(100);
    assert(copy.s0 == 50 && copy.s1 == 0);
    assert(ts_pts == 0x123456789L && ts_dts == -5 && ts_pd0 == area0 && ts_len == 50 && end_len == 50);

    /* Exactly up to the ring end. */
    assert(run(0x1000 - 50, 50, 50, 1) == 1);
    common(0x1000 - 50);
    assert(copy.s0 == 50 && copy.s1 == 0);

    /* Wrap: 30 bytes before the ring end, 20 from the start; a failed time stamp reports. */
    assert(run(0x1000 - 30, 50, 50, 0) == 1);
    assert(strcmp(order, "BCTEN") == 0);
    common(0x1000 - 30);
    assert(copy.s0 == 30 && copy.s1 == 20);
    assert(err_text == data_0030f5e0_mv && ts_len == 50 && end_len == 50);

    /* Nothing copied: no time stamp, end put with 0, result 0; a negative count likewise. */
    assert(run(8, 16, 0, 1) == 0);
    assert(strcmp(order, "BCN") == 0 && ts_len == -99 && end_len == 0);
    assert(run(8, 16, -1, 1) == 0);
    assert(strcmp(order, "BCN") == 0 && end_len == -1);
    /* One byte copied is enough. */
    assert(run(8, 16, 1, 1) == 1 && ts_len == 1 && end_len == 1);

    puts("mpeg_video: ok");
    return 0;
}
