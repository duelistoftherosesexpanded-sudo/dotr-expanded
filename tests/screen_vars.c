/* Synthetic host tests for the screen_vars group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/screen_vars.h"

struct DotrRhSysAsd gRhSys_asd_view[3];

static struct DotrScreenVars screen;
static int setup_calls;
static int setup_args[7];
static int setup_page[2];

void func_001851e8(int *buffer, int a, int b, int c, int d, int e, int f)
{
    struct DotrScreenDBuff *db = (struct DotrScreenDBuff *)buffer;

    assert(buffer == (int *)&screen);
    setup_calls++;
    setup_args[1] = a;
    setup_args[2] = b;
    setup_args[3] = c;
    setup_args[4] = d;
    setup_args[5] = e;
    setup_args[6] = f;
    /* the index words are already stored when the callee runs */
    assert(screen.unknown_268 == (((5 + screen.zpsm_index) + (5 + screen.psm_index) * 2) << 5));
    db->disp[0].dispfb.FBP = setup_page[0];
    db->disp[1].dispfb.FBP = setup_page[1];
}

static void run(int psm, int zpsm, int page0, int page1)
{
    struct DotrRhSysAsd *row = &DOTR_RH_SYS_ASD[2];

    memset(&screen, 0x55, sizeof(screen));
    memset(gRhSys_asd_view, 0, sizeof(gRhSys_asd_view));
    screen.psm_index = 0;
    screen.zpsm_index = 1;
    row->width = 0x10280;
    row->height = 0x101c0;
    row->unknown_14 = 1.5f;
    row->unknown_18 = 11;
    row->unknown_1c = 12;
    row->pages[0] = 5;
    row->pages[1] = 6;
    setup_calls = 0;
    setup_page[0] = page0;
    setup_page[1] = page1;
    lRhBase_InitScreenVariables(&screen, 2, psm, zpsm);
    assert(setup_calls == 1);
    assert(setup_args[1] == (short)psm && setup_args[2] == 0x280 && setup_args[3] == 0x1c0);
    assert(setup_args[4] == 3 && setup_args[5] == (short)zpsm && setup_args[6] == 1);
    assert(screen.mode == 2 && screen.width == 0x10280 && screen.height == 0x101c0);
    assert(screen.unknown_244 == 1.5f && screen.unknown_248 == 11 && screen.unknown_24c == 12);
    assert(screen.psm == psm && screen.zpsm == zpsm);
    assert(screen.unknown_260 == 2048 && screen.unknown_264 == 2048 && screen.unknown_26c == 0);
    assert(screen.db.clear0.r == 0 && screen.db.clear0.g == 0 && screen.db.clear0.b == 128);
    assert(screen.db.clear0.a == 255 && screen.db.clear0.q == 0.0f);
    assert(screen.db.clear1.r == 0 && screen.db.clear1.g == 0 && screen.db.clear1.b == 128);
    assert(screen.db.clear1.a == 255 && screen.db.clear1.q == 0.0f);
    assert(screen.unknown_230[0] == 0x55555555 && screen.unknown_230[1] == 0x55555555);
}

static void check_init_screen_variables(void)
{
    static const int psm_one[2] = { 0, 1 };
    static const int psm_zero[2] = { 2, 10 };
    static const int zpsm_one[2] = { 48, 49 };
    static const int zpsm_zero[2] = { 50, 58 };
    int i;

    assert(offsetof(struct DotrScreenDBuff, clear0) + 32 == 256);
    assert(offsetof(struct DotrScreenDBuff, draw1) == 336);
    assert(offsetof(struct DotrScreenVars, mode) == 568 && sizeof(struct DotrRhSysAsd) == 40);
    for (i = 0; i < 2; i++) {
        /* each case value of both switches; pages differ, so the block after the call is skipped */
        run(psm_one[i], zpsm_zero[i], 1, 2);
        assert(screen.psm_index == 1 && screen.zpsm_index == 0);
        assert(screen.unknown_268 == (5 + 6 * 2) << 5);
        assert(screen.db.disp[0].dispfb.FBP == 1 && screen.db.disp[1].dispfb.FBP == 2);
        assert(screen.db.draw0.frame.page == 0x155 && screen.db.draw1.zbuf.page == 0x155);
        assert(screen.db.disp[0].display.DH == 0x555 && screen.db.disp[1].display.DH == 0x555);
        run(psm_zero[i], zpsm_one[i], 1, 2);
        assert(screen.psm_index == 0 && screen.zpsm_index == 1);
        assert(screen.unknown_268 == (6 + 5 * 2) << 5);
    }
    /* values outside both switches leave the index words as they were */
    run(3, 51, 1, 2);
    assert(screen.psm_index == 0 && screen.zpsm_index == 1);
    /* equal pages: page 0 for the first display, the row's page for the second, copies into
     * the two draw blocks, twice the page for the depth words and height - 1 (11 bits) */
    run(1, 58, 3, 3);
    assert(screen.db.disp[0].dispfb.FBP == 0 && screen.db.disp[1].dispfb.FBP == 6);
    assert(screen.db.draw0.frame.page == 6 && screen.db.draw1.frame.page == 0);
    assert(screen.db.draw0.zbuf.page == 12 && screen.db.draw1.zbuf.page == 12);
    assert(screen.db.disp[1].display.DH == 0x1bf && screen.db.disp[0].display.DH == 0x1bf);
    assert(screen.db.disp[0].display.DW == 0x555 && screen.db.disp[0].dispfb.unknown == (0x5555555555555555UL >> 9));
}

int main(void)
{
    check_init_screen_variables();
    puts("screen_vars: ok");
    return 0;
}
