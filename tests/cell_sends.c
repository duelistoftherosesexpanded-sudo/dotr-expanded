/* Synthetic host tests for the cell_sends group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_sends.h"

/* Every callee is a stub. The log records each call in order. */
static int ready_v, bit_v, count_v, bit_calls, count_calls;
static char log_buf[64];
static int log_n;
static int *in_seen;
static float *out_seen;
static struct DotrPresetVector *at_seen;
static int tag_kind[4], tag_e[4], tag_n;
static void *tag_data[4];
static float tag_first[4];
/* Which of the word sources ran last: the low 16 bits of the callee's address. */
static int words_from, value_from;

static void note(char c)
{
    assert(log_n < 63);
    log_buf[log_n++] = c;
    log_buf[log_n] = 0;
}

int SzView_FreeRequest(int a, int b)
{
    assert(a == 0 && b == 0);
    note('i');
    return 0;
}

int SzEffect_IsModelStandby2(int packed)
{
    assert(packed == 0x0305 || packed == 0x0300);
    note('r');
    return ready_v;
}

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 3 && (b == 5 || b == 0) && bit == 4);
    bit_calls++;
    return bit_v;
}

int SzDuel_GetUnitCtlRateM(int a, int b)
{
    assert(a == 3 && (b == 5 || b == 0));
    count_calls++;
    return count_v;
}

void TaView_SetExzModelFrontView(int *out, int packed, int add)
{
    assert((packed == 0x0305 || packed == 0x0300) && add == 0x77);
    in_seen = out;
    out[0] = 21;
    words_from = 0x1380;
    note('w');
}

void TaView_SetModelFrontView(int *out, int packed, int add)
{
    TaView_SetExzModelFrontView(out, packed, add);
    words_from = 0x12e0;
}

void TaView_SetModelBackView(int *out, int packed, int add)
{
    TaView_SetExzModelFrontView(out, packed, add);
    words_from = 0x1420;
}

void Ta_GetStdFocusView(int *out, int value)
{
    assert(value == 0x66);
    in_seen = out;
    out[0] = 22;
    value_from = 0x5f00;
    note('v');
}

void Ta_GetStdFocusView2(int *out, int value)
{
    Ta_GetStdFocusView(out, value);
    value_from = 0x5f70;
}

void SzView_IV2V(int *in, float *out)
{
    assert(in == in_seen && (in[0] == 21 || in[0] == 22));
    out[0] = (float)in[0];
    out_seen = out;
    note('c');
}

float SzEffect_GetRadius2(int packed)
{
    assert(packed == 0x0305 || packed == 0x0300);
    note('f');
    return 0.5f;
}

int SzDuel_GetUnitModelID(int a, int b)
{
    assert(a == 3 && (b == 5 || b == 0));
    note('x');
    return 0x44;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == 3 && (b == 5 || b == 0));
    note('p');
    return 0x0807;
}

void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrPresetVector *v)
{
    assert(x == 7 && y == 8 && z == 0);
    at_seen = v;
    note('d');
}

void func_001fa8a0_tag(int a, int b, int kind, unsigned short d, int e, void *data)
{
    assert(a == 0 && b == 0 && d == 0xab && tag_n < 4);
    tag_kind[tag_n] = kind, tag_e[tag_n] = e, tag_data[tag_n] = data;
    tag_first[tag_n] = data == (void *)out_seen ? out_seen[0] : 0.0f;
    tag_n++;
    note('t');
}

static void (*sender)(unsigned char who, int packed, int add, int value);

static void send(int packed)
{
    log_n = tag_n = bit_calls = count_calls = words_from = value_from = 0;
    in_seen = NULL, out_seen = NULL, at_seen = NULL;
    sender(0xab, packed, 0x77, 0x66);
}

static void expect_cell_pair(int n)
{
    assert(tag_kind[n] == 3 && tag_e[n] == 0 && tag_data[n] == (void *)at_seen);
    assert(tag_kind[n + 1] == 4 && tag_e[n + 1] == 0 && tag_data[n + 1] == (void *)out_seen);
    assert(tag_first[n + 1] == 22.0f);
}

static void expect_words(int words, int asked)
{
    assert(strcmp(log_buf, "irwcfxtt") == 0 && tag_n == 2 && bit_calls == 1 && count_calls == asked);
    assert(words_from == words && value_from == 0);
    assert(tag_kind[0] == 7 && tag_e[0] == 0x44 && tag_data[0] == NULL);
    assert(tag_kind[1] == 4 && tag_e[1] == 0 && tag_data[1] == (void *)out_seen && tag_first[1] == 21.5f);
}

static void expect_cells(int value, int twice, int asked)
{
    assert(strcmp(log_buf, twice ? "irpdvcttpdvctt" : "irpdvctt") == 0 && tag_n == (twice ? 4 : 2));
    assert(bit_calls == 1 && count_calls == asked && words_from == 0 && value_from == value);
    expect_cell_pair(0);
    if (twice) {
        expect_cell_pair(2);
    }
}

/* One assertion per branch outcome. `asks` is whether the function asks 0x1e1d10,
 * `twice` whether a ready cell that fails the questions gets the cell pair twice. */
static void check_cell_send(void (*f)(unsigned char, int, int, int), int words, int value, int asks, int twice)
{
    sender = f;
    /* not ready: the cell pair once, nothing asked about the cell */
    ready_v = 0, bit_v = 1, count_v = 1;
    send(0x0305);
    assert(strcmp(log_buf, "irpdvctt") == 0 && tag_n == 2 && bit_calls == 0 && count_calls == 0);
    assert(value_from == value && words_from == 0);
    expect_cell_pair(0);
    /* ready, bit set, positive count: the words plus 0x1f5860, kinds 7 and 4 */
    ready_v = 1;
    send(0x0305);
    expect_words(words, asks);
    /* count of 0 */
    count_v = 0;
    send(0x0305);
    if (asks) {
        expect_cells(value, twice, 1);
    } else {
        expect_words(words, 0);
    }
    /* bit clear with a non-zero low byte: the count is not asked */
    bit_v = 0, count_v = 1;
    send(0x0305);
    expect_cells(value, twice, 0);
    /* bit clear with a low byte of 0: the count decides where it is asked */
    send(0x0300);
    expect_words(words, asks);
    count_v = -1;
    send(0x0300);
    if (asks) {
        expect_cells(value, twice, 1);
    } else {
        expect_words(words, 0);
    }
}

int main(void)
{
    check_cell_send(TaView_ReqExzFocusCamera, 0x1380, 0x5f00, 1, 1);
    check_cell_send(TaView_ReqFocusCamera, 0x12e0, 0x5f00, 1, 1);
    check_cell_send(TaView_ReqFocusCamera3, 0x12e0, 0x5f70, 1, 0);
    check_cell_send(TaView_ReqBackFocusCamera, 0x1420, 0x5f00, 0, 0);
    return 0;
}
