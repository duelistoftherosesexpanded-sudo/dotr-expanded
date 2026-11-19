/* Synthetic host tests for the cue_steps group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cue_steps.h"

static struct DotrCueStepRecord rec;
static int in_result, out_result, pad_result, in_calls, out_calls, draw_calls, pad_calls, cue_calls, cue_value;

int func_0027fb30_id(int id, unsigned short *p)
{
    assert(id == 5 && p == &rec.unknown_1a4);
    in_calls++;
    return in_result;
}

int func_0027fba0_id(int id, unsigned short *p)
{
    assert(p == &rec.unknown_1a4);
    assert(id == 7 || id == 5);
    out_calls++;
    return out_result;
}

int func_0027fda0_void(void)
{
    return 0xda0;
}

void SzDuel_OpenMsgPopup(int operation, int a, int b, int value)
{
    assert(operation == 1 && a == 0 && b == 0 && value == 0xda0);
    draw_calls++;
}

int SzPad_Check(int unknown, int zero, int mask)
{
    assert(unknown == 0 && zero == 0 && mask == 240);
    pad_calls++;
    return pad_result;
}

int SzSound_PlaySE(int index, int value)
{
    assert(index == 0);
    cue_calls++;
    cue_value = value;
    return 0;
}

static int step(int phase, int first, int last)
{
    memset(&rec, 0, sizeof(rec));
    rec.unknown_160 = phase;
    rec.unknown_1a2 = 0x20;
    rec.unknown_1a4 = 0x30;
    in_calls = out_calls = draw_calls = pad_calls = cue_calls = cue_value = 0;
    return TaTuto_OpenTutoMsg(first, last, &rec);
}

static void check_step(void)
{
    assert(offsetof(struct DotrCueStepRecord, unknown_160) == 0x160);
    assert(offsetof(struct DotrCueStepRecord, unknown_1a2) == 0x1a2);
    assert(offsetof(struct DotrCueStepRecord, unknown_1a4) == 0x1a4);

    /* 0xffff starts at first; the slide-in is not finished: nothing else happens. */
    in_result = 0;
    pad_result = 1;
    assert(step(0xffff, 5, 7) == 0);
    assert(rec.unknown_160 == 5 && in_calls == 1 && pad_calls == 0 && cue_calls == 0);

    /* Finished but the pad query is clear. */
    in_result = 1;
    pad_result = 0;
    assert(step(5, 5, 7) == 0);
    assert(rec.unknown_160 == 5 && pad_calls == 1 && cue_calls == 0);

    /* Finished and set: cue 3 and the next phase; cue 4 and 0xfffe when first == last. */
    pad_result = 1;
    assert(step(5, 5, 7) == 0);
    assert(rec.unknown_160 == 6 && cue_calls == 1 && cue_value == 3);
    assert(step(0xffff, 5, 5) == 0);
    assert(rec.unknown_160 == 0xfffe && cue_calls == 1 && cue_value == 4);

    /* 0xfffe: waits for the slide-out, then resets. */
    out_result = 0;
    assert(step(0xfffe, 5, 7) == 0);
    assert(rec.unknown_160 == 0xfffe && out_calls == 1 && rec.unknown_1a2 == 0x20 && rec.unknown_1a4 == 0x30);
    out_result = 1;
    assert(step(0xfffe, 5, 7) == 1);
    assert(rec.unknown_160 == 0xffff && rec.unknown_1a2 == 0x21 && rec.unknown_1a4 == 0);
    assert(in_calls == 0 && pad_calls == 0 && cue_calls == 0);

    /* Any other phase: the draw call, then the cue step against last. */
    pad_result = 0;
    assert(step(6, 5, 7) == 0);
    assert(draw_calls == 1 && pad_calls == 1 && cue_calls == 0 && rec.unknown_160 == 6);
    pad_result = 1;
    assert(step(6, 5, 7) == 0);
    assert(draw_calls == 1 && cue_value == 3 && rec.unknown_160 == 7);
    assert(step(7, 5, 7) == 0);
    assert(draw_calls == 1 && cue_value == 4 && rec.unknown_160 == 0xfffe && out_calls == 0);
}

int main(void)
{
    check_step();
    return 0;
}
