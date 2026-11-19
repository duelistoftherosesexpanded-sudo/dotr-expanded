/* Synthetic host tests for the preset_waits group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/preset_waits.h"

static int log_[96], log_count, busy;

void SzDuel_SetWindowDraw(int value) { log_[log_count++] = 0x7e0; log_[log_count++] = value; }
int SzDuel_GetWindowDisp(void) { log_[log_count++] = 0x780; return busy; }
void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value)
{
    log_[log_count++] = 0xaf0; log_[log_count++] = who; log_[log_count++] = packed;
    log_[log_count++] = add; log_[log_count++] = value;
}
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value)
{
    log_[log_count++] = 0x2c0; log_[log_count++] = operation; log_[log_count++] = a;
    log_[log_count++] = b; log_[log_count++] = value;
}
void SzDuel_CloseMsgPopup(void) { log_[log_count++] = 0x453; }
static void *init_seen, *pair_seen;
void InitEffectTemp(struct DotrPresetInit *obj) { init_seen = obj; log_[log_count++] = 0x960; }
void TaView_RestoreOriginalView(struct DotrPresetPair *p, unsigned char b) { pair_seen = p; log_[log_count++] = 0xa70; log_[log_count++] = b; }

static unsigned short queue[130];
static struct DotrWaitOwner owner;

static int run(int word8, unsigned bound, int mode)
{
    owner.unknown_08 = word8;
    owner.unknown_0c = bound;
    log_count = 0;
    busy = 0;
    init_seen = pair_seen = 0;
    return Ta_Eff_WAIT(&owner, queue, mode);
}

static int level_seen, ready, job_result, job_seen[4], busy_index, step_result, done;
static void *step_args[3];
void SzEffect_SetBlackOutForDuel(int value) { log_[log_count++] = 0xcf0; level_seen = value; }
int SzEffect_IsUse(int index) { log_[log_count++] = 0x1d0; log_[log_count++] = index; return busy_index; }
int Ta_IsFinishFade(void) { log_[log_count++] = 0xd90; return done; }
void Ta_AllUnitFade(unsigned char *mask, unsigned char *o, int c, int dir, int flag, int value)
{
    step_args[0] = mask; step_args[1] = o;
    log_[log_count++] = 0x020; log_[log_count++] = c; log_[log_count++] = dir;
    log_[log_count++] = flag; log_[log_count++] = value;
}
int TaEffect_LoadModule(struct DotrPresetObject *obj, int id) { pair_seen = obj; log_[log_count++] = 0xac0; log_[log_count++] = id; return 0; }
int TaEffect_IsLoadModule(struct DotrPresetObject *obj) { pair_seen = obj; log_[log_count++] = 0xb50; return ready; }
void TaView_ReqStdCompleteView(unsigned char flag) { log_[log_count++] = 0x740; log_[log_count++] = flag; }
void TaView_ReqFlatCompleteView(unsigned char flag) { log_[log_count++] = 0x860; log_[log_count++] = flag; }
int Ta_Do_Effect(int *job, struct DotrWaitQueue *q) { step_args[1] = q; memcpy(job_seen, job, sizeof job_seen); log_[log_count++] = 0xa30; return job_result; }
static int step(int tag, void *o, struct DotrWaitQueue *q, int *job)
{
    step_args[0] = o; step_args[1] = q; memcpy(job_seen, job, sizeof job_seen);
    log_[log_count++] = tag;
    return step_result;
}
int Ta_ManageDestroyEffect(void *o, struct DotrWaitQueue *q, int *job) { return step(0x400, o, q, job); }
int Ta_IsFinishDestEfc(void *o, struct DotrWaitQueue *q, int *job) { return step(0x670, o, q, job); }
void Ta_SetEffectDependOnFlagLst(void *o, struct DotrWaitQueue *q, int *job, int id) { step(0x720, o, q, job); log_[log_count++] = id; }

static int state52;
int SzEffect_GetStatus(int index) { log_[log_count++] = 0x2d0; log_[log_count++] = index; return state52; }
int Ta_DelUnit(int a, int b) { log_[log_count++] = 0x370; log_[log_count++] = a; log_[log_count++] = b; return 0; }
static int packed_result, pos_result;
int Ta_GetMinimumOrderUnitFromFlagList(int word) { log_[log_count++] = 0xfb0; log_[log_count++] = word; return packed_result; }
int SzDuel_GetUnitLoc(int a, int b) { log_[log_count++] = 0x109; log_[log_count++] = a; log_[log_count++] = b; return pos_result; }
int Ta_IsUnitControl(int a, int b) { log_[log_count++] = 0x360; log_[log_count++] = a; log_[log_count++] = b; return 0x99; }
void SzDuel_AddUnit(int a, int b, int c, int d, int e)
{
    log_[log_count++] = 0x580; log_[log_count++] = a; log_[log_count++] = b;
    log_[log_count++] = c; log_[log_count++] = d; log_[log_count++] = e;
}
int SzEffect_SetBusyFlag2(int packed, int flag) { log_[log_count++] = 0x970; log_[log_count++] = packed; log_[log_count++] = flag; return 0; }
static struct DotrWaitQueue wq;
static int (*stage_fn)(void *, struct DotrWaitQueue *, int);
static int go(int mode)
{
    log_count = 0;
    step_args[0] = step_args[1] = 0;
    return stage_fn(&owner, &wq, mode);
}

/* Ta_Eff_018, Ta_Eff_017_A, Ta_Eff_016 (branch coverage, one parametrised check): mode 0 / -1;
 * stage 0 with the level at 63 / 64 and TaEffect_IsLoadModule returning 0 / 1 (0x25ad60: also
 * the +0x102 counter at 16 / 17); stage 1 with SzEffect_IsUse returning 1 / 0; stages 2
 * and 3 with their callee returning 0 / nonzero; the last stage with the level clamped
 * and below 128, the +0x102 counter at 16 / 17, Ta_IsFinishFade at 0 / 1 and
 * SzDuel_GetWindowDisp at 0 / 1. Every call starts with SzDuel_SetWindowDraw(0). `early` is where
 * TaView_ReqStdCompleteView(16) is called: at the start (1) or before the stage-0 job (0). */
static void check_stages(int (*fn)(void *, struct DotrWaitQueue *, int), int id, int early, int counted)
{
    static const int tail[] = {31, 0xac0, 603, 0x020, 3, -1, 1, 1};
    static const int third[] = {0x7e0, 0, 0x670, 0x020, -1, 1, 0, 1, 0xa70, 16};
    int n = early ? 5 : 3;

    stage_fn = fn;
    memset(&wq, 0, sizeof wq);
    busy = 0; ready = 0; done = 0; busy_index = 0; step_result = 0;
    assert(go(3) == 1 && log_count == n + 11 && log_[0] == 0x7e0 && log_[1] == 0 && log_[2] == 0x960);
    assert(!early || (log_[3] == 0x740 && log_[4] == 16));
    assert(log_[n] == 0xac0 && log_[n + 1] == id && log_[n + 2] == 0xac0 && memcmp(log_ + n + 3, tail, sizeof tail) == 0);
    assert(wq.unknown_00 == 128 && step_args[0] == &owner && step_args[1] == &wq && init_seen == &wq);

    /* stage 0 */
    assert(go(-1) == 1 && wq.unknown_00 == 113 && log_count == 3 && log_[2] == 0xcf0 && level_seen == 113);
    wq.unknown_00 = 64;
    assert(go(-1) == 1 && wq.unknown_00 == 49 && wq.unknown_04 == 0);
    wq.unknown_00 = 63; ready = 0;
    assert(go(-1) == 1 && wq.unknown_00 == 48 && log_count == 4 && log_[2] == 0xb50 && level_seen == 48);
    assert(wq.unknown_04 == 0 && wq.unknown_d4 == 0 && wq.unknown_102 == 0);
    ready = 1; job_result = 0x1a5; wq.unknown_d4 = 2;
    if (counted) {
        wq.unknown_102 = 16;
        assert(go(-1) == 1 && log_count == 4 && wq.unknown_102 == 17 && wq.unknown_04 == 0 && wq.unknown_d4 == 2);
    }
    n = early ? 3 : 5;
    assert(go(-1) == 1 && log_count == n + 2 && log_[2] == 0xb50 && log_[n] == 0xa30 && log_[n + 1] == 0xcf0);
    assert(early || (log_[3] == 0x740 && log_[4] == 16));
    assert(job_seen[0] == id && job_seen[1] == 0 && job_seen[2] == 0 && job_seen[3] == 0 && step_args[1] == &wq);
    assert(wq.unknown_06[2] == 0xa5 && wq.unknown_d4 == 3 && wq.unknown_04 == 1 && wq.unknown_00 == 48);
    assert(wq.unknown_102 == 0);

    /* stage 1 */
    busy_index = 1; wq.unknown_06[0] = 0x44;
    assert(go(-1) == 1 && log_count == 4 && log_[2] == 0x1d0 && log_[3] == 0x44 && wq.unknown_04 == 1);
    busy_index = 0;
    assert(go(-1) == 1 && log_count == 6 && log_[4] == 0x720 && log_[5] == 31 && wq.unknown_04 == 2);
    assert(job_seen[0] == 999 && job_seen[1] == 0 && job_seen[2] == 0 && job_seen[3] == 0 && step_args[0] == &owner);

    /* stage 2 */
    step_result = 0;
    assert(go(-1) == 1 && log_count == 3 && log_[2] == 0x400 && wq.unknown_04 == 2 && job_seen[0] == 999);
    step_result = 5;
    assert(go(-1) == 1 && wq.unknown_04 == 3);

    /* stage 3 */
    step_result = 0; wq.unknown_102 = 9;
    assert(go(-1) == 1 && log_count == 3 && log_[2] == 0x670 && wq.unknown_04 == 3 && wq.unknown_102 == 9);
    step_result = 1;
    assert(go(-1) == 1 && log_count == 10 && memcmp(log_, third, sizeof third) == 0);
    assert(wq.unknown_102 == 0 && wq.unknown_04 == 4 && pair_seen == &wq);

    /* last stage */
    wq.unknown_00 = 100;
    assert(go(-1) == 1 && wq.unknown_00 == 115 && log_count == 3 && level_seen == 115 && wq.unknown_102 == 0);
    wq.unknown_00 = 114; wq.unknown_102 = 16; done = 1;
    assert(go(-1) == 1 && wq.unknown_00 == 128 && level_seen == 128 && wq.unknown_102 == 17 && log_count == 3);
    done = 0;
    assert(go(-1) == 1 && wq.unknown_00 == 128 && wq.unknown_102 == 18 && log_count == 4 && log_[3] == 0xd90);
    done = 1; busy = 0;
    assert(go(-1) == 0 && wq.unknown_102 == 19 && log_count == 6 && log_[4] == 0x780 && log_[5] == 0x453);
    busy = 1;
    assert(go(-1) == 0 && log_count == 8 && log_[5] == 0x7e0 && log_[6] == 1 && log_[7] == 0x453);
    busy = 0;
}

/* Ta_Eff_021 (branch coverage): mode 0 / -1; stage 0 with the level at 63 / 64 and
 * TaEffect_IsLoadModule returning 0 / 1; stages 1 and 2 with their callee returning 0 / nonzero;
 * the last stage with the level clamped and below 128, the +0x102 counter at 16 / 17,
 * Ta_IsFinishFade at 0 / 1 and SzDuel_GetWindowDisp at 0 / 1. */
/* Ta_Eff_021, Ta_Eff_023 (branch coverage): `flag_tag` is the stub tag of the flag call,
 * `id` the first id sent; `waits` nonzero when stage 0 also counts +0x102 up to 16. */
static void check_short_stages(int (*fn)(void *, struct DotrWaitQueue *, int), int flag_tag, int id, int waits)
{
    int start[] = {0x7e0, 0, 0x960, 0x740, 16, 0x020, 3, -1, 1, 1, 0xac0, 31, 0xac0, 603};
    static const int second[] = {0x7e0, 0, 0x670, 0x020, -1, 1, 0, 1, 0xa70, 16};

    stage_fn = fn;
    start[3] = flag_tag; start[11] = id;
    memset(&wq, 0, sizeof wq);
    busy = 0; ready = 0; done = 0; step_result = 0;
    assert(go(3) == 1 && log_count == 14 && memcmp(log_, start, sizeof start) == 0);
    assert(wq.unknown_00 == 128 && step_args[0] == &owner && step_args[1] == &wq && init_seen == &wq);

    /* stage 0 */
    wq.unknown_00 = 64;
    assert(go(-1) == 1 && wq.unknown_00 == 49 && log_count == 3 && log_[2] == 0xcf0 && level_seen == 49);
    if (waits) {
        /* counter 15 after the increment: TaEffect_IsLoadModule is not asked; 16: it is */
        assert(wq.unknown_102 == 1);
        wq.unknown_00 = 63; wq.unknown_102 = 14;
        assert(go(-1) == 1 && wq.unknown_00 == 48 && wq.unknown_102 == 15 && log_count == 3 && log_[2] == 0xcf0);
    } else {
        assert(wq.unknown_102 == 0);
    }
    wq.unknown_00 = 63;
    assert(go(-1) == 1 && wq.unknown_00 == 48 && log_count == 4 && log_[2] == 0xb50 && level_seen == 48);
    assert(wq.unknown_04 == 0 && wq.unknown_102 == (waits ? 16 : 0));
    ready = 1;
    assert(go(-1) == 1 && log_count == 6 && log_[3] == 0x720 && log_[4] == id && log_[5] == 0xcf0);
    assert(job_seen[0] == 999 && job_seen[1] == 0 && job_seen[2] == 0 && job_seen[3] == 0);
    assert(step_args[0] == &owner && step_args[1] == &wq && wq.unknown_04 == 1 && wq.unknown_00 == 48);

    /* stage 1 */
    assert(go(-1) == 1 && log_count == 3 && log_[2] == 0x400 && wq.unknown_04 == 1 && job_seen[0] == 999);
    step_result = 5;
    assert(go(-1) == 1 && wq.unknown_04 == 2);

    /* stage 2 */
    step_result = 0; wq.unknown_102 = 9;
    assert(go(-1) == 1 && log_count == 3 && log_[2] == 0x670 && wq.unknown_04 == 2 && wq.unknown_102 == 9);
    step_result = 1;
    assert(go(-1) == 1 && log_count == 10 && memcmp(log_, second, sizeof second) == 0);
    assert(wq.unknown_102 == 0 && wq.unknown_04 == 3 && pair_seen == &wq);

    /* last stage */
    wq.unknown_00 = 100;
    assert(go(-1) == 1 && wq.unknown_00 == 115 && log_count == 3 && level_seen == 115 && wq.unknown_102 == 0);
    wq.unknown_00 = 114; wq.unknown_102 = 16; done = 1;
    assert(go(-1) == 1 && wq.unknown_00 == 128 && level_seen == 128 && wq.unknown_102 == 17 && log_count == 3);
    done = 0;
    assert(go(-1) == 1 && wq.unknown_102 == 18 && log_count == 4 && log_[3] == 0xd90);
    done = 1;
    assert(go(-1) == 0 && wq.unknown_102 == 19 && log_count == 6 && log_[4] == 0x780 && log_[5] == 0x453);
    busy = 1;
    assert(go(-1) == 0 && log_count == 8 && log_[5] == 0x7e0 && log_[6] == 1 && log_[7] == 0x453);
    busy = 0; ready = 0; done = 0; step_result = 0;
}

/* Ta_Eff_019 (branch coverage): mode 3 / -1; stage 0 at 64 / 63 with TaEffect_IsLoadModule 0 / 1;
 * stage 1 with SzEffect_IsUse 1 or not, SzEffect_GetStatus 2 or not, the flag word 1 or not,
 * loop bits at 0, 10, 11 and 21 (both Ta_DelUnit forms, first and last index);
 * last stage as in the other routines (clamp, 16 / 17 counter, Ta_IsFinishFade, busy). */
static void check_flag_stages(void)
{
    static const int start[] = {0x7e0, 0, 0x960, 0x020, 3, -1, 1, 1, 0x740, 16, 0xac0, 56};
    static const int bits[] = {0x7e0, 0, 0x1d0, 7, 0x2d0, 7, 0x370, 0, 0, 0x370, 0, 10, 0x370, 1, 0, 0x370, 1, 10};
    static const int next[] = {0x7e0, 0, 0x1d0, 7, 0x020, -1, 1, 0, 1, 0xa70, 16};
    struct DotrWaitFlagQueue *fq = (struct DotrWaitFlagQueue *)&wq;

    stage_fn = Ta_Eff_019;
    memset(&wq, 0, sizeof wq);
    busy = 0; ready = 0; done = 0; job_result = 7; busy_index = 1; state52 = 0;
    owner.unknown_08 = 0x200c01;
    assert(go(3) == 1 && log_count == 12 && memcmp(log_, start, sizeof start) == 0);
    assert(wq.unknown_00 == 128 && init_seen == &wq && pair_seen == &wq);

    /* stage 0 */
    wq.unknown_00 = 64;
    assert(go(-1) == 1 && wq.unknown_00 == 49 && log_count == 3 && log_[2] == 0xcf0 && level_seen == 49);
    wq.unknown_00 = 63;
    assert(go(-1) == 1 && wq.unknown_00 == 48 && log_count == 4 && log_[2] == 0xb50 && wq.unknown_04 == 0);
    ready = 1; wq.unknown_d4 = 2;
    assert(go(-1) == 1 && log_count == 5 && log_[3] == 0xa30 && log_[4] == 0xcf0 && level_seen == 48);
    assert(job_seen[0] == 56 && job_seen[1] == 0 && job_seen[2] == 0x200c01 && job_seen[3] == 0);
    assert(fq->unknown_20[2] == 1 && wq.unknown_06[2] == 7 && wq.unknown_d4 == 3 && wq.unknown_04 == 1);

    /* stage 1 */
    wq.unknown_06[0] = 7; fq->unknown_20[0] = 1;
    assert(go(-1) == 1 && log_count == 6 && log_[4] == 0x2d0 && log_[5] == 7 && fq->unknown_20[0] == 1);
    state52 = 2; fq->unknown_20[0] = 2;
    assert(go(-1) == 1 && log_count == 6 && fq->unknown_20[0] == 2);
    fq->unknown_20[0] = 1;
    assert(go(-1) == 1 && log_count == 18 && memcmp(log_, bits, sizeof bits) == 0 && fq->unknown_20[0] == 0);
    assert(wq.unknown_04 == 1);
    busy_index = 0; wq.unknown_102 = 9;
    assert(go(-1) == 1 && log_count == 11 && memcmp(log_, next, sizeof next) == 0);
    assert(wq.unknown_102 == 0 && wq.unknown_04 == 2 && step_args[0] == &owner && step_args[1] == &wq);

    /* last stage */
    wq.unknown_00 = 100;
    assert(go(-1) == 1 && wq.unknown_00 == 115 && log_count == 3 && level_seen == 115 && wq.unknown_102 == 0);
    wq.unknown_00 = 114; wq.unknown_102 = 16; done = 1;
    assert(go(-1) == 1 && wq.unknown_00 == 128 && level_seen == 128 && wq.unknown_102 == 17 && log_count == 3);
    done = 0;
    assert(go(-1) == 1 && wq.unknown_102 == 18 && log_count == 4 && log_[3] == 0xd90);
    done = 1;
    assert(go(-1) == 0 && wq.unknown_102 == 19 && log_count == 6 && log_[4] == 0x780 && log_[5] == 0x453);
    busy = 1;
    assert(go(-1) == 0 && log_count == 8 && log_[5] == 0x7e0 && log_[6] == 1 && log_[7] == 0x453);
    busy = 0; ready = 0; done = 0; job_result = 0; busy_index = 0; owner.unknown_08 = 0;
}

/* Ta_Eff_060 (branch coverage): mode 3 / -1; stage 0 at 70, 49 (lowered then clamped) and
 * 48, TaEffect_IsLoadModule 0 / 1, counter 29 / 30; stage 1 with SzEffect_IsUse 1 or not,
 * SzEffect_GetStatus 2 or not, the done flag set or clear; last stage as in the others. */
static void check_packed_stages(void)
{
    static const int start[] = {0x7e0, 0, 0x960, 0xfb0, 77, 0x020, 3, -1, 1, 1, 0xaf0, 30, 0x305, 100, 180, 0xac0, 560};
    static const int sent[] = {0x7e0, 0, 0x1d0, 7, 0x2d0, 7, 0x109, 3, 5, 0x370, 3, 5, 0x360, 3, 5,
                               0x580, 0x99, 4, 2, 0x1234, 0, 0x970, 0x305, 0};
    static const int next[] = {0x7e0, 0, 0x1d0, 7, 0x020, -1, 1, 0, 1, 0xa70, 16};
    struct DotrWaitFlagQueue *fq = (struct DotrWaitFlagQueue *)&wq;

    stage_fn = Ta_Eff_060;
    memset(&wq, 0, sizeof wq);
    busy = 0; ready = 0; done = 0; job_result = 7; busy_index = 1; state52 = 0;
    packed_result = 0x305; pos_result = 0x204;
    owner.unknown_08 = 77; owner.unknown_0c = 0x1234;
    assert(go(3) == 1 && log_count == 17 && memcmp(log_, start, sizeof start) == 0);
    assert(wq.unknown_00 == 128 && wq.unknown_02 == 0x305 && init_seen == &wq);

    /* stage 0 */
    wq.unknown_00 = 70;
    assert(go(-1) == 1 && wq.unknown_00 == 55 && wq.unknown_102 == 1 && log_count == 3 && level_seen == 55);
    wq.unknown_00 = 49;
    assert(go(-1) == 1 && wq.unknown_00 == 48 && log_count == 3 && log_[2] == 0xcf0 && level_seen == 48);
    assert(go(-1) == 1 && wq.unknown_00 == 48 && log_count == 4 && log_[2] == 0xb50 && wq.unknown_04 == 0);
    ready = 1; wq.unknown_102 = 28; fq->unknown_20[0] = 5;
    assert(go(-1) == 1 && wq.unknown_102 == 29 && log_count == 4 && wq.unknown_04 == 0);
    assert(go(-1) == 1 && wq.unknown_102 == 30 && log_count == 5 && log_[3] == 0xa30 && log_[4] == 0xcf0);
    assert(job_seen[0] == 560 && job_seen[1] == 0 && job_seen[2] == 0x305 && job_seen[3] == 0);
    assert(wq.unknown_06[0] == 7 && fq->unknown_20[0] == 0 && wq.unknown_04 == 1 && level_seen == 48);

    /* stage 1 */
    assert(go(-1) == 1 && log_count == 6 && log_[4] == 0x2d0 && log_[5] == 7 && fq->unknown_20[0] == 0);
    state52 = 2; fq->unknown_20[0] = 1;
    assert(go(-1) == 1 && log_count == 6 && fq->unknown_20[0] == 1);
    fq->unknown_20[0] = 0;
    assert(go(-1) == 1 && log_count == 24 && memcmp(log_, sent, sizeof sent) == 0 && fq->unknown_20[0] == 1);
    assert(wq.unknown_04 == 1);
    busy_index = 0; wq.unknown_102 = 9;
    assert(go(-1) == 1 && log_count == 11 && memcmp(log_, next, sizeof next) == 0);
    assert(wq.unknown_102 == 0 && wq.unknown_04 == 2 && step_args[0] == &owner && step_args[1] == &wq);

    /* last stage */
    wq.unknown_00 = 100;
    assert(go(-1) == 1 && wq.unknown_00 == 115 && log_count == 3 && level_seen == 115 && wq.unknown_102 == 0);
    wq.unknown_00 = 114; wq.unknown_102 = 16; done = 1;
    assert(go(-1) == 1 && wq.unknown_00 == 128 && level_seen == 128 && wq.unknown_102 == 17 && log_count == 3);
    done = 0;
    assert(go(-1) == 1 && wq.unknown_102 == 18 && log_count == 4 && log_[3] == 0xd90);
    done = 1;
    assert(go(-1) == 0 && wq.unknown_102 == 19 && log_count == 6 && log_[4] == 0x780 && log_[5] == 0x453);
    busy = 1;
    assert(go(-1) == 0 && log_count == 8 && log_[5] == 0x7e0 && log_[6] == 1 && log_[7] == 0x453);
    busy = 0; ready = 0; done = 0; job_result = 0; busy_index = 0; owner.unknown_08 = 0; owner.unknown_0c = 0;
}

int main(void)
{
    check_packed_stages();
    check_flag_stages();
    check_stages(Ta_Eff_018, 518, 0, 0);
    check_stages(Ta_Eff_017_A, 20, 1, 0);
    check_stages(Ta_Eff_016, 516, 1, 1);
    check_short_stages(Ta_Eff_021, 0x740, 31, 0);
    check_short_stages(Ta_Eff_023, 0x860, 523, 1);
    static const int start[] = {0x960, 0xaf0, 0, 5, 300, 300, 0x2c0, 0, -120, 80, 270};

    /* mode >= 0 (0 and positive), owner word 0xffff: reset only, first counter up. */
    memset(queue, 0, sizeof queue);
    assert(run(0xffff, 0, 0) == 1 && log_count == 1 && log_[0] == 0x960 && init_seen == queue && queue[0] == 1);
    /* Owner word set: SzDuel_SetWindowDraw(0) first, then both sends with their constants. */
    assert(run(5, 0, 3) == 1 && log_count == 13 && log_[0] == 0x7e0 && log_[1] == 0);
    assert(memcmp(log_ + 2, start, sizeof start) == 0 && queue[0] == 2);

    /* mode < 0, second counter 0: at or below the bound only the first counter moves. */
    queue[0] = 4; queue[2] = 0;
    assert(run(0xffff, 4, -1) == 1 && log_count == 0 && queue[0] == 5 && queue[2] == 0);
    /* Above the bound, owner word 0xffff: SzDuel_CloseMsgPopup and the second counter. */
    assert(run(0xffff, 4, -1) == 1 && log_count == 1 && log_[0] == 0x453 && queue[0] == 6 && queue[2] == 1);
    /* Above the bound, owner word set: also TaView_RestoreOriginalView(queue, 16). */
    queue[2] = 0;
    assert(run(5, 4, -1) == 1 && log_count == 5 && log_[2] == 0x453 && log_[3] == 0xa70 && log_[4] == 16);
    assert(pair_seen == queue && queue[2] == 1);

    /* Second counter nonzero, owner word 0xffff: SzDuel_CloseMsgPopup, 0. */
    assert(run(0xffff, 0, -1) == 0 && log_count == 1 && log_[0] == 0x453 && queue[129] == 0);
    /* Owner word set: the third counter runs to 16 returning 1, and 17 ends it. */
    queue[129] = 16;
    assert(run(5, 0, -1) == 1 && log_count == 2 && queue[129] == 17);
    assert(run(5, 0, -1) == 0 && queue[129] == 18);
    assert(log_count == 4 && log_[2] == 0x780 && log_[3] == 0x453);
    /* The same with SzDuel_GetWindowDisp nonzero: SzDuel_SetWindowDraw(1) before SzDuel_CloseMsgPopup. */
    owner.unknown_08 = 5; owner.unknown_0c = 0; log_count = 0; busy = 1;
    assert(Ta_Eff_WAIT(&owner, queue, -1) == 0 && queue[129] == 19);
    assert(log_count == 6 && log_[2] == 0x780 && log_[3] == 0x7e0 && log_[4] == 1 && log_[5] == 0x453);
    return 0;
}
