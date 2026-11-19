/* Synthetic host tests for the battle_init group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/battle_init.h"

struct DotrBattleStageParam gSzBattle_aStageParam[11];
int gSzBattle_nSave;
int anOfs_STAGECAM[11];
int gSzBattle_bl[8];
struct DotrBattleVec3 data_002921a0 = { 1.0f, 2.0f, 3.0f };
struct DotrBattleVec3 data_002921b0 = { 4.0f, 5.0f, 6.0f };
char data_002cf730[4], data_002cf746[4];

/* Every callee is a stub that appends (id, a, b) to one log. */
enum { VLIGHT, DRAW, FOGC_M, FOGC_B, FOGZ_M, FOGZ_B, FOGY_M, FOGY_B, LIGHT_M, LIGHT_B, POINT_M, POINT_B,
       INT_M, INT_B, BLACK_M, BLACK_B, FOGFLAG, INTENSITY, FREE, ENV, FORMAT, REQUEST };
static struct { int id; DotrInitWord a, b; } calls[200];
static int n_calls, at;
static struct DotrBattleVec3 seen[2];
static int n_seen;
static char env[4], *format_name;
static int request_args[5];

static int note(int id, DotrInitWord a, DotrInitWord b)
{
    assert(n_calls < 200);
    calls[n_calls].id = id, calls[n_calls].a = a, calls[n_calls].b = b;
    n_calls++;
    return 0;
}
void SzView_SetStraightLight(int unknown, int index, int value) { assert(unknown == 0); note(VLIGHT, index, value); }
int SzModel_SetDrawFlag(int index, int value) { return note(DRAW, index, value); }
void SzEffect_SetFogColorForMonster(DotrInitWord value) { note(FOGC_M, value, 0); }
void SzEffect_SetFogColorForBattle(DotrInitWord value) { note(FOGC_B, value, 0); }
void SzEffect_SetFogParamZForMonster(DotrInitWord value) { note(FOGZ_M, value, 0); }
void SzEffect_SetFogParamZForBattle(DotrInitWord value) { note(FOGZ_B, value, 0); }
void SzEffect_SetFogParamYForMonster(DotrInitWord value) { note(FOGY_M, value, 0); }
void SzEffect_SetFogParamYForBattle(DotrInitWord value) { note(FOGY_B, value, 0); }
void SzEffect_SetStraightLightForMonster(DotrInitWord value) { note(LIGHT_M, value, 0); }
void SzEffect_SetStraightLightForBattle(DotrInitWord value) { note(LIGHT_B, value, 0); }
void SzEffect_SetPointLightForMonster(DotrInitWord value) { note(POINT_M, value, 0); }
void SzEffect_SetPointLightForBattle(DotrInitWord value) { note(POINT_B, value, 0); }
void SzEffect_SetIntensityForMonster(DotrInitWord value) { note(INT_M, value, 0); }
void SzEffect_SetIntensityForBattle(DotrInitWord value) { note(INT_B, value, 0); }
void SzEffect_SetBlackOutForMonster(DotrInitWord value) { note(BLACK_M, value, 0); }
void SzEffect_SetBlackOutForBattle(DotrInitWord value) { note(BLACK_B, value, 0); }
int SzModel_SetFogFlag(int index, int value) { return note(FOGFLAG, index, value); }
int SzModel_SetIntensity(int index, struct DotrBattleVec3 *in)
{
    seen[n_seen++] = *in;
    return note(INTENSITY, index, 0);
}
void SzMem_Free(void *pointer) { note(FREE, (DotrInitWord)pointer, 0); }
char *SzFile_GetEnv(const char *label)
{
    note(ENV, label == data_002cf746, 0);
    return env;
}
int func_0019b648(char *destination, const char *format, ...)
{
    format_name = destination;
    return note(FORMAT, format == data_002cf730, 0);
}
void lSzBattle_CameraCB(struct DotrPoolLoadRecord *record, int completed)
{
    (void)record;
    (void)completed;
}
void *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
                              int unknown_010, void (*callback)(struct DotrPoolLoadRecord *record, int completed),
                              int unknown_01c)
{
    assert(type == 1024 && priority == 1 && name == format_name && unknown_014 == 0 && unknown_01c == 0);
    assert(callback == lSzBattle_CameraCB);
    request_args[0] = byte_size, request_args[1] = byte_offset, request_args[2] = unknown_010;
    note(REQUEST, 0, 0);
    return 0;
}

static void want(int id, DotrInitWord a, DotrInitWord b)
{
    assert(at < n_calls && calls[at].id == id && calls[at].a == a && calls[at].b == b);
    at++;
}

/* SzBattle_Init branch and loop coverage: the first and last index of every run, fog_y equal
 * and different, stages 4, 2 and another, the saved stage equal and different, and a camera
 * file present and absent. */
static void run(int stage, int fog_equal, int saved, int have_file)
{
    struct DotrBattleStageParam *p = &gSzBattle_aStageParam[stage];
    struct DotrBattleBlock *bl = (struct DotrBattleBlock *)gSzBattle_bl;
    static int file;
    int i;

    memset(gSzBattle_aStageParam, 0, sizeof(gSzBattle_aStageParam));
    p->intensity.x = 7.0f, p->intensity.y = 8.0f, p->intensity.z = 9.0f;
    p->fog_y[0] = 1.0f;
    p->fog_y[1] = fog_equal ? 1.0f : 2.0f;
    for (i = 0; i < 11; i++) {
        anOfs_STAGECAM[i] = 100 + i * i;
    }
    gSzBattle_nSave = saved;
    bl->file = have_file ? &file : 0;
    n_calls = n_seen = at = 0;
    SzBattle_Init(stage);

    want(VLIGHT, 9, 1);
    for (i = 0; i < 84; i++) {
        want(DRAW, i, 0);
    }
    for (i = 0; i < 12; i++) {
        want(DRAW, i + 8, 1);
    }
    want(DRAW, 7, 1);
    want(FOGC_M, (DotrInitWord)p->fog_color, 0);
    want(FOGC_B, (DotrInitWord)p->fog_color, 0);
    want(FOGZ_M, 0, 0);
    want(FOGZ_B, 0, 0);
    want(FOGY_M, fog_equal ? 0 : (DotrInitWord)p->fog_y, 0);
    want(FOGY_B, fog_equal ? 0 : (DotrInitWord)p->fog_y, 0);
    want(LIGHT_M, (DotrInitWord)p->light, 0);
    want(LIGHT_B, (DotrInitWord)p->light, 0);
    want(POINT_M, 0, 0);
    want(POINT_B, 0, 0);
    want(INT_M, (DotrInitWord)&p->intensity2, 0);
    want(INT_B, (DotrInitWord)&p->intensity, 0);
    want(BLACK_M, 128, 0);
    want(BLACK_B, 128, 0);
    for (i = 0; i < 12; i++) {
        want(FOGFLAG, i + 8, stage != 4);
    }
    if (stage == 4) {
        want(INTENSITY, 8, 0);
        want(FOGFLAG, 9, 1);
        want(INTENSITY, 12, 0);
        assert(n_seen == 2 && seen[0].x == 7.0f && seen[0].y == 8.0f && seen[0].z == 3.0f);
        assert(seen[1].x == 4.0f && seen[1].y == 5.0f && seen[1].z == 6.0f);
    } else {
        assert(n_seen == 0);
    }
    if (stage == 2) {
        want(FOGFLAG, 9, 0);
    }
    assert(gSzBattle_nSave == stage);
    if (have_file) {
        want(FREE, (DotrInitWord)&file, 0);
    }
    assert(bl->file == 0);
    want(ENV, 1, 0);
    want(FORMAT, 1, 0);
    want(REQUEST, 0, 0);
    assert(at == n_calls);
    assert(request_args[1] == 100 + stage * stage && request_args[0] == 2 * stage + 1 && request_args[2] == request_args[0]);
}

int main(void)
{
    assert(sizeof(struct DotrBattleStageParam) == 0xc0 && offsetof(struct DotrBattleStageParam, fog_y) == 0xb0);
    run(4, 1, 4, 1);
    run(2, 0, 4, 0);
    run(0, 0, 0, 1);
    run(9, 1, 3, 0);
    return 0;
}
