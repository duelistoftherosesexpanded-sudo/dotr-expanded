/* Synthetic host tests for the text_picks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/text_picks.h"

static void pick(int kind, int flags, int number, int code)
{
    int out = -1;

    assert(lSzGame_GetMCardMsg(kind, flags, &out) == code);
    assert(out == number);
}

/* lSzGame_GetMCardMsg (branch coverage: every table case, each inner comparison and both states of bit 7). */
static void check_pick(void)
{
    static const struct { int low, base, step, code; } plain[] = {
        { 0, 2482, 1, 0 }, { 2, 2485, 1, 1 }, { 4, 2485, 1, 1 }, { 3, 2506, 1, 2 }, { 14, 2506, 1, 2 },
        { 6, 2488, 1, 1 }, { 7, 2495, 2, 1 }, { 11, 2510, 1, 0 }, { 12, 2512, 1, 3 }, { 13, 2514, 1, 1 },
        { 15, 2482, 1, 0 }, { 0x7f, 2482, 1, 0 },
    };
    unsigned i;
    int kind, high;

    for (i = 0; i < sizeof(plain) / sizeof(plain[0]); i++) {
        for (kind = 0; kind < 8; kind++) {
            pick(kind, plain[i].low, plain[i].base, plain[i].code);
            pick(kind, plain[i].low | 0x80, plain[i].base + plain[i].step, plain[i].code);
        }
    }
    /* case 1 ignores bit 7 */
    pick(0, 1, 2521, 2);
    pick(0, 0x81, 2521, 2);
    /* case 5: kind 6 against the rest */
    pick(6, 5, 2491, 1);
    pick(6, 0x85, 2491, 1);
    pick(5, 5, 2490, 1);
    pick(7, 0x85, 2493, 1);
    /* cases 8 and 10: kinds 0, 3, 4 against the rest (2 and 5 are the neighbours) */
    for (i = 8; i <= 10; i += 2) {
        pick(0, (int)i, 2503, 1);
        pick(3, (int)i, 2503, 1);
        pick(4, (int)i | 0x80, 2503, 1);
        pick(1, (int)i, 2520, 1);
        pick(2, (int)i, 2520, 1);
        pick(5, (int)i | 0x80, 2520, 1);
        pick(-1, (int)i, 2520, 1);
    }
    /* case 9 */
    pick(4, 9, 2501, 0);
    pick(4, 0x89, 2501, 0);
    pick(5, 9, 2518, 0);
    pick(0, 9, 2499, 0);
    pick(3, 0x89, 2500, 0);
    pick(1, 9, 2516, 0);
    pick(6, 0x89, 2517, 0);
    pick(2, 9, 2516, 0);
    /* high byte: 0x600, 0x100..0x500 by kind, others untouched; the code is kept */
    for (kind = 0; kind < 8; kind++) {
        pick(kind, 0x600 | 12, 2523, 3);
        pick(kind, 0x700 | 12, 2512, 3);
        pick(kind, 0x000 | 0x8c, 2513, 3);
        pick(kind, 0x10600 | 1, 2523, 2);
        for (high = 0x100; high <= 0x500; high += 0x100) {
            pick(kind, high | 13, kind == 4 ? 2522 : kind == 0 || kind == 3 ? 2502 : 2519, 1);
        }
    }
    pick(4, 0x100, 2522, 0);
    pick(0, 0x500, 2502, 0);
    pick(3, 0x300 | 0x80, 2502, 0);
    pick(5, 0x200, 2519, 0);
}

int main(void)
{
    check_pick();
    return 0;
}
