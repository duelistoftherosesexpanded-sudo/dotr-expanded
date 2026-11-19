/* Synthetic host tests for the object_starts group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/object_starts.h"

struct DotrStartObject *gSzModel_ahModel_start[84];

static struct DotrStartObject object;
static int cell_index, calls, word_calls, send_calls, send_kind;
static int data_fixture;

int SzBattle_InqEntry(int index)
{
    assert(index == 2 || index == 3);
    return index == 2 ? 0x1234 : 0x5678;
}

int SzDuel_GetUnitModelID(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    return cell_index;
}

int SzModel_SetAnimIntr(int index, int value)
{
    assert(index == cell_index && value == 2 && calls == 0);
    calls++;
    return 0;
}

int SzText_FreeRequest(int a, int b)
{
    assert(a == 5 && b == -1 && calls == 1);
    calls++;
    return 0;
}

int SzEffect_ClearRefID2(int packed, int value)
{
    assert(value == 1);
    assert((calls == 2 && packed == 0x1234) || (calls == 3 && packed == 0x5678));
    calls++;
    return 0;
}

int SzText_AddFile(int a, int b, const char *c, int d)
{
    assert(calls == 4 && a == 5 && b == word_calls && c == 0 && d == 100 + word_calls);
    word_calls++;
    return 1;
}

void lSzModel_EffectCB(void)
{
}

int SzEffect_EntryCallback(int a, int b, int c, int d, int e, void (*callback)(void), int g)
{
    assert(calls == 4 && word_calls == 8);
    assert(a == 1 && b == 0xfff6 && c == 0xfff8 && d == 0x5678 && e == 0x1234);
    assert(callback == lSzModel_EffectCB && g == cell_index);
    calls++;
    return 0x4900;
}

void func_001fa8a0_start(int a, int b, int c, int d, int index, void *data)
{
    assert(calls == 5 && a == 0 && b == 1 && d == 0xfff2 && index == cell_index && data == &data_fixture);
    send_kind = c;
    send_calls++;
}

static int run_start(int index, int flag, int alt)
{
    int i;

    memset(&object, 0, sizeof(object));
    for (i = 0; i < 8; i++) {
        object.effectTextureB[i] = 100 + i;
    }
    object.pCameraMotion = &data_fixture;
    object.cameraFrameCurrent = 0xfff2;
    object.cameraAxisStored = 0xfff6;
    object.unknown_208 = 0xfff8;
    cell_index = index;
    calls = word_calls = send_calls = send_kind = 0;
    return SzModel_BattleSeq(2, flag, alt);
}

static void check_start(void)
{
    /* Offsets after the pointer at +0x16c hold on the 32-bit target only (make match covers
     * them); the host checks the part before it. */
    assert(offsetof(struct DotrStartObject, effectTextureB) == 0x144);

    /* Index bounds: -1 and 84 are rejected before the table is read. */
    assert(run_start(-1, 1, 1) == 0 && calls == 0);
    assert(run_start(84, 1, 1) == 0 && calls == 0);
    /* An empty entry (index 0 and 83 are in range). */
    assert(run_start(0, 1, 1) == 0 && calls == 0);
    assert(run_start(83, 1, 1) == 0 && calls == 0);

    /* Flag clear: set up, result stored, nothing sent. */
    gSzModel_ahModel_start[83] = &object;
    assert(run_start(83, 0, 1) == 1);
    assert(calls == 5 && word_calls == 8 && send_calls == 0 && object.effectCallback == 0x4900);

    /* Flag set: kind 8 without alt, 9 with it. */
    gSzModel_ahModel_start[0] = &object;
    assert(run_start(0, 1, 0) == 1);
    assert(send_calls == 1 && send_kind == 8 && object.effectCallback == 0x4900);
    assert(run_start(0, 1, 5) == 1);
    assert(send_calls == 1 && send_kind == 9);

    /* Flag set but no data pointer: nothing sent. */
    memset(&object, 0, sizeof(object));
    cell_index = 0;
    calls = word_calls = send_calls = 0;
    for (calls = 0; calls < 8; calls++) {
        object.effectTextureB[calls] = 100 + calls;
    }
    calls = 0;
    object.cameraAxisStored = 0xfff6;
    object.unknown_208 = 0xfff8;
    assert(SzModel_BattleSeq(2, 1, 1) == 1);
    assert(calls == 5 && send_calls == 0 && object.effectCallback == 0x4900);
    gSzModel_ahModel_start[0] = 0;
    gSzModel_ahModel_start[83] = 0;
}

int main(void)
{
    check_start();
    return 0;
}
