/* Synthetic host tests for the unsigned_convert group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/unsigned_convert.h"

static long last_f, last_d;
static int calls_f, calls_d, adds;
static double add_a, add_b;

float func_001a1f18_uconv(long v)
{
    calls_f++;
    last_f = v;
    return (float)v;
}
double func_001a1e70_uconv(long v)
{
    calls_d++;
    last_d = v;
    return (double)v;
}
double func_001a2858_uconv(double a, double b)
{
    adds++;
    add_a = a;
    add_b = b;
    return a + b;
}

static void check_float(void)
{
    calls_f = 0;
    assert(_f_ulltof(5) == 5.0f && calls_f == 1 && last_f == 5);
    assert(_f_ulltof(0x7fffffffffffffffUL) == 9223372036854775807.0f && calls_f == 2);

    /* Zero and values with the top bit set take the halving path. */
    calls_f = 0;
    assert(_f_ulltof(0) == 0.0f && calls_f == 1 && last_f == 0);
    assert(_f_ulltof(0x8000000000000000UL) == 9223372036854775808.0f && last_f == 0x4000000000000000L);
    assert(_f_ulltof(0x8000000000000003UL) == 2.0f * (float)0x4000000000000001L && last_f == 0x4000000000000001L);
    assert(_f_ulltof(0xfffffffffffffffeUL) == 18446744073709551616.0f && last_f == 0x7fffffffffffffffL);
    assert(calls_f == 4);
}

static void check_double(void)
{
    calls_d = adds = 0;
    assert(_d_utod(7) == 7.0 && calls_d == 1 && last_d == 7 && adds == 0);
    /* Only the low 32 bits count, zero-extended. */
    assert(_d_utod(0xffffffff00000009UL) == 9.0 && last_d == 9 && adds == 0);
    assert(_d_utod(0xffffffffUL) == 4294967295.0 && last_d == 0xffffffffL && adds == 0);
    assert(_d_utod(0x1234567880000000UL) == 2147483648.0 && last_d == 0x80000000L && adds == 0);

    /* Zero after narrowing takes the halving path through dpadd. */
    assert(_d_utod(0x500000000UL) == 0.0 && last_d == 0 && adds == 1 && add_a == 0.0 && add_b == 0.0);
    assert(_d_utod(0) == 0.0 && adds == 2);
}

int main(void)
{
    check_float();
    check_double();
    puts("unsigned_convert: ok");
    return 0;
}
