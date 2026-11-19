/* Synthetic host tests for the duel_init group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/duel_init.h"

float data_002910b0[2];
char gSzDuel_StageParam[4], data_0029100c[4], data_00291020[4], data_002910b8[4];

/* Every callee is a stub that appends (id, a, b) to one log. */
enum { MODE, VLIGHT, DRAW, DEFORM, RATIO, FOGC_M, FOGC_D, FOGZ_M, FOGZ_D, FOGY_M, FOGY_D, LIGHT_M, LIGHT_D,
       POINT_M, POINT_D, INT_M, INT_D, BLACK_M, BLACK_D };
static struct { int id; DotrInitWord a, b; } calls[200];
static int n_calls, mode_answer;

static int note(int id, DotrInitWord a, DotrInitWord b)
{
    assert(n_calls < 200);
    calls[n_calls].id = id, calls[n_calls].a = a, calls[n_calls].b = b;
    n_calls++;
    return 0;
}
int SzGame_GetMode(int a) { note(MODE, a, 0); return mode_answer; }
void SzView_SetStraightLight(int unknown, int index, int value) { assert(unknown == 0); note(VLIGHT, index, value); }
int SzModel_SetDrawFlag(int index, int value) { return note(DRAW, index, value); }
int SzModel_SetDeform(int index, int value) { return note(DEFORM, index, value); }
int SzModel_SetDeformRatio(int index, int value) { return note(RATIO, index, value); }
void SzEffect_SetFogColorForMonster(DotrInitWord value) { note(FOGC_M, value, 0); }
void SzEffect_SetFogColorForDuel(DotrInitWord value) { note(FOGC_D, value, 0); }
void SzEffect_SetFogParamZForMonster(DotrInitWord value) { note(FOGZ_M, value, 0); }
void SzEffect_SetFogParamZForDuel(DotrInitWord value) { note(FOGZ_D, value, 0); }
void SzEffect_SetFogParamYForMonster(DotrInitWord value) { note(FOGY_M, value, 0); }
void SzEffect_SetFogParamYForDuel(DotrInitWord value) { note(FOGY_D, value, 0); }
void SzEffect_SetStraightLightForMonster(DotrInitWord value) { note(LIGHT_M, value, 0); }
void SzEffect_SetStraightLightForDuel(DotrInitWord value) { note(LIGHT_D, value, 0); }
void SzEffect_SetPointLightForMonster(DotrInitWord value) { note(POINT_M, value, 0); }
void SzEffect_SetPointLightForDuel(DotrInitWord value) { note(POINT_D, value, 0); }
void SzEffect_SetIntensityForMonster(DotrInitWord value) { note(INT_M, value, 0); }
void SzEffect_SetIntensityForDuel(DotrInitWord value) { note(INT_D, value, 0); }
void SzEffect_SetBlackOutForMonster(DotrInitWord value) { note(BLACK_M, value, 0); }
void SzEffect_SetBlackOutForDuel(DotrInitWord value) { note(BLACK_D, value, 0); }

static int at;

static void want(int id, DotrInitWord a, DotrInitWord b)
{
    assert(at < n_calls && calls[at].id == id && calls[at].a == a && calls[at].b == b);
    at++;
}

/* The part every call performs: mode query, light 9, then the seven draw-flag runs with their
 * first and last indices. */
static void want_common(void)
{
    static const int runs[6][2] = { { 0, 5 }, { 5, 2 }, { 20, 10 }, { 30, 2 }, { 32, 2 }, { 74, 10 } };
    int r, i;

    at = 0;
    want(MODE, 1, 0);
    want(VLIGHT, 9, 1);
    for (i = 0; i < 84; i++) {
        want(DRAW, i, 0);
    }
    for (r = 0; r < 6; r++) {
        for (i = 0; i < runs[r][1]; i++) {
            want(DRAW, runs[r][0] + i, 1);
        }
    }
}

static void want_deform(void)
{
    int i;

    for (i = 0; i < 10; i++) {
        want(DEFORM, 20 + i, 1);
        want(RATIO, 20 + i, 32);
    }
    for (i = 0; i < 2; i++) {
        want(DEFORM, 30 + i, 1);
        want(RATIO, 30 + i, 32);
    }
}

static void want_effects(int same)
{
    DotrInitWord y = same ? 0 : (DotrInitWord)data_002910b0;

    want(FOGC_M, (DotrInitWord)data_002910b8, 0);
    want(FOGC_D, (DotrInitWord)data_002910b8, 0);
    want(FOGZ_M, 0, 0);
    want(FOGZ_D, 0, 0);
    want(FOGY_M, y, 0);
    want(FOGY_D, y, 0);
    want(LIGHT_M, (DotrInitWord)data_00291020, 0);
    want(LIGHT_D, (DotrInitWord)data_00291020, 0);
    want(POINT_M, 0, 0);
    want(POINT_D, 0, 0);
    want(INT_M, (DotrInitWord)data_0029100c, 0);
    want(INT_D, (DotrInitWord)gSzDuel_StageParam, 0);
    want(BLACK_M, 0x80, 0);
    want(BLACK_D, 0x80, 0);
}

static void run(int mode, int flag)
{
    n_calls = 0;
    mode_answer = mode;
    lSzDuel_InitCom(flag);
    want_common();
}

/* SzDuel_Init: its callees append (id, a, b, c, d) to a second log; lSzDuel_InitCom is the real
 * function and writes the first log. */
struct DotrWindowControl data_00317490_window;
struct DotrInitVector data_002910c0 = { 1.0f, 2.0f, 3.0f, 4.0f };
struct DotrInitVector data_002910d0 = { 5.0f, 6.0f, 7.0f, 8.0f };
enum { I_DECK = 100, I_GRAVE, I_HAND, I_CUR, I_LEADER, I_REQ, I_CTL, I_DEL, I_COLOR, I_ADD, I_RATE, I_MODE, I_CARD, I_MENU,
       I_ADDDECK, I_SETDECK, I_GETOPT, I_SETOPT, I_CP, I_LOOK, I_FIELD, I_SE, I_MAP, I_LOGIC, I_FREE };
static struct { int id; DotrInitWord a, b, c, d; } ilog[500];
static int n_ilog, iat, menu3, menu4;
static struct DotrInitVector seen_cp, seen_look;

static int inote(int id, DotrInitWord a, DotrInitWord b, DotrInitWord c, DotrInitWord d)
{
    assert(n_ilog < 500);
    ilog[n_ilog].id = id, ilog[n_ilog].a = a, ilog[n_ilog].b = b, ilog[n_ilog].c = c, ilog[n_ilog].d = d;
    n_ilog++;
    return 0;
}
void SzDuel_InitDeck(int index) { inote(I_DECK, index, 0, 0, 0); }
void SzDuel_InitGrave(int index) { inote(I_GRAVE, index, 0, 0, 0); }
void SzDuel_InitHand(int index) { inote(I_HAND, index, 0, 0, 0); }
int SzData_GetCurrentDeck(int side) { inote(I_CUR, side, 0, 0, 0); return 10 + side; }
int SzData_GetLeader(int side, int list) { inote(I_LEADER, side, list, 0, 0); return 500 + side; }
void SzDuel_InitCardRequest(void) { inote(I_REQ, 0, 0, 0, 0); }
void SzDuel_InitCardControl(void) { inote(I_CTL, 0, 0, 0, 0); }
int SzDuel_DelUnit(int a, int b) { return inote(I_DEL, a, b, 0, 0); }
void SzDuel_InitMapColor(int a, int b, int c) { inote(I_COLOR, a, b, c, 0); }
void SzDuel_AddUnit(int a, int b, int c, int d, int e) { assert(e == 1); inote(I_ADD, a, b, c, d); }
int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value) { return inote(I_RATE, a, b, pair, value); }
int SzDuel_SetUnitCtlModeM(int a, int b, int value) { return inote(I_MODE, a, b, value, 0); }
int SzData_GetCardNo(int side, int list, int position) { inote(I_CARD, side, list, position, 0); return 1000 * side + position; }
int SzGame_GetMenuNum(int unknown) { inote(I_MENU, unknown, 0, 0, 0); return unknown == 3 ? menu3 : menu4; }
int SzDuel_AddDeck(int index, int value) { return inote(I_ADDDECK, index, value, 0, 0); }
int SzDuel_SetDeck(int index, int position, int value) { return inote(I_SETDECK, index, position, value, 0); }
int SzData_GetOption(int side, int which) { inote(I_GETOPT, side, which, 0, 0); return 70 + which; }
int SzData_SetOption(int side, int which, int flag) { return inote(I_SETOPT, side, which, flag, 0); }
int SzView_SetCP(int index, struct DotrInitVector *in) { seen_cp = *in; return inote(I_CP, index, 0, 0, 0); }
int SzView_SetLookSP(int index, struct DotrInitVector *in) { seen_look = *in; return inote(I_LOOK, index, 0, 0, 0); }
int SzDuel_SetFieldRate(int x, int y, int value) { return inote(I_FIELD, x, y, value, 0); }
int SzSound_TransferSEGrp(int index, int id, int force) { return inote(I_SE, index, id, force, 0); }
void SzDuel_SetupMap(int index) { inote(I_MAP, index, 0, 0, 0); }
int TaLogic_InitLogicData(int side, struct DotrInitSide *set) { return inote(I_LOGIC, side, (DotrInitWord)set, 0, 0); }
void SzMem_Free(void *pointer) { inote(I_FREE, (DotrInitWord)pointer, 0, 0, 0); }

static void iwant(int id, DotrInitWord a, DotrInitWord b, DotrInitWord c, DotrInitWord d)
{
    assert(iat < n_ilog && ilog[iat].id == id && ilog[iat].a == a && ilog[iat].b == b && ilog[iat].c == c
           && ilog[iat].d == d);
    iat++;
}

/* One call: the control block is filled with 0xaa first, so every stored member and the
 * untouched neighbour bits are distinguishable; `memory` is the word at +0x334 of both sides. */
static void check_init(int map, int side, int fresh, int memory)
{
    struct DotrInitControl *g = (struct DotrInitControl *)&data_00317490_window;
    int i, j, s;

    assert(sizeof(struct DotrInitControl) == 0xd68 && sizeof(struct DotrInitSide) == 848);
    assert(offsetof(struct DotrInitControl, unknown_c5c) == 0xc5c && offsetof(struct DotrInitControl, unknown_d34) == 0xd34);
    assert(offsetof(struct DotrInitControl, unknown_d50) == 0xd50 && offsetof(struct DotrInitControl, unknown_d63) == 0xd63);
    memset(g, 0xaa, sizeof(*g));
    g->sides[0].memory_334 = memory;
    g->sides[1].memory_334 = memory ? memory + 1 : 0;
    n_ilog = 0, n_calls = 0, mode_answer = 0;
    SzDuel_Init(map, side, fresh);
    iat = 0;
    for (s = 0; s < 2; s++) {
        iwant(I_DECK, s, 0, 0, 0);
        iwant(I_GRAVE, s, 0, 0, 0);
        iwant(I_HAND, s, 0, 0, 0);
    }
    if (fresh) {
        iwant(I_CUR, 0, 0, 0, 0);
        iwant(I_CUR, 1, 0, 0, 0);
        iwant(I_LEADER, 0, 10, 0, 0);
        iwant(I_LEADER, 1, 11, 0, 0);
        iwant(I_REQ, 0, 0, 0, 0);
        iwant(I_CTL, 0, 0, 0, 0);
        for (i = 0; i < 11; i++) {
            iwant(I_DEL, 0, i, 0, 0);
            iwant(I_DEL, 1, i, 0, 0);
        }
        iwant(I_COLOR, 0, 0, 1, 0);
        iwant(I_ADD, 0, 3, 6, 500);
        iwant(I_ADD, 1, 3, 0, 501);
        for (s = 0; s < 2; s++) {
            iwant(I_RATE, s, 0, 0, 0);
            iwant(I_MODE, s, 0, 7, 0);
        }
        for (i = 0; i < 40; i++) {
            iwant(I_CARD, 0, 10, i, 0);
            iwant(I_CARD, 1, 11, i, 0);
            iwant(I_MENU, 3, 0, 0, 0);
            if (menu3 >= 1000) {
                iwant(I_ADDDECK, 0, i, 0, 0);
            } else {
                iwant(I_SETDECK, 0, i, i, 0);
            }
            iwant(I_MENU, 4, 0, 0, 0);
            if (menu4 >= 1000) {
                iwant(I_ADDDECK, 1, 1000 + i, 0, 0);
            } else {
                iwant(I_SETDECK, 1, i, 1000 + i, 0);
            }
        }
        for (i = 0; i < 3; i++) {
            iwant(I_GETOPT, side, i, 0, 0);
            iwant(I_SETOPT, side ^ 1, i, 70 + i, 0);
        }
        iwant(I_CP, 0, 0, 0, 0);
        iwant(I_LOOK, 0, 0, 0, 0);
        assert(seen_cp.x == 1.0f && seen_cp.y == 2.0f && seen_cp.z == 3.0f && seen_cp.w == 4.0f);
        assert(seen_look.x == 5.0f && seen_look.z == 7.0f && seen_look.w == 8.0f);
        assert(seen_look.y == 3.1415927f * (side ? 49152.0f : 16384.0f) / 32768.0f);
    }
    for (j = 0; j < 7; j++) {
        for (i = 0; i < 7; i++) {
            iwant(I_FIELD, i, j, fresh ? -128 : 0, 0);
        }
    }
    if (fresh) {
        iwant(I_SE, 1, 0, 0, 0);
    }
    iwant(I_MAP, map, 0, 0, 0);
    for (s = 0; s < 2; s++) {
        struct DotrInitSide *p = &g->sides[s];

        iwant(I_LOGIC, s, (DotrInitWord)p, 0, 0);
        if (memory) {
            iwant(I_FREE, memory + s, 0, 0, 0);
        }
        assert(p->unknown_330 == -1 && p->memory_334 == 0 && p->unknown_338 == 0 && p->unknown_1d0 == 200);
        assert(p->unknown_1d4 == (s ? 0xc000 : 16384) && p->unknown_1d8 == 8192 && p->unknown_341 == 4);
        assert(p->points == (map < 22 ? 4000 : 0xaaaa) && p->unknown_342 == (map < 22 ? 3 : 0xaa));
        assert(p->unknown_343 == 0 && p->unknown_344 == 0 && p->unknown_345 == 40 && p->x == 3 && p->y == (s ? 0 : 6));
        assert(p->unknown_346_bit_0 == -1 && p->unknown_346_high == 0x55 - 0x80 && p->player == 0xaa);
    }
    assert(iat == n_ilog);
    /* the real lSzDuel_InitCom ran once with `fresh` (mode 0: 117 calls, 14 more with the flag) */
    assert(n_calls == (fresh ? 131 : 117));
    assert(g->unknown_d34 == (fresh ? 0 : 128) && g->map == map && g->unit_6c8 == 0 && g->unit_6cc == 0);
    assert(g->unknown_d35 == 0 && g->unknown_d36 == 0 && g->unknown_d50 == 2 && g->unknown_d51 == 0);
    assert(g->unknown_d52 == side && g->unknown_d53 == 0 && g->unknown_d54 == side && g->unknown_d55 == 255);
    assert(g->unknown_d56 == 255 && g->unknown_d57 == 0 && g->unknown_d58 == 24 && g->unknown_d59 == 0);
    assert(g->unknown_d5a == 0 && g->unknown_d5b == 0 && g->side == side && g->unknown_d3d == 0 && g->unknown_d3e == 128);
    for (i = 0; i < 4; i++) {
        assert(g->unknown_d38[i] == 0 && g->unknown_cb2[i] == 24 && g->unknown_caa[i] == 0);
        assert(g->unknown_c9a[i].a == 0 && g->unknown_c9a[i].b == 0);
    }
    for (i = 0; i < 17; i++) {
        assert(g->unknown_d3f[i] == 0);
    }
    for (i = 0; i < 7; i++) {
        assert(g->unknown_c5c[i] == 0);
    }
    assert(g->unknown_c78 == -1 && g->unknown_d63 == 0 && g->unknown_c86 == 0 && g->unknown_c88 == 0);
    assert(g->unknown_c96 == 0 && g->unknown_c98 == 0 && (unsigned char)g->unknown_d5c[0] == 0xaa);
    assert(g->unknown_d64_01 == 1 && g->unknown_d64_23 == 1 && g->unknown_d64_45 == -2 && g->unknown_d64_6 == 0);
    assert(g->unknown_d64_7 == 0 && ((unsigned char *)g)[0xd65] == 0 && ((unsigned char *)g)[0xd66] == 0x80);
    assert(g->unknown_d66_7 == -1 && g->unknown_d67_0 == -1 && g->unknown_d67_1 == -1 && g->unknown_d67_2 == 0);
    assert(g->unknown_d67_3 == 0 && g->unknown_d67_4 == 0 && g->unknown_d67_5 == 0 && g->unknown_d67_67 == -2);
}

/* Branch coverage. lSzDuel_InitCom: mode 1 or not, flag zero or not, the two floats equal or
 * not. SzDuel_Init: fresh or not; side 0 or 1 (angle, option copy); map 21 and 22 (points and
 * +0x342 written or left); menu numbers 3 and 4 at 999 and 1000 independently; +0x334 of the
 * sides zero or not. */
int main(void)
{
    run(0, 0);
    assert(at == n_calls && n_calls == 117);
    run(2, 0);
    assert(at == n_calls);
    run(1, 0);
    want_deform();
    assert(at == n_calls && n_calls == 141);
    data_002910b0[0] = 1.5f, data_002910b0[1] = 1.5f;
    run(0, 5);
    want_effects(1);
    assert(at == n_calls && n_calls == 131);
    data_002910b0[1] = 2.5f;
    run(0, -1);
    want_effects(0);
    assert(at == n_calls);
    run(1, 1);
    want_deform();
    want_effects(0);
    assert(at == n_calls && n_calls == 155);
    data_002910b0[1] = 1.5f;
    menu3 = 999, menu4 = 1000;
    check_init(21, 0, 1, 0);
    menu3 = 1000, menu4 = 999;
    check_init(22, 1, 1, 0x4000);
    check_init(21, 1, 0, 0);
    check_init(22, 0, 0, 0x5000);
    return 0;
}
