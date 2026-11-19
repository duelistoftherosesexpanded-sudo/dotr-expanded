/* Synthetic host tests for the kind_applies group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/kind_applies.h"

unsigned short gAnmPat_Bakura[768];
unsigned char EnchantId[768];
unsigned char EnchMatch[400 * 7];
static int log_[64], log_count, allow, base_total, e30_result, card_no;
static unsigned words[8][2];
static void put(int v) { log_[log_count++] = v; }
static int by_side, unit_kind[4], fusion_answer, side_answer;
int SzDuel_GetUnitCardNo(int a, int b) { put(0x103); put(a); put(b); return card_no + (by_side ? a * 16 : 0); }
int SzDuel_GetUnitKind(int a, int b) { put(0x700); put(a); put(b); return unit_kind[a]; }
int Fusion(int a, int b) { put(0x5a0); put(a); put(b); return fusion_answer; }
int Ta_GetCurrentSide(int a, int b) { put(0xe70de); put(a); put(b); return side_answer; }
int Ta_GetEnchantOfs(void) { put(0xea0); return base_total; }
int Do_ChkEncMatch(unsigned a, int b, int c) { put(0x370); put((int)a); put(b); put(c); return allow; }
int Ta_GetForceEffection(int a, int b) { put(0xe30); put(a); put(b); return e30_result; }
int TaUtil_CalcUnitAP(int a, int b, int op, int val) { put(0xca0); put(a); put(b); put(op); put(val); return 0; }
int TaUtil_CalcUnitDP(int a, int b, int op, int val) { put(0xe80); put(a); put(b); put(op); put(val); return 0; }
int SzDuel_CalcLP(int index, int operation, int value) { put(0x490); put(index); put(operation); put(value); return 0; }
unsigned *SzDuel_GetUnitEffect(int a, int b, int n) { put(0xb70); put(a); put(b); put(n); return words[n]; }
void Ta_SetUnitCurseRest(int a, int b, unsigned char n) { put(0x3b0); put(a); put(b); put(n); }
int SzDuel_SetUnitSpellRest(int a, int b, int value) { put(0x5e0); put(a); put(b); put(value); return 0; }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { put(0xe70); put(a); put(b); put(bit); put(value); return 0; }

/* Runs one call and checks the log after the fixed 8-word prefix (0x1e1030, 0x248ea0, 0x248370). */
static int run(int kind, int val, int c, int d, const int *want, int n)
{
    int r;

    EnchantId[5] = (unsigned char)kind;
    gAnmPat_Bakura[5] = (unsigned short)val;
    memset(words, 0xee, sizeof(words));
    log_count = 0;
    r = Enchant(1, 2, c, d);
    assert(log_[0] == 0x103 && log_[1] == 1 && log_[2] == 2 && log_[3] == 0xea0);
    assert(log_[4] == 0x370 && log_[5] == 1 && log_[6] == 2 && log_[7] == 5);
    assert(log_count == 8 + n && memcmp(log_ + 8, want, n * sizeof(int)) == 0);
    return r;
}

static void check_apply_kind(void)
{
    const int hi = 0x10000 | 0x4005; /* c >> 16 != 0; id = c & 0x3fff = 5 */
    const int lo = 0x4005;

    base_total = 100;
    e30_result = 77;

    /* 0x248370 returns 0: result -1, nothing else. */
    allow = 0;
    assert(run(0, 0, lo, 0, 0, 0) == -1);
    allow = 1;

    /* kind 4 and kinds above it: c returned, no further call. */
    assert(run(4, 0, lo, 0, 0, 0) == lo);
    assert(run(5, 0, lo, 0, 0, 0) == lo);

    /* kind 0, value 0: Ta_GetForceEffection(d & 0xff, total), op from c >> 16, then SzDuel_CalcLP(d
     * & 0xff == 0, 1, total). */
    {
        static const int w1[] = {0xe30, 0, 100, 0xca0, 1, 2, 1, 77, 0xe80, 1, 2, 1, 77, 0x490, 1, 1, 77};
        static const int w0[] = {0xe30, 3, 100, 0xca0, 1, 2, 0, 77, 0xe80, 1, 2, 0, 77, 0x490, 0, 1, 77};
        assert(run(0, 0, hi, 0x100, w1, 17) == hi);
        assert(run(0, 0, lo, 0x103, w0, 17) == lo);
    }
    /* kind 0, value != 0: total + value, op from c >> 16. */
    {
        static const int w1[] = {0xca0, 1, 2, 1, 109, 0xe80, 1, 2, 1, 109};
        static const int w0[] = {0xca0, 1, 2, 0, 109, 0xe80, 1, 2, 0, 109};
        assert(run(0, 9, hi, 0, w1, 10) == hi);
        assert(run(0, 9, lo, 0, w0, 10) == lo);
    }
    /* kind 1: values 1..5 write word pairs; 0 and 6 do nothing. */
    {
        static const int w1[] = {0xb70, 1, 2, 4, 0xb70, 1, 2, 3};
        static const int w0[] = {0xb70, 1, 2, 0};
        static const int w4[] = {0xb70, 1, 2, 4};
        assert(run(1, 1, lo, 0, w1, 8) == lo);
        assert(words[4][0] == 5211 && words[4][1] == 0 && words[3][0] == 14428 && words[3][1] == 0x1f0129);
        assert(run(1, 2, lo, 0, w0, 4) == lo && words[0][0] == 1152 && words[0][1] == 0);
        assert(run(1, 3, lo, 0, w0, 4) == lo && words[0][0] == 1159 && words[0][1] == 0);
        assert(run(1, 4, lo, 0, w0, 4) == lo && words[0][0] == 1153 && words[0][1] == 0);
        assert(run(1, 5, lo, 0, w4, 4) == lo && words[4][0] == 765 && words[4][1] == 0x10009);
        assert(run(1, 0, lo, 0, 0, 0) == lo);
        assert(run(1, 6, lo, 0, 0, 0) == lo);
    }
    /* kind 2 and kind 3. */
    {
        static const int w2[] = {0xca0, 1, 2, 4, 0, 0xe80, 1, 2, 4, 0, 0x3b0, 1, 2, 0, 0x5e0, 1, 2, 0};
        static const int w3[] = {0x3b0, 1, 2, 255, 0xe70, 1, 2, 0, 1};
        assert(run(2, 7, lo, 0, w2, 18) == lo);
        assert(run(3, 7, lo, 0, w3, 9) == lo);
    }
}

/* 0x249320: kinds 2 and 3, kind 4 with and without the value 9, and the other kinds. */
static void check_is_enc_do(void)
{
    int kind;

    for (kind = 0; kind < 6; kind++) {
        EnchantId[8] = (unsigned char)kind;
        gAnmPat_Bakura[8] = 0;
        assert(TaEnc_IsEncDo(8) == (kind == 2 || kind == 3 ? -1 : 1));
        gAnmPat_Bakura[8] = 9;
        assert(TaEnc_IsEncDo(8) == (kind >= 2 && kind <= 4 ? -1 : 1));
    }
    EnchantId[8] = 4;
    gAnmPat_Bakura[8] = 0x109; /* the whole halfword is compared */
    assert(TaEnc_IsEncDo(8) == 1);
}

static int random_answer, random_calls, elf_answer, elf_calls;
int func_0019a5c8(void) { random_calls++; return random_answer; }
int TaEnc_GetRandomElf(void) { elf_calls++; return elf_answer; }

/* Ta_GetHandEnchant3 (branch coverage): every outer kind, every inner card number and both
 * inner defaults; the remainders are checked with a negative random value as well. */
static void check_hand_enchant3(void)
{
    gAnmPat_Bakura[3] = 6;
    assert(Ta_GetHandEnchant3(99, 3) == 273);
    gAnmPat_Bakura[3] = 7;
    assert(Ta_GetHandEnchant3(99, 3) == 427);
    gAnmPat_Bakura[3] = 8;
    assert(Ta_GetHandEnchant3(342, 3) == 488 && Ta_GetHandEnchant3(7, 3) == 501);
    assert(Ta_GetHandEnchant3(8, 3) == -1);
    gAnmPat_Bakura[3] = 5;
    assert(Ta_GetHandEnchant3(342, 3) == 0x10156);
    gAnmPat_Bakura[3] = 10;
    assert(Ta_GetHandEnchant3(16, 3) == 0x10010 && random_calls == 0 && elf_calls == 0);
    gAnmPat_Bakura[3] = 9;
    random_answer = 23;
    assert(Ta_GetHandEnchant3(16, 3) == 16 && Ta_GetHandEnchant3(17, 3) == 16 && random_calls == 2);
    assert(Ta_GetHandEnchant3(167, 3) == 217 && random_calls == 3);
    random_answer = 71;
    assert(Ta_GetHandEnchant3(332, 3) == 108 && random_calls == 4);
    random_answer = -39;
    assert(Ta_GetHandEnchant3(16, 3) == 9 && Ta_GetHandEnchant3(167, 3) == 200);
    assert(Ta_GetHandEnchant3(332, 3) == 94);
    random_answer = -64;
    assert(Ta_GetHandEnchant3(332, 3) == 101 && random_calls == 8 && elf_calls == 0);
    elf_answer = 0x12345;
    assert(Ta_GetHandEnchant3(379, 3) == 0x2345 && elf_calls == 1 && random_calls == 8);
    assert(Ta_GetHandEnchant3(15, 3) == -1 && Ta_GetHandEnchant3(380, 3) == -1);
    assert(random_calls == 8 && elf_calls == 1);
}

static unsigned chk(int kind, int val, int card)
{
    static const int want[7] = { 0x103, 1, 2, 0x370, 1, 2, 5 };
    unsigned r;

    EnchantId[5] = (unsigned char)kind;
    gAnmPat_Bakura[5] = (unsigned short)val;
    card_no = card;
    log_count = 0;
    r = Chk_Enc(1, 2, 5);
    assert(log_count == 7 && memcmp(log_, want, sizeof(want)) == 0);
    card_no = 0;
    return r;
}

/* 0x248af0: branch coverage, one assertion per case of the three switches and per default. */
static void check_chk_enc(void)
{
    allow = 0;
    assert(chk(3, 0, 0) == 999);
    allow = 1;
    random_calls = elf_calls = 0;
    /* kinds 0 to 3 and any other kind */
    assert(chk(0, 0, 0) == 0x864005 && chk(0, 1, 0) == 0x860005);
    assert(chk(1, 5, 0) == 0x870005 && chk(1, 4, 0) == 0x860005);
    assert(chk(2, 0, 0) == 0x878005 && chk(3, 0, 0) == 0x874005 && chk(5, 0, 0) == 0x860005);
    /* kind 4 by value word */
    assert(chk(4, 5, 0) == 999 && chk(4, 10, 0) == 999);
    assert(chk(4, 6, 0) == 0x40444005 && chk(4, 7, 0) == 0x406ac005);
    assert(chk(4, 8, 342) == 0x407a0005 && chk(4, 8, 7) == 0x407d4005 && chk(4, 8, 8) == 999);
    assert(chk(4, 9, 15) == 999 && random_calls == 0 && elf_calls == 0);
    /* value 9: codes drawn by card number */
    random_answer = 7;
    assert(chk(4, 9, 16) == (0x40000005u | (15u << 14)) && random_calls == 1);
    random_answer = 9;
    assert(chk(4, 9, 17) == (0x40000005u | (17u << 14)) && random_calls == 2);
    random_answer = 31;
    assert(chk(4, 9, 167) == (0x40000005u | (210u << 14)) && random_calls == 3);
    random_answer = 33;
    assert(chk(4, 9, 332) == (0x40000005u | (102u << 14)) && random_calls == 4);
    /* the remainder by 32 is signed */
    random_answer = -1;
    assert(chk(4, 9, 332) == (0x40000005u | (100u << 14)) && random_calls == 5);
    random_answer = -32;
    assert(chk(4, 9, 332) == (0x40000005u | (101u << 14)) && random_calls == 6);
    elf_answer = 0x12345;
    assert(chk(4, 9, 379) == (0x40000005u | (0x2345u << 14)) && elf_calls == 1 && random_calls == 6);
}

static unsigned dchk(int kind, int val, int card)
{
    EnchantId[757] = (unsigned char)kind;
    gAnmPat_Bakura[757] = (unsigned short)val;
    return Do_Chk_Enc(card, 757);
}

/* 0x2487a0: branch coverage, the bit test both ways (also against the neighbouring bit and
 * row), then one assertion per case of the three switches and per default. */
static void check_do_chk_enc(void)
{
    memset(EnchMatch, 0xff, sizeof(EnchMatch));
    EnchMatch[0] = 0xdf;
    assert(dchk(3, 0, 0) == 999);
    memset(EnchMatch, 0, sizeof(EnchMatch));
    EnchMatch[0] = 0x20;
    assert(dchk(3, 0, 1) == 999);
    memset(EnchMatch, 0xff, sizeof(EnchMatch));
    random_calls = elf_calls = 0;
    /* kinds 0 to 3 and any other kind */
    assert(dchk(0, 0, 0) == (0x864000u | 757) && dchk(0, 1, 0) == (0x860000u | 757));
    assert(dchk(1, 5, 0) == (0x870000u | 757) && dchk(1, 4, 0) == (0x860000u | 757));
    assert(dchk(2, 0, 0) == (0x878000u | 757) && dchk(3, 0, 0) == (0x874000u | 757) && dchk(5, 0, 0) == (0x860000u | 757));
    /* kind 4 by value word */
    assert(dchk(4, 5, 0) == 999 && dchk(4, 10, 0) == 999);
    assert(dchk(4, 6, 0) == (0x40444000u | 757) && dchk(4, 7, 0) == (0x406ac000u | 757));
    assert(dchk(4, 8, 342) == (0x407a0000u | 757) && dchk(4, 8, 7) == (0x407d4000u | 757) && dchk(4, 8, 8) == 999);
    assert(dchk(4, 9, 15) == 999 && random_calls == 0 && elf_calls == 0);
    /* value 9: codes drawn by card number */
    random_answer = 7;
    assert(dchk(4, 9, 16) == ((0x40000000u | 757) | (15u << 14)) && random_calls == 1);
    random_answer = 9;
    assert(dchk(4, 9, 17) == ((0x40000000u | 757) | (17u << 14)) && random_calls == 2);
    random_answer = 31;
    assert(dchk(4, 9, 167) == ((0x40000000u | 757) | (210u << 14)) && random_calls == 3);
    random_answer = 33;
    assert(dchk(4, 9, 332) == ((0x40000000u | 757) | (102u << 14)) && random_calls == 4);
    /* the remainder by 32 is signed */
    random_answer = -1;
    assert(dchk(4, 9, 332) == ((0x40000000u | 757) | (100u << 14)) && random_calls == 5);
    random_answer = -32;
    assert(dchk(4, 9, 332) == ((0x40000000u | 757) | (101u << 14)) && random_calls == 6);
    elf_answer = 0x12345;
    assert(dchk(4, 9, 379) == ((0x40000000u | 757) | (0x2345u << 14)) && elf_calls == 1 && random_calls == 6);
}

static int has(int tag, int a, int b)
{
    int i;

    for (i = 0; i + 2 < log_count; i++) {
        if (log_[i] == tag && log_[i + 1] == a && log_[i + 2] == b) {
            return 1;
        }
    }
    return 0;
}

static int heap(int k1, int k2, int id1, int id2, int no, unsigned from, unsigned to, unsigned par)
{
    struct jp_1e41 info;
    int r;

    memset(&info, 0xee, sizeof(info));
    unit_kind[1] = k1;
    unit_kind[2] = k2;
    log_count = 0;
    r = SelfHeap(1, id1, 2, id2, &info);
    assert(info.eNO == no && info.dwFrom == from && info.dwTo == to && info.dwPar == par);
    return r;
}

/* 0x247f40: branch coverage, one assertion per outcome of every test. Chk_Enc is the real
 * function here, steered through its tables; the second `ID_2 == 0` test (kinds 2 and 0)
 * cannot be reached because the first one returns, so it has no assertion. */
static void check_self_heap(void)
{
    int pass;

    card_no = 5;
    /* nothing on the second cell: no callee is asked */
    assert(heap(0, 0, 3, 0, 95, 0x103, 0x200, 0) == 1 && log_count == 0);
    /* kinds 0 and 0 (the kind is the value shifted right by five) */
    assert(heap(0x1f, 0x1f, 0, 4, 95, 0x204, 0x100, 1) == 1 && log_count == 6);
    assert(has(0x700, 1, 0) && has(0x700, 2, 4));
    by_side = 1;
    fusion_answer = 0x4321;
    assert(heap(0x1f, 0x1f, 3, 4, 100, 0x103, 0x204, 0x4321) == 1);
    assert(has(0x103, 1, 3) && has(0x103, 2, 4) && has(0x5a0, 21, 37));
    by_side = 0;
    /* kinds 0 and 2, then 2 and 0 */
    allow = 1;
    assert(heap(0, 0x40, 0, 4, 95, 0x204, 0x100, 1) == 1 && !has(0x103, 2, 4));
    side_answer = 6;
    for (pass = 0; pass < 2; pass++) {
        int k1 = pass ? 0x5f : 0, k2 = pass ? 0 : 0x5f;
        int want = pass ? 1 : 0x10001;
        int s = pass ? 2 : 1, id = pass ? 4 : 3;

        allow = 0;
        assert(heap(k1, k2, 3, 4, 36, 0x103, 0x204, 999) == 1);
        assert(has(0x103, 3 - s, 7 - id) && has(0x370, s, id) && !has(0xe70de, s, id));
        allow = 1;
        EnchantId[5] = 4;
        gAnmPat_Bakura[5] = 6;
        if (pass) {
            assert(heap(k1, k2, 3, 4, 43, 0x103, 0x204, 0x111) == 1);
        } else {
            assert(heap(k1, k2, 3, 4, 43, 0x204, 0x103, 0x111) == 0x10001);
        }
        EnchantId[5] = 0;
        gAnmPat_Bakura[5] = 0;
        assert(heap(k1, k2, 3, 4, 37, 0x103, 0x204, 0x60005) == want && has(0xe70de, s, id));
        EnchantId[5] = 3;
        assert(heap(k1, k2, 3, 4, 36, 0x103, 0x204, 0x874005) == want && !has(0xe70de, s, id));
    }
    /* any other pair of kinds */
    assert(heap(0x40, 0x40, 3, 4, 95, 0x103, 0x204, 0) == 1 && log_count == 6);
    assert(heap(0, 0x20, 3, 4, 95, 0x103, 0x204, 0) == 1);
    assert(heap(0x20, 0, 3, 4, 95, 0x103, 0x204, 0) == 1);
    assert(heap(0x40, 0x20, 3, 4, 95, 0x103, 0x204, 0) == 1);
    card_no = 0;
}

int main(void)
{
    check_is_enc_do();
    check_apply_kind();
    check_hand_enchant3();
    check_chk_enc();
    check_do_chk_enc();
    check_self_heap();
    return 0;
}
