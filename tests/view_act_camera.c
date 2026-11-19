/* Synthetic host tests for the view_act_camera group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/view_act_camera.h"

int gSzView_pCamera;
unsigned short gSzView_wFrame;

static int pad_held[3]; /* masks reported per mode */
static int pad_calls;
static int req_calls, req_args[2][6];
static int entry_calls, model_calls, model_args[2], anim_calls, anim_args[2];

int SzPad_Check(int unknown, int zero, int mask)
{
    assert(unknown == 0 && (zero == 0 || zero == 2));
    pad_calls++;
    return (pad_held[zero] & mask) != 0;
}

void SzView_ReqCamera(int a, int b, int c, int d, int index, int data)
{
    int *r = req_args[req_calls++ % 2];

    r[0] = a;
    r[1] = b;
    r[2] = c;
    r[3] = d;
    r[4] = index;
    r[5] = data;
}

int SzBattle_InqEntry(int index)
{
    assert(index == 0);
    entry_calls++;
    return 0x0305;
}

int SzDuel_GetUnitModelID(int a, int b)
{
    model_calls++;
    model_args[0] = a;
    model_args[1] = b;
    return 21;
}

int SzModel_SetAnimIntr(int index, int value)
{
    anim_calls++;
    anim_args[0] = index;
    anim_args[1] = value;
    return 0;
}

static void run(int mode0, int mode2, int frame)
{
    pad_held[0] = mode0;
    pad_held[2] = mode2;
    gSzView_wFrame = (unsigned short)frame;
    pad_calls = req_calls = entry_calls = model_calls = anim_calls = 0;
    SzView_ActCamera();
}

/* Branch coverage: the zero-camera exit, each pad test both ways, both clamps
 * on each side of their bound, and both animation numbers. */
int main(void)
{
    gSzView_pCamera = 0;
    run(0xff, 0xff, 5);
    assert(pad_calls == 0 && req_calls == 0 && gSzView_wFrame == 5);

    gSzView_pCamera = 0x1234;
    run(0, 0, 5);
    assert(pad_calls == 4 && req_calls == 0 && entry_calls == 0 && gSzView_wFrame == 5);

    /* mode 2, mask 4: one frame down, not below 0 */
    run(0, 4, 5);
    assert(gSzView_wFrame == 4 && req_calls == 0);
    run(0, 4, 1);
    assert(gSzView_wFrame == 0);
    run(0, 4, 0);
    assert(gSzView_wFrame == 0);

    /* mode 2, mask 8: one frame up, not above 1000 */
    run(0, 8, 998);
    assert(gSzView_wFrame == 999);
    run(0, 8, 999);
    assert(gSzView_wFrame == 1000);
    run(0, 8, 1000);
    assert(gSzView_wFrame == 1000);

    /* mode 0, mask 16: request with index 7 */
    run(16, 0, 40);
    assert(req_calls == 1 && entry_calls == 0);
    assert(req_args[0][0] == 0 && req_args[0][1] == 0 && req_args[0][2] == 8);
    assert(req_args[0][3] == 40 && req_args[0][4] == 7 && req_args[0][5] == 0x1234);

    /* mode 0, mask 160: animation 2, or 5 with bit 128, then a request for that model */
    run(32, 0, 41);
    assert(pad_calls == 5 && entry_calls == 1 && model_calls == 1 && anim_calls == 1);
    assert(model_args[0] == 3 && model_args[1] == 5);
    assert(anim_args[0] == 21 && anim_args[1] == 2 && req_calls == 1);
    assert(req_args[0][0] == 0 && req_args[0][1] == 0 && req_args[0][2] == 8);
    assert(req_args[0][3] == 41 && req_args[0][4] == 21 && req_args[0][5] == 0x1234);
    run(128, 0, 41);
    assert(anim_args[0] == 21 && anim_args[1] == 5 && req_calls == 1);

    /* both requests in one call, after the frame change */
    run(16 | 32, 8, 41);
    assert(req_calls == 2 && req_args[0][4] == 7 && req_args[1][4] == 21);
    assert(req_args[0][3] == 42 && req_args[1][3] == 42);
    return 0;
}
