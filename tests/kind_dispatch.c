/* Synthetic host tests for the kind_dispatch group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/kind_dispatch.h"

struct DotrDigitView gSzGame_gl_digits;

/* Every stub appends its address; 0x1c5d70 appends its address and then its argument. */
static long seen[8];
static int seen_count;
static void put(long value) { assert(seen_count < 8); seen[seen_count++] = value; }
void SzDuel_Term(void) { put(0x1ae520); }
void SzBattle_Term(void) { put(0x1e77a0); }
void SzDuel_TermBuildDeck(void) { put(0x1b8830); }
void SzDuel_Term3DView(void) { put(0x1c1270); }
void SzDuel_TermTrade(void) { put(0x1c2530); }
void SzDuel_TermCustomDuel(void) { put(0x1c6120); }
void lSzGame_TermMemCheck(void) { put(0x1ac600); }
void lSzGame_TermTitle(void) { put(0x1ac6f0); }
void TkCamp_Term(void) { put(0x23b920); }
void ShFace_Term(void) { put(0x2464e0); }
void SzEvent_Term(void) { put(0x1e7a50); }
void SzDuel_TermMapEdit(void) { put(0x1cc610); }
void ShSlot_Term(void) { put(0x286900); }
void SzView_TermCamera(void) { put(0x1fca70); }
void SzDuel_InitCustomDuel(int unknown) { put(0x1c5d70); put(unknown); }

static void run(int kind, int mode)
{
    memset(&gSzGame_gl_digits, 0, sizeof(gSzGame_gl_digits));
    ((signed char *)&gSzGame_gl_digits)[0x21f] = (signed char)mode;
    seen_count = 0;
    SzGame_ModeTerm(kind);
}
static void one(int kind, long first)
{
    /* Mode 9 must not add the follow-up call for these kinds. */
    run(kind, 9);
    assert(seen_count == 1 && seen[0] == first);
}
static void two(int kind, long first, long second)
{
    run(kind, 9);
    assert(seen_count == 2 && seen[0] == first && seen[1] == second);
}
static void followed(int kind, long first)
{
    run(kind, 9);
    assert(seen_count == 3 && seen[0] == first && seen[1] == 0x1c5d70 && seen[2] == -1);
    run(kind, 8);
    assert(seen_count == 1 && seen[0] == first);
    run(kind, 10);
    assert(seen_count == 1 && seen[0] == first);
}

/* Branch coverage: every case, both outcomes of the mode test where there is one,
 * and kinds outside the list (below, between and above the cases). */
static void check_dispatch(void)
{
    static const int none[] = { 0, 2, 3, 4, 16, 17, 18, 20, 35, 37, -1 };
    size_t i;

    followed(1, 0x1ae520);
    one(5, 0x1e77a0);
    followed(6, 0x1b8830);
    one(7, 0x1c1270);
    one(8, 0x1c2530);
    one(9, 0x1c6120);
    one(36, 0x1ac600);
    one(10, 0x1ac6f0);
    two(11, 0x23b920, 0x2464e0);
    one(12, 0x1e7a50);
    one(13, 0x1cc610);
    one(14, 0x2464e0);
    followed(15, 0x286900);
    two(19, 0x1e77a0, 0x1fca70);
    for (i = 0; i < sizeof(none) / sizeof(none[0]); i++) {
        run(none[i], 9);
        assert(seen_count == 0);
    }
}

/* 0x1aafa0 stubs: the mode routines append their address and argument; the formatter
 * checks its arguments and writes a marker name; the queue call records its arguments. */
char data_002be7d0[] = "HOME";
char data_002be821[] = "%s/Merge/Icon.Mrg";
static char home_text[] = "home";
static int queued;
int TkMc_Load_Main(int mode) { put(0x23c770); put(mode); return 0; }
int TkMc_Custom_Main(int mode) { put(0x23cc50); put(mode); return 0; }
int TkMc_Trade_Load_Main(int mode) { put(0x23c8b0); put(mode); return 0; }
int TkMc_Trade_Save_Main(int mode) { put(0x23ca10); put(mode); return 0; }
int TkMc_Check_Main(int mode) { put(0x23c3f0); put(mode); return 0; }
char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002be7d0);
    put(0x1fff90);
    return home_text;
}
int func_0019b648(char *destination, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    assert(format == data_002be821);
    assert(va_arg(arguments, char *) == home_text);
    va_end(arguments);
    strcpy(destination, "made");
    put(0x19b648);
    return 4;
}
struct DotrResourceQueueRecord *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010,
                                              DotrResourceSubmitCallback callback, int unknown_01c)
{
    assert(type == 0 && priority == 0 && strcmp(name, "made") == 0);
    assert(unknown_014 == 0x1e301e0 && byte_size == -1 && byte_offset == 0);
    assert(unknown_010 == -1 && callback == 0 && unknown_01c == 0);
    queued++;
    return 0;
}

static struct DotrKindStateView *state_view(void)
{
    return (struct DotrKindStateView *)&gSzGame_gl_digits;
}
static void select_kind(int kind, int fill)
{
    unsigned char *bytes = (unsigned char *)&gSzGame_gl_digits;

    memset(&gSzGame_gl_digits, fill, sizeof(gSzGame_gl_digits));
    seen_count = 0;
    queued = 0;
    SzGame_InitMemCardProc(kind);
    /* Always: short cleared, kind in bits 3-5 of +0x22d, the one bit only for kind 2;
     * the other bits of both bytes and the neighbouring bytes keep the fill. */
    assert(state_view()->word_21c == 0);
    assert(bytes[0x22d] == (unsigned char)((fill & 0xc7) | ((kind & 7) << 3)));
    assert(((bytes[0x22e] >> 3) & 1) == (kind == 2));
    assert((bytes[0x22e] & 0xf0) == (fill & 0xf0));
    assert(bytes[0x21b] == (unsigned char)fill && bytes[0x21e] == (unsigned char)fill);
    assert(bytes[0x22c] == (unsigned char)fill && bytes[0x22f] == (unsigned char)fill);
}
static void mode_kind(int kind, long routine, int state)
{
    int fill;

    for (fill = 0; fill <= 0xff; fill += 0xff) {
        select_kind(kind, fill);
        assert(seen_count == 2 && seen[0] == routine && seen[1] == 1 && queued == 0);
        assert(state_view()->state == state);
    }
}
static void load_kind(int kind)
{
    int fill;

    for (fill = 0; fill <= 0xff; fill += 0xff) {
        select_kind(kind, fill);
        assert(seen_count == 2 && seen[0] == 0x1fff90 && seen[1] == 0x19b648 && queued == 1);
        assert(state_view()->state == 1);
    }
}

/* Branch coverage: each of the seven cases, both sides of the kind == 2 test, and kinds
 * outside the table (-1, 7, 10), which leave the state field alone. */
static void check_select(void)
{
    static const int outside[] = { -1, 7, 10 };
    size_t i;

    mode_kind(0, 0x23c770, 2);
    load_kind(1);
    load_kind(2);
    mode_kind(3, 0x23cc50, 1);
    mode_kind(4, 0x23c8b0, 1);
    mode_kind(5, 0x23ca10, 1);
    mode_kind(6, 0x23c3f0, 1);
    for (i = 0; i < sizeof(outside) / sizeof(outside[0]); i++) {
        select_kind(outside[i], 0xff);
        assert(seen_count == 0 && queued == 0 && state_view()->state == 7);
        select_kind(outside[i], 0);
        assert(seen_count == 0 && queued == 0 && state_view()->state == 0);
    }
}

int main(void)
{
    check_dispatch();
    check_select();
    return 0;
}
