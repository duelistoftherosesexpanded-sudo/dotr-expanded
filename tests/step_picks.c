/* Synthetic host tests for the step_picks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/step_picks.h"

#define POSITION 0x0305
#define HERE 0x0204
#define LEVEL 77

static int side;
static int record_answer;
static int count_answer[2];
static int count_calls;
static int kind_answer;
static int kind_calls;
static int pick_answer;
static int pick_calls;
static int level_calls;
static int score_answer;
static int score_calls;
static int score_d;
static int score_h;
static int free_answer;
static int free_calls;
static int fin_answer;
static int fin_calls;
static int fin_score;
static int two_calls;
static int pass_answer;
static int pass_calls;
static int last_id;
static int word;
static struct DotrStepBlock *block_seen;
static struct DotrStepOut out;
/* Set for a copy that passes 0x276f10 a literal 0 instead of a level callee's answer. */
static int no_level;
/* Set for a copy without a kind-2 callee. */
static int no_two;

int SzDuel_InqHandPhase(int index)
{
    assert(index == side);
    return record_answer;
}

int TaLogic_GetUnitNum(int a, int b)
{
    assert(a == side && b == count_calls);
    return count_answer[count_calls++];
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    return HERE;
}

int SzDuel_GetUnitMonsterCount(int s)
{
    assert(s == side);
    free_calls++;
    return free_answer;
}

int TaLogic_GetNearCard2(struct DotrStepBlock *block, int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    assert(block == block_seen && a == (HERE & 0xff) && b == (HERE >> 8));
    assert(c == (side == 0 ? 1 : 0) && e == 31 && f == 2 && g == 2);
    if (no_level) {
        assert(i == 0 && level_calls == 0);
    } else {
        assert(i == LEVEL && level_calls == score_calls + 1);
    }
    score_d = d;
    score_h = h;
    score_calls++;
    return score_answer;
}

static int kind_callee(int id, int s, struct DotrStepBlock *block)
{
    assert(s == side && out.position == POSITION);
    last_id = id;
    block_seen = block;
    block->unknown_3c = 0x1234;
    kind_calls++;
    return kind_answer;
}

static int pick_callee(int id, int s, struct DotrStepOut *o, struct DotrStepBlock *block)
{
    assert(id == last_id && s == side && o == &out && block == block_seen);
    assert(out.kind == 10 && out.shouldFlipUp == word);
    pick_calls++;
    return pick_answer;
}

static int level_callee(int id, int s)
{
    assert(id == last_id && s == side);
    level_calls++;
    return LEVEL;
}

static int fin_callee(int id, int s, unsigned char kind, struct DotrStepBlock *block, int score)
{
    assert(id == last_id && s == side && kind == (unsigned char)kind_answer && block == block_seen);
    fin_score = score;
    fin_calls++;
    return fin_answer;
}

static int two_callee(int id, int s, struct DotrStepOut *o, struct DotrStepBlock *block)
{
    assert(id == last_id && s == side && o == &out && block == block_seen);
    two_calls++;
    return 0;
}

static int pass_callee(int id, int s, struct DotrStepOut *o, struct DotrStepBlock *block, int item)
{
    assert(id == last_id && s == side && o == &out && block == block_seen && item == 0x1234);
    pass_calls++;
    return pass_answer;
}

int func_00114310_step(int s, struct DotrStepBlock *b) { return kind_callee(1, s, b); }
int func_00114d40_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(1, s, o, b); }
int func_00117c30_step(int s) { return level_callee(1, s); }
int func_00115340_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(1, s, k, b, v); }
int func_00115160_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(1, s, o, b); }
int func_00114b40_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(1, s, o, b, v); }
int func_001280c0_step(int s, struct DotrStepBlock *b) { return kind_callee(2, s, b); }
int func_00128720_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(2, s, o, b); }
int func_0012b1b0_step(int s) { return level_callee(2, s); }
int func_00128d40_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(2, s, k, b, v); }
int func_00128b50_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(2, s, o, b); }
int func_00128520_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(2, s, o, b, v); }
int func_001326f0_step(int s, struct DotrStepBlock *b) { return kind_callee(3, s, b); }
int func_00132d00_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(3, s, o, b); }
int func_00134ae0_step(int s) { return level_callee(3, s); }
int func_00133320_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(3, s, k, b, v); }
int func_00133130_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(3, s, o, b); }
int func_00132b00_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(3, s, o, b, v); }
int func_00164680_step(int s, struct DotrStepBlock *b) { return kind_callee(4, s, b); }
int func_00164ce0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(4, s, o, b); }
int func_001676d0_step(int s) { return level_callee(4, s); }
int func_001652f0_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(4, s, k, b, v); }
int func_00165100_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(4, s, o, b); }
int func_00164ae0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(4, s, o, b, v); }
int func_00168a30_step(int s, struct DotrStepBlock *b) { return kind_callee(5, s, b); }
int func_00169000_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(5, s, o, b); }
int func_0016b0e0_step(int s) { return level_callee(5, s); }
int func_00169610_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(5, s, k, b, v); }
int func_00169420_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(5, s, o, b); }
int func_00168e00_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(5, s, o, b, v); }
int func_0014ce30_step(int s, struct DotrStepBlock *b) { return kind_callee(6, s, b); }
int func_0014d3e0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(6, s, o, b); }
int func_0014f9b0_step(int s) { return level_callee(6, s); }
int func_0014da40_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(6, s, k, b, v); }
int func_0014d800_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(6, s, o, b); }
int func_0014d1e0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(6, s, o, b, v); }
int func_0015a5f0_step(int s, struct DotrStepBlock *b) { return kind_callee(7, s, b); }
int func_0015ab10_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(7, s, o, b); }
int func_0015d120_step(int s) { return level_callee(7, s); }
int func_0015af30_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(7, s, k, b, v); }
int func_0015ad00_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(7, s, o, b); }
int func_0015a910_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(7, s, o, b, v); }
int func_0016f760_step(int s, struct DotrStepBlock *b) { return kind_callee(8, s, b); }
int func_0016fef0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(8, s, o, b); }
int func_00172310_step(int s) { return level_callee(8, s); }
int func_00170620_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(8, s, k, b, v); }
int func_001703e0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(8, s, o, b); }
int func_0016fcf0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(8, s, o, b, v); }
int func_0013ae00_step(int s, struct DotrStepBlock *b) { return kind_callee(9, s, b); }
int func_0013b420_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(9, s, o, b); }
int func_0013da70_step(int s) { return level_callee(9, s); }
int func_0013ba30_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(9, s, k, b, v); }
int func_0013b840_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(9, s, o, b); }
int func_0013b220_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(9, s, o, b, v); }
int func_00155a00_step(int s, struct DotrStepBlock *b) { return kind_callee(10, s, b); }
int func_001564e0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(10, s, o, b); }
int func_00158780_step(int s) { return level_callee(10, s); }
int func_00156b40_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(10, s, k, b, v); }
int func_00156910_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(10, s, o, b); }
int func_001562e0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(10, s, o, b, v); }
/* The kind-3 callees of 0x155770 and 0x15f300. */
static int three_calls;
static int three_callee(int id, int s, struct DotrStepOut *o, struct DotrStepBlock *b)
{
    assert(id == last_id && s == side && o == &out && b == block_seen);
    three_calls++;
    return 0;
}
int func_00155c90_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return three_callee(10, s, o, b); }
int func_0015f5e0_step(int s, struct DotrStepBlock *b) { return kind_callee(11, s, b); }
int func_0015fe50_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(11, s, o, b); }
int func_00162580_step(int s) { return level_callee(11, s); }
int func_001604b0_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(11, s, k, b, v); }
int func_00160270_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(11, s, o, b); }
int func_0015fc50_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(11, s, o, b, v); }
int func_0015fa00_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return three_callee(11, s, o, b); }
int func_001482f0_step(int s, struct DotrStepBlock *b) { return kind_callee(12, s, b); }
int func_00148b60_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(12, s, o, b); }
int func_0014b330_step(int s) { return level_callee(12, s); }
int func_001491b0_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(12, s, k, b, v); }
int func_00148f80_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(12, s, o, b); }
int func_00148960_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(12, s, o, b, v); }
int func_00148630_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return three_callee(12, s, o, b); }
int func_00150f10_step(int s, struct DotrStepBlock *b) { return kind_callee(13, s, b); }
int func_001516a0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(13, s, o, b); }
int func_00153ed0_step(int s) { return level_callee(13, s); }
int func_00151cf0_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(13, s, k, b, v); }
int func_00151ac0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(13, s, o, b); }
int func_001514a0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(13, s, o, b, v); }
int func_00151170_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return three_callee(13, s, o, b); }

/* 0x11dfb0's callees. Its finishing callee gets no score: the stub reports the
 * last 0x276f10 answer so that the shared cases run; their fin_score asserts
 * say nothing about this copy. Kinds 2 and 3 share 0x11f3c0, told apart by the
 * mode argument. */
int func_0011e6e0_step(int s, struct DotrStepBlock *b) { return kind_callee(14, s, b); }
int func_0011ef60_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(14, s, o, b); }
int func_0011fa80_step(int s) { return level_callee(14, s); }
int func_0011e270_step(int s, unsigned char k, struct DotrStepBlock *b) { return fin_callee(14, s, k, b, score_answer); }
int func_0011f3c0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int mode)
{
    assert(mode == 0 || mode == 1);
    return mode ? three_callee(14, s, o, b) : two_callee(14, s, o, b);
}
int func_0011ed50_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(14, s, o, b, v); }

/* 0x122d30's callees; 0x124c90 serves kinds 2 and 3 by mode like 0x11f3c0. */
int func_00123740_step(int s, struct DotrStepBlock *b) { return kind_callee(15, s, b); }
int func_00124810_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(15, s, o, b); }
int func_001254a0_step(int s) { return level_callee(15, s); }
int func_00123010_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(15, s, k, b, v); }
int func_00124c90_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int mode)
{
    assert(mode == 0 || mode == 1);
    return mode ? three_callee(15, s, o, b) : two_callee(15, s, o, b);
}
int func_00124610_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(15, s, o, b, v); }
/* Set for a copy whose finish after the pass callee gets -1, not the score. */
static int pass_score_lost;

/* 0x13eee0's callees. After the pass callee its finish gets the low byte of
 * the pass answer as the kind; the stub checks that and hands on the kind the
 * shared asserts expect. */
int func_0013f170_step(int s, struct DotrStepBlock *b) { return kind_callee(16, s, b); }
int func_0013f850_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(16, s, o, b); }
int func_00141b30_step(int s) { return level_callee(16, s); }
int func_0013feb0_step(int s, unsigned char k, struct DotrStepBlock *b, int v)
{
    if (pass_calls) {
        assert(k == (unsigned char)pass_answer && v == -1);
        k = kind_answer;
    }
    return fin_callee(16, s, k, b, v);
}
int func_0013fcc0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(16, s, o, b); }
int func_0013f3b0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return three_callee(16, s, o, b); }
int func_0013f640_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(16, s, o, b, v); }

/* 0x143ba0's callees; its finish is checked like 0x13eee0's. */
int func_00143de0_step(int s, struct DotrStepBlock *b) { return kind_callee(17, s, b); }
int func_00144450_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(17, s, o, b); }
int func_001461c0_step(int s) { return level_callee(17, s); }
int func_00144a60_step(int s, unsigned char k, struct DotrStepBlock *b, int v)
{
    if (pass_calls) {
        assert(k == (unsigned char)pass_answer && v == -1);
        k = kind_answer;
    }
    return fin_callee(17, s, k, b, v);
}
int func_00144860_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(17, s, o, b); }
int func_00144250_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(17, s, o, b, v); }

/* 0x10dd50's callees; its pass callee takes no item. */
static int pass3_callee(int id, int s, struct DotrStepOut *o, struct DotrStepBlock *block)
{
    assert(id == last_id && s == side && o == &out && block == block_seen);
    pass_calls++;
    return pass_answer;
}
int func_0010dfc0_step(int s, struct DotrStepBlock *b) { return kind_callee(18, s, b); }
int func_0010e320_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(18, s, o, b); }
int func_0010eda0_step(int s) { return level_callee(18, s); }
int func_0010e890_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(18, s, k, b, v); }
int func_0010e540_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int mode)
{
    assert(mode == 0);
    return two_callee(18, s, o, b);
}
int func_0010f450_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pass3_callee(18, s, o, b); }

/* 0x136210's callees; it has no level callee. */
int func_00136490_step(int s, struct DotrStepBlock *b) { return kind_callee(19, s, b); }
int func_00136b80_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(19, s, o, b); }
int func_00137120_step(int s, unsigned char k, struct DotrStepBlock *b, int v) { return fin_callee(19, s, k, b, v); }
int func_00136f30_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(19, s, o, b); }
int func_00136980_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(19, s, o, b, v); }

/* 0x119300's callees. Its finish gets no score (stub as for 0x11dfb0). */
int func_00119a40_step(int s, struct DotrStepBlock *b) { return kind_callee(20, s, b); }
int func_0011a0d0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(20, s, o, b); }
int func_0011aa10_step(int s) { return level_callee(20, s); }
int func_00119520_step(int s, unsigned char k, struct DotrStepBlock *b) { return fin_callee(20, s, k, b, score_answer); }
int func_00119d30_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(20, s, o, b, v); }

/* 0x12d270's callees. The kind callee gets the fourth argument; the finish
 * after the pass callee gets the low byte of the pass answer as the kind. */
#define LIMIT 0x77
int func_0012d520_step(int s, struct DotrStepBlock *b, int limit)
{
    assert(limit == LIMIT);
    return kind_callee(21, s, b);
}
int func_0012dde0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return pick_callee(21, s, o, b); }
int func_00130780_step(int s) { return level_callee(21, s); }
int func_0012e3f0_step(int s, unsigned char k, struct DotrStepBlock *b, int v)
{
    if (pass_calls) {
        assert(k == (unsigned char)pass_answer);
        k = kind_answer;
    }
    return fin_callee(21, s, k, b, v);
}
int func_0012e1f0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b) { return two_callee(21, s, o, b); }
int func_0012dbe0_step(int s, struct DotrStepOut *o, struct DotrStepBlock *b, int v) { return pass_callee(21, s, o, b, v); }

typedef int (*Step)(int position, int side, struct DotrStepOut *out);

static int step_0012d270(int position, int s, struct DotrStepOut *o) { return TaLogic_GetMinimumTask_Summon__0012d270(position, s, o, LIMIT); }
/* Set for a copy that takes the score again after an accepted kind-0 pick. */
static int score_twice;

static int run(Step f, int s, int kind, int pick, int score, int spare, int fin, int pass)
{
    memset(&out, 0xee, sizeof(out));
    side = s;
    record_answer = 1;
    count_answer[0] = 5, count_answer[1] = 4;
    kind_answer = kind, pick_answer = pick, score_answer = score;
    free_answer = spare, fin_answer = fin, pass_answer = pass;
    count_calls = kind_calls = pick_calls = level_calls = score_calls = 0;
    free_calls = fin_calls = two_calls = pass_calls = 0;
    last_id = 0;
    return f(POSITION, s, &out);
}

/* Branch coverage: one assertion per branch outcome and per boundary of N; w is the word
 * each copy stores at +0xc. */
static void check_step(Step f, int id, int n, int a, int b, int w)
{
    word = w;
    /* 0x1de6d0 answers 0: only the position is stored. */
    memset(&out, 0xee, sizeof(out));
    side = 1, record_answer = 0, count_calls = kind_calls = 0;
    assert(f(POSITION, 1, &out) == -1);
    assert(out.position == POSITION && out.kind == 0xee && count_calls == 0 && kind_calls == 0);
    /* Both counts 5. */
    record_answer = 1, count_answer[0] = count_answer[1] = 5;
    assert(f(POSITION, 1, &out) == -1);
    assert(count_calls == 2 && kind_calls == 0);
    /* First count not 5: the second is not asked. Negative kind. */
    count_calls = 0, count_answer[0] = 4, kind_answer = -1;
    assert(f(POSITION, 1, &out) == -1);
    assert(count_calls == 1 && kind_calls == 1 && last_id == id && out.kind == 0xee);

    /* Kind 0, pick accepted, score 0: straight to the finish. */
    assert(run(f, 1, 0, 0, 0, 0, 0x4321, 0) == 1);
    assert(count_calls == 2 && last_id == id && pick_calls == 1 && score_calls == 1);
    assert(score_d == a && score_h == b && free_calls == 0);
    assert(fin_calls == 1 && fin_score == 0 && pass_calls == 0 && two_calls == 0);
    assert(out.position == POSITION && out.kind == 10 && out.loc == 0x4321 && out.shouldFlipUp == w);
    assert(out.unknown_03 == 0xee && out.unknown_06 == 0xeeee && out.unknown_08 == (int)0xeeeeeeee);
    /* The same with side 0 and a finish answer of 0, then a negative one. */
    assert(run(f, 0, 0, 0, 5, 0, 0, 0) == 1);
    assert(fin_calls == 1 && fin_score == 5 && out.loc == 0);
    assert(run(f, 0, 0, 0, 5, 0, -1, 0) == -1);
    assert(fin_calls == 1 && out.loc == 0xeeee && out.kind == 10);

    /* Kind 0, negative score, 0x1e0150 below N: the finish with that score. */
    assert(run(f, 1, 0, 0, -3, n - 1, 9, -1) == 1);
    assert(free_calls == 1 && fin_calls == 1 && fin_score == -3 && pass_calls == 0 && out.loc == 9);
    assert(run(f, 1, 0, 0, -3, n - 1, -1, -1) == -1);
    assert(fin_calls == 1 && out.loc == 0xeeee);
    /* At N: the pass callee decides. */
    assert(run(f, 1, 0, 0, -3, n, 9, -1) == -1);
    assert(free_calls == 1 && pass_calls == 1 && fin_calls == 0);
    assert(run(f, 1, 0, 0, -3, n, 9, 0) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == (pass_score_lost ? -1 : -3) && out.loc == 9);

    /* Kind 0, pick refused: no score; the pass callee's refusal ends it. (Its
     * acceptance would hand the finish a score that was never assigned, so
     * that outcome is not run here.) */
    assert(run(f, 1, 0, -1, 0, 0, 9, -1) == -1);
    assert(pick_calls == 1 && score_calls == 0 && level_calls == 0 && pass_calls == 1 && fin_calls == 0);

    /* Kind 1: no pick callee; score, pass, finish. */
    assert(run(f, 1, 1, -1, 6, 0, 8, 0) == 1);
    assert(pick_calls == 0 && score_calls == 1 && score_d == a && score_h == b && free_calls == 0);
    assert(pass_calls == 1 && two_calls == 0 && fin_calls == 1 && fin_score == (pass_score_lost ? -1 : 6) && out.loc == 8);
    assert(run(f, 1, 1, -1, 6, 0, -1, 0) == -1);
    assert(pass_calls == 1 && fin_calls == 1);
    assert(run(f, 1, 1, -1, 6, 0, 8, -1) == -1);
    assert(pass_calls == 1 && fin_calls == 0);
    if (no_two) {
        /* Kind 2 through the pass callee like kind 1. */
        assert(run(f, 0, 2, -1, 7, 0, 8, -1) == -1);
        assert(score_calls == 1 && two_calls == 0 && pass_calls == 1 && fin_calls == 0);
        assert(run(f, 0, 2, -1, 7, 0, 8, 0) == 1);
        assert(two_calls == 0 && pass_calls == 1 && fin_calls == 1 && out.loc == 8);
        return;
    }
    /* Kind 2: the kind-2 callee instead of the pass callee. */
    assert(run(f, 0, 2, -1, 7, 0, 8, -1) == 1);
    assert(pick_calls == 0 && score_calls == 1 && two_calls == 1 && pass_calls == 0);
    assert(fin_calls == 1 && fin_score == 7 && out.loc == 8);
    assert(run(f, 0, 2, -1, 7, 0, -1, -1) == -1);
    assert(two_calls == 1 && fin_calls == 1 && out.loc == 0xeeee);
}

/* 0x155770 and 0x13eee0, branch coverage: the guards and kind 0 as in check_step, except
 * that the score is taken before the kind is tested; kinds 1..4. n is the 0x1e0150 bound;
 * lost is set for the copies whose finish after the pass callee gets -1 as the score;
 * has_three is 0 for the copy without a kind-3 callee. */
static void check_step_early(Step f, int id, int n, int lost, int has_three)
{
    word = 0;
    memset(&out, 0xee, sizeof(out));
    side = 1, record_answer = 0, count_calls = kind_calls = 0;
    assert(f(POSITION, 1, &out) == -1);
    assert(out.position == POSITION && out.kind == 0xee && count_calls == 0 && kind_calls == 0);
    record_answer = 1, count_answer[0] = count_answer[1] = 5;
    assert(f(POSITION, 1, &out) == -1);
    assert(count_calls == 2 && kind_calls == 0);
    count_calls = 0, count_answer[0] = 4, kind_answer = -1;
    assert(f(POSITION, 1, &out) == -1);
    assert(count_calls == 1 && kind_calls == 1 && last_id == id && out.kind == 0xee);

    /* Kind 0, pick accepted, score 0; side 0; finish answers 0 and -1. */
    three_calls = 0;
    assert(run(f, 1, 0, 0, 0, 0, 0x4321, 0) == 1);
    assert(count_calls == 2 && last_id == id && pick_calls == 1 && score_calls == 1 + score_twice);
    assert(level_calls == score_calls);
    assert(score_d == 3 && score_h == 0 && free_calls == 0);
    assert(fin_calls == 1 && fin_score == 0 && pass_calls == 0 && two_calls == 0);
    assert(out.position == POSITION && out.kind == 10 && out.loc == 0x4321 && out.shouldFlipUp == 0);
    assert(out.unknown_03 == 0xee && out.unknown_06 == 0xeeee && out.unknown_08 == (int)0xeeeeeeee);
    assert(run(f, 0, 0, 0, 5, 0, 0, 0) == 1);
    assert(fin_calls == 1 && fin_score == 5 && out.loc == 0);
    assert(run(f, 0, 0, 0, 5, 0, -1, 0) == -1);
    assert(fin_calls == 1 && out.loc == 0xeeee && out.kind == 10);
    /* Negative score: 0x1e0150 answers n - 1 and n. */
    assert(run(f, 1, 0, 0, -3, n - 1, 9, -1) == 1);
    assert(free_calls == 1 && fin_calls == 1 && fin_score == -3 && pass_calls == 0 && out.loc == 9);
    assert(run(f, 1, 0, 0, -3, n - 1, -1, -1) == -1);
    assert(fin_calls == 1 && out.loc == 0xeeee);
    assert(run(f, 1, 0, 0, -3, n, 9, -1) == -1);
    assert(free_calls == 1 && pass_calls == 1 && fin_calls == 0);
    assert(run(f, 1, 0, 0, -3, n, 9, 0) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == (lost ? -1 : -3) && out.loc == 9);
    /* Pick refused: the score was already taken; the pass callee decides. */
    assert(run(f, 1, 0, -1, 4, 0, 9, -1) == -1);
    assert(pick_calls == 1 && score_calls == 1 && free_calls == 0 && pass_calls == 1 && fin_calls == 0);
    assert(run(f, 1, 0, -1, 4, 0, 9, 0) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == (lost ? -1 : 4) && out.loc == 9);

    /* Kinds 1 and 4: score, pass, finish. */
    assert(run(f, 1, 1, -1, 6, 0, 8, 0) == 1);
    assert(pick_calls == 0 && score_calls == 1 && score_d == 3 && score_h == 0 && free_calls == 0);
    assert(pass_calls == 1 && two_calls == 0 && fin_calls == 1 && fin_score == (lost ? -1 : 6) && out.loc == 8);
    assert(run(f, 1, 1, -1, 6, 0, -1, 0) == -1);
    assert(pass_calls == 1 && fin_calls == 1);
    assert(run(f, 1, 1, -1, 6, 0, 8, -1) == -1);
    assert(pass_calls == 1 && fin_calls == 0);
    assert(run(f, 1, 4, -1, 6, 0, 8, 0) == 1);
    assert(pass_calls == 1 && two_calls == 0 && three_calls == 0 && fin_calls == 1);
    /* Kind 2: the kind-2 callee instead of the pass callee. */
    assert(run(f, 0, 2, -1, 7, 0, 8, -1) == 1);
    assert(pick_calls == 0 && score_calls == 1 && two_calls == 1 && pass_calls == 0 && three_calls == 0);
    assert(fin_calls == 1 && fin_score == 7 && out.loc == 8);
    assert(run(f, 0, 2, -1, 7, 0, -1, -1) == -1);
    assert(two_calls == 1 && fin_calls == 1 && out.loc == 0xeeee);
    if (!has_three) {
        /* Kind 3 through the pass callee like kind 4. */
        assert(run(f, 0, 3, -1, 7, 0, 8, -1) == -1);
        assert(three_calls == 0 && pass_calls == 1 && fin_calls == 0);
        assert(run(f, 0, 3, -1, 7, 0, 8, 0) == 1);
        assert(three_calls == 0 && pass_calls == 1 && fin_calls == 1 && out.loc == 8);
        return;
    }
    /* Kind 3: its own callee. */
    assert(run(f, 0, 3, -1, 7, 0, 8, -1) == 1);
    assert(pick_calls == 0 && score_calls == 1 && three_calls == 1 && two_calls == 0 && pass_calls == 0);
    assert(fin_calls == 1 && fin_score == 7 && out.loc == 8);
    assert(run(f, 0, 3, -1, 7, 0, -1, -1) == -1);
    assert(three_calls == 2 && fin_calls == 1 && out.loc == 0xeeee);
}

/* The common shape, then kind 3 with its own callee and kind 4 through the pass callee. */
static void check_three(Step f, int id, int n)
{
    check_step(f, id, n, 3, 0, 0);
    three_calls = 0;
    assert(run(f, 0, 3, -1, 7, 0, 8, -1) == 1);
    assert(pick_calls == 0 && score_calls == 1 && three_calls == 1 && two_calls == 0 && pass_calls == 0);
    assert(fin_calls == 1 && fin_score == 7 && out.loc == 8);
    assert(run(f, 0, 3, -1, 7, 0, -1, -1) == -1);
    assert(three_calls == 2 && fin_calls == 1 && out.loc == 0xeeee);
    assert(run(f, 1, 4, -1, 6, 0, 8, 0) == 1);
    assert(pass_calls == 1 && three_calls == 2 && two_calls == 0 && fin_calls == 1 && out.loc == 8);
}

/* 0x10dd50 and its copies, branch coverage: the guards, a kind callee that refuses
 * only with -1, kind 0 as in check_step with bound n, kind 1 through the pass
 * callee, and a kind 2 that stores 0 at +4 without the finish. */
static void check_step_plain(Step f, int id, int n)
{
    word = 0;
    memset(&out, 0xee, sizeof(out));
    side = 1, record_answer = 0, count_calls = kind_calls = 0;
    assert(f(POSITION, 1, &out) == -1);
    assert(out.position == POSITION && out.kind == 0xee && count_calls == 0 && kind_calls == 0);
    record_answer = 1, count_answer[0] = count_answer[1] = 5;
    assert(f(POSITION, 1, &out) == -1);
    assert(count_calls == 2 && kind_calls == 0);
    count_calls = 0, count_answer[0] = 4, kind_answer = -1;
    assert(f(POSITION, 1, &out) == -1);
    assert(count_calls == 1 && kind_calls == 1 && last_id == id && out.kind == 0xee);
    /* A kind of -2 is not a refusal: score, pass callee, finish. */
    assert(run(f, 1, -2, -1, 6, 0, 8, 0) == 1);
    assert(pick_calls == 0 && score_calls == 1 && pass_calls == 1 && fin_calls == 1 && out.kind == 10);

    /* Kind 0, pick accepted, score 0; side 0; finish answers 0 and -1. */
    assert(run(f, 1, 0, 0, 0, 0, 0x4321, 0) == 1);
    assert(count_calls == 2 && last_id == id && pick_calls == 1 && score_calls == 1);
    assert(score_d == 3 && score_h == 0 && free_calls == 0);
    assert(fin_calls == 1 && fin_score == 0 && pass_calls == 0 && two_calls == 0);
    assert(out.position == POSITION && out.kind == 10 && out.loc == 0x4321 && out.shouldFlipUp == 0);
    assert(out.unknown_03 == 0xee && out.unknown_06 == 0xeeee && out.unknown_08 == (int)0xeeeeeeee);
    assert(run(f, 0, 0, 0, 5, 0, 0, 0) == 1);
    assert(fin_calls == 1 && fin_score == 5 && out.loc == 0);
    assert(run(f, 0, 0, 0, 5, 0, -1, 0) == -1);
    assert(fin_calls == 1 && out.loc == 0xeeee && out.kind == 10);
    /* Negative score: 0x1e0150 answers n - 1 and n. */
    assert(run(f, 1, 0, 0, -3, n - 1, 9, -1) == 1);
    assert(free_calls == 1 && fin_calls == 1 && fin_score == -3 && pass_calls == 0 && out.loc == 9);
    assert(run(f, 1, 0, 0, -3, n - 1, -1, -1) == -1);
    assert(fin_calls == 1 && out.loc == 0xeeee);
    assert(run(f, 1, 0, 0, -3, n, 9, -1) == -1);
    assert(free_calls == 1 && pass_calls == 1 && fin_calls == 0);
    assert(run(f, 1, 0, 0, -3, n, 9, 0) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == -3 && out.loc == 9);
    /* Pick refused: no score; the pass callee's refusal ends it. (Its acceptance
     * would hand the finish a score that was never assigned: not run.) */
    assert(run(f, 1, 0, -1, 0, 0, 9, -1) == -1);
    assert(pick_calls == 1 && score_calls == 0 && level_calls == 0 && pass_calls == 1 && fin_calls == 0);

    /* Kind 1: score, pass, finish. */
    assert(run(f, 1, 1, -1, 6, 0, 8, 0) == 1);
    assert(pick_calls == 0 && score_calls == 1 && free_calls == 0);
    assert(pass_calls == 1 && two_calls == 0 && fin_calls == 1 && fin_score == 6 && out.loc == 8);
    assert(run(f, 1, 1, -1, 6, 0, -1, 0) == -1);
    assert(pass_calls == 1 && fin_calls == 1);
    assert(run(f, 1, 1, -1, 6, 0, 8, -1) == -1);
    assert(pass_calls == 1 && fin_calls == 0);
    /* Kind 2: its callee, 0 at +4, no finish. */
    assert(run(f, 0, 2, -1, 7, 0, 8, -1) == 1);
    assert(score_calls == 1 && two_calls == 1 && pass_calls == 0 && fin_calls == 0 && out.loc == 0);
}

int main(void)
{
    check_step(TaLogic_GetMinimumTask_Summon__00114080, 1, 4, 3, 0, 0);
    check_step(TaLogic_GetMinimumTask_Summon__00127e30, 2, 5, 3, 0, 0);
    check_step(TaLogic_GetMinimumTask_Summon__00132460, 3, 5, 3, 0, 0);
    check_step(TaLogic_GetMinimumTask_Summon__001643f0, 4, 6, 2, 1, 0);
    check_step(TaLogic_GetMinimumTask_Summon__001687a0, 5, 6, 2, 1, 0);
    check_step(TaLogic_GetMinimumTask_Summon__0014cba0, 6, 6, 3, 0, 2);
    check_step(TaLogic_GetMinimumTask_Summon__0015a360, 7, 6, 3, 0, 2);
    check_step(TaLogic_GetMinimumTask_Summon__0016f4d0, 8, 5, 3, 0, 2);
    check_step(TaLogic_GetMinimumTask_Summon__0013ab70, 9, 6, 2, 0, 0);
    check_step_early(TaLogic_GetMinimumTask_Summon__00155770, 10, 6, 0, 1);
    /* 0x13eee0: bound 4; a pass answer of 0x105 reaches the finish as kind 5 (checked in the stub). */
    check_step_early(TaLogic_GetMinimumTask_Summon__0013eee0, 16, 4, 1, 1);
    assert(run(TaLogic_GetMinimumTask_Summon__0013eee0, 1, 1, -1, 6, 0, 8, 0x105) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == -1 && out.loc == 8);
    /* 0x143ba0: the same without a kind-3 callee (has_three 0). */
    check_step_early(TaLogic_GetMinimumTask_Summon__00143ba0, 17, 4, 1, 0);
    assert(run(TaLogic_GetMinimumTask_Summon__00143ba0, 1, 1, -1, 6, 0, 8, 0x105) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == -1 && out.loc == 8);
    /* 0x15f300: the common shape, then kind 3 with its own callee and kind 4 through the pass callee. */
    check_step(TaLogic_GetMinimumTask_Summon__0015f300, 11, 5, 3, 0, 2);
    three_calls = 0;
    assert(run(TaLogic_GetMinimumTask_Summon__0015f300, 0, 3, -1, 7, 0, 8, -1) == 1);
    assert(pick_calls == 0 && score_calls == 1 && three_calls == 1 && two_calls == 0 && pass_calls == 0);
    assert(fin_calls == 1 && fin_score == 7 && out.loc == 8);
    assert(run(TaLogic_GetMinimumTask_Summon__0015f300, 0, 3, -1, 7, 0, -1, -1) == -1);
    assert(three_calls == 2 && fin_calls == 1 && out.loc == 0xeeee);
    assert(run(TaLogic_GetMinimumTask_Summon__0015f300, 1, 4, -1, 6, 0, 8, 0) == 1);
    assert(pass_calls == 1 && three_calls == 2 && two_calls == 0 && fin_calls == 1 && out.loc == 8);
    /* 0x148020 and 0x150c40: the 0x15f300 cases with word 0 and bounds 4 and 6. */
    check_three(TaLogic_GetMinimumTask_Summon__00148020, 12, 4);
    check_three(TaLogic_GetMinimumTask_Summon__00150c40, 13, 6);
    /* 0x11dfb0: the same cases with bound 4; modes 0 and 1 of 0x11f3c0 stand for kinds 2 and 3. */
    check_three(TaLogic_GetMinimumTask_Summon__0011dfb0, 14, 4);
    /* 0x122d30: bound 4, 0x276f10 arguments 2 and 0, -1 to the finish after the pass callee. */
    pass_score_lost = 1;
    check_step(TaLogic_GetMinimumTask_Summon__00122d30, 15, 4, 2, 0, 0);
    three_calls = 0;
    assert(run(TaLogic_GetMinimumTask_Summon__00122d30, 0, 3, -1, 7, 0, 8, -1) == 1);
    assert(three_calls == 1 && two_calls == 0 && pass_calls == 0 && fin_calls == 1 && fin_score == 7);
    assert(run(TaLogic_GetMinimumTask_Summon__00122d30, 0, 3, -1, 7, 0, -1, -1) == -1);
    assert(three_calls == 2 && fin_calls == 1 && out.loc == 0xeeee);
    assert(run(TaLogic_GetMinimumTask_Summon__00122d30, 1, 4, -1, 6, 0, 8, 0) == 1);
    assert(pass_calls == 1 && three_calls == 2 && fin_calls == 1 && fin_score == -1 && out.loc == 8);
    pass_score_lost = 0;
    check_step_plain(TaLogic_GetMinimumTask_Summon__0010dd50, 18, 6);
    /* 0x136210: bound 4, 0x276f10 arguments 3 and 1, literal 0 for the level. */
    no_level = 1;
    check_step(TaLogic_GetMinimumTask_Summon__00136210, 19, 4, 3, 1, 0);
    no_level = 0;
    /* 0x119300: bound 6, no kind-2 callee; the finish stub reports the score itself. */
    no_two = 1;
    check_step(TaLogic_GetMinimumTask_Summon__00119300, 20, 6, 3, 0, 0);
    no_two = 0;
    /* 0x12d270: the 0x143ba0 cases with bound 6, the score kept for the finish after the pass
       callee, and the score taken a second time after an accepted kind-0 pick. */
    score_twice = 1;
    check_step_early(step_0012d270, 21, 6, 0, 0);
    assert(run(step_0012d270, 1, 0, -1, 4, 0, 9, -1) == -1 && score_calls == 1 && level_calls == 1);
    assert(run(step_0012d270, 1, 1, -1, 6, 0, 8, 0x105) == 1);
    assert(pass_calls == 1 && fin_calls == 1 && fin_score == 6 && out.loc == 8);
    score_twice = 0;
    return 0;
}
