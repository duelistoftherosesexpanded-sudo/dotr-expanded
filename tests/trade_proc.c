/* Synthetic host tests for the trade_proc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/trade_proc.h"

struct DotrTradeProcControl data_00317490_tradeproc;
#define C data_00317490_tradeproc

/* Per side: the card list and the count bytes. One byte more than the 854 summed, so a test
 * can show that the byte after the last one is not counted. */
static unsigned short list[2][854];
static unsigned char cnt[2][855];

/* The side the frame is run for: every callee that takes a pad or a side must be given it. */
static int cur;
/* The pad: one mask per way of asking (0, 1, 2). */
static int pad[3], pad_asks;
static int analog_on, analog_y, analog_asks;
/* Answers of the data callees. */
static int bag_count, trade_result;
/* What was called: sounds in order, counters, and one letter per call in call order
 * (S sound, B bag count, U setup, O sort, P pop, D trade, M memory card). */
static int se[8], se_n;
static int bag_id, bag_calls, setup_calls, sort_calls, pop_calls, trade_calls, memcard_calls;
static char order[16];
static int order_n;

static void note(char c)
{
    assert(order_n < 15);
    order[order_n++] = c;
    order[order_n] = 0;
}

int SzPad_Check(int p, int kind, int mask)
{
    assert(p == cur && kind >= 0 && kind < 3);
    pad_asks++;
    return pad[kind] & mask;
}
int SzPad_IsAnalog(int p)
{
    assert(p == cur);
    analog_asks++;
    return analog_on;
}
int SzPad_GetAnalogInfo(int p, int which)
{
    assert(p == cur && which == 3);
    return 0x7700 | analog_y;       /* only the low byte may be used */
}
int SzSound_PlaySE(int a, int b)
{
    assert(a == 0 && se_n < 8);
    se[se_n++] = b;
    note('S');
    return 0;
}
int SzData_GetBagCount(int side, int id, int kind)
{
    assert(side == cur && kind == 3);
    bag_id = id;
    bag_calls++;
    note('B');
    return bag_count;
}
void lSzDuel_SetupTrade(int flag)
{
    assert(flag == cur);
    setup_calls++;
    C.pairs[flag].top = 77;         /* the caller resets the first row afterwards */
    note('U');
}
void lSzDuel_SortTrade(int flag)
{
    assert(flag == cur);
    sort_calls++;
    note('O');
}
void SzGame_Pop(void)
{
    pop_calls++;
    note('P');
}
int lSzDuel_TradeDo(void)
{
    trade_calls++;
    note('D');
    return trade_result;
}
void SzGame_InitMemCardProc(int kind)
{
    assert(kind == 5);
    memcard_calls++;
    note('M');
}

/* Both sides in the given states, 100 rows each, nothing offered. Side 0 lists the ids in
 * order, side 1 backwards, so the two sides never agree on the id under the cursor. */
static void reset(int state0, int state1)
{
    int i;

    memset(&C, 0, sizeof C);
    memset(pad, 0, sizeof pad);
    memset(cnt, 0, sizeof cnt);
    for (i = 0; i < 854; i++) {
        list[0][i] = (unsigned short)i;
        list[1][i] = (unsigned short)(853 - i);
    }
    C.lists[0] = list[0];
    C.lists[1] = list[1];
    C.counts[0] = cnt[0];
    C.counts[1] = cnt[1];
    C.pairs[0].count = C.pairs[1].count = 100;
    C.state[0] = (unsigned char)state0;
    C.state[1] = (unsigned char)state1;
    analog_on = 0; analog_y = 0x80;
    bag_count = 1; trade_result = 1;
    bag_id = -1;
    bag_calls = setup_calls = sort_calls = pop_calls = trade_calls = memcard_calls = 0;
}
/* Side w in state s, the other side in state t. */
static void reset_for(int w, int s, int t)
{
    if (w) {
        reset(t, s);
    } else {
        reset(s, t);
    }
}
/* One frame for side w with the given buttons for each way of asking; gives the result. */
static int frame_r(int w, int now, int held, int repeat)
{
    cur = w;
    pad[0] = now; pad[1] = held; pad[2] = repeat;
    se_n = 0; order_n = 0; order[0] = 0; pad_asks = 0; analog_asks = 0;
    return lSzDuel_ActTradeProc(w);
}
/* The same for the frames that must answer 0: all but one path. */
static void frame(int w, int now, int held, int repeat)
{
    int r = frame_r(w, now, held, repeat);

    assert(r == 0);
}
static int one_se(int b)
{
    return se_n == 1 && se[0] == b;
}
static int states(int w, int mine, int theirs)
{
    return C.state[w] == mine && C.state[w ^ 1] == theirs;
}

/* Before the switch: the idle countdown runs only below state 4, for this side only. */
static void check_idle(int w)
{
    int o = w ^ 1;

    reset_for(w, 3, 3);
    C.idle[w] = 5; C.idle[o] = 5;
    frame(w, 0, 0, 0);
    assert(C.idle[w] == 4 && C.idle[o] == 5);
    frame(w, 0, 0x8000, 0);                 /* anything held */
    assert(C.idle[w] == 90 && C.idle[o] == 5);
    frame(w, 0, 0x0001, 0);
    assert(C.idle[w] == 90);
    C.idle[w] = 1;
    frame(w, 0, 0, 0);
    frame(w, 0, 0, 0);
    assert(C.idle[w] == 0);                 /* stops at 0 */
    frame(w, 0xffff, 0, 0xffff);            /* pressed and repeat do not count as held */
    assert(C.idle[w] == 0);
    reset_for(w, 4, 3);                     /* state 4 and above: not touched */
    C.idle[w] = 5;
    frame(w, 0, 0xffff, 0);
    assert(C.idle[w] == 5);
    frame(w, 0, 0, 0);
    assert(C.idle[w] == 5);
}

/* States 0 and 2: the panel moves 45 a frame to 0 or to 320, then the state changes. */
static void check_slides(int w)
{
    int o = w ^ 1;

    reset_for(w, 0, 0);
    C.slide[w] = 100; C.slide[o] = 100;
    frame(w, 0xffff, 0, 0xffff);
    assert(C.slide[w] == 55 && states(w, 0, 0) && C.slide[o] == 100 && se_n == 0);
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 10 && states(w, 0, 0));
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 0 && states(w, 1, 0));     /* clamped, and the list opens */
    reset_for(w, 0, 0);
    C.slide[w] = 46;
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 1 && states(w, 0, 0));
    reset_for(w, 0, 0);
    C.slide[w] = 45;
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 0 && states(w, 1, 0));
    reset_for(w, 0, 0);
    C.slide[w] = -7;                                /* below 0 is brought back to 0 */
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 0 && states(w, 1, 0));

    reset_for(w, 2, 2);
    C.slide[w] = 230; C.slide[o] = 7;
    frame(w, 0xffff, 0, 0xffff);
    assert(C.slide[w] == 275 && states(w, 2, 2) && C.slide[o] == 7 && se_n == 0);
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 320 && states(w, 3, 2));
    reset_for(w, 2, 2);
    C.slide[w] = 274;
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 319 && states(w, 2, 2));
    frame(w, 0, 0, 0);
    assert(C.slide[w] == 320 && states(w, 3, 2));   /* clamped */
}

/* State 1: cursor row, scrolling by one and by five, the analog stick. */
static void check_list_moves(int w)
{
    int o = w ^ 1;

    /* A row change sounds and ends the frame; it does not touch the first row or the idle count. */
    reset_for(w, 1, 1);
    C.pairs[w].top = 10; C.rows[w] = 2; C.idle[w] = 7;
    frame(w, 0x20, 0, 0x1000);
    assert(C.rows[w] == 1 && C.pairs[w].top == 10 && one_se(3) && C.idle[w] == 6 && states(w, 1, 1));
    frame(w, 0, 0, 0x4000);
    assert(C.rows[w] == 2 && C.pairs[w].top == 10 && one_se(3) && analog_asks == 0);
    assert(C.rows[o] == 0 && C.pairs[o].top == 0);
    /* At the first or last row the list scrolls instead, and the idle count restarts. */
    C.rows[w] = 0;
    frame(w, 0, 0, 0x1000);
    assert(C.rows[w] == 0 && C.pairs[w].top == 9 && one_se(3) && C.idle[w] == 90);
    C.rows[w] = 4;
    frame(w, 0, 0, 0x4000);
    assert(C.rows[w] == 4 && C.pairs[w].top == 10 && one_se(3));
    C.rows[w] = 3;
    frame(w, 0, 0, 0x5000);                 /* both: up is asked first */
    assert(C.rows[w] == 2);
    /* A scroll that cannot move is silent but still ends the frame: the cancel is not seen. */
    reset_for(w, 1, 1);
    frame(w, 0x20, 0, 0x1000);
    assert(C.pairs[w].top == 0 && C.rows[w] == 0 && se_n == 0 && C.idle[w] == 90 && states(w, 1, 1));
    /* Pages of five, before the row buttons; the last first row is count - 5. */
    reset_for(w, 1, 1);
    C.pairs[w].top = 10; C.rows[w] = 2;
    frame(w, 0, 0, 0x4 | 0x4000);
    assert(C.pairs[w].top == 5 && C.rows[w] == 2 && one_se(3));
    frame(w, 0, 0, 0x8);
    assert(C.pairs[w].top == 10);
    frame(w, 0, 0, 0xc);                    /* both: page up is asked first */
    assert(C.pairs[w].top == 5);
    frame(w, 0, 0, 0x4);
    frame(w, 0, 0, 0x4);
    assert(C.pairs[w].top == 0 && se_n == 0);
    C.pairs[w].top = 94;
    frame(w, 0, 0, 0x8);
    assert(C.pairs[w].top == 95 && one_se(3));
    frame(w, 0, 0, 0x8);
    assert(C.pairs[w].top == 95 && se_n == 0);
    /* Fewer than five rows: nowhere to scroll. */
    reset_for(w, 1, 1);
    C.pairs[w].count = 3;
    frame(w, 0, 0, 0x8);
    assert(C.pairs[w].top == 0 && se_n == 0 && C.idle[w] == 90);

    /* The stick, asked only when no button moved anything: dead zone of 48 either way. */
    reset_for(w, 1, 1);
    analog_on = 1; analog_y = 0xff; C.pairs[w].top = 10;
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 19 && one_se(3) && C.idle[w] == 90);   /* (127 - 48) * 15 >> 7 */
    analog_y = 0;
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 9 && one_se(3));                       /* (-128 + 48) * 15 >> 7 = -10 */
    analog_y = 0xb0;                                                /* +48: inside */
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 9 && se_n == 0 && C.idle[w] == 89 && analog_asks == 1);
    analog_y = 0x50;                                                /* -48: inside */
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 9 && se_n == 0);
    analog_y = 0xb1;                                                /* +49: outside, but 15 >> 7 is 0 */
    C.idle[w] = 50;
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 9 && se_n == 0 && C.idle[w] == 49);
    analog_y = 0xb9;                                                /* +57: 135 >> 7 is 1 */
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 10 && one_se(3));
    analog_y = 0x4f;                                                /* -49: -15 >> 7 is -1 */
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 9 && one_se(3) && C.idle[w] == 90);
    analog_y = 0xff;
    frame(w, 0, 0, 0x4000);                 /* a row change: the stick is not asked */
    assert(C.rows[w] == 1 && C.pairs[w].top == 9 && analog_asks == 0);
    frame(w, 0, 0, 0x8);                    /* a page: the stick is not asked */
    assert(C.pairs[w].top == 14 && analog_asks == 0);
    analog_on = 0;
    frame(w, 0, 0, 0);
    assert(C.pairs[w].top == 14 && se_n == 0 && analog_asks == 1);

    /* An empty list: no movement, no stick, no card buttons; the buttons after them still work. */
    reset_for(w, 1, 1);
    C.pairs[w].count = 0; C.rows[w] = 2; analog_on = 1; analog_y = 0xff;
    frame(w, 0x10, 0, 0x4000 | 0x8 | 0xc0);
    assert(C.rows[w] == 2 && C.pairs[w].top == 0 && se_n == 0 && analog_asks == 0 && bag_calls == 0 && states(w, 1, 1));
    frame(w, 0x20, 0, 0x4000);
    assert(states(w, 5, 5) && one_se(5));
}

/* State 1: offering a copy and taking it back, and the detail panel. */
static void check_list_cards(int w)
{
    int o = w ^ 1;
    int add = w ? 0x80 : 0x40;
    int sub = w ? 0x40 : 0x80;
    int id = w ? 853 - 12 : 12;

    reset_for(w, 1, 1);
    C.pairs[w].top = 10; C.rows[w] = 2;
    bag_count = 2;
    frame(w, 0x20, 0, add);                 /* ends the frame: the cancel is not seen */
    assert(bag_calls == 1 && bag_id == id && cnt[w][id] == 1 && one_se(3) && strcmp(order, "BS") == 0);
    assert(states(w, 1, 1) && cnt[o][id] == 0);
    frame(w, 0, 0, 0xc0);                   /* both: this side's own button adds */
    assert(cnt[w][id] == 2 && one_se(3));
    frame(w, 0, 0, add);                    /* no more than the bag holds */
    assert(cnt[w][id] == 2 && one_se(6) && bag_calls == 3);
    frame(w, 0, 0, sub);
    assert(cnt[w][id] == 1 && one_se(3));
    frame(w, 0, 0, sub);
    assert(cnt[w][id] == 0 && one_se(3));
    frame(w, 0, 0, sub);                    /* not below 0 */
    assert(cnt[w][id] == 0 && one_se(6) && bag_calls == 6);
    bag_count = 0;
    frame(w, 0, 0, add);
    assert(cnt[w][id] == 0 && one_se(6));
    /* Refused before the bag is asked: an empty row (999), and list mode 2. */
    reset_for(w, 1, 1);
    list[w][3] = 999; C.rows[w] = 3;
    frame(w, 0, 0, add);
    assert(one_se(6) && bag_calls == 0);
    frame(w, 0, 0, sub);
    assert(one_se(6) && bag_calls == 0);
    reset_for(w, 1, 1);
    C.modes[w] = 2; cnt[w][list[w][0]] = 1;
    frame(w, 0, 0, add);
    assert(one_se(6) && bag_calls == 0);
    frame(w, 0, 0, sub);
    assert(one_se(6) && bag_calls == 0 && cnt[w][list[w][0]] == 1);
    C.modes[w] = 1; C.modes[o] = 2;         /* the other side's mode does not refuse */
    frame(w, 0, 0, sub);
    assert(one_se(3) && bag_calls == 1 && cnt[w][list[w][0]] == 0);

    /* Detail: slides the panel out, except on an empty row. */
    reset_for(w, 1, 1);
    frame(w, 0x10 | 0x20, 0, 0);
    assert(states(w, 2, 1) && one_se(16));
    reset_for(w, 1, 1);
    list[w][0] = 999;
    frame(w, 0x10 | 0x20, 0, 0);
    assert(states(w, 1, 1) && one_se(6));
}

/* State 1: the buttons that do not need a card. */
static void check_list_other(int w)
{
    int o = w ^ 1;

    /* List mode: back or forward over three, then the list is rebuilt and shown from row 0. */
    reset_for(w, 1, 1);
    C.pairs[w].top = 10; C.modes[o] = 1;
    frame(w, 0x200, 0, 0);
    assert(C.modes[w] == 2 && C.modes[o] == 1 && setup_calls == 1 && C.pairs[w].top == 0);
    assert(one_se(19) && strcmp(order, "US") == 0);
    frame(w, 0x400, 0, 0);
    assert(C.modes[w] == 0 && setup_calls == 2);
    frame(w, 0x400, 0, 0);
    assert(C.modes[w] == 1);
    frame(w, 0x600 | 0x800 | 0x20, 0, 3);   /* both: back is asked first; nothing after it runs */
    assert(C.modes[w] == 0 && setup_calls == 4 && sort_calls == 0 && states(w, 1, 1) && one_se(19));

    /* Offer: waits for the other side; when it already waits, something must be offered. */
    reset_for(w, 1, 1);
    frame(w, 0x800 | 0x20, 0, 3);
    assert(states(w, 4, 1) && one_se(4) && sort_calls == 0);
    reset_for(w, 1, 4);
    C.confirm = 1;
    cnt[0][854] = 1; cnt[1][854] = 1;       /* the byte after the 854 is not part of the sum */
    frame(w, 0x800, 0, 0);
    assert(states(w, 1, 4) && one_se(6) && C.confirm == 1);
    cnt[w][853] = 1;                        /* the last byte is */
    frame(w, 0x800, 0, 0);
    assert(states(w, 6, 6) && one_se(4) && C.confirm == 0);
    reset_for(w, 1, 4);
    C.confirm = 1;
    cnt[o][0] = 1;                          /* the other side's offer is enough */
    frame(w, 0x800, 0, 0);
    assert(states(w, 6, 6) && one_se(4) && C.confirm == 0);
    reset_for(w, 1, 5);                     /* any other state of the other side */
    cnt[w][0] = 1;
    frame(w, 0x800, 0, 0);
    assert(states(w, 4, 5) && one_se(4));

    /* Sort keys over ten: the held button picks the second key. */
    reset_for(w, 1, 1);
    frame(w, 0x20, 0, 1);
    assert(C.keys[w][0] == 9 && C.keys[w][1] == 0 && sort_calls == 1 && one_se(19) && strcmp(order, "OS") == 0);
    assert(states(w, 1, 1));
    frame(w, 0, 0, 2);
    assert(C.keys[w][0] == 0 && sort_calls == 2);
    frame(w, 0, 0, 3);                      /* both: back is asked first */
    assert(C.keys[w][0] == 9);
    frame(w, 0, 0x100, 2);
    assert(C.keys[w][1] == 1 && C.keys[w][0] == 9 && sort_calls == 4 && C.idle[w] == 90);
    frame(w, 0, 0x100, 1);
    assert(C.keys[w][1] == 0 && C.keys[w][2] == 0 && C.keys[o][0] == 0 && C.keys[o][1] == 0);
    C.keys[w][0] = 9;
    frame(w, 0, 0, 2);
    assert(C.keys[w][0] == 0);

    /* Cancel: asks both sides, only while the other side is in its list too. */
    reset_for(w, 1, 1);
    C.confirm = 1;
    frame(w, 0x20, 0, 0);
    assert(states(w, 5, 5) && one_se(5) && C.confirm == 0);
    reset_for(w, 1, 4);
    C.confirm = 1;
    frame(w, 0x20, 0, 0);
    assert(states(w, 1, 4) && one_se(6) && C.confirm == 1);
    /* Nothing pressed: nothing happens. */
    reset_for(w, 1, 1);
    frame(w, 0, 0, 0);
    assert(states(w, 1, 1) && order_n == 0);
}

/* States 3 and 4: the detail panel's tab, and waiting for the other side. */
static void check_tab_and_wait(int w)
{
    int o = w ^ 1;

    reset_for(w, 3, 1);
    frame(w, 0x30, 0, 0x1000);              /* the tab buttons come before leaving */
    assert(C.tab[w] == 3 && C.tab[o] == 0 && one_se(19) && states(w, 3, 1));
    frame(w, 0, 0, 0x4000);
    assert(C.tab[w] == 0 && one_se(19));
    frame(w, 0, 0, 0x4000);
    assert(C.tab[w] == 1);
    frame(w, 0, 0, 0x5000);                 /* both: up is asked first */
    assert(C.tab[w] == 0);
    frame(w, 0x10, 0, 0);
    assert(states(w, 0, 1) && one_se(17));
    reset_for(w, 3, 1);
    frame(w, 0x20, 0, 0);
    assert(states(w, 0, 1) && one_se(17));
    reset_for(w, 3, 1);
    frame(w, 0xffcf, 0, 0xafff);
    assert(states(w, 3, 1) && C.tab[w] == 0 && order_n == 0);

    reset_for(w, 4, 4);
    frame(w, 0xffdf, 0xffff, 0xffff);
    assert(states(w, 4, 4) && order_n == 0);
    frame(w, 0x20, 0, 0);
    assert(states(w, 1, 4) && one_se(5));
}

/* States 5 and 6: the two yes/no boxes, shared by both sides. */
static void check_confirms(int w)
{
    int s, r;

    for (s = 5; s <= 6; s++) {
        reset_for(w, s, s);
        frame(w, 0x8000 | 0xc0 | 0x20, 0, 0);   /* the cursor comes before the answer */
        assert(C.confirm == 1 && one_se(3) && states(w, s, s));
        frame(w, 0x8000, 0, 0);
        assert(C.confirm == 1 && se_n == 0);
        frame(w, 0xa000, 0, 0);                 /* both: yes is asked first */
        assert(C.confirm == 1 && se_n == 0);
        frame(w, 0x2000, 0, 0);
        assert(C.confirm == 0 && one_se(3));
        frame(w, 0x2000, 0, 0);
        assert(C.confirm == 0 && se_n == 0);
        frame(w, 0, 0xffff, 0xffff);            /* only buttons pressed now count */
        assert(states(w, s, s) && order_n == 0);
    }

    /* State 5, leaving the screen: yes pops, no and cancel put both sides back in their lists. */
    reset_for(w, 5, 5);
    C.confirm = 1;
    frame(w, 0x40 | 0x20, 0, 0);
    assert(pop_calls == 1 && states(w, 5, 5) && one_se(4) && strcmp(order, "SP") == 0);
    reset_for(w, 5, 5);
    frame(w, 0x80, 0, 0);
    assert(pop_calls == 0 && states(w, 1, 1) && one_se(4));
    reset_for(w, 5, 5);
    C.confirm = 1;
    frame(w, 0x20, 0, 0);
    assert(pop_calls == 0 && states(w, 1, 1) && one_se(5));

    /* State 6, the trade: yes runs it. Above 0: both sides to 8. Otherwise 7, the memory card
     * screen of kind 5, and the only result of 1. */
    reset_for(w, 6, 6);
    C.confirm = 1; trade_result = 1;
    r = frame_r(w, 0x40 | 0x20, 0, 0);
    assert(r == 0 && trade_calls == 1 && states(w, 8, 8) && memcard_calls == 0 && strcmp(order, "SD") == 0);
    assert(se[0] == 4 && pop_calls == 0);
    reset_for(w, 6, 6);
    C.confirm = 1; trade_result = 0;
    r = frame_r(w, 0x80, 0, 0);
    assert(r == 1 && trade_calls == 1 && states(w, 7, 7) && memcard_calls == 1 && strcmp(order, "SDM") == 0);
    reset_for(w, 6, 6);
    C.confirm = 1; trade_result = -1;
    r = frame_r(w, 0x80, 0, 0);
    assert(r == 1 && states(w, 7, 7) && memcard_calls == 1);
    /* No and cancel: this side back to its list, the other side waiting again. */
    reset_for(w, 6, 6);
    frame(w, 0x40, 0, 0);
    assert(trade_calls == 0 && states(w, 1, 4) && one_se(4));
    reset_for(w, 6, 6);
    C.confirm = 1;
    frame(w, 0x20, 0, 0);
    assert(trade_calls == 0 && states(w, 1, 4) && one_se(5));
}

/* States 7 and 8 leave; a state above 8 does nothing at all. */
static void check_leaving(int w)
{
    int b;

    reset_for(w, 7, 7);
    frame(w, 0, 0, 0);
    assert(pop_calls == 1 && se_n == 0 && pad_asks == 0 && states(w, 7, 7));
    for (b = 0x10; b <= 0x80; b <<= 1) {
        reset_for(w, 8, 8);
        frame(w, 0xff0f, 0xffff, 0xffff);
        assert(pop_calls == 0 && order_n == 0);
        frame(w, b, 0, 0);
        assert(pop_calls == 1 && one_se(4) && strcmp(order, "SP") == 0 && states(w, 8, 8));
    }
    reset_for(w, 9, 1);
    C.idle[w] = 5; C.slide[w] = 100;
    frame(w, 0xffff, 0xffff, 0xffff);
    assert(states(w, 9, 1) && order_n == 0 && pad_asks == 0 && C.idle[w] == 5 && C.slide[w] == 100);
    reset_for(w, 255, 1);
    frame(w, 0xffff, 0xffff, 0xffff);
    assert(states(w, 255, 1) && order_n == 0 && pad_asks == 0);
}

int main(void)
{
    int w;

    for (w = 0; w < 2; w++) {
        check_idle(w);
        check_slides(w);
        check_list_moves(w);
        check_list_cards(w);
        check_list_other(w);
        check_tab_and_wait(w);
        check_confirms(w);
        check_leaving(w);
    }
    puts("trade_proc: ok");
    return 0;
}
