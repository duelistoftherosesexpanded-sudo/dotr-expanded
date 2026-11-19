/* Synthetic host tests for the camp_entry group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics.
 * Every call is checked three ways: the order and arguments of all callee calls (one trace
 * string), the whole work area and the whole camp state against an expected image (so writes
 * outside the expected members fail too), and the four bytes at 0x34f6f8-0x34f710.
 * TkCamp_Init indexes the location records with kind[0] and kind[1] unchecked: the fixtures
 * keep both within the 22 records. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_entry.h"

struct DotrCampEntryWork dotr_camp_entry_work;
struct DotrCampEntryBoard data_00299e10_campentry;
struct DotrCampEntrySpot data_00298ba0_campentry[22];
struct DotrCampEntryQuad data_0029a790_campentry;
struct DotrCampEntryQuad data_0029a7a0_campentry;
signed char Move_END_flg_byte;
signed char data_0034f6f8;
signed char data_0034f700;
signed char data_0034f708;
unsigned short DeckTab_campentry[16];
struct DotrCampEntryRow data_00299d00_campentry[32];

#define W dotr_camp_entry_work
#define BOARD data_00299e10_campentry
#define SPOT data_00298ba0_campentry

/* Expected images: copies taken before the call, to which each check applies the expected
 * writes. */
static struct DotrCampEntryWork EW;
static struct DotrCampEntryBoard EB;
static signed char e710, e6f8, e700, e708;

static char trace[1024];
static size_t used;
static int loading, leaders;
static struct DotrCampEntryVec cam_from, cam_to;

static void say(const char *format, int a, int b, int c, int d)
{
    int n = snprintf(trace + used, sizeof trace - used, format, a, b, c, d);

    assert(n > 0 && (size_t)n < sizeof trace - used);
    used += (size_t)n;
}

void Taku_Camp_Data_Init(void)
{
    say("data;", 0, 0, 0, 0);
}
void Taku_spline_init(void)
{
    say("spline;", 0, 0, 0, 0);
}
void Taku_Camp_3D_Poly_Init(void)
{
    say("poly;", 0, 0, 0, 0);
}
void Taku_Screen_Init(void)
{
    /* the byte at 0x1ed018c has been decided before the screen is set up */
    assert(W.all == EW.all || W.all == (BOARD.done0 != 0 && BOARD.done1 != 0));
    say("screen;", 0, 0, 0, 0);
}
void func_00233f50_campentry(int mode)
{
    say("cin%d;", mode, 0, 0, 0);
}
int SzModel_SetDrawFlag(int index, int value)
{
    say("hide%d,%d;", index, value, 0, 0);
    return 0;
}
int SzGame_SetMenuNum(int index, int value)
{
    say("menu%d,%d;", index, value, 0, 0);
    return 0;
}
/* Stub: records the call only. It does not clear the block, so the zeros asserted after mode 0
 * are the function's own stores. */
void func_00195108(void *a, int b)
{
    assert(a == (void *)&BOARD);
    say("clr%d;", b, 0, 0, 0);
}
int SzModel_LoadMerge(int type, int no, int force)
{
    say("pack%d,%d,%d;", type, no, force, 0);
    return 0;
}
int SzText_LoadMerge(int a, int b)
{
    say("text%d,%d;", a, b, 0, 0);
    return 0;
}
void func_001f3910_campentry(int a, int b, int c)
{
    say("mod%d,%d,%d;", a, b, c, 0);
}
void func_00217980_campentry(int n, int b)
{
    say("bgm%d,%d;", n, b, 0, 0);
}
int SzModel_IsModelLoading(int index)
{
    say("ldg%d;", index, 0, 0, 0);
    return loading;
}
int SzData_GetCurrentDeck(int side)
{
    say("deck%d;", side, 0, 0, 0);
    return 5;
}
/* Stub: the first answer is 321 and the second 654, to tell the two calls' uses apart. */
int SzData_GetLeader(int side, int deck)
{
    say("lead%d,%d;", side, deck, 0, 0);
    return leaders++ == 0 ? 321 : 654;
}
int func_001f22b0_campentry(int index, struct DotrCampEntryQuad *in)
{
    assert(in != &data_0029a790_campentry && memcmp(in, &data_0029a790_campentry, sizeof *in) == 0);
    say("pos%d;", index, 0, 0, 0);
    return 0;
}
int func_001f2430_campentry(int index, struct DotrCampEntryQuad *in)
{
    assert(in != &data_0029a7a0_campentry && memcmp(in, &data_0029a7a0_campentry, sizeof *in) == 0);
    say("rot%d;", index, 0, 0, 0);
    return 0;
}
int SzDuel_New2Xax(int value)
{
    say("x%d;", value, 0, 0, 0);
    return value + 1000;
}
int SzModel_Load(int a, int b, int c, int d)
{
    say("load%d,%d,%d,%d;", a, b, c, d);
    return 0;
}
int func_001f93f0_campentry(int a, int b, int c, int d)
{
    say("light%d,%d,%d,%d;", a, b, c, d);
    return 0;
}
void SzEffect_SetIntensityOnly(int kind, float value)
{
    assert(value == 1.0f);
    say("int%d;", kind, 0, 0, 0);
}
/* Stub: the two vectors must be the work area's own, already holding their final values. */
int func_001f9fe0_campentry(int index, struct DotrCampEntryVec *from, struct DotrCampEntryVec *to)
{
    assert(from == &W.from && to == &W.to);
    cam_from = *from;
    cam_to = *to;
    say("cam%d;", index, 0, 0, 0);
    return 0;
}

#define COMMON "data;spline;poly;cin3;hide30,0;hide31,0;screen;menu11,1;"
#define MODS "mod0,400,0;mod0,401,0;mod0,402,0;mod0,403,0;mod0,404,0;"
#define LEAD_BUSY "ldg30;"
#define LEAD_LOAD "ldg30;deck0;lead0,5;lead0,5;pos30;rot30;x321;load30,-1,1321,1;"

/* Fills every object with values the function must either leave or overwrite. */
static void prep(int side, int done0, int done1, int started, int won)
{
    int i, j;

    memset(&W, 0, sizeof W);
    memset(&BOARD, 0, sizeof BOARD);
    memset(SPOT, 0, sizeof SPOT);
    W.from.x = 1.0f;
    W.from.y = 2.0f;
    W.from.z = 3.0f;
    W.from.w = 4.0f;
    W.to.x = 5.0f;
    W.to.y = 6.0f;
    W.to.z = 7.0f;
    W.to.w = 5.5f;
    W.all = 9;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            W.records[i][j].count = 6;
            W.records[i][j].state = 5;
        }
    }
    for (i = 0; i < 32; i++) {
        W.rise[i] = 7;
        W.count[i] = 8;
    }
    W.byte_53a8 = 6;
    W.byte_53b3 = 7;
    W.byte_53c7 = 9;
    W.hash = 9;
    for (i = 0; i < 3; i++) {
        W.pick[i] = (unsigned short)(40 + i);
        W.deck[i] = (unsigned short)(50 + i);
    }
    W.leader = -3;
    BOARD.side = (signed char)side;
    BOARD.done0 = (signed char)done0;
    BOARD.done1 = (signed char)done1;
    for (i = 0; i < 57; i++) {
        BOARD.seen[i] = 9;
    }
    BOARD.started = (short)started;
    BOARD.ticks = 77;
    BOARD.unknown_c4 = 0x1234;
    BOARD.kind[0] = 2;
    BOARD.kind[1] = 7;
    BOARD.kind[2] = 9;
    BOARD.byte_c9 = 8;
    memset(BOARD.unknown_ca, 0x5a, sizeof BOARD.unknown_ca);
    SPOT[2].x = 10.0f;
    SPOT[2].y = 11.0f;
    SPOT[2].z = 20.0f;
    SPOT[7].x = 30.0f;
    SPOT[7].y = 99.0f;
    SPOT[7].z = 40.0f;
    for (i = 0; i < 4; i++) {
        data_0029a790_campentry.f[i] = (float)(i + 1);
        data_0029a7a0_campentry.f[i] = (float)(i + 5);
    }
    Move_END_flg_byte = 1;
    data_0034f6f8 = 1;
    data_0034f700 = 1;
    data_0034f708 = (signed char)won;
    memset(DeckTab_campentry, 0, sizeof DeckTab_campentry);
    memset(data_00299d00_campentry, 0, sizeof data_00299d00_campentry);
    /* entry 0: the numbers 3, 17 and 31 with the unused top bit set; entry 9 (the stale hash)
     * would give 1, 1, 1 */
    DeckTab_campentry[0] = 0x8000 | 3 | 17 << 5 | 31 << 10;
    DeckTab_campentry[9] = 1 | 1 << 5 | 1 << 10;
    data_00299d00_campentry[1].id = 0x222;
    data_00299d00_campentry[3].id = 0xfc00 | 0x155;
    data_00299d00_campentry[17].id = 0x3ff;
    data_00299d00_campentry[31].id = 0x400;
    loading = 0;
}

/* Calls the function and sets the expectations every mode shares. */
static void call(int mode)
{
    memcpy(&EW, &W, sizeof EW);
    memcpy(&EB, &BOARD, sizeof EB);
    e710 = 0;
    e6f8 = data_0034f6f8;
    e700 = data_0034f700;
    e708 = data_0034f708;
    EW.all = BOARD.done0 != 0 && BOARD.done1 != 0;
    EW.byte_53c7 = 2;
    trace[0] = 0;
    used = 0;
    leaders = 0;
    memset(&cam_from, 0, sizeof cam_from);
    memset(&cam_to, 0, sizeof cam_to);
    TkCamp_Init(mode);
}

static void same(const char *want)
{
    if (strcmp(trace, want) != 0) {
        printf("trace: %s\nwant:  %s\n", trace, want);
    }
    assert(strcmp(trace, want) == 0);
    assert(memcmp(&EW, &W, sizeof W) == 0);
    assert(memcmp(&EB, &BOARD, sizeof BOARD) == 0);
    assert(Move_END_flg_byte == e710 && data_0034f6f8 == e6f8 && data_0034f700 == e700 && data_0034f708 == e708);
}

/* The reset most modes share. */
static void expect_reset(int side, int ticks)
{
    int i;

    EB.side = (signed char)side;
    EB.done0 = 0;
    EB.done1 = 0;
    EW.all = 0;
    for (i = 0; i < 32; i++) {
        EB.seen[i] = 0;
    }
    EB.ticks = (short)ticks;
}

static void check_plain_modes(void)
{
    static const int other[] = { -1, 8, 9, 11, 19, 21, 31, 41, 51, 59, 61, 100 };
    static const struct {
        int mode, no, ticks, cinema, bgm;
    } simple[] = {
        { 3, 4, 10040, 1, 24 }, { 20, 0, 11182, 1, 34 }, { 30, 0, 12154, 1, 38 },
        { 40, 4, 11000, 0, 24 }, { 50, 4, 12000, 0, 24 },
    };
    char want[1024];
    size_t i;
    int both;

    /* A mode that is not listed returns after the common part. With both done bytes set the
     * byte at 0x1ed018c becomes 1, otherwise 0. */
    for (i = 0; i < sizeof other / sizeof other[0]; i++) {
        for (both = 0; both < 4; both++) {
            prep(4, both & 1 ? 5 : 0, both & 2 ? 7 : 0, 2, 1);
            call(other[i]);
            assert(W.all == (both == 3));
            same(COMMON);
        }
    }

    /* Mode 0, from the "all done" state: everything is reset. */
    prep(4, 5, 7, 2, 1);
    call(0);
    expect_reset(0, 10020);
    EB.started = 1;
    EW.byte_53b3 = 1;
    e6f8 = 0;
    e700 = 0;
    same(COMMON "clr332;pack3,4,0;text10,0;" MODS "cin3;bgm24,0;");
    assert(BOARD.seen[31] == 0 && BOARD.seen[32] == 9 && data_0034f708 == 1);

    /* Modes 3, 20, 30 (started and cinema 2) and 40, 50 (neither); 50 sets the side byte. */
    for (i = 0; i < sizeof simple / sizeof simple[0]; i++) {
        prep(4, 5, 7, 2, 1);
        call(simple[i].mode);
        expect_reset(simple[i].mode == 50, simple[i].ticks);
        if (simple[i].cinema) {
            EB.started = 1;
        }
        snprintf(want, sizeof want, COMMON "pack3,%d,0;" MODS "%sbgm%d,1;", simple[i].no,
                 simple[i].cinema ? "cin2;" : "", simple[i].bgm);
        same(want);
        assert(BOARD.started == (simple[i].cinema ? 1 : 2));
    }

    /* Modes 60 and 10: the hash is cleared first, so table entry 0 is the one split up. */
    for (i = 0; i < 2; i++) {
        prep(4, 0, 7, 2, 1);
        call(i == 0 ? 60 : 10);
        expect_reset(0, i == 0 ? 10050 : 10070);
        EB.started = 1;
        EW.hash = 0;
        EW.pick[0] = 3;
        EW.pick[1] = 17;
        EW.pick[2] = 31;
        EW.deck[0] = 0x155;
        EW.deck[1] = 0x3ff;
        EW.deck[2] = 0;
        same(COMMON "pack3,4,0;" MODS "cin2;bgm25,1;");
    }
}

static const int entry_modes[] = { 1, 2, 4, 5, 6, 7 };
#define ENTRY_MODES (sizeof entry_modes / sizeof entry_modes[0])

/* Builds the expected trace of an entry mode: common part, leader part, the rest. */
static const char *entry_trace(const char *rest)
{
    static char want[1024];

    snprintf(want, sizeof want, COMMON "%s%s", loading ? LEAD_BUSY : LEAD_LOAD, rest);
    if (!loading) {
        EW.leader = 654;
    }
    return want;
}

/* Byte at 0x1ed018c set (both done bytes): the all-clear state. The modules are not loaded
 * and the "just won" byte is neither read nor cleared. */
static void check_all_clear(void)
{
    size_t m;
    int i, j;

    for (m = 0; m < ENTRY_MODES; m++) {
        for (loading = 0; loading < 2; loading++) {
            int keep = loading;

            prep(4, 5, 7, 2, 1);
            loading = keep;
            call(entry_modes[m]);
            EB.done0 = 1;
            EB.done1 = 1;
            EW.all = 1;
            EB.ticks = 0;
            for (i = 0; i < 32; i++) {
                EW.rise[i] = 0;
                EW.count[i] = 0;
                EB.seen[i] = 3;
            }
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 16; j++) {
                    EW.records[i][j].state = 3;
                }
            }
            EW.byte_53b3 = 0;
            same(entry_trace("pack3,2,0;light0,7,0,0;int7;cin0;bgm4,0;"));
            assert(BOARD.seen[32] == 9 && BOARD.started == 2 && data_0034f708 == 1 && BOARD.side == 4);
            assert(W.leader == (keep ? -3 : 654));
        }
    }
}

/* Byte at 0x34f708 set: it wins over a non-zero `started`. */
static void check_just_won(void)
{
    size_t m;
    int which, i;

    for (m = 0; m < ENTRY_MODES; m++) {
        for (which = 0; which < 3; which++) {
            prep(4, which == 0 ? 5 : 0, which == 1 ? 7 : 0, 2, 1);
            loading = (int)(m + (size_t)which) & 1;
            call(entry_modes[m]);
            if (which == 0) {
                EB.side = 1;
                EB.done0 = 1;
                EB.done1 = 0;
            } else if (which == 1) {
                EB.side = 0;
                EB.done0 = 0;
                EB.done1 = 1;
            }
            for (i = 0; i < 32; i++) {
                EB.seen[i] = 0;
            }
            EB.ticks = 10060;
            EB.started = 1;
            EW.byte_53b3 = 3;
            e708 = 0;
            same(entry_trace("pack3,4,0;text10,0;cin3;bgm25,1;" MODS));
            assert(W.all == 0 && W.byte_53a8 == 6 && BOARD.byte_c9 == 8);
            /* neither done byte: the three bytes are left as they were */
            assert(which != 2 || (BOARD.side == 4 && BOARD.done0 == 0 && BOARD.done1 == 0));
        }
    }
}

/* Sets the expectations the two "normal return" variants share: the camera aims at location
 * kind[1] = 7 (the values of location kind[0] = 2 are overwritten). */
static void expect_return(void)
{
    EB.started = 1;
    EW.byte_53b3 = 0;
    EW.to.x = 30.0f;
    EW.to.y = 0.0f;
    EW.to.z = 40.0f;
    EW.from.x = 30.0f;
    EW.from.y = 96.0f;
    EW.from.z = 136.0f;
    EW.from.w = 5.5f;
}
#define RETURN_TRACE "pack3,2,0;light0,7,0,0;int7;cin0;cam0;bgm4,0;" MODS "bgm26,0;"

static void set_list(int spot, int n, const short *list)
{
    int i;

    SPOT[spot].n = (short)n;
    for (i = 0; i < n; i++) {
        SPOT[spot].list[i] = list[i];
    }
}

/* `started` not 0 with side 0: routes of cleared locations 1-11, and the count over 2-10. */
static void check_return_side0(void)
{
    static const short l0[] = { 10 }, l1[] = { 0, 5 }, l5[] = { 9 }, l11[] = { 15 }, l12[] = { 14 };
    static const short l10[] = { 1, 2, 3, 4, 6, 7, 8, 9 };
    size_t m;
    int variant, i;

    for (m = 0; m < ENTRY_MODES; m++) {
        for (variant = 0; variant < 4; variant++) {
            /* one done byte set does not matter here; started is any non-zero value */
            prep(0, variant == 1 ? 5 : 0, variant == 2 ? 7 : 0, variant == 0 ? -1 : 2, 0);
            loading = (int)(m + (size_t)variant) & 1;
            set_list(0, 1, l0);
            set_list(1, 2, l1);
            set_list(5, 1, l5);
            set_list(11, 1, l11);
            set_list(12, 1, l12);
            set_list(10, 8, l10);
            for (i = 2; i <= 10; i++) {
                BOARD.seen[i] = 0;      /* prep() leaves 9, which would count */
            }
            BOARD.seen[0] = 3;      /* overwritten with 1 before the loop: its list is not used */
            BOARD.seen[1] = 3;
            BOARD.seen[4] = 3;      /* an empty list */
            BOARD.seen[5] = 2;      /* not cleared: its list is not used */
            BOARD.seen[11] = 3;     /* the last location of the list loop, not counted */
            BOARD.seen[12] = 3;     /* beyond the list loop */
            if (variant >= 1) {
                for (i = 2; i <= 8; i++) {
                    BOARD.seen[i] = 4;      /* counted (>= 3) but no list (not 3): seven */
                }
                BOARD.seen[9] = 2;
                BOARD.seen[10] = -1;
            }
            if (variant == 2) {
                BOARD.seen[9] = 127;        /* the eighth */
            }
            if (variant == 3) {
                BOARD.seen[10] = 3;         /* the eighth, with a full list of eight */
            }
            call(entry_modes[m]);
            EB.seen[0] = 1;
            EW.records[0][0].state = 3;
            EW.records[0][5].state = 3;
            EW.records[0][15].state = 3;
            if (variant == 3) {
                for (i = 0; i < 8; i++) {
                    EW.records[0][l10[i]].state = 3;
                }
            }
            if (variant >= 2) {
                EW.records[0][11].state = 3;
                EW.records[0][12].state = 3;
                EW.records[0][13].state = 3;
            }
            expect_return();
            same(entry_trace(RETURN_TRACE));
            assert(W.records[0][10].state == 5 && W.records[0][14].state == 5 && W.records[1][0].state == 5);
            assert(W.records[0][9].state == (variant == 3 ? 3 : 5));
            assert(W.records[0][12].state == (variant >= 2 ? 3 : 5));
            /* the camera callee saw the final vectors */
            assert(cam_to.x == 30.0f && cam_to.y == 0.0f && cam_to.z == 40.0f && cam_to.w == 5.5f);
            assert(cam_from.x == 30.0f && cam_from.y == 96.0f && cam_from.z == 136.0f && cam_from.w == 5.5f);
        }
    }
}

/* `started` not 0 with side not 0: row 1, locations 12-21 and location 0. */
static void check_return_side1(void)
{
    static const short l12[] = { 14 };
    size_t m;
    int variant;

    for (m = 0; m < ENTRY_MODES; m++) {
        for (variant = 0; variant < 2; variant++) {
            prep(variant == 0 ? 1 : -1, 0, variant == 0 ? 7 : 0, 1, 0);
            loading = (int)(m + (size_t)variant) & 1;
            set_list(12, 1, l12);   /* the lists play no part on this side */
            BOARD.seen[0] = variant == 0 ? 3 : 1;
            BOARD.seen[11] = 3;     /* before the range */
            BOARD.seen[12] = 3;
            BOARD.seen[15] = 4;     /* not 3 */
            BOARD.seen[21] = 3;
            BOARD.seen[22] = 3;     /* after the range */
            call(entry_modes[m]);
            EW.records[1][0].state = 3;
            EW.records[1][1].state = 3;
            EW.records[1][10].state = 3;
            if (variant == 0) {
                EW.records[1][11].state = 3;
            }
            expect_return();
            same(entry_trace(RETURN_TRACE));
            assert(BOARD.seen[0] == (variant == 0 ? 3 : 1) && W.records[1][4].state == 5);
            assert(W.records[0][14].state == 5 && W.records[1][11].state == (variant == 0 ? 3 : 5));
        }
    }
}

/* `started` 0 and nothing else pending: the fresh start, by which done byte is set. */
static void check_fresh(void)
{
    size_t m;
    int which, i;

    for (m = 0; m < ENTRY_MODES; m++) {
        for (which = 0; which < 3; which++) {
            prep(4, which == 0 ? 5 : 0, which == 1 ? 7 : 0, 0, 0);
            loading = (int)(m + (size_t)which) & 1;
            call(entry_modes[m]);
            if (which == 0) {
                EB.side = 1;
                EB.done0 = 1;
                EB.done1 = 0;
                EW.byte_53a8 = 0;
                EB.byte_c9 = 0;
            } else if (which == 1) {
                EB.side = 0;
                EB.done0 = 0;
                EB.done1 = 1;
                EW.byte_53a8 = 1;
                EB.byte_c9 = 2;
            }
            for (i = 0; i < 32; i++) {
                EB.seen[i] = 0;
            }
            EB.ticks = 10090;
            EB.started = 1;
            EW.byte_53b3 = 3;
            same(entry_trace("pack3,4,0;text10,0;cin3;" MODS "bgm26,0;"));
            assert(W.all == 0);
            assert(which != 2 || (BOARD.side == 4 && W.byte_53a8 == 6 && BOARD.byte_c9 == 8));
        }
    }
}

int main(void)
{
    assert(sizeof(struct DotrCampEntryRecord) == 516 && offsetof(struct DotrCampEntryRecord, state) == 0x202);
    assert(offsetof(struct DotrCampEntryWork, from) == 0x30 && offsetof(struct DotrCampEntryWork, to) == 0x40);
    assert(offsetof(struct DotrCampEntryWork, all) == 0x18c && offsetof(struct DotrCampEntryWork, records) == 0x22c);
    assert(offsetof(struct DotrCampEntryWork, rise) == 0x52ec && offsetof(struct DotrCampEntryWork, count) == 0x532c);
    assert(offsetof(struct DotrCampEntryWork, byte_53a8) == 0x53a8 && offsetof(struct DotrCampEntryWork, byte_53b3) == 0x53b3);
    assert(offsetof(struct DotrCampEntryWork, byte_53c7) == 0x53c7 && offsetof(struct DotrCampEntryWork, hash) == 0x53f6);
    assert(offsetof(struct DotrCampEntryWork, pick) == 0x53f8 && offsetof(struct DotrCampEntryWork, deck) == 0x53fe);
    assert(offsetof(struct DotrCampEntryWork, leader) == 0x5404);
    assert(sizeof(struct DotrCampEntryBoard) == 332 && offsetof(struct DotrCampEntryBoard, seen) == 3);
    assert(offsetof(struct DotrCampEntryBoard, started) == 60 && offsetof(struct DotrCampEntryBoard, ticks) == 194);
    assert(offsetof(struct DotrCampEntryBoard, kind) == 198 && offsetof(struct DotrCampEntryBoard, byte_c9) == 201);
    assert(sizeof(struct DotrCampEntrySpot) == 52 && offsetof(struct DotrCampEntrySpot, z) == 8);
    assert(offsetof(struct DotrCampEntrySpot, list) == 20 && offsetof(struct DotrCampEntrySpot, n) == 36);
    assert(sizeof(struct DotrCampEntryQuad) == 16 && sizeof(struct DotrCampEntryRow) == 12);
    check_plain_modes();
    check_all_clear();
    check_just_won();
    check_return_side0();
    check_return_side1();
    check_fresh();
    puts("camp_entry: ok");
    return 0;
}
