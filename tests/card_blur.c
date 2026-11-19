/* Synthetic host tests for the card_blur group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "dotr/ee/card_blur.h"

enum { MYOT = 1, REQ, CLEAR, INITCO, BUSY, SCREEN, LOOK, SETCO, START, DRAW, TRANSIT, FLAG, FINISH };

static int log_id[40], log_a[40], log_b[40], log_n;
static void *log_p[40], *log_q[40];
static struct jp_aa2f3 req_from, req_to;
static struct jp_aa2f3 *req_from_p, *req_to_p;

static void note(int id, void *p, void *q, int a, int b)
{
    assert(log_n < 40);
    log_id[log_n] = id, log_p[log_n] = p, log_q[log_n] = q, log_a[log_n] = a, log_b[log_n] = b;
    log_n++;
}

static int is(int n, int id, void *p, void *q, int a, int b)
{
    return log_id[n] == id && log_p[n] == p && log_q[n] == q && log_a[n] == a && log_b[n] == b;
}

/* Stubs. Assumed: the request lookup answers per packed unit (100 the source, 200 the
 * target); func_00195108 only records its arguments (the tests clear the block themselves,
 * since the host block is larger than 0x430 bytes); the screen-point stub stores its two
 * coordinates in the output; Ta_LkCheckFlag answers bit b of a. */
int *ExGetMyOT(void) { note(MYOT, 0, 0, 0, 0); return 0; }
struct jp_aa2f3 *SzEffect_GetCardRequest2(int packed)
{
    note(REQ, 0, 0, packed, 0);
    return packed == 100 ? req_from_p : packed == 200 ? req_to_p : 0;
}
void func_00195108(void *a, int b) { note(CLEAR, a, 0, b, 0); }
void Ta_LkInitCoordinate(struct jp_aa4a2 *m) { note(INITCO, m, 0, 0, 0); }
int SzEffect_SetBusyFlag2(int packed, int flag) { note(BUSY, 0, 0, packed, flag); return 0; }
void Ex3d_GetPointOnScreen(struct jp_aae7b *out, float x, float y)
{
    note(SCREEN, out, 0, (int)x, (int)y);
    out->x = x, out->y = y, out->z = 3.0f, out->w = 1.0f;
}
void Ex3d_GetLookVector(struct jp_aa35d *out)
{
    note(LOOK, out, 0, 0, 0);
    out->x = 11, out->y = 12, out->z = 13, out->w = 14;
}
void Ta_LkSetCoordinate(struct jp_aa4a2 *frame) { note(SETCO, frame, 0, 0, 0); }
void SzDuel_StartCardSession(void) { note(START, 0, 0, 0, 0); }
void SzDuel_FinishCardSession(void) { note(FINISH, 0, 0, 0, 0); }
int Ta_LkPrim_DrawCard(struct jp_aa2f3 *owner, int alpha, void *point)
{
    note(DRAW, owner, point, alpha, 0);
    return 0;
}
void Ta_LkOBJ_TransitVector(struct jp_aa2f3 *dst, struct jp_aa2f3 *src, int n) { note(TRANSIT, dst, src, n, 0); }
int Ta_LkCheckFlag(int a, int b)
{
    note(FLAG, 0, 0, a, b);
    return (int)(((unsigned int)a >> b) & 1);
}

static struct jp_aa4a2 obj;
static struct jp_ab71b info = { 615, 100, 200, 0 };

static void world(int from, int to)
{
    memset(&req_from, 0, sizeof req_from);
    memset(&req_to, 0, sizeof req_to);
    req_from.Draw.pntPos.x = 1.0f, req_from.Draw.pntPos.y = 2.0f, req_from.Draw.pntPos.z = 3.5f, req_from.Draw.pntPos.w = 4.0f;
    req_from.Draw.ivecRot.x = 21, req_from.Draw.ivecRot.y = 22, req_from.Draw.ivecRot.z = 23, req_from.Draw.ivecRot.w = 24;
    req_from.Draw.icolRgba.r = 9, req_from.Draw.icolRgba.a = 0x80;
    req_from.Draw.nMax = 5, req_from.Draw.fBusy = -1;
    req_from.pVarInfo = &info;
    req_to.Draw.pntPos.x = 5.0f, req_to.Draw.pntPos.y = 6.0f, req_to.Draw.pntPos.z = 7.0f, req_to.Draw.pntPos.w = 8.0f;
    req_to.Draw.ivecRot.x = 31, req_to.Draw.ivecRot.y = 32, req_to.Draw.ivecRot.z = 33, req_to.Draw.ivecRot.w = 34;
    req_to.Draw.vecScale.z = 0.25f, req_to.Draw.vecScale.w = 0.5f;
    req_to.Draw.icolRgba.g = 7, req_to.Draw.icolRgba.a = 0x70;
    req_to.Draw.nNode = 3;
    req_from_p = from ? &req_from : 0;
    req_to_p = to ? &req_to : 0;
    log_n = 0;
}

/* Neither unit has a card request: 8, and nothing after the two lookups. */
static void check_no_requests(void)
{
    memset(&obj, 0x55, sizeof obj);
    world(0, 0);
    assert(TaLib_c615(&obj, &info, 0) == 8);
    assert(log_n == 3 && is(0, MYOT, 0, 0, 0, 0) && is(1, REQ, 0, 0, 100, 0) && is(2, REQ, 0, 0, 200, 0));
    assert(obj.kind == 0x55555555 && obj.count == 0x55555555);
}

/* nID >= 0 with both requests: the whole set-up, then the first drawing step (kind 0). */
static void check_setup_both(void)
{
    int i;

    memset(&obj, 0, sizeof obj);
    obj.count = 77, obj.effflags = 0x40;
    world(1, 1);
    assert(TaLib_c615(&obj, &info, 0) == 0);
    assert(log_n == 17);
    assert(is(3, CLEAR, &obj, 0, 0x430, 0) && is(4, INITCO, &obj, 0, 0, 0));
    assert(is(5, BUSY, 0, 0, 100, 1) && is(6, BUSY, 0, 0, 200, 1));
    assert(is(7, SCREEN, &obj.CardReqF.Draw.pntPos, 0, -125, -8) && is(8, LOOK, &obj.CardReqF.Draw.ivecLook, 0, 0, 0));
    assert(is(9, SCREEN, &obj.CardReqT.Draw.pntPos, 0, 125, -8) && is(10, LOOK, &obj.CardReqT.Draw.ivecLook, 0, 0, 0));
    assert(is(11, SETCO, &obj, 0, 0, 0) && is(12, START, 0, 0, 0, 0));
    assert(is(13, DRAW, &obj.CardReqF, 0, 128, 0) && is(14, DRAW, &obj.CardReqT, 0, 128, 0));
    assert(is(15, FINISH, 0, 0, 0, 0) && is(16, FLAG, 0, 0, 3, 31));
    /* copies of the requests, with the scale and look bits set and the others kept */
    assert(obj.CardReqF.pVarInfo == &info && obj.CardReqF.Draw.icolRgba.r == 9 && obj.CardReqF.Draw.icolRgba.a == 0x80);
    assert(obj.CardReqF.Draw.nMax == 5 && obj.CardReqF.Draw.fBusy == -1 && obj.CardReqF.Draw.fBig == 0 && obj.CardReqF.Draw.fColor == 0);
    assert(obj.CardReqF.Draw.fScale == -1 && obj.CardReqF.Draw.fLook == -1);
    assert(obj.CardReqT.Draw.nNode == 3 && obj.CardReqT.Draw.fScale == -1 && obj.CardReqT.Draw.fLook == -1 && obj.CardReqT.Draw.nMax == 0);
    assert(obj.CardReqT.Draw.icolRgba.g == 7 && obj.CardReqT.Draw.icolRgba.a == 0x70);
    /* the live requests lose their alpha */
    assert(req_from.Draw.icolRgba.a == 0 && req_from.Draw.icolRgba.r == 9 && req_to.Draw.icolRgba.a == 0 && req_to.Draw.icolRgba.g == 7);
    assert(obj.effflags == 3 && obj.kind == 0 && obj.cardmove == 14 && obj.count == 0 && obj.fadeF == 128 && obj.fadeT == 128);
    assert(obj.corF.pntScr.x == -125.0f && obj.corF.pntScr.y == -8.0f && obj.corT.pntScr.x == 125.0f && obj.corT.pntScr.y == -8.0f);
    assert(obj.corF.nCount == 1 && obj.corT.nCount == 1);
    /* source card: screen point in the copy, saved position and rotation from the live request */
    assert(obj.CardReqF.Draw.pntPos.x == -125.0f && obj.CardReqF.Draw.pntPos.y == -8.0f);
    assert(obj.corF.pntPos.x == 1.0f && obj.corF.pntPos.y == 2.0f && obj.corF.pntPos.z == 3.5f && obj.corF.pntPos.w == 4.0f);
    assert(obj.CardReqF.Draw.ivecLook.x == 11 && obj.CardReqF.Draw.ivecLook.w == 14);
    assert(obj.corF.ivecLook.x == 0 && obj.corF.ivecLook.y == 0 && obj.corF.ivecLook.z == 0);
    assert(obj.CardReqF.Draw.ivecRot.x == 0 && obj.CardReqF.Draw.ivecRot.y == 0x10000 && obj.CardReqF.Draw.ivecRot.z == 0x10000 && obj.CardReqF.Draw.ivecRot.w == 24);
    assert(obj.corF.ivecRot.x == 21 && obj.corF.ivecRot.y == 22 && obj.corF.ivecRot.z == 23 && obj.corF.ivecRot.w == 24);
    assert(obj.corF.vecScale.x == 0.1f && obj.corF.vecScale.y == 0.1f);
    assert(obj.CardReqF.Draw.vecScale.x == 0.46875f && obj.CardReqF.Draw.vecScale.y == 0.48913f);
    /* target card */
    assert(obj.corT.pntPos.x == 5.0f && obj.corT.pntPos.w == 8.0f);
    assert(obj.corT.ivecRot.x == 31 && obj.corT.ivecRot.w == 34 && obj.corT.ivecLook.z == 0);
    assert(obj.corT.vecScale.x == 0.1f && obj.corT.vecScale.y == 0.1f);
    assert(obj.CardReqT.Draw.vecScale.x == 0.46875f && obj.CardReqT.Draw.vecScale.y == 0.48913f && obj.CardReqT.Draw.vecScale.z == 0.25f);
    /* every blur coordinate starts at the target card (the later shift leaves equal values) */
    for (i = 0; i < 4; i++) {
        assert(obj.blur[i].trans.x == 125.0f && obj.blur[i].trans.y == -8.0f && obj.blur[i].trans.z == 3.0f);
        assert(obj.blur[i].rot.x == 0 && obj.blur[i].rot.y == 0x10000 && obj.blur[i].rot.z == 0x10000 && obj.blur[i].rot.w == 34);
        assert(obj.blur[i].scale.x == 0.46875f && obj.blur[i].scale.y == 0.48913f && obj.blur[i].scale.w == 0.5f);
    }
    assert(obj.wcoord.trans.x == 0.0f);
}

/* nID >= 0 with only one request: the other card's set-up, drawing and blur loop are skipped. */
static void check_setup_one(void)
{
    memset(&obj, 0, sizeof obj);
    world(1, 0);
    assert(TaLib_c615(&obj, &info, 0) == 0);
    assert(log_n == 13);
    assert(is(5, BUSY, 0, 0, 100, 1) && is(6, SCREEN, &obj.CardReqF.Draw.pntPos, 0, -125, -8) && is(7, LOOK, &obj.CardReqF.Draw.ivecLook, 0, 0, 0));
    assert(is(8, SETCO, &obj, 0, 0, 0) && is(9, START, 0, 0, 0, 0) && is(10, DRAW, &obj.CardReqF, 0, 128, 0));
    assert(is(11, FINISH, 0, 0, 0, 0) && is(12, FLAG, 0, 0, 1, 31));
    assert(obj.effflags == 1 && obj.corF.nCount == 1 && obj.corT.nCount == 0 && obj.CardReqF.Draw.fScale == -1 && obj.CardReqT.Draw.fScale == 0);
    assert(req_from.Draw.icolRgba.a == 0 && req_to.Draw.icolRgba.a == 0x70);
    assert(obj.blur[0].rot.y == 0 && obj.corT.pntScr.x == 125.0f);

    memset(&obj, 0, sizeof obj);
    world(0, 1);
    /* kind 1 straight away: the target is drawn, not the source; counter 14 only runs down */
    assert(TaLib_c615(&obj, &info, 1) == 0);
    assert(log_n == 14);
    assert(is(5, BUSY, 0, 0, 200, 1) && is(6, SCREEN, &obj.CardReqT.Draw.pntPos, 0, 125, -8) && is(7, LOOK, &obj.CardReqT.Draw.ivecLook, 0, 0, 0));
    assert(is(8, SETCO, &obj, 0, 0, 0) && is(9, START, 0, 0, 0, 0) && is(10, DRAW, &obj.CardReqT, 0, 128, 0));
    assert(is(11, SCREEN, &obj.CardReqF.Draw.pntPos, 0, -125, -8) && is(12, FINISH, 0, 0, 0, 0) && is(13, FLAG, 0, 0, 2, 31));
    assert(obj.effflags == 2 && obj.kind == 1 && obj.cardmove == 13 && obj.corF.nCount == 0 && obj.corT.nCount == 1);
    assert(req_from.Draw.icolRgba.a == 0x80 && req_to.Draw.icolRgba.a == 0);
    assert(obj.blur[3].rot.y == 0x10000 && obj.CardReqF.Draw.fScale == 0);
}

/* nID < 0: no set-up. Kind 1 counter: 7 (no move), 6 (move), 0 with bit 31 clear and set. */
static void check_kind_one(void)
{
    memset(&obj, 0, sizeof obj);
    world(1, 1);
    obj.kind = 1, obj.cardmove = 7, obj.fadeT = 90, obj.corF.pntScr.x = 40.0f, obj.corF.pntScr.y = 50.0f;
    assert(TaLib_c615(&obj, &info, -1) == 0);
    assert(log_n == 9);
    assert(is(3, SETCO, &obj, 0, 0, 0) && is(4, START, 0, 0, 0, 0) && is(5, DRAW, &obj.CardReqT, 0, 90, 0));
    assert(is(6, SCREEN, &obj.CardReqF.Draw.pntPos, 0, 40, 50) && is(7, FINISH, 0, 0, 0, 0) && is(8, FLAG, 0, 0, 0, 31));
    assert(obj.cardmove == 6 && obj.count == 0 && req_from.Draw.icolRgba.a == 0x80);

    log_n = 0;
    assert(TaLib_c615(&obj, &info, -1) == 0);
    assert(log_n == 10 && is(7, TRANSIT, &obj.CardReqT, &obj.CardReqF, 6, 0) && is(8, FINISH, 0, 0, 0, 0));
    assert(obj.cardmove == 5);

    /* counter 0, bit 31 clear: the bit is set, the counter restarts at 27, the call counts */
    log_n = 0;
    obj.cardmove = 0, obj.effflags = 4;
    assert(TaLib_c615(&obj, &info, -1) == 0);
    assert(log_n == 11 && is(7, TRANSIT, &obj.CardReqT, &obj.CardReqF, 0, 0) && is(8, FLAG, 0, 0, 4, 31));
    assert(is(9, FINISH, 0, 0, 0, 0) && is(10, FLAG, 0, 0, (int)0x80000004, 31));
    assert((unsigned int)obj.effflags == 0x80000004u && obj.cardmove == 27 && obj.count == 1);

    /* counter 0, bit 31 already set: flags unchanged */
    log_n = 0;
    obj.cardmove = 0;
    assert(TaLib_c615(&obj, &info, -1) == 0);
    assert((unsigned int)obj.effflags == 0x80000004u && obj.cardmove == 27 && obj.count == 2);
}

/* Kind 0 without a set-up: source drawn only when it has a request; blur shift; call count. */
static void check_draw_shift_count(void)
{
    int i;

    memset(&obj, 0, sizeof obj);
    world(0, 1);
    obj.fadeF = 60, obj.fadeT = 70, obj.cardmove = 9;
    for (i = 0; i < 4; i++) {
        obj.blur[i].trans.x = (float)(i + 1), obj.blur[i].rot.y = 10 * (i + 1), obj.blur[i].scale.z = (float)(100 + i);
    }
    obj.CardReqT.Draw.pntPos.x = 9.0f, obj.CardReqT.Draw.ivecRot.y = 90, obj.CardReqT.Draw.vecScale.z = 99.0f;
    obj.effflags = (int)0x80000000, obj.count = 2;
    assert(TaLib_c615(&obj, &info, -1) == 0);
    assert(log_n == 8 && is(5, DRAW, &obj.CardReqT, 0, 70, 0) && is(6, FINISH, 0, 0, 0, 0));
    assert(obj.blur[3].trans.x == 3.0f && obj.blur[2].trans.x == 2.0f && obj.blur[1].trans.x == 1.0f && obj.blur[0].trans.x == 9.0f);
    assert(obj.blur[3].rot.y == 30 && obj.blur[2].rot.y == 20 && obj.blur[1].rot.y == 10 && obj.blur[0].rot.y == 90);
    assert(obj.blur[3].scale.z == 102.0f && obj.blur[1].scale.z == 100.0f && obj.blur[0].scale.z == 99.0f);
    assert(obj.count == 3 && obj.cardmove == 9);

    /* fourth counted call answers 2; the fifth stores 999 and answers 0; 999 stays 999 */
    world(1, 1);
    assert(TaLib_c615(&obj, &info, -1) == 2);
    assert(log_n == 9 && is(5, DRAW, &obj.CardReqF, 0, 60, 0) && is(6, DRAW, &obj.CardReqT, 0, 70, 0) && obj.count == 4);
    assert(TaLib_c615(&obj, &info, -1) == 0 && obj.count == 999);
    assert(TaLib_c615(&obj, &info, -1) == 0 && obj.count == 999);
    /* bit 31 clear: not counted */
    obj.effflags = 0x7fffffff, obj.count = 3;
    assert(TaLib_c615(&obj, &info, -1) == 0 && obj.count == 3);
}

int main(void)
{
    check_no_requests();
    check_setup_both();
    check_setup_one();
    check_kind_one();
    check_draw_shift_count();
    puts("card_blur: ok");
    return 0;
}
