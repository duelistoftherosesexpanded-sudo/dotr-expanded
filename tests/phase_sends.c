/* Synthetic host tests for the phase_sends group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/phase_sends.h"

signed char Rensyu_flg, Rensyu_END;
static int log_at, log_kind[8], log_a[8], log_b[8];

static void note(int kind, int a, int b)
{
    log_kind[log_at] = kind;
    log_a[log_at] = a;
    log_b[log_at] = b;
    log_at++;
}
int SzGame_SetMenuNum(int index, int value) { note(1, index, value); return 0; }
void SzData_SetupFromConstDeck(int index, int value) { note(2, index, value); }
void SzGame_Goto(int arg0, int arg1) { note(3, arg0, arg1); }
void SzGame_Push(int arg0, int arg1) { note(4, arg0, arg1); }

/* Taku_Camp_Go_Rensyu_Duel (branch coverage): each of the five byte values with its three
 * constants and the byte afterwards (0 -> 1, 1 -> 2, 2 -> 0 with the mark
 * set, 3 and 4 unchanged), and both outcomes of the argument. */
static void check_phase(int phase, int flag, int after, int mark)
{
    static const int kinds[7] = {1, 1, 1, 1, 2, 2, 0};
    int value = 32 + phase, first = 17 + 2 * phase, i;

    Rensyu_flg = (signed char)phase;
    Rensyu_END = 0;
    log_at = 0;
    Taku_Camp_Go_Rensyu_Duel(flag);
    assert(log_at == 7);
    for (i = 0; i < 6; i++) {
        assert(log_kind[i] == kinds[i]);
    }
    assert(log_a[0] == 1 && log_b[0] == 0 && log_a[1] == 2 && log_b[1] == 0);
    assert(log_a[2] == 3 && log_b[2] == value && log_a[3] == 4 && log_b[3] == value);
    assert(log_a[4] == 0 && log_b[4] == first && log_a[5] == 1 && log_b[5] == first + 1);
    assert(log_kind[6] == (flag ? 3 : 4) && log_a[6] == 1 && log_b[6] == 0);
    assert(Rensyu_flg == after && Rensyu_END == mark);
}

int main(void)
{
    check_phase(0, 0, 1, 0);
    check_phase(1, 1, 2, 0);
    check_phase(2, 0, 0, 1);
    check_phase(3, 5, 3, 0);
    check_phase(4, 0, 4, 0);
    puts("phase_sends: ok");
    return 0;
}
