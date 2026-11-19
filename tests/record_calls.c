/* Synthetic host tests for the record_calls group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/record_calls.h"

struct DotrCallRecord data_00317490_callrecs[3];
int (*g_aLogFunc[100])(int, struct DotrCallRecord *);
signed char Rensyu_flg;
signed char Rensyu_END;

static int pad_args[3], pad_result, pad_calls;
static int gate_arg, gate_result, gate_calls;
static int slot_args[2], slot_calls;
static struct DotrBlockSlot *slot_result;
static int send_arg, send_calls, movie_calls;
static int kind_arg, kind_result, kind_calls;
static struct DotrCallRecord *kind_rec;

int SzPad_Check(int index, int mode, int mask) { pad_calls++; pad_args[0] = index; pad_args[1] = mode; pad_args[2] = mask; return pad_result; }
int TaTuto_IsTuto(int value) { gate_calls++; gate_arg = value; return gate_result; }
struct DotrBlockSlot *SzView_InqRequest(int a, int b) { slot_calls++; slot_args[0] = a; slot_args[1] = b; return slot_result; }
void Taku_Camp_Go_Rensyu_Duel(int flag) { send_calls++; send_arg = flag; }
void SzGame_Pop(void) { movie_calls++; }
static int kind_routine(int word, struct DotrCallRecord *rec) { kind_calls++; kind_arg = word; kind_rec = rec; return kind_result; }

static void reset(void)
{
    pad_calls = gate_calls = slot_calls = send_calls = movie_calls = kind_calls = 0;
    gate_result = 0;
    slot_result = 0;
}

/* lSzDuel_GetControl (branch coverage): kinds 100 and 101, kind above 101, the gate choosing the
 * record or the first record, a busy slot, a non-negative result, and the three negative
 * outcomes (flag clear, flag set with -1, flag set with another value). */
static void check_record_call(void)
{
    struct DotrCallRecord *rec = &data_00317490_callrecs[2];

    reset();
    rec->kind = 100;
    pad_result = 0x12345;
    assert(lSzDuel_GetControl(2, 7) == 0x2345 && pad_calls == 1);
    assert(pad_args[0] == 0 && pad_args[1] == 7 && pad_args[2] == 0xffff);
    rec->kind = 101;
    assert(lSzDuel_GetControl(2, 9) == 0x2345 && pad_calls == 2);
    assert(pad_args[0] == 1 && pad_args[1] == 9 && pad_args[2] == 0xffff);
    rec->kind = 102;
    assert(lSzDuel_GetControl(2, 9) == 0 && pad_calls == 2 && gate_calls == 0);

    rec->kind = 99;
    rec->lo = 0x34;
    rec->hi = 0x12;
    g_aLogFunc[99] = kind_routine;
    slot_result = (struct DotrBlockSlot *)rec;
    assert(lSzDuel_GetControl(2, 0) == 0 && gate_calls == 1 && gate_arg == 2);
    assert(slot_calls == 1 && slot_args[0] == -1 && slot_args[1] == -1 && kind_calls == 0);

    slot_result = 0;
    kind_result = 0x54321;
    assert(lSzDuel_GetControl(2, 0) == 0x4321 && kind_calls == 1 && kind_arg == 0x1234 && kind_rec == rec);
    gate_result = 1;
    kind_result = 0;
    assert(lSzDuel_GetControl(2, 0) == 0 && kind_calls == 2 && kind_rec == data_00317490_callrecs);
    assert(send_calls == 0 && movie_calls == 0);

    kind_result = -1;
    Rensyu_flg = 0;
    Rensyu_END = 0;
    assert(lSzDuel_GetControl(2, 0) == 0 && movie_calls == 1 && send_calls == 0 && Rensyu_END == 0);
    Rensyu_flg = 1;
    assert(lSzDuel_GetControl(2, 0) == 0 && movie_calls == 1 && send_calls == 1 && send_arg == 1);
    assert(Rensyu_flg == 1 && Rensyu_END == 0);
    kind_result = -2;
    assert(lSzDuel_GetControl(2, 0) == 0 && movie_calls == 2 && send_calls == 1);
    assert(Rensyu_flg == 0 && Rensyu_END == 1);
}

int main(void)
{
    check_record_call();
    puts("record_calls: ok");
    return 0;
}
