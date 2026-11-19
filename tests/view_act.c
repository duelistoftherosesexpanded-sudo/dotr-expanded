/* Synthetic host tests for the view_act group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/view_act.h"

DotrViewActWord gSzView_hView_act[1];
struct DotrViewActReset data_00293050_act;
int nCurLight_act;

enum { F_DIR = 1, F_COLOR, F_SETMODE, F_CAMERA, F_ROLL, F_HIDE };

struct call {
    int fn;
    long a[4];
    float f[3];
    DotrViewActQuad quad;
};

static struct call trace[64];
static int ntrace;

static union {
    struct DotrViewActState s;
    unsigned char raw[0x900];
} view;

static int menu, model_type, view_mode, press_on, inq_busy;
static long long game_mode;
static unsigned int press_value[12];
static int pressed[16][3];
static int npressed;

static struct call *add(int fn, long a, long b, long c, long d)
{
    struct call *t;

    assert(ntrace < 64);
    t = &trace[ntrace++];
    memset(t, 0, sizeof(*t));
    t->fn = fn;
    t->a[0] = a;
    t->a[1] = b;
    t->a[2] = c;
    t->a[3] = d;
    return t;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    assert(0);
    return 0;
}

static void press(int port, int kind, int mask)
{
    assert(npressed < 16);
    pressed[npressed][0] = port;
    pressed[npressed][1] = kind;
    pressed[npressed][2] = mask;
    npressed++;
}

int SzDebug_GetActiveMenu(void) { return menu; }
long long SzGame_GetMode(int a) { assert(a == 1); return game_mode; }
int SzDebug_GetActiveModel(void) { return 7; }
int SzModel_GetModelType(int index) { assert(index == 7); return model_type; }

/* Non-zero only for an exact (port, kind, mask) the test pressed. */
int SzPad_Check(int port, int kind, int mask)
{
    int i;

    for (i = 0; i < npressed; i++) {
        if (pressed[i][0] == port && pressed[i][1] == kind && pressed[i][2] == mask) {
            return 1;
        }
    }
    return 0;
}

int SzView_SetStraightLightDirection(int index, int position, int part, struct DotrViewActReset *in)
{
    struct call *t = add(F_DIR, index, position, part, in != 0);

    if (in != 0) {
        t->quad = in->value;
    }
    return 0;
}

int SzView_SetStraightLightColor(int index, int position, int part, void *in)
{
    add(F_COLOR, index, position, part, in != 0);
    return 0;
}

int SzView_GetMode(int index) { assert(index == 0); return view_mode; }
int SzView_SetMode(int index, int value) { add(F_SETMODE, index, value, 0, 0); return 0; }
void *SzView_InqRequest(int a, int b) { assert(a == -1 && b == -1); return inq_busy ? &view : 0; }

int SzView_SetViewCamera(int index, int model, float *target, int mode, float weight)
{
    struct call *t = add(F_CAMERA, index, model, target != 0, mode);

    t->f[0] = weight;
    return 0;
}

int SzPad_IsPress(int port) { assert(port == 1); return press_on; }

unsigned int SzPad_GetPressInfo(int port, int button)
{
    assert(port == 1 && button >= 0 && button < 12);
    return press_value[button];
}

int SzView_RollCamera(int index, int nested, int dir, float yaw, float pitch, float dist)
{
    struct call *t = add(F_ROLL, index, nested, dir, 0);

    t->f[0] = yaw;
    t->f[1] = pitch;
    t->f[2] = dist;
    return 0;
}

int SzDebug_IsHide(void) { add(F_HIDE, 0, 0, 0, 0); return 0; }

static struct DotrViewActLight *light(int type, int part)
{
    return (struct DotrViewActLight *)(view.raw + type * 0x90 + 0x60) + part;
}

static struct DotrViewActRange *range(int base, int type)
{
    return (struct DotrViewActRange *)(view.raw + base + type * 8);
}

static struct DotrViewActColor *color(int type)
{
    return (struct DotrViewActColor *)(view.raw + 0x880 + type * 4);
}

static void reset(int m, long long gm, int type)
{
    memset(&view, 0, sizeof(view));
    memset(press_value, 0, sizeof(press_value));
    gSzView_hView_act[0] = (DotrViewActWord)&view;
    menu = m;
    game_mode = gm;
    model_type = type;
    view_mode = 0;
    press_on = 0;
    inq_busy = 1;
    npressed = 0;
    ntrace = 0;
}

static void test_rejects(void)
{
    reset(0xc, 0, 2);
    assert(SzView_Act(1) == 0);
    assert(SzView_Act(-1) == 0);
    gSzView_hView_act[0] = 0;
    assert(SzView_Act(0) == 0);
    assert(ntrace == 0);
}

static void test_light_direction(void)
{
    reset(0xc, 0, 2);
    view.s.lightFlags = 1 << 2;
    memset(&data_00293050_act, 0x5a, sizeof(data_00293050_act));
    nCurLight_act = 1;
    press(0, 0, 0x4000);
    press(0, 2, 0xa000);
    press(0, 2, 0x8000);
    press(0, 2, 0x80);
    press(0, 2, 0x10);
    press(0, 2, 0xffff);
    light(2, 0)->y = 0x300;
    light(2, 0)->x = 0x50;
    assert(SzView_Act(0) == 1);
    /* Reset direction from the default, with the selected type (2) and light 1. */
    assert(nth(F_DIR, 0)->a[1] == 2 && nth(F_DIR, 0)->a[2] == 1 && nth(F_DIR, 0)->a[3] == 1);
    assert(memcmp(&nth(F_DIR, 0)->quad, &data_00293050_act.value, sizeof(DotrViewActQuad)) == 0);
    /* 0x8000 with 0xa000 steps backwards: (1 + 2) % 3 = 0. */
    assert(nCurLight_act == 0);
    assert(light(2, 0)->y == 0x200 && light(2, 0)->x == 0x150);
    assert(nth(F_DIR, 1)->a[2] == 0 && nth(F_DIR, 1)->a[3] == 0);
    assert(count(F_DIR) == 2);
    /* Debug mode: IsHide twice. */
    assert(count(F_HIDE) == 2);

    /* Without the type bit the light block of type 9 is used; 0xa000 alone steps forwards. */
    reset(0xc, 5, 2);
    nCurLight_act = 2;
    press(0, 2, 0xa000);
    press(0, 2, 0x20);
    assert(SzView_Act(0) == 1);
    assert(nCurLight_act == 0);
    assert(light(9, 0)->y == 0x100);
    assert(light(2, 0)->y == 0);
}

static void test_light_color_rotation(void)
{
    reset(0xd, 12, 1);
    view.s.lightFlags = 1 << 1;
    nCurLight_act = 2;
    light(1, 2)->rotation[0] = 1.0f;
    press(0, 0, 0x4000);
    press(0, 2, 0x81);
    press(0, 2, 0x12);
    press(0, 2, 0xa);
    assert(SzView_Act(0) == 1);
    assert(count(F_COLOR) == 1 && nth(F_COLOR, 0)->a[1] == 1 && nth(F_COLOR, 0)->a[2] == 2 && nth(F_COLOR, 0)->a[3] == 0);
    assert(light(1, 2)->rotation[0] == 1.0f - 0.01f);
    assert(light(1, 2)->rotation[1] == 0.01f);
    assert(light(1, 2)->rotation[2] == 0.01f);
}

static void test_ambient(void)
{
    /* The colour of type 3 is used only when the fog or second mask selects it. */
    reset(0xe, 0, 3);
    view.s.fogFlags = 1 << 3;
    color(3)->r = 0;
    color(3)->g = 0x80;
    color(3)->b = 0xff;
    press(0, 2, 5);
    press(0, 2, 0x41);
    press(0, 2, 0x22);
    assert(SzView_Act(0) == 1);
    assert(color(3)->r == 0 && color(3)->g == 0x7f && color(3)->b == 0xff);

    reset(0xe, 0, 3);
    press(0, 0, 0x4000);
    press(0, 2, 0x81);
    assert(SzView_Act(0) == 1);
    assert(color(9)->r == 0xff && color(9)->g == 0xff && color(9)->b == 0xfe);
    assert(color(3)->r == 0);
}

static void test_ranges(void)
{
    reset(0xf, 0, 4);
    view.s.fogFlags = 1 << 4;
    press(0, 0, 0x4000);
    press(0, 0, 0x8000);
    press(0, 2, 0x80);
    press(0, 2, 0x10);
    assert(SzView_Act(0) == 1);
    assert(range(0x7e0, 4)->near == 1990.0f && range(0x7e0, 4)->far == 4010.0f);
    assert(view.s.fogFlags == 0);

    reset(0x10, 0, 4);
    press(0, 0, 0x4000);
    press(0, 0, 0x2000);
    press(0, 2, 0x20);
    press(0, 2, 0x40);
    assert(SzView_Act(0) == 1);
    /* Type 4 not selected: the second table's type 9 entry, then the bit for type 9 is set. */
    assert(range(0x830, 9)->near == 2010.0f && range(0x830, 9)->far == 3990.0f);
    assert(view.s.secondFlags == 1 << 9);
}

static void test_mode_radius(void)
{
    reset(0x11, 0, 0);
    view_mode = 2;
    view.s.radius = 15.0f;
    press(0, 2, 0xc);
    press(0, 2, 4);
    press(0, 2, 0x80);
    assert(SzView_Act(0) == 1);
    assert(nth(F_SETMODE, 0)->a[1] == (2 + 2) % 3);
    assert(view.s.radius == 10.0f);

    reset(0x11, 0, 0);
    view_mode = 2;
    view.s.radius = 9995.0f;
    press(0, 2, 0xc);
    press(0, 2, 0x20);
    assert(SzView_Act(0) == 1);
    assert(nth(F_SETMODE, 0)->a[1] == (2 + 4) % 3);
    assert(view.s.radius == 10000.0f);

    reset(0x11, 0, 0);
    view.s.radius = 1.0f;
    press(0, 2, 0x4000);
    assert(SzView_Act(0) == 1);
    assert(view.s.radius == 800.0f);
}

static void test_not_debug_mode(void)
{
    reset(0x11, 3, 0);
    view.s.radius = 1.0f;
    press(0, 2, 0x4000);
    assert(SzView_Act(0) == 1);
    assert(view.s.radius == 1.0f);
    assert(count(F_HIDE) == 1);
}

static void test_camera(void)
{
    /* No request pending and model type 4 or 5: the view camera follows the model. */
    reset(0, 3, 5);
    inq_busy = 0;
    view.s.mode = 6;
    assert(SzView_Act(0) == 1);
    assert(count(F_CAMERA) == 1);
    assert(nth(F_CAMERA, 0)->a[0] == 0 && nth(F_CAMERA, 0)->a[1] == 7 && nth(F_CAMERA, 0)->a[2] == 0);
    assert(nth(F_CAMERA, 0)->a[3] == 6 && nth(F_CAMERA, 0)->f[0] == 1.0f);

    reset(0, 3, 6);
    inq_busy = 0;
    assert(SzView_Act(0) == 1);
    assert(count(F_CAMERA) == 0);

    /* Digital rolls. */
    reset(0, 3, 0);
    press(1, 1, 0x8000);
    press(1, 1, 0x4000);
    press(1, 1, 4);
    press(1, 1, 0x80);
    press(1, 1, 2);
    assert(SzView_Act(0) == 1);
    assert(count(F_ROLL) == 2);
    assert(nth(F_ROLL, 0)->a[2] == 1);
    assert(nth(F_ROLL, 0)->f[0] == 0.024543693f && nth(F_ROLL, 0)->f[1] == -0.024543693f);
    assert(nth(F_ROLL, 0)->f[2] == 2.0f);
    assert(nth(F_ROLL, 1)->a[2] == -1);
    assert(nth(F_ROLL, 1)->f[0] == -0.024543693f && nth(F_ROLL, 1)->f[1] == 0.0f);
    assert(nth(F_ROLL, 1)->f[2] == 2.0f);

    /* Pressure rolls: each reading scaled by 1/64. */
    reset(0, 3, 0);
    press_on = 1;
    press_value[1] = 64;
    press_value[3] = 32;
    press_value[8] = 64;
    press_value[5] = 128;
    press_value[10] = 16;
    assert(SzView_Act(0) == 1);
    assert(count(F_ROLL) == 2);
    assert(nth(F_ROLL, 0)->f[0] == 0.024543693f);
    assert(nth(F_ROLL, 0)->f[1] == -(0.024543693f * 32.0f) / 64.0f);
    assert(nth(F_ROLL, 0)->f[2] == 2.0f);
    assert(nth(F_ROLL, 1)->f[0] == (0.024543693f * 128.0f) / 64.0f);
    assert(nth(F_ROLL, 1)->f[2] == -0.5f);
}

int main(void)
{
    test_rejects();
    test_light_direction();
    test_light_color_rotation();
    test_ambient();
    test_ranges();
    test_mode_radius();
    test_not_debug_mode();
    test_camera();
    return 0;
}
