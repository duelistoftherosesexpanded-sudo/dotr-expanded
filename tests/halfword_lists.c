/* Synthetic host tests for the halfword_lists group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/halfword_lists.h"

static int fill_a, fill_b, fill_calls;
static unsigned short *fill_out;

/* Records its arguments, checks the list arrives filled and puts three entries in use. */
void TaLogic018_GetZoneSide(int a, int b, unsigned short *out)
{
    int i;

    for (i = 0; i < 49; i++) {
        assert(out[i] == 0xFFFF);
    }
    fill_a = a;
    fill_b = b;
    fill_out = out;
    fill_calls++;
    out[0] = 1;
    out[24] = 0;
    out[48] = 0xFFFE;
}

static void check_count(void)
{
    unsigned short list[50];
    int i;

    /* none in use, all in use, first and last only; entry 49 is not read */
    for (i = 0; i < 50; i++) {
        list[i] = 0xFFFF;
    }
    list[49] = 0;
    assert(TaLogic018_GetZoneSize(list) == 0);
    for (i = 0; i < 49; i++) {
        list[i] = (unsigned short)i;
    }
    assert(TaLogic018_GetZoneSize(list) == 49);
    for (i = 1; i < 48; i++) {
        list[i] = 0xFFFF;
    }
    assert(TaLogic018_GetZoneSize(list) == 2);
}

static void check_fill(void)
{
    unsigned short list[50];

    memset(list, 0, sizeof list);
    list[49] = 0x1234;
    TaLogic018_GetOutVirusZone(5, 6, list);
    assert(fill_calls == 1 && fill_a == 5 && fill_b == 6 && fill_out == list);
    assert(list[0] == 1 && list[1] == 0xFFFF && list[24] == 0 && list[47] == 0xFFFF && list[48] == 0xFFFE);
    assert(list[49] == 0x1234);
}

int main(void)
{
    check_count();
    check_fill();
    return 0;
}
