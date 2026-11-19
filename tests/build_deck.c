/* Synthetic host tests for the build_deck group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/build_deck.h"

struct DotrBuildDeckControl data_00317490_build;
#define C data_00317490_build

static unsigned short bag[854];
static unsigned short deck[41];
static unsigned short work[32];

/* The pad: one mask per way of asking (0, 1, 2). */
static int pad[3];
static int analog_on, analog_x, analog_y;
/* Answers of the data callees. */
static int current_deck, busy, bag_count, look_flag, rank, check_at, duel5, ready[3];
static int push_ok, pull_ok, leader_ok, password, pass_flag;
/* What was called: sounds in order, and counters. */
static int se[8], se_n;
static int view_id, push_calls, pop_calls, sort_bag, sort_deck, rebuild, ghost[5], ghost_calls;
static int text_free, load_merge, reset_mask, del_id, add_id[4], add_n, set_deck, recycle_n;
static int pulled_at, pushed_id, leader_pulled, leader_pushed, flag_set, duel5_set;

int SzPad_Check(int p, int kind, int mask)
{
    assert(p == 0 && kind >= 0 && kind < 3);
    return pad[kind] & mask;
}
void SzPad_ResetCounter(int p, int mask)
{
    assert(p == 0);
    reset_mask = mask;
}
int SzPad_IsAnalog(int p)
{
    assert(p == 0);
    return analog_on;
}
int SzPad_GetAnalogInfo(int p, int which)
{
    assert(p == 0 && (which == 1 || which == 3));
    return 0x7700 | (which == 3 ? analog_y : analog_x);
}
int SzSound_PlaySE(int a, int b)
{
    assert(a == 0 && se_n < 8);
    se[se_n++] = b;
    return 0;
}
int SzData_GetCurrentDeck(int side)
{
    assert(side == 1);
    return current_deck;
}
void SzData_SetCurrentDeck(int side, int d)
{
    assert(side == 1);
    set_deck = d;
    current_deck = d;
}
int SzFile_InqRequestEx(void)
{
    return busy;
}
int SzText_LoadMerge(int a, int b)
{
    assert(a == 6 && b == 1);
    load_merge++;
    return 0;
}
void SzText_Free(int a, int b)
{
    assert(a == 2 && b == 1);
    text_free++;
}
int SzData_GetBagCount(int side, int id, int kind)
{
    assert(side == 1 && (kind == -1 || kind == 3) && id >= 0);
    return bag_count;
}
int SzData_GetLookFlag(int side, int id)
{
    assert(side == 1 && id < 683);
    return look_flag;
}
void SzDuel_Init3DView(int id)
{
    view_id = id;
}
void SzGame_Push(int a, int b)
{
    assert(a == 7 && b == 2);
    push_calls++;
}
void SzGame_Pop(void)
{
    pop_calls++;
}
int SzData_CheckDeck(int side, int d, int id)
{
    assert(side == 1 && d == current_deck);
    (void)id;
    return check_at;
}
int Taku_Get_Duel5_flg(void)
{
    return duel5;
}
void Taku_Set_Duel5_flg(void)
{
    duel5_set++;
}
int lSzDuel_IsDeckReady(int side, int d)
{
    assert(side == 1 && d >= 0 && d < 3);
    return ready[d];
}
int SzData_GetRank(int side, int id)
{
    assert(side == 1);
    (void)id;
    return rank;
}
void SzData_PullDeckLeader(int side, int d)
{
    assert(side == 1 && d == current_deck);
    leader_pulled++;
}
int SzData_PushDeckLeader(int side, int d, int id)
{
    assert(side == 1 && d == current_deck);
    leader_pushed = id;
    return leader_ok;
}
void lSzDuel_ReqGhostCard(int id, int row, int a, int b, int c)
{
    ghost[0] = id; ghost[1] = row; ghost[2] = a; ghost[3] = b; ghost[4] = c;
    ghost_calls++;
}
int SzData_PushDeck(int side, int d, int id)
{
    assert(side == 1 && d == current_deck);
    pushed_id = id;
    return push_ok;
}
int SzData_PullDeck(int side, int d, int index)
{
    assert(side == 1 && d == current_deck);
    pulled_at = index;
    return pull_ok;
}
void lSzDuel_SortBag(void)
{
    sort_bag++;
}
void lSzDuel_SortDeck(void)
{
    sort_deck++;
}
int SzData_GetLeader(int side, int d)
{
    assert(side == 1);
    return 500 + d;
}
int SzData_GetCardNo(int side, int d, int i)
{
    assert(side == 1);
    return 600 + d * 100 + i;
}
int Ta_GetRecycle(int side, int id)
{
    assert(side == 1 && id == 123);
    return 300 + recycle_n++;
}
void SzData_DelBag(int side, int id)
{
    assert(side == 1);
    del_id = id;
}
void SzData_AddBag(int side, int a, int id)
{
    assert(side == 1 && a == 0 && add_n < 4);
    add_id[add_n++] = id;
}
void lSzDuel_ReBuildBag(void)
{
    rebuild++;
}
int lSzDuel_HitPassword(char *text)
{
    assert(text == C.pass);
    return password;
}
int SzData_GetPassFlag(int side, int n)
{
    assert(side == 1 && n == password);
    return pass_flag;
}
void SzData_SetPassFlag(int side, int n, int v)
{
    assert(side == 1 && n == password && v == 1);
    flag_set++;
}

static void reset(int mode)
{
    int i;

    memset(&C, 0, sizeof C);
    memset(pad, 0, sizeof pad);
    memset(work, 0, sizeof work);
    for (i = 0; i < 854; i++) {
        bag[i] = (unsigned short)i;
    }
    for (i = 0; i < 41; i++) {
        deck[i] = (unsigned short)(700 + i);
    }
    C.bag = bag;
    C.deck = deck;
    C.work = (DotrBuildWord)work;
    C.side = 1;
    C.mode = (unsigned char)mode;
    C.bagCount = 100;
    C.deckCount = 41;
    analog_on = 0; analog_x = analog_y = 0x80;
    current_deck = 0; busy = 0; bag_count = 1; look_flag = 0; rank = 1; check_at = 7; duel5 = 5;
    ready[0] = ready[1] = ready[2] = 1;
    push_ok = pull_ok = leader_ok = 1; password = 42; pass_flag = 0;
    se_n = 0; view_id = -1; push_calls = pop_calls = sort_bag = sort_deck = rebuild = ghost_calls = 0;
    text_free = load_merge = reset_mask = add_n = recycle_n = 0; del_id = -1; set_deck = -1;
    pulled_at = pushed_id = leader_pushed = -1; leader_pulled = flag_set = duel5_set = 0;
}
/* One frame with the given buttons for each way of asking. */
static void frame(int now, int held, int repeat)
{
    pad[0] = now; pad[1] = held; pad[2] = repeat;
    se_n = 0;
    SzDuel_ActBuildDeck();
}
static int one_se(int b)
{
    return se_n == 1 && se[0] == b;
}

/* The part before the mode switch: colour pulses, panel slide, idle counter, text reload. */
static void check_preamble(void)
{
    reset(10);
    C.fadeOn = 1; C.fadeA = 0x7f00; C.dirB = -1; C.fadeB = 0x400;
    C.reload = 1;
    frame(0, 0, 0);
    assert(load_merge == 1 && C.reload == 0);
    assert(C.dirA == -1 && C.fadeA == 0x8000);  /* forced up, clamped, turned round */
    assert(C.fadeB == 0 && C.dirB == 1);
    frame(0, 0, 0);
    assert(C.fadeA == 0x8000 && C.dirA == -1 && C.fadeB == 0x800 && C.dirB == 1);
    C.fadeOn = 0;
    frame(0, 0, 0);
    assert(C.fadeA == 0x7e00 && C.fadeB == 0x1000);
    busy = 1; C.reload = 1;
    frame(0, 0, 0);
    assert(load_merge == 1 && C.reload == 1);

    /* The panel moves at most 45 a frame and nothing else runs while it moves. */
    reset(10);
    C.slide = 0; C.slideTo = 100; C.prevMode = 3; C.idle = 5;
    frame(0xf0, 0xffff, 0);
    assert(C.slide == 45 && C.mode == 10 && C.idle == 5 && se_n == 0);
    frame(0, 0, 0);
    frame(0, 0, 0);
    assert(C.slide == 100);
    C.slideTo = 40;
    frame(0, 0, 0);
    assert(C.slide == 55);
    frame(0, 0, 0);
    assert(C.slide == 40);
    frame(0, 0, 0);
    assert(C.idle == 4);
    frame(0, 1, 0);
    assert(C.idle == 90);
    /* Modes 10-13: any of the four face buttons goes back. */
    frame(0xf0, 0, 0);
    assert(C.mode == 3 && one_se(4));
}

static void check_grid_and_tabs(void)
{
    /* Mode 0: the card grid. */
    reset(0);
    frame(0x20, 0, 0);
    assert(C.mode == 1 && C.slideTo == 320 && one_se(17));
    reset(0);
    C.gridTop = 70; C.gridRow = 9;
    frame(0, 0, 0x8);       /* page down: 10 rows, clamped to 76 */
    assert(C.gridTop == 76 && one_se(3));
    frame(0, 0, 0x4);       /* page up */
    assert(C.gridTop == 66);
    frame(0, 0, 0x4000);    /* down at the last row scrolls */
    assert(C.gridRow == 9 && C.gridTop == 67);
    frame(0, 0, 0x1000);    /* up moves the row */
    assert(C.gridRow == 8 && C.gridTop == 67);
    C.gridRow = 0; C.gridTop = 0;
    frame(0, 0, 0x1000);
    assert(C.gridTop == 0 && C.gridRow == 0 && se_n == 0);
    frame(0, 0, 0x8000);
    assert(C.gridCol == 9 && one_se(3));
    frame(0, 0, 0x2000);
    assert(C.gridCol == 0);
    frame(0, 0, 0x1);
    assert(C.tabA == 3 && one_se(19));
    frame(0, 0, 0x2);
    assert(C.tabA == 0);
    /* Viewing a card: a monster (below 683) that is owned or was seen. */
    C.gridTop = 60; C.gridRow = 8; C.gridCol = 2;   /* 682 */
    frame(0x40, 0, 0);
    assert(view_id == 682 && push_calls == 1 && one_se(4));
    bag_count = 0; view_id = -1;
    frame(0x40, 0, 0);
    assert(view_id == -1 && one_se(6));
    look_flag = 1;
    frame(0x10, 0, 0);
    assert(view_id == 682);
    C.gridCol = 3; view_id = -1; bag_count = 1;     /* 683: not a monster */
    frame(0x80, 0, 0);
    assert(view_id == -1 && one_se(6));
    C.gridTop = 67; C.gridRow = 0; C.gridCol = 1;   /* 671 is shown as no card */
    frame(0x80, 0, 0);
    assert(view_id == -1 && one_se(6));

    /* Mode 1 and mode 6: the two tab rows. */
    reset(1);
    frame(0x800, 0, 0);
    assert(C.mode == 0 && C.slideTo == 640 && one_se(16));
    reset(1);
    frame(0, 0, 0x4000);
    assert(C.tabA == 1 && one_se(19) && reset_mask == 0xa000);
    frame(0, 0, 0x1000);
    assert(C.tabA == 0);
    frame(0, 0, 0x2000);
    assert(C.mode == 2 && one_se(18));
    C.mode = 1;
    frame(0, 0, 0x20);
    assert(C.mode == 2 && one_se(17));
    reset(6);
    frame(0, 0, 0x1000);
    assert(C.tabB == 3 && one_se(19) && reset_mask == 0xa000);
    frame(0, 0, 0x8000);
    assert(C.mode == 5 && one_se(18));
    C.mode = 6;
    frame(0, 0, 0x20);
    assert(C.mode == 5 && one_se(17));
}

static void check_bag_list(void)
{
    /* Cursor and scrolling. */
    reset(3);
    C.bagTop = 10; C.bagCur = 4;
    frame(0, 0, 0x4000);
    assert(C.bagCur == 4 && C.bagTop == 11 && one_se(3) && C.idle == 90 && reset_mask == 0xa000);
    frame(0, 0, 0x1000);
    assert(C.bagCur == 3 && C.bagTop == 11 && one_se(3));
    frame(0, 0, 0x8);
    assert(C.bagTop == 16);
    C.bagTop = 94;
    frame(0, 0, 0x8);
    assert(C.bagTop == 95);                 /* 100 cards, five rows */
    frame(0, 0, 0x4);
    assert(C.bagTop == 90);
    /* Leader: needs a rank and three copies; replaces the old leader. */
    reset(3);
    C.bagTop = 20; C.bagCur = 2;
    frame(0x800, 0, 0);
    assert(leader_pulled == 1 && leader_pushed == 22 && deck[0] == 22 && ghost_calls == 1);
    assert(ghost[0] == 22 && ghost[1] == 2 && ghost[2] == 1 && text_free == 1 && C.reload == 1 && one_se(4));
    rank = 0; leader_pushed = -1;
    frame(0x800, 0, 0);
    assert(leader_pushed == -1 && one_se(6));
    /* Add to the deck / take out of it. */
    reset(3);
    C.bagTop = 20; C.bagCur = 2;
    frame(0, 0, 0x40);
    assert(pushed_id == 22 && deck[40] == 22 && sort_deck == 1 && ghost[2] == 1 && one_se(4));
    deck[5] = 22;
    frame(0, 0, 0x80);
    assert(pulled_at == 7 && deck[5] == 999 && deck[40] == 22 && sort_deck == 2 && ghost[4] == 1 && one_se(4));
    pull_ok = 0;
    frame(0, 0, 0x80);
    assert(one_se(6) && sort_deck == 2);
    push_ok = 0;
    frame(0, 0, 0x40);
    assert(one_se(6));
    /* Sort keys: the held button picks the second key. */
    reset(3);
    frame(0, 0, 0x2);
    assert(C.bagSort[0] == 1 && C.bagSort[1] == 0 && sort_bag == 1 && one_se(19));
    frame(0, 0x100, 0x1);
    assert(C.bagSort[1] == 9 && sort_bag == 2);
    /* Leaving, viewing, switching panel. */
    reset(3);
    frame(0x20, 0, 0);
    assert(pop_calls == 1 && one_se(5));
    ready[0] = 0;
    frame(0x20, 0, 0);
    assert(pop_calls == 1 && C.mode == 13 && C.prevMode == 3 && se[0] == 6);
    reset(2);
    frame(0x20, 0, 0);
    assert(C.mode == 3 && C.slideTo == 0 && one_se(17));
    reset(2);
    C.bagTop = 682;
    C.bagCount = 854;
    frame(0x10, 0, 0);
    assert(view_id == 682 && push_calls == 1);
    C.bagTop = 683; view_id = -1;
    frame(0x10, 0, 0);
    assert(view_id == -1 && one_se(6));
    reset(3);
    frame(0x10, 0, 0);
    assert(C.mode == 2 && C.slideTo == 320 && one_se(16));
    reset(3);
    C.bagCur = 3;
    frame(0, 0, 0x2000);
    assert(C.mode == 4 && C.deckCur == 3 && one_se(18));
    reset(2);
    frame(0, 0, 0x8000);
    assert(C.mode == 1 && one_se(18));
    /* Recycle and password boxes. */
    reset(3);
    bag_count = 3;
    frame(0x200, 0, 0);
    assert(C.mode == 7 && C.prevMode == 3 && C.sel == 0 && one_se(16));
    reset(3);
    duel5 = 4;
    frame(0x200, 0, 0);
    assert(C.mode == 3 && one_se(6));
    reset(3);
    ready[0] = ready[1] = ready[2] = 0;
    frame(0x200, 0, 0);
    assert(C.mode == 3 && one_se(6));
    reset(3);
    frame(0x400, 0, 0);
    assert(C.mode == 9 && C.prevMode == 3 && memcmp(C.pass, "00000000", 8) == 0 && one_se(16));
}

static void check_deck_list(void)
{
    reset(4);
    C.deckTop = 3; C.deckCur = 4;
    frame(0, 0, 0x4000);
    assert(C.deckTop == 4 && C.deckCur == 4 && one_se(3) && C.idle == 90);
    frame(0, 0, 0x1000);
    assert(C.deckCur == 3 && one_se(3));
    frame(0, 0, 0x8);
    assert(C.deckTop == 9);
    frame(0, 0, 0x8); frame(0, 0, 0x8); frame(0, 0, 0x8); frame(0, 0, 0x8); frame(0, 0, 0x8); frame(0, 0, 0x8);
    assert(C.deckTop == 36);                /* 41 entries, five rows */
    /* Take a card out / add one more of it. */
    reset(4);
    C.deckTop = 3; C.deckCur = 2;
    frame(0, 0, 0x80);
    assert(pulled_at == 7 && deck[5] == 999 && sort_deck == 1 && ghost[0] == 705 && ghost[4] == 1 && one_se(4));
    frame(0, 0, 0x40);
    assert(pushed_id == -1 && one_se(6));   /* an empty entry */
    deck[5] = 705;
    frame(0, 0, 0x40);
    assert(pushed_id == 705 && deck[40] == 705 && one_se(4));
    /* Entry 0 is the leader: only taking it out. */
    reset(4);
    frame(0, 0, 0x80);
    assert(leader_pulled == 1 && deck[0] == 999 && ghost[3] == 1 && text_free == 1 && C.reload == 1 && one_se(4));
    frame(0, 0, 0x80);
    assert(leader_pulled == 1 && one_se(6));
    frame(0, 0, 0x40);
    assert(se_n == 0 && pushed_id == -1);
    /* Next deck: reloads the 41 entries. */
    reset(4);
    current_deck = 2;
    frame(0x800, 0, 0);
    assert(set_deck == 0 && C.deckCount == 41 && deck[0] == 500 && deck[1] == 600 && deck[40] == 639);
    assert(sort_deck == 1 && text_free == 1 && C.reload == 1 && one_se(9));
    /* Sort keys, leaving, viewing, switching. */
    reset(5);
    frame(0, 0x100, 0x2);
    assert(C.deckSort[1] == 1 && C.deckSort[0] == 0 && sort_deck == 1 && one_se(19));
    reset(4);
    current_deck = 1; ready[1] = 0;
    frame(0x20, 0, 0);
    assert(C.mode == 13 && C.prevMode == 4);
    reset(5);
    frame(0x20, 0, 0);
    assert(C.mode == 4 && C.slideTo == 0 && one_se(17));
    reset(5);
    deck[0] = 12;
    frame(0x10, 0, 0);
    assert(view_id == 12 && one_se(4));
    reset(4);
    frame(0x10, 0, 0);
    assert(C.mode == 5 && C.slideTo == -320 && one_se(16));
    reset(4);
    C.deckCur = 2;
    frame(0, 0, 0x8000);
    assert(C.mode == 3 && C.bagCur == 2 && one_se(18));
    reset(5);
    frame(0, 0, 0x2000);
    assert(C.mode == 6 && one_se(18));
}

static void check_boxes(void)
{
    /* Mode 7: yes/no, then three cards come back (mode 8). */
    reset(7);
    C.prevMode = 3;
    frame(0x20, 0, 0);
    assert(C.mode == 3 && one_se(17));
    reset(7);
    C.prevMode = 3;
    frame(0x40, 0, 0);
    assert(C.mode == 3 && one_se(4) && del_id == -1);
    reset(7);
    C.prevMode = 3; C.bagTop = 120; C.bagCur = 3;
    frame(0x8000, 0, 0);
    assert(C.sel == 1 && one_se(3));
    frame(0x80, 0, 0);
    assert(C.mode == 8 && del_id == 123 && work[16] == 300 && work[17] == 301 && work[18] == 302);
    assert(C.step == 0 && C.sel == 0 && one_se(4));
    C.sel = 0x7f;
    frame(0, 0, 0);
    assert(add_n == 1 && add_id[0] == 300 && rebuild == 1 && C.sel == 0x80 && one_se(7));
    frame(0, 0, 0);
    assert(C.sel == 0x80 && se_n == 0);
    frame(0x10, 0, 0);
    assert(C.step == 1 && C.sel == 0 && one_se(4));
    C.step = 2; C.sel = 0x80;
    frame(0x10, 0, 0);
    assert(C.step == 3 && C.sel == 0x80);
    frame(0x10, 0, 0);
    assert(duel5_set == 1 && C.mode == 3 && one_se(4));

    /* Mode 9: eight characters 0-9 a-z, then the password is tried. */
    reset(9);
    C.prevMode = 2;
    memcpy(C.pass, "0000000z", 8);
    frame(0, 0, 0x8000);
    assert(C.sel == 7 && one_se(3));
    frame(0, 0, 0x4000);
    assert(C.pass[7] == 'y');
    frame(0, 0, 0x1000);
    frame(0, 0, 0x1000);
    assert(C.pass[7] == '0');
    frame(0, 0, 0x2000);
    assert(C.sel == 0);
    frame(0, 0, 0x4000);
    assert(C.pass[0] == 'z');
    frame(0x40, 0, 0);
    assert(C.mode == 10 && work[16] == 42 && flag_set == 1 && add_id[0] == 42 && rebuild == 1 && one_se(4));
    C.mode = 9; pass_flag = 1;
    frame(0x40, 0, 0);
    assert(C.mode == 12 && flag_set == 1 && one_se(4));
    C.mode = 9; password = -1;
    frame(0x80, 0, 0);
    assert(C.mode == 11 && one_se(6));
    C.mode = 9;
    frame(0x20, 0, 0);
    assert(C.mode == 2 && one_se(17));
}

static void check_analog(void)
{
    /* Up/down stick scrolls the bag list in modes 1-4, left/right the deck list in 3-6. */
    reset(1);
    analog_on = 1; analog_y = 0xff; C.bagTop = 10; C.tabA = 0;
    frame(0, 0, 0);
    assert(C.bagTop == 19 && one_se(3) && C.idle == 90);     /* (127 - 48) * 15 >> 7 */
    analog_y = 0;
    frame(0, 0, 0);
    assert(C.bagTop == 9);                                   /* (-128 + 48) * 15 >> 7 = -10 */
    analog_y = 0xb0;
    frame(0, 0, 0);
    assert(C.bagTop == 9 && se_n == 0);                      /* inside the dead zone */
    reset(5);
    analog_on = 1; analog_y = 0xff; analog_x = 0xff; C.bagTop = 10; C.deckTop = 10;
    frame(0, 0, 0);
    assert(C.bagTop == 10 && C.deckTop == 13);               /* 79 * 5 >> 7 */
    reset(3);
    analog_on = 1; analog_y = 0; analog_x = 0; C.bagTop = 10; C.deckTop = 10;
    frame(0, 0, 0);
    assert(C.bagTop == 0 && C.deckTop == 6 && se_n == 2);
    reset(0);
    analog_on = 1; analog_y = 0xff; analog_x = 0xff; C.bagTop = 10; C.deckTop = 10;
    frame(0, 0, 0);
    assert(C.bagTop == 10 && C.deckTop == 10);
}

int main(void)
{
    check_preamble();
    check_grid_and_tabs();
    check_bag_list();
    check_deck_list();
    check_boxes();
    check_analog();
    puts("build_deck: ok");
    return 0;
}
