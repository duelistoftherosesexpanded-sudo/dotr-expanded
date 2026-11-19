/* Synthetic host tests for the ta_values group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/ta_values.h"

unsigned char EnchantId_ta[16];
unsigned short gAnmPat_Bakura_ta[16];
static int pows;
static double last_x, last_y;

double func_001a6be8_ta(double x, double y)
{
    double r = 1.0;
    int i;

    pows++;
    last_x = x;
    last_y = y;
    for (i = 0; i < (int)y; i++) {
        r *= x;
    }
    return r;
}

static void check_bit_length(void)
{
    pows = 0;
    assert(Ta_ConvBitLengthToNumericLength(0) == 0 && pows == 0);
    assert(Ta_ConvBitLengthToNumericLength(-3) == 0 && pows == 0);
    assert(Ta_ConvBitLengthToNumericLength(1) == 1 && pows == 1 && last_x == 2.0 && last_y == 0.0);
    pows = 0;
    assert(Ta_ConvBitLengthToNumericLength(8) == 255 && pows == 8 && last_y == 7.0);
    assert(Ta_ConvBitLengthToNumericLength(16) == 65535);
}

static int effect(int kind, int value)
{
    EnchantId_ta[9] = (unsigned char)kind;
    gAnmPat_Bakura_ta[9] = (unsigned short)value;
    return Ta_GetEncEffect(9);
}

static void check_enc_effect(void)
{
    assert(effect(0, 0) == 537 && effect(0, 1) == 536 && effect(0, 5) == 536);
    assert(effect(1, 5) == 540 && effect(1, 0) == 536 && effect(1, 6) == 536);
    assert(effect(2, 0) == 542 && effect(2, 5) == 542);
    assert(effect(3, 0) == 541 && effect(3, 5) == 541);
    assert(effect(4, 0) == 536 && effect(4, 5) == 536 && effect(255, 0) == 536);
    /* The card number selects the entries. */
    EnchantId_ta[3] = 2;
    assert(Ta_GetEncEffect(3) == 542 && Ta_GetEncEffect(4) == 537);
}

int main(void)
{
    check_bit_length();
    check_enc_effect();
    puts("ta_values: ok");
    return 0;
}
