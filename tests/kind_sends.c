/* Synthetic host tests for the kind_sends group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/kind_sends.h"

static int block_words[13];
static int calls;
static int want_a;
static int want_b = 1;
static void (*focus)(int) = TaTuto_FocusUnit__00175ba0;
static int want_packed;
static int sent[2];
static float sent_first;

int SzView_FreeRequest(int a, int b)
{
    assert(a == 0 && b == 0);
    calls++;
    return 0;
}

void *SzDuel_GetUnit(int a, int b)
{
    assert(a == want_a && b == want_b);
    calls++;
    return block_words;
}

int func_001f6160_words(int packed, int *words)
{
    assert(packed == want_packed);
    words[0] = 100;
    words[1] = 200;
    words[2] = 300;
    words[3] = 400;
    calls++;
    return 0;
}

static int seen_words[4];

void SzView_IV2V(int *in, float *out)
{
    memcpy(seen_words, in, sizeof(seen_words));
    out[0] = 1.5f;
    out[1] = 2.0f;
    out[2] = 3.0f;
    out[3] = 4.0f;
    calls++;
}

float SzEffect_GetRadius2(int packed)
{
    assert(packed == want_packed);
    calls++;
    return 0.25f;
}

int SzDuel_GetUnitModelID(int a, int b)
{
    assert(a == want_a && b == want_b);
    calls++;
    return 77;
}

void func_001fa8a0_start(int a, int b, int c, int d, int index, void *data)
{
    assert(a == 0 && b == 0 && d == 0);
    if (c == 7) {
        assert(index == 77 && data == NULL);
        sent[0]++;
    } else {
        assert(c == 4 && index == 0);
        sent_first = ((float *)data)[0];
        sent[1]++;
    }
    calls++;
}

static void run(int kind, int a, int packed, int turn)
{
    calls = 0;
    sent[0] = sent[1] = 0;
    want_a = a;
    want_packed = packed;
    block_words[12] = turn;
    memset(seen_words, 0, sizeof(seen_words));
    focus(kind);
}

static void check_send(void)
{
    /* kind 368: side 0, base 18431; difference stays non-negative at the boundary */
    run(368, 0, 1, 18431);
    assert(calls == 8 && sent[0] == 1 && sent[1] == 1);
    assert(seen_words[0] == 163 && seen_words[1] == 200 && seen_words[2] == 4396 && seen_words[3] == 400);
    assert(sent_first == 1.75f);
    /* negative difference wraps by 65535 */
    run(368, 0, 1, 18432);
    assert(seen_words[1] == 200 + 65534);
    /* kind 29: side 1, packed 257 */
    run(29, 1, 257, 431);
    assert(calls == 8 && seen_words[1] == 18200);
    run(29, 1, 257, 18433);
    assert(seen_words[1] == 200 + 65533);
    /* kind 156: side 0, base 51199 */
    run(156, 0, 1, 51199);
    assert(calls == 8 && seen_words[1] == 200);
    run(156, 0, 1, 51200);
    assert(seen_words[1] == 200 + 65534);
    /* any other kind only makes the first call */
    run(30, 0, 1, 0);
    assert(calls == 1 && sent[0] == 0 && sent[1] == 0);
}

static void run2(void (*f)(int), int kind, int a, int b, int turn)
{
    focus = f;
    want_b = b;
    run(kind, a, a * 256 + b, turn);
    focus = TaTuto_FocusUnit__00175ba0;
    want_b = 1;
}

static void check_case(void (*f)(int), int kind, int a, int b, int base)
{
    /* non-negative difference at the boundary, then the wrap by 65535 */
    run2(f, kind, a, b, base);
    assert(calls == 8 && sent[0] == 1 && sent[1] == 1);
    assert(seen_words[0] == 163 && seen_words[1] == 200 && seen_words[2] == 4396 && seen_words[3] == 400);
    assert(sent_first == 1.75f);
    run2(f, kind, a, b, base + 1);
    assert(calls == 8 && seen_words[1] == 200 + 65534);
}

static void check_focus_variants(void)
{
    check_case(TaTuto_FocusUnit__00172510, 60, 0, 0, 18431);
    check_case(TaTuto_FocusUnit__00172510, 0, 1, 0, 18431);
    check_case(TaTuto_FocusUnit__00172510, 156, 0, 1, 51199);
    check_case(TaTuto_FocusUnit__00172510, 3, 1, 1, 18431);
    run2(TaTuto_FocusUnit__00172510, 58, 0, 0, 0);
    assert(calls == 1 && sent[0] == 0 && sent[1] == 0);
    check_case(TaTuto_FocusUnit__001745f0, 58, 0, 0, 18431);
    check_case(TaTuto_FocusUnit__001745f0, 156, 0, 1, 51199);
    run2(TaTuto_FocusUnit__001745f0, 60, 0, 0, 0);
    assert(calls == 1 && sent[0] == 0 && sent[1] == 0);
}

int main(void)
{
    check_send();
    check_focus_variants();
    return 0;
}
