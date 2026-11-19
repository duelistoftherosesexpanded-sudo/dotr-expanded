/* Synthetic host tests for the logic019_efc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/logic019_efc.h"

#define POS 0x0103
#define HERE 0x0507
static int ctl[8], card, status, status_calls, map[8], buf_calls;
static int l2_n, l2_first[4], l2_flip[4], l3_n, l3_ans[4], warn_calls;
static int sent, sent_arg[3];
static struct DotrPositionOrder *sent_order;
enum { S_NONE, S_ATK, S_LINE, S_BTL, S_GUARD };

int func_00250360_pair(int a, int b) { assert(a >= 0 && a < 8 && b >= 0 && b < 256); return ctl[a]; }
int SzDuel_GetUnitCardNo(int a, int b) { assert(a == 1 && b == 3); return card; }
int func_001df540_word(int a, int b) { assert(a == 1 && b == 3); return 0x77; }
int SzDuel_GetUnitLoc(int a, int b) { assert(a == 1 && b == 3); return HERE; }
void func_0027e770_buf(void *buf) { assert(buf != 0); buf_calls++; }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 1 && b == 3 && bit == 4);
    status_calls++;
    return status;
}
int SzDuel_GetMapUnitID(int x, int y) { assert(x >= 1 && x <= 5 && y == 3); return map[x]; }
int TaLogic_GetWarnHideLvl__00162580(int side) { assert(side == ctl[1]); warn_calls++; return 0x33; }
int func_002770e0_cells(int a, int b, int c, int d, int e, int f, int g, int h,
    struct DotrLogic019Cell *cells, unsigned short *j, unsigned short *k, unsigned short *l, unsigned char *buf)
{
    assert(l2_n < 2 && l3_n == l2_n);
    assert(b == 0x77 && c == 0x07 && d == 0x05 && f == l2_n + 1 && g == 0 && h == 0x33);
    assert(cells[0].id == 0xffff && cells[0].kind == 0xff && cells[21].id == 0xffff && cells[21].kind == 0xff);
    assert(*j == (l2_n ? 0x4240 : 0) && *k == 0 && *l == 0 && k != l && j != k && j != l && buf != 0);
    l2_first[l2_n] = a; l2_flip[l2_n] = e;
    *j = 0x4240 + l2_n;
    return 0x900 + l2_n++;
}
int TaLogic_GetNearUnitL3(int a, int b, int c, struct DotrLogic019Cell *cells, int e, int f, int g, int h, int i, int j,
    unsigned short k)
{
    assert(l3_n < 2 && l2_n == l3_n + 1);
    assert(a == 0x77 && b == 0x07 && c == 0x05 && cells[0].id == 0xffff && e == 2 && f == 52 && g == 0 && h == 2);
    assert(i == 0 && j == 0x33 && k == 0x4240 + l3_n);
    return l3_ans[l3_n++];
}
static void send(int which, struct DotrPositionOrder *order, int a, int b)
{
    assert(sent == S_NONE);
    sent = which; sent_order = order; sent_arg[0] = a; sent_arg[1] = b;
}
void TaLogic019_CommandAtkUnit(struct DotrPositionOrder *order) { send(S_ATK, order, 0, 0); }
void TaLogic019_CommandLineUnit(struct DotrPositionOrder *order, unsigned char deep) { send(S_LINE, order, deep, 0); }
void TaLogic019_CommandBtlUnit(struct DotrPositionOrder *order) { send(S_BTL, order, 0, 0); }
void TaLogic019_CommandLDGuardUnit(int row, int column, struct DotrPositionOrder *order) { send(S_GUARD, order, row, column); }

static void reset(struct DotrPositionOrder *order, int card_no)
{
    memset(order, 0, sizeof(*order));
    order->position = POS; order->kind = 0x1111; order->flags.word = 0xabcd1234;
    card = card_no; status = 0; status_calls = 0; buf_calls = 0; warn_calls = 0;
    memset(map, 0, sizeof(map)); memset(ctl, 0, sizeof(ctl));
    l2_n = l3_n = 0; l3_ans[0] = l3_ans[1] = -1;
    sent = S_NONE;
}
static void untouched(struct DotrPositionOrder *order)
{
    assert(order->kind == 0x1111 && order->flags.word == 0xabcd1234 && order->position == POS);
}

/* 0x15e950 branch coverage: each card case and the default; status bit clear and set for
 * card 43; for card 297 the row scan ending by an empty row, by meeting the unit itself, by
 * a unit of another controller, and by a unit of the same controller, then the two-pass
 * search answering on the first pass, on the second and on neither, with the controller
 * zero and non-zero. */
static void check_efc_monster(void)
{
    struct DotrPositionOrder order;
    int side;

    reset(&order, 5);
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_BTL && sent_order == &order && buf_calls == 1 && status_calls == 0);
    untouched(&order);

    reset(&order, 87);
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_ATK && sent_order == &order);
    untouched(&order);

    reset(&order, 43);
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_NONE && status_calls == 1 && order.kind == 0xcff);
    assert(order.flags.word == ((unsigned)HERE << 16 | 0x1234));
    reset(&order, 43);
    status = 1;
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_BTL && status_calls == 1);
    untouched(&order);

    reset(&order, 297);
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_LINE && sent_order == &order && sent_arg[0] == 1 && l2_n == 0);
    untouched(&order);

    reset(&order, 297);
    map[1] = 0x0204; ctl[2] = 1; map[2] = POS; map[3] = 0x0305;
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_LINE && l2_n == 0);

    reset(&order, 297);
    map[4] = 0x0204; ctl[2] = 1;
    TaLogic019_CommandEfcMonster(&order);
    assert(sent == S_LINE && l2_n == 0);

    for (side = 0; side < 2; side++) {
        reset(&order, 297);
        map[5] = 0x0305; ctl[1] = ctl[3] = side;
        l3_ans[0] = 0x21;
        TaLogic019_CommandEfcMonster(&order);
        assert(sent == S_NONE && l2_n == 1 && l3_n == 1 && l2_first[0] == 0 && l2_flip[0] == !side);
        assert(order.kind == 0x2ff && order.flags.word == (0x21u << 16 | 0x1234) && warn_calls == 2);

        reset(&order, 297);
        map[5] = 0x0305; ctl[1] = ctl[3] = side;
        l3_ans[1] = 0;
        TaLogic019_CommandEfcMonster(&order);
        assert(sent == S_NONE && l2_n == 2 && l3_n == 2 && l2_first[1] == 0x900);
        assert(order.kind == 0x2ff && order.flags.word == 0x1234);

        reset(&order, 297);
        map[5] = 0x0305; ctl[1] = ctl[3] = side;
        TaLogic019_CommandEfcMonster(&order);
        assert(sent == S_GUARD && sent_order == &order && sent_arg[0] == 1 && sent_arg[1] == 3);
        assert(l2_n == 2 && l3_n == 2 && warn_calls == 4);
        untouched(&order);
    }
}

int main(void)
{
    check_efc_monster();
    return 0;
}
