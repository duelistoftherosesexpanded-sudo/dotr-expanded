/* Synthetic host tests for the main_loop group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/main_loop.h"

/* Recording stubs: every callee logs an id (its address) and up to two arguments. */
struct DotrMainColor data_0028b840 = {0x11, 0x22, 0x33, 0x44};
static long ev_log[200][3];
static int ev_count, use[3], blur_level, blur_calls;
static struct DotrMainColor blur_color;
static void ev(int id, long a, long b)
{
    assert(ev_count < 200);
    ev_log[ev_count][0] = id;
    ev_log[ev_count][1] = a;
    ev_log[ev_count][2] = b;
    ev_count++;
}
DotrMainWord RhBase_GetVifPacketArea(void) { ev(0x209d20, 0, 0); return 1000; }
void RhBase_SetVifPacketArea(DotrMainWord cursor) { ev(0x209d30, cursor, 0); }
DotrMainWord RhBase_SetZBufferMode(DotrMainWord cursor, int a) { ev(0x20a0b0, cursor, a); return cursor + 1; }
DotrMainWord ExDitherMode_Pipe(DotrMainWord cursor, int value) { ev(0x208530, cursor, value); return cursor + 1; }
DotrMainWord ExDitherMatrixEz_Pipe(DotrMainWord cursor, int index) { ev(0x2085e0, cursor, index); return cursor + 1; }
DotrMainWord SzView_Set_Pipe(DotrMainWord cursor, int index) { ev(0x1f8a30, cursor, index); return cursor + 1; }
DotrMainWord SzBattle_DrawStage_Pipe(DotrMainWord cursor) { ev(0x1e5590, cursor, 0); return cursor + 1; }
DotrMainWord SzDuel_DrawStage_Pipe(DotrMainWord cursor) { ev(0x1ae5a0, cursor, 0); return cursor + 1; }
DotrMainWord RhZSort_Finish_Pipe(DotrMainWord cursor) { ev(0x213fc0, cursor, 0); return cursor + 1; }
int SzText_InitClutManage(int base, int size) { ev(0x202e00, base, size); return 0x10; }
int SzText_DoRequest(int a0, int a1) { ev(0x202ab0, a0, a1); return a0 == 4 ? 1 : a0 == 5 ? 2 : 4; }
int SzText_FreeRequest(int a, int b) { ev(0x202d60, a, b); return 0; }
void SzModel_SetDrawCache(int v) { ev(0x1ee4a0, v, 0); }
void SzModel_DrawMotionBlur(int a) { ev(0x1edf90, a, 0); }
int SzModel_SetMaxOT(int index, int value) { ev(0x1f2ff0, index, value); return 0; }
int SzEffect_IsUse2(int a, int b) { ev(0x1f5240, a, b); return use[a]; }
void SzEffect_Draw(int a) { ev(0x1f40d0, a, 0); }
void ExDitherMatrixEz(int a) { ev(0x2085a0, a, 0); }
void ExZBufferMode(int a) { ev(0x208490, a, 0); }
void ExFlushTexture(void) { ev(0x208460, 0, 0); }
void ExInitPrim(void) { ev(0x207360, 0, 0); }
void ExTermPrim(void) { ev(0x2073a0, 0, 0); }
long ExGetAlphaVal(int index) { ev(0x208d00, index, 0); return 0x55; }
void ExSortDispBlur(int a0, int a1, int a2, long a3, struct DotrMainColor *color)
{
    assert(a0 == 0 && a1 == 0);
    ev(0x208090, a2, a3);
    blur_color = *color;
    blur_calls++;
}
void SzGame_ModeDraw(void) { ev(0x1a9590, 0, 0); }
void SzGame_ModeDrawAfter(void) { ev(0x1a9800, 0, 0); }
void SzGame_ModeDrawLast(void) { ev(0x1a98b0, 0, 0); }
int SzGame_InqAutoBlur(void) { ev(0x1a8690, 0, 0); return blur_level; }
void SzDuel_DrawStageAfter(void) { ev(0x1aee80, 0, 0); }
void SzDuel_DrawUnit(void) { ev(0x1ce400, 0, 0); }
void SzDuel_DrawAllCard(void) { ev(0x1cd6e0, 0, 0); }
void SzFade_DispAll(void) { ev(0x1fcda0, 0, 0); }

static int count_id(int id)
{
    int i, n = 0;

    for (i = 0; i < ev_count; i++) {
        n += ev_log[i][0] == id;
    }
    return n;
}
/* Arguments of the n-th (from 0) logged call of id. */
static long *nth(int id, int n)
{
    int i;

    for (i = 0; i < ev_count; i++) {
        if (ev_log[i][0] == id && n-- == 0) {
            return ev_log[i];
        }
    }
    assert(0);
    return 0;
}
static void run(int u0, int u1, int u2, int blur)
{
    use[0] = u0;
    use[1] = u1;
    use[2] = u2;
    blur_level = blur;
    blur_calls = 0;
    ev_count = 0;
    memset(&blur_color, 0, sizeof blur_color);
    Main_Draw();
}
/* Main_Draw (branch coverage: each short-circuit outcome of the three idle tests, the
 * 84-step loop bound, blur level below, at and above zero; the full call order once). */
static void check_draw(void)
{
    static const long head[][3] = {
        {0x209d20, 0, 0}, {0x202e00, 0, 0}, {0x1ee4a0, 0, 0}, {0x20a0b0, 1000, 1}, {0x208530, 1001, 1},
        {0x2085e0, 1002, 1}, {0x1f8a30, 1003, 0}, {0x1e5590, 1004, 0}, {0x1ae5a0, 1005, 0},
        {0x213fc0, 1006, 0}, {0x209d30, 1007, 0}, {0x2085a0, 2, 0}, {0x1a9590, 0, 0}, {0x1ee4a0, 0, 0},
        {0x208490, 0, 0}, {0x2085a0, 3, 0}, {0x1f5240, 0, -1}, {0x1f5240, 1, -1}, {0x1f5240, 2, -1},
        {0x202d60, 4, -1}, {0x1f5240, 0, -1}, {0x1f5240, 1, -1}, {0x202d60, 5, -1}, {0x202d60, 8, -1},
        {0x1f5240, 1, -1}, {0x1f5240, 2, -1}, {0x202d60, 6, -1}, {0x202e00, 0x3bc0, 2},
        {0x202ab0, 4, 0x3bd0}, {0x202ab0, 5, 0x3bd1}, {0x202ab0, 6, 0x3bd3}, {0x202ab0, 8, 0x3bd7},
        {0x208460, 0, 0}, {0x207360, 0, 0}, {0x1aee80, 0, 0}, {0x1edf90, -1, 0}, {0x1ce400, 0, 0}
    };
    static const long tail[][3] = {
        {0x1f40d0, 0, 0}, {0x1f40d0, 1, 0}, {0x1f40d0, 2, 0}, {0x1cd6e0, 0, 0}, {0x2073a0, 0, 0},
        {0x208490, 1, 0}, {0x1a9800, 0, 0}, {0x2085a0, 2, 0}, {0x1a8690, 0, 0}, {0x208d00, 0, 0},
        {0x208090, 1, 0x55}, {0x1fcda0, 0, 0}, {0x2085a0, 3, 0}, {0x1a98b0, 0, 0}
    };
    int i;

    run(0, 0, 0, 3);
    assert(ev_count == 37 + 84 + 14);
    for (i = 0; i < 37; i++) {
        assert(ev_log[i][0] == head[i][0] && ev_log[i][1] == head[i][1] && ev_log[i][2] == head[i][2]);
    }
    for (i = 0; i < 84; i++) {
        assert(ev_log[37 + i][0] == 0x1f2ff0 && ev_log[37 + i][1] == i && ev_log[37 + i][2] == 0);
    }
    for (i = 0; i < 14; i++) {
        assert(ev_log[121 + i][0] == tail[i][0] && ev_log[121 + i][1] == tail[i][1] && ev_log[121 + i][2] == tail[i][2]);
    }
    assert(blur_calls == 1 && blur_color.r == 0x11 && blur_color.g == 0x22 && blur_color.b == 0x33);
    assert(blur_color.a == 0x30 && data_0028b840.a == 0x44);

    /* Slot 0 busy: the first two tests stop after one call each; only request 6 is freed. */
    run(1, 0, 0, 0);
    assert(count_id(0x1f5240) == 4 && count_id(0x202d60) == 1 && nth(0x202d60, 0)[1] == 6);
    assert(nth(0x1f5240, 1)[1] == 0 && nth(0x1f5240, 2)[1] == 1 && nth(0x1f5240, 3)[1] == 2);
    assert(blur_calls == 0 && count_id(0x208d00) == 0);

    /* Slot 1 busy: every test stops at its slot-1 call; nothing is freed. */
    run(0, 1, 0, -1);
    assert(count_id(0x1f5240) == 5 && count_id(0x202d60) == 0 && blur_calls == 0);

    /* Slot 2 busy: requests 5 and 8 only. */
    run(0, 0, 1, 1);
    assert(count_id(0x1f5240) == 7 && count_id(0x202d60) == 2);
    assert(nth(0x202d60, 0)[1] == 5 && nth(0x202d60, 1)[1] == 8);
    assert(blur_calls == 1 && blur_color.a == 0x10 && count_id(0x1f2ff0) == 84);
}

int main(void)
{
    check_draw();
    return 0;
}
