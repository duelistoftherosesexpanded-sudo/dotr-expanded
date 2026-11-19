/* Synthetic host tests for the hand_effects group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/hand_effects.h"

enum { INIT = 1, LOAD, ISLOAD, DO, ISUSE, SE, DEL, CLOSE, C615, STATUS,
       ENCOFS, FORCE, KIND, TURN, LDABL, TRAP, BUSY2, OPEN, WDRAW, WBUSY, WDISP, RATE, CARDNO, MSG, POPUP,
       USTAT, LP, FOCUS, BACK, RESTORE, FIRE, ENCH, ENCEFF, REFLEX, CRATE, CGOAL };
static int log_id[32], log_a[32], log_b[32], log_c[32], log_d[32], log_n;
static struct jp_8b329 log_call[32];
static void *log_p[32];
static int isload_answer, do_answer, isuse_answer, status_answer;
static int force_answer, turn_answer, ldabl_answer, trap41, trap43, trap46, disp_answer, rate_answer;
static int kind_q[4], kind_i;
static int enc_answer, reflex_answer, crate_answer, cgoal_answer;
static struct jp_8b3ce temp;

static void note(int id, void *p, int a, int b, struct jp_8b329 *call)
{
    assert(log_n < 32);
    log_id[log_n] = id, log_p[log_n] = p, log_a[log_n] = a, log_b[log_n] = b;
    if (call) {
        log_call[log_n] = *call;
    }
    log_n++;
}

void InitEffectTemp(struct jp_8b3ce *q) { note(INIT, q, 0, 0, NULL); }
int TaEffect_LoadModule(struct jp_8b3ce *q, int id) { note(LOAD, q, id, 0, NULL); return 0; }
int TaEffect_IsLoadModule(struct jp_8b3ce *q) { note(ISLOAD, q, 0, 0, NULL); return isload_answer; }
int Ta_Do_Effect(struct jp_8b329 *p, struct jp_8b3ce *q) { note(DO, q, 0, 0, p); return do_answer; }
int SzEffect_IsUse(int index) { note(ISUSE, NULL, index, 0, NULL); return isuse_answer; }
int SzEffect_GetStatus(int index) { note(STATUS, NULL, index, 0, NULL); return status_answer; }
int SzSound_PlaySE(int index, int value) { note(SE, NULL, index, value, NULL); return 0; }
int Ta_DelUnit2(int a, int b) { note(DEL, NULL, a, b, NULL); return 0; }
void SzDuel_CloseMsgPopup(void) { note(CLOSE, NULL, 0, 0, NULL); }
int TaLib_c615(void *efc, struct jp_8b329 *p, int n) { note(C615, efc, n, 0, p); return 0; }

static void note4(int id, int a, int b, int c, int d)
{
    note(id, NULL, a, b, NULL);
    log_c[log_n - 1] = c, log_d[log_n - 1] = d;
}

int Ta_GetEnchantOfs(void) { note(ENCOFS, NULL, 0, 0, NULL); return 9; }
int Ta_GetForceEffection(int a, int b) { note(FORCE, NULL, a, b, NULL); return force_answer; }
int SzDuel_GetUnitKind(int a, int b) { note(KIND, NULL, a, b, NULL); return kind_q[kind_i++]; }
int SzDuel_GetTurnOwner(void) { note(TURN, NULL, 0, 0, NULL); return turn_answer; }
int Ta_IsLDAbl(int a, int b) { note(LDABL, NULL, a, b, NULL); return ldabl_answer; }
int Ta_IsActiveTrapB(int a, int b) { note(TRAP, NULL, a, b, NULL); return b == 41 ? trap41 : b == 43 ? trap43 : trap46; }
int SzEffect_SetBusyFlag2(int packed, int flag) { note(BUSY2, NULL, packed, flag, NULL); return 0; }
void TaUtil_DoOpen(int a, int b, int mode) { note4(OPEN, a, b, mode, 0); }
void SzDuel_SetWindowDraw(int value) { note(WDRAW, NULL, value, 0, NULL); }
void SzDuel_SetWindowBusy(int value) { note(WBUSY, NULL, value, 0, NULL); }
int SzDuel_GetWindowDisp(void) { note(WDISP, NULL, 0, 0, NULL); return disp_answer; }
int SzDuel_GetUnitOpenRate(int a, int b) { note(RATE, NULL, a, b, NULL); return rate_answer; }
int SzDuel_GetUnitCardNo(int a, int b) { note(CARDNO, NULL, a, b, NULL); return 77; }
int SzDuel_GetMsgName(int index) { note(MSG, NULL, index, 0, NULL); return index + 5000; }
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value) { note4(POPUP, operation, a, b, value); }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { note4(USTAT, a, b, bit, value); return 0; }
int SzDuel_CalcLP(int index, int operation, int value) { note4(LP, index, operation, value, 0); return 0; }
void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value) { note4(FOCUS, who, packed, add, value); }
void TaView_ReqBackFocusCamera(unsigned char who, int packed, int add, int value) { note4(BACK, who, packed, add, value); }
void TaView_RestoreOriginalView(struct jp_8b3ce *Temp, unsigned char b) { note(RESTORE, Temp, b, 0, NULL); }
int TaView_ReqFireView(int a, int b, unsigned char c, unsigned char d) { note4(FIRE, a, b, c, d); return 0; }
int Enchant(int a, int b, int c, int d) { note4(ENCH, a, b, c, d); return 0; }

int Ta_GetEncEffect(int index) { note(ENCEFF, NULL, index, 0, NULL); return enc_answer; }
int TaEnc_IsActReflexEnc(int index) { note(REFLEX, NULL, index, 0, NULL); return reflex_answer; }
int SzDuel_GetUnitCtlRateC(int a, int b) { note(CRATE, NULL, a, b, NULL); return crate_answer; }
int SzDuel_GetUnitCtlGoalC(int a, int b) { note(CGOAL, NULL, a, b, NULL); return cgoal_answer; }

/* index of the nth (from 0) logged call of one kind, -1 when there is none */
static int at(int id, int nth)
{
    int i;

    for (i = 0; i < log_n; i++) {
        if (log_id[i] == id && nth-- == 0) {
            return i;
        }
    }
    return -1;
}

static int cnt(int id)
{
    int i, n = 0;

    for (i = 0; i < log_n; i++) {
        n += log_id[i] == id;
    }
    return n;
}

static int is4(int id, int nth, int a, int b, int c, int d)
{
    int i = at(id, nth);

    return i >= 0 && log_a[i] == a && log_b[i] == b && log_c[i] == c && log_d[i] == d;
}

static int is2(int id, int nth, int a, int b)
{
    int i = at(id, nth);

    return i >= 0 && log_a[i] == a && log_b[i] == b;
}

static int is_eff(int id, int number, unsigned from, unsigned to, unsigned par)
{
    int i = at(id, 0);

    return i >= 0 && log_call[i].eNO == number && log_call[i].dwFrom == from && log_call[i].dwTo == to &&
           log_call[i].dwPar == par;
}

static int is_call(int at, int number, const struct jp_8b329 *in)
{
    return log_call[at].eNO == number && log_call[at].dwFrom == in->dwFrom && log_call[at].dwTo == in->dwTo &&
           log_call[at].dwPar == 0;
}

/* Ta_Eff_Hand_Fail (branch coverage). */
static void check_fail(void)
{
    struct jp_8b329 in = {7, 0x80000305u, 0x0102, 0x55};

    assert(offsetof(struct jp_8b3ce, ePos) == 0x20 && offsetof(struct jp_8b3ce, eMod) == 0x80);
    assert(offsetof(struct jp_8b3ce, fObj2) == 0xd0 && offsetof(struct jp_8b3ce, nTasakiEfc) == 0x110);
    assert(sizeof(struct jp_8b3ce) == 0x540);
    /* ID >= 0: set-up, module 538, and the 602 call with a zero third argument */
    memset(&temp, 0, sizeof(temp));
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, 0) == 1 && log_n == 3);
    assert(log_id[0] == INIT && log_p[0] == &temp && log_id[1] == LOAD && log_p[1] == &temp && log_a[1] == 538);
    assert(log_id[2] == C615 && log_p[2] == temp.nTasakiEfc && log_a[2] == 0 && is_call(2, 602, &in));
    /* ID < 0, level 0, module not loaded: only the 602 call with -1 */
    isload_answer = 0;
    temp.ePos[0] = 5;
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 1 && log_n == 2 && log_id[0] == ISLOAD && log_p[0] == &temp);
    assert(log_id[1] == C615 && log_p[1] == temp.nTasakiEfc && log_a[1] == -1 && is_call(1, 602, &in));
    assert(temp.wLvl == 0 && temp.ePos[0] == 5);
    /* loaded: effect 538 started, its low byte kept, level raised */
    isload_answer = 1;
    do_answer = 0x1a7;
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == DO && log_p[1] == &temp);
    assert(is_call(1, 538, &in) && temp.eID[0] == 0xa7 && temp.ePos[0] == 0 && temp.wLvl == 1);
    assert(log_id[2] == C615 && log_a[2] == -1 && is_call(2, 602, &in));
    /* later levels, effect in use: the counter goes up; counts 1 and 15 play a sound */
    isuse_answer = 1;
    temp.wLvl = 2;
    temp.fObj2 = 0;
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 1 && log_n == 1 && log_id[0] == ISUSE && log_a[0] == 0xa7 && temp.fObj2 == 1);
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 1 && log_n == 2 && log_id[1] == SE && log_a[1] == 1 && log_b[1] == 76);
    assert(temp.fObj2 == 2);
    temp.fObj2 = 15;
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 1 && log_n == 2 && log_id[1] == SE && log_a[1] == 1 && log_b[1] == 74);
    assert(temp.fObj2 == 16);
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 1 && log_n == 1 && temp.fObj2 == 17);
    /* effect over: the source unit is removed (unsigned shift) and the popup closed */
    isuse_answer = 0;
    log_n = 0;
    assert(Ta_Eff_Hand_Fail(&in, &temp, -1) == 0 && log_n == 3 && log_id[1] == DEL && log_a[1] == 0x800003 && log_b[1] == 5);
    assert(log_id[2] == CLOSE && temp.fObj2 == 17 && temp.wLvl == 2);
}

/* Ta_Eff_Hand_Fusion (branch coverage). */
static void check_fusion(void)
{
    struct jp_8b329 in = {7, 0x80000305u, 0x0102, 0x55};

    /* ID >= 0: set-up, module 600, and the 602 call with a zero third argument */
    memset(&temp, 0, sizeof(temp));
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, 0) == 1 && log_n == 3);
    assert(log_id[0] == INIT && log_p[0] == &temp && log_id[1] == LOAD && log_p[1] == &temp && log_a[1] == 600);
    assert(log_id[2] == C615 && log_p[2] == temp.nTasakiEfc && log_a[2] == 0 && is_call(2, 602, &in));
    /* ID < 0, level 0, module not loaded: only the 602 call with -1 */
    isload_answer = 0;
    temp.ePos[0] = 5;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 2 && log_id[0] == ISLOAD && log_p[0] == &temp);
    assert(log_id[1] == C615 && log_p[1] == temp.nTasakiEfc && log_a[1] == -1 && is_call(1, 602, &in));
    assert(temp.wLvl == 0 && temp.ePos[0] == 5);
    /* loaded: effect 600 started, its low byte kept, level raised */
    isload_answer = 1;
    do_answer = 0x1a7;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == DO && log_p[1] == &temp);
    assert(is_call(1, 600, &in) && temp.eID[0] == 0xa7 && temp.ePos[0] == 0 && temp.wLvl == 1);
    assert(log_id[2] == C615 && log_a[2] == -1 && is_call(2, 602, &in));
    /* later levels, effect in use, status not 2: counts 1, 18 and 80 play a sound */
    isuse_answer = 1;
    status_answer = 1;
    temp.wLvl = 2;
    temp.fObj2 = 0;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 2 && log_id[0] == ISUSE && log_a[0] == 0xa7);
    assert(log_id[1] == STATUS && log_a[1] == 0xa7 && temp.fObj2 == 1);
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == SE && log_a[1] == 1 && log_b[1] == 76);
    assert(log_id[2] == STATUS && temp.fObj2 == 2);
    temp.fObj2 = 18;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == SE && log_a[1] == 1 && log_b[1] == 58);
    assert(temp.fObj2 == 19);
    temp.fObj2 = 80;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == SE && log_a[1] == 1 && log_b[1] == 59);
    assert(temp.fObj2 == 81 && temp.ePos[0] == 0);
    /* status 2, first time: both units removed (unsigned shift), parameter in the high half */
    status_answer = 2;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == (0x55 << 16 | 2) && log_n == 4 && temp.ePos[0] == 1);
    assert(log_id[2] == DEL && log_a[2] == 0x800003 && log_b[2] == 5 && log_id[3] == DEL && log_a[3] == 1 && log_b[3] == 2);
    /* status 2 again: nothing more */
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 1 && log_n == 2 && temp.ePos[0] == 1 && temp.fObj2 == 83);
    /* effect over: only the popup is closed */
    isuse_answer = 0;
    log_n = 0;
    assert(Ta_Eff_Hand_Fusion(&in, &temp, -1) == 0 && log_n == 2 && log_id[1] == CLOSE && temp.fObj2 == 83 && temp.wLvl == 2);
}

static struct jp_8b329 force_in;

static int force(int id, int k0, int k1, int k2)
{
    kind_q[0] = k0, kind_q[1] = k1, kind_q[2] = k2, kind_i = 0;
    log_n = 0;
    return Ta_Eff_Hand_Force(&force_in, &temp, id);
}

/* Ta_Eff_Hand_Force (branch coverage). The two `else` arms after Ta_Do_Effect's result is stored in an
 * unsigned byte and tested for >= 0 cannot be reached and are not tested. */
static void check_force(void)
{
    struct jp_8b329 in = {7, 0x02030405u, 1, 0x55};

    force_in = in;
    assert(offsetof(struct jp_8b3ce, bCamLvl) == 0x100 && offsetof(struct jp_8b3ce, wCamTime) == 0x102);
    assert(offsetof(struct jp_8b3ce, nTasakiEfc) == 0x110 && sizeof(struct jp_8b3ce) == 0x540);
    /* start, source kind below 32: the unit is the high half; ability 11 and equal low kind bits */
    memset(&temp, 0, sizeof(temp));
    force_answer = 300, turn_answer = 1, ldabl_answer = 1;
    assert(force(0, 0x04, 0x23, 0x43) == 1 && log_id[0] == INIT && log_p[0] == &temp && cnt(ENCOFS) == 1);
    assert(is2(FORCE, 0, 1, 9) && temp.wCount_1 == 300 && is2(KIND, 0, 2, 3) && is2(LDABL, 0, 11, 1));
    assert(is2(KIND, 1, 2, 3) && is2(KIND, 2, 1, 0) && cnt(TURN) == 2 && cnt(TRAP) == 0);
    assert(cnt(LOAD) == 1 && is2(LOAD, 0, 537, 0) && temp.wLvl == 2 && temp.wCount_2 == 0);
    assert(log_id[log_n - 1] == C615 && log_p[log_n - 1] == temp.nTasakiEfc && log_a[log_n - 1] == 0);
    assert(is_eff(C615, 602, 0x0203, 0x0405, 0));
    /* source kind 32 or more: the unit is the low half; kinds differ, no trap 41 and no trap 43 */
    memset(&temp, 0, sizeof(temp));
    trap41 = -1, trap43 = -1, trap46 = -1;
    assert(force(0, 0x20, 0x03, 0x04) == 1 && is2(KIND, 1, 4, 5) && cnt(TRAP) == 2 && is2(TRAP, 0, 1, 41));
    assert(is2(TRAP, 1, 1, 43) && cnt(LOAD) == 1 && is2(LOAD, 0, 537, 0) && temp.wLvl == 2 && temp.wCount_2 == 0);
    /* ability answer not 1 (one kind query only); trap 43 and trap 46 */
    memset(&temp, 0, sizeof(temp));
    ldabl_answer = 0, trap43 = 0x0102, trap46 = 0x0304;
    temp.wLvl = 9, temp.wCount_2 = 9;
    assert(force(0, 0x04, 0, 0) == 1 && cnt(KIND) == 1 && cnt(TRAP) == 3 && is2(TRAP, 2, 1, 46));
    assert(is2(LOAD, 0, 602, 0) && is2(LOAD, 1, 537, 0) && is2(BUSY2, 0, 0x0304, 0) && is4(OPEN, 0, 3, 4, 1, 0));
    assert(temp.fObj == 0x0304 && temp.fObj2 == 0 && temp.wLvl == 0 && temp.wCount_2 == 0);
    /* trap 43 without trap 46 */
    memset(&temp, 0, sizeof(temp));
    trap46 = -1;
    temp.wLvl = 9;
    assert(force(0, 0x04, 0, 0) == 1 && is2(LOAD, 0, 602, 0) && is2(LOAD, 1, 537, 0) && is2(BUSY2, 0, 0x0102, 0));
    assert(is4(OPEN, 0, 1, 2, 1, 0) && temp.fObj == 0x0102 && temp.wLvl == 0 && temp.wCount_2 == 1);
    /* trap 41 and trap 46 */
    memset(&temp, 0, sizeof(temp));
    trap41 = 0x0506, trap46 = 0x0304;
    temp.wLvl = 9, temp.wCount_2 = 9;
    assert(force(0, 0x04, 0, 0) == 1 && cnt(TRAP) == 2 && is2(TRAP, 1, 1, 46) && is2(LOAD, 1, 537, 0));
    assert(is2(BUSY2, 0, 0x0304, 0) && is4(OPEN, 0, 3, 4, 1, 0) && temp.fObj == 0x0304 && temp.fObj2 == 0);
    assert(temp.wLvl == 0 && temp.wCount_2 == 0 && cnt(WDRAW) == 0);
    /* trap 41 alone: window drawing off, module 547, second object word set */
    memset(&temp, 0, sizeof(temp));
    trap46 = -1;
    temp.wLvl = 9, temp.wCount_2 = 9;
    assert(force(0, 0x04, 0, 0) == 1 && is2(WDRAW, 0, 0, 0) && is2(LOAD, 0, 602, 0) && is2(LOAD, 1, 547, 0));
    assert(is2(BUSY2, 0, 0x0506, 0) && is4(OPEN, 0, 5, 6, 1, 0) && temp.fObj == 0x0506 && temp.fObj2 == 1);
    assert(temp.wLvl == 0 && temp.wCount_2 == 0 && is_eff(C615, 602, 0x0203, 0x0405, 0));

    /* ID < 0, modules not loaded: the 602 step with -1; the flag is set once the answer is 1 */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 300;
    isload_answer = 0;
    assert(force(-1, 0, 0, 0) == 1 && log_n == 2 && log_id[0] == C615 && log_a[0] == -1 && log_id[1] == ISLOAD);
    assert(is_eff(C615, 602, 0x0203, 0x0405, 0) && temp.eMod.fLoad == 0);
    isload_answer = 1;
    assert(force(-1, 0, 0, 0) == 1 && log_n == 2 && temp.eMod.fLoad != 0);
    /* loaded, a level without a case: nothing */
    temp.wLvl = 5;
    assert(force(-1, 0, 0, 0) == 1 && log_n == 0 && temp.wLvl == 5);
    /* level 0, card not fully open: only the 602 step */
    temp.wLvl = 0, temp.fObj = 0x0304, rate_answer = 10;
    assert(force(-1, 0, 0, 0) == 1 && log_n == 3 && is2(WDRAW, 0, 0, 0) && is2(RATE, 0, 3, 4));
    assert(log_a[at(C615, 0)] == -1 && is_eff(C615, 602, 0x0203, 0x0405, 0) && temp.wLvl == 0);
    /* level 0, open, both words zero: named popup, focus, effect 602 on the object */
    rate_answer = 255, do_answer = 0x1a7, temp.ePos[0] = 9;
    assert(force(-1, 0, 0, 0) == 1 && is2(MSG, 0, 825, 0) && is4(POPUP, 0, 0, -120, 80, 5825) && is2(WBUSY, 0, 1, 0));
    assert(is4(FOCUS, 0, 0, 0x0304, 180, 180) && is_eff(DO, 602, 0, 0x0304, 0) && log_p[at(DO, 0)] == &temp);
    assert(temp.eID[0] == 0xa7 && temp.ePos[0] == 0 && temp.oNum == 1 && temp.wLvl == 1 && cnt(C615) == 0);
    /* level 0, open, either word set: popup 259 */
    temp.wLvl = 0, temp.wCount_2 = 1;
    assert(force(-1, 0, 0, 0) == 1 && cnt(MSG) == 0 && is4(POPUP, 0, 0, -120, 80, 259) && temp.oNum == 2);
    temp.wLvl = 0, temp.wCount_2 = 0, temp.fObj2 = 1;
    assert(force(-1, 0, 0, 0) == 1 && cnt(MSG) == 0 && is4(POPUP, 0, 0, -120, 80, 259) && temp.oNum == 3);
    /* level 1, effect still in use */
    temp.wLvl = 1, temp.oNum = 1, isuse_answer = 1;
    assert(force(-1, 0, 0, 0) == 1 && log_n == 2 && is2(WDRAW, 0, 0, 0) && is2(ISUSE, 0, 0xa7, 0) && temp.wLvl == 1);
    /* level 1, effect over, second word set: back camera on the other side, level 16 */
    isuse_answer = 0;
    assert(force(-1, 0, 0, 0) == 1 && is4(USTAT, 0, 3, 4, 0, 1) && is2(WBUSY, 0, 1, 0) && is4(BACK, 0, 0, 0, 180, 180));
    assert(temp.wLvl == 16 && cnt(C615) == 0);
    temp.wLvl = 1, force_in.dwTo = 0;
    assert(force(-1, 0, 0, 0) == 1 && is4(BACK, 0, 0, 0x100, 180, 180) && temp.wLvl == 16);
    force_in.dwTo = 1;
    /* level 1, effect over, second word clear: step 5, view restored, next level */
    temp.wLvl = 1, temp.fObj2 = 0;
    assert(force(-1, 0, 0, 0) == 1 && cnt(WDRAW) == 2 && is2(WDRAW, 1, 1, 0) && is_eff(C615, 5, 0x0203, 0x0405, 0));
    assert(is2(WBUSY, 0, 0, 0) && log_p[at(RESTORE, 0)] == &temp && is2(RESTORE, 0, 0, 0) && temp.wLvl == 2);
    /* level 2, first word 1, source kind 32 or more: card-name popup, effect 537 with packed parameter */
    temp.wCount_2 = 1;
    assert(force(-1, 0x20, 0, 0) == 1 && is_eff(C615, 5, 0x0203, 0x0405, 0) && is2(CARDNO, 0, 3, 4) && is2(MSG, 0, 77, 0));
    assert(is4(POPUP, 0, 0, 0, -160, 5077) && is2(KIND, 0, 2, 3) && is_eff(DO, 537, 0x0203, 0x0405, 300 << 16 | 1 << 8 | 3));
    assert(temp.eID[1] == 0xa7 && temp.ePos[1] == 0 && temp.oNum == 2 && temp.wLvl == 3);
    /* level 2, first word not 1, source kind below 32 */
    temp.wLvl = 2, temp.wCount_2 = 0;
    assert(force(-1, 0x04, 0, 0) == 1 && cnt(CARDNO) == 0 && is4(POPUP, 0, 0, 0, -160, 268));
    assert(is_eff(DO, 537, 0x0203, 0x0405, 300 << 16) && temp.oNum == 3 && temp.wLvl == 3);
    /* level 3, effect over */
    temp.oNum = 1;
    assert(force(-1, 0, 0, 0) == 0 && log_n == 2 && is2(ISUSE, 0, 0xa7, 0) && log_id[1] == CLOSE);
    /* level 3, in use, status not 2: counts 1, 15 and 16 play a sound */
    isuse_answer = 1, status_answer = 1, temp.fObj2 = 1;
    assert(force(-1, 0, 0, 0) == 1 && is2(SE, 0, 1, 76) && temp.fObj2 == 2 && is2(STATUS, 0, 0xa7, 0) && cnt(KIND) == 0);
    assert(force(-1, 0, 0, 0) == 1 && cnt(SE) == 0 && temp.fObj2 == 3);
    temp.fObj2 = 15;
    assert(force(-1, 0, 0, 0) == 1 && is2(SE, 0, 1, 77) && temp.fObj2 == 16);
    assert(force(-1, 0, 0, 0) == 1 && is2(SE, 0, 1, 59) && temp.fObj2 == 17);
    /* status 2, position byte zero, source kind below 32: enchant the high unit, remove the low one */
    status_answer = 2, temp.wCount_2 = 1, temp.ePos[0] = 0x100;
    assert(force(-1, 0x04, 0, 0) == 1 && is4(ENCH, 0, 2, 3, 1 << 16 | 0x55, 1) && is2(DEL, 0, 4, 5));
    assert(temp.ePos[0] == 0xffff);
    /* position byte not zero: nothing more */
    assert(force(-1, 0x04, 0, 0) == 1 && cnt(ENCH) == 0 && cnt(DEL) == 0 && cnt(KIND) == 0);
    /* source kind 32 or more: the units change roles */
    temp.ePos[0] = 0;
    assert(force(-1, 0x20, 0, 0) == 1 && is4(ENCH, 0, 4, 5, 1 << 16 | 0x55, 1) && is2(DEL, 0, 2, 3));
    assert(temp.ePos[0] == 0xffff);
    /* level 16: popup 814, effect 547 from the other side's row */
    temp.wLvl = 16;
    assert(force(-1, 0, 0, 0) == 1 && is2(WDRAW, 0, 0, 0) && is2(MSG, 0, 814, 0) && is4(POPUP, 0, 0, -120, 80, 5814));
    assert(is_eff(DO, 547, 0, 0x100, 300) && temp.eID[1] == 0xa7 && temp.oNum == 2 && temp.wLvl == 17);
    temp.wLvl = 16, force_in.dwTo = 0;
    assert(force(-1, 0, 0, 0) == 1 && is_eff(DO, 547, 0x100, 0, 300) && temp.oNum == 3 && temp.wLvl == 17);
    force_in.dwTo = 1;
    /* level 17, in use: the camera counter goes up; at 6 the fire view is requested */
    temp.oNum = 1, temp.wCamTime = 5;
    assert(force(-1, 0, 0, 0) == 1 && log_n == 2 && is2(WDRAW, 0, 0, 0) && temp.wCamTime == 6 && temp.wLvl == 17);
    assert(force(-1, 0, 0, 0) == 1 && is4(FIRE, 0, 30, 0x100, 100, 180) && temp.wCamTime == 7);
    /* level 17, effect over: one unit removed by source kind, life points, view restored */
    isuse_answer = 0, disp_answer = 1;
    assert(force(-1, 0x04, 0, 0) == 0 && is2(DEL, 0, 4, 5) && is4(LP, 0, 1, 1, 300, 0) && cnt(CLOSE) == 1);
    assert(is2(WBUSY, 0, 0, 0) && is2(RESTORE, 0, 0, 0) && cnt(WDISP) == 1 && cnt(WDRAW) == 2 && is2(WDRAW, 1, 1, 0));
    disp_answer = 0;
    assert(force(-1, 0x20, 0, 0) == 0 && is2(DEL, 0, 2, 3) && cnt(WDRAW) == 1 && temp.wLvl == 17);
}

static struct jp_8b329 enc_in = {7, 0x01020304u, 6, 0x10055};

static int enc(int id, int k0, int k1, int k2)
{
    kind_q[0] = k0, kind_q[1] = k1, kind_q[2] = k2, kind_i = 0;
    log_n = 0;
    return Ta_Eff_Hand_Enc(&enc_in, &temp, id);
}

/* Ta_Eff_Hand_Enc (branch coverage). */
static void check_enc(void)
{
    static const int mods[5][2] = {{536, 536}, {540, 540}, {541, 541}, {542, 542}, {0x10999, 536}};
    static const int se15[5] = {59, 75, 78, 79, 0};
    int i;

    /* effect 537 (low 16 bits of the answer): the whole call goes to Ta_Eff_Hand_Force */
    memset(&temp, 0, sizeof(temp));
    enc_answer = 0x10000 | 537;
    isload_answer = 0;
    assert(enc(-1, 0, 0, 0) == 1 && log_n == 3 && log_id[0] == ENCEFF && log_a[0] == 0x10055);
    assert(log_id[1] == C615 && log_a[1] == -1 && log_id[2] == ISLOAD);
    /* ID >= 0: module by effect, no trap 43: level 2 */
    for (i = 0; i < 5; i++) {
        memset(&temp, 0, sizeof(temp));
        temp.wCount_1 = 9;
        enc_answer = mods[i][0];
        trap43 = -1;
        assert(enc(0, 0, 0, 0) == 1 && log_n == 7 && log_id[1] == INIT && log_p[1] == &temp);
        assert(cnt(ENCEFF) == 2 && is2(ENCEFF, 1, 0x10055, 0) && is2(LOAD, 0, mods[i][1], 0) && is2(KIND, 0, 1, 2));
        assert(is2(TRAP, 0, 6, 43) && temp.wLvl == 2 && temp.wCount_1 == 0);
        assert(log_id[6] == C615 && log_p[6] == temp.nTasakiEfc && log_a[6] == 0 && is_eff(C615, 602, 0x102, 0x304, 0));
    }
    enc_answer = 536;
    /* trap 43 found, reflex enchant active: open the trap-46 unit */
    memset(&temp, 0, sizeof(temp));
    trap43 = 0x203, trap46 = 0x104, reflex_answer = 1;
    assert(enc(0, 0, 0, 0) == 1 && is2(REFLEX, 0, 0x55, 0) && cnt(TURN) == 0 && is2(WDRAW, 0, 0, 0));
    assert(is2(LOAD, 1, 602, 0) && is2(TRAP, 1, 6, 46) && temp.wLvl == 0 && temp.wCount_1 == 0 && temp.wCount_2 == 0x104);
    assert(is4(OPEN, 0, 1, 4, 1, 0) && is_eff(C615, 602, 0x102, 0x304, 0));
    /* no reflex, no LD ability, no trap 46: open the trap-43 unit */
    memset(&temp, 0, sizeof(temp));
    trap46 = -1, reflex_answer = 0, ldabl_answer = 0, turn_answer = 1;
    assert(enc(0, 0, 0, 0) == 1 && is2(LDABL, 0, 11, 1) && cnt(KIND) == 1);
    assert(temp.wLvl == 0 && temp.wCount_1 == 1 && temp.wCount_2 == 0x203 && is4(OPEN, 0, 2, 3, 1, 0));
    /* LD ability and equal kinds (unit from the low half when the kind is 32 or more): level 2 */
    memset(&temp, 0, sizeof(temp));
    ldabl_answer = 1;
    assert(enc(0, 0x20, 0x45, 0x25) == 1 && is2(KIND, 1, 3, 4) && is2(KIND, 2, 1, 0) && cnt(OPEN) == 0);
    assert(temp.wLvl == 2 && cnt(LOAD) == 1);
    /* kinds differ (unit from the high half): open */
    memset(&temp, 0, sizeof(temp));
    assert(enc(0, 0, 0x45, 0x26) == 1 && is2(KIND, 1, 1, 2) && cnt(OPEN) == 1 && temp.wCount_1 == 1);
    /* ID < 0, module not loaded: 602 call with -1; the window call only for a non-zero wCount_2 */
    memset(&temp, 0, sizeof(temp));
    isload_answer = 0;
    assert(enc(-1, 0, 0, 0) == 1 && log_n == 3 && log_id[1] == ISLOAD && log_p[1] == &temp);
    assert(log_id[2] == C615 && log_a[2] == -1 && is_eff(C615, 602, 0x102, 0x304, 0));
    temp.wCount_2 = 0x203;
    assert(enc(-1, 0, 0, 0) == 1 && log_n == 4 && is2(WDRAW, 0, 0, 0) && log_id[3] == C615);
    /* level 0: not open yet */
    isload_answer = 1;
    rate_answer = 254;
    assert(enc(-1, 0, 0, 0) == 1 && is2(WDRAW, 0, 0, 0) && is2(RATE, 0, 2, 3) && cnt(DO) == 0);
    assert(log_a[at(C615, 0)] == -1 && is_eff(C615, 602, 0x102, 0x304, 0) && temp.wLvl == 0);
    /* level 0: open, message by wCount_1, effect 602 on the unit */
    rate_answer = 255, do_answer = 0x1c8;
    assert(enc(-1, 0, 0, 0) == 1 && is2(MSG, 0, 825, 0) && is4(POPUP, 0, 0, -120, 80, 5825) && is2(WBUSY, 0, 1, 0));
    assert(is4(FOCUS, 0, 0, 0x203, 180, 180) && is_eff(DO, 602, 0, 0x203, 0) && log_p[at(DO, 0)] == &temp);
    assert(temp.eID[0] == 0xc8 && temp.oNum == 1 && temp.wLvl == 1 && cnt(C615) == 0);
    temp.wLvl = 0, temp.wCount_1 = 1;
    assert(enc(-1, 0, 0, 0) == 1 && cnt(MSG) == 0 && is4(POPUP, 0, 0, -120, 80, 259) && temp.oNum == 2 && temp.wLvl == 1);
    /* level 1: effect still in use */
    isuse_answer = 1;
    assert(enc(-1, 0, 0, 0) == 1 && is2(ISUSE, 0, 0xc8, 0) && is2(WDRAW, 0, 0, 0) && log_n == 4 && temp.wLvl == 1);
    temp.wCount_2 = 0;
    assert(enc(-1, 0, 0, 0) == 1 && log_n == 3 && cnt(WDRAW) == 0);
    /* level 1: effect done */
    temp.wCount_2 = 0x203;
    isuse_answer = 0;
    assert(enc(-1, 0, 0, 0) == 1 && is4(USTAT, 0, 2, 3, 0, 1) && cnt(CLOSE) == 1 && is_eff(C615, 5, 0x102, 0x304, 0));
    assert(log_a[at(C615, 0)] == -1 && is2(WBUSY, 0, 0, 0) && is2(RESTORE, 0, 0, 0) && log_p[at(RESTORE, 0)] == &temp);
    assert(temp.wLvl == 2);
    /* level 2: effect number by enchant effect, parameter 3 for a kind of 32 or more */
    for (i = 0; i < 5; i++) {
        temp.wLvl = 2, temp.oNum = 2, temp.ePos[2] = 7, temp.wCount_1 = 1;
        enc_answer = mods[i][0];
        assert(enc(-1, i & 1 ? 0x20 : 0x1f, 0, 0) == 1 && is_eff(C615, 5, 0x102, 0x304, 0) && is2(KIND, 0, 1, 2));
        assert(is_eff(DO, mods[i][1], 0x102, 0x304, i & 1 ? 3 : 0) && temp.eID[2] == 0xc8 && temp.ePos[2] == 0);
        assert(temp.oNum == 3 && temp.wLvl == 3);
        assert(cnt(POPUP) == (i == 0) && (i != 0 || (is2(CARDNO, 0, 2, 3) && is2(MSG, 0, 77, 0) && is4(POPUP, 0, 0, 0, -160, 5077))));
    }
    temp.wLvl = 2, temp.wCount_1 = 0;
    enc_answer = 536;
    assert(enc(-1, 0, 0, 0) == 1 && cnt(CARDNO) == 0 && is4(POPUP, 0, 0, 0, -160, 262) && temp.oNum == 4);
    /* level 3, in use: sounds by the counter fObj2 and the enchant effect */
    temp.wLvl = 3, temp.oNum = 1, temp.wCount_1 = 2;
    isuse_answer = 1, status_answer = 1;
    temp.fObj2 = 0;
    assert(enc(-1, 0, 0, 0) == 1 && cnt(SE) == 0 && temp.fObj2 == 1 && is2(ISUSE, 0, 0xc8, 0) && is2(STATUS, 0, 0xc8, 0));
    assert(enc(-1, 0, 0, 0) == 1 && is2(SE, 0, 1, 76) && temp.fObj2 == 2 && cnt(ENCH) == 0 && cnt(ENCEFF) == 1);
    for (i = 0; i < 5; i++) {
        enc_answer = mods[i][0];
        temp.fObj2 = 15;
        assert(enc(-1, 0, 0, 0) == 1 && cnt(SE) == (se15[i] != 0) && (se15[i] == 0 || is2(SE, 0, 1, se15[i])));
        assert(temp.fObj2 == 16 && cnt(ENCEFF) == 2);
        assert(enc(-1, 0, 0, 0) == 1 && cnt(SE) == (i == 2 || i == 3) && (cnt(SE) == 0 || is2(SE, 0, 1, 59)));
        assert(temp.fObj2 == 17);
    }
    /* status 2: enchant once (low byte of the position word), target by kind */
    status_answer = 2;
    temp.ePos[0] = 0x101;
    assert(enc(-1, 0, 0, 0) == 1 && cnt(KIND) == 0 && cnt(ENCH) == 0 && temp.ePos[0] == 0x101);
    temp.ePos[0] = 0x100;
    assert(enc(-1, 0x1f, 0, 0) == 1 && is2(KIND, 0, 1, 2) && is4(ENCH, 0, 1, 2, 0x20000 | 0x10055, 0) && temp.ePos[0] == 0xffff);
    temp.ePos[0] = 0;
    assert(enc(-1, 0x20, 0, 0) == 1 && is4(ENCH, 0, 3, 4, 0x20000 | 0x10055, 0) && temp.ePos[0] == 0xffff && temp.wLvl == 3);
    /* level 3, effect done: wait for the control rate, then delete the other unit */
    isuse_answer = 0;
    crate_answer = 1, cgoal_answer = 2, disp_answer = 1;
    assert(enc(-1, 0x1f, 0, 0) == 1 && cnt(CLOSE) == 1 && is2(CRATE, 0, 3, 4) && is2(CGOAL, 0, 3, 4) && cnt(DEL) == 0);
    assert(enc(-1, 0x20, 0, 0) == 1 && cnt(CLOSE) == 1 && is2(CRATE, 0, 1, 2) && is2(CGOAL, 0, 1, 2) && cnt(DEL) == 0);
    cgoal_answer = 1;
    assert(enc(-1, 0x1f, 0, 0) == 0 && is2(DEL, 0, 3, 4) && cnt(WDISP) == 1 && is2(WDRAW, 0, 1, 0) && cnt(CLOSE) == 2);
    disp_answer = 0;
    assert(enc(-1, 0x20, 0, 0) == 0 && is2(DEL, 0, 1, 2) && cnt(WDRAW) == 0 && cnt(CLOSE) == 2);
    /* any other level */
    temp.wLvl = 4;
    assert(enc(-1, 0, 0, 0) == 1 && log_n == 2 && temp.wLvl == 4);
}

int main(void)
{
    check_fail();
    check_fusion();
    check_force();
    check_enc();
    return 0;
}
