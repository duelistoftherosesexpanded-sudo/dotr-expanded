/* Synthetic host tests for the effect_manage group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/effect_manage.h"

/* Every sequence function is a stub that records which one ran and with what arguments. */
static int seq_called, seq_count, seq_result, seq_id;
static struct jp_11dc9 *seq_info;
static struct jp_11e6e *seq_temp;
#define SEQ_STUB(name, number) \
    int name(struct jp_11dc9 *eInfo, struct jp_11e6e *pTemp, int ID) \
    { \
        seq_called = (number); \
        seq_count++; \
        seq_info = eInfo; \
        seq_temp = pTemp; \
        seq_id = ID; \
        return seq_result; \
    }
SEQ_STUB(Ta_Eff_000, 1)
SEQ_STUB(Ta_Eff_001, 2)
SEQ_STUB(Ta_Eff_002, 3)
SEQ_STUB(Ta_Eff_003B, 4)
SEQ_STUB(Ta_Eff_004, 5)
SEQ_STUB(Ta_Eff_005, 6)
SEQ_STUB(Ta_Eff_006, 7)
SEQ_STUB(Ta_Eff_007, 8)
SEQ_STUB(Ta_Eff_008, 9)
SEQ_STUB(Ta_Eff_009, 10)
SEQ_STUB(Ta_Eff_012, 11)
SEQ_STUB(Ta_Eff_013, 12)
SEQ_STUB(Ta_Eff_014, 13)
SEQ_STUB(Ta_Eff_015, 14)
SEQ_STUB(Ta_Eff_016, 15)
SEQ_STUB(Ta_Eff_017_A, 16)
SEQ_STUB(Ta_Eff_018, 17)
SEQ_STUB(Ta_Eff_019, 18)
SEQ_STUB(Ta_Eff_020, 19)
SEQ_STUB(Ta_Eff_021, 20)
SEQ_STUB(Ta_Eff_023, 21)
SEQ_STUB(Ta_Eff_025, 22)
SEQ_STUB(Ta_Eff_026, 23)
SEQ_STUB(Ta_Eff_028, 24)
SEQ_STUB(Ta_Eff_029, 25)
SEQ_STUB(Ta_Eff_029_CNG, 26)
SEQ_STUB(Ta_Eff_030, 27)
SEQ_STUB(Ta_Eff_031, 28)
SEQ_STUB(Ta_Eff_032, 29)
SEQ_STUB(Ta_Eff_033, 30)
SEQ_STUB(Ta_Eff_034, 31)
SEQ_STUB(Ta_Eff_035, 32)
SEQ_STUB(Ta_Eff_017_MEGUMI, 33)
SEQ_STUB(Ta_Eff_006_WIND, 34)
SEQ_STUB(Ta_Eff_BTL_000, 35)
SEQ_STUB(Ta_Eff_036, 36)
SEQ_STUB(Ta_Eff_037, 37)
SEQ_STUB(Ta_Eff_BTL_001, 38)
SEQ_STUB(Ta_Eff_BTL_002, 39)
SEQ_STUB(Ta_Eff_100, 40)
SEQ_STUB(Ta_Eff_044, 41)
SEQ_STUB(Ta_Eff_045, 42)
SEQ_STUB(Ta_Eff_046, 43)
SEQ_STUB(Ta_Eff_051, 44)
SEQ_STUB(Ta_Eff_054, 45)
SEQ_STUB(Ta_Eff_056, 46)
SEQ_STUB(Ta_Eff_057, 47)
SEQ_STUB(Ta_Eff_058, 48)
SEQ_STUB(Ta_Eff_059, 49)
SEQ_STUB(Ta_Eff_060, 50)
SEQ_STUB(Ta_Eff_063, 51)
SEQ_STUB(Ta_Eff_064, 52)
SEQ_STUB(Ta_Eff_065, 53)
SEQ_STUB(Ta_Eff_066, 54)
SEQ_STUB(Ta_Eff_067, 55)
SEQ_STUB(Ta_Eff_070, 56)
SEQ_STUB(Ta_Eff_071, 57)
SEQ_STUB(Ta_Eff_072, 58)
SEQ_STUB(Ta_Eff_101, 59)
SEQ_STUB(Ta_Eff_102, 60)
SEQ_STUB(Ta_Eff_109, 61)
SEQ_STUB(Ta_Eff_112, 62)
SEQ_STUB(Ta_Eff_113, 63)
SEQ_STUB(Ta_Eff_114, 64)
SEQ_STUB(Ta_Eff_Hand_Dest, 65)
SEQ_STUB(Ta_Eff_ETC, 66)
SEQ_STUB(Ta_Eff_WAIT, 67)

static const struct {
    int eNO;
    int stub;
} seq_cases[] = {
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 4},
    {4, 5},
    {5, 6},
    {6, 7},
    {7, 8},
    {8, 9},
    {9, 10},
    {12, 11},
    {13, 12},
    {14, 13},
    {15, 14},
    {16, 15},
    {17, 16},
    {18, 17},
    {19, 18},
    {20, 19},
    {21, 20},
    {23, 21},
    {25, 22},
    {26, 23},
    {28, 24},
    {29, 25},
    {91, 26},
    {30, 27},
    {31, 28},
    {32, 29},
    {33, 30},
    {34, 31},
    {35, 32},
    {90, 33},
    {92, 34},
    {93, 35},
    {36, 36},
    {37, 37},
    {94, 38},
    {95, 39},
    {43, 40},
    {44, 41},
    {45, 42},
    {46, 43},
    {51, 44},
    {54, 45},
    {56, 46},
    {57, 47},
    {58, 48},
    {59, 49},
    {60, 50},
    {63, 51},
    {64, 52},
    {65, 53},
    {66, 54},
    {67, 55},
    {70, 56},
    {71, 57},
    {72, 58},
    {100, 40},
    {101, 59},
    {102, 60},
    {109, 61},
    {112, 62},
    {113, 63},
    {114, 64},
    {150, 65},
    {255, 66},
    {254, 67},
};

/* Unit queries: kind is 64 * side + 32 * unit, the type of kind >> 5 comes from a table, the
 * status bit 4 from another; the two setters append to a log. */
static int type_of[2][16], status4[2][16];
static int log_calls[64][5], log_count;
int SzDuel_GetUnitKind(int a, int b) { return ((a * 16 + b) << 5) | 7; }
int SzDuel_Kind2Type(int value) { return type_of[value / 16][value % 16]; }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(bit == 4);
    return status4[a][b];
}
int SzEffect_SetBusyFlag2(int packed, int flag)
{
    log_calls[log_count][0] = 1;
    log_calls[log_count][1] = packed;
    log_calls[log_count][2] = flag;
    log_count++;
    return 0;
}
int SzDuel_SetUnitStatus(int a, int b, int bit, int value)
{
    log_calls[log_count][0] = 2;
    log_calls[log_count][1] = a;
    log_calls[log_count][2] = b;
    log_calls[log_count][3] = bit;
    log_calls[log_count][4] = value;
    log_count++;
    return 0;
}

static void world_reset(void)
{
    seq_called = seq_count = seq_id = 0;
    seq_info = NULL;
    seq_temp = NULL;
    memset(type_of, 0, sizeof(type_of));
    memset(status4, 0, sizeof(status4));
    log_count = 0;
}

static void test_ta_manage_effect(void)
{
    struct jp_11dc9 info;
    static int temp_storage;
    struct jp_11e6e *temp = (struct jp_11e6e *)&temp_storage;
    size_t n;
    int i;

    memset(&info, 0, sizeof(info));
    /* Missing effect or work area: 0, nothing called. */
    world_reset();
    seq_result = 5;
    type_of[0][1] = 2;
    status4[0][1] = 1;
    assert(Ta_Manage_Effect(NULL, temp, 1) == 0);
    assert(Ta_Manage_Effect(&info, NULL, 1) == 0);
    assert(seq_count == 0 && log_count == 0);
    /* Every effect number with a sequence: that function alone, arguments passed on, its
     * non-zero result returned and no unit touched. Bit 15 and above are ignored. */
    for (n = 0; n < sizeof(seq_cases) / sizeof(seq_cases[0]); n++) {
        world_reset();
        type_of[0][1] = 2;
        status4[0][1] = 1;
        seq_result = 100 + (int)n;
        info.eNO = seq_cases[n].eNO | (n & 1 ? 0x18000 : 0);
        assert(Ta_Manage_Effect(&info, temp, 40 + (int)n) == 100 + (int)n);
        assert(seq_count == 1 && seq_called == seq_cases[n].stub);
        assert(seq_info == &info && seq_temp == temp && seq_id == 40 + (int)n);
        assert(log_count == 0);
    }
    assert(n == 68);
    /* A sequence that returns 0: the unit pass runs. Units 1..10 of both sides; only type 2
     * with status bit 4 is released, side 0 busy flag first, side 1 status first. */
    world_reset();
    seq_result = 0;
    type_of[0][0] = type_of[0][11] = type_of[1][0] = type_of[1][11] = 2;
    status4[0][0] = status4[0][11] = status4[1][0] = status4[1][11] = 1;
    type_of[0][1] = 2;
    status4[0][1] = 1;
    type_of[0][2] = 2;
    type_of[0][3] = 1;
    status4[0][3] = 1;
    type_of[1][10] = 2;
    status4[1][10] = 3;
    type_of[1][9] = 2;
    type_of[1][8] = 3;
    status4[1][8] = 1;
    info.eNO = 1;
    assert(Ta_Manage_Effect(&info, temp, 2) == 0);
    assert(seq_count == 1 && log_count == 4);
    assert(log_calls[0][0] == 1 && log_calls[0][1] == 1 && log_calls[0][2] == 0);
    assert(log_calls[1][0] == 2 && log_calls[1][1] == 0 && log_calls[1][2] == 1 && log_calls[1][3] == 0 &&
           log_calls[1][4] == 1);
    assert(log_calls[2][0] == 2 && log_calls[2][1] == 1 && log_calls[2][2] == 10 && log_calls[2][3] == 0 &&
           log_calls[2][4] == 1);
    assert(log_calls[3][0] == 1 && log_calls[3][1] == 0x10A && log_calls[3][2] == 0);
    /* Number 999 and numbers without a sequence (10, 115, 1000): no sequence, result 0, unit
     * pass runs. */
    for (i = 0; i < 4; i++) {
        static const int none[4] = {999, 10, 115, 1000};
        world_reset();
        seq_result = 9;
        type_of[1][5] = 2;
        status4[1][5] = 1;
        info.eNO = none[i];
        assert(Ta_Manage_Effect(&info, temp, 0) == 0);
        assert(seq_count == 0 && log_count == 2);
        assert(log_calls[0][0] == 2 && log_calls[0][1] == 1 && log_calls[0][2] == 5);
        assert(log_calls[1][0] == 1 && log_calls[1][1] == 0x105);
    }
}

int main(void)
{
    test_ta_manage_effect();
    return 0;
}
