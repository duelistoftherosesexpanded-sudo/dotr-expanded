/* Synthetic host tests for the object_runs group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/object_runs.h"

int gSzEvent_nSave[1];
struct DotrRunPair CastleData[4];
struct DotrRunVector data_00292470 = { 9.0f, 8.0f, 7.0f, 6.0f };

static int gate_result, gate_argument, live_calls, live_seen[48], live_off[80];
static int sent, sent_index[48], sent_cursor[48];
static struct DotrRunVector sent_vector[48];
static int sent_has_vector[48];

int SzModel_GetDrawFlag(int index)
{
    gate_argument = index;
    return gate_result;
}

int SzModel_IsModelDisp(int index)
{
    live_seen[live_calls++] = index;
    return !live_off[index];
}

int SzModel_DrawWithOutCalc_Pipe(int cursor, int index, DotrRunWord c, void *d, int e, int f, int g)
{
    assert(d == 0 && e == 0 && f == 0 && g == 0);
    sent_cursor[sent] = cursor;
    sent_index[sent] = index;
    sent_has_vector[sent] = c != 0;
    if (c)
        sent_vector[sent] = *(struct DotrRunVector *)c;
    sent++;
    return cursor + 0x10;
}

static int run(int mode, int gate)
{
    gSzEvent_nSave[0] = mode;
    gate_result = gate;
    gate_argument = -1;
    live_calls = sent = 0;
    return SzEvent_Draw_Pipe(0x100);
}

/* 0x1e78c0 branch coverage: the gate, both modes, each halfword sign, each
 * SzModel_IsModelDisp outcome, the list end and the 34..73 bounds. */
static void check_runs(void)
{
    int i;

    /* gate closed: nothing else is called */
    assert(run(2, 0) == 0x100 && gate_argument == 34 && live_calls == 0 && sent == 0);
    /* mode 2: {3, 5} both sent; {-1, 6} second only; {7, 8} with 41 rejected; end */
    CastleData[0].first = 3; CastleData[0].second = 5;
    CastleData[0].x = 1.5f; CastleData[0].z = 2.5f;
    CastleData[1].first = -1; CastleData[1].second = 6;
    CastleData[1].x = 3.5f; CastleData[1].z = 4.5f;
    CastleData[2].first = 7; CastleData[2].second = 8;
    CastleData[3].first = 1; CastleData[3].second = -1;
    live_off[41] = 1;
    assert(run(2, 1) == 0x140 && sent == 4 && live_calls == 5);
    assert(live_seen[0] == 37 && live_seen[1] == 39 && live_seen[2] == 40 && live_seen[3] == 41 && live_seen[4] == 42);
    assert(sent_index[0] == 37 && sent_index[1] == 39 && sent_index[2] == 40 && sent_index[3] == 42);
    assert(sent_cursor[0] == 0x100 && sent_cursor[1] == 0x110 && sent_cursor[3] == 0x130);
    assert(sent_has_vector[0] && sent_vector[0].x == 1.5f && sent_vector[0].y == 8.0f);
    assert(sent_vector[0].z == 2.5f && sent_vector[0].w == 6.0f);
    assert(sent_has_vector[1] && sent_vector[1].x == 1.5f && sent_vector[1].z == 2.5f);
    assert(sent_has_vector[2] && sent_vector[2].x == 3.5f && sent_vector[2].z == 4.5f);
    assert(data_00292470.x == 9.0f && data_00292470.z == 7.0f);
    live_off[41] = 0;
    /* an empty list */
    CastleData[0].second = -1;
    assert(run(2, 1) == 0x100 && live_calls == 0 && sent == 0);
    /* any other mode: 34..73, with 34 and 73 rejected once each */
    assert(run(1, 1) == 0x100 + 40 * 0x10 && live_calls == 40 && sent == 40);
    for (i = 0; i < 40; i++)
        assert(live_seen[i] == 34 + i && sent_index[i] == 34 + i && !sent_has_vector[i] && sent_cursor[i] == 0x100 + i * 0x10);
    live_off[34] = live_off[73] = 1;
    assert(run(3, 1) == 0x100 + 38 * 0x10 && live_calls == 40 && sent == 38);
    assert(sent_index[0] == 35 && sent_index[37] == 72);
    live_off[34] = live_off[73] = 0;
}

int main(void)
{
    check_runs();
    return 0;
}
