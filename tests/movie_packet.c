/* Synthetic host tests for 0x1a8520 of the movie_start group. Packet fixtures are host data and
 * do not model GS/DMA hardware; stubbed callees have assumed semantics. */
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include "dotr/ee/movie_start.h"

static unsigned char *base;
static int step, input[6];

static uint32_t word(const void *p)
{
    uint32_t value;
    memcpy(&value, p, sizeof(value));
    return value;
}
static uint64_t half(const void *p)
{
    uint64_t value;
    memcpy(&value, p, sizeof(value));
    return value;
}
static void check_tag(const unsigned char *p, uint64_t first)
{
    assert(half(p) == first && half(p + 8) == 14);
}

int func_00186120(int value)
{
    assert(step++ == 0 && value == 2);
    return -19;
}
int func_00184e10(PacketQuadword *p, short a, short b, short c, int d, int e)
{
    assert(step++ == 1);
    base = (unsigned char *)p - 32;
    assert(word(base) == 0x10000010 && word(base + 4) == 0);
    assert(word(base + 8) == 0 && word(base + 12) == 0);
    check_tag(base + 16, UINT64_C(0x1000000000000008));
    assert(a == (short)input[0] && b == (short)input[4] && c == (short)input[5]);
    assert(d == 0 && e == 0);
    memset(p, 0xa5, 32);
    return 2;
}
int func_00184ff8(PacketQuadword *p, int a, int b, int c, int d, int e,
                 int f, int g, int h, int i, int j)
{
    assert(step++ == 2);
    assert((unsigned char *)p == base + 80);
    check_tag(base + 64, UINT64_C(0x1000000000008006));
    assert(a == 2 && b == 0 && c == 0 && d == 0xfff && e == 0xfff);
    assert(f == input[1] && g == input[2] && h == input[3] && i == 0 && j == 0);
    memset(p, 0x3c, 48);
    return 3;
}
void func_0017bb60(int value)
{
    int i;
    assert(step++ == 3 && value == 0);
    for (i = 32; i < 64; i++) assert(base[i] == 0xa5);
    for (i = 80; i < 128; i++) assert(base[i] == 0x3c);
    assert(word(base + 128) == 0x70000000);
    assert(word(base + 132) == 0 && word(base + 136) == 0 && word(base + 140) == 0);
}
int func_00185578(int a, int b)
{
    assert((step == 4 || step == 6) && a == 0 && b == 0);
    step++;
    return 0;
}
void func_00186470(int channel, PacketQuadword *p)
{
    assert(step++ == 5 && channel == -19 && (unsigned char *)p == base);
}

int main(void)
{
    static const int cases[][6] = {
        {10, 0, 0, 0, 1024, 1024},
        {0x18001, INT_MIN, INT_MAX, -17, INT_MAX, INT_MIN},
        {-32768, 13, -15, 19, 32768, 65535}
    };
    unsigned int i;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        memcpy(input, cases[i], sizeof(input));
        step = 0;
        SzGame_ClearGsMem(input[0], input[1], input[2], input[3], input[4], input[5]);
        assert(step == 7);
    }
    return 0;
}
