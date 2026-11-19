/* Synthetic host tests for the hand_combo group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/hand_combo.h"

static int hand_side, hand_count, hand[8];
static unsigned char kind_of[1024];
static int fusion_rets[8], fusion_log[8][2], fusion_calls;
static unsigned int enc_rets[8];
static int enc_log[8][2], enc_calls, init_calls;

int SzDuel_GetHandCount(int index) { hand_side = index; return hand_count; }
int SzDuel_InqHand(int index, int position)
{
    assert(index == hand_side && position >= 0 && position < hand_count);
    return hand[position];
}
void SzDuel_InitCardInfo(struct jp_86d82 *block, int id)
{
    assert(id >= 0 && id < 1024 && id != 999);
    memset(block, 0xEE, sizeof(*block));
    block->bVarKind = (unsigned char)((kind_of[id] << 5) | 0x1F);
    init_calls++;
}
int Fusion(int a, int b)
{
    fusion_log[fusion_calls][0] = a;
    fusion_log[fusion_calls][1] = b;
    return fusion_rets[fusion_calls++];
}
unsigned int Do_Chk_Enc(int cNO, int eNO)
{
    enc_log[enc_calls][0] = cNO;
    enc_log[enc_calls][1] = eNO;
    return enc_rets[enc_calls++];
}

static int side_ret, side_log[3], side_calls;
int Ta_SideMatch(int a, int b, int mode)
{
    side_log[0] = a;
    side_log[1] = b;
    side_log[2] = mode;
    side_calls++;
    return side_ret;
}

#define ENC_HIT(id) ((1u << 30) | ((unsigned int)(id) << 14) | 0x2ABC)
#define ENC_MISS(id) (((unsigned int)(id) << 14) | 0x2ABC)

static void combo_reset(void)
{
    hand_side = -1;
    hand_count = 0;
    fusion_calls = enc_calls = init_calls = 0;
    memset(kind_of, 0, sizeof(kind_of));
}

/* Branch coverage: one assertion per arm of both paths, per result test and per loop exit. */
static void check_combo_pair(void)
{
    /* both cards given: the hand is not read */
    combo_reset();
    fusion_rets[0] = 55;
    assert(Ta_GetComboResult(1, 10, 20) == 55 && hand_side == 1 && init_calls == 2);
    assert(fusion_calls == 1 && fusion_log[0][0] == 10 && fusion_log[0][1] == 20 && enc_calls == 0);
    combo_reset();
    fusion_rets[0] = 999;
    assert(Ta_GetComboResult(0, 10, 20) == 20 && hand_side == 0);

    combo_reset();
    kind_of[10] = 2;
    enc_rets[0] = 999;
    assert(Ta_GetComboResult(1, 10, 20) == 20);
    assert(enc_calls == 1 && enc_log[0][0] == 20 && enc_log[0][1] == 10 && fusion_calls == 0);
    enc_calls = 0;
    enc_rets[0] = ENC_HIT(77);
    assert(Ta_GetComboResult(1, 10, 20) == 77);
    enc_calls = 0;
    enc_rets[0] = ENC_MISS(77);
    assert(Ta_GetComboResult(1, 10, 20) == 20);

    combo_reset();
    kind_of[20] = 2;
    enc_rets[0] = 999;
    assert(Ta_GetComboResult(1, 10, 20) == 20);
    assert(enc_calls == 1 && enc_log[0][0] == 10 && enc_log[0][1] == 20 && fusion_calls == 0);
    enc_calls = 0;
    enc_rets[0] = ENC_HIT(0x3FFF);
    assert(Ta_GetComboResult(1, 10, 20) == 0x3FFF);
    enc_calls = 0;
    enc_rets[0] = ENC_MISS(77);
    assert(Ta_GetComboResult(1, 10, 20) == 10);

    /* every other pair of kinds gives the second card without a call */
    combo_reset();
    kind_of[10] = 1;
    assert(Ta_GetComboResult(1, 10, 20) == 20);
    kind_of[10] = 2; kind_of[20] = 2;
    assert(Ta_GetComboResult(1, 10, 20) == 20);
    kind_of[10] = 0; kind_of[20] = 1;
    assert(Ta_GetComboResult(1, 10, 20) == 20);
    kind_of[10] = 2; kind_of[20] = 1;
    assert(Ta_GetComboResult(1, 10, 20) == 20 && fusion_calls == 0 && enc_calls == 0);
}

static void check_combo_hand(void)
{
    int i;

    /* nothing given, empty hand */
    combo_reset();
    assert(Ta_GetComboResult(1, 999, 999) == 999 && init_calls == 0);
    /* a second card without a first is ignored; first slot empty, next one stops the walk */
    assert(Ta_GetComboResult(1, 999, 20) == 999 && init_calls == 0);
    assert(Ta_GetComboResult(1, 10, 999) == 10 && init_calls == 1);

    /* order 0 entries are skipped; the walk stops at the first empty order slot */
    combo_reset();
    hand_count = 3;
    hand[0] = 0x1000 | 20; hand[1] = 0x0000 | 21; hand[2] = 0x3000 | 30;
    fusion_rets[0] = 40;
    assert(Ta_GetComboResult(1, 10, 999) == 40);
    assert(fusion_calls == 1 && fusion_log[0][0] == 10 && fusion_log[0][1] == 20);
    /* the count bounds the hand read */
    combo_reset();
    hand_count = 0;
    hand[0] = 0x1000 | 20;
    assert(Ta_GetComboResult(1, 10, 999) == 10 && fusion_calls == 0);
    /* no first card: the first ordered card is taken as it is */
    combo_reset();
    hand_count = 1;
    hand[0] = 0x1000 | 20;
    assert(Ta_GetComboResult(1, 999, 999) == 20 && fusion_calls == 0 && enc_calls == 0 && init_calls == 2);

    /* five steps: fusion miss, enchant hit, enchant miss on the other side, two plain steps */
    combo_reset();
    hand_count = 5;
    for (i = 0; i < 5; i++) {
        hand[i] = ((i + 1) << 12) | (20 + i);
    }
    kind_of[21] = 2; kind_of[77] = 2; kind_of[23] = 1;
    fusion_rets[0] = 999;
    enc_rets[0] = ENC_HIT(77);
    enc_rets[1] = ENC_MISS(5);
    assert(Ta_GetComboResult(0, 10, 999) == 24);
    assert(fusion_calls == 1 && fusion_log[0][0] == 10 && fusion_log[0][1] == 20);
    assert(enc_calls == 2 && enc_log[0][0] == 20 && enc_log[0][1] == 21);
    assert(enc_log[1][0] == 22 && enc_log[1][1] == 77);

    /* five steps: both enchant orders with no result, enchant hit, enchant miss keeping the
     * running card, fusion hit */
    combo_reset();
    hand_count = 5;
    for (i = 0; i < 5; i++) {
        hand[4 - i] = ((i + 1) << 12) | (31 + i);
    }
    kind_of[30] = 2; kind_of[32] = 2; kind_of[34] = 2;
    enc_rets[0] = 999;
    enc_rets[1] = 999;
    enc_rets[2] = ENC_HIT(90);
    enc_rets[3] = ENC_MISS(5);
    fusion_rets[0] = 91;
    assert(Ta_GetComboResult(1, 30, 999) == 91);
    assert(enc_calls == 4 && enc_log[0][0] == 31 && enc_log[0][1] == 30);
    assert(enc_log[1][0] == 31 && enc_log[1][1] == 32);
    assert(enc_log[2][0] == 33 && enc_log[2][1] == 32);
    assert(enc_log[3][0] == 90 && enc_log[3][1] == 34);
    assert(fusion_calls == 1 && fusion_log[0][0] == 90 && fusion_log[0][1] == 35);
}

static int agree(int kind, int attr, int key)
{
    struct jp_86d82 v;

    memset(&v, 0, sizeof(v));
    v.bVarKind = (unsigned char)kind;
    v.bVarAttr = (unsigned char)attr;
    v.wCardNo = 345;
    v.bOwner = 1;
    side_ret = 1;
    return Ta_GetUnitAgreeWithVarInfo(&v, 4, 5, key);
}

/* Branch coverage: the kind gate, both owner selections, every key with a hit and a miss, the
 * default key and both halves of the final test. */
static void check_unit_agree(void)
{
    static const int kind_keys[] = { 14, 15, 2, 6, 7, 16, 17, 19 };
    struct jp_86d82 v;
    size_t i;

    /* kind with a bit above the low five: no call at all */
    combo_reset();
    side_calls = 0;
    assert(agree(0x20 | 14, 0, 14) == 0 && side_calls == 0);

    /* owner passed through; fRevolt inverts it (non-zero owner becomes 0, zero becomes 1) */
    assert(agree(14, 0, 14) == 1 && side_calls == 1);
    assert(side_log[0] == 1 && side_log[1] == 4 && side_log[2] == 5);
    memset(&v, 0, sizeof(v));
    v.fRevolt = -1;
    v.bOwner = 1;
    side_ret = 1;
    assert(Ta_GetUnitAgreeWithVarInfo(&v, 4, 5, 29) == 1 && side_log[0] == 0);
    v.bOwner = 0;
    assert(Ta_GetUnitAgreeWithVarInfo(&v, 4, 5, 29) == 1 && side_log[0] == 1);
    v.fRevolt = 0;
    assert(Ta_GetUnitAgreeWithVarInfo(&v, 4, 5, 29) == 1 && side_log[0] == 0);

    /* kind keys: the key must equal the kind */
    for (i = 0; i < sizeof(kind_keys) / sizeof(kind_keys[0]); i++) {
        assert(agree(kind_keys[i], 0, kind_keys[i]) == 1);
        assert(agree(kind_keys[i] ^ 1, 0, kind_keys[i]) == 0);
    }
    /* attribute keys: 21 wants attribute 1, 20 attribute 0, 25 attribute 5; 29 always agrees */
    assert(agree(0, 1, 21) == 1 && agree(0, 0, 21) == 0);
    assert(agree(0, 0, 20) == 1 && agree(0, 1, 20) == 0);
    assert(agree(0, 5, 25) == 1 && agree(0, 4, 25) == 0);
    assert(agree(0, 0, 29) == 1);
    /* key 49: Do_Chk_Enc(card, 767) other than 999 agrees */
    combo_reset();
    enc_rets[0] = 0;
    assert(agree(0, 0, 49) == 1 && enc_calls == 1 && enc_log[0][0] == 345 && enc_log[0][1] == 767);
    combo_reset();
    enc_rets[0] = 999;
    assert(agree(0, 0, 49) == 0 && enc_calls == 1);
    /* any other key never agrees; neither does a side mismatch */
    assert(agree(3, 0, 3) == 0 && agree(0, 0, 0) == 0 && agree(0, 0, 50) == 0);
    memset(&v, 0, sizeof(v));
    side_ret = 0;
    assert(Ta_GetUnitAgreeWithVarInfo(&v, 4, 5, 29) == 0);
}

int main(void)
{
    check_combo_pair();
    check_combo_hand();
    check_unit_agree();
    puts("hand_combo: ok");
    return 0;
}
