/* Synthetic host tests for the stack_forwarders group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/stack_forwarders.h"

static int seen[16];
static int calls;

int TaLogic_GetNearUnit(int r0, int r1, int r2, int r3, int r4, int r5, int r6, int r7, int s0,
                  int s1, int s2, int s3, int s4, int s5, int s6, int s7)
{
    int args[16];
    args[0] = r0, args[1] = r1, args[2] = r2, args[3] = r3;
    args[4] = r4, args[5] = r5, args[6] = r6, args[7] = r7;
    args[8] = s0, args[9] = s1, args[10] = s2, args[11] = s3;
    args[12] = s4, args[13] = s5, args[14] = s6, args[15] = s7;
    memcpy(seen, args, sizeof(seen));
    calls++;
    return 0x5a5a + s6;
}

typedef int (*Forwarder)(int, int, int, int, int, int, int, int, int, unsigned short, int, int,
                         unsigned char, int);

static void check_forwarder(Forwarder f, int flag, int constant)
{
    static const int want[13] = {11, -12, 13, -14, 15, -16, 17, -18, -19, 0xfedc, -21, 22, 0xe7};
    int i;

    memset(seen, 0, sizeof(seen));
    calls = 0;
    assert(f(11, -12, 13, -14, 15, -16, 17, -18, -19, 0xfedc, -21, 22, 0xe7, -24) ==
           0x5a5a + constant);
    assert(calls == 1);
    for (i = 0; i < 13; i++) {
        assert(seen[i] == want[i]);
    }
    assert(seen[13] == flag);
    assert(seen[14] == constant);
    assert(seen[15] == -24);
}

int main(void)
{
    static const struct {
        Forwarder f;
        int flag;
        int constant;
    } copies[] = {
        {TaLogic002_GetNearUnit, 0, 2400},
        {TaLogic001_GetNearUnit, 0, 3000},
        {TaLogic003_GetNearUnit, 0, 1500},
        {TaLogic004_GetNearUnit, 0, 1500},
        {TaLogic005_GetNearUnit, 0, 1500},
        {TaLogic006_GetNearUnit, 0, 2000},
        {TaLogic007_GetNearUnit, 0, 2500},
        {TaLogic008_GetNearUnit, 0, 1800},
        {TaLogic010_GetNearUnit, 0, 2500},
        {TaLogic011_GetNearUnit, 0, 1800},
        {TaLogic012_GetNearUnit, 0, 1800},
        {TaLogic013_GetNearUnit, 0, 2400},
        {TaLogic014_GetNearUnit, 0, 2500},
        {TaLogic015_GetNearUnit, 0, 2000},
        {TaLogic018_GetNearUnit, 0, 2500},
        {TaLogic019_GetNearUnit, 0, 2500},
        {TaLogic022_GetNearUnit, 0, 2500},
        {TaLogic009_GetNearUnit, 1, 2200},
        {TaLogic020_GetNearUnit, 1, 3000},
        {TaLogic021_GetNearUnit, 1, 3000},
    };
    size_t i;

    for (i = 0; i < sizeof(copies) / sizeof(copies[0]); i++) {
        check_forwarder(copies[i].f, copies[i].flag, copies[i].constant);
    }
    return 0;
}
