/* Synthetic host tests for the job_steps group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/job_steps.h"

static int log_[64], log_count;
static int side_result, cell_result, reach_result;
static void put(int v) { log_[log_count++] = v; }
void SzDuel_CloseMsgPopup(void) { put(0x453); }
void InitEffectTemp(DotrJobStepQueue *q) { put(0x960); put(q != 0); }
int Ta_GetCurrentSide(int a, int b) { put(0x4de); put(a); put(b); return side_result; }
int SzDuel_GetUnitLoc(int a, int b) { put(0x109); put(a); put(b); return cell_result; }
int Ta_IsLDAbl(int a, int b) { put(0x174); put(a); put(b); return reach_result; }
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int kind)
{
    put(0x490); put(x); put(y); put(c); put(reach); put(e); put(mode); put(kind);
}
void Ta_UnitFade(int side, int pos, int dir, int value, int flags)
{
    put(0xc0); put(side); put(pos); put(dir); put(value); put(flags);
}
int Ta_DelUnit(int a, int b) { put(0x370); put(a); put(b); return 0; }

static int log_is(const int *want, int n)
{
    return log_count == n && memcmp(log_, want, n * sizeof(int)) == 0;
}

static void check_swap_step(void)
{
    DotrJobStepRequest in;
    DotrJobStepQueue q;

    /* Null request or queue: one 0x1e4530 call, 0. */
    memset(&in, 0, sizeof(in));
    memset(&q, 0, sizeof(q));
    log_count = 0;
    assert(Ta_Eff_BTL_002(0, &q, 0) == 0 && log_count == 1 && log_[0] == 0x453);
    log_count = 0;
    assert(Ta_Eff_BTL_002(&in, 0, 0) == 0 && log_count == 1 && log_[0] == 0x453);

    /* mode 0, flags 0: words copied in order, no tint call. */
    in.first = 0x0102;
    in.second = 0x0304;
    in.flags = 0;
    log_count = 0;
    assert(Ta_Eff_BTL_002(&in, &q, 0) == 1 && q.first == 0x0102 && q.second == 0x0304);
    {
        static const int want[] = {0x960, 1, 0xc0, 1, 2, -1, 1, 3};
        assert(log_is(want, 8));
    }

    /* flags 1: words swapped; flags >> 1 still zero. */
    in.flags = 1;
    log_count = 0;
    assert(Ta_Eff_BTL_002(&in, &q, 5) == 1 && q.first == 0x0304 && q.second == 0x0102);
    {
        static const int want[] = {0x960, 1, 0xc0, 3, 4, -1, 1, 3};
        assert(log_is(want, 8));
    }

    /* flags 2: not swapped, tint call built from the second word. */
    in.flags = 2;
    side_result = 7;
    cell_result = 0x0506;
    reach_result = 9;
    log_count = 0;
    assert(Ta_Eff_BTL_002(&in, &q, 0) == 1 && q.first == 0x0102 && q.second == 0x0304);
    {
        static const int want[] = {0x960, 1, 0x4de, 3, 4, 0x109, 7, 0, 0x174, 9, 7,
                                   0x490, 6, 5, 0, 9, 1, 1, 11, 0xc0, 1, 2, -1, 1, 3};
        assert(log_is(want, 25));
    }

    /* mode < 0: count below 16 increments and returns 1; boundary at 15 and 16. */
    q.count = 15;
    log_count = 0;
    assert(Ta_Eff_BTL_002(&in, &q, -1) == 1 && q.count == 16 && log_count == 0);
    assert(Ta_Eff_BTL_002(&in, &q, -1) == 0 && q.count == 17);
    {
        static const int want[] = {0x370, 1, 2, 0x453};
        assert(log_is(want, 4));
    }
}

int main(void)
{
    check_swap_step();
    return 0;
}
