/* Synthetic host tests for the blend_slots group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/blend_slots.h"

struct DotrBlendSlot *data_00317b60;

static int calls, args[8][6];

int lSzDuel_DrawBuildDeck_GhostCard_Pipe(int a, int id, int c, int d, int e, unsigned char f)
{
    assert(calls < 8);
    args[calls][0] = a;
    args[calls][1] = id;
    args[calls][2] = c;
    args[calls][3] = d;
    args[calls][4] = e;
    args[calls][5] = f;
    return 0x1000 + calls++;
}

static void expect(int call, int a, int id, int c, int d, int e, int f)
{
    assert(args[call][0] == a && args[call][1] == id && args[call][2] == c);
    assert(args[call][3] == d && args[call][4] == e && args[call][5] == f);
}

/* lSzDuel_ReqGhostCard (0x1ba520) branch and loop coverage: first record free, only the last
 * free, none free; every stored field, each flag set and clear, the five-bit mask of n. */
static void check_request(void)
{
    struct DotrBlendSlot slots[9];
    int i;

    memset(slots, 0, sizeof(slots));
    data_00317b60 = slots;
    slots[0].n = 31;
    slots[0].b5 = slots[0].b6 = slots[0].b7 = -1;
    assert(lSzDuel_ReqGhostCard(-2, 0xe5, 0x100, 0x102, 0x1000) == 1);
    assert(slots[0].id == 0xfffe && slots[0].level == 0x80 && slots[0].n == 5);
    assert(slots[0].b5 == 0 && slots[0].b6 == 0 && slots[0].b7 == 0 && slots[1].level == 0);
    for (i = 1; i < 7; i++) {
        slots[i].level = 1;
    }
    assert(lSzDuel_ReqGhostCard(0x1234, 0x3f, 1, 0x101, -1) == 1);
    assert(slots[7].id == 0x1234 && slots[7].level == 0x80 && slots[7].n == 31);
    assert(slots[7].b5 == -1 && slots[7].b6 == -1 && slots[7].b7 == -1);
    assert(slots[6].id == 0 && slots[6].level == 1);
    assert(lSzDuel_ReqGhostCard(0x777, 1, 1, 1, 1) == 0);
    assert(slots[8].id == 0 && slots[8].level == 0 && slots[7].id == 0x1234 && slots[0].id == 0xfffe);
}

/* lSzDuel_DrawBuildDeck_Ghost_Pipe branch coverage: one assertion per branch outcome (dead
 * record, each of the three bits set and clear) and per side of the 16 bound. */
int main(void)
{
    struct DotrBlendSlot slots[8];
    const int base = 0x10000, out = 0x20000;
    const int near = base + 8, far = base + 328;

    assert(sizeof(struct DotrBlendSlot) == 4 && offsetof(struct DotrBlendSlot, level) == 2);
    check_request();
    memset(slots, 0, sizeof(slots));
    data_00317b60 = slots;

    /* all dead: no call, the first argument comes back */
    assert(lSzDuel_DrawBuildDeck_Ghost_Pipe(0x55, base, out) == 0x55 && calls == 0);

    /* slot 1: no bits; level 17 steps to 1 */
    slots[1].id = 0x1234;
    slots[1].level = 17;
    slots[1].n = 0;
    /* slot 2: bit 5 swaps the ends; level 16 steps to 0 */
    slots[2].id = 0xfedc;
    slots[2].level = 16;
    slots[2].n = 31;
    slots[2].b5 = -1;
    /* slot 4: bit 6 is forwarded as -1; level 1 steps to 0 */
    slots[4].id = 7;
    slots[4].level = 1;
    slots[4].n = 3;
    slots[4].b6 = -1;
    /* slot 7: bit 7 gives 128 - level; level 255 steps to 239 */
    slots[7].id = 9;
    slots[7].level = 255;
    slots[7].n = 1;
    slots[7].b7 = -1;
    assert(lSzDuel_DrawBuildDeck_Ghost_Pipe(0x55, base, out) == 0x1003 && calls == 4);
    expect(0, 0x55, 0x1234, near + ((17 * (far - near)) >> 7), out + 112, 0, 17);
    expect(1, 0x1000, 0xfedc, far + ((16 * (near - far)) >> 7), out + 112 + 31 * 54, 0, 16);
    expect(2, 0x1001, 7, near + ((1 * (far - near)) >> 7), out + 112 + 3 * 54, -1, 1);
    expect(3, 0x1002, 9, near + ((255 * (far - near)) >> 7), out + 112 + 54, 0, (unsigned char)(128 - 255));
    assert(slots[1].level == 1 && slots[2].level == 0 && slots[4].level == 0 && slots[7].level == 239);
    assert(slots[0].level == 0 && slots[1].id == 0x1234 && slots[2].n == 31 && slots[2].b5 == -1);
    return 0;
}
