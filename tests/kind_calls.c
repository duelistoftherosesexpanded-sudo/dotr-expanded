/* Synthetic host tests for the kind_calls group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/kind_calls.h"

unsigned char dotr_kind_bytes_00a9c002[16];

static int last_callee, last_c, last_d, call_count;

static int called(int callee, int a, int index, int c, int d)
{
    assert(a == 100 && index == 9);
    last_callee = callee;
    last_c = c;
    last_d = d;
    call_count++;
    return 200 + callee;
}

int lSzDuel_DrawCustomDuel_Menu_Top_Pipe(int a, int index, int c, int d)
{
    return called(0, a, index, c, d);
}

int lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(int a, int index, int c, int d)
{
    return called(1, a, index, c, d);
}

int lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(int a, int index, int c, int d)
{
    return called(2, a, index, c, d);
}

int lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(int a, int index, int c, int d)
{
    return called(3, a, index, c, d);
}

int lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe(int a, int index, int c, int d)
{
    return called(4, a, index, c, d);
}

int lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe(int a, int index, int c, int d)
{
    return called(5, a, index, c, d);
}

int lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(int a, int index, int c, int d)
{
    return called(6, a, index, c, d);
}

static void check_dispatch(void)
{
    /* Callee number (as in the stub list; 0 is lSzDuel_DrawCustomDuel_Menu_Top_Pipe, -1 none)
     * and the c / d deltas for low nibbles 0-15 under high nibble 1. */
    static const int callee[16] = {1, 2, 3, 4, 5, 0, 0, 6, -1, -1, -1, -1, -1, -1, -1, -1};
    static const int dc[8] = {14, 14, 14, 14, 14, 0, 0, 14};
    static const int dd[8] = {11, 11, 11, 11, 11, 0, 0, -6};
    int low, high;

    for (low = 0; low < 16; low++) {
        dotr_kind_bytes_00a9c002[9] = 0x10 | low;
        call_count = 0;
        if (callee[low] < 0) {
            /* 8 is the table bound: no call, first argument returned. */
            assert(lSzDuel_DrawCustomDuel_Menu_Pipe(100, 9, 30, 40) == 100 && call_count == 0);
        } else {
            assert(lSzDuel_DrawCustomDuel_Menu_Pipe(100, 9, 30, 40) == 200 + callee[low]);
            assert(call_count == 1 && last_callee == callee[low]);
            assert(last_c == 30 + dc[low] && last_d == 40 + dd[low]);
        }
    }
    /* Any other high nibble: lSzDuel_DrawCustomDuel_Menu_Top_Pipe with the arguments unchanged,
     * whatever the low one. */
    for (high = 0; high < 16; high++) {
        if (high == 1) {
            continue;
        }
        dotr_kind_bytes_00a9c002[9] = (high << 4) | 3;
        call_count = 0;
        assert(lSzDuel_DrawCustomDuel_Menu_Pipe(100, 9, 30, 40) == 200);
        assert(call_count == 1 && last_callee == 0 && last_c == 30 && last_d == 40);
    }
}

int main(void)
{
    check_dispatch();
    return 0;
}
