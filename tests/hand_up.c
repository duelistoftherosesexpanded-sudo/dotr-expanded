/* Synthetic host tests for the hand_up group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/hand_up.h"

struct DotrOwnerDrawControl data_00317490;
/* Byte view of the block; offsets are from its start. */
#define BYTES ((unsigned char *)&data_00317490)

struct Call {
    char kind;
    int a[7];
};
static struct Call calls[32];
static int call_count;
static int inq_values[5];
static int hand_counts[2];
static int hand_count_calls;
static int deck_count;
static int destiny_result;
static int destiny_calls;
static int random_value;
static int deck_card;

static void record(char kind, int a, int b, int c, int d, int e, int f, int g)
{
    struct Call *call = &calls[call_count++];
    assert(call_count <= 32);
    call->kind = kind;
    call->a[0] = a; call->a[1] = b; call->a[2] = c; call->a[3] = d;
    call->a[4] = e; call->a[5] = f; call->a[6] = g;
}

static void expect(int n, char kind, int a, int b, int c, int d, int e, int f, int g)
{
    assert(n < call_count);
    assert(calls[n].kind == kind);
    assert(calls[n].a[0] == a && calls[n].a[1] == b && calls[n].a[2] == c);
    assert(calls[n].a[3] == d && calls[n].a[4] == e && calls[n].a[5] == f);
    assert(calls[n].a[6] == g);
}

void SzDuel_PackHand(int index) { record('P', index, 0, 0, 0, 0, 0, 0); }
int lSzDuel_InitHandInfo(int a, int b, int c, int d, int e, int f, int g)
{
    record('I', a, b, c, d, e, f, g);
    return 0;
}
int SzDuel_InqHand(int index, int position)
{
    record('Q', index, position, 0, 0, 0, 0, 0);
    return inq_values[position];
}
int SzDuel_GetHandCount(int index)
{
    assert(index == 1);
    assert(hand_count_calls < 2);
    return hand_counts[hand_count_calls++];
}
int SzDuel_GetDeckCount(int index)
{
    assert(index == 1);
    return deck_count;
}
int Ta_GetDestinyDraw(int index, unsigned short *first, short *out, int *note)
{
    assert(index == 1 && note == NULL);
    destiny_calls++;
    first[0] = 11; first[1] = 22; first[2] = 33;
    out[0] = 30; out[1] = 40;
    return destiny_result;
}
int SzDuel_DrawDeck(int index)
{
    record('D', index, 0, 0, 0, 0, 0, 0);
    return deck_card;
}
int SzDuel_AddHand(int index, int value)
{
    record('A', index, value, 0, 0, 0, 0, 0);
    return 0;
}
int func_0019a5c8(void) { return random_value; }
int func_001a22c0(float value) { return (int)value; }

static void reset(void)
{
    memset(&data_00317490, 0, sizeof(data_00317490));
    memset(BYTES + 3372, 0xee, 8);
    call_count = 0;
    hand_count_calls = 0;
    destiny_calls = 0;
    destiny_result = 0;
    deck_count = 0;
    random_value = 0;
    deck_card = 0x77;
}

/* draw == 0: occupied slots are re-read (negative becomes 999, otherwise the low 12 bits),
 * the rest are cleared; the stored count bounds the slot-number list. */
static void check_no_draw(void)
{
    reset();
    BYTES[3401] = 2;
    inq_values[0] = -1;
    inq_values[1] = 0x1234;
    hand_counts[0] = 3;
    lSzDuel_InitHandUp(1, 0);
    assert(call_count == 9);
    expect(0, 'P', 1, 0, 0, 0, 0, 0, 0);
    expect(1, 'I', 1, 0, 999, 0, 0, 0, 0);
    expect(2, 'Q', 1, 0, 0, 0, 0, 0, 0);
    expect(3, 'I', 1, 1, 999, -250, 448, -250, -8);
    expect(4, 'Q', 1, 1, 0, 0, 0, 0, 0);
    expect(5, 'I', 1, 2, 0x234, -125, 448, -125, -8);
    expect(6, 'I', 1, 3, 999, 0, 0, 0, 0);
    expect(7, 'I', 1, 4, 999, 0, 0, 0, 0);
    expect(8, 'I', 1, 5, 999, 0, 0, 0, 0);
    assert(hand_count_calls == 1);
    assert(BYTES[3402] == 3);
    assert(BYTES[3372] == 1 && BYTES[3373] == 2);
    assert(BYTES[3374] == 3 && BYTES[3375] == 0xee);

    /* Five occupied slots: neither fill loop runs; a zero final count writes no list. */
    reset();
    BYTES[3401] = 5;
    inq_values[4] = 0x1234;
    hand_counts[0] = 0;
    lSzDuel_InitHandUp(1, 0);
    assert(call_count == 12);
    expect(11, 'I', 1, 5, 0x234, 250, 448, 250, -8);
    assert(BYTES[3402] == 0 && BYTES[3372] == 0xee);
}

/* draw != 0 without the destiny flag: slots up to min(hand + deck, 5) draw from the deck. */
static void check_draw(void)
{
    /* Empty deck, flag set: the deck test fails first; one drawn slot, four cleared. */
    reset();
    BYTES[848 + 836] = 1;
    hand_counts[0] = 1;
    hand_counts[1] = 1;
    lSzDuel_InitHandUp(1, 1);
    assert(call_count == 13 && destiny_calls == 0);
    expect(2, 'D', 1, 0, 0, 0, 0, 0, 0);
    expect(3, 'A', 1, 0x77, 0, 0, 0, 0, 0);
    expect(4, 'I', 1, 1, 0x77, 750, 448, 750, -8);
    expect(5, 'A', 1, 999, 0, 0, 0, 0, 0);
    expect(6, 'I', 1, 2, 999, 0, 0, 0, 0);
    expect(12, 'I', 1, 5, 999, 0, 0, 0, 0);
    assert(BYTES[3402] == 1 && BYTES[3372] == 1);
    assert(BYTES[3373] == 0xee);

    /* Flag byte zero, and flag byte with bit 7: no destiny slot. Sum above five is capped. */
    reset();
    deck_count = 9;
    hand_counts[0] = 4;
    hand_counts[1] = 5;
    lSzDuel_InitHandUp(1, 1);
    assert(call_count == 17 && destiny_calls == 0);
    expect(14, 'D', 1, 0, 0, 0, 0, 0, 0);
    expect(16, 'I', 1, 5, 0x77, 750, 448, 750, -8);
    reset();
    BYTES[848 + 836] = 0x81;
    deck_count = 1;
    hand_counts[0] = 0;
    hand_counts[1] = 1;
    lSzDuel_InitHandUp(1, 1);
    assert(call_count == 13 && destiny_calls == 0);
    /* The other player's flag is not read. */
    reset();
    BYTES[836] = 1;
    deck_count = 1;
    hand_counts[0] = 0;
    hand_counts[1] = 1;
    lSzDuel_InitHandUp(1, 1);
    assert(call_count == 13 && destiny_calls == 0);
}

static void run_destiny(int random, int result)
{
    reset();
    BYTES[848 + 836] = 1;
    BYTES[3401] = 2;
    inq_values[0] = 1;
    inq_values[1] = 2;
    deck_count = 2;
    hand_counts[0] = 1;
    hand_counts[1] = 4;
    random_value = random;
    destiny_result = result;
    lSzDuel_InitHandUp(1, 1);
    assert(destiny_calls == 1);
    /* Slot 3 is not the last one: plain deck draw. */
    expect(6, 'D', 1, 0, 0, 0, 0, 0, 0);
    expect(7, 'A', 1, 0x77, 0, 0, 0, 0, 0);
    expect(8, 'I', 1, 3, 0x77, 750, 448, 750, -8);
}

/* Destiny flag: one more slot (3 + 1 here), whose card comes from Ta_GetDestinyDraw's three
 * cards by a percentage roll against its two rates (30, 40 in the stub). */
static void check_destiny(void)
{
    run_destiny(0, 0); /* roll 0 < 30 */
    assert(call_count == 13);
    expect(9, 'A', 1, 11, 0, 0, 0, 0, 0);
    expect(10, 'I', 1, 4, 11, 750, 448, 750, -8);
    expect(11, 'A', 1, 999, 0, 0, 0, 0, 0);
    expect(12, 'I', 1, 5, 999, 0, 0, 0, 0);
    assert(BYTES[3402] == 4 && BYTES[3375] == 4);
    assert(BYTES[3376] == 0xee);
    run_destiny(0x26666680, 0); /* roll 30: not below the first rate */
    expect(9, 'A', 1, 22, 0, 0, 0, 0, 0);
    run_destiny(0x40000000, 0); /* roll 50 < 70 */
    expect(9, 'A', 1, 22, 0, 0, 0, 0, 0);
    run_destiny(0x7fffff00, 0); /* roll 99 */
    expect(9, 'A', 1, 33, 0, 0, 0, 0, 0);
    run_destiny(0, -1); /* no destiny card: deck draw */
    assert(call_count == 14);
    expect(9, 'D', 1, 0, 0, 0, 0, 0, 0);
    expect(10, 'A', 1, 0x77, 0, 0, 0, 0, 0);

    /* Four plus one would exceed five: the limit stays five and slot 5 is the destiny slot. */
    reset();
    BYTES[848 + 836] = 1;
    BYTES[3401] = 4;
    deck_count = 3;
    hand_counts[0] = 4;
    hand_counts[1] = 5;
    lSzDuel_InitHandUp(1, 1);
    assert(destiny_calls == 1 && call_count == 12);
    expect(10, 'A', 1, 11, 0, 0, 0, 0, 0);
    expect(11, 'I', 1, 5, 11, 750, 448, 750, -8);
}

int main(void)
{
    check_no_draw();
    check_draw();
    check_destiny();
    puts("hand_up: ok");
    return 0;
}
