/* Synthetic host tests for the eff_109 group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/eff_109.h"

enum {
    F_BLUR = 1, F_FREECARD, F_DISP, F_DRAW, F_DEL, F_CALCPOS, F_GOAL, F_RATE, F_OPENRATE, F_CLOSE, F_PRESET,
    F_LOAD, F_SETPOS, F_SETROT, F_SCALE, F_BO_MON, F_BO_DUEL, F_VIEWFREE, F_REQCAM, F_ADDFILE, F_TEXTFREE,
    F_SE, F_BGM, F_FADE, F_OPEN, F_INITTEMP, F_DOEFF, F_LOADMOD, F_FOCUS, F_EXZ, F_EXZBO
};

struct call {
    int fn;
    long a[6];
    float f[4];
};

static struct call trace[128];
static int ntrace;
static struct DotrEff109Card card;
static unsigned char model_req[0x100], new_req[0x100];
static int units_present[11], loading_bgm, standby, load_module, tuto, in_use[8], status[8], model_ready;
static int blackout_mon, blackout_duel, next_handle;
static unsigned int textures[8];
static struct DotrEff109Request last_req[8];
static int nreq;
static int camera_word;

static struct call *add(int fn, long a, long b, long c, long d, long e, long f)
{
    struct call *t;

    assert(ntrace < 128);
    t = &trace[ntrace++];
    memset(t, 0, sizeof(*t));
    t->fn = fn;
    t->a[0] = a;
    t->a[1] = b;
    t->a[2] = c;
    t->a[3] = d;
    t->a[4] = e;
    t->a[5] = f;
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

void SzGame_SetAutoBlur(int level) { add(F_BLUR, level, 0, 0, 0, 0, 0); }
int SzDuel_FreeCardRequest(void *request) { add(F_FREECARD, request == model_req, 0, 0, 0, 0, 0); return 0; }
unsigned char *SzDuel_GetCardRequest(void) { return new_req; }
int SzDuel_GetTurnOwner(void) { return 1; }
void SzDuel_SetWindowDisp(int on) { add(F_DISP, on, 0, 0, 0, 0, 0); }
void SzDuel_SetWindowDraw(int on) { add(F_DRAW, on, 0, 0, 0, 0, 0); }
DotrEff109Word SzDuel_GetUnit(int side, int index)
{
    if (index == 0) {
        return (DotrEff109Word)&card;
    }
    (void)side;
    return units_present[index];
}
int SzDuel_DelUnit(int side, int index) { add(F_DEL, side, index, 0, 0, 0, 0); return 0; }
int SzDuel_GetUnitLoc(int side, int index) { return side << 12 | index << 4; }
void SzDuel_CalcUnitPos(int x, int y, int a, void *out) { add(F_CALCPOS, x, y, a, out == new_req, 0, 0); }
int SzDuel_SetUnitCtlGoalC(int side, int index, signed char goal) { add(F_GOAL, side, index, goal, 0, 0, 0); return 0; }
int SzDuel_SetUnitCtlRateC(int side, int index, int a, int b) { add(F_RATE, side, index, a, b, 0, 0); return 0; }
int SzDuel_SetUnitOpenRate(int side, int index, int a, int rate) { add(F_OPENRATE, side, index, a, rate, 0, 0); return 0; }
void SzDuel_CloseMsgPopup(void) { add(F_CLOSE, 0, 0, 0, 0, 0, 0); }
int SzModel_Preset(int model, int a, int b) { add(F_PRESET, model, a, b, 0, 0, 0); return 0; }
int SzModel_Load(int model, int a, int id, int b) { add(F_LOAD, model, a, id, b, 0, 0); return 0; }
unsigned char *SzModel_GetEffTextureB(int model) { assert(model == 0x20); return (unsigned char *)textures; }
DotrEff109Word SzModel_GetCamera(int model, int index)
{
    assert(model == 0x20 && index == 0x13);
    return (DotrEff109Word)&camera_word;
}
int SzModel_IsModelStandby(int model) { assert(model == 0x20); return model_ready; }
static struct call *vec(int fn, int model, const void *v)
{
    struct call *t = add(fn, model, 0, 0, 0, 0, 0);

    memcpy(t->f, v, sizeof(t->f));
    return t;
}
int SzModel_SetPos(int model, void *pos) { vec(F_SETPOS, model, pos); return 0; }
int SzModel_SetRot(int model, void *rot)
{
    struct call *t = add(F_SETROT, model, 0, 0, 0, 0, 0);

    memcpy(&t->a[1], rot, sizeof(int));
    t->a[2] = ((int *)rot)[1];
    t->a[3] = ((int *)rot)[2];
    return 0;
}
int SzModel_SetScale(int model, void *scale) { vec(F_SCALE, model, scale); return 0; }
int SzEffect_IsUse(int handle) { assert(handle >= 0 && handle < 8); return in_use[handle]; }
int SzEffect_GetStatus(int handle) { return status[handle]; }
int SzEffect_IsModelStandby2(int unit) { (void)unit; return standby; }
void SzEffect_SetBlackOutForMonster(int level) { add(F_BO_MON, level, 0, 0, 0, 0, 0); blackout_mon = level; }
void SzEffect_SetBlackOutForDuel(int level) { add(F_BO_DUEL, level, 0, 0, 0, 0, 0); blackout_duel = level; }
unsigned char SzEffect_GetBlackOutForMonster(void) { return (unsigned char)blackout_mon; }
unsigned char SzEffect_GetBlackOutForDuel(void) { return (unsigned char)blackout_duel; }
int SzView_FreeRequest(int a, int b) { add(F_VIEWFREE, a, b, 0, 0, 0, 0); return 0; }
int SzView_ReqCamera(int owner, int target, int type, int count, int value, void *data)
{
    assert(owner == 0 && target == 0 && type == 8 && count == 0 && value == 0x20 && data == &camera_word);
    add(F_REQCAM, 0, 0, 0, 0, 0, 0);
    return 0;
}
int SzText_AddFile(int group, int index, char *name, int value) { add(F_ADDFILE, group, index, name != 0, value, 0, 0); return 0; }
int SzText_FreeRequest(int group, int index) { add(F_TEXTFREE, group, index, 0, 0, 0, 0); return 0; }
int SzSound_PlaySE(int index, int value) { add(F_SE, index, value, 0, 0, 0, 0); return 0; }
int SzSound_PlayBGM(int index, int flag) { add(F_BGM, index, flag, 0, 0, 0, 0); return 0; }
int SzSound_IsLoadingBGM(void) { return loading_bgm; }
void Ta_UnitFade(int side, int index, int a, int b, int c) { add(F_FADE, side, index, a, b, c, 0); }
void TaUtil_DoOpen(int side, int index, int flag) { add(F_OPEN, side, index, flag, 0, 0, 0); }
void InitEffectTemp(void *state) { add(F_INITTEMP, state != 0, 0, 0, 0, 0, 0); }
int Ta_Do_Effect(void *request, void *state)
{
    assert(state != 0 && nreq < 8);
    last_req[nreq++] = *(struct DotrEff109Request *)request;
    add(F_DOEFF, ((struct DotrEff109Request *)request)->command, 0, 0, 0, 0, 0);
    return next_handle++;
}
int TaEffect_LoadModule(void *state, short module) { add(F_LOADMOD, state != 0, module, 0, 0, 0, 0); return 0; }
int TaEffect_IsLoadModule(void *state) { (void)state; return load_module; }
void TaView_ReqFocusCamera(int a, int unit, int b, int c) { add(F_FOCUS, a, unit, b, c, 0, 0); }
void TaView_ReqExzFocusCamera(int a, int unit, int b, int c) { add(F_EXZ, a, unit, b, c, 0, 0); }
void Ta109_BlackOutForExz(int direction) { add(F_EXZBO, direction, 0, 0, 0, 0, 0); }
int TaTuto_IsTuto(int side) { assert(side == 1); return tuto; }

static struct DotrEff109State st;
/* side 0; units 1, 2, 3, 4 in bits 0..11; flag 9 */
static unsigned int src[4] = { 0, 0, 1 | 2 << 3 | 3 << 6 | 4 << 9, 9 };

static void reset(void)
{
    memset(&st, 0, sizeof(st));
    memset(&card, 0, sizeof(card));
    memset(units_present, 0, sizeof(units_present));
    memset(in_use, 0, sizeof(in_use));
    memset(status, 0, sizeof(status));
    card.x = 3;
    card.y = 4;
    loading_bgm = standby = load_module = tuto = model_ready = 0;
    blackout_mon = blackout_duel = 0;
    next_handle = 0;
    nreq = 0;
    ntrace = 0;
    src[1] = 0;
}

static void test_setup(void)
{
    reset();
    units_present[2] = 1;
    units_present[10] = 1;
    assert(Ta_Eff_109(src, &st, 0) == 1);
    assert(trace[0].fn == F_DRAW && trace[0].a[0] == 0);
    assert(nth(F_BGM, 0)->a[0] == 0x2b && nth(F_BGM, 0)->a[1] == 0);
    assert(count(F_LOADMOD) == 4 && nth(F_LOADMOD, 0)->a[1] == 0x3d && nth(F_LOADMOD, 3)->a[1] == 0x26c);
    assert(count(F_LOAD) == 1 && nth(F_LOAD, 0)->a[0] == 0x20 && nth(F_LOAD, 0)->a[2] == 0x2e1);
    assert(nth(F_SCALE, 0)->f[0] == 0.0f && nth(F_SCALE, 0)->f[3] == 0.0f);
    /* The four units opened, then the opponent's present units 1..10. */
    assert(count(F_OPEN) == 6);
    assert(nth(F_OPEN, 0)->a[1] == 1 && nth(F_OPEN, 1)->a[1] == 2 && nth(F_OPEN, 2)->a[1] == 3);
    assert(nth(F_OPEN, 3)->a[1] == 4 && nth(F_OPEN, 3)->a[0] == 0);
    assert(nth(F_OPEN, 4)->a[0] == 1 && nth(F_OPEN, 4)->a[1] == 2 && nth(F_OPEN, 5)->a[1] == 10);
    assert(st.fade == 0x80 && st.stage == 0 && st.phase == 0);

    /* In the tutorial the model is not loaded. */
    reset();
    tuto = 1;
    assert(Ta_Eff_109(src, &st, 5) == 1);
    assert(count(F_LOAD) == 0 && count(F_SCALE) == 0);
}

static void test_stages(void)
{
    reset();
    /* Stage 0 waits for the BGM. */
    loading_bgm = 1;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(st.stage == 0 && count(F_FOCUS) == 0);
    loading_bgm = 0;
    ntrace = 0;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(nth(F_FOCUS, 0)->a[1] == 1 && nth(F_FOCUS, 0)->a[2] == 0x32 && nth(F_FOCUS, 0)->a[3] == 0xb4);
    assert(nth(F_BGM, 0)->a[1] == 1);
    assert(card.model == new_req && *(struct DotrEff109Card **)(new_req + 0x90) == &card);
    assert(nth(F_CALCPOS, 0)->a[0] == 3 && nth(F_CALCPOS, 0)->a[1] == 4 && nth(F_CALCPOS, 0)->a[3] == 1);
    assert(nth(F_SETPOS, 0)->f[2] == 350.0f && nth(F_SETROT, 0)->a[2] == 0x8000);
    assert(nth(F_OPEN, 0)->a[1] == 0 && nth(F_GOAL, 0)->a[2] == 0);
    assert(nth(F_RATE, 0)->a[2] == -1 && nth(F_OPENRATE, 0)->a[3] == 0x80);
    assert(st.stage == 1);

    /* Side 1: the model faces the other way. */
    reset();
    src[1] = 1;
    card.model = model_req;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(card.model == model_req && nth(F_SETPOS, 0)->f[2] == -350.0f && nth(F_SETROT, 0)->a[2] == 0);
    assert(nth(F_FOCUS, 0)->a[1] == (1 << 8 | 1));
    src[1] = 0;

    /* Stages 1..3 focus the next units after 25 frames and a standby model. */
    reset();
    st.stage = 1;
    st.timer = 24;
    standby = 1;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(st.stage == 1 && st.timer == 25);
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(st.stage == 2 && st.timer == 0 && nth(F_FOCUS, 0)->a[1] == 2);
    st.timer = 25;
    standby = 0;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(st.stage == 2 && st.timer == 26);
    standby = 1;
    Ta_Eff_109(src, &st, -1);
    assert(st.stage == 3 && nth(F_FOCUS, 1)->a[1] == 3);
    st.timer = 25;
    Ta_Eff_109(src, &st, -1);
    assert(st.stage == 4 && nth(F_FOCUS, 2)->a[1] == 4);
    st.timer = 25;
    Ta_Eff_109(src, &st, -1);
    assert(st.stage == 5 && nth(F_FOCUS, 3)->a[1] == 0 && nth(F_FOCUS, 3)->a[2] == 0xc8);
    assert(nth(F_BLUR, 0)->a[0] == 6);

    /* Stage 5: a sound and a camera on the first frame, blur from frame 12, then on frame 24
     * with the module loaded: the exz focus and the first effect request (phase 1). */
    ntrace = 0;
    st.timer = 0;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(nth(F_SE, 0)->a[1] == 0x27 && nth(F_FOCUS, 0)->a[0] == 0xc && st.timer == 1);
    st.timer = 12;
    Ta_Eff_109(src, &st, -1);
    assert(nth(F_BLUR, 0)->a[0] == 4 && st.stage == 5);
    st.timer = 24;
    Ta_Eff_109(src, &st, -1);
    assert(st.stage == 5);
    load_module = 1;
    ntrace = 0;
    st.timer = 24;
    Ta_Eff_109(src, &st, -1);
    assert(nth(F_BLUR, 1)->a[0] == 0 && nth(F_EXZ, 0)->a[2] == 0xb4 && st.stage == 6);
    assert(st.phase == 2 && st.count == 1 && last_req[0].command == 0x26c && last_req[0].cell == (4 << 4));
}

static void test_phases(void)
{
    reset();
    st.stage = 6;
    st.phase = 2;
    st.count = 1;
    next_handle = 1;
    st.timer = 25;
    Ta_Eff_109(src, &st, -1);
    assert(st.phase == 3 && last_req[0].cell == (3 << 4) && st.handles[1] == 1 && st.count == 2);
    st.timer = 25;
    Ta_Eff_109(src, &st, -1);
    assert(st.phase == 4 && last_req[1].cell == (2 << 4));
    st.timer = 25;
    Ta_Eff_109(src, &st, -1);
    assert(st.phase == 5 && last_req[2].cell == (1 << 4));
    st.timer = 3;
    Ta_Eff_109(src, &st, -1);
    assert(st.phase == 5 && st.timer == 4);
    st.timer = 25;
    Ta_Eff_109(src, &st, -1);
    assert(st.phase == 6 && last_req[3].cell == 0 && st.count == 5);
    assert(nth(F_FADE, 0)->a[2] == 1 && nth(F_FADE, 0)->a[4] == 2);

    /* Phase 6 waits for every handle. */
    in_use[3] = 1;
    ntrace = 0;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(st.phase == 6);
    assert(st.count == 5);
    assert(ntrace == 1 && trace[0].fn == F_DRAW);
    in_use[3] = 0;
    /* The model not ready yet: the count is cleared and nothing else happens. */
    Ta_Eff_109(src, &st, -1);
    assert(st.count == 0 && st.phase == 6);
    model_ready = 1;
    textures[0] = 0x11;
    textures[7] = 0x77;
    ntrace = 0;
    nreq = 0;
    next_handle = 5;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(nth(F_TEXTFREE, 0)->a[0] == 5 && nth(F_TEXTFREE, 0)->a[1] == -1);
    assert(count(F_ADDFILE) == 8 && nth(F_ADDFILE, 0)->a[3] == 0x11 && nth(F_ADDFILE, 7)->a[3] == 0x77);
    assert(nth(F_ADDFILE, 7)->a[1] == 7 && nth(F_ADDFILE, 0)->a[2] == 0);
    assert(count(F_VIEWFREE) == 1 && count(F_REQCAM) == 1);
    assert(nreq == 3 && last_req[0].command == 0x3d && last_req[2].command == 0x3f);
    assert(last_req[1].value == 0 && last_req[1].cell == (int)src[2] && last_req[1].flag == 9);
    assert(st.count == 3 && st.counters[0] == 0x3d && st.counters[2] == 0x3f && st.handles[2] == 7);
    assert(st.phase == 7);

    /* Phase 7: black-out step each frame; after 101 frames the model is scaled to 1. */
    st.timer = 100;
    ntrace = 0;
    Ta_Eff_109(src, &st, -1);
    assert(nth(F_EXZBO, 0)->a[0] == -1 && st.phase == 7);
    Ta_Eff_109(src, &st, -1);
    assert(st.phase == 8 && st.timer == 0 && nth(F_SCALE, 0)->f[0] == 1.0f && nth(F_SCALE, 0)->f[3] == 1.0f);

    /* Phase 8: a handle in its second status removes the units and frees the card model. */
    card.model = model_req;
    in_use[6] = 1;
    status[6] = 2;
    ntrace = 0;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(nth(F_EXZBO, 0)->a[0] == 1 && count(F_DEL) == 4 && nth(F_DEL, 0)->a[1] == 1 && nth(F_DEL, 3)->a[1] == 4);
    assert(nth(F_PRESET, 0)->a[0] == 0x20 && nth(F_PRESET, 0)->a[2] == 999);
    assert(nth(F_FREECARD, 0)->a[0] == 1 && card.model == 0);
    in_use[6] = 0;
    ntrace = 0;
    assert(Ta_Eff_109(src, &st, -1) == 0);
    assert(nth(F_TEXTFREE, 0)->a[0] == 5 && count(F_CLOSE) == 1);
}

static void test_tutorial_end(void)
{
    reset();
    st.stage = 6;
    st.phase = 6;
    tuto = 1;
    blackout_mon = 4;
    blackout_duel = 0;
    assert(Ta_Eff_109(src, &st, -1) == 1);
    assert(blackout_mon == 0 && count(F_BO_DUEL) == 0);
    ntrace = 0;
    assert(Ta_Eff_109(src, &st, -1) == 0);
    assert(nth(F_DISP, 0)->a[0] == 0 && count(F_CLOSE) == 1);
}

int main(void)
{
    test_setup();
    test_stages();
    test_phases();
    test_tutorial_end();
    return 0;
}
