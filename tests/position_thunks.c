/* Synthetic host tests for the position_thunks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/position_thunks.h"

static int seen_side, seen_slot, calls;

int Ta_GetSommonPiecePreference(int a, int i)
{
    seen_side = a;
    seen_slot = i;
    calls++;
    return 0x100 * (i + 1) + a;
}

/* Forwarder coverage: one call checking both forwarded arguments and the result. */
static void check_thunk(int (*f)(int, int))
{
    calls = 0;
    assert(f(1, 5) == 0x601 && calls == 1 && seen_side == 1 && seen_slot == 5);
}

int main(void)
{
    check_thunk(TaLogic_GetSommonPiecePreference__0010ed90);
    check_thunk(TaLogic_GetSommonPiecePreference__0011a4c0);
    check_thunk(TaLogic_GetSommonPiecePreference__0011f530);
    check_thunk(TaLogic_GetSommonPiecePreference__00124f50);
    check_thunk(TaLogic_GetSommonPiecePreference__0014da30);
    check_thunk(TaLogic_GetSommonPiecePreference__001604a0);
    check_thunk(TaLogic_GetSommonPiecePreference__00170610);
    return 0;
}
