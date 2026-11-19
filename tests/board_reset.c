/* Synthetic host tests for the board_reset group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/board_reset.h"

struct DotrStateControl data_00317490_control[2];
struct DotrStateItem gSzDuel_aaVarInfo[3][11];
static struct DotrStateCell cells[7][7];
static char trace[256];
static int cell_asks, cell_resets, item_resets, next_result;

static void note(char c) { size_t n = strlen(trace); assert(n + 1 < sizeof(trace)); trace[n] = c; }

void RhMem_FillMemory(void *destination, int value, int size)
{
    assert(destination == data_00317490_control && value == 0 && size == 3440);
    memset(destination, value, (size_t)size);
    note('m');
}
void SzDuel_InitCardRequest(void) { note('a'); }
void SzDuel_InitCardControl(void) { note('b'); }
struct DotrStateCell *SzDuel_GetMapInfo(int x, int y)
{
    /* Row by row, x fastest. */
    assert(x == cell_asks % 7 && y == cell_asks / 7 && cell_resets == cell_asks);
    cell_asks++;
    return &cells[y][x];
}
void SzDuel_InitMapInfo(struct DotrStateCell *cell)
{
    assert(cell == &cells[cell_resets / 7][cell_resets % 7]);
    memset(cell, 0xee, sizeof(*cell));
    cell_resets++;
}
void SzDuel_InitCardInfo(struct DotrStateBlock *block, int unknown)
{
    assert(cell_resets == 49 && unknown == -1);
    assert((void *)block == (void *)&gSzDuel_aaVarInfo[item_resets / 11][item_resets % 11]);
    item_resets++;
}
int SzDuel_Init(int unknown_0, int unknown_1, int unknown_2)
{
    unsigned char *bytes = (unsigned char *)data_00317490_control;

    assert(unknown_0 == 0 && unknown_1 == 0 && unknown_2 == 0);
    assert(item_resets == 33 && bytes[828] == 100 && bytes[1676] == 100);
    note('n');
    return next_result;
}

/* Branch coverage: the loop bounds 7 x 7 and 3 x 11 are checked through the call
 * counts and the order asserted in the stubs. */
static void check_reset(void)
{
    unsigned char *bytes = (unsigned char *)data_00317490_control;
    int x, y, i;

    memset(data_00317490_control, 0x55, sizeof(data_00317490_control));
    next_result = 0x1234;
    assert(SzDuel_Initialize() == 0x1234);
    assert(strcmp(trace, "mabn") == 0 && cell_asks == 49 && cell_resets == 49 && item_resets == 33);
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            assert(cells[y][x].field == (x + y) % 10 && cells[y][x].mapMovable == 0xee && cells[y][x].fieldRate == (short)0xeeee);
        }
    }
    /* Exactly 3440 bytes cleared; only the two bytes set afterwards. */
    for (i = 0; i < 3440; i++) {
        assert(bytes[i] == ((i == 828 || i == 1676) ? 100 : 0));
    }
    assert(bytes[3440] == 0x55);
}

int main(void)
{
    check_reset();
    return 0;
}
