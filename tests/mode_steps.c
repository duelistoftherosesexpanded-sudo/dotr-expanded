/* Synthetic host tests for the mode_steps group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/mode_steps.h"

struct DotrMiscLink data_0034fb00;

/* Stubs: each counts its call, checks or records its argument and returns step_result;
 * 0x23c170 returns reset_result. */
static int calls[10];
static int reset_arg;
static int step_result;
static int reset_result;
static int step_mode;
int TakuCard_inti(int a) { reset_arg = a; calls[0]++; return reset_result; }
int TakuSave_card_file_kakikomi(void) { calls[8]++; return step_result; }
int TakuSave_card_unformat(void) { calls[9]++; return step_result; }
int TakuTrade_Save_card_kansi(int mode) { step_mode = mode; calls[1]++; return step_result; }
int TakuSave_card_filename_check(int a) { assert(a == 0); calls[2]++; return step_result; }
int TakuSave_card_file_open(void) { calls[3]++; return step_result; }
int TakuSave_card_kansi(int a) { assert(a == 0); calls[4]++; return step_result; }
int TakuSave_make_file(void) { calls[5]++; return step_result; }
int TakuTrade_Save_card_file_kakikomi_Ver2(void) { calls[6]++; return step_result; }
int TakuSave_card_format(void) { calls[7]++; return step_result; }

static struct DotrMiscLink *link = &data_0034fb00;
static short *held(void) { return (short *)link->unknown_58; }
static void setup(int step, int result)
{
    memset(link, 0, sizeof(*link));
    memset(calls, 0, sizeof(calls));
    link->unknown_30 = (short)step;
    link->unknown_34 = 0x11;
    link->unknown_40 = 0x22;
    link->unknown_2be = 0x33;
    step_result = result;
    reset_result = 0;
    step_mode = -1;
}
static int only(int index)
{
    int i;

    for (i = 0; i < 10; i++) {
        if (calls[i] != (i == index)) {
            return 0;
        }
    }
    return 1;
}

/* Branch coverage: both outcomes of the mode-1 reset, the held test, every table entry
 * (0-14) and the bounds 15 and -1, each inner condition both ways, and the result
 * handling (0, non-zero, 256, word +8 equal to 1 or not). */
static void check_steps(void)
{
    static const int plain[] = { 1, 2, 3, 4, 5, 10 };
    static const int plain_call[] = { 2, 3, 4, 5, 6, 7 };
    static const int empty[] = { 6, 7, 8, 9, 12, 15, -1 };
    size_t i;

    /* Mode 1, reset refused: held flag set, 0x600, nothing else touched. */
    setup(0, 5);
    reset_result = 1;
    assert(TkMc_Trade_Save_Main(1) == 0x600 && only(0) && *held() == 1 && reset_arg == 1);
    assert(link->unknown_2be == 0x33 && link->unknown_34 == 0x11);
    /* Held flag already set: 0x600 without a step call, in mode 0 and after a reset. */
    setup(0, 5);
    *held() = 1;
    assert(TkMc_Trade_Save_Main(0) == 0x600 && only(-1) && link->unknown_2be == 0x33);
    setup(0, 5);
    *held() = 1;
    assert(TkMc_Trade_Save_Main(1) == 0x600 && only(0) && link->unknown_2be == 0);
    /* Mode 1, reset accepted: step runs, then the three halfwords are cleared. */
    setup(0, 5);
    link->unknown_30 = 0;
    assert(TkMc_Trade_Save_Main(1) == 5 && calls[0] == 1 && calls[1] == 1 && step_mode == 1);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0 && link->unknown_30 == 0);
    assert(link->unknown_2be == 5);
    setup(3, 0);
    assert(TkMc_Trade_Save_Main(1) == 0 && calls[4] == 1 && link->unknown_30 == 0);

    /* Step 0: result 2 clears the step (already 0, so use the stored result). */
    setup(0, 2);
    assert(TkMc_Trade_Save_Main(0) == 2 && only(1) && step_mode == 0 && link->unknown_30 == 0);
    setup(0, 4);
    assert(TkMc_Trade_Save_Main(3) == 4 && only(1) && step_mode == 3);
    assert(link->unknown_34 == 0x11 && link->unknown_40 == 0x22);
    /* Result 0 keeps +0x2be; word +8 equal to 1 adds 0x80; 256 is returned as is. */
    setup(0, 0);
    assert(TkMc_Trade_Save_Main(0) == 0x33 && link->unknown_2be == 0x33);
    setup(0, 0);
    link->unknown_08 = 1;
    assert(TkMc_Trade_Save_Main(0) == 0xb3 && link->unknown_2be == 0xb3);
    setup(0, 6);
    link->unknown_08 = 2;
    assert(TkMc_Trade_Save_Main(0) == 6);
    setup(0, 256);
    link->unknown_08 = 1;
    assert(TkMc_Trade_Save_Main(0) == 256 && link->unknown_2be == 256);

    for (i = 0; i < sizeof(plain) / sizeof(plain[0]); i++) {
        setup(plain[i], 7);
        assert(TkMc_Trade_Save_Main(2) == 7 && only(plain_call[i]));
        assert(link->unknown_30 == plain[i] && link->unknown_2be == 7);
    }
    for (i = 0; i < sizeof(empty) / sizeof(empty[0]); i++) {
        setup(empty[i], 7);
        assert(TkMc_Trade_Save_Main(2) == 0x33 && only(-1) && link->unknown_30 == empty[i]);
    }

    /* Step 11: +0x2be equal to 14 and mode 2 moves to step 10 with result 11. */
    setup(11, 7);
    link->unknown_2be = 14;
    assert(TkMc_Trade_Save_Main(2) == 11 && only(1) && step_mode == 2);
    assert(link->unknown_30 == 10 && link->unknown_34 == 0 && link->unknown_40 == 0);
    setup(11, 7);
    link->unknown_2be = 14;
    assert(TkMc_Trade_Save_Main(0) == 7 && link->unknown_30 == 11 && link->unknown_34 == 0x11);
    setup(11, 7);
    link->unknown_2be = 13;
    assert(TkMc_Trade_Save_Main(2) == 7 && link->unknown_30 == 11 && link->unknown_40 == 0x22);
    /* Step 13: +0x2be equal to 1 and mode 2 moves to step 5 with result 9. */
    setup(13, 7);
    link->unknown_2be = 1;
    assert(TkMc_Trade_Save_Main(2) == 9 && only(1) && link->unknown_30 == 5);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0);
    setup(13, 7);
    link->unknown_2be = 1;
    assert(TkMc_Trade_Save_Main(3) == 7 && link->unknown_30 == 13);
    setup(13, 7);
    link->unknown_2be = 2;
    assert(TkMc_Trade_Save_Main(2) == 7 && link->unknown_30 == 13 && link->unknown_34 == 0x11);
    /* Step 14: result 2 becomes 10; mode 2 clears and gives 0x700. */
    setup(14, 2);
    assert(TkMc_Trade_Save_Main(0) == 10 && only(1) && link->unknown_30 == 14);
    setup(14, 4);
    assert(TkMc_Trade_Save_Main(0) == 4 && link->unknown_34 == 0x11);
    setup(14, 2);
    assert(TkMc_Trade_Save_Main(2) == 0x700 && link->unknown_30 == 0);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0 && link->unknown_2be == 0x700);
}

/* Branch coverage for 0x23c560: both modes of the reset, every table entry (0-15) and
 * the bounds 16 and -1, each inner condition both ways, result 0 / non-zero / 256, and
 * the word +8 equal to 1 adding no flag. */
static void check_steps_2(void)
{
    static const int plain[] = { 1, 2, 3, 4, 5, 10, 15 };
    static const int plain_call[] = { 2, 3, 4, 5, 8, 7, 9 };
    static const int empty[] = { 6, 7, 8, 9, 12, 16, -1 };
    size_t i;

    setup(0, 5);
    reset_arg = -1;
    assert(TkMc_Save_Main(1) == 5 && calls[0] == 1 && reset_arg == 0 && calls[4] == 1);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0 && link->unknown_30 == 0);
    setup(2, 0);
    assert(TkMc_Save_Main(1) == 0 && calls[0] == 1 && calls[3] == 1 && link->unknown_30 == 0);
    /* A set +0x58 does not hold this routine. */
    setup(0, 2);
    *held() = 1;
    link->unknown_08 = 1;
    assert(TkMc_Save_Main(0) == 2 && only(4) && link->unknown_2be == 2);
    setup(0, 4);
    assert(TkMc_Save_Main(3) == 4 && only(4) && link->unknown_34 == 0x11);
    setup(0, 0);
    link->unknown_08 = 1;
    assert(TkMc_Save_Main(0) == 0x33 && link->unknown_2be == 0x33);
    setup(0, 256);
    assert(TkMc_Save_Main(0) == 256 && link->unknown_2be == 256);

    for (i = 0; i < sizeof(plain) / sizeof(plain[0]); i++) {
        setup(plain[i], 7);
        assert(TkMc_Save_Main(2) == 7 && only(plain_call[i]));
        assert(link->unknown_30 == plain[i] && link->unknown_2be == 7);
    }
    for (i = 0; i < sizeof(empty) / sizeof(empty[0]); i++) {
        setup(empty[i], 7);
        assert(TkMc_Save_Main(2) == 0x33 && only(-1) && link->unknown_30 == empty[i]);
    }

    setup(11, 7);
    link->unknown_2be = 14;
    assert(TkMc_Save_Main(2) == 11 && only(4) && link->unknown_30 == 10);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0);
    setup(11, 7);
    link->unknown_2be = 14;
    assert(TkMc_Save_Main(0) == 7 && link->unknown_30 == 11 && link->unknown_34 == 0x11);
    setup(11, 7);
    link->unknown_2be = 13;
    assert(TkMc_Save_Main(2) == 7 && link->unknown_30 == 11 && link->unknown_40 == 0x22);
    setup(13, 7);
    link->unknown_2be = 1;
    assert(TkMc_Save_Main(2) == 9 && only(4) && link->unknown_30 == 5);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0);
    setup(13, 7);
    link->unknown_2be = 1;
    assert(TkMc_Save_Main(3) == 7 && link->unknown_30 == 13);
    setup(13, 7);
    link->unknown_2be = 2;
    assert(TkMc_Save_Main(2) == 7 && link->unknown_30 == 13 && link->unknown_34 == 0x11);
    setup(14, 2);
    assert(TkMc_Save_Main(0) == 10 && only(4) && link->unknown_30 == 14);
    setup(14, 4);
    assert(TkMc_Save_Main(0) == 4 && link->unknown_34 == 0x11);
    setup(14, 2);
    assert(TkMc_Save_Main(2) == 0x700 && link->unknown_30 == 0);
    assert(link->unknown_34 == 0 && link->unknown_40 == 0 && link->unknown_2be == 0x700);
}

int main(void)
{
    check_steps();
    check_steps_2();
    return 0;
}
