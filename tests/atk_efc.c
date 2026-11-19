/* Synthetic host tests for the atk_efc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/atk_efc.h"

static struct jp_570b5 dat;
static struct jp_5730a entry[2];
static int has_entry[2], ret[2], adv, calls;

static int attr_answer = -1, kind_answer;
int SzDuel_GetUnitAttr(int a, int b)
{
    if (attr_answer >= 0) {
        assert(a == 1 && b == 7);
        return attr_answer;
    }
    assert((a == 0 && b == 3) || (a == 1 && b == 4));
    return 10 + a;
}
int SzDuel_GetUnitKind(int a, int b) { assert(a == 1 && b == 7); return kind_answer; }
int ChkAttrAdv(unsigned int kind, int other) { assert(kind == 10 && other == 11); return adv; }
unsigned *func_00250740_word(int a, int b, int n)
{
    assert(b == 3 + a && n == 0);
    return has_entry[a] ? (unsigned *)&entry[a] : NULL;
}
unsigned int Do_AtkEfc(unsigned int dwID, unsigned int dwParam, struct jp_570b5 *pDat, int s1, int id1, int s2, int id2,
                       unsigned int side)
{
    int n = (int)side;

    assert(pDat == &dat && s1 == n && id1 == 3 + n && s2 == 1 - n && id2 == 4 - n);
    assert(dwID == entry[n].dwID && dwParam == entry[n].dwParam);
    calls |= 1 << n;
    return ret[n];
}

static unsigned run(int e0, int e1, int r0, int r1)
{
    has_entry[0] = e0; has_entry[1] = e1; ret[0] = r0; ret[1] = r1;
    calls = 0;
    return Manage_AtkEfc(0, 3, 1, 4, &dat);
}

/* 0x261f20: branch coverage. */
static void check_manage(void)
{
    entry[0].dwID = 7; entry[0].dwParam = 8; entry[1].dwID = 9; entry[1].dwParam = 10;
    memset(&dat, 0, sizeof(dat));
    dat.fGuard1 = 1; dat.fLand1 = 1; dat.fLand2 = 1;
    /* no entries, no advantage: 0, flags untouched */
    adv = 0;
    assert(run(0, 0, 1, 1) == 0 && calls == 0 && dat.AttrAdv == 0 && dat.fLand1 == 1 && dat.fLand2 == 1);
    /* advantage alone goes on; results stay 0 without entries */
    adv = 2;
    assert(run(0, 0, 1, 1) == 0 && calls == 0 && dat.AttrAdv == 2);
    adv = 0;
    /* one entry each way; bit 24 goes to fLand1 / fLand2 */
    assert(run(1, 0, 0x1000001, 1) == 0x1000001 && calls == 1 && dat.fLand1 == 1 && dat.fLand2 == 1);
    assert(run(1, 0, 1, 1) == 0x1000001 && dat.fLand1 == 0 && dat.fLand2 == 1);
    assert(run(0, 1, 1, 1) == 0x10001 && calls == 2 && dat.fLand2 == 0 && dat.fGuard1 == 1);
    assert(run(0, 1, 1, 0x1000001) == 0x10001 && dat.fLand2 == 1);
    /* both low bytes 1 */
    assert(run(1, 1, 0x10001, 0x201) == 0x1010001 && calls == 3);
    /* low bytes other than 1: bit 16 decides */
    assert(run(1, 1, 0x10002, 0x10000) == 0x1010000);
    assert(run(1, 1, 0x20002, 0x10000) == 0x10000);
    assert(run(1, 1, 0x10000, 0x20000) == 0x1000000);
    assert(run(1, 1, 0x20000, 0x20002) == 0);
    /* low byte 1 comes before bit 16 */
    assert(run(1, 1, 0x10000, 0x10001) == 0x10001);
}

static void check_match_obj(void)
{
    int k;

    /* cases 0-19: the low five bits of the unit kind against the case number */
    for (k = 0; k < 20; k++) {
        kind_answer = 0xE0 | k;
        assert(Do_Match_AtkEfcObj(k, 1, 7) == 1);
        kind_answer = 0xE0 | ((k + 1) & 0x1F);
        assert(Do_Match_AtkEfcObj(k, 1, 7) == -1);
    }
    /* cases 20-25: the unit attribute against case number - 20 */
    for (k = 0; k < 6; k++) {
        attr_answer = k;
        assert(Do_Match_AtkEfcObj(20 + k, 1, 7) == 1);
        attr_answer = k + 1;
        assert(Do_Match_AtkEfcObj(20 + k, 1, 7) == -1);
    }
    /* outside the table: no callee, 0 */
    attr_answer = 99, kind_answer = 99;
    assert(Do_Match_AtkEfcObj(26, 0, 0) == 0);
    assert(Do_Match_AtkEfcObj(-1, 0, 0) == 0);
    attr_answer = -1;
}

int main(void)
{
    check_match_obj();
    check_manage();
    return 0;
}
