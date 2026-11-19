/* Synthetic host tests for the camp_bun_hantei group. Fixtures are host data, not game data. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_bun_hantei.h"

short dotr_bun_work_01ed5378[0x7a];
signed char dotr_bun_camp_00299e10[0xc8];
short CutSceneNumber_bunhantei;

/* Sets the camp state (the counter at +0xc2 and its own symbol kept equal, as on the target)
 * and the work block, with sentinels in the outputs so that untouched fields show. */
static void setup(int side, int location, int ticks, int page)
{
    short t = (short)ticks;

    memset(dotr_bun_camp_00299e10, 0, sizeof dotr_bun_camp_00299e10);
    memset(dotr_bun_work_01ed5378, 0, sizeof dotr_bun_work_01ed5378);
    dotr_bun_camp_00299e10[0] = (signed char)side;
    dotr_bun_camp_00299e10[0xc6] = (signed char)location;
    memcpy(&dotr_bun_camp_00299e10[0xc2], &t, sizeof t);
    CutSceneNumber_bunhantei = t;
    DOTR_BUN_PAGE = (signed char)page;
    DOTR_BUN_MSG = 0x7777;
    DOTR_BUN_COUNT = 0x55;
    DOTR_BUN_MODE = 0x55;
    DOTR_BUN_NAME0 = DOTR_BUN_NAME1 = 0x6666;
    DOTR_BUN_SIDE0 = DOTR_BUN_SIDE1 = 0x55;
    DOTR_BUN_FLAG0 = DOTR_BUN_FLAG1 = 0x55;
    DOTR_BUN_LOOK0 = DOTR_BUN_LOOK1 = 0x55;
}

static void run(void)
{
    (void)Taku_Camp_BUN_HANTEI();
}

static void check_early(void)
{
    /* Counter below 0xc8, side 0: per location, then the 0x64 special case. */
    setup(0, 2, 0x10, 0);
    run();
    assert(DOTR_BUN_MSG == 0x887 && DOTR_BUN_COUNT == 1 && DOTR_BUN_MODE == 0);
    setup(0, 3, 0x10, 2);
    run();
    assert(DOTR_BUN_MSG == 0x8a0 && DOTR_BUN_COUNT == 6);
    setup(0, 2, 0x64, 0);
    run();
    assert(DOTR_BUN_MSG == 0x86c && DOTR_BUN_COUNT == 1 && DOTR_BUN_MODE == 1);
    /* Location 11 by the state byte of location 11 and the byte at 0x1ed53c2. */
    setup(0, 11, 0x10, 1);
    dotr_bun_camp_00299e10[0xe] = 2;
    run();
    assert(DOTR_BUN_MSG == 0x8e0 && DOTR_BUN_COUNT == 9);
    setup(0, 11, 0x10, 1);
    dotr_bun_camp_00299e10[0xe] = 3;
    DOTR_BUN_BYTE_53C2 = 1;
    run();
    assert(DOTR_BUN_MSG == 0x902 && DOTR_BUN_COUNT == 1);
    setup(0, 11, 0x10, 1);
    dotr_bun_camp_00299e10[0xe] = 0;
    run();
    assert(DOTR_BUN_MSG == 0x7777 && DOTR_BUN_COUNT == 0x55 && DOTR_BUN_MODE == 0);

    /* Side 1: per location, then counters 0x69 and 0x6f. */
    setup(1, 12, 0x10, 3);
    run();
    assert(DOTR_BUN_MSG == 0x938 && DOTR_BUN_COUNT == 2 && DOTR_BUN_MODE == 0);
    setup(1, 12, 0x69, 3);
    run();
    assert(DOTR_BUN_MSG == 0x94e && DOTR_BUN_COUNT == 1 && DOTR_BUN_MODE == 1);
    setup(1, 1, 0x6f, 0);
    run();
    assert(DOTR_BUN_MSG == 0x970 && DOTR_BUN_COUNT == 1 && DOTR_BUN_MODE == 2);
    assert(DOTR_BUN_NAME1 == 0xc5 && DOTR_BUN_SIDE1 == 1 && DOTR_BUN_FLAG1 == 1);
    assert(DOTR_BUN_NAME0 == 0x6666 && DOTR_BUN_FLAG0 == 0x55);
}

static void check_middle(void)
{
    setup(0, 2, 0xc8, 0);
    run();
    assert(DOTR_BUN_MSG == 0x888 && DOTR_BUN_COUNT == 1 && DOTR_BUN_MODE == 0);
    setup(0, 11, 0xd4, 0);
    run();
    assert(DOTR_BUN_MSG == 0x8e9 && DOTR_BUN_COUNT == 1);
    setup(0, 11, 0xd5, 1);
    run();
    assert(DOTR_BUN_MSG == 0x903 && DOTR_BUN_COUNT == 2);
    setup(0, 11, 0xd6, 1);
    run();
    assert(DOTR_BUN_MSG == 0x7777 && DOTR_BUN_MODE == 0);
    setup(1, 21, 0x12b, 1);
    run();
    assert(DOTR_BUN_MSG == 0x964 && DOTR_BUN_COUNT == 3);

    setup(0, 3, 0x12c, 1);
    run();
    assert(DOTR_BUN_MSG == 0x8a7 && DOTR_BUN_COUNT == 4 && DOTR_BUN_MODE == 0);
    setup(0, 11, 0x139, 0);
    run();
    assert(DOTR_BUN_MSG == 0x905 && DOTR_BUN_COUNT == 1);
    setup(1, 0, 0x18f, 2);
    run();
    assert(DOTR_BUN_MSG == 0x96e && DOTR_BUN_COUNT == 3);
}

static void check_events(void)
{
    /* 0x2724: page 10 becomes 11; every panel flag and look byte cleared. */
    setup(0, 0, 0x2724, 10);
    run();
    assert(DOTR_BUN_PAGE == 11 && DOTR_BUN_MSG == 0x7f7 && DOTR_BUN_COUNT == 15 && DOTR_BUN_MODE == 1);
    assert(DOTR_BUN_FLAG0 == 0 && DOTR_BUN_FLAG1 == 0 && DOTR_BUN_LOOK0 == 0 && DOTR_BUN_LOOK1 == 0);
    setup(0, 0, 0x2724, 9);
    run();
    assert(DOTR_BUN_PAGE == 9 && DOTR_BUN_MSG == 0x7f5);

    setup(1, 0, 0x2738, 1);
    run();
    assert(DOTR_BUN_MSG == 0x7fc && DOTR_BUN_COUNT == 8 && DOTR_BUN_MODE == 2);
    assert(DOTR_BUN_NAME1 == 0xc4 && DOTR_BUN_SIDE1 == 0 && DOTR_BUN_FLAG0 == 0 && DOTR_BUN_FLAG1 == 1);

    /* 0x2760: the left panel for listed messages (shown from page 4), else the right one. */
    setup(0, 0, 0x2760, 1);
    run();
    assert(DOTR_BUN_MSG == 0x825 && DOTR_BUN_COUNT == 0x3e && DOTR_BUN_MODE == 3);
    assert(DOTR_BUN_NAME0 == 0xc5 && DOTR_BUN_SIDE0 == 1 && DOTR_BUN_FLAG0 == 0x55 && DOTR_BUN_LOOK0 == 0);
    setup(0, 0, 0x2760, 4);
    run();
    assert(DOTR_BUN_MSG == 0x828 && DOTR_BUN_MODE == 3 && DOTR_BUN_FLAG0 == 1);
    setup(0, 0, 0x2760, 3);
    run();
    assert(DOTR_BUN_MSG == 0x827 && DOTR_BUN_MODE == 2 && DOTR_BUN_NAME1 == 0xc4 && DOTR_BUN_FLAG1 == 1);

    setup(0, 0, 0x276a, 2);
    run();
    assert(DOTR_BUN_MSG == 0x864 && DOTR_BUN_MODE == 3 && DOTR_BUN_FLAG0 == 1);
    setup(0, 0, 0x276a, 3);
    run();
    assert(DOTR_BUN_MODE == 2 && DOTR_BUN_FLAG1 == 1 && DOTR_BUN_FLAG0 == 0x55);

    setup(0, 0, 0x2b0c, 4);
    run();
    assert(DOTR_BUN_MSG == 0x870 && DOTR_BUN_COUNT == 0x18 && DOTR_BUN_MODE == 3 && DOTR_BUN_NAME0 == 0xd7);
    setup(0, 0, 0x2b84, 5);
    run();
    assert(DOTR_BUN_MSG == 0x8bb && DOTR_BUN_MODE == 3 && DOTR_BUN_FLAG0 == 0x55);
    setup(0, 0, 0x2b84, 7);
    run();
    assert(DOTR_BUN_MSG == 0x8bd && DOTR_BUN_FLAG0 == 1);

    /* 0x2b8c: from page 10 the right panel's flag is cleared again. */
    setup(0, 0, 0x2b8c, 4);
    run();
    assert(DOTR_BUN_MSG == 0x8d9 && DOTR_BUN_MODE == 3 && DOTR_BUN_FLAG0 == 1);
    setup(0, 0, 0x2b8c, 10);
    run();
    assert(DOTR_BUN_MSG == 0x8df && DOTR_BUN_MODE == 2 && DOTR_BUN_NAME1 == 0xd7 && DOTR_BUN_FLAG1 == 0);

    setup(0, 0, 0x2b98, 3);
    run();
    assert(DOTR_BUN_MSG == 0x8ed && DOTR_BUN_LOOK1 == 1);
    setup(0, 0, 0x2b98, 1);
    run();
    assert(DOTR_BUN_LOOK1 == 2);
    setup(0, 0, 0x2b98, 4);
    run();
    assert(DOTR_BUN_LOOK1 == 0x55);

    setup(0, 0, 0x2bac, 1);
    run();
    assert(DOTR_BUN_MSG == 0x906 && DOTR_BUN_MODE == 3 && DOTR_BUN_LOOK0 == 2 && DOTR_BUN_LOOK1 == 2);
    setup(0, 0, 0x2bac, 2);
    run();
    assert(DOTR_BUN_MODE == 2 && DOTR_BUN_LOOK0 == 0x55);

    setup(0, 0, 0x2ee0, 4);
    run();
    assert(DOTR_BUN_MSG == 0x921 && DOTR_BUN_MODE == 2 && DOTR_BUN_FLAG0 == 1 && DOTR_BUN_LOOK1 == 2);
    setup(0, 0, 0x2ee0, 5);
    run();
    assert(DOTR_BUN_MODE == 3 && DOTR_BUN_FLAG1 == 0 && DOTR_BUN_LOOK1 == 2);
    setup(0, 0, 0x2ee0, 3);
    run();
    assert(DOTR_BUN_MODE == 3 && DOTR_BUN_FLAG1 == 1 && DOTR_BUN_LOOK0 == 0 && DOTR_BUN_LOOK1 == 0);

    setup(0, 0, 0x2f64, 1);
    run();
    assert(DOTR_BUN_MSG == 0x974 && DOTR_BUN_MODE == 3 && DOTR_BUN_NAME0 == 0xd8 && DOTR_BUN_SIDE0 == 0);
    setup(0, 0, 0x2f64, 7);
    run();
    assert(DOTR_BUN_MSG == 0x97a && DOTR_BUN_MODE == 2 && DOTR_BUN_LOOK1 == 2);

    setup(0, 0, 0x2f76, 1);
    run();
    assert(DOTR_BUN_MSG == 0x989 && DOTR_BUN_LOOK0 == 2 && DOTR_BUN_LOOK1 == 2);
    setup(0, 0, 0x2f7c, 0);
    run();
    assert(DOTR_BUN_MSG == 0x9a1 && DOTR_BUN_COUNT == 0xe && DOTR_BUN_MODE == 1);

    /* An unlisted counter value changes nothing. */
    setup(0, 0, 0x3000, 0);
    run();
    assert(DOTR_BUN_MSG == 0x7777 && DOTR_BUN_COUNT == 0x55 && DOTR_BUN_MODE == 0x55);
}

int main(void)
{
    check_early();
    check_middle();
    check_events();
    return 0;
}
