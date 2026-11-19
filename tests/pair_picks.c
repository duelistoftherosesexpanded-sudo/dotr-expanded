/* Synthetic host tests for the pair_picks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/pair_picks.h"

static int list_count, word_zero_at, bit_set_at, missing_at;
static int pair_calls, pair_modes[8], pair_answers[2];
static struct DotrMiscCellPair *pair_seen[8];
static unsigned short handed[22];
static int handed_calls, bit_arg, set_calls;
static struct DotrMiscEntry16 *set_seen;

int TaLogic_GetUnitCtlLstLen(unsigned short *words) { (void)words; return list_count; }
int func_001df540_word(int a, int b) { return ((a << 8) | b) == word_zero_at ? 0 : 1; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { bit_arg = bit; return ((a << 8) | b) == bit_set_at; }
int TaLogic_GetActionUnitFromOwnerID(struct DotrMiscEntry16 *p, int value)
{
    set_seen = p;
    set_calls++;
    return value == missing_at ? -1 : (value & 0xff);
}
int TaLogic_IsAtkNowToGoal(struct DotrMiscCellPair *pair, unsigned char mode)
{
    pair_seen[pair_calls & 7] = pair;
    pair_modes[pair_calls++ & 7] = mode;
    return pair_answers[mode];
}
int TaLogic_GetMaxApUnit(unsigned short *words)
{
    memcpy(handed, words, sizeof handed);
    handed_calls++;
    return 77;
}

static struct DotrMiscEntry16 set[8];
static unsigned short words[4];

static void reset(void)
{
    memset(set, 0, sizeof set);
    list_count = 0;
    word_zero_at = bit_set_at = missing_at = -1;
    pair_calls = handed_calls = set_calls = 0;
    pair_answers[0] = pair_answers[1] = 0;
    bit_arg = -1;
}

/* The halfword at offset 4 of an entry: its high byte and its low byte. */
static void set_kind(int index, int high, int low)
{
    ((struct DotrMiscCellPair *)&set[index])->unknown_02[1] = (unsigned short)((high << 8) | low);
}

int main(void)
{
    int i;

    /* Empty list: nothing asked, -1. */
    reset();
    assert(TaLogic_GetAtkUnitNow(9, words, set) == -1);
    assert(set_calls == 0 && pair_calls == 0 && handed_calls == 0);

    /* One entry rejected at each gate in turn: no word, bit set, not in the set. */
    words[0] = 0x0301;
    reset(); list_count = 1; word_zero_at = 0x0301; set_kind(1, 2, 2); pair_answers[1] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == -1 && set_calls == 0);
    reset(); list_count = 1; bit_set_at = 0x0301; set_kind(1, 2, 2); pair_answers[1] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == -1 && set_calls == 0 && bit_arg == 0);
    reset(); list_count = 1; missing_at = 0x0301; set_kind(1, 2, 2); pair_answers[1] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == -1 && set_calls == 1 && pair_calls == 0 && set_seen == set);

    /* High byte 2 and the mode-1 test passes: picked without the mode-0 test. */
    reset(); list_count = 1; set_kind(1, 2, 2); pair_answers[1] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == 77);
    assert(pair_calls == 1 && pair_modes[0] == 1 && pair_seen[0] == (struct DotrMiscCellPair *)&set[1]);
    assert(handed_calls == 1 && handed[0] == 0x0301);
    for (i = 1; i < 22; i++) assert(handed[i] == 0xffff);

    /* High byte 2, mode-1 test fails, low byte 2 and the mode-0 test passes. */
    reset(); list_count = 1; set_kind(1, 2, 2); pair_answers[0] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == 77);
    assert(pair_calls == 2 && pair_modes[0] == 1 && pair_modes[1] == 0 && handed[0] == 0x0301);

    /* Both tests fail: -1. */
    reset(); list_count = 1; set_kind(1, 2, 2);
    assert(TaLogic_GetAtkUnitNow(0, words, set) == -1 && pair_calls == 2 && handed_calls == 0);

    /* High byte not 2: only the mode-0 test is asked; low byte not 2: neither. */
    reset(); list_count = 1; set_kind(1, 3, 2); pair_answers[0] = pair_answers[1] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == 77 && pair_calls == 1 && pair_modes[0] == 0);
    reset(); list_count = 1; set_kind(1, 3, 3); pair_answers[0] = pair_answers[1] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == -1 && pair_calls == 0);
    reset(); list_count = 1; set_kind(1, 2, 3); pair_answers[0] = 1;
    assert(TaLogic_GetAtkUnitNow(0, words, set) == -1 && pair_calls == 1 && pair_modes[0] == 1);

    /* Three entries, the middle one rejected: the survivors keep their order and
     * the count bounds the walk (the fourth word is not read). */
    words[0] = 0x0102; words[1] = 0x0203; words[2] = 0x0304; words[3] = 0x0405;
    reset(); list_count = 3; bit_set_at = 0x0203; pair_answers[1] = 1;
    set_kind(2, 2, 0); set_kind(3, 2, 0); set_kind(4, 2, 0); set_kind(5, 2, 0);
    assert(TaLogic_GetAtkUnitNow(0, words, set) == 77);
    assert(handed[0] == 0x0102 && handed[1] == 0x0304 && handed[2] == 0xffff);
    assert(set_calls == 2 && pair_calls == 2);
    return 0;
}
