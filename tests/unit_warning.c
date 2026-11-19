/* Synthetic host tests for the unit_warning group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/unit_warning.h"

#define OWNER 0x0102

static char owner_object, other_object;
static int control, side_seen, list_len, win_calls;
static struct {
    unsigned short word;
    int present, loc, win;
} units[8];

static int find(int a, int b)
{
    int i;

    for (i = 0; i < list_len; i++) {
        if (units[i].word == ((a << 8) | b)) {
            return i;
        }
    }
    assert(0);
    return -1;
}

struct jp_9a533 *SzDuel_GetUnit(int a, int b)
{
    if (((a << 8) | b) == OWNER) {
        return (struct jp_9a533 *)&owner_object;
    }
    return units[find(a, b)].present ? (struct jp_9a533 *)&other_object : NULL;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(units[find(a, b)].present);
    return units[find(a, b)].loc;
}

int Ta_IsUnitControl2(struct jp_9a533 *vInfo)
{
    assert((char *)vInfo == &owner_object);
    return control;
}

void TaLogic_GetUnitCtlLst(int nSide, unsigned short *wUnitLst, int sKind, unsigned char fView)
{
    int i;

    assert(sKind == 29 && fView == 1);
    for (i = 0; i < 22; i++) {
        assert(wUnitLst[i] == 0xffff);
    }
    side_seen = nSide;
    for (i = 0; i < list_len; i++) {
        wUnitLst[i] = units[i].word;
    }
}

int TaLogic_GetUnitCtlLstLen(unsigned short *wUnitLst)
{
    assert(list_len == 0 || wUnitLst[0] == units[0].word);
    return list_len;
}

int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z,
                       unsigned char fView)
{
    int i = find(wUnit >> 8, wUnit & 0xff);

    assert((char *)pVarInfo == &owner_object && bWarnLvl == 77 && fView == 1);
    assert(x == (units[i].loc & 0xff) && z == (units[i].loc >> 8));
    win_calls |= 1 << i;
    return units[i].win;
}

static void unit(int i, int present, int z, int x, int win)
{
    units[i].word = (unsigned short)(0x0201 + i);
    units[i].present = present;
    units[i].loc = (z << 8) | x;
    units[i].win = win;
}

/* Branch coverage: control flag both ways, empty and filled list, missing unit,
 * distance above / equal to / below the range and zero, positive, negative and zero
 * verdicts, both orders of each coordinate pair, the truncating division. */
static void check_warning(void)
{
    control = 0;
    list_len = 8;
    win_calls = 0;
    unit(0, 0, 5, 5, -100); /* no unit: skipped */
    unit(1, 1, 5, 7, -6);   /* distance 2: 6 / 2 = 3 */
    unit(2, 1, 7, 3, -100); /* distance 4 = range: no verdict asked */
    unit(3, 1, 5, 5, -100); /* distance 0 */
    unit(4, 1, 4, 5, 9);    /* distance 1, positive verdict counts 0 */
    unit(5, 1, 9, 9, -100); /* distance 8 > range */
    unit(6, 1, 3, 4, -7);   /* distance 3: 7 / 3 = 2 */
    unit(7, 1, 6, 5, 0);    /* distance 1, verdict 0 */
    assert(TaLogic_GetUnitWarning(5, 5, OWNER, 4, 77, 1) == 5);
    assert(side_seen == 1);
    assert(win_calls == ((1 << 1) | (1 << 4) | (1 << 6) | (1 << 7)));

    control = 1;
    list_len = 0;
    win_calls = 0;
    assert(TaLogic_GetUnitWarning(5, 5, OWNER, 4, 77, 1) == 0);
    assert(side_seen == 0 && win_calls == 0);
}

int main(void)
{
    check_warning();
    return 0;
}
