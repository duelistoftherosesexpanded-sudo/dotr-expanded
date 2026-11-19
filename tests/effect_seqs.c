/* Synthetic host tests for the effect_seqs group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/effect_seqs.h"

enum { WDRAW = 1, WDISP, INIT, LOAD, ISLOAD, NUM, OWNER, FOCUS, STDVIEW, FADE, POPUP, DEPEND, BLACK, ISUSE, CLOSE, DP, AP,
       RESTORE, FINFADE, MSG, UDP, UAP, CARDNO, ADDDECK, PACK, DELUNIT, DOEFF, STATUS, KIND, ATTR, TDEL, ROT, FADE2,
       TOPVIEW, DOOPEN, USTAT, CURSE, OPENRATE, BUSY2, LOC, ADDUNIT, LPREV, CALCLP, UKIND, TOPDOWN, SIDE, SPELL, GAP, GDP, GAP3, GDP3, STANDBY, OUCH, UFADE,
       HANDCLOSE, DESTINY, CTRL, BLACKB, SE, BACKFOCUS, CTLC };
static int log_id[64], log_a[64], log_b[64], log_c[64], log_d[64], log_n;
static void *log_p[64];
static int disp_answer, isload_answer, num_answer, owner_answer, finfade_answer, isuse_answer[24];
static struct jp_69942 temp;
static struct jp_6989c depend_call, do_call, do_prev;
static int do_answer, status_answer[24], status_once, openrate_answer, loc_answer, addunit_e;

static int isload_drop, lprev_zero, getlp_zero;
static int note(int id, void *p, int a, int b, int c, int d)
{
    assert(log_n < 64);
    log_id[log_n] = id, log_p[log_n] = p, log_a[log_n] = a, log_b[log_n] = b, log_c[log_n] = c, log_d[log_n] = d;
    return log_n++;
}
static int is(int at, int id, int a, int b, int c, int d)
{
    return log_id[at] == id && log_a[at] == a && log_b[at] == b && log_c[at] == c && log_d[at] == d;
}

void SzDuel_SetWindowDraw(int value) { note(WDRAW, NULL, value, 0, 0, 0); }
int SzDuel_GetWindowDisp(void) { note(WDISP, NULL, 0, 0, 0, 0); return disp_answer; }
void InitEffectTemp(struct jp_69942 *q) { note(INIT, q, 0, 0, 0, 0); }
int TaEffect_LoadModule(struct jp_69942 *q, int id) { note(LOAD, q, id, 0, 0, 0); return 0; }
int TaEffect_IsLoadModule(struct jp_69942 *q) { note(ISLOAD, q, 0, 0, 0, 0); return isload_drop && log_n > 2 ? 0 : isload_answer; }
enum { GUSTAT = 200, CONV, RANDOM, CTLRATE };
static int num_by_value, owner_by_n, gustat_zero_pos = -1, random_answer;
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    note(NUM, NULL, (int)value, 0, 0, 0);
    return num_by_value && value == 0 ? 0 : num_answer;
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n) { note(OWNER, NULL, word, n, 0, 0); return owner_by_n ? owner_answer + n : owner_answer; }
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position) { note(CONV, NULL, (int)word, high, position, 0); return word * 16 + position; }
int Ta_GetRandomUnit(int word) { note(RANDOM, NULL, word, 0, 0, 0); return random_answer; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { note(GUSTAT, NULL, a, b, bit, 0); return b != gustat_zero_pos; }
int SzDuel_SetUnitCtlRate(int a, int b, int pair, int value) { note(CTLRATE, NULL, a, b, pair, value); return 0; }
void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value) { note(FOCUS, NULL, who, packed, add, value); }
void TaView_ReqStdCompleteView(unsigned char who) { note(STDVIEW, NULL, who, 0, 0, 0); }
void Ta_AllUnitFade(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID, int dir, int flag, int value)
{
    log_p[note(FADE, eInfo, ID, dir, flag, value)] = Temp;
}
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value) { note(POPUP, NULL, operation, a, b, value); }
void Ta_SetEffectDependOnFlagLst(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall, int id)
{
    (void)eInfo;
    depend_call = *pCall;
    note(DEPEND, Temp, id, 0, 0, 0);
}
void SzEffect_SetBlackOutForDuel(int value) { note(BLACK, NULL, value, 0, 0, 0); }
int SzEffect_IsUse(int index) { note(ISUSE, NULL, index, 0, 0, 0); return isuse_answer[index]; }
void SzDuel_CloseMsgPopup(void) { note(CLOSE, NULL, 0, 0, 0, 0); }
int TaUtil_CalcUnitDP(int a, int b, int op, int val) { note(DP, NULL, a, b, op, val); return 0; }
int TaUtil_CalcUnitAP(int a, int b, int op, int val) { note(AP, NULL, a, b, op, val); return 0; }
void TaView_RestoreOriginalView(struct jp_69942 *Temp, unsigned char b) { note(RESTORE, Temp, b, 0, 0, 0); }
int Ta_IsFinishFade(void) { note(FINFADE, NULL, 0, 0, 0, 0); return finfade_answer; }
int SzDuel_GetMsgName(int index) { note(MSG, NULL, index, 0, 0, 0); return 0x4D5347; }
int SzDuel_GetUnitDP(int a, int b, int flag) { note(UDP, NULL, a, b, flag, 0); return 500 + a * 100 + b; }
int SzDuel_GetUnitAP(int a, int b, int flag) { note(UAP, NULL, a, b, flag, 0); return 300 + a * 10 + b; }
int SzDuel_GetUnitCardNo(int a, int b) { note(CARDNO, NULL, a, b, 0, 0); return 700 + a * 20 + b; }
int SzDuel_AddDeck(int index, int value) { note(ADDDECK, NULL, index, value, 0, 0); return 0; }
void SzDuel_PackDeck(int index) { note(PACK, NULL, index, 0, 0, 0); }
int SzDuel_DelUnit(int a, int b) { note(DELUNIT, NULL, a, b, 0, 0); return 0; }
int SzEffect_GetStatus(int index)
{
    int r = status_answer[index];

    note(STATUS, NULL, index, 0, 0, 0);
    if (status_once) {
        status_answer[index] = 0;
    }
    return r;
}
int SzDuel_SetUnitKind(int a, int b, int value) { note(KIND, NULL, a, b, value, 0); return 0; }
int SzDuel_SetUnitAttr(int a, int b, int value) { note(ATTR, NULL, a, b, value, 0); return 0; }
int Ta_Do_Effect(struct jp_6989c *eInfo, struct jp_69942 *Temp)
{
    do_prev = do_call;
    do_call = *eInfo;
    note(DOEFF, Temp, 0, 0, 0, 0);
    return do_answer;
}
int Ta_DelUnit(int a, int b) { note(TDEL, NULL, a, b, 0, 0); return 0; }
void TaView_ReqRotCamera(unsigned char b, int delta) { note(ROT, NULL, b, delta, 0, 0); }
void Ta_AllUnitFade2(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID, int dir, int flag, int value)
{
    log_p[note(FADE2, eInfo, ID, dir, flag, value)] = Temp;
}
void TaView_ReqTopView(void) { note(TOPVIEW, NULL, 0, 0, 0, 0); }
void TaUtil_DoOpen(int a, int b, int mode) { note(DOOPEN, NULL, a, b, mode, 0); }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { note(USTAT, NULL, a, b, bit, value); return 0; }
void Ta_SetUnitCurseRest(int a, int b, unsigned char n) { note(CURSE, NULL, a, b, n, 0); }
static int openrate_side2, ctlc_answer, ukind_zero;
int SzDuel_GetUnitOpenRate(int a, int b)
{
    note(OPENRATE, NULL, a, b, 0, 0);
    return openrate_side2 && a == 2 ? openrate_side2 : openrate_answer;
}
int SzDuel_GetUnitCtlRateC(int a, int b) { note(CTLC, NULL, a, b, 0, 0); return ctlc_answer; }
int SzEffect_SetBusyFlag2(int packed, int flag) { note(BUSY2, NULL, packed, flag, 0, 0); return 0; }
static int standby_answer;
int SzEffect_IsModelStandby2(int packed) { note(STANDBY, NULL, packed, 0, 0, 0); return standby_answer; }
void TaEffect_SetOuchMotion(int packed) { note(OUCH, NULL, packed, 0, 0, 0); }
void Ta_UnitFade(int side, int pos, int dir, int value, int flags) { note(UFADE, NULL, side, pos, dir, value), (void)flags; }
static int loc_by_unit;
int SzDuel_GetUnitLoc(int a, int b)
{
    note(LOC, NULL, a, b, 0, 0);
    return loc_by_unit ? ((a + 2) << 8) | (b + 1) : loc_answer;
}
int TaUtil_GetUnitAP(int a, int b, int c, int d) { note(GAP, NULL, a, b, c, d); return 1000 + a * 100 + b; }
int TaUtil_GetUnitDP(int a, int b, int c, int d) { note(GDP, NULL, a, b, c, d); return 2000 + a * 100 + b; }
int TaUtil_GetUnitAP3(int a, int b, int c, int d) { note(GAP3, NULL, a, b, c, d); return 300; }
int TaUtil_GetUnitDP3(int a, int b, int c, int d) { note(GDP3, NULL, a, b, c, d); return 400; }
static struct jp_6a917 *addunit_answer;
struct jp_6a917 *SzDuel_AddUnit(int a, int b, int c, int d, int e) { addunit_e = e; note(ADDUNIT, NULL, a, b, c, d); return addunit_answer; }
int Ta_GetLPRevByNaturalEffect(int a, int b) { note(LPREV, NULL, a, b, 0, 0); return lprev_zero ? 0 : 0x1234; }
int SzDuel_CalcLP(int index, int operation, int value) { note(CALCLP, NULL, index, operation, value, 0); return 0; }
int SzDuel_GetUnitKind(int a, int b) { note(UKIND, NULL, a, b, 0, 0); return ukind_zero ? 3 : ((a * 11 + b + 1) << 5) | 3; }
void TaView_ReqStdTopDownView(unsigned char flag) { note(TOPDOWN, NULL, flag, 0, 0, 0); }
int TaUtil_GetObjSideNum(unsigned int dwObj) { note(SIDE, NULL, (int)dwObj, 0, 0, 0); return 5; }
int SzDuel_SetUnitSpellRest(int a, int b, int value) { note(SPELL, NULL, a, b, value, 0); return 0; }

static int ctrl_answer;
void SzDuel_CloseHandPopup(void) { note(HANDCLOSE, NULL, 0, 0, 0, 0); }
void TaView_SetDestinyCamera(unsigned char flag) { note(DESTINY, NULL, flag, 0, 0, 0); }
int Ta_CalcCtrlRate(unsigned short value, int amount, int direction) { note(CTRL, NULL, value, amount, direction, 0); return ctrl_answer; }
void SzEffect_SetBlackOutForBattle(int value) { note(BLACKB, NULL, value, 0, 0, 0); }
int SzSound_PlaySE(int index, int value) { note(SE, NULL, index, value, 0, 0); return 0; }
void TaView_ReqBackFocusCamera(unsigned char who, int packed, int add, int value) { note(BACKFOCUS, NULL, who, packed, add, value); }

static int step(int (*f)(struct jp_6989c *, struct jp_69942 *, int), struct jp_6989c *in, int id)
{
    log_n = 0;
    return f(in, &temp, id);
}

/* Ta_Eff_057 (branch coverage). */
static void check_057(void)
{
    /* targets: own slots 0 and 10, other side's slots 0 and 10 (bits 21, 11, 10, 0) */
    struct jp_6989c in = {57, 0x102, 0x200C01, 0x7001F4};

    assert(offsetof(struct jp_69942, oNum) == 0xd4 && offsetof(struct jp_69942, camDat) == 0xe0);
    assert(offsetof(struct jp_69942, wCamTime) == 0x102 && offsetof(struct jp_69942, nTasakiEfc) == 0x110);
    assert(sizeof(struct jp_69942) == 0x540);
    memset(&temp, 0, sizeof(temp));
    /* ID >= 0 with one target: camera on that unit */
    num_answer = 1, owner_answer = 0x10a;
    assert(step(Ta_Eff_057, &in, 3) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, NUM, 0x200C01, 0, 0, 0) && is(3, OWNER, 0x200C01, 0, 0, 0) && is(4, FOCUS, 16, 0x10a, 100, 180));
    assert(is(5, FADE, 3, -1, 1, 1) && log_p[5] == &temp && is(6, LOAD, 557, 0, 0, 0) && temp.wCount_1 == 128);
    /* several targets: the standard view */
    num_answer = 2;
    assert(step(Ta_Eff_057, &in, 0) == 1 && log_n == 6 && is(3, STDVIEW, 16, 0, 0, 0) && is(4, FADE, 0, -1, 1, 1));
    /* level 0: the black-out level falls by 15 to 48 (49 is clamped from below) */
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0) && temp.wCount_1 == 113);
    temp.wCount_1 = 49;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    /* at 48: waits for the module */
    isload_answer = 0;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0) && temp.wLvl == 0);
    /* loaded: popup, effects from the flag list with the untouched 999 call record */
    isload_answer = 1;
    temp.wCount_1 = 0x130;
    temp.wCount_1 = 40;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 5 && is(2, POPUP, 0, -120, 80, 263) && is(3, DEPEND, 557, 0, 0, 0));
    assert(log_p[3] == &temp && depend_call.eNO == 999 && depend_call.dwFrom == 0 && depend_call.dwTo == 0 && depend_call.dwPar == 0);
    assert(is(4, BLACK, 48, 0, 0, 0) && temp.wLvl == 1 && temp.wCount_1 == 48);
    /* level 1: waits while any of the oNum effects is in use */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9;
    isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 5, 0, 0, 0) && is(2, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    /* all over: DP and AP of every flagged unit changed by the low half of dwPar */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 14 && log_id[3] == CLOSE);
    assert(is(4, DP, 0, 0, 1, 0x1F4) && is(5, AP, 0, 0, 1, 0x1F4) && is(6, DP, 0, 10, 1, 0x1F4) && is(7, AP, 0, 10, 1, 0x1F4));
    assert(is(8, DP, 1, 0, 1, 0x1F4) && is(9, AP, 1, 0, 1, 0x1F4) && is(10, DP, 1, 10, 1, 0x1F4) && is(11, AP, 1, 10, 1, 0x1F4));
    assert(is(12, FADE, -1, 1, 0, 1) && is(13, RESTORE, 16, 0, 0, 0) && log_p[13] == &temp && temp.wLvl == 2);
    /* later levels: the level rises by 15 to 128 */
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120;
    temp.wCamTime = 16;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 17);
    /* at 128 with the counter above 16: waits for the fade */
    finfade_answer = 0;
    assert(step(Ta_Eff_057, &in, -1) == 1 && log_n == 3 && log_id[2] == FINFADE && temp.wCamTime == 18);
    finfade_answer = 1, disp_answer = 0;
    assert(step(Ta_Eff_057, &in, -1) == 0 && log_n == 5 && log_id[3] == WDISP && log_id[4] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_057, &in, -1) == 0 && log_n == 6 && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
}

typedef int (*seq_fn)(struct jp_6989c *, struct jp_69942 *, int);

/* The ID >= 0 entry shared by 0x25d230, 0x25d640 and 0x25de70: one target gets the camera,
 * several get the standard view. */
static void check_entry(seq_fn f, struct jp_6989c *in, int module, int who)
{
    memset(&temp, 0, sizeof(temp));
    num_answer = 1, owner_answer = 0x10a;
    assert(step(f, in, 3) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, NUM, (int)in->dwTo, 0, 0, 0) && is(3, OWNER, (int)in->dwTo, 0, 0, 0) && is(4, FOCUS, who, 0x10a, 100, 180));
    assert(is(5, FADE, 3, -1, 1, 1) && log_p[5] == &temp && is(6, LOAD, module, 0, 0, 0) && temp.wCount_1 == 128);
    num_answer = 2;
    assert(step(f, in, 0) == 1 && log_n == 6 && is(3, STDVIEW, who, 0, 0, 0) && is(4, FADE, 0, -1, 1, 1));
}

/* The last level shared by the same three: the black-out level rises by 15 to 128, then the
 * +0x102 counter has to pass 16 and the fade has to finish. */
static void check_leave(seq_fn f, struct jp_6989c *in)
{
    temp.wLvl = 4, temp.wCount_1 = 100, temp.wCamTime = 0;
    assert(step(f, in, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120;
    temp.wCamTime = 16;
    assert(step(f, in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 17);
    finfade_answer = 0;
    assert(step(f, in, -1) == 1 && log_n == 3 && log_id[2] == FINFADE && temp.wCamTime == 18);
    finfade_answer = 1, disp_answer = 0;
    assert(step(f, in, -1) == 0 && log_n == 5 && log_id[3] == WDISP && log_id[4] == CLOSE);
    disp_answer = 1;
    assert(step(f, in, -1) == 0 && log_n == 6 && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
}

/* Level 0 of 0x25d230 and 0x25d640 up to the loaded module: falls by 15 to 48, clamps, waits. */
static void check_029_fall(seq_fn f, struct jp_6989c *in)
{
    assert(step(f, in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0) && temp.wCount_1 == 113);
    temp.wCount_1 = 49;
    assert(step(f, in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    isload_answer = 0;
    assert(step(f, in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0) && temp.wLvl == 0);
    isload_answer = 1;
}

/* Ta_Eff_029 (branch coverage). */
static void check_029(void)
{
    /* targets: own slots 0 and 10, other side's slots 0 and 10 (bits 21, 11, 10, 0) */
    struct jp_6989c in = {29, 1, 0x200C01, 0x201F4};

    check_entry(Ta_Eff_029, &in, 529, 16);
    check_029_fall(Ta_Eff_029, &in);
    assert(step(Ta_Eff_029, &in, -1) == 1 && log_n == 5 && is(2, POPUP, 0, -120, 80, 262) && is(3, DEPEND, 529, 0, 0, 0));
    assert(log_p[3] == &temp && depend_call.eNO == 999 && depend_call.dwFrom == 0 && depend_call.dwTo == 0 && depend_call.dwPar == 0);
    assert(is(4, BLACK, 48, 0, 0, 0) && temp.wLvl == 1 && temp.wCount_1 == 48);
    /* level 1: waits while any of the oNum effects is in use */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9;
    isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_029, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 5, 0, 0, 0) && is(2, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    /* all over, dwFrom == 1: DP and AP of every flagged unit, operation and amount from dwPar */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_029, &in, -1) == 1 && log_n == 14 && log_id[3] == CLOSE);
    assert(is(4, DP, 0, 0, 2, 0x1F4) && is(5, AP, 0, 0, 2, 0x1F4) && is(6, DP, 0, 10, 2, 0x1F4) && is(7, AP, 0, 10, 2, 0x1F4));
    assert(is(8, DP, 1, 0, 2, 0x1F4) && is(9, AP, 1, 0, 2, 0x1F4) && is(10, DP, 1, 10, 2, 0x1F4) && is(11, AP, 1, 10, 2, 0x1F4));
    assert(is(12, FADE, -1, 1, 0, 1) && is(13, RESTORE, 16, 0, 0, 0) && log_p[13] == &temp && temp.wLvl == 2);
    /* dwFrom != 1: DP only */
    temp.wLvl = 1, in.dwFrom = 0;
    assert(step(Ta_Eff_029, &in, -1) == 1 && log_n == 10 && is(4, DP, 0, 0, 2, 0x1F4) && is(5, DP, 0, 10, 2, 0x1F4));
    assert(is(6, DP, 1, 0, 2, 0x1F4) && is(7, DP, 1, 10, 2, 0x1F4) && log_id[8] == FADE && temp.wLvl == 2);
    check_leave(Ta_Eff_029, &in);
}

/* Ta_Eff_029_CNG (branch coverage). */
static void check_029_cng(void)
{
    struct jp_6989c in = {29, 0, 0x200C01, 0};

    check_entry(Ta_Eff_029_CNG, &in, 529, 16);
    check_029_fall(Ta_Eff_029_CNG, &in);
    /* loaded: the popup shows message name 736 */
    assert(step(Ta_Eff_029_CNG, &in, -1) == 1 && log_n == 6 && is(2, MSG, 736, 0, 0, 0) && is(3, POPUP, 0, -120, 80, 0x4D5347));
    assert(is(4, DEPEND, 529, 0, 0, 0) && depend_call.eNO == 999 && is(5, BLACK, 48, 0, 0, 0) && temp.wLvl == 1);
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9;
    isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_029_CNG, &in, -1) == 1 && log_n == 3 && temp.wLvl == 1);
    /* all over: each flagged unit's AP gains DP - AP and its DP gains AP - DP (operation 0) */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_029_CNG, &in, -1) == 1 && log_n == 30 && log_id[3] == CLOSE);
    assert(is(4, UDP, 0, 0, 0, 0) && is(5, UAP, 0, 0, 0, 0) && is(6, UAP, 0, 0, 0, 0) && is(7, UDP, 0, 0, 0, 0));
    assert(is(8, AP, 0, 0, 0, 200) && is(9, DP, 0, 0, 0, -200) && is(14, AP, 0, 10, 0, 200) && is(15, DP, 0, 10, 0, -200));
    assert(is(16, UDP, 1, 0, 0, 0) && is(20, AP, 1, 0, 0, 290) && is(21, DP, 1, 0, 0, -290));
    assert(is(22, UDP, 1, 10, 0, 0) && is(23, UAP, 1, 10, 0, 0) && is(26, AP, 1, 10, 0, 290) && is(27, DP, 1, 10, 0, -290));
    assert(is(28, FADE, -1, 1, 0, 1) && is(29, RESTORE, 16, 0, 0, 0) && temp.wLvl == 2);
    check_leave(Ta_Eff_029_CNG, &in);
}

/* Ta_Eff_030 (branch coverage). */
static void check_030(void)
{
    struct jp_6989c in = {30, 0, 0x200C01, 0};

    check_entry(Ta_Eff_030, &in, 30, 16);
    /* level 0: falls by 15 while above 63 (no clamp from below), then 48 */
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0));
    temp.wCount_1 = 64;
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 49, 0, 0, 0) && temp.wCount_1 == 49);
    temp.wCount_1 = 63;
    isload_answer = 0;
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0) && temp.wCamTime == 0);
    /* loaded: the +0x102 counter has to pass 16 first */
    isload_answer = 1, temp.wCamTime = 16;
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 3 && temp.wCamTime == 17 && temp.wLvl == 0);
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 4 && is(2, DEPEND, 30, 0, 0, 0) && depend_call.eNO == 999);
    assert(is(3, BLACK, 48, 0, 0, 0) && temp.wCamTime == 0 && temp.wLvl == 1);
    /* level 1: waits for the effects, then every flagged unit's card goes back to its deck */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9;
    isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 3 && temp.wLvl == 1);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 19 && temp.wLvl == 2);
    assert(is(3, CARDNO, 0, 0, 0, 0) && is(4, ADDDECK, 0, 700, 0, 0) && is(5, PACK, 0, 0, 0, 0) && is(6, DELUNIT, 0, 0, 0, 0));
    assert(is(7, CARDNO, 0, 10, 0, 0) && is(8, ADDDECK, 0, 710, 0, 0) && is(10, DELUNIT, 0, 10, 0, 0));
    assert(is(11, CARDNO, 1, 0, 0, 0) && is(12, ADDDECK, 1, 720, 0, 0) && is(13, PACK, 0, 0, 0, 0) && is(14, DELUNIT, 1, 0, 0, 0));
    assert(is(15, CARDNO, 1, 10, 0, 0) && is(16, ADDDECK, 1, 730, 0, 0) && is(17, PACK, 0, 0, 0, 0) && is(18, DELUNIT, 1, 10, 0, 0));
    /* level 2: the counter has to pass 16, then the view and the fade */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17 && temp.wLvl == 2);
    assert(step(Ta_Eff_030, &in, -1) == 1 && log_n == 3 && is(1, RESTORE, 16, 0, 0, 0) && is(2, FADE, -1, 1, 0, 1));
    assert(temp.wCamTime == 0 && temp.wLvl == 3);
    check_leave(Ta_Eff_030, &in);
}

/* Ta_Eff_017_MEGUMI (branch coverage). */
static void check_017_megumi(void)
{
    struct jp_6989c in = {17, 1, 0x200C01, 0x201F4};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_017_MEGUMI, &in, 3) == 1 && log_n == 6 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT);
    assert(is(2, STDVIEW, 16, 0, 0, 0) && is(3, LOAD, 20, 0, 0, 0) && is(4, LOAD, 529, 0, 0, 0) && is(5, FADE, 3, -1, 1, 1));
    assert(log_p[5] == &temp && temp.wCount_1 == 128);
    /* level 0: falls by 15 while above 63, then 48; waits for the modules and the counter */
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0));
    temp.wCount_1 = 64;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 49, 0, 0, 0));
    temp.wCount_1 = 63;
    isload_answer = 0;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 3 && is(2, BLACK, 48, 0, 0, 0) && temp.wCamTime == 0);
    isload_answer = 1, temp.wCamTime = 16;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 3 && temp.wCamTime == 17 && temp.wLvl == 0);
    /* then the popup and effect 20, whose handle is stored and counted */
    do_answer = 7;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 5 && is(2, POPUP, 0, -120, 80, 262) && log_id[3] == DOEFF);
    assert(log_p[3] == &temp && do_call.eNO == 20 && do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0);
    assert(is(4, BLACK, 48, 0, 0, 0) && temp.eID[0] == 7 && temp.oNum == 1 && temp.wLvl == 1 && temp.wCamTime == 0);
    /* level 1: waits for that effect, then the effects from the flag list */
    isuse_answer[7] = 1;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[7] = 0;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 3 && is(2, DEPEND, 529, 0, 0, 0) && depend_call.eNO == 999);
    assert(temp.wLvl == 2);
    /* level 2: waits for all effects, then the point changes of Ta_Eff_029 */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9;
    isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 3 && temp.wLvl == 2);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 12 && log_id[3] == CLOSE && temp.wLvl == 3);
    assert(is(4, DP, 0, 0, 2, 0x1F4) && is(5, AP, 0, 0, 2, 0x1F4) && is(6, DP, 0, 10, 2, 0x1F4) && is(7, AP, 0, 10, 2, 0x1F4));
    assert(is(8, DP, 1, 0, 2, 0x1F4) && is(9, AP, 1, 0, 2, 0x1F4) && is(10, DP, 1, 10, 2, 0x1F4) && is(11, AP, 1, 10, 2, 0x1F4));
    temp.wLvl = 2, in.dwFrom = 0;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 8 && is(4, DP, 0, 0, 2, 0x1F4) && is(5, DP, 0, 10, 2, 0x1F4));
    assert(is(6, DP, 1, 0, 2, 0x1F4) && is(7, DP, 1, 10, 2, 0x1F4) && temp.wLvl == 3);
    /* level 3: the counter has to pass 16, then the fade and the view */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17 && temp.wLvl == 3);
    assert(step(Ta_Eff_017_MEGUMI, &in, -1) == 1 && log_n == 3 && is(1, FADE, -1, 1, 0, 1) && is(2, RESTORE, 16, 0, 0, 0));
    assert(temp.wCamTime == 0 && temp.wLvl == 4);
    check_leave(Ta_Eff_017_MEGUMI, &in);
}

/* Ta_Eff_066 (branch coverage). */
static void check_066(void)
{
    struct jp_6989c in = {66, 54, 0x200C01, 5};

    check_entry(Ta_Eff_066, &in, 513, 30);
    /* level 0: the +0x102 counter runs every step; the level falls by 15 to 48 */
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0) && temp.wCamTime == 1);
    temp.wCount_1 = 49;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    isload_answer = 0;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0));
    /* loaded but the counter below 30 */
    isload_answer = 1, temp.wCamTime = 28;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 3 && temp.wCamTime == 29 && temp.wLvl == 0);
    /* at 30: popup 272 for dwFrom 54, 273 otherwise */
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 5 && is(2, POPUP, 0, -120, 80, 272) && is(3, DEPEND, 513, 0, 0, 0));
    assert(depend_call.eNO == 999 && is(4, BLACK, 48, 0, 0, 0) && temp.wLvl == 1 && temp.wCamTime == 0);
    temp.wLvl = 0, temp.wCamTime = 29, in.dwFrom = 55;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 5 && is(2, POPUP, 0, -120, 80, 273) && temp.wLvl == 1);
    /* level 1: an effect in use with status 2 and a live position sets the unit's kind (54) */
    in.dwFrom = 54;
    temp.oNum = 3, temp.eID[0] = 5, temp.eID[1] = 9, temp.eID[2] = 11;
    temp.ePos[0] = 0x105, temp.ePos[1] = 0x207, temp.ePos[2] = 0xFFFF;
    isuse_answer[5] = isuse_answer[9] = isuse_answer[11] = 1;
    status_answer[5] = 2, status_answer[9] = 1, status_answer[11] = 2;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 9 && is(1, ISUSE, 5, 0, 0, 0) && is(2, STATUS, 5, 0, 0, 0));
    assert(is(3, STATUS, 5, 0, 0, 0) && is(4, KIND, 1, 5, 5, 0) && is(5, ISUSE, 9, 0, 0, 0) && is(6, STATUS, 9, 0, 0, 0));
    assert(is(7, ISUSE, 11, 0, 0, 0) && is(8, STATUS, 11, 0, 0, 0) && temp.ePos[0] == 0xFFFF && temp.ePos[1] == 0x207);
    assert(temp.wLvl == 1);
    /* or its attribute (55), or nothing (others); the position is retired each time */
    in.dwFrom = 55, temp.ePos[0] = 0xA03;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 9 && is(4, ATTR, 10, 3, 5, 0) && temp.ePos[0] == 0xFFFF);
    in.dwFrom = 56, temp.ePos[0] = 0x105;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 8 && is(4, ISUSE, 9, 0, 0, 0) && temp.ePos[0] == 0xFFFF);
    /* the status is read twice: a change between the reads leaves the position alone */
    in.dwFrom = 54, temp.ePos[0] = 0x105, status_once = 1;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 8 && is(3, STATUS, 5, 0, 0, 0) && is(4, ISUSE, 9, 0, 0, 0));
    assert(temp.ePos[0] == 0x105);
    status_once = 0;
    /* one effect over is not enough */
    isuse_answer[5] = 0;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 6 && temp.wLvl == 1);
    /* all over: popup closed, view restored, fade */
    isuse_answer[9] = isuse_answer[11] = 0;
    assert(step(Ta_Eff_066, &in, -1) == 1 && log_n == 7 && log_id[4] == CLOSE && is(5, RESTORE, 16, 0, 0, 0));
    assert(is(6, FADE, -1, 1, 0, 1) && temp.wLvl == 2);
    check_leave(Ta_Eff_066, &in);
}

/* Ta_Eff_006_WIND (branch coverage). */
static void check_006_wind(void)
{
    struct jp_6989c in = {6, 1, 0x200C01, 0x201F4};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_006_WIND, &in, 3) == 1 && log_n == 5 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT);
    assert(is(2, STDVIEW, 30, 0, 0, 0) && is(3, LOAD, 6, 0, 0, 0) && is(4, LOAD, 529, 0, 0, 0) && temp.wCount_1 == 128);
    /* level 0: falls by 15 to 48, then starts effect 6 once the modules are loaded */
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0));
    temp.wCount_1 = 49;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    isload_answer = 0;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 3 && is(2, BLACK, 48, 0, 0, 0) && temp.wLvl == 0);
    isload_answer = 1, do_answer = 7;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 4 && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 6 && do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0);
    assert(is(3, BLACK, 48, 0, 0, 0) && temp.eID[0] == 7 && temp.oNum == 1 && temp.wLvl == 1);
    /* level 1: waits for it, then popup 262 and the flag-list effects */
    isuse_answer[7] = 1;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[7] = 0;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 262) && is(3, DEPEND, 529, 0, 0, 0));
    assert(depend_call.eNO == 999 && temp.wLvl == 2);
    /* level 2: waits for all effects, then the point changes, the view and the window */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9;
    isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 3 && temp.wLvl == 2);
    isuse_answer[9] = 0, disp_answer = 0;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 14 && log_id[3] == CLOSE && temp.wLvl == 3);
    assert(is(4, DP, 0, 0, 2, 0x1F4) && is(5, AP, 0, 0, 2, 0x1F4) && is(6, DP, 0, 10, 2, 0x1F4) && is(7, AP, 0, 10, 2, 0x1F4));
    assert(is(8, DP, 1, 0, 2, 0x1F4) && is(9, AP, 1, 0, 2, 0x1F4) && is(10, DP, 1, 10, 2, 0x1F4) && is(11, AP, 1, 10, 2, 0x1F4));
    assert(is(12, RESTORE, 16, 0, 0, 0) && log_id[13] == WDISP);
    temp.wLvl = 2, in.dwFrom = 0, disp_answer = 1;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 11 && is(4, DP, 0, 0, 2, 0x1F4) && is(7, DP, 1, 10, 2, 0x1F4));
    assert(is(8, RESTORE, 16, 0, 0, 0) && log_id[9] == WDISP && is(10, WDRAW, 1, 0, 0, 0) && temp.wLvl == 3);
    /* later levels: rises by 15 to 128; the +0x102 counter has to reach 16, then the fade */
    temp.wCount_1 = 100, temp.wCamTime = 0;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120, temp.wCamTime = 15;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 16);
    finfade_answer = 0;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 1 && log_n == 3 && log_id[2] == FINFADE && temp.wCamTime == 17);
    finfade_answer = 1;
    assert(step(Ta_Eff_006_WIND, &in, -1) == 0 && log_n == 4 && log_id[3] == CLOSE);
}

/* Ta_Eff_006 (branch coverage). */
static void check_006(void)
{
    struct jp_6989c in = {6, 1, 0x200C01, 0x201F4};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_006, &in, 0) == 1 && log_n == 3 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && is(2, LOAD, 6, 0, 0, 0));
    /* level 0 waits for the module; camera level 0 asks for the standard view */
    isload_answer = 0;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, STDVIEW, 30, 0, 0, 0));
    assert(temp.wLvl == 0 && temp.bCamLvl == 1);
    isload_answer = 1, do_answer = 7;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 6 && do_call.dwFrom == 0 && do_call.dwTo == 0x200C01 && do_call.dwPar == 0);
    assert(temp.eID[0] == 7 && temp.wLvl == 1 && temp.bCamLvl == 1 && temp.wCamTime == 1);
    /* level 1 waits for the effect; camera level 1 counts to 30 */
    isuse_answer[7] = 1, temp.wCamTime = 29;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.bCamLvl == 1 && temp.wCamTime == 30);
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 2 && temp.wLvl == 1 && temp.bCamLvl == 2 && temp.wCamTime == 0);
    /* camera level 2 before the end: rotation on the first frame, the counter wraps at 60 */
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 3 && is(2, ROT, 60, 16383, 0, 0) && temp.wCamTime == 1);
    temp.wCamTime = 58;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 59);
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 0 && temp.bCamLvl == 2);
    /* the effect ended: one Ta_DelUnit per set bit, sides split at 11 */
    isuse_answer[7] = 0, temp.wCamTime = 5;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 6 && is(2, TDEL, 0, 0, 0, 0) && is(3, TDEL, 0, 10, 0, 0));
    assert(is(4, TDEL, 1, 0, 0, 0) && is(5, TDEL, 1, 10, 0, 0) && temp.wLvl == 2 && temp.wCamTime == 6);
    /* later levels set bit 0: the view is restored, then the counter has to pass 16 */
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 2 && is(1, RESTORE, 16, 0, 0, 0) && log_p[1] == &temp);
    assert(temp.wCamTime == 0 && temp.bCamLvl == 3);
    temp.wCamTime = 15;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 16);
    temp.wLvl = 1, isuse_answer[7] = 1;
    assert(step(Ta_Eff_006, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 17);
    temp.wLvl = 2, disp_answer = 0;
    assert(step(Ta_Eff_006, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE && temp.wCamTime == 18);
    disp_answer = 1;
    assert(step(Ta_Eff_006, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    isuse_answer[7] = 0;
}

/* Ta_Eff_003B (branch coverage). */
static void check_003b(void)
{
    struct jp_6989c in = {3, 0x104, 0x20A, 0};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_003B, &in, 3) == 1 && log_n == 5 && log_id[0] == INIT && is(1, FADE2, 3, -1, 1, 3) && log_p[1] == &temp);
    assert(is(2, STDVIEW, 30, 0, 0, 0) && is(3, LOAD, 503, 0, 0, 0) && is(4, LOAD, 502, 0, 0, 0));
    /* level 0: the counter has to pass 30, then the modules, then effects 503 and 502 */
    temp.wCamTime = 30;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 0 && temp.wCamTime == 31);
    isload_answer = 0;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 1 && log_id[0] == ISLOAD && temp.wCamTime == 32 && temp.wLvl == 0);
    isload_answer = 1, do_answer = 7;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 3 && log_id[1] == DOEFF && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_prev.eNO == 503 && do_prev.dwFrom == 1 && do_prev.dwTo == 0x20A && do_prev.dwPar == 8);
    assert(do_call.eNO == 502 && do_call.dwFrom == 0 && do_call.dwTo == 0x104 && do_call.dwPar == 7);
    assert(temp.eID[0] == 7 && temp.eID[1] == 7 && temp.ePos[0] == 0x20A && temp.ePos[1] == 0x104);
    assert(temp.oNum == 2 && temp.wLvl == 1);
    /* level 1: a running effect in status 2 has its position marked 0xFFFF once */
    temp.oNum = 4, temp.eID[0] = 5, temp.eID[1] = 9, temp.eID[2] = 11, temp.eID[3] = 12;
    temp.ePos[1] = 0x105, temp.ePos[2] = 0x203, temp.ePos[3] = 0x1FFFF;
    status_once = 0;
    isuse_answer[5] = 0, isuse_answer[9] = 1, isuse_answer[11] = 1, isuse_answer[12] = 1;
    status_answer[9] = 2, status_answer[11] = 1, status_answer[12] = 2;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 7 && is(0, ISUSE, 5, 0, 0, 0) && is(1, ISUSE, 9, 0, 0, 0));
    assert(is(2, STATUS, 9, 0, 0, 0) && is(4, STATUS, 11, 0, 0, 0) && is(6, STATUS, 12, 0, 0, 0));
    assert(temp.ePos[1] == 0xFFFF && temp.ePos[2] == 0x203 && temp.ePos[3] == 0x1FFFF && temp.wLvl == 1);
    isuse_answer[9] = 0, isuse_answer[11] = 0, isuse_answer[12] = 0, temp.wCamTime = 9;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 6 && is(4, RESTORE, 30, 0, 0, 0) && is(5, FADE2, -1, 1, 1, 3));
    assert(log_p[5] == &temp && temp.wLvl == 2 && temp.wCamTime == 0);
    /* later levels: the counter has to pass 30, then the fade */
    temp.wCamTime = 30;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 0 && temp.wCamTime == 31);
    finfade_answer = 0;
    assert(step(Ta_Eff_003B, &in, -1) == 1 && log_n == 1 && log_id[0] == FINFADE);
    finfade_answer = 1;
    assert(step(Ta_Eff_003B, &in, -1) == 0 && log_n == 1 && temp.wCamTime == 33);
    status_answer[9] = 0, status_answer[11] = 0, status_answer[12] = 0;
}

/* Ta_Eff_012 (branch coverage). */
static void check_012(void)
{
    struct jp_6989c in = {12, 0, 0x200C01, 0x203};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_012, &in, 2) == 1 && log_n == 4 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT);
    assert(log_id[2] == TOPVIEW && is(3, LOAD, 512, 0, 0, 0));
    /* level 0 waits for the module, then popup 264 and the flag-list effects */
    isload_answer = 0;
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0 && temp.wCamTime == 1);
    isload_answer = 1, temp.wCamTime = 10;
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 264) && is(3, DEPEND, 512, 0, 0, 0));
    assert(depend_call.eNO == 999 && temp.wLvl == 1 && temp.wCamTime == 11 && temp.bCamLvl == 0);
    /* later levels: a running effect in status 2 opens its unit once; camera level 0 passes 10 */
    temp.oNum = 4, temp.eID[0] = 5, temp.eID[1] = 9, temp.eID[2] = 11, temp.eID[3] = 12;
    temp.ePos[1] = 0x105, temp.ePos[2] = 0x203, temp.ePos[3] = -1;
    status_once = 0;
    isuse_answer[5] = 0, isuse_answer[9] = 1, isuse_answer[11] = 1, isuse_answer[12] = 1;
    status_answer[9] = 2, status_answer[11] = 1, status_answer[12] = 2;
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 13 && is(1, ISUSE, 5, 0, 0, 0) && is(3, STATUS, 9, 0, 0, 0));
    assert(is(4, DOOPEN, 1, 5, 1, 0) && is(5, USTAT, 1, 5, 0, 1) && is(6, CURSE, 1, 5, 3, 0) && is(8, STATUS, 11, 0, 0, 0));
    assert(is(10, STATUS, 12, 0, 0, 0) && log_id[11] == CLOSE && is(12, STDVIEW, 16, 0, 0, 0));
    assert(temp.ePos[1] == -1 && temp.ePos[2] == 0x203 && temp.bCamLvl == 1 && temp.wCamTime == 0);
    /* camera level 1 waits for bit 0, then passes 16 and restores the view */
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 9 && temp.wCamTime == 0 && temp.bCamLvl == 1);
    isuse_answer[9] = 0, isuse_answer[11] = 0, isuse_answer[12] = 0, temp.wCamTime = 16;
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 6 && log_id[5] == CLOSE && temp.wCamTime == 17 && temp.bCamLvl == 1);
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 7 && is(6, RESTORE, 16, 0, 0, 0) && log_p[6] == &temp);
    assert(temp.wCamTime == 0 && temp.bCamLvl == 2);
    /* later camera levels pass 16, restore the window and set bit 1 */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 6 && temp.wCamTime == 17);
    isuse_answer[5] = 1, disp_answer = 0;
    assert(step(Ta_Eff_012, &in, -1) == 1 && log_n == 8 && log_id[7] == WDISP);
    isuse_answer[5] = 0;
    assert(step(Ta_Eff_012, &in, -1) == 0 && log_n == 8 && log_id[6] == WDISP && log_id[7] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_012, &in, -1) == 0 && log_n == 9 && is(7, WDRAW, 1, 0, 0, 0) && log_id[8] == CLOSE);
    status_answer[9] = 0, status_answer[11] = 0, status_answer[12] = 0;
}

/* Ta_Eff_102 (branch coverage). */
static void check_102(void)
{
    struct jp_6989c in = {102, 1, 0x305, 7};

    memset(&temp, 0, sizeof(temp));
    /* ID >= 0: open the unit, focus on it, load module 602 */
    assert(step(Ta_Eff_102, &in, 0) == 1 && log_n == 4);
    assert(is(0, INIT, 0, 0, 0, 0) && log_p[0] == &temp && is(1, DOOPEN, 3, 5, 1, 0));
    assert(is(2, FOCUS, 0, 0x305, 180, 180) && is(3, LOAD, 602, 0, 0, 0) && log_p[3] == &temp);
    /* level 0: waits for the open rate to reach 255, then for the module */
    openrate_answer = 254, isload_answer = 1;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 1 && is(0, OPENRATE, 3, 5, 0, 0) && temp.wLvl == 0);
    openrate_answer = 255, isload_answer = 0;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 2 && is(1, ISLOAD, 0, 0, 0, 0) && temp.wLvl == 0);
    /* dwFrom == 1: popup with the unit's card name, then effect 602 on dwTo */
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 6);
    assert(is(2, CARDNO, 3, 5, 0, 0) && is(3, MSG, 700 + 3 * 20 + 5, 0, 0, 0) && is(4, POPUP, 0, -120, 80, 0x4D5347));
    assert(is(5, DOEFF, 0, 0, 0, 0) && log_p[5] == &temp);
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x305 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.wLvl == 1);
    /* dwFrom != 1: fixed message 269 */
    temp.wLvl = 0, in.dwFrom = 2;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 269) && is(3, DOEFF, 0, 0, 0, 0));
    assert(temp.wLvl == 1);
    /* level 1: waits while the effect is in use */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 1 && is(0, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    /* finished with dwPar != 0: the unit is deleted */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 5 && is(1, USTAT, 3, 5, 0, 1) && is(2, CLOSE, 0, 0, 0, 0));
    assert(is(3, RESTORE, 16, 0, 0, 0) && log_p[3] == &temp && is(4, TDEL, 3, 5, 0, 0) && temp.wLvl == 2);
    /* finished with dwPar == 0: busy flag instead */
    temp.wLvl = 1, in.dwPar = 0;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 5 && is(4, BUSY2, 0x305, 0, 0, 0) && temp.wLvl == 2);
    /* level 2: counts wCamTime past 16, then closes the popup and ends */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_102, &in, -1) == 1 && log_n == 0 && temp.wCamTime == 17);
    assert(step(Ta_Eff_102, &in, -1) == 0 && log_n == 1 && is(0, CLOSE, 0, 0, 0, 0) && temp.wCamTime == 18);
}

/* Ta_Eff_063 (branch coverage). */
static void check_063(void)
{
    struct jp_6989c in = {63, 0, 0x200C01, 0x2A};

    memset(&temp, 0, sizeof(temp));
    /* ID >= 0: module 560; one target gets the camera, several the standard view */
    num_answer = 1, owner_answer = 0x10a;
    assert(step(Ta_Eff_063, &in, 3) == 1 && log_n == 6 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, LOAD, 560, 0, 0, 0) && is(3, NUM, 0x200C01, 0, 0, 0) && is(4, OWNER, 0x200C01, 0, 0, 0));
    assert(is(5, FOCUS, 30, 0x10a, 100, 180));
    num_answer = 2;
    assert(step(Ta_Eff_063, &in, 0) == 1 && log_n == 5 && is(4, STDVIEW, 30, 0, 0, 0));
    /* level 0 waits for the module and for wCamTime to reach 30, then the flag-list effects */
    isload_answer = 0;
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wCamTime == 0);
    isload_answer = 1, temp.wCamTime = 29;
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 30 && temp.wLvl == 0);
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 3 && is(2, DEPEND, 560, 0, 0, 0) && log_p[2] == &temp);
    assert(depend_call.eNO == 999 && temp.wCamTime == 31 && temp.wLvl == 1);
    /* level 1: a running effect in status 2 moves its unit once (deleted, added back at its location) */
    temp.oNum = 4, temp.eID[0] = 5, temp.eID[1] = 9, temp.eID[2] = 11, temp.eID[3] = 12;
    temp.ePos[1] = 0x105, temp.ePos[2] = 0x203, temp.ePos[3] = 0xFFFF;
    status_once = 0, loc_answer = 0x604, addunit_e = -1;
    isuse_answer[5] = 0, isuse_answer[9] = 1, isuse_answer[11] = 1, isuse_answer[12] = 1;
    status_answer[9] = 2, status_answer[11] = 1, status_answer[12] = 2;
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 11 && is(1, ISUSE, 5, 0, 0, 0) && is(3, STATUS, 9, 0, 0, 0));
    assert(is(4, LOC, 1, 5, 0, 0) && is(5, TDEL, 1, 5, 0, 0) && is(6, ADDUNIT, 1, 4, 6, 0x2A) && addunit_e == 0);
    assert(is(8, STATUS, 11, 0, 0, 0) && is(10, STATUS, 12, 0, 0, 0));
    assert(temp.ePos[1] == 0xFFFF && temp.ePos[2] == 0x203 && temp.wLvl == 1);
    /* one effect still in use: stays; none in use: next level, counter cleared, view restored */
    isuse_answer[9] = 0, isuse_answer[11] = 0, temp.wCamTime = 5;
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 6 && log_id[5] == STATUS && temp.wLvl == 1 && temp.wCamTime == 5);
    isuse_answer[12] = 0;
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 6 && is(5, RESTORE, 16, 0, 0, 0) && log_p[5] == &temp);
    assert(temp.wLvl == 2 && temp.wCamTime == 0);
    /* later levels pass 16, restore the window, close the popup and end */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_063, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_063, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_063, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    status_answer[9] = 0, status_answer[11] = 0, status_answer[12] = 0;
}

/* Ta_Eff_056 (branch coverage). */
static void check_056(void)
{
    struct jp_6989c in = {56, 0x200C01, 0x305, 0x2A};

    memset(&temp, 0, sizeof(temp));
    /* ID >= 0: every listed unit is opened, module 59, standard view */
    num_answer = 2, owner_answer = 0x10a;
    assert(step(Ta_Eff_056, &in, 0) == 1 && log_n == 11 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, NUM, 0x200C01, 0, 0, 0) && is(3, OWNER, 0x200C01, 0, 0, 0) && is(4, DOOPEN, 1, 10, 1, 0));
    assert(is(6, OWNER, 0x200C01, 1, 0, 0) && is(7, DOOPEN, 1, 10, 1, 0) && log_id[8] == NUM);
    assert(is(9, LOAD, 59, 0, 0, 0) && log_p[9] == &temp && is(10, STDVIEW, 16, 0, 0, 0));
    /* level 0: fewer than three listed units at open rate 255: waits without asking for the module */
    num_answer = 3, openrate_answer = 254, isload_answer = 1;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 11 && is(2, OWNER, 0x200C01, 0, 0, 0) && is(3, OPENRATE, 1, 10, 0, 0));
    assert(is(8, OWNER, 0x200C01, 2, 0, 0) && log_id[10] == NUM && temp.wLvl == 0);
    num_answer = 2, openrate_answer = 255;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 8 && log_id[7] == NUM && temp.wLvl == 0);
    /* three open units: waits for the module, then level 1, popup 261 and effect 59 with eInfo's words */
    num_answer = 3, isload_answer = 0;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 12 && log_id[11] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 14 && is(12, POPUP, 0, -120, 80, 261) && log_id[13] == DOEFF);
    assert(log_p[13] == &temp && do_call.eNO == 59 && do_call.dwFrom == 0x200C01 && do_call.dwTo == 0x305);
    assert(do_call.dwPar == 0x2A && temp.eID[0] == 9 && temp.wLvl == 1);
    /* level 1: while the effect is in use, status 2 deletes every listed unit */
    status_once = 0, isuse_answer[9] = 1, status_answer[9] = 1;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 9, 0, 0, 0) && is(2, STATUS, 9, 0, 0, 0));
    status_answer[9] = 2, num_answer = 2;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 10 && is(4, OWNER, 0x200C01, 0, 0, 0) && is(5, TDEL, 1, 10, 0, 0));
    assert(is(7, OWNER, 0x200C01, 1, 0, 0) && is(8, TDEL, 1, 10, 0, 0) && log_id[9] == NUM && temp.wLvl == 1);
    /* effect over: popup closed, view restored, next level */
    isuse_answer[9] = 0, status_answer[9] = 0;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 4 && log_id[2] == CLOSE && is(3, RESTORE, 16, 0, 0, 0));
    assert(log_p[3] == &temp && temp.wLvl == 2);
    /* later levels pass 16, restore the window, close the popup and end */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_056, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_056, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_056, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
}

/* Ta_Eff_059 (branch coverage). */
static void check_059(void)
{
    /* dwTo: own slots 0 and 10, other side's slots 0 and 10 (bits 21, 11, 10, 0) */
    struct jp_6989c in = {59, 1, 0x200C01, 0x640BB8};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_059, &in, 0) == 1 && log_n == 6 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, STDVIEW, 16, 0, 0, 0) && is(3, LOAD, 508, 0, 0, 0) && is(4, LOAD, 529, 0, 0, 0) && is(5, STDVIEW, 16, 0, 0, 0));
    /* level 0 waits for the module, then effect 508 with the revised LP value */
    isload_answer = 0;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 4 && is(2, LPREV, 44, 0xBB8, 0, 0) && log_id[3] == DOEFF);
    assert(log_p[3] == &temp && do_call.eNO == 508 && do_call.dwFrom == 0 && do_call.dwTo == 1 && do_call.dwPar == 0x1234);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0xFFFF && temp.oNum == 1 && temp.wLvl == 1);
    /* level 1 waits for that effect, then changes the LP of dwFrom */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 4 && is(2, LPREV, 44, 0xBB8, 0, 0) && is(3, CALCLP, 1, 1, 0x1234, 0));
    assert(temp.wLvl == 2);
    /* level 2: popup 262 and the flag-list effects */
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 3 && is(1, POPUP, 0, -120, 80, 262) && is(2, DEPEND, 529, 0, 0, 0));
    assert(log_p[2] == &temp && depend_call.eNO == 999 && temp.wLvl == 3);
    /* level 3 waits until no effect is in use, then DP and AP of every flagged unit */
    temp.oNum = 2, temp.eID[1] = 11, isuse_answer[11] = 1;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 3 && is(2, ISUSE, 11, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[11] = 0;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 13 && log_id[3] == CLOSE && is(4, DP, 0, 0, 0, 0x64));
    assert(is(5, AP, 0, 0, 0, 0x64) && is(6, DP, 0, 10, 0, 0x64) && is(7, AP, 0, 10, 0, 0x64) && is(8, DP, 1, 0, 0, 0x64));
    assert(is(9, AP, 1, 0, 0, 0x64) && is(10, DP, 1, 10, 0, 0x64) && is(11, AP, 1, 10, 0, 0x64));
    assert(is(12, RESTORE, 16, 0, 0, 0) && log_p[12] == &temp && temp.wLvl == 4);
    /* later levels pass 16, restore the window, close the popup and end */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_059, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_059, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_059, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
}

/* Ta_Eff_015 (branch coverage). */
static void check_015(void)
{
    /* dwTo: own slots 0 and 10, other side's slots 0 and 10 (bits 21, 11, 10, 0) */
    struct jp_6989c in = {15, 0, 0x200C01, 0};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_015, &in, 0) == 1 && log_n == 4 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, TOPDOWN, 0, 0, 0, 0) && is(3, LOAD, 15, 0, 0, 0));
    /* level 0 waits for the module, then one effect 15 per flagged unit with its kind >> 5 */
    isload_answer = 0;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 10 && is(2, UKIND, 0, 0, 0, 0) && log_id[3] == DOEFF);
    assert(is(4, UKIND, 0, 10, 0, 0) && is(6, UKIND, 1, 0, 0, 0) && is(8, UKIND, 1, 10, 0, 0) && log_p[9] == &temp);
    assert(do_prev.eNO == 15 && do_prev.dwFrom == 0 && do_prev.dwTo == 0x100 && do_prev.dwPar == 12);
    assert(do_call.eNO == 15 && do_call.dwFrom == 0 && do_call.dwTo == 0x10a && do_call.dwPar == 22);
    assert(temp.oNum == 4 && temp.eID[0] == 9 && temp.eID[3] == 9 && temp.ePos[0] == 0 && temp.ePos[1] == 10);
    assert(temp.ePos[2] == 0x100 && temp.ePos[3] == 0x10a && temp.wLvl == 1 && temp.bCamLvl == 0);
    /* later levels: the first bit needs every effect out of use */
    temp.eID[3] = 11, isuse_answer[9] = 1, isuse_answer[11] = 1;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 5 && is(1, ISUSE, 9, 0, 0, 0) && is(4, ISUSE, 11, 0, 0, 0));
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 5 && temp.bCamLvl == 0);
    /* camera level 0 with that bit: view restored over 30, counter cleared */
    isuse_answer[11] = 0, temp.wCamTime = 7;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 6 && is(5, RESTORE, 30, 0, 0, 0) && log_p[5] == &temp);
    assert(temp.bCamLvl == 1 && temp.wCamTime == 0);
    /* later camera levels: the second bit once the counter has passed 30; both bits end the sequence */
    temp.wCamTime = 30;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 5 && temp.wCamTime == 31);
    isuse_answer[11] = 1;
    assert(step(Ta_Eff_015, &in, -1) == 1 && log_n == 5 && temp.wCamTime == 32);
    isuse_answer[11] = 0, disp_answer = 0;
    assert(step(Ta_Eff_015, &in, -1) == 0 && log_n == 7 && log_id[5] == WDISP && log_id[6] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_015, &in, -1) == 0 && log_n == 8 && is(6, WDRAW, 1, 0, 0, 0) && log_id[7] == CLOSE);
}

/* Ta_Eff_000 (branch coverage). */
static void check_000(void)
{
    struct jp_6989c in = {0, 1, 0x200C01, 0};

    memset(&temp, 0, sizeof(temp));
    /* start: one target focuses the camera on its owner, several ask for the complete view */
    num_answer = 1, owner_answer = 0x10a;
    assert(step(Ta_Eff_000, &in, 0) == 1 && log_n == 8 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, NUM, 0x200C01, 0, 0, 0) && is(3, FADE, 0, -1, 1, 1) && is(4, LOAD, 500, 0, 0, 0) && is(5, NUM, 0x200C01, 0, 0, 0));
    assert(is(6, OWNER, 0x200C01, 0, 0, 0) && is(7, FOCUS, 32, 0x10a, 100, 180) && temp.wCount_1 == 128);
    num_answer = 2;
    assert(step(Ta_Eff_000, &in, 0) == 1 && log_n == 7 && is(6, STDVIEW, 32, 0, 0, 0));
    /* level 0: fade down by 15 to the floor of 48 */
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0) && temp.wCount_1 == 113);
    temp.wCount_1 = 50;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    /* at the floor the counter runs to 48 before the module is asked for */
    temp.wCamTime = 47;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 48);
    isload_answer = 0;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && temp.eMod.fLoad == 0);
    isload_answer = 1;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && temp.eMod.fLoad != 0 && temp.wLvl == 0);
    /* loaded: message 280 without a parameter, 264 with one, for the side the stub answers */
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 5 && is(1, SIDE, 0x200C01, 0, 0, 0) && is(2, POPUP, 5, -120, 80, 280));
    assert(is(3, DEPEND, 500, 0, 0, 0) && depend_call.eNO == 999 && is(4, BLACK, 48, 0, 0, 0) && temp.wLvl == 1);
    temp.wLvl = 0, in.dwPar = 3;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 5 && is(2, POPUP, 5, -120, 80, 264) && temp.wLvl == 1);
    /* level 1 waits until no effect is in use */
    temp.oNum = 2, temp.eID[0] = 9, temp.eID[1] = 11, temp.ePos[0] = 0x10a, temp.ePos[1] = 3;
    isuse_answer[9] = 1, isuse_answer[11] = 0;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 9, 0, 0, 0) && is(2, ISUSE, 11, 0, 0, 0) && temp.wLvl == 1);
    /* dwFrom 1: spell rest 255 per position */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 7 && log_id[3] == CLOSE && is(4, SPELL, 1, 10, 255, 0));
    assert(is(5, SPELL, 0, 3, 255, 0) && is(6, FADE, -1, 1, 0, 1) && temp.wLvl == 2);
    /* otherwise the curse rest from dwPar, and the status bit only for a non-zero dwPar */
    temp.wLvl = 1, in.dwFrom = 0;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 9 && is(4, CURSE, 1, 10, 3, 0) && is(5, USTAT, 1, 10, 0, 1));
    assert(is(6, CURSE, 0, 3, 3, 0) && is(7, USTAT, 0, 3, 0, 1) && log_id[8] == FADE && temp.wLvl == 2);
    temp.wLvl = 1, in.dwPar = 0;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 7 && is(4, CURSE, 1, 10, 0, 0) && is(5, CURSE, 0, 3, 0, 0));
    /* later levels fade up by 15 to 128; the first bit is set at 128 */
    temp.wCount_1 = 48, temp.wCamTime = 16;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 63, 0, 0, 0) && temp.wCamTime == 16);
    temp.wCount_1 = 120;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 17);
    /* camera level 0 with that bit: view restored once the counter has passed 16 */
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 3 && is(2, RESTORE, 32, 0, 0, 0) && log_p[2] == &temp);
    assert(temp.bCamLvl == 1 && temp.wCamTime == 0);
    /* later camera levels: the second bit once the counter has passed 32; both bits wait for the fade */
    temp.wCamTime = 32;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 33);
    temp.wCount_1 = 48;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 63, 0, 0, 0) && temp.wCamTime == 34);
    temp.wCount_1 = 128, finfade_answer = 0;
    assert(step(Ta_Eff_000, &in, -1) == 1 && log_n == 3 && log_id[2] == FINFADE);
    finfade_answer = 1, disp_answer = 0;
    assert(step(Ta_Eff_000, &in, -1) == 0 && log_n == 5 && log_id[3] == WDISP && log_id[4] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_000, &in, -1) == 0 && log_n == 6 && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
}

/* Ta_Eff_064 (branch coverage). */
static void check_064(void)
{
    /* dwFrom: own slot 0 and the other side's slot 0 (bits 21 and 10); dwTo: unit (1, 10) */
    struct jp_6989c in = {64, 0x200400, 0x10a, 0};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_064, &in, 0) == 1 && log_n == 4 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, FOCUS, 30, 0x10a, 100, 180) && is(3, LOAD, 529, 0, 0, 0));
    /* level 0 waits for the module, then for the counter to reach 30 */
    isload_answer = 0;
    assert(step(Ta_Eff_064, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wCamTime == 0);
    isload_answer = 1, temp.wCamTime = 29;
    assert(step(Ta_Eff_064, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 30 && temp.wLvl == 0);
    do_answer = 9;
    assert(step(Ta_Eff_064, &in, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 271) && log_id[3] == DOEFF);
    assert(do_call.eNO == 529 && do_call.dwFrom == 0 && do_call.dwTo == 0x10a && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0x10a && temp.wLvl == 1 && temp.wCamTime == 31);
    /* level 1 waits for the effect */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_064, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    /* then every flagged unit's AP and DP, less the target's own, is added to the target */
    isuse_answer[9] = 0, loc_by_unit = 1;
    assert(step(Ta_Eff_064, &in, -1) == 1 && log_n == 19 && is(2, LOC, 1, 10, 0, 0) && is(3, LOC, 0, 0, 0, 0));
    assert(is(4, GAP, 0, 0, 1, 2) && is(5, GDP, 0, 0, 1, 2) && is(6, GAP3, 1, 10, 11, 3) && is(7, AP, 1, 10, 0, 700));
    assert(is(8, GDP3, 1, 10, 11, 3) && is(9, DP, 1, 10, 0, 1600) && is(10, LOC, 1, 0, 0, 0) && is(11, GAP, 1, 0, 1, 3));
    assert(is(12, GDP, 1, 0, 1, 3) && is(13, GAP3, 1, 10, 11, 3) && is(14, AP, 1, 10, 0, 800) && is(15, GDP3, 1, 10, 11, 3));
    assert(is(16, DP, 1, 10, 0, 1700) && log_id[17] == CLOSE && is(18, RESTORE, 16, 0, 0, 0) && log_p[18] == &temp);
    assert(temp.wLvl == 2 && temp.wCamTime == 0);
    loc_by_unit = 0;
    /* later levels end once the counter has passed 16 */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_064, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_064, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_064, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
}

/* Ta_Eff_035 runs its loop 100 times: these stubs count calls and keep the first arguments
 * instead of writing to the call log. */
static int rand_n, rand_vals[4], mapid_answer, mapid_n, mapid_args[2], ld_answer, ld_n, ld_args[5];
static int stack_answer, stack_n, maze_n, move_n, move_args[3][4];
int func_0019a5c8(void) { return rand_vals[rand_n++ % 4]; }
int SzDuel_GetMapUnitID(int x, int y)
{
    if (mapid_n++ == 0) {
        mapid_args[0] = x, mapid_args[1] = y;
    }
    return mapid_answer;
}
int Ta_IsLDRangeAvilityWithLocation(int a, int b, int c, int d, int e)
{
    if (ld_n++ == 0) {
        ld_args[0] = a, ld_args[1] = b, ld_args[2] = c, ld_args[3] = d, ld_args[4] = e;
    }
    return ld_answer;
}
int Ta_GetStackMapPiece(void) { return stack_n++ == 0 ? stack_answer : -1; }
void Ta_InitMazePiece(void) { maze_n++; }
void SzDuel_MoveUnit(int a, int b, int c, int d)
{
    if (move_n < 3) {
        move_args[move_n][0] = a, move_args[move_n][1] = b, move_args[move_n][2] = c, move_args[move_n][3] = d;
    }
    move_n++;
}
static int moved(int at, int a, int b, int c, int d)
{
    return move_args[at][0] == a && move_args[at][1] == b && move_args[at][2] == c && move_args[at][3] == d;
}
static int step_035(struct jp_6989c *in)
{
    temp.ePos[0] = 0;
    rand_n = mapid_n = ld_n = stack_n = maze_n = move_n = 0;
    return step(Ta_Eff_035, in, -1);
}
/* Ta_Eff_035 (branch coverage). */
static void check_035(void)
{
    struct jp_6989c in = {35, 0, 0, 0};

    memset(&temp, 0, sizeof(temp));
    status_once = 0, loc_by_unit = 0, loc_answer = 0x302;
    assert(step(Ta_Eff_035, &in, 0) == 1 && log_n == 4 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, LOAD, 35, 0, 0, 0) && is(3, STDVIEW, 16, 0, 0, 0));
    /* level 0 waits for the module, then starts effect 35 with position word 0 */
    isload_answer = 0;
    assert(step(Ta_Eff_035, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0 && temp.oNum == 0);
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_035, &in, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 35 && do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0 && temp.oNum == 1 && temp.wLvl == 1);
    /* later: while the effect is in use, nothing before status 2 or after the shuffle was done */
    isuse_answer[9] = 1, status_answer[9] = 1;
    rand_vals[0] = 8, rand_vals[1] = 9, rand_vals[2] = 3, rand_vals[3] = 24, mapid_answer = -1;
    assert(step_035(&in) == 1 && log_n == 3 && is(1, ISUSE, 9, 0, 0, 0) && is(2, STATUS, 9, 0, 0, 0) && maze_n == 0);
    status_answer[9] = 2;
    rand_n = maze_n = 0, temp.ePos[0] = 1;
    assert(step(Ta_Eff_035, &in, -1) == 1 && log_n == 3 && maze_n == 0 && rand_n == 0);
    /* the shuffle: 100 draws of (x % 7, z % 7, odd -> side 1, % 10 + 1); a free cell takes the unit */
    assert(step_035(&in) == 1 && log_n == 3 && maze_n == 1 && rand_n == 400 && mapid_n == 100 && move_n == 100);
    assert(mapid_args[0] == 1 && mapid_args[1] == 2 && moved(0, 1, 5, 1, 2) && ld_n == 0 && temp.ePos[0] == 1);
    rand_vals[2] = 4;
    assert(step_035(&in) == 1 && moved(0, 0, 5, 1, 2));
    /* an occupied cell: skipped when the occupant passes the range test or sits in slot 0 */
    rand_vals[2] = 3, mapid_answer = 0x203, ld_answer = 1;
    assert(step_035(&in) == 1 && ld_n == 100 && stack_n == 0 && move_n == 0);
    assert(ld_args[0] == 2 && ld_args[1] == 3 && ld_args[2] == 1 && ld_args[3] == 2 && ld_args[4] == 11);
    ld_answer = 0, mapid_answer = 0x200;
    assert(step_035(&in) == 1 && ld_n == 100 && stack_n == 0 && move_n == 0);
    /* otherwise a spare cell is asked for; without one nothing moves, with one the two units swap
     * through it */
    mapid_answer = 0x203, stack_answer = -1;
    assert(step_035(&in) == 1 && stack_n == 100 && move_n == 0 && log_n == 3);
    stack_answer = 0x406;
    assert(step_035(&in) == 1 && stack_n == 100 && move_n == 3 && log_n == 4 && is(3, LOC, 1, 5, 0, 0));
    assert(moved(0, 1, 5, 6, 4) && moved(1, 2, 3, 2, 3) && moved(2, 1, 5, 1, 2) && temp.ePos[0] == 1);
    /* effect out of use: the view is restored once, then the counter has to pass 16 */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_035, &in, -1) == 1 && log_n == 3 && is(2, RESTORE, 16, 0, 0, 0) && log_p[2] == &temp && temp.bCamLvl == 1);
    temp.wCamTime = 16;
    assert(step(Ta_Eff_035, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_035, &in, -1) == 0 && log_n == 4 && log_id[2] == WDISP && log_id[3] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_035, &in, -1) == 0 && log_n == 5 && is(3, WDRAW, 1, 0, 0, 0) && log_id[4] == CLOSE);
}

/* Ta_Eff_013 (branch coverage). */
static void check_013(void)
{
    struct jp_6989c in = {13, 1, 0x200400, 7};

    memset(&temp, 0, sizeof(temp));
    num_answer = 1, owner_answer = 0x10a, status_once = 0;
    assert(step(Ta_Eff_013, &in, 3) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, FADE, 3, -1, 1, 1) && is(3, NUM, 0x200400, 0, 0, 0) && is(4, OWNER, 0x200400, 0, 0, 0));
    assert(is(5, FOCUS, 0, 0x10a, 100, 180) && is(6, LOAD, 513, 0, 0, 0) && temp.wCount_1 == 128);
    num_answer = 2;
    assert(step(Ta_Eff_013, &in, 0) == 1 && log_n == 6 && is(4, STDVIEW, 16, 0, 0, 0) && is(5, LOAD, 513, 0, 0, 0));
    /* level 0: the black-out level falls by 15 while above 63, then is held at 48 */
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 85, 0, 0, 0));
    temp.wCount_1 = 64;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 49, 0, 0, 0));
    temp.wCount_1 = 63, isload_answer = 0, temp.wCamTime = 17;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0) && temp.wCamTime == 17);
    isload_answer = 1, temp.wCamTime = 16;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 3 && temp.wCamTime == 17 && temp.wLvl == 0);
    /* counter past 16: the flag-list effects, except for dwFrom 2 or 4 with dwPar 0 */
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 4 && is(2, DEPEND, 513, 0, 0, 0) && log_p[2] == &temp);
    assert(depend_call.eNO == 999 && temp.wLvl == 1 && temp.wCamTime == 0);
    temp.wLvl = 0, temp.wCamTime = 17, in.dwFrom = 2;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 4 && is(2, DEPEND, 513, 0, 0, 0) && temp.wLvl == 1);
    temp.wLvl = 0, temp.wCamTime = 17, in.dwFrom = 3, in.dwPar = 0;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 4 && is(2, DEPEND, 513, 0, 0, 0) && temp.wLvl == 1);
    temp.wLvl = 0, temp.wCamTime = 17, in.dwFrom = 2, do_answer = 9;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 8 && is(2, NUM, 0x200400, 0, 0, 0) && is(3, OWNER, 0x200400, 0, 0, 0));
    assert(log_id[4] == DOEFF && is(5, OWNER, 0x200400, 1, 0, 0) && log_id[6] == DOEFF && is(7, BLACK, 48, 0, 0, 0));
    assert(do_call.eNO == 513 && do_call.dwFrom == 0 && do_call.dwTo == 0x10a && do_call.dwPar == 1);
    assert(temp.oNum == 2 && temp.eID[0] == 9 && temp.eID[1] == 9 && temp.ePos[0] == 0x10a && temp.ePos[1] == 0x10a);
    assert(temp.wLvl == 1 && temp.wCamTime == 0);
    temp.wLvl = 0, temp.wCamTime = 17, in.dwFrom = 4, num_answer = 0;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 4 && log_id[2] == NUM && temp.oNum == 2 && temp.wLvl == 1);
    /* level 1: an effect in use with status 2 and a position is applied once */
    temp.eID[0] = 9, temp.eID[1] = 11, temp.ePos[0] = 0x105, temp.ePos[1] = 0x20a;
    isuse_answer[9] = 1, isuse_answer[11] = 0, status_answer[9] = 1, in.dwFrom = 3, in.dwPar = 7;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 4 && is(1, ISUSE, 9, 0, 0, 0) && is(2, STATUS, 9, 0, 0, 0));
    assert(is(3, ISUSE, 11, 0, 0, 0) && temp.ePos[0] == 0x105 && temp.wLvl == 1);
    status_answer[9] = 2, temp.ePos[0] = -1;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 4);
    temp.ePos[0] = 0x105;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 7 && is(3, BUSY2, 0x105, 0, 0, 0) && is(4, USTAT, 1, 5, 5, 7));
    assert(is(5, USTAT, 1, 5, 3, 7) && temp.ePos[0] == -1);
    temp.ePos[0] = 0x105, in.dwFrom = 5;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 7 && is(5, USTAT, 1, 5, 3, 7));
    temp.ePos[0] = 0x105, in.dwFrom = 2;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 6 && is(4, DOOPEN, 1, 5, 7, 0) && temp.ePos[0] == -1);
    temp.ePos[0] = 0x105, in.dwFrom = 4;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 6 && is(4, DOOPEN, 1, 5, 7, 0));
    temp.ePos[0] = 0x105, in.dwFrom = 1;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 5 && is(3, BUSY2, 0x105, 0, 0, 0) && temp.ePos[0] == -1);
    /* every effect out of use and the counter past 16: view restored, fade back, next level */
    isuse_answer[9] = 0, temp.wCamTime = 16;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 3 && temp.wCamTime == 17 && temp.wLvl == 1);
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 5 && is(3, RESTORE, 16, 0, 0, 0) && log_p[3] == &temp);
    assert(is(4, FADE, -1, 1, 0, 1) && temp.wCamTime == 0 && temp.wLvl == 2);
    /* last level: up by 15 to 128, counter past 16, fade finished; the popup closes first */
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120, temp.wCamTime = 16;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 17);
    finfade_answer = 0;
    assert(step(Ta_Eff_013, &in, -1) == 1 && log_n == 3 && log_id[2] == FINFADE && temp.wCamTime == 18);
    finfade_answer = 1, disp_answer = 0;
    assert(step(Ta_Eff_013, &in, -1) == 0 && log_n == 5 && log_id[3] == CLOSE && log_id[4] == WDISP);
    disp_answer = 1;
    assert(step(Ta_Eff_013, &in, -1) == 0 && log_n == 6 && is(5, WDRAW, 1, 0, 0, 0));
}

/* Ta_Eff_112 (branch coverage). */
static void check_112(void)
{
    struct jp_6989c in = {112, 0, 0x200400, 0};

    memset(&temp, 0, sizeof(temp));
    num_answer = 2, owner_answer = 0x10a, status_once = 0;
    assert(step(Ta_Eff_112, &in, 3) == 1 && log_n == 14 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, FADE, 3, -1, 1, 1) && is(3, LOAD, 612, 0, 0, 0) && is(4, NUM, 0x200400, 0, 0, 0) && is(5, OWNER, 0x200400, 0, 0, 0));
    assert(is(6, BUSY2, 0x10a, 0, 0, 0) && is(7, DOOPEN, 1, 10, 1, 0) && is(9, OWNER, 0x200400, 1, 0, 0));
    assert(is(13, STDVIEW, 30, 0, 0, 0) && temp.wCount_1 == 128);
    num_answer = 0;
    assert(step(Ta_Eff_112, &in, 0) == 1 && log_n == 6 && is(5, STDVIEW, 30, 0, 0, 0));
    /* level 0: falls by 15 while above 48 and never below it */
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 85, 0, 0, 0));
    temp.wCount_1 = 49;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    /* at 48: every flagged unit's model has to be standing by, then the module, then the counter */
    num_answer = 2, standby_answer = 0, isload_answer = 1, temp.wCamTime = 46;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 10 && is(2, OWNER, 0x200400, 0, 0, 0) && is(3, STANDBY, 0x10a, 0, 0, 0));
    assert(is(5, OWNER, 0x200400, 1, 0, 0) && log_id[8] == NUM && is(9, BLACK, 48, 0, 0, 0) && temp.wCamTime == 46);
    standby_answer = 1, isload_answer = 0;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 11 && log_id[9] == ISLOAD && temp.wCamTime == 46);
    isload_answer = 1, temp.wCamTime = 45;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 11 && temp.wCamTime == 46 && temp.wLvl == 0);
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 13 && is(10, POPUP, 0, -120, 80, 276) && is(11, DEPEND, 612, 0, 0, 0));
    assert(log_p[11] == &temp && depend_call.eNO == 999 && temp.wCamTime == 0 && temp.wLvl == 1);
    /* a single unit also gets the focus camera */
    temp.wLvl = 0, temp.wCamTime = 46, num_answer = 1;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 12 && is(7, OWNER, 0x200400, 0, 0, 0) && is(8, FOCUS, 0, 0x10a, 100, 180));
    assert(is(9, POPUP, 0, -120, 80, 276) && temp.wLvl == 1);
    /* level 1: an effect in use sets the hit motion, fades its unit once after 32 counts, and
     * removes the unit at status 2 */
    temp.oNum = 2, temp.eID[0] = 9, temp.eID[1] = 11, temp.ePos[0] = 0x105, temp.ePos[1] = 0x20a;
    isuse_answer[9] = 1, isuse_answer[11] = 0, status_answer[9] = 1, temp.wCamTime = 31;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 5 && is(1, ISUSE, 9, 0, 0, 0) && is(2, OUCH, 0x105, 0, 0, 0));
    assert(is(3, STATUS, 9, 0, 0, 0) && is(4, ISUSE, 11, 0, 0, 0) && temp.wCamTime == 32 && temp.ePos[0] == 0x105);
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 6 && is(3, UFADE, 1, 5, -1, 1) && temp.ePos[0] == 0x10105);
    assert(temp.wCamTime == 33);
    status_answer[9] = 2;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 7 && is(2, OUCH, 0x105, 0, 0, 0) && is(4, BUSY2, 0x105, 1, 0, 0));
    assert(is(5, TDEL, 1, 5, 0, 0) && temp.ePos[0] == -1 && temp.wCamTime == 33);
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 5 && is(2, OUCH, 0xFFFF, 0, 0, 0) && temp.wLvl == 1);
    /* every effect out of use: fade back, popup closed, view restored */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_112, &in, -1) == 1 && log_n == 6 && is(3, FADE, -1, 1, 0, 1) && log_id[4] == CLOSE);
    assert(is(5, RESTORE, 16, 0, 0, 0) && log_p[5] == &temp && temp.wCamTime == 0 && temp.wLvl == 2);
    check_leave(Ta_Eff_112, &in);
}

/* Ta_Eff_114 (branch coverage). */
static void check_114(void)
{
    struct jp_6989c in = {114, 0x105, 0, 0};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_114, &in, 0) == 1 && log_n == 5 && log_id[0] == INIT && log_p[0] == &temp && is(1, LOAD, 625, 0, 0, 0));
    assert(log_id[2] == HANDCLOSE && is(3, DESTINY, 0, 0, 0, 0) && is(4, BUSY2, 0x10500, 1, 0, 0) && temp.wCount_1 == 128);
    /* level 0: the counter steps down; its low byte goes to both black-outs */
    ctrl_answer = 0x130;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 4 && is(0, WDRAW, 0, 0, 0, 0) && is(1, CTRL, 128, 15, -1, 0));
    assert(is(2, BLACKB, 0x30, 0, 0, 0) && is(3, BLACK, 0x30, 0, 0, 0) && temp.wCount_1 == 0x130 && temp.wLvl == 0);
    /* at zero the module has to be loaded before the level advances */
    ctrl_answer = 0, isload_answer = 0;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 5 && is(2, BLACKB, 0, 0, 0, 0) && is(3, BLACK, 0, 0, 0, 0));
    assert(log_id[4] == ISLOAD && log_p[4] == &temp && temp.wLvl == 0);
    isload_answer = 1;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 5 && temp.wLvl == 1);
    /* level 1: message popup and the effect call on the unit's location */
    loc_by_unit = 0, loc_answer = 0x20a, do_answer = 9;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 4 && is(1, POPUP, 0, -120, 80, 277) && is(2, LOC, 0x105, 0, 0, 0));
    assert(log_id[3] == DOEFF && log_p[3] == &temp && do_call.eNO == 625 && do_call.dwFrom == 0 && do_call.dwTo == 0x20a);
    assert(do_call.dwPar == 0 && temp.eID[0] == 9 && temp.wLvl == 2);
    /* level 2: sounds at counts 0 and 19 while the effect is in use */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 9, 0, 0, 0) && is(2, SE, 0, 34, 0, 0) && temp.wCamTime == 1);
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 2);
    temp.wCamTime = 19;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 5 && is(2, BACKFOCUS, 0, 0x10500, 300, 300) && is(3, SE, 0, 37, 0, 0));
    assert(is(4, SE, 0, 36, 0, 0) && temp.wCamTime == 20 && temp.wLvl == 2);
    isuse_answer[9] = 0, temp.wCount_1 = 7;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 4 && log_id[2] == CLOSE && is(3, RESTORE, 16, 0, 0, 0) && log_p[3] == &temp);
    assert(temp.wCamTime == 0 && temp.wCount_1 == 0 && temp.wLvl == 3);
    /* level 3: the counter steps up to 128, then the sequence ends after count 16 */
    ctrl_answer = 100;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 4 && is(1, CTRL, 0, 15, 1, 0) && is(2, BLACKB, 100, 0, 0, 0));
    assert(is(3, BLACK, 100, 0, 0, 0) && temp.wCamTime == 1 && temp.wCount_1 == 100);
    ctrl_answer = 128, temp.wCamTime = 15;
    assert(step(Ta_Eff_114, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 16 && temp.wCount_1 == 128);
    disp_answer = 0;
    assert(step(Ta_Eff_114, &in, -1) == 0 && log_n == 5 && is(2, BUSY2, 0x10500, 0, 0, 0) && log_id[3] == WDISP && log_id[4] == CLOSE);
    disp_answer = 1, temp.wCamTime = 16;
    assert(step(Ta_Eff_114, &in, -1) == 0 && log_n == 6 && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
    disp_answer = 0;
}

/* Ta_Eff_045 (branch coverage). */
static void check_045(void)
{
    struct jp_6989c in = {45, 0x105, 0x20a, 0};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_045, &in, 0) == 1 && log_n == 9 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, DOOPEN, 1, 5, 1, 0) && is(3, DOOPEN, 2, 10, 1, 0) && is(4, FOCUS, 0, 0x105, 100, 180) && is(5, LOAD, 545, 0, 0, 0));
    assert(is(6, LOAD, 601, 0, 0, 0) && is(7, LOAD, 602, 0, 0, 0) && is(8, LOAD, 603, 0, 0, 0));
    /* level 0 waits for both open rates (255), the target's control rate (128), a standing-by
     * model when the target's kind is zero, and the modules */
    openrate_answer = 254, openrate_side2 = 0;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 2 && is(1, OPENRATE, 1, 5, 0, 0));
    openrate_answer = 255, openrate_side2 = 254;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 3 && is(2, OPENRATE, 2, 10, 0, 0));
    openrate_side2 = 0, ctlc_answer = 127;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 4 && is(3, CTLC, 2, 10, 0, 0));
    ctlc_answer = 128, ukind_zero = 0, isload_answer = 0;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 6 && is(4, UKIND, 2, 10, 0, 0) && log_id[5] == ISLOAD && temp.wLvl == 0);
    ukind_zero = 1, standby_answer = 0, isload_answer = 1;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 6 && is(5, STANDBY, 0x20a, 0, 0, 0) && temp.wLvl == 0);
    standby_answer = 1;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 7 && log_id[6] == ISLOAD && log_p[6] == &temp && temp.wLvl == 1);
    ukind_zero = 0;
    /* level 1: popup and effect 602 on the source unit */
    do_answer = 9;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 3 && is(1, POPUP, 0, -120, 80, 259) && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0 && temp.eID[0] == 9 && temp.wLvl == 2);
    /* level 2: when it ends, status bit 0 and effect 601 with parameter 1 */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[9] = 0, do_answer = 11;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 4 && is(2, USTAT, 1, 5, 0, 1) && log_id[3] == DOEFF);
    assert(do_call.eNO == 601 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 1 && temp.eID[0] == 11 && temp.wLvl == 3);
    /* level 3: when it ends, the source unit is removed and effects 603 and 545 start on the target */
    isuse_answer[11] = 1;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 11, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[11] = 0, do_answer = 12;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 6 && is(2, FOCUS, 16, 0x20a, 100, 180) && is(3, TDEL, 1, 5, 0, 0));
    assert(log_id[4] == DOEFF && log_id[5] == DOEFF && do_prev.eNO == 603 && do_prev.dwTo == 0x20a && do_prev.dwPar == 0);
    assert(do_call.eNO == 545 && do_call.dwFrom == 0 && do_call.dwTo == 0x20a && do_call.dwPar == 0);
    assert(temp.eID[0] == 12 && temp.eID[1] == 12 && temp.wLvl == 4);
    /* level 4: both effects have to end */
    temp.eID[0] = 9, temp.eID[1] = 11, isuse_answer[9] = 1;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0));
    isuse_answer[9] = 0, isuse_answer[11] = 1;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 3 && is(2, ISUSE, 11, 0, 0, 0) && temp.wLvl == 4);
    isuse_answer[11] = 0;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 6 && is(3, TDEL, 2, 10, 0, 0) && log_id[4] == CLOSE && is(5, RESTORE, 16, 0, 0, 0));
    assert(log_p[5] == &temp && temp.wLvl == 5);
    /* level 5: ends after count 16 */
    temp.wCamTime = 16, disp_answer = 0;
    assert(step(Ta_Eff_045, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    assert(step(Ta_Eff_045, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE && temp.wCamTime == 18);
    disp_answer = 1;
    assert(step(Ta_Eff_045, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    disp_answer = 0;
}

/* Ta_Eff_044 (branch coverage). */
static void check_044(void)
{
    struct jp_6989c in = {44, 0x105, 0x20a, 0x30000};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_044, &in, 0) == 1 && log_n == 8 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, DOOPEN, 1, 5, 1, 0) && is(3, DOOPEN, 2, 10, 1, 0) && is(4, FOCUS, 0, 0x105, 100, 180) && is(5, LOAD, 601, 0, 0, 0));
    assert(is(6, LOAD, 602, 0, 0, 0) && is(7, LOAD, 544, 0, 0, 0));
    /* a non-zero low parameter word loads module 557 as well */
    in.dwPar = 0x30064;
    assert(step(Ta_Eff_044, &in, 0) == 1 && log_n == 9 && is(8, LOAD, 557, 0, 0, 0) && log_p[8] == &temp);
    /* level 0: popup, then waits for both open rates (255) and the modules; then effect 602 */
    openrate_answer = 254, openrate_side2 = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 3 && is(1, POPUP, 0, -120, 80, 259) && is(2, OPENRATE, 1, 5, 0, 0));
    openrate_answer = 255, openrate_side2 = 254;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 4 && is(3, OPENRATE, 2, 10, 0, 0));
    openrate_side2 = 0, isload_answer = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 5 && log_id[4] == ISLOAD && log_p[4] == &temp && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 6 && log_id[5] == DOEFF && log_p[5] == &temp);
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0 && temp.eID[0] == 9 && temp.wLvl == 1);
    /* level 1: waits while the effect answers 1; then status bit 0 on the source unit */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 3 && is(2, USTAT, 1, 5, 0, 1) && temp.wLvl == 2);
    /* level 2: when it ends, effect 601 with parameter 1 */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[9] = 0, do_answer = 11;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 3 && log_id[2] == DOEFF);
    assert(do_call.eNO == 601 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 1 && temp.eID[0] == 11 && temp.wLvl == 3);
    /* level 3: when it ends, camera, popup 264, the source unit is removed, effect 544 on the target;
     * effect 557 only with a low parameter word and a target of kind zero */
    isuse_answer[11] = 1;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 11, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[11] = 0, do_answer = 12, in.dwPar = 0x30000, temp.eID[1] = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 6 && is(2, FOCUS, 16, 0x20a, 100, 180) && is(3, POPUP, 0, -120, 80, 264));
    assert(is(4, TDEL, 1, 5, 0, 0) && log_id[5] == DOEFF && do_call.eNO == 544 && do_call.dwFrom == 0 && do_call.dwTo == 0x20a);
    assert(do_call.dwPar == 0 && temp.eID[0] == 12 && temp.eID[1] == 0 && temp.wLvl == 4);
    temp.wLvl = 3, in.dwPar = 0x30064, ukind_zero = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 7 && is(6, UKIND, 2, 10, 0, 0) && temp.eID[1] == 0 && temp.wLvl == 4);
    temp.wLvl = 3, ukind_zero = 1, do_answer = 13;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 9 && is(7, POPUP, 0, -120, 80, 263) && log_id[8] == DOEFF);
    assert(do_prev.eNO == 544 && do_call.eNO == 557 && do_call.dwFrom == 0 && do_call.dwTo == 0x20a && do_call.dwPar == 0);
    assert(temp.eID[0] == 13 && temp.eID[1] == 13 && temp.wLvl == 4);
    /* level 4: waits while the first effect answers 1; no status call */
    isuse_answer[13] = 1;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 13, 0, 0, 0) && temp.wLvl == 4);
    isuse_answer[13] = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 2 && temp.wLvl == 5);
    /* level 5: status bit 0 and the curse count (parameter bits 16-23) on the target; attack and
     * defence change only with a low parameter word and a target of kind zero */
    in.dwPar = 0x30000;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 5 && is(1, USTAT, 2, 10, 0, 1) && is(2, CURSE, 2, 10, 3, 0));
    assert(log_id[3] == CLOSE && is(4, RESTORE, 16, 0, 0, 0) && log_p[4] == &temp && temp.wLvl == 6);
    temp.wLvl = 5, in.dwPar = 0x30064, ukind_zero = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 6 && is(3, UKIND, 2, 10, 0, 0) && log_id[4] == CLOSE && temp.wLvl == 6);
    temp.wLvl = 5, ukind_zero = 1;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 8 && is(4, AP, 2, 10, 1, 100) && is(5, DP, 2, 10, 1, 100));
    assert(log_id[6] == CLOSE && is(7, RESTORE, 16, 0, 0, 0) && temp.wLvl == 6);
    ukind_zero = 0;
    /* level 6: ends after count 16 */
    temp.wCamTime = 16, disp_answer = 0;
    assert(step(Ta_Eff_044, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    assert(step(Ta_Eff_044, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE && temp.wCamTime == 18);
    disp_answer = 1;
    assert(step(Ta_Eff_044, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    disp_answer = 0;
}

/* Ta_Eff_051 (branch coverage). */
static void check_051(void)
{
    /* dwFrom: side 1 slot 2; dwTo: side 1 slot 3; dwPar 1 also deletes dwFrom */
    struct jp_6989c in = {51, 0x102, 0x103, 1};

    memset(&temp, 0, sizeof(temp));
    status_once = 0;
    assert(step(Ta_Eff_051, &in, 0) == 1 && log_n == 5 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, FOCUS, 0, 0x102, 180, 180) && is(3, LOAD, 602, 0, 0, 0) && is(4, LOAD, 51, 0, 0, 0));
    /* level 0 waits for the module, then popup 259 and effect 602 on dwFrom */
    isload_answer = 0;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9, temp.ePos[0] = 7;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 259) && log_id[3] == DOEFF);
    assert(log_p[3] == &temp && do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x102 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0 && temp.oNum == 1 && temp.wLvl == 1);
    /* level 1 waits for that effect, then marks the unit, names its card and starts effect 51 on dwFrom */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0, do_answer = 11, temp.ePos[1] = 7;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 7 && is(2, USTAT, 1, 2, 0, 1) && is(3, CARDNO, 1, 2, 0, 0));
    assert(is(4, MSG, 722, 0, 0, 0) && is(5, POPUP, 0, -120, 80, 0x4D5347) && log_id[6] == DOEFF);
    assert(do_call.eNO == 51 && do_call.dwFrom == 0 && do_call.dwTo == 0x102 && do_call.dwPar == 0);
    assert(temp.eID[1] == 11 && temp.ePos[1] == 0 && temp.oNum == 2 && temp.wLvl == 2);
    /* level 2 needs the last effect in use, at status 2 and with position word 0 */
    isuse_answer[11] = 0;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 11, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[11] = 1, status_answer[11] = 1;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 3 && is(2, STATUS, 11, 0, 0, 0) && temp.wLvl == 2);
    status_answer[11] = 2, temp.ePos[1] = 5;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 3 && temp.wLvl == 2 && temp.oNum == 2);
    temp.ePos[1] = 0, temp.ePos[2] = 7, do_answer = 13;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 5 && is(3, FOCUS, 0, 0x103, 100, 180) && log_id[4] == DOEFF);
    assert(do_call.eNO == 51 && do_call.dwFrom == 0 && do_call.dwTo == 0x103 && do_call.dwPar == 0);
    assert(temp.eID[2] == 13 && temp.ePos[2] == 0 && temp.ePos[1] == 1 && temp.wLvl == 3 && temp.oNum == 3);
    /* level 3 moves the camera to dwTo */
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 2 && is(1, FOCUS, 16, 0x103, 300, 300) && temp.wLvl == 4);
    /* level 4 waits until no effect is in use, then deletes the units */
    isuse_answer[9] = 0, isuse_answer[11] = 1, isuse_answer[13] = 0;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 4 && is(1, ISUSE, 9, 0, 0, 0) && is(3, ISUSE, 13, 0, 0, 0) && temp.wLvl == 4);
    isuse_answer[11] = 0;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 8 && is(4, TDEL, 1, 2, 0, 0) && is(5, TDEL, 1, 3, 0, 0));
    assert(log_id[6] == CLOSE && is(7, RESTORE, 16, 0, 0, 0) && log_p[7] == &temp && temp.wLvl == 5);
    in.dwPar = 0, temp.wLvl = 4;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 7 && is(4, TDEL, 1, 3, 0, 0) && log_id[5] == CLOSE && temp.wLvl == 5);
    /* later levels pass 16, restore the window, close the popup and end */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_051, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_051, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_051, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    status_answer[11] = 0;
    memset(&temp, 0, sizeof(temp));
}

/* Ta_SetGravityEffect (branch coverage). The three stubs below answer from tables; the reserve stub
 * also copies four words of the table it is given, to show what the function stored there. */
enum { MINORDER = 300, CONVOFF, RESERVE };
static int minorder_answer[8], minorder_n, reserve_answer[4], reserve_n, reserve_seen[4][4];
int Ta_GetMinimumOrderUnitFromFlagList2(int word, int side)
{
    note(MINORDER, NULL, word, side, 0, 0);
    assert(minorder_n < 8);
    return minorder_answer[minorder_n++];
}
unsigned ConvUnitIDtoFlagList_OFF(unsigned x, int flag, int n) { note(CONVOFF, NULL, (int)x, flag, n, 0); return x - 1; }
int Ta_RequestGravityReserve(int a, int b, int c, int *table)
{
    note(RESERVE, table, a, b, c, 0);
    assert(reserve_n < 4);
    reserve_seen[reserve_n][0] = table[0], reserve_seen[reserve_n][1] = table[1];
    reserve_seen[reserve_n][2] = table[2], reserve_seen[reserve_n][3] = table[21];
    return reserve_answer[reserve_n++];
}
static int gravity(struct jp_6989c *in, int dist)
{
    log_n = 0, minorder_n = 0, reserve_n = 0;
    return Ta_SetGravityEffect(in, &temp, dist);
}
static void check_gravity(void)
{
    /* units 0x100.. stand at x 3, z 1, 2, 3, 4; dwFrom is x 3, z 5 */
    struct jp_6989c in = {72, 0x305, 0x77, 1};
    static const int none[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

    memset(&temp, 0, sizeof(temp));
    num_by_value = 1, num_answer = 4, owner_by_n = 1, owner_answer = 0x100, loc_by_unit = 1, do_answer = 9;
    /* distance 3 selects unit 0x101 only; of four picks one is negative, one gets no place and two
     * start effect 572 */
    minorder_answer[0] = -1, minorder_answer[1] = 0x102, minorder_answer[2] = 0x105, minorder_answer[3] = 0x106;
    reserve_answer[0] = -1, reserve_answer[1] = 0x406, reserve_answer[2] = 0x407;
    assert(gravity(&in, 3) == 4 && log_n == 23 && is(0, NUM, 0x77, 0, 0, 0) && is(1, OWNER, 0x77, 0, 0, 0) && is(2, LOC, 1, 0, 0, 0));
    assert(is(5, CONV, 0, 1, 1, 0) && is(8, OWNER, 0x77, 3, 0, 0) && is(9, LOC, 1, 3, 0, 0) && is(10, NUM, 1, 0, 0, 0));
    assert(is(11, MINORDER, 1, 1, 0, 0) && is(12, MINORDER, 1, 1, 0, 0) && is(13, CONVOFF, 1, 1, 2, 0) && is(14, RESERVE, 5, 3, 0x102, 0));
    assert(is(15, MINORDER, 0, 1, 0, 0) && is(16, CONVOFF, 0, 1, 5, 0) && is(17, RESERVE, 5, 3, 0x105, 0) && log_id[18] == DOEFF);
    assert(is(19, MINORDER, -1, 1, 0, 0) && is(20, CONVOFF, -1, 1, 6, 0) && is(21, RESERVE, 5, 3, 0x106, 0) && log_id[22] == DOEFF);
    assert(log_p[18] == &temp && log_p[14] == log_p[21]);
    assert(reserve_seen[0][0] == -1 && reserve_seen[0][1] == -1 && reserve_seen[0][2] == -1 && reserve_seen[0][3] == -1);
    assert(reserve_seen[2][0] == -1 && reserve_seen[2][1] == -1 && reserve_seen[2][2] == 0x406 && reserve_seen[2][3] == -1);
    assert(do_prev.eNO == 572 && do_prev.dwFrom == 0 && do_prev.dwTo == 0x105 && do_prev.dwPar == 0x406);
    assert(do_call.eNO == 572 && do_call.dwFrom == 0 && do_call.dwTo == 0x106 && do_call.dwPar == 0x407);
    assert(temp.eID[0] == 9 && temp.eID[1] == 9 && temp.ePos[0] == 0x1050406 && temp.ePos[1] == 0x1060407 && temp.oNum == 2);
    /* dwFrom left of and below the units, then right of them: both orders of each coordinate pair */
    memcpy(minorder_answer, none, sizeof(none));
    num_answer = 3, in.dwFrom = 0x102;
    assert(gravity(&in, 3) == 3 && log_n == 13 && is(3, CONV, 0, 1, 0, 0) && is(8, CONV, 0, 1, 2, 0) && is(9, NUM, 2, 0, 0, 0));
    assert(is(12, MINORDER, 2, 1, 0, 0) && temp.oNum == 2);
    in.dwFrom = 0x502;
    assert(gravity(&in, 2) == 3 && log_n == 12 && is(5, CONV, 0, 1, 1, 0) && is(8, NUM, 1, 0, 0, 0));
    /* no unit at that distance, and no unit at all */
    assert(gravity(&in, 9) == 0 && log_n == 8 && is(7, NUM, 0, 0, 0, 0));
    num_answer = 0;
    assert(gravity(&in, 2) == 0 && log_n == 2 && is(1, NUM, 0, 0, 0, 0));
    num_by_value = 0, owner_by_n = 0, loc_by_unit = 0;
    memset(&temp, 0, sizeof(temp));
}

/* Ta_ManageDestroyEffect (branch coverage). */
static void check_manage_destroy(void)
{
    struct jp_6989c in = {0, 0, 0x777, 0}, call;

    /* first loop: only a running effect with status 2 and a non-negative position is marked */
    memset(&temp, 0, sizeof(temp));
    temp.oNum = 3, temp.eID[0] = 5, temp.eID[1] = 7, temp.eID[2] = 9;
    temp.ePos[0] = 10, temp.ePos[1] = 20, temp.ePos[2] = -5;
    isuse_answer[5] = 0, isuse_answer[7] = isuse_answer[9] = 1, status_once = 0, status_answer[7] = 1, status_answer[9] = 2;
    log_n = 0;
    assert(Ta_ManageDestroyEffect(&in, &temp, &call) == 0 && log_n == 5 && is(0, ISUSE, 5, 0, 0, 0) && is(1, ISUSE, 7, 0, 0, 0));
    assert(is(2, STATUS, 7, 0, 0, 0) && is(3, ISUSE, 9, 0, 0, 0) && is(4, STATUS, 9, 0, 0, 0));
    assert(temp.ePos[0] == 10 && temp.ePos[1] == 20 && temp.ePos[2] == -5 && temp.wCount_2 == 0);
    status_answer[7] = 2;
    log_n = 0;
    assert(Ta_ManageDestroyEffect(&in, &temp, &call) == 0 && log_n == 5 && temp.ePos[1] == -1 && temp.ePos[0] == 10 && temp.wCount_2 == 2);
    isuse_answer[7] = isuse_answer[9] = 0, status_answer[7] = status_answer[9] = 0;
    /* the count runs to 17 before anything else happens */
    temp.oNum = 0, temp.wCount_2 = 16;
    log_n = 0;
    assert(Ta_ManageDestroyEffect(&in, &temp, &call) == 0 && log_n == 0 && temp.wCount_2 == 17);
    /* count 17: no listed unit qualifies (status 4 clear, or kind not zero), so the pick is made
     * from the whole target list; effect 603 on the pick, every other unit fades */
    num_answer = 3, num_by_value = 1, owner_answer = 0x105, owner_by_n = 1, gustat_zero_pos = 5, ukind_zero = 0;
    random_answer = 0x106, do_answer = 12;
    log_n = 0;
    assert(Ta_ManageDestroyEffect(&in, &temp, &call) == 1 && log_n == 28 && is(0, NUM, 0x777, 0, 0, 0) && is(1, OWNER, 0x777, 0, 0, 0));
    assert(is(2, GUSTAT, 1, 5, 4, 0) && is(4, OWNER, 0x777, 1, 0, 0) && is(5, GUSTAT, 1, 6, 4, 0) && is(6, UKIND, 1, 6, 0, 0));
    assert(is(10, UKIND, 1, 7, 0, 0) && is(11, NUM, 0x777, 0, 0, 0) && is(12, NUM, 0, 0, 0, 0) && is(13, RANDOM, 0x777, 0, 0, 0));
    assert(is(14, LOC, 1, 6, 0, 0) && log_id[15] == DOEFF && log_p[15] == &temp && is(16, FOCUS, 0, 0x106, 100, 180));
    assert(call.eNO == 603 && call.dwFrom == 0 && call.dwTo == 0x106 && call.dwPar == 0 && temp.eID[0] == 12 && temp.oNum == 1);
    assert(is(18, OWNER, 0x777, 0, 0, 0) && is(19, UFADE, 1, 5, -1, 7) && is(20, CTLRATE, 1, 5, -1, 0) && is(22, OWNER, 0x777, 1, 0, 0));
    assert(is(23, NUM, 0x777, 0, 0, 0) && is(25, UFADE, 1, 7, -1, 7) && is(26, CTLRATE, 1, 7, -1, 0) && temp.wCount_2 == 0);
    /* units with status 4 and kind zero are collected and the pick is made from them */
    temp.wCount_2 = 17, ukind_zero = 1, temp.oNum = 0, temp.eID[0] = 0, do_answer = 13;
    log_n = 0;
    assert(Ta_ManageDestroyEffect(&in, &temp, &call) == 1 && log_n == 30 && is(7, CONV, 0, 1, 6, 0) && is(12, CONV, 6, 1, 7, 0));
    assert(is(14, NUM, 103, 0, 0, 0) && is(15, RANDOM, 103, 0, 0, 0) && temp.eID[0] == 13 && temp.oNum == 1 && temp.wCount_2 == 0);
    num_by_value = 0, owner_by_n = 0, gustat_zero_pos = -1, ukind_zero = 0;
}

static int swap_n, swap_args[4];
void Ta_SwapUnit(int a, int b, int c, int d) { swap_n++, swap_args[0] = a, swap_args[1] = b, swap_args[2] = c, swap_args[3] = d; }

/* Ta_Eff_032 (branch coverage). */
static void check_032(void)
{
    struct jp_6989c in = {0, 0, 0x777, 0};

    /* set-up call: one target focuses the camera on it, otherwise the standard view; two modules */
    memset(&temp, 0, sizeof(temp));
    num_answer = 1, owner_answer = 0x105;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, 4) == 1 && log_n == 8 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && is(2, NUM, 0x777, 0, 0, 0));
    assert(is(3, OWNER, 0x777, 0, 0, 0) && is(4, FOCUS, 0, 0x105, 100, 180) && is(5, FADE, 4, -1, 1, 1) && log_p[5] == &temp);
    assert(is(6, LOAD, 531, 0, 0, 0) && is(7, LOAD, 532, 0, 0, 0) && temp.wCount_1 == 128);
    num_answer = 2;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, 0) == 1 && log_n == 7 && is(3, STDVIEW, 0, 0, 0, 0) && is(4, FADE, 0, -1, 1, 1));
    /* stage 0: the level falls by 15 to 48, then waits for the modules and starts effect 531 */
    temp.wLvl = 0, temp.wCount_1 = 100;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 85, 0, 0, 0) && temp.wCount_1 == 85);
    temp.wCount_1 = 50;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    isload_answer = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0) && temp.wLvl == 0);
    isload_answer = 1, depend_call.eNO = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 4 && is(2, DEPEND, 531, 0, 0, 0) && log_p[2] == &temp && temp.wLvl == 1);
    assert(depend_call.eNO == 999 && depend_call.dwFrom == 0 && depend_call.dwTo == 0 && depend_call.dwPar == 0);
    /* stage 1: waits until no listed effect is in use */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 7, temp.ePos[0] = -1, temp.ePos[1] = 0x105;
    isuse_answer[5] = 0, isuse_answer[7] = 1;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 3 && is(1, ISUSE, 5, 0, 0, 0) && is(2, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
    /* a negative position is skipped; an empty destination takes the unit; effect 532 follows */
    isuse_answer[7] = 0, loc_by_unit = 0, loc_answer = 0x203, mapid_answer = -1, mapid_n = ld_n = move_n = swap_n = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 5 && is(3, LOC, 1, 5, 0, 0) && is(4, DEPEND, 532, 0, 0, 0) && temp.wLvl == 2);
    assert(mapid_n == 1 && mapid_args[0] == 2 && mapid_args[1] == 3 && move_n == 1 && ld_n == 0 && swap_n == 0);
    assert(move_args[0][0] == 1 && move_args[0][1] == 5 && move_args[0][2] == 2 && move_args[0][3] == 3);
    /* an occupant whose low byte is zero is left alone */
    temp.wLvl = 1, mapid_answer = 0x100, mapid_n = move_n = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 5 && mapid_n == 1 && move_n == 0 && ld_n == 0 && swap_n == 0 && temp.wLvl == 2);
    /* an occupant the range check accepts is not swapped */
    temp.wLvl = 1, mapid_answer = 0x107, ld_answer = 1, mapid_n = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && ld_n == 1 && swap_n == 0 && move_n == 0 && temp.ePos[1] == 0x105);
    assert(ld_args[0] == 1 && ld_args[1] == 7 && ld_args[2] == 2 && ld_args[3] == 3 && ld_args[4] == 11);
    /* otherwise the two units swap and the occupant's own entries are cleared, so it is not moved again */
    temp.wLvl = 1, temp.oNum = 3, temp.eID[2] = 5, temp.ePos[0] = 0x105, temp.ePos[1] = 0x107, temp.ePos[2] = 0x107;
    ld_answer = 0, mapid_n = ld_n = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 6 && is(4, LOC, 1, 5, 0, 0) && is(5, DEPEND, 532, 0, 0, 0) && temp.wLvl == 2);
    assert(mapid_n == 1 && ld_n == 1 && swap_n == 1 && swap_args[0] == 1 && swap_args[1] == 5 && swap_args[2] == 1 && swap_args[3] == 7);
    assert(temp.ePos[0] == 0x105 && temp.ePos[1] == -1 && temp.ePos[2] == -1 && move_n == 0);
    /* stage 2: the first call sets the camera, calls up to the 24th only count */
    temp.wCamTime = 0, num_answer = 1, owner_answer = 0x209;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 4 && is(1, NUM, 0x777, 0, 0, 0) && is(3, FOCUS, 30, 0x209, 100, 180) && temp.wCamTime == 1);
    temp.wCamTime = 0, num_answer = 3;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 3 && is(2, STDVIEW, 16, 0, 0, 0) && temp.wCamTime == 1);
    temp.wCamTime = 23;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 1 && temp.wCamTime == 24 && temp.wLvl == 2);
    /* from the 25th call: wait for the effects, then restore the view and fade back */
    isuse_answer[7] = 1;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 4 && is(2, ISUSE, 7, 0, 0, 0) && temp.wCamTime == 25 && temp.wLvl == 2);
    isuse_answer[7] = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -9) == 1 && log_n == 6 && is(4, RESTORE, 16, 0, 0, 0) && log_p[4] == &temp && is(5, FADE, -9, 1, 0, 1));
    assert(temp.wLvl == 3 && temp.wCamTime == 26);
    /* last stage: the level rises by 15 to 128, then 17 more calls, the fade and the window */
    temp.wCount_1 = 100, temp.wCamTime = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCount_1 == 128 && temp.wCamTime == 1);
    temp.wCamTime = 16;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 2 && temp.wCamTime == 17);
    finfade_answer = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == FINFADE && temp.wCamTime == 18);
    finfade_answer = 1, disp_answer = 1;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 0 && log_n == 6 && log_id[3] == WDISP && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
    disp_answer = 0;
    log_n = 0;
    assert(Ta_Eff_032(&in, &temp, -1) == 0 && log_n == 5 && log_id[3] == WDISP && log_id[4] == CLOSE);
    finfade_answer = 0, isload_answer = 0, mapid_n = ld_n = move_n = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { ANIM2 = 400, GRATEM, SGOALM, SRATEM };
static int ratem_answer;
int SzEffect_SetAnimIntr2(int packed, int value) { note(ANIM2, NULL, packed, value, 0, 0); return 0; }
int SzDuel_GetUnitCtlRateM(int a, int b) { note(GRATEM, NULL, a, b, 0, 0); return ratem_answer; }
int SzDuel_SetUnitCtlGoalM(int a, int b, int value) { note(SGOALM, NULL, a, b, value, 0); return 0; }
int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value) { note(SRATEM, NULL, a, b, pair, value); return 0; }

/* Ta_Eff_031 (branch coverage). */
static void check_031(void)
{
    struct jp_6989c in = {0, 0x10205, 0x777, 0x0304};
    int stage, r;

    /* set-up call: two modules, then the camera (one target: focus; otherwise standard view) */
    memset(&temp, 0, sizeof(temp));
    num_answer = 1, owner_answer = 0x105;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, 4) == 1 && log_n == 8 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && is(2, LOAD, 531, 0, 0, 0));
    assert(is(3, LOAD, 532, 0, 0, 0) && is(4, NUM, 0x777, 0, 0, 0) && is(5, OWNER, 0x777, 0, 0, 0) && is(6, FOCUS, 0, 0x105, 100, 180));
    assert(is(7, FADE, 4, -1, 1, 1) && log_p[7] == &temp && temp.wCount_1 == 128);
    num_answer = 2;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, 0) == 1 && log_n == 7 && is(5, STDVIEW, 0, 0, 0, 0) && is(6, FADE, 0, -1, 1, 1));
    /* stage 0: the level falls by 15 to 48 */
    temp.wLvl = 0, temp.wCount_1 = 100;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 85, 0, 0, 0));
    temp.wCount_1 = 50;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    /* then the modules are awaited and the loaded flag set */
    isload_answer = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && temp.eMod.fLoad == 0);
    isload_answer = 1;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && temp.eMod.fLoad != 0 && temp.wLvl == 0);
    /* with the flag set effect 531 starts on the low half of dwFrom; -3 changes nothing, -1 and -2 end
     * the sequence */
    do_answer = -3;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == DOEFF && log_p[1] == &temp && is(2, BLACK, 48, 0, 0, 0));
    assert(do_call.eNO == 531 && do_call.dwFrom == 0 && do_call.dwTo == 0x205 && do_call.dwPar == 0 && temp.oNum == 0 && temp.wLvl == 0);
    do_answer = -1;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 0 && log_n == 3 && log_id[2] == CLOSE && temp.wLvl == 0);
    do_answer = -2;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 0 && log_n == 3 && log_id[2] == CLOSE && temp.wLvl == 0);
    do_answer = 12;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && temp.eID[0] == 12 && temp.oNum == 1 && temp.wLvl == 1);
    /* stage 1: waits for the last effect to be in use with status 2 */
    isuse_answer[12] = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 12, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[12] = 1, status_once = 0, status_answer[12] = 1;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && is(2, STATUS, 12, 0, 0, 0) && temp.wLvl == 1);
    /* high half of dwFrom 1: busy flag and animation cleared; a zero rate is set up; the unit moves */
    status_answer[12] = 2, ratem_answer = 0, move_n = 0, temp.wCamTime = 9;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 8 && is(3, BUSY2, 0x205, 0, 0, 0) && is(4, ANIM2, 0x205, 0, 0, 0));
    assert(is(5, GRATEM, 2, 5, 0, 0) && is(6, SGOALM, 2, 5, 128, 0) && is(7, SRATEM, 2, 5, 1, 128) && move_n == 1);
    assert(move_args[0][0] == 2 && move_args[0][1] == 5 && move_args[0][2] == 4 && move_args[0][3] == 3);
    assert(temp.wCamTime == 0 && temp.wLvl == 2);
    temp.wLvl = 1, ratem_answer = 7, move_n = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 6 && is(5, GRATEM, 2, 5, 0, 0) && move_n == 1 && temp.wLvl == 2);
    temp.wLvl = 1, in.dwFrom = 0x20205, move_n = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && move_n == 1 && move_args[0][0] == 2 && move_args[0][1] == 5 && temp.wLvl == 2);
    /* stage 2: camera on the first call, counting up to the 24th */
    temp.wCamTime = 0, num_answer = 1, owner_answer = 0x209;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 4 && is(3, FOCUS, 30, 0x209, 100, 180) && temp.wCamTime == 1);
    temp.wCamTime = 0, num_answer = 3;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && is(2, STDVIEW, 16, 0, 0, 0) && temp.wCamTime == 1);
    temp.wCamTime = 23;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 1 && temp.wCamTime == 24);
    /* then, once the last effect is no longer in use, effect 532 with the same result handling */
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 12, 0, 0, 0) && temp.wCamTime == 25);
    isuse_answer[12] = 0, do_answer = -3;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && do_call.eNO == 532 && do_call.dwTo == 0x205);
    assert(do_call.dwFrom == 0 && do_call.dwPar == 0 && temp.oNum == 1 && temp.wLvl == 2);
    do_answer = -1;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 0 && log_n == 4 && log_id[3] == CLOSE);
    do_answer = -2;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 0 && log_n == 4 && log_id[3] == CLOSE && temp.wLvl == 2);
    do_answer = 13;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && temp.eID[1] == 13 && temp.oNum == 2 && temp.wLvl == 3);
    /* stage 3: waits for it, then restores the view and fades back */
    isuse_answer[13] = 1;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 13, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[13] = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -9) == 1 && log_n == 4 && is(2, RESTORE, 16, 0, 0, 0) && log_p[2] == &temp && is(3, FADE, -9, 1, 0, 1));
    assert(temp.wLvl == 4);
    /* last stage: the level rises by 15 to 128, then 17 more calls, the fade and the window */
    temp.wCount_1 = 100, temp.wCamTime = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCount_1 == 128 && temp.wCamTime == 1);
    temp.wCamTime = 16;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 2 && temp.wCamTime == 17);
    finfade_answer = 0;
    log_n = 0;
    assert(Ta_Eff_031(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == FINFADE);
    finfade_answer = 1;
    for (stage = 0; stage < 2; stage++) {
        disp_answer = !stage;
        log_n = 0;
        r = Ta_Eff_031(&in, &temp, -1);
        assert(r == 0 && log_n == 5 + disp_answer && log_id[3] == WDISP && log_id[log_n - 1] == CLOSE);
        assert(!disp_answer || is(4, WDRAW, 1, 0, 0, 0));
    }
    finfade_answer = 0, isload_answer = 0, do_answer = 0, status_answer[12] = 0, move_n = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { SEARCH = 300, REMOVE };
static int search_answer;
int SearchUnitByRange(int a, int b, int c, int d, int e, int f) { note(SEARCH, NULL, a * 100 + b * 10 + c, d, e, f); return search_answer; }
static int remove2_answer;
int Ta_RemoveObjFromFlagLst2(int word, int side) { note(REMOVE, NULL, word, side, 0, 0); return remove2_answer; }

/* Ta_Eff_028: branch coverage. */
static void check_028(void)
{
    struct jp_6989c in = { 0, 0, 0, 0 };
    int stage;

    /* a non-negative ID only prepares the effect */
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, 0) == 1 && log_n == 5 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, STDVIEW, 16, 0, 0, 0) && is(3, LOAD, 22, 0, 0, 0) && is(4, LOAD, 603, 0, 0, 0) && temp.wCount_1 == 128);
    /* stage 0: the level falls by 15 to 48, then waits for the module and starts effect 22 */
    temp.wCount_1 = 60;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    temp.wCount_1 = 49;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0));
    isload_answer = 0;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, BLACK, 48, 0, 0, 0) && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 4 && log_id[2] == DOEFF && log_p[2] == &temp && do_call.eNO == 22);
    assert(do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0 && temp.eID[0] == 9 && temp.oNum == 1 && temp.wLvl == 1);
    /* stage 1: waits for it, then clears curse and spell rests and resets AP/DP of each unit in range */
    isuse_answer[9] = 1;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0, search_answer = 0x30, in.dwFrom = 5, num_answer = 0;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 5 && is(2, SEARCH, 0, 3, 52, 14) && is(3, REMOVE, 0x30, 5, 0, 0));
    assert(is(4, NUM, 0x30, 0, 0, 0) && temp.wLvl == 2);
    temp.wLvl = 1, num_answer = 2, owner_by_n = 1, owner_answer = 0x105;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 17 && is(5, OWNER, 0x30, 0, 0, 0) && is(6, CURSE, 1, 5, 0, 0));
    assert(is(7, SPELL, 1, 5, 0, 0) && is(8, AP, 1, 5, 4, 0) && is(9, DP, 1, 5, 4, 0) && is(10, NUM, 0x30, 0, 0, 0));
    assert(is(11, OWNER, 0x30, 1, 0, 0) && is(12, CURSE, 1, 6, 0, 0) && is(15, DP, 1, 6, 4, 0) && temp.wLvl == 2);
    owner_by_n = 0;
    /* stage 2: effect 603 per set bit of dwTo (sides split at 11), then stage 3 in the same call */
    in.dwTo = (1u << 18) | (1u << 9), do_answer = 9, isuse_answer[9] = 1, temp.wCount_1 = 48;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 6 && log_id[1] == DOEFF && log_id[2] == DOEFF && is(5, ISUSE, 9, 0, 0, 0));
    assert(do_prev.eNO == 603 && do_prev.dwFrom == 0 && do_prev.dwTo == 3 && do_prev.dwPar == 1);
    assert(do_call.eNO == 603 && do_call.dwFrom == 0 && do_call.dwTo == 0x101 && do_call.dwPar == 1);
    assert(temp.oNum == 3 && temp.eID[1] == 9 && temp.eID[2] == 9 && temp.wCount_1 == 0 && temp.wLvl == 3);
    /* stage 3: waits for all of them, then deletes the units and restores the view */
    temp.eID[2] = 11, isuse_answer[9] = 0, isuse_answer[11] = 1, temp.wCamTime = 5;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 4 && is(3, ISUSE, 11, 0, 0, 0) && temp.wLvl == 3 && temp.wCamTime == 5);
    isuse_answer[11] = 0;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 7 && is(4, TDEL, 0, 3, 0, 0) && is(5, TDEL, 1, 1, 0, 0));
    assert(is(6, RESTORE, 16, 0, 0, 0) && log_p[6] == &temp && temp.wLvl == 4 && temp.wCamTime == 0);
    /* last stage: the level rises by 15 to 128, then 18 more calls and the window */
    temp.wCount_1 = 100;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCount_1 == 128 && temp.wCamTime == 1);
    temp.wCamTime = 16;
    log_n = 0;
    assert(Ta_Eff_028(&in, &temp, -1) == 1 && log_n == 2 && temp.wCamTime == 17);
    for (stage = 0; stage < 2; stage++) {
        disp_answer = !stage;
        log_n = 0;
        assert(Ta_Eff_028(&in, &temp, -1) == 0 && log_n == 4 + disp_answer && log_id[2] == WDISP && log_id[log_n - 1] == CLOSE);
        assert(!disp_answer || is(3, WDRAW, 1, 0, 0, 0));
    }
    isload_answer = 0, do_answer = 0, num_answer = 0, owner_answer = 0, search_answer = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { HCOUNT = 500, POPHAND, INQHAND, DELHAND, ADDGRAVE };
static int hcount_answer, pophand_answer, inq_answer[2][5];
int SzDuel_GetHandCount(int index) { note(HCOUNT, NULL, index, 0, 0, 0); return hcount_answer; }
int SzDuel_InqHand(int index, int position) { note(INQHAND, NULL, index, position, 0, 0); return inq_answer[index][position]; }
int SzDuel_DelHand(int index, int position) { note(DELHAND, NULL, index, position, 0, 0); return 0; }
int SzDuel_AddGrave(int index, int value) { note(ADDGRAVE, NULL, index, value, 0, 0); return 0; }
int TaEff_PopHand(struct jp_6989c *eInfo, struct jp_69942 *Temp, unsigned char dir, int x, unsigned char mode)
{
    note(POPHAND, eInfo, dir, x, mode, Temp == &temp);
    return pophand_answer;
}

/* Ta_Eff_014: branch coverage. */
static void check_014(void)
{
    struct jp_6989c in = { 0, 0, 0, 0 };
    int k;

    /* a non-negative ID only prepares the effect; module 611 only with a parameter */
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, 0) == 1 && log_n == 3 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && is(2, LOAD, 514, 0, 0, 0));
    in.dwPar = 1;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, 0) == 1 && log_n == 4 && is(3, LOAD, 611, 0, 0, 0));
    /* stage 0: waits for the module, then starts effect 514 */
    isload_answer = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && log_p[1] == &temp && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && log_p[2] == &temp && do_call.eNO == 514);
    assert(do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0 && temp.eID[0] == 9 && temp.wLvl == 1);
    /* stage 1: waits while the effect reports exactly 1 */
    isuse_answer[9] = 1;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    /* stage 1 with a parameter: the other side's hand; empty skips on, a refused pop waits */
    isuse_answer[9] = 0, hcount_answer = 0, in.dwTo = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 3 && is(2, HCOUNT, 1, 0, 0, 0) && temp.wLvl == 2);
    temp.wLvl = 1, hcount_answer = 2, pophand_answer = 0, in.dwTo = 1;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 4 && is(2, HCOUNT, 0, 0, 0, 0) && is(3, POPHAND, 0, 0, 1, 1));
    assert(log_p[3] == &in && temp.wLvl == 1);
    pophand_answer = 1, do_answer = 7, in.dwTo = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 5 && is(3, POPHAND, 0, 1, 1, 1) && log_id[4] == DOEFF && do_call.eNO == 611);
    assert(do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0 && temp.eID[0] == 7 && temp.wLvl == 2 && temp.ePos[0] == 0);
    in.dwTo = 1, temp.wLvl = 1, temp.eID[0] = 9;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && temp.ePos[0] == 1 && temp.wLvl == 2);
    /* stage 1 without one: an empty hand ends the effect */
    in.dwPar = 0, temp.wLvl = 1, temp.eID[0] = 9, hcount_answer = 0;
    for (k = 0; k < 2; k++) {
        disp_answer = !k;
        log_n = 0;
        assert(Ta_Eff_014(&in, &temp, -1) == 0 && log_n == 5 + disp_answer && is(2, HCOUNT, 1, 0, 0, 0) && log_id[3] == WDISP);
        assert(log_id[log_n - 1] == CLOSE && (!disp_answer || is(4, WDRAW, 1, 0, 0, 0)));
    }
    /* below 150 it pops with mode 1 and counts; from 150 with mode 0, ending when refused */
    hcount_answer = 3, temp.wCount_1 = 149, pophand_answer = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 4 && is(3, POPHAND, 1, 1, 1, 1) && temp.wCount_1 == 150);
    pophand_answer = 1;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 4 && is(3, POPHAND, 1, 1, 0, 1) && temp.wCount_1 == 150 && temp.wLvl == 1);
    pophand_answer = 0;
    for (k = 0; k < 2; k++) {
        disp_answer = !k;
        log_n = 0;
        assert(Ta_Eff_014(&in, &temp, -1) == 0 && log_n == 6 + disp_answer && log_id[4] == HANDCLOSE && log_id[5] == WDISP);
        assert(!disp_answer || is(6, WDRAW, 1, 0, 0, 0));
    }
    /* stage 2: while the effect runs the counter steps; leaving 6 closes the hand popup */
    temp.wLvl = 2, isuse_answer[9] = 1, temp.wCount_1 = 5, temp.fObj = 3, temp.wCount_2 = 4;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 2 && temp.wCount_1 == 6 && temp.fObj == 3 && temp.wCount_2 == 4);
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == HANDCLOSE && temp.wCount_1 == 7 && temp.fObj == 0 && temp.wCount_2 == 0);
    /* then: empty hand jumps to stage 16, a refused pop waits, an accepted one starts effect 611 */
    isuse_answer[9] = 0, hcount_answer = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 3 && is(2, HCOUNT, 1, 0, 0, 0) && temp.wCount_1 == 0 && temp.wLvl == 16);
    temp.wLvl = 2, hcount_answer = 1, temp.wCount_1 = 3;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 4 && is(3, POPHAND, 1, 1, 1, 1) && temp.wCount_1 == 0 && temp.wLvl == 2);
    pophand_answer = 1, do_answer = 9, temp.ePos[0] = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 5 && log_id[4] == DOEFF && do_call.eNO == 611 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 1 && temp.wLvl == 3);
    /* stage 3: the same wait without the clears, then the last stage */
    isuse_answer[9] = 1, temp.wCount_1 = 5, temp.fObj = 3;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 2 && temp.wCount_1 == 6 && temp.wLvl == 3);
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == HANDCLOSE && temp.wCount_1 == 7 && temp.fObj == 3);
    isuse_answer[9] = 0;
    log_n = 0;
    assert(Ta_Eff_014(&in, &temp, -1) == 1 && log_n == 2 && temp.wLvl == 4);
    /* last stage: every hand card of both sides other than 999 (low 12 bits) goes to the grave */
    for (k = 0; k < 5; k++) {
        inq_answer[0][k] = 999, inq_answer[1][k] = 0x1000 | 999;
    }
    inq_answer[0][4] = 0x2005, inq_answer[1][0] = 0;
    for (k = 0; k < 2; k++) {
        disp_answer = !k;
        log_n = 0;
        assert(Ta_Eff_014(&in, &temp, -1) == 0 && log_n == 17 + disp_answer && is(1, INQHAND, 0, 0, 0, 0) && is(5, INQHAND, 0, 4, 0, 0));
        assert(is(6, DELHAND, 0, 4, 0, 0) && is(7, ADDGRAVE, 0, 5, 0, 0) && is(8, INQHAND, 1, 0, 0, 0) && is(9, DELHAND, 1, 0, 0, 0));
        assert(is(10, ADDGRAVE, 1, 0, 0, 0) && is(14, INQHAND, 1, 4, 0, 0) && log_id[15] == WDISP && log_id[log_n - 1] == CLOSE);
        assert(!disp_answer || is(16, WDRAW, 1, 0, 0, 0));
    }
    isload_answer = 0, do_answer = 0, hcount_answer = 0, pophand_answer = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { SGOALC = 600, SRATEC, GGOALM, GCURSE, GSPELL };
static int goalm_answer, curse_answer, spell_answer;
int SzDuel_SetUnitCtlGoalC(int a, int b, int value) { note(SGOALC, NULL, a, b, value, 0); return 0; }
int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value) { note(SRATEC, NULL, a, b, pair, value); return 0; }
int SzDuel_GetUnitCtlGoalM(int a, int b) { note(GGOALM, NULL, a, b, 0, 0); return goalm_answer; }
int SzDuel_GetUnitCurseRest(int a, int b) { note(GCURSE, NULL, a, b, 0, 0); return curse_answer; }
int SzDuel_GetUnitSpellRest(int a, int b) { note(GSPELL, NULL, a, b, 0, 0); return spell_answer; }

/* Ta_Eff_067: branch coverage. */
static void check_067(void)
{
    struct jp_6989c in = { 0, 0x203, 0x30, 0 };
    int k;

    /* a non-negative ID prepares: without a parameter the fade and the camera, with one two more modules */
    num_answer = 1, owner_answer = 0x105;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, 0) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && is(2, LOAD, 513, 0, 0, 0));
    assert(is(3, FADE, 0, -1, 1, 1) && log_p[3] == &temp && is(4, NUM, 0x30, 0, 0, 0) && is(5, OWNER, 0x30, 0, 0, 0));
    assert(is(6, FOCUS, 30, 0x105, 100, 180));
    num_answer = 2;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, 0) == 1 && log_n == 6 && is(5, STDVIEW, 30, 0, 0, 0));
    in.dwPar = 1;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, 0) == 1 && log_n == 5 && is(3, LOAD, 610, 0, 0, 0) && is(4, LOAD, 572, 0, 0, 0));
    /* stage 0: waits for the module; without a parameter 31 calls, then the popup and the list effect */
    isload_answer = 0;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, in.dwPar = 0, temp.wCamTime = 29;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 2 && temp.wCamTime == 30 && temp.wLvl == 0);
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 275) && is(3, DEPEND, 513, 0, 0, 0));
    assert(log_p[3] == &temp && depend_call.eNO == 999 && temp.wCamTime == 31 && temp.wLvl == 1);
    /* with one it starts effect 513 on dwFrom at once */
    in.dwPar = 1, temp.wLvl = 0, do_answer = 9;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && do_call.eNO == 513 && do_call.dwFrom == 0);
    assert(do_call.dwTo == 0x203 && do_call.dwPar == 0 && temp.eID[0] == 9 && temp.oNum == 1 && temp.wLvl == 1);
    /* stage 1: waits for every effect; parameter 1 then waits for the control rate to reach its goal */
    isuse_answer[9] = 1;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0, ratem_answer = 5, goalm_answer = 6;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 4 && is(2, GRATEM, 2, 3, 0, 0) && is(3, GGOALM, 2, 3, 0, 0) && temp.wLvl == 1);
    goalm_answer = 5, do_answer = 7;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 6 && is(4, POPUP, 0, -120, 80, 274) && log_id[5] == DOEFF);
    assert(do_call.eNO == 610 && do_call.dwFrom == 0x203 && do_call.dwTo == 0x30 && do_call.dwPar == 0);
    assert(temp.eID[1] == 7 && temp.oNum == 2 && temp.wLvl == 2);
    /* any other parameter (and no effects to wait for) goes to stage 4 and fades back */
    in.dwPar = 0, temp.wLvl = 1, temp.oNum = 0, temp.wCamTime = 9;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -5) == 1 && log_n == 4 && log_id[1] == CLOSE && is(2, RESTORE, 16, 0, 0, 0));
    assert(is(3, FADE, -5, 1, 0, 1) && log_p[3] == &temp && temp.wLvl == 4 && temp.wCamTime == 0);
    /* stage 2: waits while the last effect reports exactly 1, then fades the unit and clears its control */
    in.dwPar = 1, temp.wLvl = 2, temp.oNum = 2, isuse_answer[7] = 1;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[7] = 0;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 7 && is(2, BUSY2, 0x203, 0, 0, 0) && is(3, UFADE, 2, 3, -1, 1));
    assert(is(4, SGOALC, 2, 3, 0, 0) && is(5, SRATEC, 2, 3, -1, 0) && is(6, UFADE, 2, 3, -1, 1) && temp.wLvl == 3);
    /* stage 3: after the fade deletes the unit, flips status 1 of the target and clears status 0 when
     * no rests remain */
    in.dwTo = 0x104, finfade_answer = 0;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 2 && log_id[1] == FINFADE && temp.wLvl == 3);
    finfade_answer = 1, curse_answer = 1, temp.wCamTime = 9;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 8 && is(2, DELUNIT, 2, 3, 0, 0) && is(3, GUSTAT, 1, 4, 1, 0));
    assert(is(4, USTAT, 1, 4, 1, 0) && is(5, GCURSE, 1, 4, 0, 0) && is(6, RESTORE, 16, 0, 0, 0) && log_id[7] == CLOSE);
    assert(temp.wLvl == 4 && temp.wCamTime == 0);
    temp.wLvl = 3, gustat_zero_pos = 4, curse_answer = 0, spell_answer = 1;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 9 && is(4, USTAT, 1, 4, 1, 1) && is(6, GSPELL, 1, 4, 0, 0) && log_id[7] == RESTORE);
    temp.wLvl = 3, spell_answer = 0;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 10 && is(7, USTAT, 1, 4, 0, 0) && log_id[8] == RESTORE && temp.wLvl == 4);
    /* last stage: 18 calls, then the finished fade, then the window */
    temp.wCamTime = 16;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    finfade_answer = 0;
    log_n = 0;
    assert(Ta_Eff_067(&in, &temp, -1) == 1 && log_n == 2 && log_id[1] == FINFADE && temp.wCamTime == 18);
    finfade_answer = 1;
    for (k = 0; k < 2; k++) {
        disp_answer = !k;
        log_n = 0;
        assert(Ta_Eff_067(&in, &temp, -1) == 0 && log_n == 4 + disp_answer && log_id[2] == WDISP && log_id[log_n - 1] == CLOSE);
        assert(!disp_answer || is(3, WDRAW, 1, 0, 0, 0));
    }
    isload_answer = 0, do_answer = 0, num_answer = 0, owner_answer = 0, finfade_answer = 0, gustat_zero_pos = -1, ratem_answer = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { TOWNER = 700, LDABL, RANGE, DEPEND2, DEPEND3 };
static int towner_answer, ldabl_answer, range_tail[3];
int SzDuel_GetTurnOwner(void) { note(TOWNER, NULL, 0, 0, 0, 0); return towner_answer; }
int Ta_IsLDAbl(int a, int b) { note(LDABL, NULL, a, b, 0, 0); return ldabl_answer; }
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int kind)
{
    range_tail[0] = e, range_tail[1] = mode, range_tail[2] = kind;
    note(RANGE, NULL, x, y, c, reach);
}
void Ta_SetEffectDependOnFlagLst2(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall, int eNO)
{
    depend_call = *pCall;
    note(DEPEND2, eInfo, eNO, 0, 0, 0);
    (void)Temp;
}
void Ta_SetEffectDependOnFlagLst3(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall, int eNO)
{
    depend_call = *pCall;
    note(DEPEND3, eInfo, eNO, 0, 0, 0);
    (void)Temp;
}

static void check_113(void)
{
    struct jp_6989c in = {113, 0x400003, 0, 0x80000000};
    int saved_by_unit = loc_by_unit;

    memset(&temp, 0, sizeof(temp));
    loc_by_unit = 0, loc_answer = 0x305, ldabl_answer = 6, isload_answer = 0;
    /* start: negative dwPar picks the other player; dwFrom's low 22 bits pick range kind 16 and module 603 */
    towner_answer = 0;
    assert(step(Ta_Eff_113, &in, 0) == 1 && log_n == 10 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_id[2] == TOWNER);
    assert(temp.wCount_1 == 1 && is(3, LOC, 1, 0, 0, 0) && is(4, FOCUS, 0, 0x100, 300, 300) && is(5, POPUP, 0, -120, 80, 270));
    assert(is(6, LDABL, 9, 1, 0, 0) && is(7, RANGE, 5, 3, 0, 6) && range_tail[0] == 1 && range_tail[1] == 1 && range_tail[2] == 16);
    assert(is(8, LOAD, 613, 0, 0, 0) && is(9, LOAD, 603, 0, 0, 0));
    towner_answer = 1;
    assert(step(Ta_Eff_113, &in, 0) == 1 && temp.wCount_1 == 0);
    /* non-negative dwPar: the turn owner itself; dwTo picks kind 15 and module 500 */
    in.dwFrom = 0x400000, in.dwTo = 8, in.dwPar = 0;
    assert(step(Ta_Eff_113, &in, 0) == 1 && log_n == 10 && temp.wCount_1 == 1 && is(7, RANGE, 5, 3, 0, 6) && range_tail[2] == 15);
    assert(is(8, LOAD, 613, 0, 0, 0) && is(9, LOAD, 500, 0, 0, 0));
    /* dwPar's low 22 bits pick kind 13 and module 513 */
    in.dwTo = 0, in.dwPar = 0x400010;
    assert(step(Ta_Eff_113, &in, 0) == 1 && log_n == 10 && range_tail[2] == 13 && is(9, LOAD, 513, 0, 0, 0));
    /* none of the three: no range call, only module 613 */
    in.dwPar = 0x400000;
    assert(step(Ta_Eff_113, &in, 0) == 1 && log_n == 7 && is(6, LOAD, 613, 0, 0, 0));
    /* all three: one range call (kind 16), four modules */
    in.dwFrom = 1, in.dwTo = 2, in.dwPar = 4;
    assert(step(Ta_Eff_113, &in, 0) == 1 && log_n == 12 && range_tail[2] == 16 && is(10, LOAD, 500, 0, 0, 0) && is(11, LOAD, 513, 0, 0, 0));
    /* level 0 waits for the module, then starts effect 613 at the player */
    temp.wLvl = 0, temp.wCount_1 = 1;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9, temp.ePos[0] = 77;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 6 && is(2, LOC, 1, 0, 0, 0) && is(3, LDABL, 9, 1, 0, 0) && is(4, RANGE, 5, 3, 0, 6));
    assert(range_tail[0] == 0 && range_tail[1] == -1 && range_tail[2] == 16 && log_id[5] == DOEFF && log_p[5] == &temp);
    assert(do_call.eNO == 613 && do_call.dwFrom == 0 && do_call.dwTo == 0x100 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0 && temp.wLvl == 1);
    /* level 1 waits for that effect to end */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 3 && is(2, STDVIEW, 16, 0, 0, 0) && temp.wLvl == 2);
    /* level 2: counter below 16 only counts; at 16 the three dependent effects by list */
    temp.wCamTime = 15;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 16 && temp.wLvl == 2);
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 4 && is(1, DEPEND3, 603, 0, 0, 0) && is(2, DEPEND, 500, 0, 0, 0) && is(3, DEPEND2, 513, 0, 0, 0));
    assert(log_p[1] == &in && log_p[2] == &temp && depend_call.eNO == 999 && temp.wCamTime == 17 && temp.wLvl == 3);
    temp.wLvl = 2, in.dwFrom = 0x400000, in.dwTo = 0, in.dwPar = 0x400000;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 1 && temp.wLvl == 3);
    /* level 3: advances when none of the oNum effects is in use */
    temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9, isuse_answer[5] = 0, isuse_answer[9] = 1;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 5, 0, 0, 0) && is(2, ISUSE, 9, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 3 && temp.wLvl == 4);
    temp.oNum = 0, temp.wLvl = 3;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 1 && temp.wLvl == 4);
    /* level 4: delete dwFrom's units, mark dwTo's, open dwPar's */
    in.dwFrom = 0x400003, in.dwTo = 8, in.dwPar = 0x400010;
    num_answer = 1, num_by_value = 0, owner_by_n = 0, owner_answer = 0x10a, temp.wCamTime = 5;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 13 && is(1, NUM, 3, 0, 0, 0) && is(2, OWNER, 3, 0, 0, 0) && is(3, TDEL, 1, 10, 0, 0));
    assert(is(4, NUM, 8, 0, 0, 0) && is(5, OWNER, 8, 0, 0, 0) && is(6, SPELL, 1, 10, 255, 0) && is(7, USTAT, 1, 10, 0, 1));
    assert(is(8, NUM, 0x10, 0, 0, 0) && is(9, OWNER, 0x10, 0, 0, 0) && is(10, DOOPEN, 1, 10, 1, 0) && log_id[11] == CLOSE);
    assert(is(12, RESTORE, 16, 0, 0, 0) && log_p[12] == &temp && temp.wCamTime == 0 && temp.wLvl == 5);
    num_answer = 0, temp.wLvl = 4;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 6 && log_id[4] == CLOSE && temp.wLvl == 5);
    num_answer = 2, temp.wLvl = 4;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 20 && is(3, TDEL, 1, 10, 0, 0) && is(4, OWNER, 3, 1, 0, 0) && is(16, OWNER, 0x10, 1, 0, 0));
    /* past level 4: counter up to 16 only counts, then the window is redrawn if shown and 0 is returned */
    temp.wCamTime = 16;
    assert(step(Ta_Eff_113, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17 && temp.wLvl == 5);
    for (disp_answer = 0; disp_answer < 2; disp_answer++) {
        temp.wCamTime = 17;
        assert(step(Ta_Eff_113, &in, -1) == 0 && log_n == 3 + disp_answer && log_id[1] == WDISP && log_id[log_n - 1] == CLOSE);
        assert(!disp_answer || is(2, WDRAW, 1, 0, 0, 0));
    }
    disp_answer = 0, loc_by_unit = saved_by_unit;
}

enum { FOCUS2 = 800, FIELD };
static int field_answer;
void TaView_ReqFocusCamera2(unsigned char b, int cell, int value) { note(FOCUS2, NULL, b, cell, value, 0); }
int SzDuel_GetField(int x, int y) { note(FIELD, NULL, x, y, 0, 0); return field_answer; }

static void check_002(void)
{
    struct jp_6989c in = {2, 0x302, 0, 8};

    /* start: one cell in reach gets the focus camera, dwPar 8 loads module 503 */
    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_002, &in, 4) == 1 && log_n == 4 && log_id[0] == INIT && is(1, FADE2, 4, -1, 1, 3) && log_p[1] == &temp);
    assert(is(2, FOCUS2, 0, 0x302, 180, 0) && is(3, LOAD, 503, 0, 0, 0));
    /* reach 1: five cells, the standard view, and module 502 for any other dwPar */
    in.dwTo = 1, in.dwPar = 7;
    assert(step(Ta_Eff_002, &in, 0) == 1 && log_n == 4 && is(2, STDVIEW, 30, 0, 0, 0) && is(3, LOAD, 502, 0, 0, 0));
    /* level 0 waits for the fade, then for the module */
    finfade_answer = 0, isload_answer = 1;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 1 && log_id[0] == FINFADE && temp.wLvl == 0);
    finfade_answer = 1, isload_answer = 0;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 2 && log_id[1] == ISLOAD && temp.wLvl == 0);
    /* dwPar 8, one cell: effect 503 with dwFrom 1 at the cell, recorded in eID/ePos */
    isload_answer = 1, do_answer = 9, in.dwTo = 0, in.dwPar = 8;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 503 && do_call.dwFrom == 1 && do_call.dwTo == 0x302 && do_call.dwPar == 8);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0x302 && temp.oNum == 1 && temp.wLvl == 1);
    /* reach 1: five cells in row order; later ones carry dwFrom 0 */
    memset(&temp, 0, sizeof(temp));
    in.dwTo = 1;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 7 && do_call.eNO == 503 && do_call.dwFrom == 0 && do_call.dwTo == 0x303);
    assert(do_prev.dwTo == 0x402 && temp.oNum == 5 && temp.ePos[0] == 0x301 && temp.ePos[1] == 0x202 && temp.ePos[4] == 0x303 && temp.eID[4] == 9);
    /* reach 2 at a corner stays inside the 7x7 board: six cells */
    memset(&temp, 0, sizeof(temp));
    in.dwFrom = 0x600, in.dwTo = 2;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 8 && temp.oNum == 6 && temp.ePos[0] == 0x400 && temp.ePos[5] == 0x602);
    /* reach 16 takes the seven cells of dwFrom's column instead */
    memset(&temp, 0, sizeof(temp));
    in.dwFrom = 0x302, in.dwTo = 16;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 9 && temp.oNum == 7 && temp.ePos[0] == 0x300 && temp.ePos[6] == 0x306 && do_call.dwTo == 0x306);
    /* other dwPar: effect 502 unless the cell's field is 8, which leaves the call block as it was */
    memset(&temp, 0, sizeof(temp));
    in.dwTo = 0, in.dwPar = 7, field_answer = 3;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 4 && is(2, FIELD, 2, 3, 0, 0) && log_id[3] == DOEFF);
    assert(do_call.eNO == 502 && do_call.dwFrom == 1 && do_call.dwTo == 0x302 && do_call.dwPar == 7 && temp.oNum == 1);
    memset(&temp, 0, sizeof(temp));
    field_answer = 8;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 4 && do_call.eNO == 999 && do_call.dwTo == 0 && temp.oNum == 1 && temp.ePos[0] == 0x302);
    memset(&temp, 0, sizeof(temp));
    in.dwTo = 1;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 12 && is(10, FIELD, 3, 3, 0, 0) && do_call.eNO == 999 && temp.oNum == 5);
    memset(&temp, 0, sizeof(temp));
    field_answer = 3;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 12 && do_call.eNO == 502 && do_call.dwFrom == 0 && do_call.dwTo == 0x303 && do_call.dwPar == 7);
    /* level 1: an effect in use with status 2 has its position's low half set to 0xffff once */
    temp.wLvl = 1, temp.oNum = 2, temp.eID[0] = 5, temp.eID[1] = 9, temp.ePos[1] = 0x302;
    isuse_answer[5] = 0, isuse_answer[9] = 1, status_answer[9] = 1, status_once = 0;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 3 && is(0, ISUSE, 5, 0, 0, 0) && is(1, ISUSE, 9, 0, 0, 0) && is(2, STATUS, 9, 0, 0, 0));
    assert(temp.ePos[1] == 0x302 && temp.wLvl == 1);
    status_answer[9] = 2;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 3 && temp.ePos[1] == 0xffff && temp.wLvl == 1);
    temp.ePos[1] = 0x1ffff;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 3 && temp.ePos[1] == 0x1ffff && temp.wLvl == 1);
    /* all ended: next level, view restored, fade back */
    isuse_answer[9] = 0, status_answer[9] = 0;
    assert(step(Ta_Eff_002, &in, -6) == 1 && log_n == 4 && is(2, RESTORE, 30, 0, 0, 0) && is(3, FADE2, -6, 1, 1, 3) && temp.wLvl == 2);
    /* past level 1: counts to 31, then waits for the fade and returns 0 */
    temp.wCamTime = 30;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 0 && temp.wCamTime == 31);
    finfade_answer = 0;
    assert(step(Ta_Eff_002, &in, -1) == 1 && log_n == 1 && log_id[0] == FINFADE && temp.wCamTime == 32);
    finfade_answer = 1;
    assert(step(Ta_Eff_002, &in, -1) == 0 && log_n == 2 && log_id[1] == CLOSE);
    finfade_answer = 0, isload_answer = 0, do_answer = 0;
}

enum { TRAPB = 900, CURSIDE };
static int trap_seq[4], trap_n;
int Ta_IsActiveTrapB(int a, int b) { note(TRAPB, NULL, a, b, 0, 0); return trap_seq[trap_n++ & 3]; }
static int curside_answer, curside_lvl;
int Ta_GetCurrentSide(int a, int b)
{
    note(CURSIDE, NULL, a, b, 0, 0);
    if (curside_lvl) {
        temp.wLvl = (unsigned short)curside_lvl;
    }
    return curside_answer ? curside_answer : 6;
}

/* Ta_Eff_004 (branch coverage). */
static void check_004(void)
{
    struct jp_6989c in = {4, 1, 100, 0};
    int k;

    memset(isuse_answer, 0, sizeof(isuse_answer));
    /* start, no trap on either side: level 2, only module 21 */
    memset(&temp, 0, sizeof(temp));
    trap_seq[0] = -1, trap_seq[1] = -1, trap_n = 0;
    assert(step(Ta_Eff_004, &in, 0) == 1 && log_n == 5 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, TRAPB, 1, 45, 0, 0) && is(3, TRAPB, 0, 45, 0, 0) && is(4, LOAD, 21, 0, 0, 0));
    assert(temp.wCount_1 == 0xFF && temp.wCount_2 == 0xFF && temp.wLvl == 2);
    /* both traps on both sides: message 825, the second trap opened and kept, module 602 */
    in.dwFrom = 0;
    trap_seq[0] = 0x203, trap_seq[1] = 0x104, trap_seq[2] = 0x305, trap_seq[3] = 0x406, trap_n = 0;
    assert(step(Ta_Eff_004, &in, 0) == 1 && log_n == 15 && is(2, TRAPB, 0, 45, 0, 0) && is(3, TRAPB, 0, 46, 0, 0));
    assert(is(4, MSG, 825, 0, 0, 0) && is(5, POPUP, 0, -120, 80, 0x4D5347) && is(6, DOOPEN, 1, 4, 1, 0) && is(7, LOAD, 602, 0, 0, 0));
    assert(is(8, TRAPB, 1, 45, 0, 0) && is(9, TRAPB, 0, 46, 0, 0) && is(10, MSG, 825, 0, 0, 0) && is(12, DOOPEN, 4, 6, 1, 0));
    assert(is(13, LOAD, 602, 0, 0, 0) && is(14, LOAD, 21, 0, 0, 0) && temp.wCount_2 == 0x104 && temp.wCount_1 == 0x406 && temp.wLvl == 0);
    assert(in.dwTo == 100);
    /* only trap 45: message 824, eleven units of its side cleared from dwTo, the first trap opened
     * and kept */
    trap_seq[0] = 0x203, trap_seq[1] = -1, trap_seq[2] = 0x305, trap_seq[3] = -1, trap_n = 0;
    assert(step(Ta_Eff_004, &in, 0) == 1 && log_n == 59 && is(4, MSG, 824, 0, 0, 0) && is(5, POPUP, 0, -120, 80, 0x4D5347));
    assert(is(6, CURSIDE, 2, 3, 0, 0) && is(7, CONVOFF, 100, 6, 0, 0) && is(27, CONVOFF, 90, 6, 10, 0) && is(28, DOOPEN, 2, 3, 1, 0));
    assert(is(29, LOAD, 602, 0, 0, 0) && is(32, MSG, 824, 0, 0, 0) && is(34, CURSIDE, 3, 5, 0, 0) && is(55, CONVOFF, 79, 6, 10, 0));
    assert(is(56, DOOPEN, 3, 5, 1, 0) && is(58, LOAD, 21, 0, 0, 0) && in.dwTo == 78 && temp.wCount_2 == 0x203 && temp.wCount_1 == 0x305 && temp.wLvl == 0);
    /* only the other side has a trap: level 0 stays, the first side adds nothing */
    trap_seq[0] = -1, trap_seq[1] = 0x305, trap_seq[2] = 0x406, trap_n = 0;
    assert(step(Ta_Eff_004, &in, 0) == 1 && log_n == 10 && temp.wCount_2 == 0xFF && temp.wCount_1 == 0x406 && temp.wLvl == 0);

    /* level 0: nothing kept, nothing done (camera level 1 is idle) */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = temp.wCount_2 = 0xFF, temp.bCamLvl = 1;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1 && is(0, WDRAW, 0, 0, 0, 0));
    /* the kept trap is not fully open yet */
    temp.wCount_2 = 0x104, openrate_answer = 0x80;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && is(1, OPENRATE, 1, 4, 0, 0) && temp.wLvl == 0);
    /* open, module not loaded yet; both kept traps are asked */
    temp.wCount_1 = 0x406, openrate_answer = 0xFF, isload_answer = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 5 && log_id[2] == ISLOAD && is(3, OPENRATE, 4, 6, 0, 0) && log_id[4] == ISLOAD && temp.wLvl == 0);
    /* loaded: effect 602 for each, ids recorded, level 1 */
    isload_answer = 1, do_answer = 7;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 7 && log_id[3] == DOEFF && log_id[6] == DOEFF && log_p[6] == &temp);
    assert(do_prev.eNO == 602 && do_prev.dwFrom == 0 && do_prev.dwTo == 0x104 && do_prev.dwPar == 0 && do_call.eNO == 602 && do_call.dwTo == 0x406);
    assert(temp.eID[0] == 7 && temp.eID[1] == 7 && temp.oNum == 2 && temp.wLvl == 1);
    /* a negative id is dropped for the first trap but stored for the second */
    temp.oNum = 0, temp.wLvl = 0, do_answer = -1;
    assert(step(Ta_Eff_004, &in, -1) == 1 && temp.oNum == 1 && temp.eID[0] == 0xFF && temp.wLvl == 1);

    /* level 1: waits until no recorded effect is in use */
    temp.oNum = 2, temp.eID[0] = 3, temp.eID[1] = 4, isuse_answer[3] = 1;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 4 && is(1, ISUSE, 3, 0, 0, 0) && is(2, ISUSE, 4, 0, 0, 0) && is(3, USTAT, 4, 6, 0, 1) && temp.wLvl == 1);
    isuse_answer[3] = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 5 && log_id[4] == CLOSE && temp.wLvl == 2);

    /* level 2: camera level forced to 2 (standard view 20), waits for the module */
    isload_answer = 0, temp.eID[0] = 3;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && is(2, STDVIEW, 20, 0, 0, 0) && temp.bCamLvl == 3 && temp.wLvl == 2);
    /* loaded, negative id: eID[0] kept, level 3 */
    isload_answer = 1, do_answer = -1;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 4 && log_id[2] == DOEFF && is(3, STDVIEW, 20, 0, 0, 0) && temp.eID[0] == 3 && temp.wLvl == 3);
    assert(do_call.eNO == 21 && do_call.dwFrom == 0 && do_call.dwTo == 78 && do_call.dwPar == 0);
    temp.wLvl = 2, do_answer = 5;
    assert(step(Ta_Eff_004, &in, -1) == 1 && temp.eID[0] == 5 && temp.wLvl == 3 && temp.bCamLvl == 3);

    /* level 3, effect in use: sound 73 every 28th step of fObj, which wraps at 28 */
    temp.bCamLvl = 1, isuse_answer[5] = 1, temp.fObj = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 5, 0, 0, 0) && is(2, SE, 1, 73, 0, 0) && temp.fObj == 1);
    temp.fObj = 26;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && temp.fObj == 27);
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && temp.fObj == 0);
    temp.fObj = 28;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 3 && log_id[2] == SE && temp.fObj == 0);
    /* effect over: flagged units deleted (bit 21 - n; side 1 from n = 11), all 22 busy flags cleared */
    isuse_answer[5] = 0, in.dwTo = (1 << 21) | (1 << 11) | (1 << 10) | 1;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 28 && is(2, TDEL, 0, 0, 0, 0) && is(3, TDEL, 0, 10, 0, 0) && is(4, TDEL, 1, 0, 0, 0) && is(5, TDEL, 1, 10, 0, 0));
    assert(is(6, BUSY2, 0, 0, 0, 0) && is(7, BUSY2, 0x100, 0, 0, 0) && is(26, BUSY2, 10, 0, 0, 0) && is(27, BUSY2, 0x10A, 0, 0, 0) && temp.wLvl == 3);

    /* camera level 0: focus on the trap kept in wCount_1, else the one in wCount_2 */
    temp.wLvl = 0, temp.wCount_1 = temp.wCount_2 = 0xFF, temp.bCamLvl = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && is(1, FOCUS, 0, 0xFF, 180, 180) && temp.bCamLvl == 1);
    temp.wLvl = 4, temp.wCount_1 = 0x406, temp.wCount_2 = 0x104, temp.bCamLvl = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && is(1, FOCUS, 0, 0x406, 180, 180) && temp.bCamLvl == 1);
    temp.wCount_1 = 0xFF, temp.bCamLvl = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && is(1, FOCUS, 0, 0x104, 180, 180));
    /* camera level 3: waits past 20 steps */
    temp.bCamLvl = 3, temp.wCamTime = 19;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 20 && temp.bCamLvl == 3);
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 0 && temp.bCamLvl == 4);
    /* camera level 4, sequence unfinished and flags left: rotate at step 0, counter wraps at 60 */
    temp.wLvl = 0, temp.wCount_2 = 0xFF;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && is(1, ROT, 60, 16383, 0, 0) && temp.wCamTime == 1 && temp.bCamLvl == 4);
    temp.wCamTime = 58;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 59);
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 0 && temp.bCamLvl == 4);
    /* no flags left: the view is restored */
    k = in.dwTo, in.dwTo = 0, temp.wCamTime = 9;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && is(1, RESTORE, 16, 0, 0, 0) && log_p[1] == &temp && temp.wCamTime == 0 && temp.bCamLvl == 5);
    /* sequence finished (level past 3): restored too */
    in.dwTo = k, temp.wLvl = 4, temp.bCamLvl = 4;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 2 && log_id[1] == RESTORE && temp.bCamLvl == 5);
    /* camera done: 16 more steps, then both conditions end the sequence */
    temp.wCamTime = 15;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 16);
    disp_answer = 0;
    assert(step(Ta_Eff_004, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE && temp.wCamTime == 17);
    disp_answer = 1;
    assert(step(Ta_Eff_004, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    /* camera done but the sequence is not */
    temp.wLvl = 0;
    assert(step(Ta_Eff_004, &in, -1) == 1 && log_n == 1);
    disp_answer = 0, isload_answer = 0, do_answer = 0, openrate_answer = 0;
}

enum { SKY = 1000, LDRANGE, RESIST };
static int ldrange_answer, resist_answer;
int Ta_IsLDRangeAvility(int a, int b, int c) { note(LDRANGE, NULL, a, b, c, 0); return ldrange_answer; }
int Ta_IsResistAll(int a, int b) { note(RESIST, NULL, a, b, 0, 0); return resist_answer; }
void TaView_ReqSkyView(void) { note(SKY, NULL, 0, 0, 0, 0); }

static int step_025(struct jp_6989c *in)
{
    temp.bCamLvl = 2, temp.wCamTime = 0, temp.wLvl = 1;
    mapid_n = 0;
    return step(Ta_Eff_025, in, -1);
}
/* Ta_Eff_025 (branch coverage). */
static void check_025(void)
{
    struct jp_6989c in = {25, 0, 0, 0};

    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    status_once = 0, loc_by_unit = 0, loc_answer = 0x302, ldabl_answer = 4, disp_answer = 0, isload_answer = 0;
    /* start: ten random cells, both modules requested */
    rand_n = 0, rand_vals[0] = 8, rand_vals[1] = 3, rand_vals[2] = 15, rand_vals[3] = 6;
    assert(step(Ta_Eff_025, &in, 0) == 1 && log_n == 4);
    assert(is(0, WDRAW, 0, 0, 0, 0) && is(1, INIT, 0, 0, 0, 0) && log_p[1] == &temp);
    assert(is(2, LOAD, 525, 0, 0, 0) && is(3, LOAD, 603, 0, 0, 0));
    assert(temp.ePos[0] == 0x103 && temp.ePos[1] == 0x106 && temp.ePos[9] == 0x106 && temp.ePos[10] == 0);
    assert(temp.wCount_1 == 128);

    /* camera stage 0: waits for the sequence to leave stage 0 */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 85, 0, 0, 0));
    assert(temp.wCamTime == 0 && temp.wCount_1 == 85);
    temp.wLvl = 2, temp.wCount_1 = 0;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 3 && is(1, STDVIEW, 16, 0, 0, 0) && is(2, BLACK, 15, 0, 0, 0));
    assert(temp.wCamTime == 1 && temp.bCamLvl == 0);
    temp.wCamTime = 5;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 6 && temp.bCamLvl == 0);
    temp.wCamTime = 15;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 0 && temp.bCamLvl == 1);
    /* camera stage 1: rotates until the sequence reaches stage 2 */
    temp.wLvl = 0, temp.wCount_1 = 100;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 3 && is(1, ROT, 60, 16383, 0, 0) && temp.wCamTime == 1);
    temp.wCamTime = 10, temp.wCount_1 = 100;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 11);
    temp.wCamTime = 59, temp.wCount_1 = 100;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 0 && temp.bCamLvl == 1);
    temp.wLvl = 2, temp.wCount_1 = 0, temp.wCamTime = 7;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 4 && is(1, CLOSE, 0, 0, 0, 0) && is(2, RESTORE, 16, 0, 0, 0));
    assert(log_p[2] == &temp && temp.wCamTime == 0 && temp.bCamLvl == 2);
    /* last camera stage and the last sequence stage: both bits end the effect */
    temp.wCamTime = 15, temp.wCount_1 = 113;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 16);
    temp.wCamTime = 16, temp.wCount_1 = 0;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 15, 0, 0, 0));
    temp.wCount_1 = 120, disp_answer = 1;
    assert(step(Ta_Eff_025, &in, -1) == 0 && log_n == 5 && is(1, BLACK, 128, 0, 0, 0) && is(2, WDISP, 0, 0, 0, 0));
    assert(is(3, WDRAW, 1, 0, 0, 0) && is(4, CLOSE, 0, 0, 0, 0) && temp.wCount_1 == 128);
    disp_answer = 0;
    assert(step(Ta_Eff_025, &in, -1) == 0 && log_n == 4 && is(3, CLOSE, 0, 0, 0, 0));

    /* sequence stage 0: fade to 48, then wait for the modules */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 49;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 3 && is(1, ISLOAD, 0, 0, 0, 0) && is(2, BLACK, 48, 0, 0, 0));
    assert(temp.wLvl == 0);
    isload_answer = 1;
    assert(step(Ta_Eff_025, &in, -1) == 1 && log_n == 4 && is(2, SKY, 0, 0, 0, 0) && is(3, BLACK, 48, 0, 0, 0));
    assert(temp.wLvl == 1);
    isload_answer = 0;

    /* stage 1, no launch (count not a multiple of 8): finished marks are removed, the rest wait */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 3, temp.ePos[12] = 0x205, temp.ePos[15] = 0x103, temp.eID[5] = 7, isuse_answer[7] = 1;
    assert(step_025(&in) == 1 && log_n == 22 && is(log_n - 1, TDEL, 1, 3, 0, 0));
    assert(temp.ePos[12] == 0 && temp.ePos[15] == 0x103 && temp.wCount_1 == 4 && temp.wLvl == 1 && temp.wCount_2 == 0);
    isuse_answer[7] = 0;
    /* launch: effect 525 at the next cell; an equal earlier cell is marked */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 8, temp.wCount_2 = 1, temp.ePos[0] = 0x304, temp.ePos[1] = 0x304, temp.eID[1] = 3;
    do_answer = 9, isuse_answer[9] = 1, status_answer[9] = 2, isuse_answer[3] = 1, status_answer[3] = 2;
    mapid_answer = -1;
    assert(step_025(&in) == 1 && log_n == 42 && is(1, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 525 && do_call.dwFrom == 0 && do_call.dwTo == 0x304 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.oNum == 1 && temp.wCount_2 == 2 && temp.wCount_1 == 9);
    assert(temp.ePos[0] == 0x10304 && temp.ePos[1] == 0x10304 && mapid_n == 1 && mapid_args[0] == 4 && mapid_args[1] == 3);
    /* ten launched already: no further launch; all ten finished ends the stage */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 8, temp.wCount_2 = 10;
    assert(step_025(&in) == 1 && log_n == 51 && temp.wCount_2 == 10 && temp.wCount_1 == 48 && temp.wLvl == 2);
    assert(temp.oNum == 0);

    /* a landed mark (in use, status 2): the cell's unit decides */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 3, temp.wCount_2 = 1, temp.eID[0] = 3, temp.ePos[0] = 0x304, status_answer[3] = 1;
    assert(step_025(&in) == 1 && is(2, STATUS, 3, 0, 0, 0) && is(3, ISUSE, 3, 0, 0, 0) && temp.ePos[0] == 0x304);
    status_answer[3] = 2, mapid_answer = 0x200;
    temp.wCount_1 = 3;
    assert(step_025(&in) == 1 && is(3, ISUSE, 3, 0, 0, 0) && temp.ePos[0] == 0x10304);
    temp.ePos[0] = 0x304, temp.wCount_1 = 3, mapid_answer = 0x105, ldrange_answer = 1;
    assert(step_025(&in) == 1 && is(3, CURSIDE, 1, 5, 0, 0) && is(4, LDRANGE, 1, 5, 11, 0));
    assert(is(5, POPUP, 0, -120, 80, 270) && is(6, LOC, 6, 0, 0, 0) && is(7, LDABL, 9, 6, 0, 0) && is(8, RANGE, 2, 3, 0, 4));
    assert(range_tail[0] == 1 && range_tail[1] == 1 && range_tail[2] == 11 && temp.ePos[0] == 0x10304);
    temp.ePos[0] = 0x304, temp.wCount_1 = 3, ldrange_answer = 0, resist_answer = 1;
    assert(step_025(&in) == 1 && is(5, RESIST, 1, 5, 0, 0) && is(6, MSG, 83, 0, 0, 0) && is(7, POPUP, 0, 0, 0, 0x4D5347));
    assert(temp.ePos[0] == 0x10304 && temp.ePos[10] == 0);
    temp.ePos[0] = 0x304, temp.wCount_1 = 3, in.dwPar = 6, do_answer = 12;
    assert(step_025(&in) == 1 && is(5, RESIST, 1, 5, 0, 0) && is(6, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 603 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 1);
    assert(temp.eID[10] == 12 && temp.ePos[10] == 0x105 && temp.ePos[0] == 0x10304 && is(log_n - 1, TDEL, 1, 5, 0, 0));
    temp.ePos[0] = 0x304, temp.ePos[10] = 0, temp.eID[10] = 0, temp.wCount_1 = 3, in.dwPar = 0, resist_answer = 0, do_answer = 13;
    assert(step_025(&in) == 1 && is(5, RESIST, 1, 5, 0, 0) && is(6, DOEFF, 0, 0, 0, 0));
    assert(temp.eID[10] == 13 && temp.ePos[10] == 0x105);
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    do_answer = 0, mapid_answer = 0, ldabl_answer = 0;
}

enum { REMOVE1 = 1100, AGREE };
static int remove_answer, agree_zero_packed, agree_word_on;
int Ta_RemoveObjFromFlagLst(int word, int side) { note(REMOVE1, NULL, word, side, 0, 0); return remove_answer; }
int Ta_GetAgreeOwnerIDon22bitFlagLst(int packed, unsigned word) { note(AGREE, NULL, packed, (int)word, 0, 0); return packed != agree_zero_packed && !(agree_word_on && (int)word == agree_word_on); }

static void reset_007(int lvl)
{
    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    temp.wLvl = lvl, temp.wCount_1 = 0x105;
}
/* Ta_Eff_007 (branch coverage; the always-true test of the stored effect id has one outcome). */
static void check_007(void)
{
    struct jp_6989c in = {7, 0x304, 0x12345, 0x10001};

    status_once = 0, loc_by_unit = 0, loc_answer = 0x302, ldabl_answer = 4, disp_answer = 0, isload_answer = 0;
    openrate_side2 = 0, remove_answer = 0x777, agree_zero_packed = 0x203;
    /* start, no trap B 75: range visual and the two modules */
    reset_007(0);
    trap_n = 0, trap_seq[0] = -1;
    assert(step(Ta_Eff_007, &in, 0) == 1 && log_n == 8);
    assert(is(0, WDRAW, 0, 0, 0, 0) && is(1, INIT, 0, 0, 0, 0) && log_p[1] == &temp);
    assert(is(2, REMOVE1, 0x12345, 1, 0, 0) && is(3, TRAPB, 0x10001, 75, 0, 0) && temp.fObj2 == 0x777);
    assert(is(4, RANGE, 4, 3, 0, 0) && range_tail[0] == 1 && range_tail[1] == 18 && range_tail[2] == 255);
    assert(is(5, STDVIEW, 30, 0, 0, 0) && is(6, LOAD, 507, 0, 0, 0) && is(7, LOAD, 603, 0, 0, 0));
    /* start, trap B 75 only */
    reset_007(0);
    temp.wCount_1 = 0;
    trap_n = 0, trap_seq[0] = 0x105, trap_seq[1] = -1;
    assert(step(Ta_Eff_007, &in, 0) == 1 && log_n == 9);
    assert(is(3, TRAPB, 0x10001, 75, 0, 0) && is(4, TRAPB, 0, 46, 0, 0) && is(5, FOCUS, 0, -1, 180, 180));
    assert(is(6, DOOPEN, 1, 5, 1, 0) && is(7, LOAD, 602, 0, 0, 0) && is(8, LOAD, 603, 0, 0, 0));
    assert(temp.wLvl == 32 && temp.wCount_1 == 0x105 && temp.wCount_2 == 34);
    /* start, both traps; a zero dwPar asks for side 1 */
    reset_007(0);
    in.dwPar = 0;
    trap_n = 0, trap_seq[0] = 0x105, trap_seq[1] = 0x203;
    assert(step(Ta_Eff_007, &in, 0) == 1 && log_n == 9);
    assert(is(2, REMOVE1, 0x12345, 0, 0, 0) && is(4, TRAPB, 1, 46, 0, 0) && is(5, FOCUS, 0, 0x105, 180, 180));
    assert(is(6, DOOPEN, 2, 3, 1, 0) && is(7, LOAD, 602, 0, 0, 0) && is(8, LOAD, 507, 0, 0, 0));
    assert(temp.wLvl == 32 && temp.wCount_1 == 0x203 && temp.wCount_2 == 36);

    /* modules not loaded; unknown stage */
    reset_007(0);
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && is(1, ISLOAD, 0, 0, 0, 0));
    assert(temp.wLvl == 0);
    isload_answer = 1;
    reset_007(7);
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 2 && temp.wLvl == 7);
    /* stage 0 */
    reset_007(0);
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 3 && is(2, DEPEND, 507, 0, 0, 0) && log_p[2] == &temp);
    assert(depend_call.eNO == 999 && depend_call.dwFrom == 0 && depend_call.dwTo == 0 && depend_call.dwPar == 0);
    assert(temp.wLvl == 1);
    /* stage 36 */
    reset_007(36);
    temp.oNum = 3, temp.wCount_2 = 5;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 4 && is(2, STDVIEW, 30, 0, 0, 0) && is(3, DEPEND, 507, 0, 0, 0));
    assert(temp.oNum == 0 && temp.wCount_2 == 0 && temp.wLvl == 1);
    /* stage 35, window hidden / shown */
    reset_007(35);
    temp.wCamTime = 9;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 5 && is(2, TDEL, 1, 5, 0, 0) && is(3, RESTORE, 16, 0, 0, 0));
    assert(is(4, WDISP, 0, 0, 0, 0) && temp.wCamTime == 0 && temp.wLvl == 2);
    reset_007(35);
    disp_answer = 1;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 6 && is(5, WDRAW, 1, 0, 0, 0) && temp.wLvl == 2);
    disp_answer = 0;
    /* stage 34 */
    reset_007(34);
    temp.oNum = 2, do_answer = 7;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 3 && is(2, DOEFF, 0, 0, 0, 0) && log_p[2] == &temp);
    assert(do_call.eNO == 603 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0);
    assert(temp.eID[2] == 7 && temp.oNum == 3 && temp.wCount_2 == 35 && temp.wLvl == 33);
    /* stage 33: waits for the last effect */
    reset_007(33);
    temp.oNum = 3, temp.eID[2] = 7, temp.wCount_2 = 35, isuse_answer[7] = 1;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 3 && is(2, ISUSE, 7, 0, 0, 0) && temp.wLvl == 33);
    isuse_answer[7] = 0;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 5 && is(3, USTAT, 1, 5, 0, 1) && is(4, CLOSE, 0, 0, 0, 0));
    assert(temp.wLvl == 35);
    /* stage 32: waits for the open rate, then one of two messages */
    reset_007(32);
    openrate_answer = 100;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 3 && is(2, OPENRATE, 1, 5, 0, 0) && temp.wLvl == 32);
    openrate_answer = 255;
    temp.wCount_2 = 36, temp.oNum = 1, do_answer = 9;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 6 && is(3, MSG, 825, 0, 0, 0));
    assert(is(4, POPUP, 0, -120, 80, 0x4D5347) && is(5, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0);
    assert(temp.eID[1] == 9 && temp.oNum == 2 && temp.wLvl == 33);
    reset_007(32);
    temp.wCount_2 = 34;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 6 && is(3, MSG, 820, 0, 0, 0) && temp.wLvl == 33);
    /* stage 2: counts past 16 */
    reset_007(2);
    temp.wCamTime = 16;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 2 && temp.wCamTime == 17);
    assert(step(Ta_Eff_007, &in, -1) == 0 && log_n == 3 && is(2, CLOSE, 0, 0, 0, 0) && temp.wCamTime == 18);

    /* stage 1: a finished effect on an agreeing unit starts effect 603; another gets the range notice */
    reset_007(1);
    temp.oNum = 3, temp.fObj2 = 0x55, do_answer = 8, ldrange_answer = 1;
    temp.eID[0] = 4, temp.eID[1] = 5, temp.eID[2] = 6;
    temp.ePos[0] = 0x105, temp.ePos[1] = 0x203, temp.ePos[2] = 0x400;
    isuse_answer[4] = isuse_answer[5] = 1, status_answer[4] = status_answer[5] = 2;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 17);
    assert(is(2, ISUSE, 4, 0, 0, 0) && is(3, STATUS, 4, 0, 0, 0) && is(4, AGREE, 0x105, 0x55, 0, 0) && is(5, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 603 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 1);
    assert(temp.eID[3] == 8 && temp.ePos[3] == 0x105 && temp.wCount_2 == 1 && temp.ePos[0] == -1);
    assert(is(6, ISUSE, 5, 0, 0, 0) && is(7, STATUS, 5, 0, 0, 0) && is(8, AGREE, 0x203, 0x55, 0, 0));
    assert(is(9, CURSIDE, 2, 3, 0, 0) && is(10, LOC, 6, 0, 0, 0) && is(11, LDRANGE, 2, 3, 11, 0) && is(12, LDABL, 9, 6, 0, 0));
    assert(is(13, RANGE, 2, 3, 0, 4) && range_tail[0] == 1 && range_tail[1] == 1 && range_tail[2] == 11);
    assert(is(14, POPUP, 0, -120, 80, 270) && temp.ePos[1] == -1);
    assert(is(15, ISUSE, 6, 0, 0, 0) && temp.ePos[2] == 0x400 && is(16, ISUSE, 4, 0, 0, 0) && temp.wLvl == 1);
    /* stage 1: unfinished effect, negative position, no range ability */
    reset_007(1);
    temp.oNum = 3, ldrange_answer = 0;
    temp.eID[0] = 4, temp.eID[1] = 5, temp.eID[2] = 6;
    temp.ePos[0] = 0x105, temp.ePos[1] = -1, temp.ePos[2] = 0x203;
    isuse_answer[4] = isuse_answer[5] = isuse_answer[6] = 1, status_answer[4] = 1, status_answer[5] = status_answer[6] = 2;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 13);
    assert(is(3, STATUS, 4, 0, 0, 0) && is(4, ISUSE, 5, 0, 0, 0) && is(5, STATUS, 5, 0, 0, 0) && is(6, ISUSE, 6, 0, 0, 0));
    assert(is(8, AGREE, 0x203, 0, 0, 0) && is(9, CURSIDE, 2, 3, 0, 0) && is(10, LOC, 6, 0, 0, 0) && is(11, LDRANGE, 2, 3, 11, 0));
    assert(is(12, ISUSE, 4, 0, 0, 0) && temp.ePos[0] == 0x105 && temp.ePos[2] == -1 && temp.wCount_2 == 0);
    /* stage 1, all effects over: flagged units are deleted, positions 0, 10, 11 and 21 */
    reset_007(1);
    temp.oNum = 2, temp.wCount_2 = 1, temp.wCamTime = 5;
    temp.fObj2 = (1u << 21) | (1u << 11) | (1u << 10) | 1u;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 14);
    assert(is(6, ISUSE, 0, 0, 0, 0) && is(7, TDEL, 0, 0, 0, 0) && is(8, TDEL, 0, 10, 0, 0) && is(9, TDEL, 1, 0, 0, 0));
    assert(is(10, TDEL, 1, 10, 0, 0) && is(11, CLOSE, 0, 0, 0, 0) && is(12, RESTORE, 16, 0, 0, 0) && is(13, WDISP, 0, 0, 0, 0));
    assert(temp.wCamTime == 0 && temp.wLvl == 2);
    reset_007(1);
    disp_answer = 1;
    assert(step(Ta_Eff_007, &in, -1) == 1 && log_n == 6 && is(5, WDRAW, 1, 0, 0, 0) && temp.wLvl == 2);
    disp_answer = 0;
}

enum { FLATVIEW = 1200, FINDEST, GGOALC, DBGPRINT };
static int findest_answer, goalc_answer, print_args[4];
static struct jp_6989c findest_call;
char data_002b86e8[1];
int Ta_IsFinishDestEfc(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall)
{
    findest_call = *pCall;
    log_p[note(FINDEST, eInfo, 0, 0, 0, 0)] = Temp;
    return findest_answer;
}
void TaView_ReqFlatCompleteView(unsigned char flag) { note(FLATVIEW, NULL, flag, 0, 0, 0); }
int SzDuel_GetUnitCtlGoalC(int a, int b) { note(GGOALC, NULL, a, b, 0, 0); return goalc_answer; }
int func_00199b50(const char *format, ...)
{
    va_list ap;
    int k;

    va_start(ap, format);
    for (k = 0; k < 4; k++) {
        print_args[k] = va_arg(ap, int);
    }
    va_end(ap);
    note(DBGPRINT, (void *)format, 0, 0, 0, 0);
    return 0;
}
static void reset_020(int lvl)
{
    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    temp.wLvl = lvl;
}
/* Ta_Eff_020 (branch coverage; the always-true tests of the stored effect id have one outcome, and
 * stage 1 is not run with an empty flag list, where the original reads Unit before any store). */
static void check_020(void)
{
    struct jp_6989c in = {20, 0x304, 0x12345, 0};

    status_once = 0, loc_by_unit = 0, num_by_value = 0, owner_by_n = 1, owner_answer = 0x103, gustat_zero_pos = 3;
    /* start call */
    reset_020(0);
    assert(step(Ta_Eff_020, &in, 5) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && is(1, INIT, 0, 0, 0, 0) && log_p[1] == &temp);
    assert(is(2, LOAD, 17, 0, 0, 0) && is(3, LOAD, 24, 0, 0, 0) && is(4, LOAD, 603, 0, 0, 0));
    assert(is(5, FADE, 5, -1, 1, 1) && log_p[5] == &temp && is(6, FLATVIEW, 16, 0, 0, 0) && temp.wCount_1 == 128);
    reset_020(0);
    assert(step(Ta_Eff_020, &in, 0) == 1 && log_n == 7 && is(5, FADE, 0, -1, 1, 1));
    /* stage 0: fade above 63; at 63 with the wait below / at 16; modules not loaded / loaded */
    reset_020(0);
    temp.wCount_1 = 64;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 49, 0, 0, 0) && temp.wCount_1 == 49 && temp.wCamTime == 1);
    reset_020(0);
    temp.wCount_1 = 63, temp.wCamTime = 14;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCamTime == 15 && temp.wLvl == 0);
    isload_answer = 0;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 3 && is(1, ISLOAD, 0, 0, 0, 0) && log_p[1] == &temp && is(2, BLACK, 48, 0, 0, 0));
    assert(temp.wCamTime == 16 && temp.wLvl == 0);
    isload_answer = 1, do_answer = 9, temp.oNum = 2;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 4 && is(2, DOEFF, 0, 0, 0, 0) && log_p[2] == &temp && is(3, BLACK, 48, 0, 0, 0));
    assert(do_call.eNO == 17 && do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0);
    assert(temp.eID[2] == 9 && temp.oNum == 3 && temp.wLvl == 1);
    /* stage 1: effect still in use */
    reset_020(1);
    temp.eID[0] = 9, isuse_answer[9] = 1;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    /* stage 1: unit 0x103 lacks status 4, unit 0x104 has kind 0 and makes the list */
    isuse_answer[9] = 0, num_answer = 2, ukind_zero = 1, random_answer = 0x205, do_answer = 7;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 23);
    assert(is(2, NUM, 0x12345, 0, 0, 0) && is(3, OWNER, 0x12345, 0, 0, 0) && is(4, GUSTAT, 1, 3, 4, 0) && is(5, NUM, 0x12345, 0, 0, 0));
    assert(is(6, OWNER, 0x12345, 1, 0, 0) && is(7, GUSTAT, 1, 4, 4, 0) && is(8, UKIND, 1, 4, 0, 0) && is(9, CONV, 0, 1, 4, 0));
    assert(is(10, NUM, 0x12345, 0, 0, 0) && is(11, NUM, 4, 0, 0, 0) && is(12, RANDOM, 4, 0, 0, 0) && is(13, FOCUS, 0, 0x205, 100, 180));
    assert(is(14, LOC, 1, 4, 0, 0) && is(15, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 24 && do_call.dwFrom == 0 && do_call.dwTo == 0x205 && do_call.dwPar == 0);
    assert(is(16, NUM, 0x12345, 0, 0, 0) && is(17, OWNER, 0x12345, 0, 0, 0) && is(18, UFADE, 1, 3, -1, 7) && is(19, CTLRATE, 1, 3, -1, 0));
    assert(is(20, NUM, 0x12345, 0, 0, 0) && is(21, OWNER, 0x12345, 1, 0, 0) && is(22, NUM, 0x12345, 0, 0, 0));
    assert(temp.fObj2 == 0x205 && temp.eID[0] == 7 && temp.oNum == 1 && temp.ePos[0] == 0x205 && temp.wLvl == 2);
    /* stage 1: kind not 0, so the list stays empty and the whole target list is used */
    reset_020(1);
    ukind_zero = 0, num_by_value = 1;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 22 && is(8, UKIND, 1, 4, 0, 0) && is(9, NUM, 0x12345, 0, 0, 0));
    assert(is(10, NUM, 0, 0, 0, 0) && is(11, RANDOM, 0x12345, 0, 0, 0) && temp.wLvl == 2);
    num_by_value = 0, gustat_zero_pos = -1;
    /* stage 2: not in use; status not 2; position already cleared; effect 603 started */
    reset_020(2);
    temp.eID[0] = 7;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[7] = 1, status_answer[7] = 1;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 3 && is(2, STATUS, 7, 0, 0, 0) && temp.wLvl == 2);
    status_answer[7] = 2, temp.ePos[0] = -1;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 3 && temp.wLvl == 2);
    temp.ePos[0] = 0, temp.fObj2 = 0x205, temp.oNum = 1, do_answer = 8;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 4 && is(3, DOEFF, 0, 0, 0, 0) && temp.ePos[0] == -1);
    assert(do_call.eNO == 603 && do_call.dwFrom == 0 && do_call.dwTo == 0x205 && do_call.dwPar == 0);
    assert(temp.eID[1] == 8 && temp.oNum == 2 && temp.wLvl == 3);
    /* stage 3: destroy effects running / finished */
    findest_answer = 0, temp.wCamTime = 5;
    assert(step(Ta_Eff_020, &in, -3) == 1 && log_n == 2 && is(1, FINDEST, 0, 0, 0, 0) && log_p[1] == &temp && temp.wLvl == 3);
    assert(findest_call.eNO == 999 && findest_call.dwFrom == 0 && findest_call.dwTo == 0 && findest_call.dwPar == 0);
    findest_answer = 1;
    assert(step(Ta_Eff_020, &in, -3) == 1 && log_n == 4 && is(2, FADE, -3, 1, 0, 1) && log_p[2] == &temp && is(3, RESTORE, 16, 0, 0, 0));
    assert(temp.wCamTime == 0 && temp.wLvl == 4);
    /* later stages: report, fade out below / at the limit, the wait, the fade answer, the window */
    goalm_answer = 11, goalc_answer = 12, ratem_answer = 13, ctlc_answer = 14, temp.wCount_1 = 100;
    /* The four getters are call arguments: their order is the compiler's (left to right in the
     * original), so only their presence is checked here. */
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 7);
    assert(log_id[1] + log_id[2] + log_id[3] + log_id[4] == GGOALM + GGOALC + GRATEM + CTLC && log_a[1] + log_a[2] + log_a[3] + log_a[4] == 0);
    assert(log_b[1] + log_b[2] + log_b[3] + log_b[4] == 0);
    assert(is(5, DBGPRINT, 0, 0, 0, 0) && log_p[5] == data_002b86e8 && is(6, BLACK, 115, 0, 0, 0));
    assert(print_args[0] == 11 && print_args[1] == 12 && print_args[2] == 13 && print_args[3] == 14 && temp.wCamTime == 0);
    temp.wCamTime = 16;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 7 && is(6, BLACK, 128, 0, 0, 0) && temp.wCount_1 == 128 && temp.wCamTime == 17);
    finfade_answer = 0;
    assert(step(Ta_Eff_020, &in, -1) == 1 && log_n == 8 && is(7, FINFADE, 0, 0, 0, 0) && temp.wCamTime == 18);
    finfade_answer = 1, disp_answer = 0;
    assert(step(Ta_Eff_020, &in, -1) == 0 && log_n == 10 && is(8, WDISP, 0, 0, 0, 0) && is(9, CLOSE, 0, 0, 0, 0));
    disp_answer = 1;
    assert(step(Ta_Eff_020, &in, -1) == 0 && log_n == 11 && is(9, WDRAW, 1, 0, 0, 0) && is(10, CLOSE, 0, 0, 0, 0));
    disp_answer = 0, finfade_answer = 0, isload_answer = 0, owner_by_n = 0, goalm_answer = ratem_answer = ctlc_answer = 0;
}

enum { PUTUNIT = 900, SMODEC };
int SzDuel_PutUnit(int a, int b, int c) { note(PUTUNIT, NULL, a, b, c, 0); return 0; }
int SzDuel_SetUnitCtlModeC(int a, int b, int value) { note(SMODEC, NULL, a, b, value, 0); return 0; }
static void reset_100(int lvl)
{
    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    temp.wLvl = lvl, temp.eID[0] = 7;
}
/* Ta_Eff_100 (branch coverage: each start outcome, each wait of stage 0 and the three effect
 * numbers, every case of both sound switches plus a frame with no sound, both ends of each
 * running/finished test). */
static void check_100(void)
{
    static const int se999[6][2] = {{0, 56}, {20, 57}, {30, 76}, {36, 74}, {48, 60}, {40, 0}};
    static const int se[7][2] = {{0, 56}, {20, 57}, {30, 76}, {40, 58}, {100, 59}, {120, 60}, {36, 0}};
    struct jp_6989c in = {100, 0x102, 0x304, 5};
    int i;

    status_once = 0;
    /* start call: neither unit shown / both shown; the module depends on dwPar and eNO */
    reset_100(0), ratem_answer = 0;
    assert(step(Ta_Eff_100, &in, 0) == 1 && log_n == 4 && is(0, INIT, 0, 0, 0, 0) && log_p[0] == &temp && is(1, GRATEM, 1, 2, 0, 0));
    assert(is(2, GRATEM, 3, 4, 0, 0) && is(3, LOAD, 600, 0, 0, 0) && log_p[3] == &temp && temp.wCount_1 == 0 && temp.wCount_2 == 0);
    reset_100(0), ratem_answer = 1, in.eNO = 43;
    assert(step(Ta_Eff_100, &in, 3) == 1 && log_n == 6 && is(2, UFADE, 1, 2, -1, 1) && is(3, GRATEM, 3, 4, 0, 0));
    assert(is(4, UFADE, 3, 4, -1, 1) && is(5, LOAD, 543, 0, 0, 0) && temp.wCount_1 == 1 && temp.wCount_2 == 1);
    reset_100(0), ratem_answer = 0, in.dwPar = 999;
    assert(step(Ta_Eff_100, &in, 0) == 1 && log_n == 4 && is(3, LOAD, 538, 0, 0, 0));
    /* stage 0: module not loaded, either unit still fading, then the effect call */
    reset_100(0), isload_answer = 0;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 1 && is(0, ISLOAD, 0, 0, 0, 0) && log_p[0] == &temp && temp.wLvl == 0);
    isload_answer = 1, temp.wCount_1 = 1, ratem_answer = 5, goalm_answer = 6;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 3 && is(1, GRATEM, 1, 2, 0, 0) && is(2, GGOALM, 1, 2, 0, 0) && temp.wLvl == 0);
    temp.wCount_1 = 0, temp.wCount_2 = 1;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 3 && is(1, GRATEM, 3, 4, 0, 0) && is(2, GGOALM, 3, 4, 0, 0) && temp.wLvl == 0);
    temp.wCount_1 = 1, goalm_answer = 5, do_answer = 9, temp.ePos[0] = 3;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 6 && is(5, DOEFF, 0, 0, 0, 0) && log_p[5] == &temp);
    assert(do_call.eNO == 538 && do_call.dwFrom == 0x102 && do_call.dwTo == 0x304 && do_call.dwPar == 1);
    assert(temp.eID[0] == 9 && temp.ePos[0] == 0 && temp.wLvl == 1);
    reset_100(0), in.dwPar = 5;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 2 && is(1, DOEFF, 0, 0, 0, 0) && temp.wLvl == 1);
    assert(do_call.eNO == 543 && do_call.dwFrom == 0x102 && do_call.dwTo == 0x304 && do_call.dwPar == 1);
    reset_100(0), in.eNO = 100;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 2 && do_call.eNO == 600 && do_call.dwPar == 1 && temp.wLvl == 1);
    /* later stages, dwPar 999: sounds while the effect runs, then the unit is removed */
    in.dwPar = 999;
    for (i = 0; i < 6; i++) {
        reset_100(1), isuse_answer[7] = 1, temp.fObj2 = se999[i][0];
        assert(step(Ta_Eff_100, &in, -1) == 1 && is(0, ISUSE, 7, 0, 0, 0) && temp.fObj2 == (unsigned)se999[i][0] + 1);
        assert(se999[i][1] ? log_n == 2 && is(1, SE, 1, se999[i][1], 0, 0) : log_n == 1);
    }
    reset_100(2);
    assert(step(Ta_Eff_100, &in, -1) == 0 && log_n == 3 && is(1, TDEL, 1, 2, 0, 0) && is(2, CLOSE, 0, 0, 0, 0));
    temp.wCount_2 = 1;
    assert(step(Ta_Eff_100, &in, -1) == 0 && log_n == 4 && is(1, TDEL, 1, 2, 0, 0) && is(2, UFADE, 3, 4, 1, 1) && is(3, CLOSE, 0, 0, 0, 0));
    /* later stages, other dwPar: sounds, the status test, the one-time replacement, the end */
    in.dwPar = 5;
    for (i = 0; i < 7; i++) {
        reset_100(1), isuse_answer[7] = 1, status_answer[7] = 1, temp.fObj2 = se[i][0];
        assert(step(Ta_Eff_100, &in, -1) == 1 && is(0, ISUSE, 7, 0, 0, 0) && temp.fObj2 == (unsigned)se[i][0] + 1);
        assert(se[i][1] ? log_n == 3 && is(1, SE, 1, se[i][1], 0, 0) : log_n == 2);
        assert(is(log_n - 1, STATUS, 7, 0, 0, 0) && temp.ePos[0] == 0);
    }
    reset_100(1), isuse_answer[7] = 1, status_answer[7] = 2, temp.fObj2 = 1;
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 9 && is(1, STATUS, 7, 0, 0, 0) && is(2, CARDNO, 3, 4, 0, 0));
    assert(is(3, ADDGRAVE, 3, 764, 0, 0) && is(4, PUTUNIT, 3, 4, 5, 0) && is(5, TDEL, 1, 2, 0, 0) && is(6, SMODEC, 3, 4, 1, 0));
    assert(is(7, SGOALC, 3, 4, 0, 0) && is(8, SRATEC, 3, 4, -1, 0) && temp.ePos[0] == 1);
    assert(step(Ta_Eff_100, &in, -1) == 1 && log_n == 2 && is(1, STATUS, 7, 0, 0, 0) && temp.ePos[0] == 1);
    isuse_answer[7] = 0;
    assert(step(Ta_Eff_100, &in, -1) == 0 && log_n == 6 && is(1, SMODEC, 3, 4, 1, 0) && is(2, SGOALC, 3, 4, 128, 0));
    assert(is(3, SRATEC, 3, 4, 1, 128) && is(4, SE, 0, 2, 0, 0) && is(5, CLOSE, 0, 0, 0, 0));
    isload_answer = 0, ratem_answer = goalm_answer = 0, do_answer = 0;
}
enum { CKIND = 1300, MCOUNT, SCOUNT, GCOUNT, INQGRAVE, DELGRAVE, PACKGRAVE };
static int ckind_answer, mcount_answer, scount_answer, gcount_answer, inqgrave_answer[4], inqgrave_side_add;
int TaHand_GetCardKind(int id) { note(CKIND, NULL, id, 0, 0, 0); return ckind_answer; }
int SzDuel_GetUnitMonsterCount(int side) { note(MCOUNT, NULL, side, 0, 0, 0); return mcount_answer; }
int SzDuel_GetUnitSpellCount(int side) { note(SCOUNT, NULL, side, 0, 0, 0); return scount_answer; }
int SzDuel_GetGraveCount(int index) { note(GCOUNT, NULL, index, 0, 0, 0); return gcount_answer; }
int SzDuel_InqGrave(int index, int position) { note(INQGRAVE, NULL, index, position, 0, 0); return inqgrave_answer[position] + index * inqgrave_side_add; }
int SzDuel_DelGrave(int index, int position) { note(DELGRAVE, NULL, index, position, 0, 0); return 0; }
void SzDuel_PackGrave(int index) { note(PACKGRAVE, NULL, index, 0, 0, 0); }
static void reset_001(int lvl)
{
    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    temp.wLvl = lvl, temp.eID[0] = 7;
}
/* Ta_Eff_001 (branch coverage: the start call and stage 0 with no card, a full monster row, a full
 * spell row and room on either; module loading or loaded; the effect start; effect running in
 * each status; both grave branches with and without a new unit, each kind, each dwFrom flag, the
 * grave search with a hit, without one and with an empty grave; the open step with and without a
 * unit; the camera restore, the wait at and past its bound, window shown or not). */
static void check_001(void)
{
    struct jp_6989c in = {1, 1, 0x304, 999};
    struct jp_6a917 unit;

    status_once = 0;
    /* start call */
    reset_001(0);
    assert(step(Ta_Eff_001, &in, 0) == 0 && log_n == 2 && is(0, INIT, 0, 0, 0, 0) && log_p[0] == &temp && is(1, CLOSE, 0, 0, 0, 0));
    in.dwPar = 0x20123, ckind_answer = 0x1F, mcount_answer = 5, scount_answer = 0;
    assert(step(Ta_Eff_001, &in, 0) == 0 && log_n == 4 && is(1, CKIND, 0x123, 0, 0, 0) && is(2, MCOUNT, 1, 0, 0, 0));
    assert(is(3, CLOSE, 0, 0, 0, 0) && in.dwPar == 999);
    in.dwPar = 0x20123, mcount_answer = 4;
    assert(step(Ta_Eff_001, &in, 0) == 1 && log_n == 6 && is(2, MCOUNT, 1, 0, 0, 0) && is(3, WDRAW, 0, 0, 0, 0));
    assert(is(4, LOAD, 501, 0, 0, 0) && log_p[4] == &temp && is(5, FOCUS2, 16, 0x304, 180, 0) && in.dwPar == 0x20123);
    ckind_answer = 0x20, mcount_answer = 5, scount_answer = 5;
    assert(step(Ta_Eff_001, &in, 0) == 0 && log_n == 4 && is(2, SCOUNT, 1, 0, 0, 0) && is(3, CLOSE, 0, 0, 0, 0) && in.dwPar == 999);
    in.dwPar = 0x20123, scount_answer = 4;
    assert(step(Ta_Eff_001, &in, 0) == 1 && log_n == 6 && is(2, SCOUNT, 1, 0, 0, 0) && is(5, FOCUS2, 16, 0x304, 180, 0));
    /* stage 0 */
    reset_001(0), in.dwPar = 999;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 1 && is(0, CLOSE, 0, 0, 0, 0));
    in.dwPar = 0x20123, ckind_answer = 0x1F, mcount_answer = 5, scount_answer = 0, disp_answer = 0;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 4 && is(0, CKIND, 0x123, 0, 0, 0) && is(1, MCOUNT, 1, 0, 0, 0));
    assert(is(2, WDISP, 0, 0, 0, 0) && is(3, CLOSE, 0, 0, 0, 0) && in.dwPar == 0x20123);
    disp_answer = 1;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 5 && is(3, WDRAW, 1, 0, 0, 0) && is(4, CLOSE, 0, 0, 0, 0));
    ckind_answer = 0x20, mcount_answer = 0, scount_answer = 5, disp_answer = 0;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 4 && is(1, SCOUNT, 1, 0, 0, 0) && is(2, WDISP, 0, 0, 0, 0) && is(3, CLOSE, 0, 0, 0, 0));
    disp_answer = 1;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 5 && is(3, WDRAW, 1, 0, 0, 0) && is(4, CLOSE, 0, 0, 0, 0));
    scount_answer = 4, isload_answer = 0;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 4 && is(2, WDRAW, 0, 0, 0, 0) && is(3, ISLOAD, 0, 0, 0, 0) && log_p[3] == &temp && temp.wLvl == 0);
    ckind_answer = 0x1F, mcount_answer = 4, isload_answer = 1;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 4 && is(1, MCOUNT, 1, 0, 0, 0) && temp.wLvl == 1);
    /* stage 1 */
    do_answer = 9;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && is(1, DOEFF, 0, 0, 0, 0) && log_p[1] == &temp);
    assert(do_call.eNO == 501 && do_call.dwFrom == 0 && do_call.dwTo == 0x304 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.oNum == 1 && temp.wLvl == 2);
    /* stage 2: effect gone / running in another status */
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 3);
    reset_001(2), isuse_answer[7] = 1, status_answer[7] = 1;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 3 && is(2, STATUS, 7, 0, 0, 0) && temp.ePos[0] == 0 && temp.wLvl == 2);
    /* status 2, no card number: the top grave card becomes the unit */
    status_answer[7] = 2, in.dwPar = 999, in.dwFrom = 1, gcount_answer = 3, inqgrave_answer[2] = 0x55, addunit_answer = NULL;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 6 && is(3, GCOUNT, 1, 0, 0, 0) && is(4, INQGRAVE, 1, 2, 0, 0));
    assert(is(5, ADDUNIT, 1, 4, 3, 0x55) && addunit_e == 0 && temp.ePos[0] == 0xFFFF && temp.fObj == 0 && temp.wLvl == 2);
    /* the stored 0xFFFF does not stop a second pass: only its low byte is compared with 0xFFFF */
    memset(&unit, 0, sizeof(unit)), unit.bVarKind = 0x1F, unit.bOwner = 1, unit.bID = 6, addunit_answer = &unit;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 6 && unit.fFinish == 0 && temp.fObj == 0);
    unit.bVarKind = 0x20, in.dwFrom = 0x10001;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 6 && unit.fFinish != 0 && temp.fObj == 0x106);
    in.dwFrom = 0x20001, in.dwPar = 999, temp.fObj = 0;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 9 && is(6, GCOUNT, 1, 0, 0, 0) && is(7, DELGRAVE, 0, 2, 0, 0));
    assert(is(8, PACKGRAVE, 0, 0, 0, 0) && temp.fObj == 0);
    /* status 2 with a card number */
    reset_001(2), isuse_answer[7] = 1, status_answer[7] = 2, in.dwFrom = 1, in.dwPar = 0x20123, addunit_answer = NULL;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 4 && is(3, ADDUNIT, 1, 4, 3, 0x123) && addunit_e == 0 && temp.ePos[0] == 0xFFFF);
    memset(&unit, 0, sizeof(unit)), unit.bVarKind = 0x1F, unit.bOwner = 1, unit.bID = 6, addunit_answer = &unit;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 4 && unit.fFinish == 0 && temp.fObj == 0);
    unit.bVarKind = 0x20, in.dwFrom = 0x10001;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 4 && unit.fFinish != 0 && temp.fObj == 0x106);
    in.dwFrom = 0x20001, temp.fObj = 0, gcount_answer = 0;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 6 && is(4, GCOUNT, 2, 0, 0, 0) && is(5, PACKGRAVE, 2, 0, 0, 0) && temp.fObj == 0);
    gcount_answer = 3, inqgrave_answer[0] = 1, inqgrave_answer[1] = 0x123, inqgrave_answer[2] = 0x123;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 9 && is(5, INQGRAVE, 2, 0, 0, 0) && is(6, INQGRAVE, 2, 1, 0, 0));
    assert(is(7, DELGRAVE, 2, 1, 0, 0) && is(8, PACKGRAVE, 2, 0, 0, 0));
    inqgrave_answer[1] = 2, inqgrave_answer[2] = 3;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 9 && is(7, INQGRAVE, 2, 2, 0, 0) && is(8, PACKGRAVE, 2, 0, 0, 0));
    /* stage 3 */
    reset_001(3);
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 1 && is(0, WDRAW, 0, 0, 0, 0) && temp.wLvl == 4);
    reset_001(3), temp.fObj = 0x106;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 2 && is(1, DOOPEN, 1, 6, 1, 0) && temp.wLvl == 4);
    /* last stage */
    temp.wCamTime = 5;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && is(1, RESTORE, 16, 0, 0, 0) && log_p[1] == &temp);
    assert(temp.wCamTime == 0 && temp.bCamLvl == 1 && temp.wLvl == 4);
    temp.wCamTime = 16;
    assert(step(Ta_Eff_001, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17 && temp.bCamLvl == 1);
    disp_answer = 0;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 3 && is(1, WDISP, 0, 0, 0, 0) && is(2, CLOSE, 0, 0, 0, 0) && temp.wCamTime == 18);
    disp_answer = 1;
    assert(step(Ta_Eff_001, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && is(3, CLOSE, 0, 0, 0, 0));
    isload_answer = 0, disp_answer = 0, do_answer = 0, addunit_answer = NULL;
}
static int same_call(int eNO, unsigned dwTo, unsigned dwPar)
{
    return do_call.eNO == eNO && do_call.dwFrom == 0 && do_call.dwTo == dwTo && do_call.dwPar == dwPar;
}
/* The closing wait shared by 0x275740 and 0x2759d0: at the bound, past it with the window hidden
 * and shown. */
static void check_closing_wait(int (*f)(struct jp_6989c *, struct jp_69942 *, int), struct jp_6989c *in)
{
    temp.wLvl = 3, temp.wCamTime = 16;
    assert(step(f, in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(f, in, -1) == 0 && log_n == 3 && is(1, WDISP, 0, 0, 0, 0) && is(2, CLOSE, 0, 0, 0, 0) && temp.wCamTime == 18);
    disp_answer = 1;
    assert(step(f, in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && is(3, CLOSE, 0, 0, 0, 0));
}

/* Ta_Eff_070 (branch coverage: the start call; each stage with its wait taken and not taken; the
 * closing wait at and past its bound, window shown or not). */
static void check_275740(void)
{
    struct jp_6989c in = {0, 0x304, 0x105, 0};

    memset(&temp, 0, sizeof temp), openrate_side2 = 0;
    assert(step(Ta_Eff_070, &in, 0) == 1 && log_n == 6 && is(0, WDRAW, 0, 0, 0, 0) && is(1, INIT, 0, 0, 0, 0) && log_p[1] == &temp);
    assert(is(2, LOAD, 601, 0, 0, 0) && is(3, LOAD, 602, 0, 0, 0) && is(4, DOOPEN, 3, 4, 1, 0) && is(5, FOCUS, 0, 0x304, 100, 180));
    isload_answer = 0;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 2 && is(1, ISLOAD, 0, 0, 0, 0) && temp.wLvl == 0);
    isload_answer = 1, openrate_answer = 254;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 3 && is(2, OPENRATE, 3, 4, 0, 0) && temp.wLvl == 0);
    openrate_answer = 255, do_answer = 7;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 259) && is(4, DOEFF, 0, 0, 0, 0));
    assert(log_p[4] == &temp && same_call(602, 0x304, 0) && temp.eID[0] == 7 && temp.wLvl == 1);
    isuse_answer[7] = 1;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[7] = 0, do_answer = 9;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 7 && is(2, USTAT, 3, 4, 0, 1) && is(3, CARDNO, 3, 4, 0, 0) && is(4, MSG, 764, 0, 0, 0));
    assert(is(5, POPUP, 0, -120, 80, 0x4D5347) && is(6, DOEFF, 0, 0, 0, 0) && same_call(601, 0x304, 1) && temp.eID[0] == 9 && temp.wLvl == 2);
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[9] = 0;
    assert(step(Ta_Eff_070, &in, -1) == 1 && log_n == 5 && is(2, TDEL, 3, 4, 0, 0) && is(3, CLOSE, 0, 0, 0, 0) && is(4, RESTORE, 16, 0, 0, 0));
    assert(log_p[4] == &temp && temp.wLvl == 3);
    check_closing_wait(Ta_Eff_070, &in);
}

/* Ta_Eff_071 (branch coverage: the start call and stages 1 and 2 with dwPar zero and non-zero;
 * each wait taken and not taken; the closing wait as above). */
static void check_2759d0(void)
{
    struct jp_6989c in = {0, 0x304, 0x105, 0};
    int par;

    for (par = 0; par < 2; par++) {
        memset(&temp, 0, sizeof temp), openrate_side2 = 0, in.dwPar = par;
        assert(step(Ta_Eff_071, &in, 0) == 1 && log_n == 6 && is(0, WDRAW, 0, 0, 0, 0) && is(1, INIT, 0, 0, 0, 0) && log_p[1] == &temp);
        assert(is(2, DOOPEN, 3, 4, 1, 0) && is(3, FOCUS, 0, 0x304, 100, 180) && is(4, LOAD, 602, 0, 0, 0) && is(5, LOAD, par ? 500 : 513, 0, 0, 0));
        openrate_answer = 254, isload_answer = 1;
        assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 2 && is(1, OPENRATE, 3, 4, 0, 0) && temp.wLvl == 0);
        openrate_answer = 255, isload_answer = 0;
        assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 3 && is(2, ISLOAD, 0, 0, 0, 0) && temp.wLvl == 0);
        isload_answer = 1, do_answer = 7;
        assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 259) && is(4, DOEFF, 0, 0, 0, 0));
        assert(same_call(602, 0x304, 0) && temp.eID[0] == 7 && temp.wLvl == 1);
        isuse_answer[7] = 1;
        assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
        isuse_answer[7] = 0, do_answer = 9;
        assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 8 && is(2, USTAT, 3, 4, 0, 1) && is(3, FOCUS, 0, 0x105, 100, 180));
        assert(is(4, CARDNO, 3, 4, 0, 0) && is(5, MSG, 764, 0, 0, 0) && is(6, POPUP, 0, -120, 80, 0x4D5347) && is(7, DOEFF, 0, 0, 0, 0));
        assert(same_call(par ? 500 : 513, 0x105, 0) && temp.eID[0] == 9 && temp.wLvl == 2);
        isuse_answer[9] = 1;
        assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 2);
        isuse_answer[9] = 0;
        if (par) {
            assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 5 && is(2, CURSE, 1, 5, 255, 0) && is(3, CLOSE, 0, 0, 0, 0) && is(4, RESTORE, 16, 0, 0, 0));
        } else {
            assert(step(Ta_Eff_071, &in, -1) == 1 && log_n == 6 && is(2, USTAT, 1, 5, 3, 0) && is(3, USTAT, 1, 5, 5, 0));
            assert(is(4, CLOSE, 0, 0, 0, 0) && is(5, RESTORE, 16, 0, 0, 0));
        }
        assert(temp.wLvl == 3);
        check_closing_wait(Ta_Eff_071, &in);
    }
}

/* Ta_Eff_054 (branch coverage). Ta_ManageDestroyEffect is the real function of this group. */
enum { FOCUS3 = 1400, UCTL };
static int uctl_by_side;
int Ta_IsUnitControl(int a, int b) { note(UCTL, NULL, a, b, 0, 0); return uctl_by_side ? a : 3; }
void TaView_ReqFocusCamera3(unsigned char who, int packed, int add, int value) { note(FOCUS3, NULL, who, packed, add, value); }
static void check_054(void)
{
    /* dwFrom: side 1 slot 2; dwPar: kind 49, unit side 2 slot 5 */
    struct jp_6989c in = {54, 0x102, 0, 0x310205};

    memset(&temp, 0, sizeof(temp));
    status_once = 0, openrate_side2 = 0, num_by_value = 0, owner_by_n = 0;
    assert(step(Ta_Eff_054, &in, 0) == 1 && log_n == 9 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, DOOPEN, 2, 5, 1, 0) && is(3, LOAD, 34, 0, 0, 0) && is(4, LOAD, 555, 0, 0, 0) && is(5, LOAD, 557, 0, 0, 0));
    assert(is(6, LOAD, 602, 0, 0, 0) && is(7, LOAD, 603, 0, 0, 0) && is(8, FOCUS3, 0, 0x205, 180, 180) && temp.wLvl == 16);
    /* level 16 waits for the unit to be open and the modules, then effect 602 and popup 259 */
    openrate_answer = 254, isload_answer = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 2 && is(1, OPENRATE, 2, 5, 0, 0) && temp.wLvl == 16);
    openrate_answer = 255, isload_answer = 0;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 3 && log_id[2] == ISLOAD && log_p[2] == &temp && temp.wLvl == 16);
    isload_answer = 1, do_answer = 9;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 5 && log_id[3] == DOEFF && log_p[3] == &temp && is(4, POPUP, 0, -120, 80, 259));
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x205 && do_call.dwPar == 0 && temp.eID[0] == 9 && temp.wLvl == 0);
    in.dwPar = 0x040205, temp.wLvl = 16, do_answer = 8;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 5 && do_call.eNO == 602 && do_call.dwTo == 0x205 && do_call.dwPar == 1);
    assert(temp.eID[0] == 8 && temp.wLvl == 0);
    /* level 0 waits for that effect, marks the unit, names its card, deletes it unless the kind is 49,
     * and starts effect 34 on dwFrom */
    isuse_answer[8] = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 8, 0, 0, 0) && temp.wLvl == 0);
    isuse_answer[8] = 0, do_answer = 11;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 9 && is(2, USTAT, 2, 5, 0, 1) && is(3, CARDNO, 2, 5, 0, 0) && is(4, MSG, 745, 0, 0, 0));
    assert(is(5, POPUP, 0, -120, 80, 0x4D5347) && is(6, TDEL, 2, 5, 0, 0) && is(7, FOCUS, 0, 0x102, 100, 180) && log_id[8] == DOEFF);
    assert(do_call.eNO == 34 && do_call.dwFrom == 0 && do_call.dwTo == 0x102 && do_call.dwPar == 0 && temp.eID[0] == 11 && temp.wLvl == 1);
    in.dwPar = 0x310205, temp.wLvl = 0;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 8 && is(6, FOCUS, 0, 0x102, 100, 180) && log_id[7] == DOEFF && temp.wLvl == 1);
    /* level 1 waits for effect 34, then goes by kind: 49 starts effect 555 on dwTo */
    isuse_answer[11] = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 11, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[11] = 0, in.dwTo = 0x103, do_answer = 13, temp.oNum = 1, temp.ePos[1] = 7;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 4 && is(2, FOCUS, 0, 0x103, 100, 180) && log_id[3] == DOEFF);
    assert(do_call.eNO == 555 && do_call.dwFrom == 0 && do_call.dwTo == 0x103 && do_call.dwPar == 0);
    assert(temp.eID[1] == 13 && temp.ePos[1] == 1 && temp.wLvl == 2 && temp.oNum == 2);
    /* kind 48 goes straight to level 4; any other kind but 4 only waits */
    in.dwPar = 0x300205, temp.wLvl = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 4 && log_id[2] == CLOSE && is(3, RESTORE, 16, 0, 0, 0) && log_p[3] == &temp && temp.wLvl == 4);
    in.dwPar = 0x070205, temp.wLvl = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 2 && temp.wLvl == 1);
    /* kind 4 searches the targets; none: count to 17 and end */
    in.dwPar = 0x040205, in.dwTo = 0x55, search_answer = 0x888, remove2_answer = 0, temp.wCamTime = 16;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 6 && is(2, CURSIDE, 1, 2, 0, 0) && is(3, SEARCH, 6, 2, 53, 14));
    assert(is(4, UCTL, 2, 5, 0, 0) && is(5, REMOVE, 0x888, 3, 0, 0) && in.dwTo == 0 && temp.wCamTime == 17 && temp.wLvl == 1);
    assert(step(Ta_Eff_054, &in, -1) == 0 && log_n == 7 && log_id[6] == CLOSE && temp.wCamTime == 18 && temp.wLvl == 1);
    /* one target: camera on its owner; several: the standard view; then the dependent effects 555 */
    remove2_answer = 0x777, num_answer = 1, owner_answer = 0x105;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 10 && in.dwTo == 0x777 && is(6, NUM, 0x777, 0, 0, 0) && is(7, OWNER, 0x777, 0, 0, 0));
    assert(is(8, FOCUS, 0, 0x105, 100, 180) && is(9, DEPEND, 555, 0, 0, 0) && log_p[9] == &temp && depend_call.eNO == 999 && temp.wLvl == 2);
    num_answer = 2, temp.wLvl = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 9 && is(7, STDVIEW, 0, 0, 0, 0) && is(8, DEPEND, 555, 0, 0, 0) && temp.wLvl == 2);
    /* level 2, kind 4: Ta_ManageDestroyEffect decides (a started count only runs, then it picks at 17) */
    temp.oNum = 0, temp.wCount_2 = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 1 && temp.wCount_2 == 2 && temp.wLvl == 2);
    num_answer = 3, num_by_value = 1, owner_by_n = 1, gustat_zero_pos = 5, ukind_zero = 0, random_answer = 0x106, do_answer = 12;
    temp.wCount_2 = 17;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 29 && do_call.eNO == 603 && do_call.dwTo == 0x106 && temp.eID[0] == 12 && temp.wLvl == 3);
    num_by_value = 0, owner_by_n = 0, gustat_zero_pos = -1;
    /* level 2, another kind ends; kind 49 needs the last effect in use, at status 2, position not negative */
    in.dwPar = 0x070205, temp.wLvl = 2;
    assert(step(Ta_Eff_054, &in, -1) == 0 && log_n == 2 && log_id[1] == CLOSE && temp.wLvl == 2);
    in.dwPar = 0x310205, in.dwTo = 0x103, temp.oNum = 1, temp.eID[0] = 13, temp.ePos[0] = 0, isuse_answer[13] = 0;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 13, 0, 0, 0) && temp.wLvl == 2);
    isuse_answer[13] = 1, status_answer[13] = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 3 && is(2, STATUS, 13, 0, 0, 0) && temp.wLvl == 2);
    status_answer[13] = 2, temp.ePos[0] = -1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 3 && temp.wLvl == 2 && temp.oNum == 1);
    temp.ePos[0] = 0, temp.ePos[1] = 7, do_answer = 15;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 4 && log_id[3] == DOEFF && do_call.eNO == 557 && do_call.dwFrom == 0);
    assert(do_call.dwTo == 0x103 && do_call.dwPar == 0 && temp.eID[1] == 15 && temp.ePos[1] == 1 && temp.wLvl == 3 && temp.oNum == 2);
    /* level 3, kind 49: waits for every effect, then halves the target's attack and restores the view */
    isuse_answer[13] = 0, isuse_answer[15] = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 3 && is(1, ISUSE, 13, 0, 0, 0) && is(2, ISUSE, 15, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[15] = 0;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 7 && is(3, UAP, 1, 3, 0, 0) && is(4, AP, 1, 3, 1, 156) && log_id[5] == CLOSE);
    assert(is(6, RESTORE, 16, 0, 0, 0) && log_p[6] == &temp && temp.wLvl == 4);
    /* level 3, kind 4: waits for Ta_IsFinishDestEfc; another kind ends */
    in.dwPar = 0x040205, temp.wLvl = 3, findest_answer = 0;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 2 && log_id[1] == FINDEST && log_p[1] == &temp && findest_call.eNO == 999 && temp.wLvl == 3);
    findest_answer = 1;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 4 && log_id[2] == CLOSE && is(3, RESTORE, 16, 0, 0, 0) && temp.wLvl == 4);
    in.dwPar = 0x070205, temp.wLvl = 3;
    assert(step(Ta_Eff_054, &in, -1) == 0 && log_n == 2 && log_id[1] == CLOSE && temp.wLvl == 3);
    /* level 4 passes 16, restores the window, closes the popup and ends; other levels do nothing */
    temp.wLvl = 4, temp.wCamTime = 16;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 0;
    assert(step(Ta_Eff_054, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_054, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
    temp.wLvl = 5;
    assert(step(Ta_Eff_054, &in, -1) == 1 && log_n == 1 && temp.wLvl == 5 && temp.wCamTime == 19);
    status_answer[13] = 0, findest_answer = 0, remove2_answer = 0, search_answer = 0, openrate_answer = 0, disp_answer = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { REFLEX = 1000, LOADEFF, MSOUND, ENCH };
static int reflex_answer;
int TaEnc_IsActReflexEnc(int index) { note(REFLEX, NULL, index, 0, 0, 0); return reflex_answer; }
int Ta_Eff_036_LoadEffect(struct jp_6989c *eInfo, struct jp_69942 *Temp) { note(LOADEFF, Temp, (int)eInfo->dwPar, 0, 0, 0); return 1; }
void Ta_Eff_036_ManageSound(struct jp_6989c *eInfo, struct jp_69942 *Temp) { note(MSOUND, Temp, (int)eInfo->dwPar, 0, 0, 0); }
int Enchant(int a, int b, int c, int d) { note(ENCH, NULL, a, b, c, d); return 0; }

static int run_036(unsigned par, int id)
{
    struct jp_6989c e;

    e.eNO = 36, e.dwFrom = 0x0102, e.dwTo = 0x0203, e.dwPar = par;
    log_n = 0, trap_n = 0, ld_n = 0;
    return Ta_Eff_036(&e, &temp, id);
}

/* Ta_Eff_036: branch coverage, one case per outcome of each test in the start call and in
 * levels 0, 1, 16, 17 and the default. */
static void check_036(void)
{
    memset(&temp, 0, sizeof temp);
    memset(isuse_answer, 0, sizeof isuse_answer);
    memset(status_answer, 0, sizeof status_answer);
    status_once = 0, openrate_side2 = 0, loc_by_unit = 0, loc_answer = 0x0507, do_answer = 9;

    /* start, parameter 999, kind 0: both status bits set, module 538 */
    ukind_zero = 1, gustat_zero_pos = -1;
    assert(run_036(999, 0) == 1 && log_n == 8 && temp.fObj == 1);
    assert(is(0, INIT, 0, 0, 0, 0) && is(1, GUSTAT, 2, 3, 2, 0) && is(2, USTAT, 2, 3, 4, 1));
    assert(is(3, GUSTAT, 1, 2, 2, 0) && is(4, USTAT, 1, 2, 4, 1) && is(5, UKIND, 1, 2, 0, 0));
    assert(is(6, USTAT, 2, 3, 2, 1) && is(7, LOAD, 538, 0, 0, 0));
    /* target status clear, kind nonzero, no trap: wCount_1 cleared and the effect loaded */
    ukind_zero = 0, gustat_zero_pos = 3, trap_seq[0] = -1, temp.wCount_1 = 7;
    assert(run_036(5, 0) == 1 && log_n == 8 && temp.fObj == 2 && temp.wCount_1 == 0);
    assert(is(2, GUSTAT, 1, 2, 2, 0) && is(3, USTAT, 1, 2, 4, 1) && is(5, USTAT, 2, 3, 2, 1));
    assert(is(6, TRAPB, 1, 43, 0, 0) && is(7, LOADEFF, 5, 0, 0, 0) && log_p[7] == &temp);
    /* source status clear, trap found, no reflex: the second unit is the source for fObj 2 */
    gustat_zero_pos = 2, trap_seq[0] = 0x0304, reflex_answer = 0, temp.wCount_1 = 7;
    assert(run_036(5, 0) == 1 && log_n == 10 && temp.wCount_1 == 0);
    assert(is(2, USTAT, 2, 3, 4, 1) && is(3, GUSTAT, 1, 2, 2, 0) && is(4, UKIND, 1, 2, 0, 0));
    assert(is(7, CARDNO, 1, 2, 0, 0) && is(8, REFLEX, 722, 0, 0, 0) && is(9, LOADEFF, 5, 0, 0, 0));
    /* reflex, range answer 1: effect loaded; the range call gets the target and both location halves */
    gustat_zero_pos = -1, reflex_answer = 1, ld_answer = 1;
    assert(run_036(5, 0) == 1 && ld_n == 1 && is(log_n - 1, LOADEFF, 5, 0, 0, 0));
    assert(ld_args[0] == 2 && ld_args[1] == 3 && ld_args[2] == 7 && ld_args[3] == 5 && ld_args[4] == 11);
    assert(is(log_n - 3, LOC, 2, 3, 0, 0) && is(log_n - 2, LOC, 2, 3, 0, 0));
    /* range answer 0, second trap found; kind 0 swaps the two units */
    ukind_zero = 1, ld_answer = 0, trap_seq[1] = 0x0405;
    assert(run_036(5, 0) == 1 && temp.wLvl == 16 && temp.wCount_1 == 0 && temp.wCount_2 == 0x0405);
    assert(ld_args[0] == 1 && ld_args[1] == 2 && is(8, CARDNO, 2, 3, 0, 0) && is(9, REFLEX, 743, 0, 0, 0));
    assert(is(log_n - 5, LOAD, 602, 0, 0, 0) && is(log_n - 4, TRAPB, 2, 46, 0, 0));
    assert(is(log_n - 3, FOCUS, 0, 0x0405, 180, 180) && is(log_n - 2, BUSY2, 0x0405, 0, 0, 0));
    assert(is(log_n - 1, DOOPEN, 4, 5, 1, 0));
    /* second trap missing: the first one is opened */
    trap_seq[1] = -1;
    assert(run_036(5, 0) == 1 && temp.wLvl == 16 && temp.wCount_1 == 1 && temp.wCount_2 == 0x0304);
    assert(is(log_n - 3, FOCUS, 0, 0x0304, 180, 180) && is(log_n - 2, BUSY2, 0x0304, 0, 0, 0));
    assert(is(log_n - 1, DOOPEN, 3, 4, 1, 0));

    /* level 0 */
    memset(&temp, 0, sizeof temp);
    isload_answer = 0;
    assert(run_036(999, -1) == 1 && log_n == 1 && is(0, ISLOAD, 0, 0, 0, 0) && temp.wLvl == 0);
    isload_answer = 1, temp.wCount_1 = 1, temp.fObj = 2, temp.oNum = 1, temp.eMod.wModule[1] = 538;
    assert(run_036(999, -1) == 1 && log_n == 4 && is(1, MSG, 816, 0, 0, 0));
    assert(is(2, POPUP, 0, 0, -160, 0x4D5347) && is(3, DOEFF, 0, 0, 0, 0) && log_p[3] == &temp);
    assert(do_call.eNO == 538 && do_call.dwFrom == 0x0102 && do_call.dwTo == 0x0203 && do_call.dwPar == 2);
    assert(temp.eID[1] == 9 && temp.ePos[1] == 0 && temp.oNum == 2 && temp.wLvl == 1);
    temp.wLvl = 0, temp.wCount_1 = 0, temp.fObj = 1, reflex_answer = 1, temp.ePos[2] = 5;
    assert(run_036(5, -1) == 1 && log_n == 5 && is(1, CARDNO, 2, 3, 0, 0) && is(2, REFLEX, 743, 0, 0, 0));
    assert(is(3, POPUP, 0, 0, -160, 262) && do_call.dwPar == 1 && temp.ePos[2] == 0 && temp.oNum == 3);
    temp.wLvl = 0, temp.fObj = 2, reflex_answer = 0;
    assert(run_036(5, -1) == 1 && log_n == 4 && is(1, CARDNO, 1, 2, 0, 0) && is(3, DOEFF, 0, 0, 0, 0));

    /* level 1 */
    memset(&temp, 0, sizeof temp);
    temp.wLvl = 1, temp.oNum = 1, temp.eID[0] = 3, isuse_answer[3] = 0, gustat_zero_pos = 3;
    assert(run_036(999, -1) == 0 && log_n == 7 && is(0, ISUSE, 3, 0, 0, 0) && is(1, CLOSE, 0, 0, 0, 0));
    assert(is(2, TDEL, 1, 2, 0, 0) && is(3, GUSTAT, 2, 3, 4, 0) && is(4, USTAT, 2, 3, 2, 0));
    assert(is(5, SE, 0, 2, 0, 0) && is(6, CLOSE, 0, 0, 0, 0));
    gustat_zero_pos = -1;
    assert(run_036(999, -1) == 0 && log_n == 6 && is(4, SE, 0, 2, 0, 0));
    assert(run_036(5, -1) == 0 && log_n == 4 && is(2, SE, 0, 2, 0, 0) && is(3, CLOSE, 0, 0, 0, 0));
    isuse_answer[3] = 1, status_answer[3] = 1;
    assert(run_036(5, -1) == 1 && log_n == 3 && is(1, MSOUND, 5, 0, 0, 0) && is(2, STATUS, 3, 0, 0, 0));
    status_answer[3] = 2, temp.ePos[0] = 1;
    assert(run_036(5, -1) == 1 && log_n == 3 && temp.ePos[0] == 1);
    temp.ePos[0] = 0x100;
    assert(run_036(999, -1) == 1 && log_n == 3 && temp.ePos[0] == 0x100);
    ukind_zero = 1, temp.wCount_1 = 1, gustat_zero_pos = 2;
    assert(run_036(0x4005, -1) == 1 && log_n == 8 && is(3, UKIND, 1, 2, 0, 0));
    assert(is(4, ENCH, 1, 2, 0x10005, 0) && is(5, TDEL, 2, 3, 0, 0) && is(6, GUSTAT, 1, 2, 4, 0));
    assert(is(7, USTAT, 1, 2, 2, 0) && temp.ePos[0] == 0xFFFF);
    temp.ePos[0] = 0, gustat_zero_pos = -1;
    assert(run_036(0x4005, -1) == 1 && log_n == 7 && is(6, GUSTAT, 1, 2, 4, 0) && temp.ePos[0] == 0xFFFF);
    ukind_zero = 0, temp.ePos[0] = 0, temp.wCount_1 = 0;
    assert(run_036(0x4005, -1) == 1 && log_n == 7 && is(4, ENCH, 2, 3, 5, 0) && is(5, TDEL, 1, 2, 0, 0));
    assert(is(6, GUSTAT, 2, 3, 4, 0) && temp.ePos[0] == 0xFFFF);
    temp.ePos[0] = 0, gustat_zero_pos = 3;
    assert(run_036(0x4005, -1) == 1 && log_n == 8 && is(7, USTAT, 2, 3, 2, 0) && temp.ePos[0] == 0xFFFF);

    /* level 16 */
    memset(&temp, 0, sizeof temp);
    temp.wLvl = 16, temp.wCount_2 = 0x0405, openrate_answer = 254, isload_answer = 1;
    assert(run_036(5, -1) == 1 && log_n == 1 && is(0, OPENRATE, 4, 5, 0, 0) && temp.wLvl == 16);
    openrate_answer = 255, isload_answer = 0;
    assert(run_036(5, -1) == 1 && log_n == 2 && is(1, ISLOAD, 0, 0, 0, 0) && temp.wLvl == 16);
    isload_answer = 1, temp.wCount_1 = 1, temp.ePos[0] = 5;
    assert(run_036(5, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 259) && is(3, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x0405 && do_call.dwPar == 0);
    assert(temp.wLvl == 17 && temp.eID[0] == 9 && temp.ePos[0] == 0 && temp.oNum == 1);
    temp.wLvl = 16, temp.wCount_1 = 0;
    assert(run_036(5, -1) == 1 && log_n == 5 && is(2, MSG, 825, 0, 0, 0) && is(3, POPUP, 0, -120, 80, 0x4D5347));
    assert(temp.wLvl == 17 && temp.eID[1] == 9 && temp.oNum == 2);

    /* level 17 and the default */
    isuse_answer[9] = 1;
    assert(run_036(5, -1) == 1 && log_n == 1 && is(0, ISUSE, 9, 0, 0, 0) && temp.wLvl == 17);
    isuse_answer[9] = 0;
    assert(run_036(5, -1) == 1 && log_n == 4 && is(1, USTAT, 4, 5, 0, 1) && is(2, LOADEFF, 5, 0, 0, 0));
    assert(is(3, RESTORE, 0, 0, 0, 0) && log_p[3] == &temp && temp.wLvl == 0);
    temp.wLvl = 5;
    assert(run_036(5, -1) == 1 && log_n == 0 && temp.wLvl == 5);
    memset(isuse_answer, 0, sizeof isuse_answer);
    memset(status_answer, 0, sizeof status_answer);
    ukind_zero = 0, gustat_zero_pos = -1;
}

enum { GETLP = 1300 };
int SzDuel_GetLP(int side) { note(GETLP, NULL, side, 0, 0, 0); return getlp_zero ? 0 : 3000; }
static int run_008(unsigned par, int id)
{
    struct jp_6989c e;

    e.eNO = 8, e.dwFrom = 0, e.dwTo = 1, e.dwPar = par;
    log_n = 0, trap_n = 0;
    return Ta_Eff_008(&e, &temp, id);
}

/* Ta_Eff_008 (branch coverage). */
static void check_008(void)
{
    static const int waits[3] = { 2, 4, 17 };
    int i;

    memset(&temp, 0, sizeof temp);
    memset(isuse_answer, 0, sizeof isuse_answer);
    openrate_side2 = 0, do_answer = 9, disp_answer = 0, gustat_zero_pos = -1;

    /* start, no trap 42: level 32; the amount comes from the natural-effect revision */
    trap_seq[0] = -1;
    assert(run_008(700, 0) == 1 && log_n == 5 && is(0, LPREV, 43, 700, 0, 0) && is(1, WDRAW, 0, 0, 0, 0));
    assert(log_id[2] == INIT && log_p[2] == &temp && is(3, LOAD, 508, 0, 0, 0) && is(4, TRAPB, 1, 42, 0, 0));
    assert(temp.wLvl == 32);
    /* parameter kind 4: the amount is the low half less the side's life points */
    assert(run_008(0x40000 | 3500, 0) == 1 && log_n == 5 && is(0, GETLP, 1, 0, 0, 0) && is(1, WDRAW, 0, 0, 0, 0));
    /* trap 42 without trap 46: its position kept, modules 602 and 548, level 64 */
    trap_seq[0] = 0x0304, trap_seq[1] = -1;
    assert(run_008(700, 0) == 1 && log_n == 8 && is(5, TRAPB, 1, 46, 0, 0) && is(6, LOAD, 602, 0, 0, 0));
    assert(is(7, LOAD, 548, 0, 0, 0) && temp.wCount_2 == 0x0304 && temp.wLvl == 64);
    /* both traps: trap 46 is focused, kept and opened, level 16 */
    trap_seq[1] = 0x0506;
    assert(run_008(700, 0) == 1 && log_n == 9 && is(6, LOAD, 602, 0, 0, 0) && is(7, FOCUS, 0, 0x0506, 180, 180));
    assert(is(8, DOOPEN, 5, 6, 1, 0) && temp.wCount_2 == 0x0506 && temp.wLvl == 16 && temp.wCount_1 == 1);

    /* modules not loaded: nothing happens */
    memset(&temp, 0, sizeof temp);
    isload_answer = 0, temp.wLvl = 3;
    assert(run_008(700, -1) == 1 && log_n == 3 && log_id[2] == ISLOAD && log_p[2] == &temp && temp.wLvl == 3);
    isload_answer = 1;

    /* level 0: waits for the last effect, then adds the amount and ends */
    temp.wLvl = 0, temp.oNum = 1, temp.eID[0] = 3, isuse_answer[3] = 1;
    assert(run_008(700, -1) == 1 && log_n == 4 && is(3, ISUSE, 3, 0, 0, 0));
    isuse_answer[3] = 0;
    assert(run_008(700, -1) == 0 && log_n == 8 && is(4, CALCLP, 1, 0, 0x1234, 0) && log_id[5] == CLOSE);
    assert(log_id[6] == WDISP && log_id[7] == CLOSE);
    disp_answer = 1;
    assert(run_008(0x40000 | 3500, -1) == 0 && log_n == 9 && is(4, CALCLP, 1, 0, 500, 0) && is(7, WDRAW, 1, 0, 0, 0));
    /* a negative amount is clamped to 0 */
    assert(run_008(0x40000 | 2500, -1) == 0 && is(4, CALCLP, 1, 0, 0, 0));

    /* level 1: waits for the open rate (opening a unit whose status bit 4 is clear), then effect 602 */
    temp.wLvl = 1, temp.wCount_2 = 0x0506, openrate_answer = 254;
    assert(run_008(700, -1) == 1 && log_n == 5 && is(3, OPENRATE, 5, 6, 0, 0) && is(4, GUSTAT, 5, 6, 4, 0));
    gustat_zero_pos = 6;
    assert(run_008(700, -1) == 1 && log_n == 6 && is(5, DOOPEN, 5, 6, 1, 0) && temp.wLvl == 1);
    openrate_answer = 255;
    assert(run_008(700, -1) == 1 && log_n == 8 && is(4, FOCUS, 0, 0x0506, 180, 180) && is(5, MSG, 815, 0, 0, 0));
    assert(is(6, POPUP, 0, -120, 80, 0x4D5347) && log_id[7] == DOEFF && log_p[7] == &temp);
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x0506 && do_call.dwPar == 0);
    assert(temp.eID[1] == 9 && temp.oNum == 2 && temp.wLvl == 2);

    /* levels 2, 4 and 17: wait for the last effect; all but 4 set the unit's status bit 0 */
    for (i = 0; i < 3; i++) {
        temp.wLvl = waits[i], isuse_answer[9] = 1;
        assert(run_008(700, -1) == 1 && log_n == 4 && is(3, ISUSE, 9, 0, 0, 0) && temp.wLvl == waits[i]);
        isuse_answer[9] = 0;
        assert(run_008(700, -1) == 1 && log_n == (waits[i] == 4 ? 4 : 5) && temp.wLvl == waits[i] + 1);
        assert(waits[i] == 4 || is(4, USTAT, 5, 6, 0, 1));
    }

    /* level 3: popup 267 and effect 548 with the amount */
    temp.wLvl = 3;
    assert(run_008(700, -1) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 267) && log_id[4] == DOEFF);
    assert(do_call.eNO == 548 && do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0x1234);
    assert(temp.eID[2] == 9 && temp.oNum == 3 && temp.wLvl == 4);

    /* level 5: status bit 0, the amount subtracted, view restored, camera time cleared */
    temp.wLvl = 5, temp.wCamTime = 7;
    assert(run_008(700, -1) == 1 && log_n == 7 && log_id[3] == CLOSE && is(4, USTAT, 5, 6, 0, 1));
    assert(is(5, CALCLP, 1, 1, 0x1234, 0) && is(6, RESTORE, 16, 0, 0, 0) && log_p[6] == &temp);
    assert(temp.wLvl == 6 && temp.wCamTime == 0);

    /* level 6: counts camera time; ends once the count read is above 16 */
    temp.wCamTime = 16;
    assert(run_008(700, -1) == 1 && log_n == 3 && temp.wCamTime == 17 && temp.wLvl == 6);
    assert(run_008(700, -1) == 0 && log_n == 6 && log_id[3] == WDISP && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
    disp_answer = 0, temp.wCamTime = 17;
    assert(run_008(700, -1) == 0 && log_n == 5 && log_id[4] == CLOSE && temp.wCamTime == 18);

    /* level 16: as level 1 without the camera request, message 825 */
    temp.wLvl = 16, openrate_answer = 254, gustat_zero_pos = -1;
    assert(run_008(700, -1) == 1 && log_n == 5 && is(4, GUSTAT, 5, 6, 4, 0) && temp.wLvl == 16);
    gustat_zero_pos = 6;
    assert(run_008(700, -1) == 1 && log_n == 6 && is(5, DOOPEN, 5, 6, 1, 0));
    openrate_answer = 255;
    assert(run_008(700, -1) == 1 && log_n == 7 && is(4, MSG, 825, 0, 0, 0) && is(5, POPUP, 0, -120, 80, 0x4D5347));
    assert(do_call.eNO == 602 && do_call.dwTo == 0x0506 && do_call.dwPar == 0 && temp.oNum == 4 && temp.wLvl == 17);

    /* level 18: popup 266, effect 508 with the amount, back to level 0 */
    temp.wLvl = 18;
    assert(run_008(700, -1) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 266) && log_id[4] == DOEFF);
    assert(do_call.eNO == 508 && do_call.dwTo == 0 && do_call.dwPar == 0x1234 && temp.oNum == 5 && temp.wLvl == 0);
    /* level 32: the effect first, then the popup */
    temp.wLvl = 32;
    assert(run_008(700, -1) == 1 && log_n == 5 && log_id[3] == DOEFF && is(4, POPUP, 0, -120, 80, 266));
    assert(do_call.eNO == 508 && do_call.dwPar == 0x1234 && temp.eID[5] == 9 && temp.oNum == 6 && temp.wLvl == 0);
    /* level 64: the amount flagged with 0x10000, popup, level 65, effect */
    temp.wLvl = 64;
    assert(run_008(700, -1) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 266) && log_id[4] == DOEFF);
    assert(do_call.eNO == 508 && do_call.dwPar == 0x11234 && temp.oNum == 7 && temp.wLvl == 65);
    /* level 65: waits for the last effect, then level 1 */
    isuse_answer[9] = 1;
    assert(run_008(700, -1) == 1 && log_n == 4 && temp.wLvl == 65);
    isuse_answer[9] = 0;
    assert(run_008(700, -1) == 1 && log_n == 4 && temp.wLvl == 1);
    /* any other level is idle */
    temp.wLvl = 7;
    assert(run_008(700, -1) == 1 && log_n == 3 && temp.wLvl == 7);
}

/* Ta_Eff_ETC (branch coverage): start call, level 0 with modules not loaded / loaded, level 1
 * with the effect still in use / finished. */
static void check_etc(void)
{
    struct jp_6989c in = {600, 0x11, 0x200C01, 0x33};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_ETC, &in, 0) == 1 && log_n == 2 && log_id[0] == INIT && log_p[0] == &temp && is(1, LOAD, 513, 0, 0, 0));
    isload_answer = 0;
    assert(step(Ta_Eff_ETC, &in, -1) == 1 && log_n == 1 && log_id[0] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 7, temp.oNum = 2, temp.eID[0] = 4;
    assert(step(Ta_Eff_ETC, &in, -1) == 1 && log_n == 3 && is(1, POPUP, 0, -120, 80, 260) && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 513 && do_call.dwFrom == 0 && do_call.dwTo == 0x200C01 && do_call.dwPar == 0);
    assert(temp.wLvl == 1 && temp.eID[2] == 7 && temp.eID[0] == 4 && temp.oNum == 2);
    isuse_answer[4] = 1;
    assert(step(Ta_Eff_ETC, &in, -1) == 1 && log_n == 1 && is(0, ISUSE, 4, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[4] = 0;
    assert(step(Ta_Eff_ETC, &in, -1) == 0 && log_n == 2 && is(0, ISUSE, 4, 0, 0, 0) && log_id[1] == CLOSE);
    isload_answer = 0, do_answer = 0;
}

enum { AUTOBLUR = 900 };
void SzGame_SetAutoBlur(int value) { note(AUTOBLUR, NULL, value, 0, 0, 0); }

/* Ta_Eff_072 (branch and loop coverage): start call; level 0 fall, clamp, wait for 30 frames
 * and for the modules; level 1 wait; level 2 end of range, no unit, units found; level 3
 * wait, move and both exits; level 4 rise, frame wait, both window answers; other level. */
static void check_072(void)
{
    struct jp_6989c in = {72, 0x305, 0x77, 1};
    static const int none[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_072, &in, 0) == 1 && log_n == 29 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT);
    assert(is(2, BUSY2, 0, 1, 0, 0) && is(3, BUSY2, 0x100, 1, 0, 0) && is(22, BUSY2, 10, 1, 0, 0) && is(23, BUSY2, 0x10a, 1, 0, 0));
    assert(is(24, FOCUS, 30, 0x305, 100, 180) && is(25, LOAD, 513, 0, 0, 0) && is(26, LOAD, 572, 0, 0, 0));
    assert(is(27, STDVIEW, 30, 0, 0, 0) && is(28, AUTOBLUR, 4, 0, 0, 0) && temp.wCount_1 == 128 && temp.wCount_2 == 2);
    /* level 0 */
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0) && temp.wCamTime == 1);
    temp.wCount_1 = 40, temp.wCamTime = 5;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    temp.wCamTime = 29, isload_answer = 0;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 3 && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1, do_answer = 7;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 4 && log_id[2] == DOEFF && is(3, BLACK, 48, 0, 0, 0));
    assert(do_call.eNO == 513 && do_call.dwFrom == 0 && do_call.dwTo == 0x305 && do_call.dwPar == 0);
    assert(temp.eID[0] == 7 && temp.ePos[0] == 0x305 && temp.wLvl == 1);
    /* level 1 */
    isuse_answer[7] = 1;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[7] = 0;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 3 && is(2, STDVIEW, 0, 0, 0, 0) && temp.wLvl == 2);
    /* level 2: no unit at the distance, units found, end of the range */
    num_answer = 0, temp.wCount_2 = 5, temp.oNum = 3;
    assert(step(Ta_Eff_072, &in, -1) == 1 && temp.wLvl == 2 && temp.wCount_2 == 6 && temp.oNum == 0);
    num_by_value = 1, num_answer = 4, owner_by_n = 1, owner_answer = 0x100, loc_by_unit = 1, do_answer = 9;
    minorder_answer[0] = -1, minorder_answer[1] = 0x102, minorder_answer[2] = 0x105, minorder_answer[3] = 0x106;
    reserve_answer[0] = -1, reserve_answer[1] = 0x406, reserve_answer[2] = 0x407;
    temp.wCount_2 = 3;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 24 && temp.wLvl == 3 && temp.wCount_2 == 3 && temp.oNum == 2);
    assert(temp.eID[0] == 9 && temp.eID[1] == 9 && temp.ePos[0] == 0x1050406 && temp.ePos[1] == 0x1060407);
    num_by_value = 0, owner_by_n = 0, loc_by_unit = 0;
    memcpy(minorder_answer, none, sizeof(none));
    /* level 3 */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 3);
    isuse_answer[9] = 0, move_n = 0;
    assert(step(Ta_Eff_072, &in, -1) == 1 && is(1, ISUSE, 9, 0, 0, 0) && is(2, ISUSE, 9, 0, 0, 0) && log_n == 3);
    assert(move_n == 2 && moved(0, 1, 5, 6, 4) && moved(1, 1, 6, 7, 4));
    assert(temp.wLvl == 2 && temp.wCount_2 == 4);
    temp.wLvl = 3, temp.wCount_2 = 14;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 3 && move_n == 4 && temp.wLvl == 4 && temp.wCount_2 == 14);
    temp.wLvl = 2, temp.wCamTime = 9;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 3 && is(1, RESTORE, 16, 0, 0, 0) && is(2, AUTOBLUR, 0, 0, 0, 0));
    assert(temp.wLvl == 4 && temp.wCamTime == 0 && temp.oNum == 0);
    /* level 4 */
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wCamTime == 0);
    temp.wCount_1 = 120, temp.wCamTime = 16;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 128, 0, 0, 0) && temp.wCamTime == 17);
    disp_answer = 1;
    assert(step(Ta_Eff_072, &in, -1) == 0 && log_n == 27 && is(2, BUSY2, 0, 0, 0, 0) && is(23, BUSY2, 0x10a, 0, 0, 0));
    assert(log_id[24] == WDISP && is(25, WDRAW, 1, 0, 0, 0) && log_id[26] == CLOSE);
    disp_answer = 0, temp.wCamTime = 17;
    assert(step(Ta_Eff_072, &in, -1) == 0 && log_n == 26 && log_id[25] == CLOSE);
    /* any other level */
    temp.wLvl = 9;
    assert(step(Ta_Eff_072, &in, -1) == 1 && log_n == 1 && temp.wLvl == 9);
    isload_answer = 0, do_answer = 0;
    memset(&temp, 0, sizeof(temp));
}

enum { FINEFF = 1500 };
static int fineff_answer, mapunit_at, mapunit_n;
int Ta_IsFinishEffect(struct jp_69942 *Temp) { note(FINEFF, Temp, 0, 0, 0, 0); return fineff_answer; }
void *SzDuel_GetMapUnit(int x, int y)
{
    mapunit_n++;
    return mapunit_at == -2 || mapunit_at == ((y << 8) | x) ? &temp : NULL;
}

/* Level 1 of Ta_Eff_046 from a fresh Temp; the source unit stands at x 0, y 6, so only the
 * cells (0,5), (1,5) and (1,6) are inside the 7 x 7 map. */
static int step_046_cells(struct jp_6989c *in)
{
    int i;

    memset(&temp, 0, sizeof(temp));
    for (i = 0; i < 8; i++) {
        temp.ePos[i] = 0x7777;
    }
    temp.wLvl = 1, temp.wCount_1 = 9;
    mapunit_n = 0;
    return step(Ta_Eff_046, in, -1);
}

/* Ta_Eff_046 (branch and loop coverage): start call; level 0 fall, clamp, open-rate and module
 * waits, both popups; level 1 wait, map bounds, the centre cell, no unit, parameter 1, same and
 * other controller, range ability, resistance, target among the cells or not; level 2 sounds on
 * frame 1 and 5, effect unused / not finished / finished with and without a card, both ends;
 * level 3 wait, deletions, no target; level 4 rise and clamp; other level waits and both
 * window answers. */
static void check_046(void)
{
    struct jp_6989c in = {46, 0x203, 0x104, 1};

    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    status_once = 0, loc_by_unit = 0, do_answer = 9;
    assert(step(Ta_Eff_046, &in, 0) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT);
    assert(is(2, DOOPEN, 2, 3, 1, 0) && is(3, FOCUS, 0, 0x203, 100, 180) && temp.wCount_1 == 128);
    assert(is(4, LOAD, 602, 0, 0, 0) && is(5, LOAD, 603, 0, 0, 0) && is(6, LOAD, 546, 0, 0, 0));
    /* level 0 */
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 113, 0, 0, 0));
    temp.wCount_1 = 50;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 48, 0, 0, 0) && temp.wCount_1 == 48);
    openrate_answer = 254, openrate_side2 = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 3 && is(1, OPENRATE, 2, 3, 0, 0) && is(2, BLACK, 48, 0, 0, 0));
    openrate_answer = 255, isload_answer = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 4 && log_id[2] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 6 && is(3, POPUP, 0, -120, 80, 260) && log_id[4] == DOEFF);
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x203 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.wLvl == 1 && is(5, BLACK, 48, 0, 0, 0));
    temp.wLvl = 0, in.dwPar = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 6 && is(3, POPUP, 0, -120, 80, 259) && temp.wLvl == 1);
    /* level 1: wait for the first effect */
    isuse_answer[9] = 1;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0;
    /* no unit on any cell, parameter 1: three cell effects, nothing else */
    loc_answer = 0x600, mapunit_at = -1, mapid_answer = 0x104, in.dwPar = 1, do_answer = 7;
    assert(step_046_cells(&in) == 1 && log_n == 9 && is(1, ISUSE, 0, 0, 0, 0) && is(2, USTAT, 2, 3, 0, 1));
    assert(is(3, FOCUS, 16, 0x203, 500, 500) && is(4, LOC, 2, 3, 0, 0) && log_id[5] == DOEFF && log_id[8] == DOEFF);
    assert(mapunit_n == 3 && temp.ePos[0] == 0x203 && temp.ePos[1] == -1 && temp.ePos[2] == -1 && temp.ePos[3] == -1);
    assert(temp.oNum == 4 && temp.wCount_1 == 1 && temp.wLvl == 2 && temp.eID[0] == 7 && temp.eID[3] == 7);
    assert(do_call.eNO == 546 && do_call.dwFrom == 0 && do_call.dwTo == 0x601 && do_call.dwPar == 0);
    assert(do_prev.dwTo == 0x501 && temp.ePos[4] == 0x7777);
    /* no unit, parameter 0: the target gets its own effect 603 */
    in.dwPar = 0;
    assert(step_046_cells(&in) == 1 && log_n == 10 && log_id[9] == DOEFF && temp.oNum == 5 && temp.ePos[4] == -1);
    assert(do_call.eNO == 603 && do_call.dwFrom == 0 && do_call.dwTo == 0x104 && do_call.dwPar == 1 && temp.eID[4] == 7);
    /* a unit on one cell only, parameter 1: taken without any test */
    in.dwPar = 1, mapunit_at = 0x501;
    assert(step_046_cells(&in) == 1 && log_n == 9 && temp.ePos[1] == -1 && temp.ePos[2] == 0x104 && temp.ePos[3] == -1);
    assert(temp.wCount_1 == 2 && temp.oNum == 4);
    /* units everywhere, parameter 0, same controller: taken, the target is among them */
    in.dwPar = 0, mapunit_at = -2, uctl_by_side = 0;
    assert(step_046_cells(&in) == 1 && log_n == 15 && is(7, UCTL, 1, 4, 0, 0) && is(8, UCTL, 2, 3, 0, 0));
    assert(temp.ePos[1] == 0x104 && temp.ePos[3] == 0x104 && temp.wCount_1 == 4 && temp.oNum == 4 && temp.wLvl == 2);
    /* same controller, the target is not among them */
    mapid_answer = 0x105;
    assert(step_046_cells(&in) == 1 && log_n == 16 && temp.ePos[3] == 0x105 && temp.oNum == 5 && do_call.eNO == 603);
    /* other controller: range ability keeps the unit out */
    uctl_by_side = 1, ldrange_answer = 1, resist_answer = 0, mapid_answer = 0x104;
    assert(step_046_cells(&in) == 1 && log_n == 19 && is(9, LDRANGE, 1, 4, 11, 0) && log_id[10] == DOEFF);
    assert(temp.ePos[1] == 0x7777 && temp.wCount_1 == 1 && temp.oNum == 5 && temp.ePos[4] == -1);
    /* resistance keeps the unit out */
    ldrange_answer = 0, resist_answer = 1;
    assert(step_046_cells(&in) == 1 && log_n == 22 && is(10, RESIST, 1, 4, 0, 0) && temp.ePos[1] == 0x7777);
    assert(temp.wCount_1 == 1 && temp.oNum == 5);
    /* neither: taken; target among them, then not */
    resist_answer = 0;
    assert(step_046_cells(&in) == 1 && log_n == 21 && temp.ePos[1] == 0x104 && temp.wCount_1 == 4 && temp.oNum == 4);
    mapid_answer = 0x105;
    assert(step_046_cells(&in) == 1 && log_n == 22 && temp.ePos[3] == 0x105 && temp.wCount_1 == 4 && temp.oNum == 5);
    uctl_by_side = 0, mapunit_at = -1;
    /* level 2 */
    memset(&temp, 0, sizeof(temp));
    temp.wLvl = 2, temp.oNum = 2, temp.wCount_1 = 2, temp.eID[0] = 4, temp.eID[1] = 5;
    temp.ePos[0] = 0x105, temp.ePos[1] = 0x200;
    isuse_answer[4] = isuse_answer[5] = 1, status_answer[4] = status_answer[5] = 2;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 8 && is(1, SE, 1, 49, 0, 0) && is(2, ISUSE, 4, 0, 0, 0));
    assert(is(3, STATUS, 4, 0, 0, 0) && is(4, UFADE, 1, 5, -1, 1) && is(5, SE, 1, 49, 0, 0) && is(7, STATUS, 5, 0, 0, 0));
    assert(temp.fObj2 == 1 && temp.ePos[0] == -1 && temp.ePos[2] == 0x105 && temp.ePos[1] == 0x200);
    assert(temp.wCount_2 == 2 && temp.wLvl == 3);
    memset(&temp, 0, sizeof(temp));
    temp.wLvl = 2, temp.oNum = 1, temp.wCount_1 = 1, temp.eID[0] = 6, temp.fObj2 = 4;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 6 && is(4, SE, 1, 49, 0, 0) && is(5, ISUSE, 6, 0, 0, 0));
    assert(temp.fObj2 == 5 && temp.wCount_2 == 0 && temp.wLvl == 2);
    temp.eID[0] = 4, status_answer[4] = 1;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 3 && is(2, STATUS, 4, 0, 0, 0) && temp.wCount_2 == 0);
    status_answer[4] = 2, temp.ePos[0] = -1;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 3 && temp.wCount_2 == 0 && temp.wLvl == 2);
    temp.wCount_1 = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 3 && temp.wLvl == 3);
    isuse_answer[4] = isuse_answer[5] = 0, status_answer[4] = status_answer[5] = 0;
    /* level 3 */
    memset(&temp, 0, sizeof(temp));
    temp.wLvl = 3, temp.oNum = 1, temp.wCount_2 = 3, temp.wCount_1 = 5;
    temp.ePos[0] = 0x606, temp.ePos[1] = 0x305, temp.ePos[2] = -1, temp.ePos[3] = 0x400, temp.ePos[4] = 0x606;
    finfade_answer = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 2 && log_id[1] == FINFADE && temp.wLvl == 3);
    finfade_answer = 1;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 4 && is(2, TDEL, 3, 5, 0, 0) && is(3, TDEL, 1, 4, 0, 0));
    assert(temp.wCount_1 == 48 && temp.wLvl == 4);
    temp.wLvl = 3, in.dwTo = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 3 && is(2, TDEL, 3, 5, 0, 0) && temp.wLvl == 4);
    in.dwTo = 0x104;
    /* level 4 */
    temp.wCount_1 = 100;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 2 && is(1, BLACK, 115, 0, 0, 0) && temp.wLvl == 4);
    temp.wCount_1 = 120, temp.wCamTime = 5;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 4 && is(1, BLACK, 128, 0, 0, 0) && log_id[2] == CLOSE);
    assert(is(3, RESTORE, 16, 0, 0, 0) && temp.wCount_1 == 128 && temp.wLvl == 5 && temp.wCamTime == 0);
    /* other level */
    temp.wCamTime = 16, fineff_answer = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 2 && log_id[1] == FINEFF && log_p[1] == &temp && temp.wCamTime == 18);
    fineff_answer = 1, finfade_answer = 0;
    assert(step(Ta_Eff_046, &in, -1) == 1 && log_n == 3 && log_id[2] == FINFADE);
    finfade_answer = 1, disp_answer = 0;
    assert(step(Ta_Eff_046, &in, -1) == 0 && log_n == 5 && log_id[3] == WDISP && log_id[4] == CLOSE);
    disp_answer = 1;
    assert(step(Ta_Eff_046, &in, -1) == 0 && log_n == 6 && is(4, WDRAW, 1, 0, 0, 0) && log_id[5] == CLOSE);
    disp_answer = 0, finfade_answer = 0, fineff_answer = 0, isload_answer = 0, do_answer = 0, openrate_answer = 0;
    loc_answer = 0, mapid_answer = 0;
}

enum { RMASTER = 1600 };
static int rmaster_answer;
int Ta_IsResistAllMaster(int a, int b) { note(RMASTER, NULL, a, b, 0, 0); return rmaster_answer; }

static int step_026(struct jp_6989c *in, int lvl)
{
    temp.wLvl = (unsigned short)lvl;
    return step(Ta_Eff_026, in, -1);
}

/* Ta_Eff_026 (branch coverage): parameter -1 on both entries; start; level 0 both load answers;
 * levels 1 and 3 busy and free; level 2 list and single; level 4 single unit (range ability,
 * resist-all, own side, both status answers, curse and spell rests) and list (bit positions 0,
 * 10, 11 and 21, range ability, same side, both status answers, turn owner or not, rests, and
 * the wait behind the loop, reached only when a callee lowers the level); level 16 busy, both
 * count outcomes; other level wait and both window answers. */
static void check_026(void)
{
    struct jp_6989c in = {26, 5, 0, 0xFFFFFFFF};

    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    loc_by_unit = 0, gustat_zero_pos = -1, num_by_value = 0, owner_by_n = 0, uctl_by_side = 0;
    assert(step(Ta_Eff_026, &in, 0) == 0 && log_n == 2 && log_id[0] == INIT && log_p[0] == &temp && log_id[1] == CLOSE);
    assert(step(Ta_Eff_026, &in, -1) == 0 && log_n == 1 && log_id[0] == CLOSE);
    in.dwPar = 0x105;
    assert(step(Ta_Eff_026, &in, 0) == 1 && log_n == 6 && log_id[0] == INIT && is(1, WDRAW, 0, 0, 0, 0) && is(2, STDVIEW, 16, 0, 0, 0));
    assert(is(3, LOAD, 513, 0, 0, 0) && is(4, LOAD, 26, 0, 0, 0) && is(5, LOAD, 27, 0, 0, 0) && log_p[5] == &temp);
    /* level 0 */
    isload_answer = 0, do_answer = 9;
    assert(step_026(&in, 0) == 1 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1;
    assert(step_026(&in, 0) == 1 && log_n == 3 && log_id[2] == DOEFF && log_p[2] == &temp && temp.wLvl == 1 && temp.eID[0] == 9);
    assert(do_call.eNO == 26 && do_call.dwFrom == 0 && do_call.dwTo == 0 && do_call.dwPar == 0);
    /* levels 1 and 3 */
    isuse_answer[9] = 1;
    assert(step_026(&in, 1) == 1 && log_n == 2 && is(1, ISUSE, 9, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[9] = 0, temp.wCamTime = 5;
    assert(step_026(&in, 1) == 1 && log_n == 2 && temp.wLvl == 2 && temp.wCamTime == 5);
    assert(step_026(&in, 3) == 1 && log_n == 3 && is(2, RESTORE, 16, 0, 0, 0) && log_p[2] == &temp && temp.wLvl == 4 && temp.wCamTime == 0);
    /* level 2 */
    assert(step_026(&in, 2) == 1 && log_n == 3 && is(1, FOCUS, 0, 0x105, 100, 180) && log_id[2] == DOEFF && temp.wLvl == 3);
    assert(do_call.eNO == 27 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0);
    in.dwPar = 0xFFFE, in.dwTo = 0x30, num_answer = 1, owner_answer = 0x204;
    assert(step_026(&in, 2) == 1 && log_n == 5 && is(1, DEPEND, 27, 0, 0, 0) && log_p[1] == &temp && depend_call.eNO == 999);
    assert(is(2, NUM, 0x30, 0, 0, 0) && is(3, OWNER, 0x30, 0, 0, 0) && is(4, FOCUS, 0, 0x204, 100, 180) && temp.wLvl == 3);
    num_answer = 2;
    assert(step_026(&in, 2) == 1 && log_n == 3 && temp.wLvl == 3);
    /* level 4, one unit */
    in.dwPar = 0x105, ldrange_answer = 1, loc_answer = 0x304, ldabl_answer = 2, curside_answer = 0, curside_lvl = 0;
    assert(step_026(&in, 4) == 1 && log_n == 7 && is(1, CURSIDE, 1, 5, 0, 0) && is(2, LDRANGE, 1, 5, 11, 0) && is(3, LOC, 6, 0, 0, 0));
    assert(is(4, LDABL, 9, 6, 0, 0) && is(5, RANGE, 4, 3, 0, 2) && range_tail[0] == 1 && range_tail[1] == 1 && range_tail[2] == 11);
    assert(is(6, POPUP, 0, -120, 80, 270) && temp.wLvl == 5);
    ldrange_answer = 0, resist_answer = 1, rmaster_answer = 0x207, do_answer = 4;
    assert(step_026(&in, 4) == 1 && log_n == 7 && is(3, RESIST, 1, 5, 0, 0) && is(4, RMASTER, 1, 5, 0, 0) && is(5, FOCUS, 0, 0x207, 100, 180));
    assert(log_id[6] == DOEFF && temp.wCount_2 == 0x207 && temp.eID[0] == 4 && temp.wLvl == 16);
    assert(do_call.eNO == 513 && do_call.dwFrom == 0 && do_call.dwTo == 0x207 && do_call.dwPar == 0);
    resist_answer = 0, in.dwFrom = 6;
    assert(step_026(&in, 4) == 1 && log_n == 5 && is(4, CURSIDE, 1, 5, 0, 0) && temp.wLvl == 5);
    in.dwFrom = 5, curse_answer = 1, spell_answer = 0;
    assert(step_026(&in, 4) == 1 && log_n == 8 && is(5, GUSTAT, 1, 5, 1, 0) && is(6, USTAT, 1, 5, 1, 0) && is(7, GCURSE, 1, 5, 0, 0));
    gustat_zero_pos = 5, curse_answer = 0, spell_answer = 1;
    assert(step_026(&in, 4) == 1 && log_n == 9 && is(6, USTAT, 1, 5, 1, 1) && is(8, GSPELL, 1, 5, 0, 0));
    spell_answer = 0;
    assert(step_026(&in, 4) == 1 && log_n == 10 && is(9, USTAT, 1, 5, 0, 0) && temp.wLvl == 5);
    gustat_zero_pos = -1;
    /* level 4, list: no unit */
    in.dwPar = 0xFFFE, in.dwTo = 0;
    assert(step_026(&in, 4) == 1 && log_n == 1 && temp.wLvl == 5);
    /* positions 0 and 21: side 0 slot 0, side 1 slot 10; turn owner controls, no rests */
    in.dwTo = (1u << 21) | 1, towner_answer = 3;
    assert(step_026(&in, 4) == 1 && log_n == 21 && is(1, CURSIDE, 0, 0, 0, 0) && is(2, LDRANGE, 0, 0, 11, 0) && is(3, CURSIDE, 0, 0, 0, 0));
    assert(is(4, GUSTAT, 0, 0, 1, 0) && is(5, USTAT, 0, 0, 1, 0) && log_id[6] == TOWNER && is(7, UCTL, 0, 0, 0, 0));
    assert(is(8, GCURSE, 0, 0, 0, 0) && is(9, GSPELL, 0, 0, 0, 0) && is(10, USTAT, 0, 0, 0, 0));
    assert(is(11, CURSIDE, 1, 10, 0, 0) && is(12, LDRANGE, 1, 10, 11, 0) && is(13, CURSIDE, 1, 10, 0, 0) && is(14, GUSTAT, 1, 10, 1, 0));
    assert(is(15, USTAT, 1, 10, 1, 0) && log_id[16] == TOWNER && is(17, UCTL, 1, 10, 0, 0) && is(18, GCURSE, 1, 10, 0, 0));
    assert(is(19, GSPELL, 1, 10, 0, 0) && is(20, USTAT, 1, 10, 0, 0));
    /* status bit clear on slot 10, spell rest */
    gustat_zero_pos = 10, spell_answer = 1;
    assert(step_026(&in, 4) == 1 && log_n == 19 && is(9, GSPELL, 0, 0, 0, 0) && is(14, USTAT, 1, 10, 1, 1) && is(18, GSPELL, 1, 10, 0, 0));
    gustat_zero_pos = 0, spell_answer = 0, curse_answer = 1;
    assert(step_026(&in, 4) == 1 && log_n == 17 && is(5, USTAT, 0, 0, 1, 1) && is(8, GCURSE, 0, 0, 0, 0) && is(16, GCURSE, 1, 10, 0, 0));
    /* the turn owner does not control them */
    gustat_zero_pos = -1, towner_answer = 2;
    assert(step_026(&in, 4) == 1 && log_n == 17 && is(8, USTAT, 0, 0, 0, 1) && is(16, USTAT, 1, 10, 0, 1));
    /* units of the side in the parameter are left alone; positions 10 and 11 */
    curside_answer = 0xFFFE, in.dwTo = (1u << 11) | (1u << 10);
    assert(step_026(&in, 4) == 1 && log_n == 7 && is(1, CURSIDE, 0, 10, 0, 0) && is(3, CURSIDE, 0, 10, 0, 0) && is(4, CURSIDE, 1, 0, 0, 0));
    assert(is(5, LDRANGE, 1, 0, 11, 0) && is(6, CURSIDE, 1, 0, 0, 0));
    /* range ability on either side */
    curside_answer = 0, ldrange_answer = 1;
    assert(step_026(&in, 4) == 1 && log_n == 13 && is(3, LOC, 6, 0, 0, 0) && is(4, LDABL, 9, 6, 0, 0) && is(5, RANGE, 4, 3, 0, 2));
    assert(is(6, POPUP, 0, -120, 80, 270) && is(7, CURSIDE, 1, 0, 0, 0) && is(9, LOC, 6, 0, 0, 0) && is(11, RANGE, 4, 3, 0, 2));
    assert(is(12, POPUP, 0, -120, 80, 270) && range_tail[2] == 11);
    /* the wait behind the loop needs the level back at 4 */
    ldrange_answer = 0, curside_answer = 0xFFFE, curside_lvl = 4, in.dwTo = 1u << 21, temp.wCamTime = 16;
    assert(step_026(&in, 4) == 1 && log_n == 4 && temp.wCamTime == 17 && temp.wLvl == 4);
    disp_answer = 1;
    assert(step_026(&in, 4) == 0 && log_n == 7 && log_id[4] == WDISP && is(5, WDRAW, 1, 0, 0, 0) && log_id[6] == CLOSE && temp.wCamTime == 18);
    disp_answer = 0;
    assert(step_026(&in, 4) == 0 && log_n == 6 && log_id[4] == WDISP && log_id[5] == CLOSE);
    curside_answer = 0, curside_lvl = 0;
    /* level 16 */
    temp.eID[0] = 4, isuse_answer[4] = 1, temp.wCount_1 = 30;
    assert(step_026(&in, 16) == 1 && log_n == 4 && is(1, MSG, 83, 0, 0, 0) && is(2, POPUP, 0, -120, 80, 0x4D5347) && is(3, ISUSE, 4, 0, 0, 0));
    assert(temp.wCount_1 == 30 && temp.wLvl == 16);
    isuse_answer[4] = 0, temp.wCount_1 = 31;
    assert(step_026(&in, 16) == 1 && log_n == 4 && temp.wCount_1 == 32 && temp.wLvl == 16);
    temp.wCount_1 = 30, temp.wCamTime = 7;
    assert(step_026(&in, 16) == 1 && log_n == 6 && log_id[4] == CLOSE && is(5, RESTORE, 16, 0, 0, 0) && temp.wCount_1 == 31);
    assert(temp.wCamTime == 0 && temp.wLvl == 17);
    /* any other level */
    temp.wCamTime = 16;
    assert(step_026(&in, 17) == 1 && log_n == 1 && temp.wCamTime == 17);
    disp_answer = 1;
    assert(step_026(&in, 17) == 0 && log_n == 4 && log_id[1] == CLOSE && log_id[2] == WDISP && is(3, WDRAW, 1, 0, 0, 0));
    assert(step_026(&in, 5) == 0 && log_n == 4);
    disp_answer = 0;
    assert(step_026(&in, 17) == 0 && log_n == 3 && log_id[2] == WDISP);
}

enum { DIST = 1800 };
static int dist_seq[4];
int Ta_GetLocDistanceFromCenterOnRangeXY(int packed, int step, int mode)
{
    note(DIST, NULL, packed, step, mode, 0);
    return dist_seq[mode & 3];
}

static int step_005(struct jp_6989c *in, int lvl, int cam)
{
    temp.wLvl = (unsigned short)lvl;
    temp.bCamLvl = (unsigned char)cam;
    return step(Ta_Eff_005, in, -1);
}

/* Ta_Eff_005 (branch coverage): start with no trap, the first only and both; modules not loaded;
 * level 128 (open rate, both messages), 129 (busy, both stored levels), 130 (below ten on a sixth
 * frame and not, ten busy and free), 160 and an unlisted one; camera levels 0, 1, 2, 32, 33 and
 * other at both sides of their waits; both finish bits with each fade and window answer; levels
 * 127 and 255 on a plain frame; the first list at every test (free, status, no position, no map
 * unit, owner not listed, slot 0, 1, 5 and 6, range ability or not); the second list with each
 * rate; a twelfth frame at level 0, at level 3 with two hits and with none (running effects left
 * or not, end with one or both conditions and the removal loop). Level 255 on a twelfth frame
 * is not run: the original reads a counter there that nothing has set. */
static void check_005(void)
{
    struct jp_6989c in = {5, 0x304, 0x30, 0x10002};

    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    status_once = 0, loc_by_unit = 0, openrate_side2 = 0, curside_answer = 0, curside_lvl = 0, num_by_value = 0, owner_by_n = 0;
    isload_answer = 0, finfade_answer = 0, disp_answer = 0, remove_answer = 0x77, agree_zero_packed = -1, agree_word_on = 0;
    /* start */
    trap_n = 0, trap_seq[0] = -1;
    assert(step(Ta_Eff_005, &in, 0) == 1 && log_n == 7 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, REMOVE1, 0x30, 2, 0, 0) && is(3, TRAPB, 0, 80, 0, 0) && is(4, FOCUS2, 0, 0x304, 200, 0));
    assert(is(5, LOAD, 505, 0, 0, 0) && is(6, LOAD, 602, 0, 0, 0) && temp.fObj2 == 0x77 && temp.wLvl == 0 && temp.bCamLvl == 0);
    in.dwPar = 2, trap_n = 0, trap_seq[0] = 0x203, trap_seq[1] = -1;
    assert(step(Ta_Eff_005, &in, 0) == 1 && log_n == 9 && is(3, TRAPB, 1, 80, 0, 0) && is(4, TRAPB, 0, 46, 0, 0));
    assert(is(5, FOCUS, 0, 0x203, 180, 180) && is(6, DOOPEN, 2, 3, 1, 0) && is(7, LOAD, 505, 0, 0, 0) && is(8, LOAD, 602, 0, 0, 0));
    assert(temp.ePos[0] == 0x203 && temp.fObj == 130 && temp.wLvl == 128 && temp.bCamLvl == 32);
    in.dwPar = 0x10002, trap_n = 0, trap_seq[1] = 0x105;
    assert(step(Ta_Eff_005, &in, 0) == 1 && log_n == 9 && is(4, TRAPB, 1, 46, 0, 0) && is(5, FOCUS, 0, 0x105, 180, 180));
    assert(is(6, DOOPEN, 1, 5, 1, 0) && temp.ePos[0] == 0x105 && temp.fObj == 160 && temp.wLvl == 128 && temp.bCamLvl == 32);
    /* modules not loaded */
    assert(step_005(&in, 128, 0) == 1 && log_n == 2 && log_id[1] == ISLOAD && log_p[1] == &temp && temp.wCamTime == 0);
    isload_answer = 1;
    /* level 128 */
    temp.ePos[0] = 0x203, openrate_answer = 100;
    assert(step_005(&in, 128, 32) == 1 && log_n == 3 && is(2, OPENRATE, 2, 3, 0, 0) && temp.wLvl == 128);
    openrate_answer = 255, temp.fObj = 130, do_answer = 9;
    assert(step_005(&in, 128, 32) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 259) && log_id[4] == DOEFF && temp.wLvl == 129);
    assert(temp.eID[0] == 9 && do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x203 && do_call.dwPar == 0);
    temp.fObj = 160;
    assert(step_005(&in, 128, 32) == 1 && log_n == 6 && is(3, MSG, 825, 0, 0, 0) && is(4, POPUP, 0, -120, 80, 0x4D5347) && log_id[5] == DOEFF);
    /* level 129 */
    temp.oNum = 1, isuse_answer[9] = 1;
    assert(step_005(&in, 129, 32) == 1 && log_n == 3 && is(2, ISUSE, 9, 0, 0, 0) && temp.wLvl == 129 && temp.bCamLvl == 32);
    isuse_answer[9] = 0, temp.fObj = 130, temp.wCamTime = 7;
    assert(step_005(&in, 129, 32) == 1 && log_n == 6 && is(3, USTAT, 2, 3, 0, 1) && log_id[4] == CLOSE && is(5, FOCUS, 30, 0x203, 500, 500));
    assert(temp.wLvl == 130 && temp.bCamLvl == 1 && temp.wCamTime == 0);
    temp.fObj = 160, temp.wCamTime = 7;
    assert(step_005(&in, 129, 32) == 1 && log_n == 6 && is(5, FOCUS2, 0, 0x304, 200, 0) && temp.wLvl == 160 && temp.bCamLvl == 0);
    assert(temp.wCamTime == 8);
    /* level 130 */
    temp.wCount_2 = 9, temp.wCount_1 = 6, loc_answer = 0x405, do_answer = 11;
    assert(step_005(&in, 130, 32) == 1 && log_n == 6 && is(2, MSG, 821, 0, 0, 0) && is(3, POPUP, 0, -120, 80, 0x4D5347));
    assert(is(4, LOC, 2, 3, 0, 0) && log_id[5] == DOEFF && do_call.eNO == 505 && do_call.dwFrom == 0 && do_call.dwTo == 0x405);
    assert(do_call.dwPar == 0 && temp.eID[1] == 11 && temp.ePos[1] == 0x203 && temp.wCount_2 == 10 && temp.wCount_1 == 7);
    assert(temp.oNum == 1 && temp.wLvl == 130);
    temp.wCount_2 = 0;
    assert(step_005(&in, 130, 32) == 1 && log_n == 4 && temp.wCount_2 == 0 && temp.wCount_1 == 8);
    temp.wCount_2 = 10, isuse_answer[9] = 1;
    assert(step_005(&in, 130, 2) == 1 && log_n == 6 && log_id[4] == CLOSE && is(5, ISUSE, 9, 0, 0, 0) && temp.bCamLvl == 2);
    assert(temp.wCount_1 == 8);
    isuse_answer[9] = 0, temp.wCamTime = 5;
    assert(step_005(&in, 130, 2) == 1 && log_n == 8 && log_id[6] == CLOSE && is(7, RESTORE, 16, 0, 0, 0) && log_p[7] == &temp);
    assert(temp.bCamLvl == 255 && temp.wCamTime == 0);
    /* level 160 */
    do_answer = 12, temp.wCount_1 = 3;
    assert(step_005(&in, 160, 32) == 1 && log_n == 4 && log_id[2] == DOEFF && is(3, FOCUS2, 0, 0x304, 200, 0));
    assert(do_call.eNO == 505 && do_call.dwFrom == 0 && do_call.dwTo == 0x304 && do_call.dwPar == 0);
    assert(temp.eID[0] == 12 && temp.ePos[0] == 0x304 && temp.oNum == 2 && temp.wCount_1 == 4 && temp.wLvl == 0);
    /* an unlisted level */
    assert(step_005(&in, 131, 32) == 1 && log_n == 2 && temp.wLvl == 131 && temp.bCamLvl == 32);
    /* camera levels */
    temp.wCamTime = 16;
    assert(step_005(&in, 131, 0) == 1 && log_n == 2 && temp.wCamTime == 17 && temp.bCamLvl == 0);
    assert(step_005(&in, 131, 0) == 1 && log_n == 3 && is(2, STDVIEW, 30, 0, 0, 0) && temp.wCamTime == 0 && temp.bCamLvl == 1);
    temp.wCamTime = 30;
    assert(step_005(&in, 131, 1) == 1 && log_n == 2 && temp.wCamTime == 31 && temp.bCamLvl == 1);
    assert(step_005(&in, 131, 1) == 1 && temp.wCamTime == 32 && temp.bCamLvl == 2);
    assert(step_005(&in, 131, 2) == 1 && log_n == 2 && temp.bCamLvl == 2 && temp.wCamTime == 32);
    temp.ePos[0] = 0x203;
    assert(step_005(&in, 131, 33) == 1 && log_n == 3 && is(2, FOCUS, 30, 0x203, 500, 500) && temp.bCamLvl == 1 && temp.wCamTime == 0);
    temp.wCamTime = 16;
    assert(step_005(&in, 131, 255) == 1 && log_n == 2 && temp.wCamTime == 17);
    assert(step_005(&in, 131, 255) == 1 && log_n == 2 && temp.wCamTime == 18);
    /* both finish bits */
    temp.wCount_2 = 10, temp.oNum = 1, temp.eID[0] = 9;
    assert(step_005(&in, 130, 255) == 1 && log_n == 7 && log_id[6] == FINFADE);
    finfade_answer = 1;
    assert(step_005(&in, 130, 255) == 0 && log_n == 9 && log_id[7] == WDISP && log_id[8] == CLOSE);
    disp_answer = 1;
    assert(step_005(&in, 130, 255) == 0 && log_n == 10 && is(8, WDRAW, 1, 0, 0, 0) && log_id[9] == CLOSE);
    disp_answer = 0, finfade_answer = 0;
    /* levels 127 and 255 on a plain frame */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 1;
    assert(step_005(&in, 127, 32) == 1 && log_n == 2 && temp.wCount_1 == 2 && temp.wLvl == 127);
    assert(step_005(&in, 255, 32) == 1 && log_n == 2 && temp.wCount_1 == 3 && temp.wLvl == 255);
    /* first list */
    temp.oNum = 1, temp.eID[0] = 9, temp.ePos[0] = 0x203, temp.fObj2 = 0x77;
    assert(step_005(&in, 5, 32) == 1 && log_n == 3 && is(2, ISUSE, 9, 0, 0, 0));
    isuse_answer[9] = 1, status_answer[9] = 1;
    assert(step_005(&in, 5, 32) == 1 && log_n == 4 && is(3, STATUS, 9, 0, 0, 0));
    status_answer[9] = 2, temp.ePos[0] = -1, mapunit_n = 0;
    assert(step_005(&in, 5, 32) == 1 && log_n == 4 && mapunit_n == 0);
    temp.ePos[0] = 0x203, mapunit_at = -1;
    assert(step_005(&in, 5, 32) == 1 && log_n == 4 && mapunit_n == 1);
    mapunit_at = 0x203, mapid_answer = 0x105, mapid_n = 0, agree_zero_packed = 0x105;
    assert(step_005(&in, 5, 32) == 1 && log_n == 5 && is(4, AGREE, 0x105, 0x30, 0, 0) && temp.ePos[0] == 0x203);
    assert(mapid_args[0] == 3 && mapid_args[1] == 2);
    agree_zero_packed = -1, temp.wCount_1 = 1, ctlc_answer = 1;
    assert(step_005(&in, 5, 32) == 1 && log_n == 8 && is(5, AGREE, 0x105, 0x77, 0, 0) && is(6, UFADE, 1, 5, -1, 1) && is(7, CTLC, 1, 5, 0, 0));
    assert(temp.ePos[0] == -1 && temp.ePos[13] == 0x105 && temp.wCount_2 == 1);
    temp.ePos[0] = 0x203, temp.wCount_2 = 0, mapid_answer = 0x100;
    assert(step_005(&in, 5, 32) == 1 && log_n == 6 && temp.ePos[0] == -1 && temp.wCount_2 == 0);
    temp.ePos[0] = 0x203, mapid_answer = 0x106;
    assert(step_005(&in, 5, 32) == 1 && log_n == 6 && temp.wCount_2 == 0);
    temp.ePos[0] = 0x203, mapid_answer = 0x201;
    assert(step_005(&in, 5, 32) == 1 && log_n == 8 && is(6, UFADE, 2, 1, -1, 1) && temp.ePos[13] == 0x201 && temp.wCount_2 == 1);
    temp.ePos[0] = 0x203, temp.wCount_2 = 0, mapid_answer = 0x105, agree_word_on = 0x77, ldrange_answer = 0, loc_answer = 0x405;
    assert(step_005(&in, 5, 32) == 1 && log_n == 9 && is(6, CURSIDE, 1, 5, 0, 0) && is(7, LOC, 6, 0, 0, 0) && is(8, LDRANGE, 1, 5, 11, 0));
    temp.ePos[0] = 0x203, ldrange_answer = 1, ldabl_answer = 2;
    assert(step_005(&in, 5, 32) == 1 && log_n == 12 && is(9, LDABL, 9, 6, 0, 0) && is(10, RANGE, 5, 4, 0, 2) && is(11, POPUP, 0, -120, 80, 270));
    assert(range_tail[0] == 1 && range_tail[1] == 1 && range_tail[2] == 11 && temp.ePos[0] == -1);
    agree_word_on = 0, ldrange_answer = 0;
    /* second list */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 1, temp.wCount_2 = 2, temp.ePos[13] = 0x105, temp.ePos[14] = 0x206, ctlc_answer = 1, ratem_answer = 1;
    assert(step_005(&in, 5, 32) == 1 && log_n == 4 && is(2, CTLC, 1, 5, 0, 0) && is(3, CTLC, 2, 6, 0, 0));
    ctlc_answer = 0;
    assert(step_005(&in, 5, 32) == 1 && log_n == 6 && is(3, GRATEM, 1, 5, 0, 0) && is(5, GRATEM, 2, 6, 0, 0));
    /* twelfth frame, level 3, nothing in range: one end condition, then both */
    temp.wCount_1 = 12, dist_seq[0] = dist_seq[1] = dist_seq[2] = dist_seq[3] = -1, num_answer = 0;
    assert(step_005(&in, 3, 2) == 1 && log_n == 10 && is(6, DIST, 0x304, 3, 0, 0) && is(9, DIST, 0x304, 3, 3, 0) && temp.wLvl == 255);
    assert(temp.wCount_1 == 13 && temp.bCamLvl == 2);
    temp.wCount_1 = 12, ratem_answer = 0;
    assert(step_005(&in, 3, 2) == 1 && log_n == 13 && is(10, NUM, 0, 0, 0, 0) && log_id[11] == CLOSE && is(12, RESTORE, 16, 0, 0, 0));
    assert(temp.bCamLvl == 255);
    temp.wCount_1 = 12, ctlc_answer = 1;
    assert(step_005(&in, 3, 2) == 1 && log_n == 8 && temp.bCamLvl == 2);
    /* twelfth frame, level 0 */
    memset(&temp, 0, sizeof(temp));
    temp.wCount_1 = 12, temp.wCount_2 = 1, temp.ePos[13] = 0x105, do_answer = 13;
    assert(step_005(&in, 0, 32) == 1 && log_n == 4 && log_id[3] == DOEFF && temp.eID[0] == 13 && temp.ePos[0] == 0x304);
    assert(temp.oNum == 1 && temp.wLvl == 1 && temp.wCount_1 == 13);
    assert(do_call.eNO == 505 && do_call.dwFrom == 0 && do_call.dwTo == 0x304 && do_call.dwPar == 0);
    /* twelfth frame, level 3, two of four in range */
    temp.wCount_1 = 24, dist_seq[0] = 0x405, dist_seq[3] = 0x506, do_answer = 14;
    assert(step_005(&in, 3, 32) == 1 && log_n == 10 && is(4, DIST, 0x304, 3, 0, 0) && log_id[5] == DOEFF && is(8, DIST, 0x304, 3, 3, 0));
    assert(log_id[9] == DOEFF && do_prev.dwTo == 0x405 && do_call.dwTo == 0x506 && temp.ePos[1] == 0x405 && temp.ePos[2] == 0x506);
    assert(temp.eID[2] == 14 && temp.oNum == 3 && temp.wLvl == 4);
    /* none in range: level 255, an effect still running, then none */
    temp.wCount_1 = 12, dist_seq[0] = -1, dist_seq[3] = -1, temp.oNum = 2, temp.eID[0] = 9, temp.eID[1] = 11, temp.wCount_2 = 0;
    isuse_answer[9] = 1, isuse_answer[11] = 0, status_answer[9] = 0;
    assert(step_005(&in, 3, 2) == 1 && log_n == 11 && is(9, ISUSE, 9, 0, 0, 0) && is(10, ISUSE, 11, 0, 0, 0) && temp.wLvl == 255);
    assert(temp.bCamLvl == 2);
    temp.wCount_1 = 12, isuse_answer[9] = 0, num_answer = 2, owner_answer = 0x105, owner_by_n = 1, temp.fObj2 = 0x77;
    assert(step_005(&in, 3, 2) == 1 && log_n == 19 && is(10, NUM, 0x77, 0, 0, 0) && is(11, OWNER, 0x77, 0, 0, 0) && is(12, TDEL, 1, 5, 0, 0));
    assert(is(14, OWNER, 0x77, 1, 0, 0) && is(15, TDEL, 1, 6, 0, 0) && is(16, NUM, 0x77, 0, 0, 0) && log_id[17] == CLOSE);
    assert(log_id[18] == RESTORE && temp.bCamLvl == 255);
    owner_by_n = 0, num_answer = 0, ctlc_answer = 0, isload_answer = 0, do_answer = 0, trap_n = 0, mapunit_at = 0;
}

enum { BLACKM = 1900, CALCEXP };
void SzEffect_SetBlackOutForMonster(int value) { note(BLACKM, NULL, value, 0, 0, 0); }
int SzData_CalcExp(int side, int id, int operation, int value) { note(CALCEXP, NULL, side, id, operation, value); return 0; }

static int step_btl(struct jp_6989c *in, int lvl, int cam)
{
    temp.wLvl = (unsigned short)lvl;
    temp.bCamLvl = (unsigned char)cam;
    return step(Ta_Eff_BTL_000, in, -1);
}

/* Ta_Eff_BTL_000 (branch coverage): either missing argument; start with the ability and each of
 * the two parameter fields, and with none; camera levels 0 and 1 on their first frame, below 12,
 * below 24 and at 24 with each test failing in turn and all passing; modules not loaded; level 0
 * with the second load test failing, without and with each message effect (stored id zero and
 * not for the second); level 1 with no effect, a running one and none running; level 2 above
 * zero and at zero; level 3 with and without a correction; level 4 running (status 2 or not,
 * count 1 or not) and finished with each life-point answer; level 5 below and at 128, running at
 * frame 20 and not, and finished; levels 16 and 17 (running, finished); another level below 128
 * and at 128 with no bonus, a positive and a negative one and each window answer. */
static void check_btl_000(void)
{
    struct jp_6989c in = {0, 0x304, 1, 0};

    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    status_once = 0, openrate_side2 = 0, isload_answer = 0, disp_answer = 0, ldabl_answer = 1;
    log_n = 0;
    assert(Ta_Eff_BTL_000(NULL, &temp, 0) == 0 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && is(1, CLOSE, 0, 0, 0, 0));
    log_n = 0;
    assert(Ta_Eff_BTL_000(&in, NULL, 0) == 0 && log_n == 2 && is(1, CLOSE, 0, 0, 0, 0));
    /* start */
    in.dwPar = 0x100000;
    assert(step(Ta_Eff_BTL_000, &in, 0) == 1 && log_n == 8 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, LDABL, 6, 1, 0, 0) && is(3, LOAD, 557, 0, 0, 0) && is(4, LOAD, 513, 0, 0, 0) && is(5, LOAD, 604, 0, 0, 0));
    assert(is(6, LOAD, 605, 0, 0, 0) && is(7, FOCUS, 0, 0x304, 300, 280) && temp.wLvl == 16 && temp.wCount_1 == 128);
    ldabl_answer = 0, temp.wLvl = 0, in.dwPar = 0x400000;
    assert(step(Ta_Eff_BTL_000, &in, 0) == 1 && log_n == 7 && is(3, LOAD, 513, 0, 0, 0) && is(4, LOAD, 604, 0, 0, 0) && temp.wLvl == 0);
    in.dwPar = 0x0f0002;
    assert(step(Ta_Eff_BTL_000, &in, 0) == 1 && log_n == 6 && is(3, LOAD, 604, 0, 0, 0) && is(5, FOCUS, 0, 0x304, 300, 280));
    /* camera level 0 */
    temp.wCamTime = 0;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 4 && is(1, SE, 0, 39, 0, 0) && is(2, AUTOBLUR, 6, 0, 0, 0));
    assert(is(3, FOCUS, 12, 0x304, 100, 180) && temp.wCamTime == 1);
    temp.wCamTime = 10;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 1 && temp.wCamTime == 11);
    assert(step_btl(&in, 0, 0) == 1 && log_n == 2 && is(1, AUTOBLUR, 4, 0, 0, 0) && temp.wCamTime == 12);
    temp.wCamTime = 22;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 2 && temp.wCamTime == 23);
    openrate_answer = 0;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 3 && is(2, OPENRATE, 3, 4, 0, 0) && temp.wCamTime == 24);
    openrate_answer = 255, ctlc_answer = 0;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 4 && is(3, CTLC, 3, 4, 0, 0));
    ctlc_answer = 128, standby_answer = 0;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 5 && is(4, STANDBY, 0x304, 0, 0, 0) && temp.bCamLvl == 0);
    standby_answer = 1;
    assert(step_btl(&in, 0, 0) == 1 && log_n == 7 && is(5, AUTOBLUR, 0, 0, 0, 0) && is(6, FOCUS, 0, 0x100, 300, 280));
    assert(temp.wCamTime == 0 && temp.bCamLvl == 1);
    /* camera level 1 */
    assert(step_btl(&in, 0, 1) == 1 && log_n == 4 && is(1, SE, 0, 39, 0, 0) && is(2, AUTOBLUR, 6, 0, 0, 0));
    assert(is(3, FOCUS, 12, 0x100, 100, 180) && temp.wCamTime == 1);
    temp.wCamTime = 10;
    assert(step_btl(&in, 0, 1) == 1 && log_n == 1 && temp.wCamTime == 11);
    assert(step_btl(&in, 0, 1) == 1 && log_n == 2 && is(1, AUTOBLUR, 4, 0, 0, 0));
    temp.wCamTime = 22;
    assert(step_btl(&in, 0, 1) == 1 && log_n == 2 && temp.wCamTime == 23);
    standby_answer = 0;
    assert(step_btl(&in, 0, 1) == 1 && log_n == 3 && is(2, STANDBY, 0x100, 0, 0, 0) && temp.bCamLvl == 1);
    standby_answer = 1;
    assert(step_btl(&in, 0, 1) == 1 && log_n == 5 && is(3, AUTOBLUR, 0, 0, 0, 0) && log_id[4] == RESTORE && log_p[4] == &temp);
    assert(log_a[4] == 0 && temp.wCamTime == 0 && temp.bCamLvl == 2);
    /* modules not loaded */
    assert(step_btl(&in, 3, 2) == 1 && log_n == 2 && log_id[1] == ISLOAD && log_p[1] == &temp && temp.wLvl == 3);
    isload_answer = 1;
    /* level 0 */
    isload_drop = 1;
    assert(step_btl(&in, 0, 2) == 1 && log_n == 3 && log_id[2] == ISLOAD && temp.wLvl == 0);
    isload_drop = 0, in.dwPar = 0x0f0002;
    assert(step_btl(&in, 0, 2) == 1 && log_n == 5 && is(3, BUSY2, 0x304, 1, 0, 0) && is(4, BUSY2, 0x100, 1, 0, 0) && temp.wLvl == 1);
    in.dwPar = 0x500000, in.dwFrom = 0x10304, do_answer = 7, temp.oNum = 0;
    assert(step_btl(&in, 0, 2) == 1 && log_n == 9 && is(3, BUSY2, 0x10304, 1, 0, 0) && is(5, POPUP, 0, -120, 80, 260));
    assert(log_id[6] == DOEFF && log_p[6] == &temp && is(7, POPUP, 0, -120, 80, 260) && log_id[8] == DOEFF);
    assert(do_prev.eNO == 513 && do_prev.dwFrom == 0 && do_prev.dwTo == 0x304 && do_prev.dwPar == 0);
    assert(do_call.eNO == 513 && do_call.dwFrom == 0 && do_call.dwTo == 1 && do_call.dwPar == 0);
    assert(temp.eID[0] == 7 && temp.eID[1] == 7 && temp.oNum == 2 && temp.wLvl == 1);
    in.dwPar = 0x400000, in.dwFrom = 0x304, do_answer = 0, temp.oNum = 0, temp.eID[0] = 9;
    assert(step_btl(&in, 0, 2) == 1 && log_n == 7 && temp.eID[0] == 0 && temp.oNum == 0);
    in.dwPar = 0x100000;
    assert(step_btl(&in, 0, 2) == 1 && log_n == 7 && temp.oNum == 1 && do_call.dwTo == 0x304);
    /* level 1 */
    temp.oNum = 0;
    assert(step_btl(&in, 1, 2) == 1 && log_n == 3 && is(2, CLOSE, 0, 0, 0, 0) && temp.wLvl == 2 && temp.oNum == 0);
    temp.oNum = 2, temp.eID[0] = 3, temp.eID[1] = 4, isuse_answer[4] = 1;
    assert(step_btl(&in, 1, 2) == 1 && log_n == 4 && is(2, ISUSE, 3, 0, 0, 0) && is(3, ISUSE, 4, 0, 0, 0) && temp.wLvl == 1 && temp.oNum == 2);
    isuse_answer[4] = 0;
    assert(step_btl(&in, 1, 2) == 1 && log_n == 5 && is(4, CLOSE, 0, 0, 0, 0) && temp.wLvl == 2 && temp.oNum == 0);
    /* level 2 */
    temp.wCount_1 = 128, ctrl_answer = 0x171;
    assert(step_btl(&in, 2, 2) == 1 && log_n == 5 && is(2, CTRL, 128, 15, -1, 0) && is(3, BLACK, 0x71, 0, 0, 0) && is(4, BLACKM, 0x71, 0, 0, 0));
    assert(temp.wCount_1 == 0x171 && temp.wLvl == 2);
    ctrl_answer = 0;
    assert(step_btl(&in, 2, 2) == 1 && log_n == 5 && is(2, CTRL, 0x171, 15, -1, 0) && is(3, BLACK, 0, 0, 0, 0) && is(4, BLACKM, 0, 0, 0, 0));
    assert(temp.wCount_1 == 0 && temp.wLvl == 3);
    /* level 3 */
    in.dwPar = 0x10002, do_answer = 5, temp.oNum = 1, temp.ePos[1] = 9, lprev_zero = 0;
    assert(step_btl(&in, 3, 2) == 1 && log_n == 4 && is(2, LPREV, 44, 2, 0, 0) && log_id[3] == DOEFF);
    assert(do_call.eNO == 604 && do_call.dwFrom == 0x304 && do_call.dwTo == 0 && do_call.dwPar == 0x1234);
    assert(temp.eID[1] == 5 && temp.ePos[1] == 0 && temp.oNum == 2 && temp.wLvl == 4);
    lprev_zero = 1;
    assert(step_btl(&in, 3, 2) == 1 && do_call.dwPar == 0x30000 && temp.oNum == 3);
    lprev_zero = 0;
    /* level 4 */
    temp.oNum = 1, temp.eID[0] = 3, temp.ePos[0] = 1, isuse_answer[3] = 1, status_answer[3] = 0;
    assert(step_btl(&in, 4, 2) == 1 && log_n == 4 && is(2, ISUSE, 3, 0, 0, 0) && is(3, STATUS, 3, 0, 0, 0) && temp.ePos[0] == 1 && temp.wLvl == 4);
    status_answer[3] = 2;
    assert(step_btl(&in, 4, 2) == 1 && log_n == 5 && is(4, SE, 1, 65, 0, 0) && temp.ePos[0] == 2);
    assert(step_btl(&in, 4, 2) == 1 && log_n == 4 && temp.ePos[0] == 3 && temp.wLvl == 4);
    isuse_answer[3] = 0, getlp_zero = 0;
    assert(step_btl(&in, 4, 2) == 1 && log_n == 11 && log_id[3] == DOEFF && is(4, LPREV, 44, 2, 0, 0) && is(5, CALCLP, 1, 1, 0x1234, 0));
    assert(is(6, GETLP, 1, 0, 0, 0) && is(7, CARDNO, 3, 4, 0, 0) && is(8, CALCEXP, 3, 764, 0, 20) && is(9, CARDNO, 1, 0, 0, 0));
    assert(is(10, CALCEXP, 1, 720, 0, -8) && do_call.eNO == 605 && do_call.dwFrom == 0x304 && do_call.dwTo == 0 && do_call.dwPar == 0);
    assert(temp.eID[1] == 5 && temp.oNum == 2 && temp.wLvl == 5);
    temp.oNum = 1, getlp_zero = 1;
    assert(step_btl(&in, 4, 2) == 1 && log_n == 11 && is(8, CALCEXP, 3, 764, 0, 24) && is(10, CALCEXP, 1, 720, 0, -8) && temp.wLvl == 5);
    getlp_zero = 0;
    /* level 5 */
    temp.oNum = 1, temp.wCount_1 = 90, ctrl_answer = 105, isuse_answer[3] = 1, temp.fObj2 = 20;
    assert(step_btl(&in, 5, 2) == 1 && log_n == 7 && is(2, CTRL, 90, 15, 1, 0) && is(3, BLACK, 105, 0, 0, 0) && is(4, BLACKM, 105, 0, 0, 0));
    assert(is(5, ISUSE, 3, 0, 0, 0) && is(6, SE, 0, 2, 0, 0) && temp.fObj2 == 21 && temp.wCount_1 == 105 && temp.wLvl == 5);
    ctrl_answer = 128;
    assert(step_btl(&in, 5, 2) == 1 && log_n == 4 && is(3, ISUSE, 3, 0, 0, 0) && temp.fObj2 == 22 && temp.wLvl == 5);
    isuse_answer[3] = 0;
    assert(step_btl(&in, 5, 2) == 1 && log_n == 4 && temp.fObj2 == 22 && temp.wLvl == 6);
    /* levels 16 and 17 */
    do_answer = 3;
    assert(step_btl(&in, 16, 2) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 270) && log_id[3] == DOEFF);
    assert(do_call.eNO == 557 && do_call.dwFrom == 0 && do_call.dwTo == 0x304 && do_call.dwPar == 0 && temp.eID[0] == 3 && temp.wLvl == 17);
    isuse_answer[3] = 1;
    assert(step_btl(&in, 17, 2) == 1 && log_n == 3 && is(2, ISUSE, 3, 0, 0, 0) && temp.wLvl == 17);
    isuse_answer[3] = 0;
    assert(step_btl(&in, 17, 2) == 1 && log_n == 4 && is(3, CLOSE, 0, 0, 0, 0) && temp.wLvl == 0);
    /* any other level */
    ctrl_answer = 127, temp.wCount_1 = 112;
    assert(step_btl(&in, 6, 2) == 1 && log_n == 5 && is(2, CTRL, 112, 15, 1, 0) && is(3, BLACK, 127, 0, 0, 0) && is(4, BLACKM, 127, 0, 0, 0));
    ctrl_answer = 128, disp_answer = 0;
    assert(step_btl(&in, 6, 2) == 0 && log_n == 7 && is(3, BLACK, 128, 0, 0, 0) && is(4, BLACKM, 128, 0, 0, 0) && log_id[5] == WDISP);
    assert(is(6, CLOSE, 0, 0, 0, 0) && temp.wCount_1 == 128 && temp.wLvl == 6);
    in.dwFrom = 0x01230304, disp_answer = 1;
    assert(step_btl(&in, 6, 2) == 0 && log_n == 10 && is(3, AP, 3, 4, 1, 0x123) && is(4, DP, 3, 4, 1, 0x123) && log_id[7] == WDISP);
    assert(is(8, WDRAW, 1, 0, 0, 0) && is(9, CLOSE, 0, 0, 0, 0));
    in.dwFrom = 0x81230304, disp_answer = 0;
    assert(step_btl(&in, 6, 2) == 0 && log_n == 9 && is(3, AP, 3, 4, 0, 0x123) && is(4, DP, 3, 4, 0, 0x123) && is(8, CLOSE, 0, 0, 0, 0));
}

enum { SLEEP = 1950, OPT3D, ISTUTO };
static int opt3d_answer, istuto_answer;
int SzEffect_Sleep(int index, int flag) { note(SLEEP, NULL, index, flag, 0, 0); return 0; }
int SzDuel_GetOption3DBattle(int side) { note(OPT3D, NULL, side, 0, 0, 0); return opt3d_answer; }
int TaTuto_IsTuto(int value) { note(ISTUTO, NULL, value, 0, 0, 0); return istuto_answer; }

static int step_btl1(struct jp_6989c *in, int lvl, int cam)
{
    temp.wLvl = (unsigned short)lvl;
    temp.bCamLvl = (unsigned char)cam;
    return step(Ta_Eff_BTL_001, in, -1);
}

static int step_btl1_l2(struct jp_6989c *in, unsigned par, int zero)
{
    in->dwPar = par, lprev_zero = zero;
    temp.wCamTime = 7, temp.oNum = 0, temp.ePos[0] = 9, temp.ePos[1] = 9;
    return step_btl1(in, 2, 2);
}

/* Ta_Eff_BTL_001 (branch coverage): either missing argument; start with each of the two
 * parameter fields and with none; camera levels 0 and 1 on their first frame, below 12, below 24
 * and at 24 with each test failing in turn and all passing; level 0 with the modules not loaded,
 * without and with each message effect (stored id zero and not for the second); level 1 with no
 * effect, a running one and none running; level 2 above zero, at zero before and at the eighth
 * frame, there with no correction asked (0xffff), a zero and a non-zero correction and bit 28
 * set or clear; level 3 running (status not 2, step 0 with each of the three tests failing and
 * all passing, steps 1, 2 and 3) and finished with bits 16 and 17 each with and without 0xffff
 * and neither; level 4 below and at 128, running at frames 0, 20, 40 with each bit and count
 * answer and at another frame, finished with no bits from 25 up, bit 26 with and without bit
 * 19, bit 25 with and without bit 18, and bit 27 alone; level 5 below and at 128, running and
 * finished; another level below 128, at 128 with the first effect answering 1, another non-zero
 * value and zero, with no bonus, a positive and a negative one for each side and each window
 * answer. Not reachable from a host test: the false outcome of the two `>= 0` tests on the
 * stored byte, and the store of 5 to wLvl that level 4 overwrites with 16. */
static void check_btl_001(void)
{
    struct jp_6989c in = {0, 0x304, 0x105, 0};

    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    status_once = 0, openrate_side2 = 0, isload_answer = 0, disp_answer = 0, isload_drop = 0;
    log_n = 0;
    assert(Ta_Eff_BTL_001(NULL, &temp, 0) == 0 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && is(1, CLOSE, 0, 0, 0, 0));
    log_n = 0;
    assert(Ta_Eff_BTL_001(&in, NULL, 0) == 0 && log_n == 2 && is(1, CLOSE, 0, 0, 0, 0));
    /* start */
    in.dwPar = 0x100000, in.dwFrom = 0x10304;
    assert(step(Ta_Eff_BTL_001, &in, 0) == 1 && log_n == 6 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, LOAD, 513, 0, 0, 0) && is(3, LOAD, 604, 0, 0, 0) && is(4, LOAD, 605, 0, 0, 0) && is(5, FOCUS, 0, 0x304, 300, 280));
    assert(temp.wCount_1 == 128 && temp.wLvl == 0);
    in.dwPar = 0x400000, in.dwFrom = 0x304;
    assert(step(Ta_Eff_BTL_001, &in, 0) == 1 && log_n == 6 && is(2, LOAD, 513, 0, 0, 0) && is(3, LOAD, 604, 0, 0, 0));
    in.dwPar = 0x0f0002;
    assert(step(Ta_Eff_BTL_001, &in, 0) == 1 && log_n == 5 && is(2, LOAD, 604, 0, 0, 0) && is(4, FOCUS, 0, 0x304, 300, 280));
    /* camera level 0 */
    temp.wCamTime = 0;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 4 && is(1, SE, 0, 39, 0, 0) && is(2, AUTOBLUR, 6, 0, 0, 0));
    assert(is(3, FOCUS, 12, 0x304, 100, 180) && temp.wCamTime == 1);
    temp.wCamTime = 10;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 1 && temp.wCamTime == 11);
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 2 && is(1, AUTOBLUR, 4, 0, 0, 0) && temp.wCamTime == 12);
    temp.wCamTime = 22;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 2 && temp.wCamTime == 23);
    openrate_answer = 0;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 3 && is(2, OPENRATE, 3, 4, 0, 0) && temp.wCamTime == 24);
    openrate_answer = 255, ctlc_answer = 0;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 4 && is(3, CTLC, 3, 4, 0, 0));
    ctlc_answer = 128, standby_answer = 0;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 5 && is(4, STANDBY, 0x304, 0, 0, 0) && temp.bCamLvl == 0);
    standby_answer = 1;
    assert(step_btl1(&in, 0, 0) == 1 && log_n == 7 && is(5, AUTOBLUR, 0, 0, 0, 0) && is(6, FOCUS, 0, 0x105, 300, 280));
    assert(temp.wCamTime == 0 && temp.bCamLvl == 1);
    /* camera level 1 */
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 4 && is(1, SE, 0, 39, 0, 0) && is(2, AUTOBLUR, 6, 0, 0, 0));
    assert(is(3, FOCUS, 12, 0x105, 100, 180) && temp.wCamTime == 1);
    temp.wCamTime = 10;
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 1 && temp.wCamTime == 11);
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 2 && is(1, AUTOBLUR, 4, 0, 0, 0));
    temp.wCamTime = 22;
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 2 && temp.wCamTime == 23);
    openrate_answer = 0;
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 3 && is(2, OPENRATE, 1, 5, 0, 0));
    openrate_answer = 255, ctlc_answer = 0;
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 4 && is(3, CTLC, 1, 5, 0, 0));
    ctlc_answer = 128, standby_answer = 0;
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 5 && is(4, STANDBY, 0x105, 0, 0, 0) && temp.bCamLvl == 1);
    standby_answer = 1;
    assert(step_btl1(&in, 0, 1) == 1 && log_n == 7 && is(5, AUTOBLUR, 0, 0, 0, 0) && log_id[6] == RESTORE && log_p[6] == &temp);
    assert(log_a[6] == 0 && temp.wCamTime == 0 && temp.bCamLvl == 2);
    /* level 0 */
    isload_answer = 0;
    assert(step_btl1(&in, 0, 2) == 1 && log_n == 2 && log_id[1] == ISLOAD && log_p[1] == &temp && temp.wLvl == 0);
    isload_answer = 1, in.dwPar = 0x0f0002;
    assert(step_btl1(&in, 0, 2) == 1 && log_n == 4 && is(2, BUSY2, 0x304, 1, 0, 0) && is(3, BUSY2, 0x105, 1, 0, 0) && temp.wLvl == 1);
    in.dwPar = 0x500000, in.dwFrom = 0x10304, do_answer = 7, temp.oNum = 0;
    assert(step_btl1(&in, 0, 2) == 1 && log_n == 8 && is(2, BUSY2, 0x304, 1, 0, 0) && is(4, POPUP, 0, -120, 80, 260));
    assert(log_id[5] == DOEFF && log_p[5] == &temp && is(6, POPUP, 0, -120, 80, 260) && log_id[7] == DOEFF);
    assert(do_prev.eNO == 513 && do_prev.dwFrom == 0 && do_prev.dwTo == 0x304 && do_prev.dwPar == 0);
    assert(do_call.eNO == 513 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0);
    assert(temp.eID[0] == 7 && temp.eID[1] == 7 && temp.oNum == 2 && temp.wLvl == 1);
    in.dwPar = 0x400000, in.dwFrom = 0x304, do_answer = 0, temp.oNum = 0, temp.eID[0] = 9;
    assert(step_btl1(&in, 0, 2) == 1 && log_n == 6 && temp.eID[0] == 0 && temp.oNum == 0);
    in.dwPar = 0x100000;
    assert(step_btl1(&in, 0, 2) == 1 && log_n == 6 && temp.oNum == 1 && do_call.dwTo == 0x304);
    /* level 1 */
    temp.oNum = 0;
    assert(step_btl1(&in, 1, 2) == 1 && log_n == 2 && is(1, CLOSE, 0, 0, 0, 0) && temp.wLvl == 2 && temp.oNum == 0);
    temp.oNum = 2, temp.eID[0] = 3, temp.eID[1] = 4, isuse_answer[4] = 1;
    assert(step_btl1(&in, 1, 2) == 1 && log_n == 3 && is(1, ISUSE, 3, 0, 0, 0) && is(2, ISUSE, 4, 0, 0, 0) && temp.wLvl == 1 && temp.oNum == 2);
    isuse_answer[4] = 0;
    assert(step_btl1(&in, 1, 2) == 1 && log_n == 4 && is(3, CLOSE, 0, 0, 0, 0) && temp.wLvl == 2 && temp.oNum == 0);
    /* level 2 */
    temp.wCamTime = 0, temp.wCount_1 = 128, ctrl_answer = 0x171;
    assert(step_btl1(&in, 2, 2) == 1 && log_n == 4 && is(1, CTRL, 128, 15, -1, 0) && is(2, BLACK, 0x71, 0, 0, 0) && is(3, BLACKM, 0x71, 0, 0, 0));
    assert(temp.wCount_1 == 0x171 && temp.wLvl == 2 && temp.wCamTime == 1);
    ctrl_answer = 0, temp.wCamTime = 5;
    assert(step_btl1(&in, 2, 2) == 1 && log_n == 4 && is(1, CTRL, 0x171, 15, -1, 0) && is(2, BLACK, 0, 0, 0, 0) && is(3, BLACKM, 0, 0, 0, 0));
    assert(temp.wCount_1 == 0 && temp.wLvl == 2 && temp.wCamTime == 6);
    do_answer = 6;
    assert(step_btl1_l2(&in, 0x1018FFFF, 0) == 1 && log_n == 7 && log_id[4] == DOEFF && log_id[5] == DOEFF && is(6, SLEEP, 6, 1, 0, 0));
    assert(do_prev.eNO == 604 && do_prev.dwFrom == 0x304 && do_prev.dwTo == 0x105 && do_prev.dwPar == 0xA0000);
    assert(do_call.eNO == 605 && do_call.dwFrom == 0x304 && do_call.dwTo == 0x105 && do_call.dwPar == 2);
    assert(temp.eID[0] == 6 && temp.eID[1] == 6 && temp.ePos[0] == 0 && temp.ePos[1] == 0 && temp.oNum == 1 && temp.wLvl == 3 && temp.wCamTime == 8);
    assert(step_btl1_l2(&in, 0x400002, 1) == 1 && log_n == 8 && is(4, LPREV, 44, 2, 0, 0) && do_prev.dwPar == 0x130000 && do_call.dwPar == 0);
    assert(step_btl1_l2(&in, 0x10040002, 0) == 1 && log_n == 8 && do_prev.dwPar == 0x11234 && do_call.dwPar == 1);
    assert(step_btl1_l2(&in, 0x500002, 0) == 1 && log_n == 8 && do_prev.dwPar == 0x181234 && do_call.dwPar == 0 && temp.wLvl == 3);
    /* level 3 */
    temp.eID[0] = 3, isuse_answer[3] = 1, status_answer[3] = 0, temp.ePos[0] = 0;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 3 && is(1, ISUSE, 3, 0, 0, 0) && is(2, STATUS, 3, 0, 0, 0) && temp.ePos[0] == 0 && temp.wLvl == 3);
    status_answer[3] = 2, opt3d_answer = 0, towner_answer = 1;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && log_id[3] == TOWNER && is(4, OPT3D, 1, 0, 0, 0) && temp.ePos[0] == 1);
    temp.ePos[0] = 0, opt3d_answer = 1, in.dwPar = 0x1FFFF;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && temp.ePos[0] == 1);
    temp.ePos[0] = 0, in.dwPar = 2, istuto_answer = 1;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 7 && log_id[5] == TOWNER && is(6, ISTUTO, 1, 0, 0, 0) && temp.ePos[0] == 1);
    temp.ePos[0] = 0, istuto_answer = 0;
    assert(step_btl1(&in, 3, 2) == 4 && log_n == 8 && is(7, SLEEP, 3, 1, 0, 0) && temp.ePos[0] == 1 && temp.wLvl == 3);
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && is(3, LOAD, 500, 0, 0, 0) && is(4, SE, 1, 65, 0, 0) && temp.ePos[0] == 2);
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && is(3, LOAD, 500, 0, 0, 0) && temp.ePos[0] == 3);
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 3 && temp.ePos[0] == 3 && temp.wLvl == 3);
    isuse_answer[3] = 0, uctl_by_side = 1, in.dwPar = 0x10002, temp.wCount_1 = 55, temp.oNum = 1, temp.eID[1] = 8, temp.fObj2 = 7;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 8 && is(2, BLACK, 0, 0, 0, 0) && is(3, BLACKM, 0, 0, 0, 0));
    /* the two argument calls of SzDuel_CalcLP come in the host compiler's order */
    assert((is(4, UCTL, 1, 5, 0, 0) && is(5, LPREV, 44, 2, 0, 0)) || (is(5, UCTL, 1, 5, 0, 0) && is(4, LPREV, 44, 2, 0, 0)));
    assert(is(6, CALCLP, 1, 1, 0x1234, 0) && is(7, SLEEP, 8, 0, 0, 0));
    assert(temp.wCount_1 == 0 && temp.oNum == 2 && temp.fObj2 == 0 && temp.wLvl == 4);
    in.dwPar = 0x1FFFF, temp.oNum = 1;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && is(4, SLEEP, 8, 0, 0, 0));
    in.dwPar = 0x3FFFF, temp.oNum = 1;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && is(4, SLEEP, 8, 0, 0, 0));
    in.dwPar = 0x20002, temp.oNum = 1;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 8 && (is(4, UCTL, 3, 4, 0, 0) || is(5, UCTL, 3, 4, 0, 0)) && is(6, CALCLP, 3, 1, 0x1234, 0) && is(7, SLEEP, 8, 0, 0, 0));
    in.dwPar = 2, temp.oNum = 1;
    assert(step_btl1(&in, 3, 2) == 1 && log_n == 5 && temp.oNum == 2 && temp.wLvl == 4);
    /* level 4 */
    temp.oNum = 2, temp.eID[1] = 3, isuse_answer[3] = 1, temp.wCount_1 = 90, ctrl_answer = 105, temp.fObj2 = 0, temp.ePos[1] = 0, in.dwPar = 0x80000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 7 && is(1, CTRL, 90, 15, 1, 0) && is(2, BLACK, 105, 0, 0, 0) && is(3, BLACKM, 105, 0, 0, 0));
    assert(is(4, ISUSE, 3, 0, 0, 0) && is(5, SE, 1, 66, 0, 0) && is(6, SE, 1, 45, 0, 0) && temp.fObj2 == 1 && temp.ePos[1] == 1 && temp.wLvl == 4);
    ctrl_answer = 128, temp.fObj2 = 0, in.dwPar = 0xC0000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 7 && is(2, ISUSE, 3, 0, 0, 0) && is(5, SE, 1, 66, 0, 0) && is(6, SE, 1, 45, 0, 0) && temp.ePos[1] == 3);
    temp.fObj2 = 0, in.dwPar = 0x40000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 5 && temp.ePos[1] == 4);
    temp.fObj2 = 0, in.dwPar = 0;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.ePos[1] == 4);
    temp.fObj2 = 20;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.fObj2 == 21);
    temp.fObj2 = 20, temp.ePos[1] = 0;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 5 && is(3, SE, 0, 2, 0, 0) && is(4, SE, 0, 2, 0, 0));
    temp.fObj2 = 20, in.dwPar = 0x80000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 4 && is(3, SE, 0, 2, 0, 0));
    temp.fObj2 = 20, in.dwPar = 0x40000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 4 && is(3, SE, 0, 2, 0, 0));
    temp.fObj2 = 40, in.dwPar = 0;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.fObj2 == 41);
    temp.fObj2 = 40, temp.ePos[1] = 1;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 5 && is(3, SE, 0, 2, 0, 0) && is(4, SE, 0, 2, 0, 0));
    temp.fObj2 = 40, in.dwPar = 0x80000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 4 && is(3, SE, 0, 2, 0, 0));
    temp.fObj2 = 40, in.dwPar = 0x40000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 4 && is(3, SE, 0, 2, 0, 0));
    temp.fObj2 = 7;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.fObj2 == 8 && temp.wLvl == 4);
    isuse_answer[3] = 0, in.dwPar = 0x1FFFFFF;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.wLvl == 16 && temp.fObj2 == 8);
    in.dwPar = 0x4000000, do_answer = 11;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 7 && is(3, CURSE, 3, 4, 1, 0) && is(4, FOCUS, 0, 0x304, 180, 180) && is(5, POPUP, 0, -120, 80, 264));
    assert(log_id[6] == DOEFF && do_call.eNO == 500 && do_call.dwFrom == 0 && do_call.dwTo == 0x304 && do_call.dwPar == 0);
    assert(temp.eID[0] == 11 && temp.wLvl == 16);
    in.dwPar = 0x4080000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.wLvl == 16);
    in.dwPar = 0x6080000, do_answer = 12;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 7 && is(3, POPUP, 0, -120, 80, 264) && is(4, CURSE, 1, 5, 1, 0) && is(5, FOCUS, 0, 0x105, 180, 180));
    assert(log_id[6] == DOEFF && do_call.eNO == 500 && do_call.dwFrom == 0 && do_call.dwTo == 0x105 && do_call.dwPar == 0);
    assert(temp.eID[0] == 12 && temp.wLvl == 16);
    in.dwPar = 0x2040000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.wLvl == 16);
    in.dwPar = 0x2000000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 7 && is(4, CURSE, 1, 5, 1, 0));
    in.dwPar = 0x8000000;
    assert(step_btl1(&in, 4, 2) == 1 && log_n == 3 && temp.wLvl == 16);
    /* level 5 */
    temp.eID[0] = 3, isuse_answer[3] = 1, temp.wCount_1 = 90, ctrl_answer = 105;
    assert(step_btl1(&in, 5, 2) == 1 && log_n == 5 && is(1, CTRL, 90, 15, 1, 0) && is(2, BLACK, 105, 0, 0, 0) && is(3, BLACKM, 105, 0, 0, 0));
    assert(is(4, ISUSE, 3, 0, 0, 0) && temp.wLvl == 5);
    ctrl_answer = 128;
    assert(step_btl1(&in, 5, 2) == 1 && log_n == 3 && is(2, ISUSE, 3, 0, 0, 0) && temp.wLvl == 5);
    isuse_answer[3] = 0;
    assert(step_btl1(&in, 5, 2) == 1 && log_n == 3 && temp.wLvl == 5 && temp.wCount_1 == 128);
    /* any other level */
    ctrl_answer = 127, temp.wCount_1 = 112;
    assert(step_btl1(&in, 6, 2) == 1 && log_n == 4 && is(1, CTRL, 112, 15, 1, 0) && is(2, BLACK, 127, 0, 0, 0) && is(3, BLACKM, 127, 0, 0, 0));
    ctrl_answer = 128, isuse_answer[3] = 1;
    assert(step_btl1(&in, 6, 2) == 1 && log_n == 3 && is(2, ISUSE, 3, 0, 0, 0) && temp.wLvl == 6);
    isuse_answer[3] = 2, disp_answer = 0;
    assert(step_btl1(&in, 6, 2) == 0 && log_n == 7 && is(3, CLOSE, 0, 0, 0, 0) && log_id[4] == RESTORE && log_p[4] == &temp && log_a[4] == 0);
    assert(log_id[5] == WDISP && is(6, CLOSE, 0, 0, 0, 0) && temp.wCount_1 == 128 && temp.wLvl == 6);
    isuse_answer[3] = 0, in.dwFrom = 0x01230304, in.dwTo = 0x81240105, disp_answer = 1;
    assert(step_btl1(&in, 6, 2) == 0 && log_n == 12 && is(5, AP, 3, 4, 1, 0x123) && is(6, DP, 3, 4, 1, 0x123) && is(7, AP, 1, 5, 0, 0x124));
    assert(is(8, DP, 1, 5, 0, 0x124) && log_id[9] == WDISP && is(10, WDRAW, 1, 0, 0, 0) && is(11, CLOSE, 0, 0, 0, 0));
    in.dwFrom = 0x81230304, in.dwTo = 0x01240105, disp_answer = 0;
    assert(step_btl1(&in, 6, 2) == 0 && log_n == 11 && is(5, AP, 3, 4, 0, 0x123) && is(6, DP, 3, 4, 0, 0x123) && is(7, AP, 1, 5, 1, 0x124));
    assert(is(8, DP, 1, 5, 1, 0x124) && is(10, CLOSE, 0, 0, 0, 0));
    uctl_by_side = 0, towner_answer = 0, isload_answer = 0, do_answer = 0, ctrl_answer = 0, ctlc_answer = 0, standby_answer = 0, openrate_answer = 0;
}

enum { ENCOFS = 2000, FORCE, FIREVIEW };
static int force_answer;
int Ta_GetEnchantOfs(void) { note(ENCOFS, NULL, 0, 0, 0, 0); return 77; }
int Ta_GetForceEffection(int a, int b) { note(FORCE, NULL, a, b, 0, 0); return force_answer; }
int TaView_ReqFireView(int a, int b, unsigned char c, unsigned char d) { note(FIREVIEW, NULL, a, b, c, d); return 0; }

static int run_037(unsigned to, unsigned par, int id)
{
    struct jp_6989c e;

    e.eNO = 37, e.dwFrom = 0x0102, e.dwTo = to, e.dwPar = par;
    log_n = 0, trap_n = 0, ld_n = 0;
    return Ta_Eff_037(&e, &temp, id);
}

/* Ta_Eff_037: branch coverage, one case per outcome of each test in the start call and in
 * levels 0, 1, 2, 3, 16, 17, 32, 33 and the default. */
static void check_037(void)
{
    static const int se_at[7][2] = { { 0, 56 }, { 20, 57 }, { 30, 76 }, { 36, 77 }, { 37, 59 }, { 64, 60 }, { 5, 0 } };
    int i;

    memset(&temp, 0, sizeof temp);
    memset(isuse_answer, 0, sizeof isuse_answer);
    memset(status_answer, 0, sizeof status_answer);
    status_once = 0, openrate_side2 = 0, loc_by_unit = 0, loc_answer = 0x0507, do_answer = 9, disp_answer = 0;

    /* start, kind 0, both status bits set, in range: module 537, level 2 with the force value */
    ukind_zero = 1, gustat_zero_pos = -1, ld_answer = 1, force_answer = 6, temp.wCount_2 = 4;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 13 && temp.fObj == 1);
    assert(is(0, INIT, 0, 0, 0, 0) && log_p[0] == &temp && is(1, GUSTAT, 2, 3, 2, 0) && is(2, USTAT, 2, 3, 4, 1));
    assert(is(3, GUSTAT, 1, 2, 2, 0) && is(4, USTAT, 1, 2, 4, 1) && is(5, UKIND, 1, 2, 0, 0));
    assert(is(6, USTAT, 2, 3, 2, 1) && is(7, ENCOFS, 0, 0, 0, 0) && is(8, FORCE, 1, 77, 0, 0));
    assert(is(9, UKIND, 1, 2, 0, 0) && is(10, LOC, 1, 2, 0, 0) && is(11, LOC, 1, 2, 0, 0) && is(12, LOAD, 537, 0, 0, 0));
    assert(ld_n == 1 && ld_args[0] == 1 && ld_args[1] == 2 && ld_args[2] == 7 && ld_args[3] == 5 && ld_args[4] == 11);
    assert(temp.wLvl == 2 && temp.wCount_1 == 6 && temp.wCount_2 == 0);
    /* target status clear, kind nonzero: the range call gets the target; a negative force value is
     * stored as 0 */
    ukind_zero = 0, gustat_zero_pos = 3, force_answer = -3, temp.wCount_1 = 7;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 12 && temp.fObj == 2 && temp.wCount_1 == 0 && temp.wLvl == 2);
    assert(is(2, GUSTAT, 1, 2, 2, 0) && is(3, USTAT, 1, 2, 4, 1) && is(5, USTAT, 2, 3, 2, 1));
    assert(ld_args[0] == 2 && ld_args[1] == 3 && is(9, LOC, 2, 3, 0, 0));
    /* source status clear */
    gustat_zero_pos = 2;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 12 && is(2, USTAT, 2, 3, 4, 1) && is(3, GUSTAT, 1, 2, 2, 0) && is(4, UKIND, 1, 2, 0, 0));
    /* out of range, trap 41 and trap 46 found: modules 602 and 537, the second trap is opened, level 16 */
    ukind_zero = 1, gustat_zero_pos = -1, ld_answer = 0, force_answer = 6;
    trap_seq[0] = 0x0304, trap_seq[1] = 0x0405, temp.wCount_2 = 4;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 19 && temp.wLvl == 16 && temp.fObj == 0x0405);
    assert(temp.wCount_1 == 6 && temp.wCount_2 == 0);
    assert(is(12, TRAPB, 0x29, 41, 0, 0) && is(13, LOAD, 602, 0, 0, 0) && is(14, TRAPB, 0x0203, 46, 0, 0) && is(15, LOAD, 537, 0, 0, 0));
    assert(is(16, FOCUS, 0, 0x0405, 180, 180) && is(17, BUSY2, 0x0405, 0, 0, 0) && is(18, DOOPEN, 4, 5, 1, 0));
    /* trap 46 missing: module 547, the first trap is opened, level 32 */
    trap_seq[1] = -1, temp.wCount_2 = 4;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 20 && temp.wLvl == 32 && temp.fObj == 0x0304);
    assert(temp.wCount_1 == 6 && temp.wCount_2 == 0);
    assert(is(15, LOAD, 547, 0, 0, 0) && is(16, WDRAW, 0, 0, 0, 0) && is(17, FOCUS, 0, 0x0304, 180, 180));
    assert(is(18, BUSY2, 0x0304, 0, 0, 0) && is(19, DOOPEN, 3, 4, 1, 0));
    /* trap 41 missing, trap 43 and trap 46 found */
    trap_seq[0] = -1, trap_seq[1] = 0x0405, trap_seq[2] = 0x0506, temp.wCount_2 = 4;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 20 && temp.wLvl == 16 && temp.fObj == 0x0506);
    assert(temp.wCount_1 == 6 && temp.wCount_2 == 0);
    assert(is(13, TRAPB, 2, 43, 0, 0) && is(14, LOAD, 537, 0, 0, 0) && is(15, LOAD, 602, 0, 0, 0) && is(16, TRAPB, 0x0203, 46, 0, 0));
    assert(is(17, FOCUS, 0, 0x0506, 180, 180) && is(18, BUSY2, 0x0506, 0, 0, 0) && is(19, DOOPEN, 5, 6, 1, 0));
    /* trap 43 alone: it is opened and wCount_2 is 1 */
    trap_seq[2] = -1;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 20 && temp.wLvl == 16 && temp.fObj == 0x0405);
    assert(temp.wCount_1 == 6 && temp.wCount_2 == 1);
    assert(is(17, FOCUS, 0, 0x0405, 180, 180) && is(18, BUSY2, 0x0405, 0, 0, 0) && is(19, DOOPEN, 4, 5, 1, 0));
    /* no trap at all: module 537, level 2 */
    trap_seq[1] = -1;
    assert(run_037(0x0203, 0x290005, 0) == 1 && log_n == 15 && is(13, TRAPB, 2, 43, 0, 0) && is(14, LOAD, 537, 0, 0, 0));
    assert(temp.wLvl == 2 && temp.wCount_1 == 6 && temp.wCount_2 == 0);

    /* module not loaded: nothing else happens */
    memset(&temp, 0, sizeof temp);
    isload_answer = 0;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 1 && is(0, ISLOAD, 0, 0, 0, 0) && log_p[0] == &temp && temp.wLvl == 0);
    /* level 0: effect 547 from the side opposite the target's */
    isload_answer = 1, temp.wCount_1 = 6;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 4 && is(1, WDRAW, 0, 0, 0, 0) && is(2, POPUP, 0, -120, 80, 267));
    assert(is(3, DOEFF, 0, 0, 0, 0) && log_p[3] == &temp);
    assert(do_call.eNO == 547 && do_call.dwFrom == 0 && do_call.dwTo == 0x0200 && do_call.dwPar == 6);
    assert(temp.eID[0] == 9 && temp.oNum == 1 && temp.wLvl == 1);
    temp.wLvl = 0;
    assert(run_037(0x0003, 5, -1) == 1 && do_call.dwFrom == 0x0100 && do_call.dwTo == 0 && temp.oNum == 2 && temp.wLvl == 1);

    /* level 1: effect running, the fire view is requested at count 6 only */
    isuse_answer[9] = 1, temp.wCamTime = 5;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 3 && is(1, WDRAW, 0, 0, 0, 0) && is(2, ISUSE, 9, 0, 0, 0));
    assert(temp.wCamTime == 6 && temp.wLvl == 1);
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 4 && is(3, FIREVIEW, 30, 0, 100, 180) && temp.wCamTime == 7);
    temp.wCamTime = 6;
    assert(run_037(0, 5, -1) == 1 && log_n == 4 && is(3, FIREVIEW, 30, 0x0100, 100, 180));
    /* effect over, kind 0: the target is removed, life points changed, level 64 */
    isuse_answer[9] = 0, ukind_zero = 1, disp_answer = 0;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 9 && is(3, CLOSE, 0, 0, 0, 0) && is(4, UKIND, 1, 2, 0, 0));
    assert(is(5, TDEL, 2, 3, 0, 0) && is(6, CALCLP, 1, 1, 6, 0) && is(7, RESTORE, 16, 0, 0, 0) && log_p[7] == &temp);
    assert(is(8, WDISP, 0, 0, 0, 0) && temp.wCamTime == 0 && temp.wLvl == 64);
    /* kind nonzero: the source is removed; window shown: it is drawn again */
    temp.wLvl = 1, ukind_zero = 0, disp_answer = 1;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 10 && is(5, TDEL, 1, 2, 0, 0) && is(9, WDRAW, 1, 0, 0, 0) && temp.wLvl == 64);

    /* level 2: message by wCount_2, effect 537 with the packed parameter */
    temp.wLvl = 2, temp.wCount_1 = 6, temp.wCount_2 = 1, ukind_zero = 1;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 5 && is(1, MSG, 816, 0, 0, 0) && is(2, POPUP, 0, 0, -160, 0x4D5347));
    assert(is(3, UKIND, 1, 2, 0, 0) && is(4, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 537 && do_call.dwFrom == 0x0102 && do_call.dwTo == 0x0203 && do_call.dwPar == 0x060101);
    assert(temp.wLvl == 3);
    temp.wLvl = 2, temp.wCount_2 = 0, ukind_zero = 0;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 4 && is(1, POPUP, 0, 0, -160, 268) && do_call.dwPar == 0x060002 && temp.wLvl == 3);

    /* level 3: one sound per listed count, none otherwise */
    isuse_answer[9] = 1, status_answer[9] = 1;
    for (i = 0; i < 7; i++) {
        temp.fObj2 = se_at[i][0];
        assert(run_037(0x0203, 5, -1) == 1 && (int)temp.fObj2 == se_at[i][0] + 1 && is(1, ISUSE, 9, 0, 0, 0));
        if (se_at[i][1] != 0) {
            assert(log_n == 4 && is(2, SE, 1, se_at[i][1], 0, 0) && is(3, STATUS, 9, 0, 0, 0));
        } else {
            assert(log_n == 3 && is(2, STATUS, 9, 0, 0, 0));
        }
    }
    /* status 2, kind 0: the source is enchanted, the target removed, its status bit cleared when bit 4
     * is clear */
    status_answer[9] = 2, ukind_zero = 1, gustat_zero_pos = 2, temp.wCount_2 = 1, temp.fObj2 = 5, temp.ePos[0] = 0;
    assert(run_037(0x0203, 0x12A4005, -1) == 1 && log_n == 8 && is(3, UKIND, 1, 2, 0, 0));
    assert(is(4, ENCH, 1, 2, 0x010005, 0x2A) && is(5, TDEL, 2, 3, 0, 0) && is(6, GUSTAT, 1, 2, 4, 0) && is(7, USTAT, 1, 2, 2, 0));
    assert(temp.ePos[0] == 0xFFFF && temp.wLvl == 3);
    gustat_zero_pos = -1, temp.fObj2 = 5;
    assert(run_037(0x0203, 0x12A4005, -1) == 1 && log_n == 7 && is(6, GUSTAT, 1, 2, 4, 0) && temp.ePos[0] == 0xFFFF);
    /* kind nonzero: the target is enchanted and the source removed */
    ukind_zero = 0, gustat_zero_pos = 3, temp.fObj2 = 5, temp.ePos[0] = 0;
    assert(run_037(0x0203, 0x12A4005, -1) == 1 && log_n == 8 && is(4, ENCH, 2, 3, 0x010005, 0x2A) && is(5, TDEL, 1, 2, 0, 0));
    assert(is(6, GUSTAT, 2, 3, 4, 0) && is(7, USTAT, 2, 3, 2, 0) && temp.ePos[0] == 0xFFFF);
    gustat_zero_pos = -1, temp.fObj2 = 5;
    assert(run_037(0x0203, 0x12A4005, -1) == 1 && log_n == 7 && is(6, GUSTAT, 2, 3, 4, 0));
    /* effect over: the sequence ends */
    isuse_answer[9] = 0;
    assert(run_037(0x0203, 5, -1) == 0 && log_n == 5 && is(2, CLOSE, 0, 0, 0, 0) && is(3, SE, 0, 2, 0, 0) && is(4, CLOSE, 0, 0, 0, 0));

    /* level 16: waits for the trap to be open, then effect 602 on it */
    temp.wLvl = 16, temp.fObj = 0x0405, temp.oNum = 0, temp.ePos[0] = 3, openrate_answer = 254;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 2 && is(1, OPENRATE, 4, 5, 0, 0) && temp.wLvl == 16);
    openrate_answer = 255, temp.wCount_2 = 1, do_answer = 8;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 4 && is(2, POPUP, 0, -120, 80, 259) && is(3, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x0405 && do_call.dwPar == 0);
    assert(temp.eID[0] == 8 && temp.ePos[0] == 0 && temp.oNum == 1 && temp.wLvl == 17);
    temp.wLvl = 16, temp.wCount_2 = 0;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 5 && is(2, MSG, 825, 0, 0, 0) && is(3, POPUP, 0, -120, 80, 0x4D5347) && temp.wLvl == 17);

    /* level 17: waits while the effect answers 1, then back to level 2 */
    isuse_answer[8] = 1;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 2 && is(1, ISUSE, 8, 0, 0, 0) && temp.wLvl == 17);
    isuse_answer[8] = 0;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 5 && is(2, USTAT, 4, 5, 0, 1) && is(3, CLOSE, 0, 0, 0, 0));
    assert(is(4, RESTORE, 0, 0, 0, 0) && log_p[4] == &temp && temp.wLvl == 2);

    /* level 32: as level 16 with message 814 */
    temp.wLvl = 32, temp.oNum = 0, temp.ePos[0] = 3, openrate_answer = 254;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 3 && is(1, WDRAW, 0, 0, 0, 0) && is(2, OPENRATE, 4, 5, 0, 0) && temp.wLvl == 32);
    openrate_answer = 255;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 5 && is(3, POPUP, 0, -120, 80, 814) && is(4, DOEFF, 0, 0, 0, 0));
    assert(do_call.eNO == 602 && do_call.dwFrom == 0 && do_call.dwTo == 0x0405 && do_call.dwPar == 0);
    assert(temp.eID[0] == 8 && temp.ePos[0] == 0 && temp.oNum == 1 && temp.wLvl == 33);

    /* level 33: waits for the effect, then the camera goes back and level 0 follows */
    isuse_answer[8] = 2;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 3 && is(1, WDRAW, 0, 0, 0, 0) && is(2, ISUSE, 8, 0, 0, 0) && temp.wLvl == 33);
    isuse_answer[8] = 0;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 5 && is(3, USTAT, 4, 5, 0, 1) && is(4, BACKFOCUS, 0, 0x0400, 100, 180));
    assert(temp.wLvl == 0);

    /* any other level: counts to 17, then ends */
    temp.wLvl = 64, temp.wCamTime = 16;
    assert(run_037(0x0203, 5, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    assert(run_037(0x0203, 5, -1) == 0 && log_n == 3 && is(1, SE, 0, 2, 0, 0) && is(2, CLOSE, 0, 0, 0, 0) && temp.wCamTime == 18);
    gustat_zero_pos = -1, ukind_zero = 0;
}

enum { CCINFO = 5800, SUMPIECE };
static struct jp_6b127 cconst_058[8];
static int sumpiece_answer[4];
struct jp_6b127 *SzDuel_GetCardConstInfo(int id) { note(CCINFO, NULL, id, 0, 0, 0); return &cconst_058[id & 7]; }
int Ta_GetSummonablePieceWithOrder(int side, int i, int c) { note(SUMPIECE, NULL, side, i, c, 0); return sumpiece_answer[i]; }
static void reset_058(int own)
{
    memset(&temp, 0, sizeof(temp));
    memset(isuse_answer, 0, sizeof(isuse_answer));
    memset(status_answer, 0, sizeof(status_answer));
    temp.wLvl = 3, temp.oNum = 3, temp.eID[0] = 5, temp.eID[1] = 9, temp.eID[2] = 11;
    temp.ePos[0] = 0x203, temp.ePos[1] = 0x104, temp.ePos[2] = 0xFFFF;
    if (own) {
        isuse_answer[5] = 1, status_answer[5] = 2;
    }
}
/* Ta_Eff_058 (branch coverage: the start call; modules loading and loaded; the first effect running
 * and gone; the summon loop with no entry, a found place and no place; level 3 with every effect
 * gone below and above the wait bound, an effect in another status, a consumed place, the grave
 * search won by the own grave (equal strength: the later card), won by the other grave (strictly
 * stronger only), with no eligible kind, with empty graves, with and without a new unit; the
 * closing wait at its bound with the window shown and hidden). */
static void check_058(void)
{
    struct jp_6989c in = {58, 300, 1, 2};
    struct jp_6a917 unit;
    int i;

    memset(&temp, 0, sizeof(temp));
    assert(step(Ta_Eff_058, &in, 0) == 1 && log_n == 5 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == INIT && log_p[1] == &temp);
    assert(is(2, STDVIEW, 16, 0, 0, 0) && is(3, LOAD, 508, 0, 0, 0) && is(4, LOAD, 501, 0, 0, 0));
    /* level 0 */
    isload_drop = 0, isload_answer = 0, do_answer = 7;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 2 && is(0, WDRAW, 0, 0, 0, 0) && log_id[1] == ISLOAD && temp.wLvl == 0);
    isload_answer = 1;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 3 && log_id[2] == DOEFF && log_p[2] == &temp);
    assert(do_call.eNO == 508 && do_call.dwFrom == 0 && do_call.dwTo == 1 && do_call.dwPar == 300);
    assert(temp.eID[0] == 7 && temp.ePos[0] == 0xFFFF && temp.wLvl == 1);
    /* level 1 */
    memset(isuse_answer, 0, sizeof(isuse_answer)), isuse_answer[7] = 1;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 2 && is(1, ISUSE, 7, 0, 0, 0) && temp.wLvl == 1);
    isuse_answer[7] = 0;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 3 && is(2, CALCLP, 1, 1, 300, 0) && temp.wLvl == 2);
    /* level 2 */
    sumpiece_answer[0] = 0x203, sumpiece_answer[1] = -1, do_answer = 9;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 5 && is(1, SUMPIECE, 1, 0, 1, 0) && log_id[2] == DOEFF && is(3, SUMPIECE, 1, 1, 1, 0));
    assert(do_prev.eNO == 501 && do_prev.dwFrom == 0 && do_prev.dwTo == 0x203 && do_prev.dwPar == 0);
    assert(do_call.eNO == 501 && do_call.dwFrom == 0 && do_call.dwTo == (unsigned)-1 && do_call.dwPar == 0);
    assert(temp.eID[0] == 9 && temp.eID[1] == 9 && temp.ePos[0] == 0x203 && temp.ePos[1] == 0xFFFF && temp.oNum == 2 && temp.wLvl == 3);
    memset(&temp, 0, sizeof(temp)), temp.wLvl = 2, in.dwPar = 0;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 1 && temp.oNum == 0 && temp.wLvl == 3);
    /* level 3: every effect gone, below and above the wait bound */
    reset_058(0), temp.wCamTime = 30;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 4 && is(1, ISUSE, 5, 0, 0, 0) && is(3, ISUSE, 11, 0, 0, 0) && temp.wCamTime == 31 && temp.wLvl == 3);
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 5 && is(4, RESTORE, 16, 0, 0, 0) && log_p[4] == &temp && temp.wCamTime == 0 && temp.wLvl == 4);
    /* an effect running in another status; one whose place is already consumed */
    reset_058(0), isuse_answer[5] = 1, status_answer[5] = 1, temp.wCamTime = 31;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 5 && is(2, STATUS, 5, 0, 0, 0) && temp.wCamTime == 31 && temp.wLvl == 3);
    reset_058(0), isuse_answer[11] = 1, status_answer[11] = 2;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 5 && is(4, STATUS, 11, 0, 0, 0) && temp.ePos[2] == 0xFFFF);
    /* the own grave wins; of two equally strong cards the later one */
    memset(cconst_058, 0, sizeof(cconst_058));
    for (i = 0; i < 8; i++) {
        cconst_058[i].bKind = 0x1F, cconst_058[i].wAP = 500;
    }
    cconst_058[6].wAP = cconst_058[7].wAP = 600;
    gcount_answer = 3, inqgrave_answer[0] = 2, inqgrave_answer[1] = 999, inqgrave_answer[2] = 3, inqgrave_side_add = 0;
    memset(&unit, 0, sizeof(unit)), addunit_answer = &unit;
    reset_058(1);
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 20 && is(3, GCOUNT, 1, 0, 0, 0) && is(4, INQGRAVE, 1, 0, 0, 0) && is(5, CCINFO, 2, 0, 0, 0));
    assert(is(6, INQGRAVE, 1, 1, 0, 0) && is(7, INQGRAVE, 1, 2, 0, 0) && is(8, CCINFO, 3, 0, 0, 0) && is(9, GCOUNT, 0, 0, 0, 0) && is(10, INQGRAVE, 0, 0, 0, 0));
    assert(is(15, ADDUNIT, 1, 3, 2, 3) && addunit_e == 0 && is(16, DELGRAVE, 1, 2, 0, 0) && is(17, PACKGRAVE, 1, 0, 0, 0) && is(19, ISUSE, 11, 0, 0, 0));
    assert(unit.fFinish != 0 && temp.ePos[0] == 0xFFFF && temp.ePos[1] == 0x104);
    /* the other grave wins only with a strictly stronger card: its first 600, not the second */
    in.dwTo = 0, inqgrave_side_add = 4, addunit_answer = NULL;
    reset_058(1);
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 21 && is(3, GCOUNT, 0, 0, 0, 0) && is(9, GCOUNT, 1, 0, 0, 0) && is(10, INQGRAVE, 1, 0, 0, 0));
    assert(is(11, CCINFO, 6, 0, 0, 0) && is(16, ADDUNIT, 0, 3, 2, 6) && is(17, DELGRAVE, 1, 0, 0, 0) && is(18, PACKGRAVE, 1, 0, 0, 0) && temp.ePos[0] == 0xFFFF);
    /* no card of an eligible kind; empty graves */
    for (i = 0; i < 8; i++) {
        cconst_058[i].bKind = 0x20;
    }
    reset_058(1);
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 18 && is(15, CCINFO, 7, 0, 0, 0) && is(16, ISUSE, 9, 0, 0, 0) && temp.ePos[0] == 0x203);
    gcount_answer = 0;
    reset_058(1);
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 7 && is(3, GCOUNT, 0, 0, 0, 0) && is(4, GCOUNT, 1, 0, 0, 0) && is(5, ISUSE, 9, 0, 0, 0) && temp.ePos[0] == 0x203);
    inqgrave_side_add = 0;
    /* closing wait */
    memset(&temp, 0, sizeof(temp)), temp.wLvl = 4, temp.wCamTime = 16, disp_answer = 0;
    assert(step(Ta_Eff_058, &in, -1) == 1 && log_n == 1 && temp.wCamTime == 17);
    assert(step(Ta_Eff_058, &in, -1) == 0 && log_n == 3 && log_id[1] == WDISP && log_id[2] == CLOSE && temp.wCamTime == 18);
    disp_answer = 1;
    assert(step(Ta_Eff_058, &in, -1) == 0 && log_n == 4 && is(2, WDRAW, 1, 0, 0, 0) && log_id[3] == CLOSE);
}
int main(void)
{
    check_057();
    check_029();
    check_029_cng();
    check_030();
    check_017_megumi();
    check_066();
    check_006_wind();
    check_006();
    check_003b();
    check_012();
    check_102();
    check_063();
    check_056();
    check_059();
    check_015();
    check_000();
    check_064();
    check_013();
    check_112();
    check_035();
    check_114();
    check_045();
    check_044();
    check_manage_destroy();
    check_051();
    check_gravity();
    check_032();
    check_031();
    check_028();
    check_014();
    check_067();
    check_113();
    check_002();
    check_004();
    check_025();
    check_007();
    check_020();
    check_100();
    check_001();
    check_275740();
    check_2759d0();
    check_054();
    check_036();
    check_008();
    check_etc();
    check_072();
    check_046();
    check_026();
    check_005();
    check_btl_000();
    check_btl_001();
    check_037();
    check_058();
    return 0;
}
