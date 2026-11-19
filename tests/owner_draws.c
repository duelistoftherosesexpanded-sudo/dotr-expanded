/* Synthetic host tests for the owner_draws group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/owner_draws.h"
#include "dotr/ee/camp_tikei.h"

struct DotrOwnerDrawControl data_00317490;

/* Every stub appends an id and checks the cursor chain: each cursor-taking
 * callee must receive the previous one's result. */
static int calls, ids[32], words[32];
static float xs[2], ys[2];
static int moves;
static struct DotrOwnerDrawBlock *seen;

static DotrOwnerDrawWord step(int id, DotrOwnerDrawWord cursor, int word)
{
    assert(calls < 32);
    ids[calls] = id;
    words[calls] = word;
    calls++;
    return cursor + 1;
}

DotrOwnerDrawWord SzDuel_StartCardSession_Pipe(DotrOwnerDrawWord cursor)
{
    return step(2, cursor, 0);
}

void SzDuel_SetCardDefPrioZ(int value)
{
    step(3, 0, value);
}

/* Fixtures for 0x22a140, which draws three cards with a block it patches differently. */
struct DotrOwnerDeckBank dotr_owner_deck_bank_01ed0000;
struct DotrTikeiCamp data_00299e10_camp;
static int deck_mode, deck_moves;
static float deck_x[3], deck_y[3];

int SzSound_PlaySE(int index, int value)
{
    step(13, 0, index * 100 + value);
    return 0;
}

static int card_test;
static struct DotrOwnerDrawBlock snaps[32];
static float snap_x[32], snap_y[32];

/* 0x1cea50's stubs keep a copy of the block as it is at each call. */
static void card_step(int id, int word, struct DotrOwnerDrawBlock *block, float x, float y)
{
    snaps[calls] = *block;
    snap_x[calls] = x;
    snap_y[calls] = y;
    step(id, 0, word);
}

int SzDuel_GetCardNodeOnScreen(struct DotrOwnerDrawBlock *block, int a, float x, float y)
{
    if (deck_mode) {
        /* initialised by the real 0x1cd560, then the three floats and the word */
        assert(block->unknown_30 == 0.46875f && block->unknown_34 == 0.48913044f);
        assert(block->unknown_38 == 1.0f && block->unknown_3c == 0 && block->unknown_4a == 7);
        assert(a == 0 && deck_moves < 3);
        seen = block;
        deck_x[deck_moves] = x;
        deck_y[deck_moves] = y;
        deck_moves++;
        step(4, 0, 0);
        return 0;
    }
    if (moves == 0) {
        /* The block was initialised by the real 0x1cd560, then patched. */
        seen = block;
        assert(block->flag_14 == 0 && block->flag_12 == -1 && block->flag_15 == -1);
        assert(block->color[3] == 0x5a && block->color[0] == 128 && block->unknown_4a == 7);
    }
    assert(block == seen && a == 0 && moves < 2);
    xs[moves] = x;
    ys[moves] = y;
    moves++;
    step(4, 0, 0);
    return 0;
}

DotrOwnerDrawWord SzDuel_SetupCard2_Pipe(DotrOwnerDrawWord cursor, int a0, struct DotrOwnerDrawBlock *block)
{
    assert(block == seen);
    return step(5, cursor, a0);
}

DotrOwnerDrawWord SzDuel_DrawCard_Pipe(DotrOwnerDrawWord cursor, struct DotrOwnerDrawBlock *block)
{
    assert(block == seen);
    return step(6, cursor, (int)cursor);
}

DotrOwnerDrawWord SzDuel_FinishCardSession_Pipe(DotrOwnerDrawWord cursor)
{
    return step(7, cursor, (int)cursor);
}

/* 0x1b83f0: one assertion per early return (each of the three zero tests)
 * and the full call order with every forwarded argument. */
static void check_pair_draw(void)
{
    static const int order[9] = {2, 3, 4, 5, 6, 4, 5, 6, 7};

    assert(offsetof(struct DotrOwnerDrawControl, first) == 0x6c8);
    assert(offsetof(struct DotrOwnerDrawControl, second) == 0x6cc);
    assert(offsetof(struct DotrOwnerDrawControl, alpha) == 0xd3b);

    data_00317490.first = 0;
    data_00317490.second = 0x22;
    data_00317490.alpha = 0x5a;
    assert(SzDuel_DrawJoinCard_Pipe(100) == 100 && calls == 0);
    data_00317490.first = 0x11;
    data_00317490.second = 0;
    assert(SzDuel_DrawJoinCard_Pipe(100) == 100 && calls == 0);
    data_00317490.second = 0x22;
    data_00317490.alpha = 0;
    assert(SzDuel_DrawJoinCard_Pipe(100) == 100 && calls == 0);

    data_00317490.alpha = 0x5a;
    assert(SzDuel_DrawJoinCard_Pipe(100) == 106);
    assert(calls == 9 && memcmp(ids, order, 9 * sizeof(int)) == 0);
    assert(words[1] == 0xfffffe && words[3] == 0x11 && words[6] == 0x22);
    /* cursor chain: 0x1d9a20 -> 101; 0x1dbc40 -> 102; 0x1dbd30 -> 103; ... 0x1d9e00 -> 106 */
    assert(words[4] == 102 && words[7] == 104 && words[8] == 105);
    assert(moves == 2 && xs[0] == -150.0f && ys[0] == 0.0f && xs[1] == 150.0f && ys[1] == 0.0f);
}

/* 0x1cd560: null returns 0 and stores nothing (nothing to observe); otherwise
 * every stored member is checked against a block pre-filled with the opposite
 * bits, and the members it does not store must keep the fill. */
static void check_block_init(void)
{
    struct DotrOwnerDrawBlock block;
    int i;

    assert(offsetof(struct DotrOwnerDrawBlock, color) == 0x40);
    assert(offsetof(struct DotrOwnerDrawBlock, unknown_50) == 0x50);
    assert(offsetof(struct DotrOwnerDrawBlock, unknown_78) == 0x78);
    assert(offsetof(struct DotrOwnerDrawBlock, unknown_84) == 0x84);
    assert(SzDuel_ClearDrawCard(0) == 0);

    memset(&block, 0xff, sizeof(block));
    block.flag_12 = 0;
    block.flag_14 = 0;
    block.flag_15 = 0;
    assert(SzDuel_ClearDrawCard(&block) == 1);
    assert(block.unknown_00[0] == 0 && block.unknown_00[1] == 0 && block.unknown_00[2] == 0);
    assert(block.unknown_0c == 1.0f && block.unknown_10 == 16384);
    for (i = 0; i < 7; i++) {
        assert(block.unknown_14[i] == 0);
    }
    assert(block.unknown_30 == 0.1f && block.unknown_34 == 0.1f && block.unknown_38 == 1.0f);
    assert(block.unknown_3c == 0);
    for (i = 0; i < 4; i++) {
        assert(block.color[i] == 128 && block.unknown_44[i] == 0);
    }
    assert(block.unknown_48 == 0 && block.unknown_4a == 7 && block.unknown_4c == 0 && block.unknown_50 == 0);
    assert(block.unknown_78 == 0x70002000 && block.unknown_7c == 0x70000000);
    assert(block.unknown_80_bits_0_11 == 170 && block.count == 0 && block.unknown_82_bits_12_13 == 1);
    assert(block.flag_12 == -1 && block.flag_13 == 0 && block.flag_14 == -1 && block.flag_15 == -1);
    assert(block.flag_30 == 0 && block.flag_31 == 0);
    for (i = 0; i < 0x20; i++) {
        assert(block.unknown_58[i] == 0xff);
    }
    for (i = 0; i < 12; i++) {
        assert(block.unknown_84[i] == 0xff);
    }
}

/* Stubs for 0x23bff0. */
static struct DotrOwnerDrawItem *item_seen;
static int item_id, owner_gate, kind_seen, value_seen;

void func_001dcd70_owner(struct DotrOwnerDrawItem *item, int id)
{
    memset(item, 0xee, sizeof(*item));
    item_seen = item;
    item_id = id;
    step(8, 0, id);
}

float func_00205c40_owner(int *out)
{
    seen = (struct DotrOwnerDrawBlock *)((char *)out - offsetof(struct DotrOwnerDrawBlock, unknown_14[3]));
    if (card_test) {
        card_step(9, 0, seen, 0.0f, 0.0f);
        return 0.0f;
    }
    /* initialised by the real 0x1cd560, then the four flags and two floats */
    assert(seen->flag_12 == -1 && seen->flag_13 == -1 && seen->flag_14 == -1 && seen->flag_15 == 0);
    assert(seen->unknown_30 == 0.5f && seen->unknown_34 == 0.5f && seen->unknown_38 == 1.0f);
    assert(seen->unknown_80_bits_0_11 == 170 && seen->unknown_10 == 16384);
    assert(item_seen->kind == kind_seen && item_seen->unknown_00[67] == 0xee && item_seen->unknown_45[0] == 0xee);
    step(9, 0, 0);
    return 0.0f;
}

void func_00205cb0_owner(struct DotrOwnerDrawBlock *block, float x, float y)
{
    assert(block == seen && block->unknown_10 == 0 && block->unknown_14[0] == value_seen);
    assert(block->unknown_14[1] == 0 && block->unknown_14[2] == 0);
    xs[0] = x;
    ys[0] = y;
    step(10, 0, 0);
}

int SzDuel_GetCardNode(struct DotrOwnerDrawBlock *block, int a)
{
    assert(block == seen);
    step(11, 0, a);
    return owner_gate;
}

DotrOwnerDrawWord func_001dbc40_item(DotrOwnerDrawWord cursor, struct DotrOwnerDrawItem *item, struct DotrOwnerDrawBlock *block)
{
    assert(block == seen && (item == 0 || item == item_seen));
    return step(5, cursor, item != 0);
}

DotrOwnerDrawWord func_001d9e70_owner(DotrOwnerDrawWord cursor)
{
    return step(12, cursor, (int)cursor);
}

/* 0x23bff0: the full call order with every forwarded argument and stored
 * member, and both outcomes of the 0x1d9f00 test (item pointer or null). */
static void check_item_draw(void)
{
    static const int order[9] = {8, 9, 2, 3, 10, 11, 5, 6, 12};
    int pass;

    assert(sizeof(struct DotrOwnerDrawItem) == 96);
    assert(offsetof(struct DotrOwnerDrawItem, kind) == 0x44);
    for (pass = 0; pass < 2; pass++) {
        calls = 0;
        owner_gate = pass;
        kind_seen = 0x37;
        value_seen = 0x1234;
        assert(Taku_Camp_Card_Get(100, 0x37, 2.5f, -3.5f, 0x1234) == 104);
        assert(calls == 9 && memcmp(ids, order, 9 * sizeof(int)) == 0);
        assert(item_id == 999 && words[3] == 0xffffff && words[5] == 1);
        assert(words[6] == pass && words[7] == 102 && words[8] == 103);
        assert(xs[0] == 2.5f && ys[0] == -3.5f);
    }
}

/* 0x22a140: one assertion per gate outcome (stage below 10070, above 10080, phase 4),
 * the full call order with every forwarded argument for one pass, every arm of the
 * three position switches (direction 0, 1, other; card 0, 1, 2) and every outcome of
 * the slide steps (step below 2, step 2 with phase 0 or not, step 3 for the last card
 * with phase 3 or not and for the others, each with its bound reached and not reached).
 * Not reachable: phase 3 failing the `phase < 4` test. The switch defaults (a card
 * index outside 0..2) cannot occur in the loop. */
static struct DotrOwnerDeckBank *deck_reset(int step_no, int phase, int dir)
{
    struct DotrOwnerDeckBank *bank = &dotr_owner_deck_bank_01ed0000;

    memset(bank, 0, sizeof(*bank));
    data_00299e10_camp.stage = 10070;
    bank->step = step_no;
    bank->phase = phase;
    bank->dir = dir;
    bank->top = 1;
    bank->frac = 50;
    bank->last = 7;
    bank->cards[0] = 111;
    bank->cards[1] = 222;
    bank->cards[2] = 333;
    calls = 0;
    deck_moves = 0;
    return bank;
}

static void check_deck_cards(void)
{
    static const int order[14] = {2, 3, 4, 8, 5, 6, 4, 8, 5, 6, 4, 8, 5, 6};
    struct DotrOwnerDeckBank *bank;

    assert(offsetof(struct DotrOwnerDeckBank, step) == 0x53a6);
    assert(offsetof(struct DotrOwnerDeckBank, top) == 0x53a8);
    assert(offsetof(struct DotrOwnerDeckBank, cards) == 0x53fe);
    assert(offsetof(struct DotrOwnerDeckBank, phase) == 0x5494);
    assert(offsetof(struct DotrOwnerDeckBank, seen) == 0x5496);
    assert(offsetof(struct DotrOwnerDeckBank, pos) == 0x549a);
    assert(offsetof(struct DotrTikeiCamp, stage) == 194);
    deck_mode = 1;

    bank = deck_reset(0, 0, 0);
    data_00299e10_camp.stage = 10069;
    assert(Taku_DECK_CARD_DISP(100) == 100 && calls == 0);
    data_00299e10_camp.stage = 10081;
    assert(Taku_DECK_CARD_DISP(100) == 100 && calls == 0);
    data_00299e10_camp.stage = 10080;
    bank->phase = 4;
    assert(Taku_DECK_CARD_DISP(100) == 100 && calls == 0 && bank->last == 7);

    /* step 0, direction 0: fixed offsets 36 / 18 / 0, cards (top + 2 - i) % 3 */
    bank = deck_reset(0, 0, 0);
    data_00299e10_camp.stage = 10080;
    bank->pos[0] = 0;
    bank->pos[1] = 50;
    bank->pos[2] = 100;
    assert(Taku_DECK_CARD_DISP(100) == 108);
    assert(calls == 15 && memcmp(ids, order, sizeof(order)) == 0 && ids[14] == 12);
    assert(words[1] == 0xfffff8 && words[3] == 111 && words[7] == 333 && words[11] == 222);
    assert(words[4] == 1 && words[5] == 102 && words[9] == 104 && words[13] == 106 && words[14] == 107);
    assert(deck_x[0] == -614.0f && deck_x[1] == -312.0f && deck_x[2] == -10.0f);
    assert(deck_y[0] == 10.0f && deck_y[1] == 10.0f && deck_y[2] == 10.0f);
    assert(bank->last == 0 && bank->pos[0] == 0 && bank->pos[1] == 50 && bank->pos[2] == 100);
    assert(bank->phase == 0);

    /* direction 1: cards top % 3, (top + 2) % 3, (top + 1) % 3; offsets from frac */
    bank = deck_reset(0, 0, 1);
    bank->pos[0] = bank->pos[1] = bank->pos[2] = 100;
    assert(Taku_DECK_CARD_DISP(100) == 108);
    assert(words[3] == 222 && words[7] == 111 && words[11] == 333);
    assert(deck_x[0] == 8.0f && deck_x[1] == 17.0f && deck_x[2] == -1.0f);

    /* any other direction: the default cards, offsets from frac */
    bank = deck_reset(0, 0, 2);
    bank->pos[0] = bank->pos[1] = bank->pos[2] = 100;
    assert(Taku_DECK_CARD_DISP(100) == 108);
    assert(words[3] == 111 && words[7] == 333 && words[11] == 222);
    assert(deck_x[0] == 8.0f && deck_x[1] == 17.0f && deck_x[2] == -1.0f);

    /* step 2, phase 0: +18; sound once per card above 0; clamp at 100 records the card */
    bank = deck_reset(2, 0, 0);
    bank->pos[0] = -18;
    bank->pos[1] = 90;
    bank->pos[2] = 82;
    bank->seen[2] = 1;
    assert(Taku_DECK_CARD_DISP(100) == 108 && calls == 16);
    assert(ids[10] == 13 && words[10] == 9);
    assert(bank->pos[0] == 0 && bank->pos[1] == 100 && bank->pos[2] == 100);
    assert(bank->seen[0] == 0 && bank->seen[1] == 1 && bank->seen[2] == 1);
    assert(bank->last == 2 && bank->phase == 1);

    /* step 2, phase 0, second card reaching 100 only: no phase change */
    bank = deck_reset(2, 0, 0);
    bank->pos[1] = 90;
    bank->seen[0] = bank->seen[1] = bank->seen[2] = 1;
    assert(Taku_DECK_CARD_DISP(100) == 108 && calls == 15);
    assert(bank->pos[0] == 18 && bank->pos[1] == 100 && bank->pos[2] == 18);
    assert(bank->last == 1 && bank->phase == 0);

    /* step 2, phase not 0: every position becomes 100 */
    bank = deck_reset(2, 1, 0);
    assert(Taku_DECK_CARD_DISP(100) == 108 && calls == 15);
    assert(bank->pos[0] == 100 && bank->pos[1] == 100 && bank->pos[2] == 100);
    assert(bank->last == 7 && bank->phase == 1);

    /* step 3, phase 3: +6 up to 400 for the first two, -6 down to 0 for the last */
    bank = deck_reset(3, 3, 0);
    bank->pos[0] = 390;
    bank->pos[1] = 396;
    bank->pos[2] = 5;
    assert(Taku_DECK_CARD_DISP(100) == 108);
    assert(bank->pos[0] == 396 && bank->pos[1] == 400 && bank->pos[2] == 0 && bank->phase == 4);

    /* step 3, phase 2: the first card reaching 400 sets phase 3; the last then steps down */
    bank = deck_reset(3, 2, 0);
    bank->pos[0] = 399;
    bank->pos[2] = 10;
    assert(Taku_DECK_CARD_DISP(100) == 108);
    assert(bank->pos[0] == 400 && bank->pos[1] == 6 && bank->pos[2] == 4 && bank->phase == 3);

    /* step 3, phase 0: the last card is held at 100 */
    bank = deck_reset(3, 0, 0);
    bank->pos[2] = 7;
    assert(Taku_DECK_CARD_DISP(100) == 108);
    assert(bank->pos[0] == 6 && bank->pos[1] == 6 && bank->pos[2] == 100 && bank->phase == 0);
    assert(bank->last == 7);

    deck_mode = 0;
    calls = 0;
}

/* Stubs and data for 0x1cea50. */
struct DotrOwnerDrawVec data_0031825c;
int data_00318270;
static int pad_gate;

int SzDuel_Xax2New(int value)
{
    step(20, 0, value);
    return value + 1000;
}

int func_001da2b0_card(struct DotrOwnerDrawCardBlock *block, int a, float x, float y)
{
    assert((struct DotrOwnerDrawBlock *)block == seen && a == 0);
    card_step(4, 0, seen, x, y);
    return 0;
}

void func_00205cb0_card(struct DotrOwnerDrawCardBlock *block, float x, float y)
{
    assert((struct DotrOwnerDrawBlock *)block == seen);
    card_step(10, 0, seen, x, y);
}

void SzDuel_StartCardSession(void)
{
    card_step(21, 0, seen, 0.0f, 0.0f);
}

void SzDuel_SetupCard(struct DotrOwnerDrawItem *item, struct DotrOwnerDrawCardBlock *block)
{
    assert((struct DotrOwnerDrawBlock *)block == seen && (item == 0 || item == item_seen));
    card_step(22, item != 0, seen, 0.0f, 0.0f);
}

void SzDuel_DrawCard(struct DotrOwnerDrawCardBlock *block)
{
    assert((struct DotrOwnerDrawBlock *)block == seen);
    card_step(23, 0, seen, 0.0f, 0.0f);
}

int SzPad_Check(int unknown, int zero, int mask)
{
    assert(unknown == 0 && zero == 1 && mask == 0x80);
    step(24, 0, 0);
    return pad_gate;
}

void SzDuel_FinishCardSession(void)
{
    step(25, 0, 0);
}

/* SzDuel_DrawCardTest (branch coverage): the full call order with every forwarded argument, the
 * flag byte and the copied vector as each callee sees them, both outcomes of each
 * 0x1d9f00 test (item pointer or null) and both outcomes of the pad test (the fifth
 * card with flag 14 and the three scale words, or nothing). */
static void check_card_test(void)
{
    static const int order[24] = {20, 8, 9, 21, 4, 22, 23, 4, 22, 23, 10, 11, 22, 23,
                                  10, 11, 22, 23, 24, 10, 11, 22, 23, 25};
    static const int flag_12[4] = {-1, 0, -1, 0};
    static const float px[4] = {-192.0f, 0.0f, 192.0f, 0.0f};
    static const float py[4] = {0.0f, -96.0f, 0.0f, 96.0f};
    int pass, i, at;

    assert(offsetof(struct DotrOwnerDrawCardBlock, look) == 0x20);
    assert(offsetof(struct DotrOwnerDrawCardBlock, unknown_30) == 0x30);
    assert(offsetof(struct DotrOwnerDrawCardBlock, unknown_80) == 0x80);
    card_test = 1;
    for (pass = 0; pass < 2; pass++) {
        calls = 0;
        owner_gate = pass;
        pad_gate = pass;
        data_00318270 = 77;
        data_0031825c.x = 1.5f;
        data_0031825c.y = -2.5f;
        data_0031825c.z = 3.5f;
        data_0031825c.w = 4.5f;
        SzDuel_DrawCardTest();
        if (pass == 0) {
            assert(calls == 20 && memcmp(ids, order, 19 * sizeof(int)) == 0 && ids[19] == 25);
        } else {
            assert(calls == 24 && memcmp(ids, order, 24 * sizeof(int)) == 0);
        }
        assert(words[0] == 77 && item_id == 1077);
        /* at 0x205c40: the block from the real 0x1cd560 with flag 13 set, 14 and 15 clear */
        assert(snaps[2].flag_12 == -1 && snaps[2].flag_13 == -1 && snaps[2].flag_14 == 0 && snaps[2].flag_15 == 0);
        assert(snaps[2].unknown_80_bits_0_11 == 170 && snaps[2].unknown_10 == 16384);
        /* at the session start: the four floats are in place */
        assert(memcmp(&snaps[3].unknown_10, &data_0031825c, 16) == 0);
        for (i = 0; i < 4; i++) {
            at = i < 2 ? 4 + i * 3 : 10 + (i - 2) * 4;
            assert(snaps[at].flag_12 == flag_12[i] && snap_x[at] == px[i] && snap_y[at] == py[i]);
            assert(snaps[at].flag_13 == -1 && snaps[at].flag_14 == 0 && snaps[at].unknown_30 == 0.1f);
        }
        assert(words[5] == 1 && words[8] == 1);
        assert(words[11] == 1 && words[12] == pass && words[15] == 1 && words[16] == pass);
        if (pass == 1) {
            assert(snaps[19].flag_12 == -1 && snaps[19].flag_14 == -1 && snaps[19].flag_15 == 0);
            assert(snaps[19].unknown_30 == 0.46875f && snaps[19].unknown_34 == 0.48913044f);
            assert(snaps[19].unknown_38 == 1.0f && snaps[19].unknown_3c == 0);
            assert(snap_x[19] == 0.0f && snap_y[19] == 96.0f && words[20] == 1 && words[21] == 1);
        }
    }
    card_test = 0;
}

int main(void)
{
    check_block_init();
    check_pair_draw();
    check_item_draw();
    check_deck_cards();
    check_card_test();
    puts("owner_draws: ok");
    return 0;
}
