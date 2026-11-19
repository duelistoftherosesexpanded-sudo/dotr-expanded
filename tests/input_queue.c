/* Synthetic host tests for the input_queue group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/input_queue.h"

static int owner_seq[4], owner_n, owner_calls;
static int loc_answer, loc_calls, loc_args[2];
static int status_seq[4], status_calls, status_args[3];
static int phase_answer, phase_calls, phase_arg;
static int move_answer, move_calls, move_args[6];

/* Answers owner_seq[] in order; the last prepared answer repeats. */
int SzDuel_GetTurnOwner(void)
{
    int i = owner_calls < owner_n ? owner_calls : owner_n - 1;
    owner_calls++;
    return owner_seq[i];
}
int SzDuel_GetUnitLoc(int a, int b)
{
    loc_calls++;
    loc_args[0] = a;
    loc_args[1] = b;
    return loc_answer;
}
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(status_calls < 4);
    status_args[0] = a;
    status_args[1] = b;
    status_args[2] = bit;
    return status_seq[status_calls++];
}
int SzDuel_InqHandPhase(int index)
{
    phase_calls++;
    phase_arg = index;
    return phase_answer;
}
int TaLogic_GetQueueXY_Move1(int nSide, int nID, int nX, int nZ, int gX, int gZ)
{
    move_calls++;
    move_args[0] = nSide;
    move_args[1] = nID;
    move_args[2] = nX;
    move_args[3] = nZ;
    move_args[4] = gX;
    move_args[5] = gZ;
    return move_answer;
}

static struct DotrInputQueueCtl ctl;
static struct DotrInputQueueTask task;

static void reset(int owner, int loc, int kind)
{
    int i;
    memset(&ctl, 0, sizeof(ctl));
    memset(&task, 0, sizeof(task));
    for (i = 0; i < 32; i++)
        ctl.ids[i] = 0xeeee;
    for (i = 0; i < 5; i++)
        task.slots[i] = 0xff;
    task.position = 0x0107;
    task.kind = (unsigned char)kind;
    owner_seq[0] = owner;
    owner_n = 1;
    owner_calls = loc_calls = status_calls = phase_calls = move_calls = 0;
    loc_answer = loc;
    phase_answer = 0;
    move_answer = 0;
    memset(status_seq, 0, sizeof(status_seq));
}

/* The queue holds exactly the n words given, then untouched entries. */
static void expect(int n, const unsigned short *words)
{
    int i;
    for (i = 0; i < n; i++)
        assert(ctl.ids[i] == words[i]);
    for (; i < 32; i++)
        assert(ctl.ids[i] == 0xeeee);
}

static void check_rejected(void)
{
    /* Kinds 255 and 14: -1 before the unit is looked up; nothing is queued. */
    reset(0, 0x0302, 255);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0504) == -1);
    assert(owner_calls == 1 && loc_calls == 0 && ctl.mode == 0);
    expect(0, 0);
    reset(0, 0x0302, 14);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0504) == -1);
    assert(loc_calls == 0 && ctl.mode == 0);
    expect(0, 0);
}

static void check_cursor_moves(void)
{
    static const unsigned short right_down_0[] = { 0x8000, 0x8000, 0x1000, 64 };
    static const unsigned short right_down_1[] = { 0x2000, 0x4000, 0x4000, 64 };
    static const unsigned short left_up_0[] = { 0x2000, 0x4000, 64 };
    static const unsigned short left_up_1[] = { 0x8000, 0x8000, 0x1000, 64 };
    static const unsigned short other_side[] = { 0x2000, 0x2000, 0x4000, 0x4000, 64 };
    static const unsigned short none[] = { 64 };

    /* Cursor right of and below the unit, owner 0; an unlisted kind queues only the closing 64. */
    reset(0, 0x0302, 0);
    ctl.mode = 31;
    ctl.unknown_1a0_5 = 5;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0404) == 1);
    assert(owner_calls == 1 && loc_calls == 1 && loc_args[0] == 1 && loc_args[1] == 7);
    expect(4, right_down_0);
    /* The five-bit count wraps and leaves the other three bits. */
    assert(ctl.mode == 0 && ctl.unknown_1a0_5 == 5);
    /* The same directions for owner 1. */
    reset(1, 0x0302, 0);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0503) == 1);
    expect(4, right_down_1);
    assert(ctl.mode == 1);
    /* Cursor left of and above the unit. */
    reset(0, 0x0302, 0);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0201) == 1);
    expect(3, left_up_0);
    reset(1, 0x0302, 0);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0200) == 1);
    expect(4, left_up_1);
    /* An owner that is neither 0 nor 1 takes the "not 0" word forwards and the "not 1" word backwards. */
    reset(2, 0x0302, 0);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0403) == 1);
    assert(ctl.ids[0] == 0x2000 && ctl.ids[1] == 0x4000 && ctl.ids[2] == 64);
    reset(2, 0x0302, 0);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0100) == 1);
    expect(5, other_side);
    /* Cursor on the unit: no moves. Only the low sixteen bits of the cursor are used on the target;
     * the host parameter already has that width. */
    reset(0, 0x0302, 0);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0302) == 1);
    expect(1, none);
}

static void check_kind_6(void)
{
    static const unsigned short x_then_z[] = { 64, 0x8000, 0x8000, 0x4000, 64 };
    static const unsigned short z_then_x[] = { 64, 0x4000, 0x8000, 0x8000, 64 };
    static const unsigned short flag_only[] = { 64, 2, 0x4000, 64 };
    static const unsigned short both_set[] = { 64, 2, 4, 64 };
    static const unsigned short plain[] = { 64, 64 };
    static const unsigned short both_clear[] = { 64, 8, 64 };

    /* Low byte of the position zero: no flag words. Unit at (4,2), target (2,3): two columns back,
     * one row on. */
    reset(0, 0x0204, 6);
    task.position = 0x0100;
    task.target = 0x0302;
    task.shouldFlipUp = 1;
    move_answer = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(move_calls == 1 && move_args[0] == 1 && move_args[1] == 1 && move_args[2] == 4 && move_args[3] == 2);
    assert(move_args[4] == 2 && move_args[5] == 3 && status_calls == 0);
    expect(5, x_then_z);
    assert(ctl.mode == 1);
    /* Any other answer queues the rows first. */
    reset(0, 0x0204, 6);
    task.position = 0x0100;
    task.target = 0x0302;
    move_answer = 2;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    expect(5, z_then_x);
    /* Low byte set and the +0x0c word 1: the word 2; off the target no status is read. */
    reset(0, 0x0204, 6);
    task.target = 0x0304;
    task.shouldFlipUp = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 0 && move_args[0] == 1 && move_args[1] == 1);
    expect(4, flag_only);
    reset(0, 0x0204, 6);
    task.target = 0x0203;
    task.shouldFlipUp = 0;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 0 && ctl.ids[0] == 64 && ctl.ids[1] == 0x8000 && ctl.ids[2] == 64);
    /* On the target with the +0x10 byte 0: status 5 read twice, both set queue the word 4. */
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    task.shouldFlipUp = 1;
    status_seq[0] = 1;
    status_seq[1] = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 2 && status_args[0] == 1 && status_args[1] == 7 && status_args[2] == 5);
    expect(4, both_set);
    /* First clear, or second clear: nothing. */
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 1);
    expect(2, plain);
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    status_seq[0] = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 2);
    expect(2, plain);
    /* The +0x10 byte 1: both reads clear queue the word 8. */
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    task.atkDefPosition = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 2);
    expect(3, both_clear);
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    task.atkDefPosition = 1;
    status_seq[0] = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 1);
    expect(2, plain);
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    task.atkDefPosition = 1;
    status_seq[1] = 1;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 2);
    expect(2, plain);
    /* Any other +0x10 byte: no status read. */
    reset(0, 0x0204, 6);
    task.target = 0x0204;
    task.atkDefPosition = 2;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(status_calls == 0);
    expect(2, plain);
}

static void check_kind_10(void)
{
    static const unsigned short hand[] = { 128, 0x2000, 0x1000, 64, 0x2000, 0x2000, 0x1000, 0x8000, 0x8000, 0x1000, 0x1000, 64, 64 };
    static const unsigned short empty[] = { 128, 64, 64, 64 };

    /* Not the hand phase: nothing queued, the count still goes up. */
    reset(1, 0x0204, 10);
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(phase_calls == 1 && phase_arg == 1 && ctl.mode == 1);
    expect(0, 0);
    /* Hand phase: 128, the moves to the target, 64, then for each used slot the steps from the previous
     * slot value (0 at first; 0x2000 upwards, 0x8000 downwards) and 0x1000, then two 64. */
    reset(0, 0x0204, 10);
    phase_answer = 1;
    task.target = 0x0105;
    task.slots[0] = 2;
    task.slots[2] = 0;
    task.slots[4] = 0;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(phase_arg == 0);
    expect(13, hand);
    /* All slots 255. */
    reset(0, 0x0204, 10);
    phase_answer = 1;
    task.target = 0x0204;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    expect(4, empty);
}

static void check_kind_12_13(void)
{
    static const unsigned short same[] = { 64, 2, 64, 0x4000, 0x4000, 64 };
    static const unsigned short other_0[] = { 64, 2, 64, 0x2000, 0x4000, 64 };
    static const unsigned short other_1[] = { 64, 2, 64, 0x8000, 64 };
    static const unsigned short count[] = { 64, 2, 64, 0x4000, 0x4000, 0x4000, 64 };
    static const unsigned short zero[] = { 64, 2, 64, 64 };

    /* Kind 12: high byte of the target equal to the owner, low four bits count the 0x4000 words. */
    reset(0, 0x0204, 12);
    task.target = 0x00f2;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(owner_calls == 2);
    expect(6, same);
    /* Another side: one word chosen by a third owner query. */
    reset(0, 0x0204, 12);
    task.target = 0x0111;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(owner_calls == 3);
    expect(6, other_0);
    reset(1, 0x0204, 12);
    task.target = 0x0010;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(owner_calls == 3);
    expect(5, other_1);
    /* Kind 13: the whole target word counts. */
    reset(0, 0x0204, 13);
    task.target = 3;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    assert(owner_calls == 1 && ctl.mode == 1);
    expect(7, count);
    reset(0, 0x0204, 13);
    task.target = 0;
    assert(TaLogic_GetInputQueFromMinimumTask(&ctl, &task, 0x0204) == 1);
    expect(4, zero);
}

int main(void)
{
    check_rejected();
    check_cursor_moves();
    check_kind_6();
    check_kind_10();
    check_kind_12_13();
    return 0;
}
