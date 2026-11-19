/* Synthetic host tests for the memcard_proc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/memcard_proc.h"

struct DotrMemCardProcView gSzGame_gl_mcview;
#define G gSzGame_gl_mcview
#define BYTES ((unsigned char *)&gSzGame_gl_mcview)

/* Answers of the callees. */
static int busy, mc_result, text_out, msg_class, pad[2];
/* What was called, in order: Q file request, a card routine's letter and its mode (L load,
 * S save, C custom, T trade load, U trade save, K check), G message, P and the sound
 * number, X popup closed, O and the popup operation. The pad is counted, not traced. */
static char trace[64];
static int trace_n, pad_asks[2], open_value;

static void note(char c, int digit)
{
    assert(trace_n + 3 < (int)sizeof trace);
    trace[trace_n++] = c;
    if (digit >= 0) {
        assert(digit < 10);
        trace[trace_n++] = (char)('0' + digit);
    }
    trace[trace_n] = 0;
}

static int routine(char c, int mode)
{
    assert(mode >= 0 && mode <= 2);
    note(c, mode);
    return mc_result;
}

int SzFile_InqRequestEx(void) { note('Q', -1); return busy; }
int TkMc_Load_Main(int mode) { return routine('L', mode); }
int TkMc_Save_Main(int mode) { return routine('S', mode); }
int TkMc_Custom_Main(int mode) { return routine('C', mode); }
int TkMc_Trade_Load_Main(int mode) { return routine('T', mode); }
int TkMc_Trade_Save_Main(int mode) { return routine('U', mode); }
int TkMc_Check_Main(int mode) { return routine('K', mode); }
int lSzGame_GetMCardMsg(int kind, int flags, int *out)
{
    /* Given the dialogue kind, the stored (16-bit) result and the block's text word. */
    assert(kind == (int)G.kind && flags == (mc_result & 0xffff) && G.res == flags && out == &G.msg);
    note('G', -1);
    *out = text_out;
    return msg_class;
}
int SzPad_Check(int p, int kind, int mask)
{
    assert((p == 0 || p == 1) && kind == 0);
    pad_asks[p]++;
    return pad[p] & mask;
}
int SzSound_PlaySE(int a, int b)
{
    assert(a == 0);
    note('P', b);
    return 0;
}
void SzDuel_CloseMsgPopup(void) { note('X', -1); }
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value)
{
    assert(a == 0 && b == 0);
    note('O', operation);
    open_value = value;
}

/* One frame with the three fields set; the text word is left as it is. */
static int go(int kind, int state, int second)
{
    G.kind = (unsigned char)kind;
    G.state = (unsigned char)state;
    G.second = (unsigned char)second;
    trace_n = 0;
    trace[0] = 0;
    pad_asks[0] = pad_asks[1] = 0;
    open_value = 0x55555555;
    return SzGame_ActMemCardProc();
}

static void answers(int result, int text, int klass, int pad0, int pad1)
{
    mc_result = result;
    text_out = text;
    msg_class = klass;
    pad[0] = pad0;
    pad[1] = pad1;
}

static int was(const char *expected, int state, int second)
{
    return strcmp(trace, expected) == 0 && (int)G.state == state && (int)G.second == second;
}

/* "Q" + the routine and its mode + "G" + tail. */
static const char *tr(char letter, int mode, const char *tail)
{
    static char buffer[32];

    snprintf(buffer, sizeof buffer, "Q%c%dG%s", letter, mode, tail);
    return buffer;
}

static void check_layout(void)
{
    assert(offsetof(struct DotrMemCardProcView, msg) == 0x210 && offsetof(struct DotrMemCardProcView, res) == 0x21c);
    memset(&G, 0, sizeof G);
    G.kind = 7;
    assert(BYTES[0x22d] == 0x38 && BYTES[0x22e] == 0);
    G.kind = 0;
    G.state = 7;
    assert(BYTES[0x22d] == 0 && BYTES[0x22e] == 0x07);
    G.state = 0;
    G.second = 1;
    assert(BYTES[0x22e] == 0x08);
    G.second = 0;
    /* The byte symbol the function casts is the same block. */
    assert((void *)gSzGame_gl_mcbytes == (void *)&G);
    /* The bits of the two bytes that are not the dialogue's stay set through every test. */
    BYTES[0x22d] = 0xc7;
    BYTES[0x22e] = 0xf0;
}

static void check_idle_and_busy(void)
{
    int kind;

    busy = 0;
    answers(0, 2490, 1, 0xffff, 0xffff);
    G.msg = 2400;
    for (kind = 0; kind < 8; kind++) {
        assert(go(kind, 0, 1) == -1 && was("", 0, 1) && G.msg == 2400 && pad_asks[0] == 0);
    }
    busy = 1;
    for (kind = 0; kind < 8; kind++) {
        assert(go(kind, 1, 1) == 0 && was("Q", 1, 1) && G.msg == 2400 && pad_asks[0] == 0);
        assert(go(kind, 5, 0) == 0 && was("Q", 5, 0));
    }
    /* Even a negative text word is left alone while a file request is pending. */
    G.msg = -1;
    assert(go(0, 2, 0) == 0 && was("Q", 2, 0));
    busy = 0;
}

/* The kinds with one card routine and one acknowledge: 0, 3, 4, 5 and 6. `two` is whether
 * the second pad may also acknowledge; `errors` has bit h set when a result with high byte h
 * leads to the closing step 5. */
static void check_simple(int kind, char letter, int two, unsigned errors)
{
    int s, h, button;

    for (s = 1; s <= 4; s++) {
        int mode = s == 1 ? 0 : s - 2;

        /* Result 0 and no message: step 2, the text shown with operation 5, the pad not asked. */
        answers(0x23, 2490, 0, 0xf0, 0xf0);
        assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O5"), 2, 1));
        assert(open_value == 2490 && G.msg == 2490 && G.res == 0x23 && pad_asks[0] == 0 && pad_asks[1] == 0);

        /* A message, no button. */
        answers(0, 2490, 1, 0xff0f, 0);
        assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O6"), 2, 1) && open_value == 2490);
        assert(pad_asks[0] == 1 && pad_asks[1] == (two ? 1 : 0));

        /* A yes/no class message here is only shown: operation 7 with the cursor in bit 15,
         * and the cursor reset when the step changed. Kind 5 shows every message as class 1. */
        answers(0, 2490, 2, 0, 0);
        if (kind == 5) {
            assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O6"), 2, 1) && open_value == 2490);
        } else {
            assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O7"), 2, s == 2) && open_value == (2490 | 0x8000));
            assert(G.msg == (2490 | 0x8000));
            answers(0, 2490 | 0x8000, 2, 0, 0);
            assert(go(kind, s, 0) == 0 && was(tr(letter, mode, "O7"), 2, 0) && open_value == 2490);
        }
        answers(0, 2490, 3, 0, 0);
        assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O6"), 2, 1));
        answers(0, 2490, 4, 0, 0);
        assert(go(kind, s, 1) == 0 && was(tr(letter, mode, kind == 5 ? "O6" : "O5"), 2, 1));

        /* Any of the four buttons ends the dialogue: result 2. */
        for (button = 0x10; button <= 0x80; button <<= 1) {
            answers(0, 2490, 1, button, 0);
            assert(go(kind, s, 1) == 2 && was(tr(letter, mode, "P4X"), 0, 1) && G.msg == -1);
        }
        /* The second pad only for the trade kinds. */
        answers(0, 2490, 1, 0, 0x40);
        if (two) {
            assert(go(kind, s, 1) == 2 && was(tr(letter, mode, "P4X"), 0, 1) && G.msg == -1);
        } else {
            assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O6"), 2, 1) && pad_asks[1] == 0);
        }

        /* The result's high byte; only 16 bits of the result are kept. */
        for (h = 1; h <= 9; h++) {
            int hi = h == 9 ? 0x80 : h;

            answers(0x10000 | (hi << 8) | 5, 2490, 1, 0xf0, 0xf0);
            if (hi == 7) {
                assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O6"), 3, 0));
            } else if (kind == 6 && hi == 1) {
                /* The check kind ends quietly on an error: result 1. */
                assert(go(kind, s, 1) == 1 && was(tr(letter, mode, "X"), 0, 1) && G.msg == -1);
            } else {
                assert(go(kind, s, 1) == 0 && was(tr(letter, mode, "O6"), ((errors >> h) & 1) ? 5 : 2, 1));
            }
            assert(G.res == ((hi << 8) | 5) && pad_asks[0] == 0 && pad_asks[1] == 0);
        }

        /* The message routine may leave no text: the popup closes, result 1. */
        answers(0, -1, 0, 0, 0);
        assert(go(kind, s, 1) == 1 && was(tr(letter, mode, "X"), 2, 1));
    }

    /* Step 5, the closing acknowledge (the check kind has none). */
    answers(0, 0, 0, 0, 0);
    G.msg = 2499;
    if (kind == 6) {
        pad[0] = 0xf0;
        assert(go(kind, 5, 1) == 0 && was("QO5", 5, 1) && open_value == 2499 && pad_asks[0] == 0);
    } else {
        assert(go(kind, 5, 1) == 0 && was("QO6", 5, 1) && open_value == 2499);
        assert(pad_asks[0] == 1 && pad_asks[1] == (two ? 1 : 0));
        pad[1] = 0x10;
        if (two) {
            assert(go(kind, 5, 1) == 1 && was("QP4X", 0, 1) && G.msg == -1);
        } else {
            assert(go(kind, 5, 1) == 0 && was("QO6", 5, 1));
        }
        pad[1] = 0;
        G.msg = 2499;
        pad[0] = 0xff0f;
        assert(go(kind, 5, 1) == 0 && was("QO6", 5, 1));
        pad[0] = 0x20;
        assert(go(kind, 5, 1) == 1 && was("QP4X", 0, 1) && G.msg == -1);
        /* A text word already negative: closed without a button, the step kept. */
        pad[0] = 0;
        assert(go(kind, 5, 1) == 1 && was("QX", 5, 1));
    }
    /* Steps 6 and 7 do nothing but show the text. */
    G.msg = 2499;
    pad[0] = 0xf0;
    assert(go(kind, 6, 1) == 0 && was("QO5", 6, 1) && open_value == 2499 && pad_asks[0] == 0);
    assert(go(kind, 7, 0) == 0 && was("QO5", 7, 0));
}

static void check_check_kind(void)
{
    /* Text 2482 is not shown by the check kind. */
    answers(0, 2482, 0, 0, 0);
    assert(go(6, 1, 1) == 1 && was("QK0GX", 2, 1) && G.msg == -1);
    answers(0, 2482, 1, 0x10, 0);
    assert(go(6, 3, 1) == 2 && was("QK1GP4X", 0, 1) && G.msg == -1);
    answers(0x700, 2482, 1, 0, 0);
    assert(go(6, 4, 1) == 1 && was("QK2GX", 3, 0));
    answers(0, 2481, 0, 0, 0);
    assert(go(6, 1, 1) == 0 && was("QK0GO5", 2, 1) && open_value == 2481);
    answers(0, 2483, 0, 0, 0);
    assert(go(6, 1, 1) == 0 && was("QK0GO5", 2, 1));
    /* The other kinds show it. */
    answers(0, 2482, 0, 0, 0);
    assert(go(0, 1, 1) == 0 && was("QL0GO5", 2, 1) && open_value == 2482);
}

/* Kinds 1 and 2, step 1. */
static void check_save_start(void)
{
    /* Kind 1 starts the save at once; its result and the pad are not used. */
    answers(0x777, 2490, 2, 0xffff, 0);
    G.msg = 2400;
    G.res = 0x1234;
    assert(go(1, 1, 0) == 0 && was("QS1O5", 2, 0) && open_value == 2400 && G.res == 0x1234 && pad_asks[0] == 0);
    assert(go(1, 1, 1) == 0 && was("QS1O5", 2, 1));

    /* Kind 2 asks first: text 2505 with the yes/no cursor. */
    answers(0x777, 2490, 0, 0, 0);
    G.msg = 2400;
    assert(go(2, 1, 0) == 0 && was("QO7", 1, 0) && open_value == 2505 && G.msg == 2505 && pad_asks[0] == 3);
    assert(go(2, 1, 1) == 0 && was("QO7", 1, 1) && open_value == (2505 | 0x8000));
    /* 0x8000 moves to yes, the other direction to no; a sound only when it changed. */
    pad[0] = 0x8000;
    assert(go(2, 1, 0) == 0 && was("QP3O7", 1, 1) && open_value == (2505 | 0x8000));
    assert(go(2, 1, 1) == 0 && was("QO7", 1, 1) && open_value == (2505 | 0x8000));
    pad[0] = 0x2000;
    assert(go(2, 1, 1) == 0 && was("QP3O7", 1, 0) && open_value == 2505);
    assert(go(2, 1, 0) == 0 && was("QO7", 1, 0));
    pad[0] = 0x8000 | 0x2000;
    assert(go(2, 1, 0) == 0 && was("QP3O7", 1, 1));
    /* A direction comes before the buttons. */
    pad[0] = 0x2000 | 0xc0 | 0x20;
    assert(go(2, 1, 0) == 0 && was("QO7", 1, 0));
    /* Confirm on yes starts the save; the cursor is reset because the step changed, after
     * its bit went into the shown text. */
    pad[0] = 0x40;
    assert(go(2, 1, 1) == 0 && was("QP4S1O7", 2, 0) && open_value == (2505 | 0x8000));
    pad[0] = 0x80;
    assert(go(2, 1, 1) == 0 && was("QP4S1O7", 2, 0));
    /* Confirm on no, and cancel, end the dialogue: result 2. */
    pad[0] = 0x40 | 0x20;
    assert(go(2, 1, 0) == 2 && was("QP4X", 0, 0) && G.msg == -1);
    pad[0] = 0x20;
    assert(go(2, 1, 1) == 2 && was("QP5X", 0, 1) && G.msg == -1);
    assert(go(2, 1, 0) == 2 && was("QP5X", 0, 0));
    /* The fourth button does nothing here. */
    pad[0] = 0x10;
    assert(go(2, 1, 0) == 0 && was("QO7", 1, 0));
    pad[0] = 0;
}

/* Kinds 1 and 2, steps 2-5. */
static void check_save_steps(int kind)
{
    int s, h;

    for (s = 2; s <= 4; s++) {
        int mode = s - 2;

        answers(0x23, 2490, 0, 0xffff, 0);
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "O5"), 2, 1) && open_value == 2490 && pad_asks[0] == 0);

        /* Yes/no message. */
        answers(0, 2485, 2, 0, 0);
        assert(go(kind, s, 0) == 0 && was(tr('S', mode, "O7"), 2, 0) && open_value == 2485 && pad_asks[0] == 3);
        pad[0] = 0x8000;
        assert(go(kind, s, 0) == 0 && was(tr('S', mode, "P3O7"), 2, s == 2) && open_value == (2485 | 0x8000));
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "O7"), 2, s == 2));
        pad[0] = 0x2000;
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "P3O7"), 2, 0) && open_value == 2485);
        /* Confirm on yes: step 4. */
        pad[0] = 0x40;
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "P4O7"), 4, s == 4) && open_value == (2485 | 0x8000));
        pad[0] = 0x80;
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "P4O7"), 4, s == 4));
        /* Confirm on no, and cancel: kind 1 ends (result 2), kind 2 goes back to its
         * question with the cursor on yes. */
        pad[0] = 0x40;
        if (kind == 1) {
            assert(go(kind, s, 0) == 2 && was(tr('S', mode, "P4X"), 0, 0) && G.msg == -1);
            pad[0] = 0x20;
            assert(go(kind, s, 1) == 2 && was(tr('S', mode, "P5X"), 0, 1) && G.msg == -1);
        } else {
            assert(go(kind, s, 0) == 0 && was(tr('S', mode, "P4O7"), 1, 1) && open_value == (2485 | 0x8000));
            pad[0] = 0x20;
            assert(go(kind, s, 0) == 0 && was(tr('S', mode, "P5O7"), 1, 1) && open_value == (2485 | 0x8000));
        }
        pad[0] = 0x10;
        assert(go(kind, s, 0) == 0 && was(tr('S', mode, "O7"), 2, 0));

        /* Class 3: any button goes on to step 4. */
        answers(0, 2506, 3, 0xff0f, 0);
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "O6"), 2, 1) && open_value == 2506 && pad_asks[0] == 1);
        pad[0] = 0x10;
        assert(go(kind, s, 1) == 0 && was(tr('S', mode, "P4O6"), 4, 1));
        assert(go(kind, s, 0) == 0 && was(tr('S', mode, "P4O6"), 4, 0));

        /* Any other class: a button ends (kind 1) or goes back (kind 2). */
        answers(0, 2488, 1, 0, 0);
        assert(go(kind, s, 0) == 0 && was(tr('S', mode, "O6"), 2, 0));
        msg_class = 4;
        assert(go(kind, s, 0) == 0 && was(tr('S', mode, "O5"), 2, 0));
        pad[0] = 0x80;
        if (kind == 1) {
            assert(go(kind, s, 0) == 2 && was(tr('S', mode, "P4X"), 0, 0) && G.msg == -1);
            msg_class = 1;
            assert(go(kind, s, 1) == 2 && was(tr('S', mode, "P4X"), 0, 1));
        } else {
            assert(go(kind, s, 0) == 0 && was(tr('S', mode, "P4O5"), 1, 1) && open_value == 2488);
            msg_class = 1;
            assert(go(kind, s, 0) == 0 && was(tr('S', mode, "P4O6"), 1, 1));
        }

        /* The result's high byte: 1 closes, 7 asks, others stay. */
        for (h = 1; h <= 8; h++) {
            answers(0x10000 | (h << 8) | 5, 2485 | 0x8000, 2, 0xffff, 0);
            assert(go(kind, s, 1) == 0 && pad_asks[0] == 0);
            if (h == 7) {
                assert(was(tr('S', mode, "O7"), 3, 0) && open_value == 2485);
            } else if (h == 1) {
                assert(was(tr('S', mode, "O7"), 5, 0) && open_value == (2485 | 0x8000));
            } else {
                assert(was(tr('S', mode, "O7"), 2, s == 2) && open_value == (2485 | 0x8000));
            }
        }
        answers(0, -3, 2, 0, 0);
        assert(go(kind, s, 1) == 1 && was(tr('S', mode, "X"), 2, 1));
    }

    answers(0, 0, 0, 0, 0xf0);
    G.msg = 2499;
    assert(go(kind, 5, 1) == 0 && was("QO6", 5, 1) && open_value == 2499 && pad_asks[0] == 1 && pad_asks[1] == 0);
    pad[0] = 0x10;
    assert(go(kind, 5, 1) == 1 && was("QP4X", 0, 1) && G.msg == -1);
    G.msg = 2499;
    assert(go(kind, 6, 1) == 0 && was("QO5", 6, 1));
}

int main(void)
{
    check_layout();
    check_idle_and_busy();
    check_simple(0, 'L', 0, 1u << 1);
    check_simple(3, 'C', 0, 0x3eu);
    check_simple(4, 'T', 1, 1u << 1);
    check_simple(5, 'U', 1, (1u << 1) | (1u << 6));
    check_simple(6, 'K', 0, 0);
    check_check_kind();
    check_save_start();
    check_save_steps(1);
    check_save_steps(2);
    /* Kind 7 has no case: the text is only shown. */
    answers(0, 0, 0, 0xffff, 0xffff);
    G.msg = 2499;
    assert(go(7, 1, 1) == 0 && was("QO5", 1, 1) && open_value == 2499 && pad_asks[0] == 0);
    assert(go(7, 5, 0) == 0 && was("QO5", 5, 0));
    G.msg = -1;
    assert(go(7, 3, 0) == 1 && was("QX", 3, 0));
    assert((BYTES[0x22d] & 0xc7) == 0xc7 && (BYTES[0x22e] & 0xf0) == 0xf0);
    puts("memcard_proc: ok");
    return 0;
}
