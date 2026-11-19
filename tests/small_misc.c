/* Synthetic tests of the miscellaneous tiny routines; every callee is a stub.
 * 0x242cd0 never returns, so its stub leaves through longjmp. */
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dotr/ee/small_misc.h"

int isCountVblank[29], frd[1], data_003506c8[1];
int isWithAudio[1];
struct DotrMiscObject audioDec;
char data_0030f3ae[1], data_0030f8e9[1];
struct DotrMiscEntry52 gSh_Face[256];
struct DotrMiscEntry7 data_0029d586[4];
unsigned char EnchantId[0x10000];
float data_00294050[1025];
static float a22c0_last;
int func_001a22c0(float value)
{
    a22c0_last = value;
    return (int)value;
}

static jmp_buf escape;
static int loop_limit, tick_calls, tick_argument, print_value, calls[6];
static char *print_text;
static void *adjusted;

void func_0017b790(int unknown)
{
    tick_calls++;
    tick_argument = unknown;
    if (loop_limit && tick_calls >= loop_limit) longjmp(escape, 1);
}

void func_00199b50(char *text, int value) { print_text = text; print_value = value; }
void func_0018e200(void) { calls[0]++; }
int viBufBeginPut(void *p) { adjusted = p; return 0x40f0; }
int viBufEndPut(void *p) { adjusted = p; return 0x41f0; }
void viBufAddDMA(int *p) { assert(p == data_003506c8 && tick_calls == 1); calls[1]++; }
void viBufStopDMA(int *p) { assert(p == data_003506c8); calls[2]++; }
void viBufRestartDMA(int *p) { assert(p == data_003506c8); calls[3]++; }

int data_00317b68[1], gSzModel_pfhCache[1], data_0031c2e4[1], data_0032a220[1], require_coordinate_no[1];
unsigned char gSzMsg_ml[1], gSzData_bRoseFlag[1];
signed char data_00299eda[1];
short winCount[1];

static int t_id, t_args[8], t_count;
static void *t_pointer;
static int (*t_callback)(void);

static int thunk(int id, int count, int a, int b, int c, int d, int e, int f, int g, int h)
{
    t_id = id;
    t_count = count;
    t_args[0] = a;
    t_args[1] = b;
    t_args[2] = c;
    t_args[3] = d;
    t_args[4] = e;
    t_args[5] = f;
    t_args[6] = g;
    t_args[7] = h;
    return 0x700 + id;
}

int SzDuel_StartWindowSessionWithPalette_Pipe(int a, int b) { return thunk(1, 2, a, b, 0, 0, 0, 0, 0, 0); }
int lSzDuel_DrawWindow_ExBoard_Pipe(int a, int b, int c, int d, int e, int f, int g, int h) { return thunk(2, 8, a, b, c, d, e, f, g, h); }
int SzDuel_DrawWindow_ExDigit_Pipe(int a, int b, int c, int d, int e, int f, int g, int h) { return thunk(3, 8, a, b, c, d, e, f, g, h); }
int SzEffect_EntryCallback(int a, int b, int c, int d, int e, int f, int g) { return thunk(4, 7, a, b, c, d, e, f, g, 0); }
int SzFile_InqRequestX(int a, int b) { return thunk(5, 2, a, b, 0, 0, 0, 0, 0, 0); }
int Ta_GetCurrentSide(void) { return thunk(6, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
int TaView_ReqFocusCamera(int a, int b, int c, int d) { return thunk(7, 4, a, b, c, d, 0, 0, 0, 0); }
int TaHand_GetCardSummonCost2(int a) { return thunk(8, 1, a, 0, 0, 0, 0, 0, 0, 0); }
int lSzDuel_SortFunc1(void) { return 0; }
int lSzDuel_SortFunc2(void) { return 0; }
unsigned char HELP_flg[1];
struct DotrMiscControlTail data_00317490_tail;
void SzGame_Push(int arg0, int arg1) { (void)arg0; (void)arg1; }
static int grid_calls[64][2], grid_count, grid_negative_at = -1, grid_negative_at2 = -1, grid_negative_value;
int SzDuel_GetMapUnitID(int x, int y)
{
    grid_calls[grid_count][0] = x;
    grid_calls[grid_count][1] = y;
    grid_count++;
    return x * 7 + y == grid_negative_at || x * 7 + y == grid_negative_at2 ? grid_negative_value : x == 6 ? 5 : 0;
}
void func_00199bc0(int a, int b, int c, int (*callback)(void))
{
    thunk(9, 3, a, b, c, 0, 0, 0, 0, 0);
    t_callback = callback;
}
int ExSortNode_Pipe(int a, void *b, int c, int d, int e, int f)
{
    t_pointer = b;
    return thunk(10, 6, a, 0, c, d, e, f, 0, 0);
}
unsigned char *RhMem_FillMemory(unsigned char *destination, int value, int count)
{
    t_pointer = destination;
    thunk(11, 2, value, count, 0, 0, 0, 0, 0, 0);
    return destination + count;
}

int gRhZSort_zsl[4];
struct DotrMiscHigh data_01ed01a4;
static int slot_argument = -1, word_a, word_b, item_seen;
static int stub_q_result, stub_q_kind, stub_q_mode, stub_g_calls, stub_g_result = 0x7e57, stub_g_args[4];

void SzMsg_Free(int unknown) { slot_argument = unknown; }
static unsigned word_mask = ~0u; /* bit i set: func_001df540_word(_, i) is nonzero */
static int word_calls, word_seen[16];
int func_001df540_word(int a, int b)
{
    word_a = a;
    word_b = b;
    word_seen[word_calls++ & 15] = b;
    return (word_mask >> b) & 1 ? 0x5540 : 0;
}
static int gate_result = 0xfdd0;
int Ta_IsVirus_Chk2(int item) { item_seen = item; return gate_result; }

static void check_third_batch(void)
{
    struct DotrMiscTag tag;
    struct DotrMiscHigh expected;
    int values[4] = {10, 20, 30, 40};
    unsigned char from[6] = {1, 2, 3, 4, 5, 6}, to[6] = {9, 9, 9, 9, 9, 9};
    assert(SzDuel_FinishWindowSession_Pipe(-41) == -41 && slot_argument == 0);
    RhZSort_Start(7, -8);
    assert(gRhZSort_zsl[0] == 7 && gRhZSort_zsl[1] == -8 && gRhZSort_zsl[2] == 1 && gRhZSort_zsl[3] == -8);
    /* 3 * (e*(b-a)/d + d*(c-b)/e) with exact binary values. */
    assert(func(1.0f, 3.0f, 7.0f, 2.0f, 4.0f) == 3.0f * (4.0f + 2.0f));
    memset(&data_01ed01a4, 0x66, sizeof(data_01ed01a4));
    memcpy(&expected, &data_01ed01a4, sizeof(expected));
    expected.unknown_00 = (short)0x8123;
    expected.unknown_0e = -2;
    expected.unknown_0c = 0;
    expected.unknown_02 = 0;
    expected.unknown_10 = 1;
    Taku_Fune_init(0x18123, -2);
    assert(memcmp(&expected, &data_01ed01a4, sizeof(expected)) == 0);
    Swap(values, 0, 3);
    assert(values[0] == 40 && values[3] == 10 && values[1] == 20 && values[2] == 30);
    Swap(values, 2, 2);
    assert(values[2] == 30);
    assert(Ta_IsVirus_Chk(2, 9) == 0xfdd0 && word_a == 2 && word_b == 9 && item_seen == 0x5540);
    memset(&tag, 0x7f, sizeof(tag));
    assert(Ta_Do_HandDestroyEfc(&tag, 0x12, 0x34) == 1);
    assert(tag.unknown_00 == 150 && tag.unknown_04 == 0 && tag.unknown_08 == 0x1234 && tag.unknown_0c == 0);
    stub_q_result = 0;
    stub_g_result = 0;
    assert(Ta_GetLDRange(77) == 3 && stub_g_args[0] == 77 && stub_g_args[2] == 9);
    stub_g_result = -1;
    assert(Ta_GetLDRange(78) == 5);
    stub_g_result = 0x7e57;
    TaLogic_CopyArray(from, to);
    assert(memcmp(to, from, 5) == 0 && to[5] == 9);
}

struct DotrMiscControlWords data_00317490_words;
struct DotrMiscEntry160 gSzDuel_aCardControl[6];
struct DotrMiscEntry160 gSzDuel_aCardRequest[32];
int gSzView_pCamera[1], gSh_FaceMode[1], gSh_pbVif1[1];
struct DotrMiscEntry12 gSzFade_fl[4];
struct DotrMiscLargeEntry gSzData_All[2];

static int f_calls[12], f_args[6], released_word;
static struct DotrMiscNode *node_seen;
static struct DotrMiscObject *object_seen;
static int *adjusted_word;

void SzModel_Preset(int a, int b, int c) { assert(f_calls[1] == 0); f_calls[0]++; f_args[0] = a; f_args[1] = b; f_args[2] = c; }
void func_001f9940_three(int a, int b, int c) { assert(f_calls[0] == 1); f_calls[1]++; f_args[3] = a; f_args[4] = b; f_args[5] = c; }
static int node_want = 0x55, node_calls;
void SzDuel_ClearDrawCard(struct DotrMiscNode *p) { node_seen = p; node_calls++; assert(p->unknown_90 == node_want); }
void SzBattle_Return(int a, int b) { assert(a == f_calls[2] && b == 0); f_calls[2]++; }
void func_00217b50_word(int value) { released_word = value; f_calls[3]++; }
int SzFade_Disp(int index) { assert(index == f_calls[4]); f_calls[4]++; return 1; }
int RhView_Term(void) { assert(f_calls[5] == 0 && f_calls[6] == 0); f_calls[5]++; return 1; }
int RhMemCard_Term(void) { assert(f_calls[5] == 1); f_calls[6]++; return 1; }
void func_00182a28(int value) { f_args[f_calls[7]] = value; f_calls[7]++; }
void changeMasterVolume(int value) { assert(f_calls[7] == 2 && value == 0); f_calls[8]++; }
void viBufDelete(int *p) { adjusted_word = p; assert(f_calls[9] == 0); }
void func_0018ed10(struct DotrMiscObject *p) { object_seen = p; f_calls[9]++; }
int SzDuel_GetUnitSpellCount(int a) { return 1000 + a; }
int SzDuel_GetUnitMonsterCount(int a) { return 2000 + a; }

static void check_fourth_batch(void)
{
    struct DotrMiscNode node;
    struct DotrMiscObject object;
    struct DotrMiscTriple triple, triple_expected;
    struct DotrMiscVector3 a = {1.0f, 2.0f, 3.0f}, b = {0.5f, -4.0f, 8.0f};
    struct DotrMiscEntry16 entries[22];
    unsigned char five[5] = {9, 8, 7, 6, 5};
    int i;
    for (i = 0; i < 8; i++) data_00317490_words.unknown_6a0[i] = 100 + i;
    for (i = 0; i < 8; i++) assert(SzDuel_GetWindowTexture(i) == 100 + i);
    assert(SzDuel_GetWindowTexture(-1) == 0 && SzDuel_GetWindowTexture(8) == 0);
    SzDuel_Term3DView();
    assert(f_calls[0] == 1 && f_calls[1] == 1);
    assert(f_args[0] == 29 && f_args[1] == -1 && f_args[2] == 999 && f_args[3] == 0 && f_args[4] == 4 && f_args[5] == 0);
    assert(SzDuel_FreeCardRequest(0) == 0);
    node.unknown_90 = 0x55;
    assert(SzDuel_FreeCardRequest(&node) == 1 && node_seen == &node && node.unknown_90 == 0);
    for (i = 0; i < 6; i++) assert(SzDuel_GetCardControl(i) == &gSzDuel_aCardControl[i]);
    assert(SzDuel_GetCardControl(-1) == 0 && SzDuel_GetCardControl(6) == 0);
    SzBattle_Term();
    assert(f_calls[2] == 2);
    SzFade_DispAll();
    assert(f_calls[4] == 2);
    /* Release once when the word is nonzero, then clear it. */
    gSzView_pCamera[0] = 0;
    SzView_TermCamera();
    assert(f_calls[3] == 0);
    gSzView_pCamera[0] = 0x7654;
    SzView_TermCamera();
    assert(f_calls[3] == 1 && released_word == 0x7654 && gSzView_pCamera[0] == 0);
    memset(gSzFade_fl, 0, sizeof(gSzFade_fl));
    assert(SzFade_Destroy(2) == 1);
    assert(gSzFade_fl[2].unknown_07_bit_6 == -1 && gSzFade_fl[2].unknown_07_bits_0_5 == 0);
    assert(gSzFade_fl[1].unknown_07_bit_6 == 0 && gSzFade_fl[3].unknown_07_bit_6 == 0);
    assert(RhBase_Term() == 1 && f_calls[5] == 1 && f_calls[6] == 1);
    gSzData_bRoseFlag[0] = 3;
    assert(SzData_GetData(3) == &gSzData_All[0] && SzData_GetData(4) == &gSzData_All[1]);

    memset(&triple, 0x7f, sizeof(triple));
    memset(&triple_expected, 0, sizeof(triple_expected));
    triple_expected.unknown_00.x = triple_expected.unknown_00.y = 1.0f;
    triple_expected.unknown_00.z = triple_expected.unknown_00.unknown_w = 1.0f;
    Taku_Coord_Init(&triple);
    assert(memcmp(&triple, &triple_expected, sizeof(triple)) == 0);
    memset(&triple, 0x7f, sizeof(triple));
    Ta_LkInitCoordinate(&triple);
    assert(memcmp(&triple, &triple_expected, sizeof(triple)) == 0);
    Ta_LkAddVector(&a, &b);
    assert(a.x == 1.5f && a.y == -2.0f && a.z == 11.0f);
    Ta_LkSubVector(&a, &b);
    assert(a.x == 1.0f && a.y == 2.0f && a.z == 3.0f);

    memset(&object, 0, sizeof(object));
    object.unknown_44 = 44;
    object.unknown_58 = 58;
    assert(audioDecDelete(&object) == 1 && f_calls[7] == 2 && f_calls[8] == 1);
    assert(f_args[0] == 44 && f_args[1] == 58);
    assert(videoDecDelete(&object) == 1 && adjusted_word == &object.unknown_48 && object_seen == &object);

    memset(gSh_Face, 0x44, sizeof(gSh_Face));
    gSh_pbVif1[0] = 9;
    ShFace_Init(-21);
    assert(gSh_FaceMode[0] == -21 && gSh_pbVif1[0] == 0);
    for (i = 0; i < 16; i++) assert(gSh_Face[i].unknown_00 == (i < 8 ? 0 : 0x44444444));

    /* Bit count over the low 22 bits only. */
    assert(Ta_GetUnitNumFrom22bitFlagList(0) == 0 && Ta_GetUnitNumFrom22bitFlagList(0x3fffff) == 22 && Ta_GetUnitNumFrom22bitFlagList(0xffffffffu) == 22);
    assert(Ta_GetUnitNumFrom22bitFlagList(0x00400000) == 0 && Ta_GetUnitNumFrom22bitFlagList(0x200001) == 2);
    for (i = 0; i < 22; i++) {
        entries[i].unknown_00 = (unsigned short)(500 + i);
        memset(entries[i].unknown_02, 0, sizeof(entries[i].unknown_02));
    }
    assert(TaLogic_IsUnitAssign(500, entries) == 1 && TaLogic_IsUnitAssign(521, entries) == 1 && TaLogic_IsUnitAssign(522, entries) == 0);
    assert(TaLogic_GetActionUnitFromOwnerID(entries, 500) == 0 && TaLogic_GetActionUnitFromOwnerID(entries, 521) == 21 && TaLogic_GetActionUnitFromOwnerID(entries, 499) == -1);
    assert(TaLogic_GetUnitNum(5, 1) == 1005 && TaLogic_GetUnitNum(5, 0) == 2005 && TaLogic_GetUnitNum(5, 2) == 2005);
    assert(TaLogic_IsUseHandInCombo(9, five) == 1 && TaLogic_IsUseHandInCombo(5, five) == 1 && TaLogic_IsUseHandInCombo(4, five) == 0);
}

static void check_second_batch(void)
{
    struct DotrMiscSource source;
    struct DotrMiscVector3 from = {1.0f, -2.0f, 3.5f}, to = {0.0f, 0.0f, 0.0f};
    int value;
    lSzGame_TermMemCheck();
    lSzGame_TermTitle();
    SzDuel_TermBuildDeck();
    SzDuel_TermTrade();
    SzDuel_TermCustomDuel();
    SzDuel_TermMapEdit();
    SzEvent_Term();
    RhFile_VSyncProcess();
    TaLogic_InitIgnoreLst();
    TaTuto_Init032();
    ShSlot_Term();
    assert(SzView_Init() == 1 && RhFile_Term() == 1 && RhGeom_Term() == 1 && TaUtil_GetObjSideNum(0x200C01) == 0);
    /* Thunks: forwarded arguments with appended zeros or masked values. */
    assert(SzDuel_StartWindowSession_Pipe(-5) == 0x701 && t_args[0] == -5 && t_args[1] == 0);
    assert(lSzDuel_DrawWindow_Board_Pipe(1, 2, 3, 4, 5, 6, 7) == 0x702 && t_args[6] == 7 && t_args[7] == 0 && t_args[0] == 1);
    assert(SzDuel_DrawWindow_Digit_Pipe(1, 2, 3, 4, 5, 6) == 0x703 && t_args[5] == 6 && t_args[6] == 0 && t_args[7] == 0);
    assert(SzEffect_Entry(1, 2, 3, 4, 5) == 0x704 && t_args[4] == 5 && t_args[5] == 0 && t_args[6] == 0);
    assert(SzFile_InqRequest(9) == 0x705 && t_args[0] == 9 && t_args[1] == 0);
    assert(Ta_IsUnitControl() == 0x706);
    assert(TaView_ReqFireView(1, 2, 0xab, 0xcd) == 0x707 && t_args[2] == 0xab && t_args[3] == 0xcd);
    assert(TaHand_GetCardSummonCost(0x12345) == 0x708 && t_args[0] == 0x345);
    data_00317b68[0] = 0x4242;
    lSzDuel_SortBag();
    assert(t_id == 9 && t_args[0] == 0x4242 && t_args[1] == 854 && t_args[2] == 2 && t_callback == lSzDuel_SortFunc1);
    memset(&source, 0, sizeof(source));
    source.unknown_78 = 77;
    source.unknown_82_bits_0_11 = -5;
    source.unknown_82_bits_12_15 = 7;
    assert(SzDuel_DrawCard_Pipe(3, &source) == 0x70a && t_pointer == (void *)source.unknown_48);
    assert(t_args[0] == 3 && t_args[2] == -5 && t_args[3] == 77 && t_args[4] == 0 && t_args[5] == 0);
    assert(SzMsg_Initialize() == gSzMsg_ml + 636 && t_pointer == (void *)gSzMsg_ml);
    assert(t_args[0] == 0 && t_args[1] == 636);
    /* Stored words and bytes. */
    SzModel_SetDrawCache(31);
    SzMsg_SetDefPrioZ(-32);
    assert(gSzModel_pfhCache[0] == 31 && data_0031c2e4[0] == -32);
    data_0032a220[0] = 5;
    assert(RhGeom_Init() == 1 && data_0032a220[0] == 0);
    data_0032a220[0] = 6;
    assert(RhGeom_ResetMatrixStack() == 1 && data_0032a220[0] == 0);
    require_coordinate_no[0] = 7;
    GetAnyCoordinateInit();
    assert(require_coordinate_no[0] == 0);
    SzData_SetRoseFlag(0x1f3);
    assert(gSzData_bRoseFlag[0] == 0xf3);
    data_00299eda[0] = -9;
    assert(Taku_Get_Duel5_flg() == -9);
    Taku_Set_Duel5_flg();
    assert(data_00299eda[0] == 0);
    winCount[0] = -1234;
    assert(Taku_Get_WinCount() == -1234);
    Taku_Vector_Set(&to, &from);
    assert(to.x == 1.0f && to.y == -2.0f && to.z == 3.5f);
    for (value = 0; value < 31; value++) {
        assert(Ta_LkCheckFlag(1 << value, value) == 1 && Ta_LkCheckFlag(~(1 << value), value) == 0);
    }
}

static void check_gated_comparison(void)
{
    static const int flags[] = {0, 1, -1, INT_MIN, INT_MAX};
    static const int words[] = {INT_MIN, 0, INT_MAX};
    struct DotrMiscObject expected;
    size_t f, a, b;
    for (f = 0; f < sizeof(flags) / sizeof(flags[0]); f++) {
        isWithAudio[0] = flags[f];
        for (a = 0; a < sizeof(words) / sizeof(words[0]); a++) {
            for (b = 0; b < sizeof(words) / sizeof(words[0]); b++) {
                memset(&audioDec, 0xa5, sizeof(audioDec));
                audioDec.unknown_48 = words[a];
                audioDec.unknown_54 = words[b];
                memcpy(&expected, &audioDec, sizeof(expected));
                assert(isAudioOK() == (!flags[f] || words[b] >= words[a]));
                assert(memcmp(&expected, &audioDec, sizeof(expected)) == 0);
                assert(isWithAudio[0] == flags[f]);
            }
        }
    }
}

struct DotrMiscEntry80 require_coordinate[256];
unsigned short gSzData_ConstDeck[51][41];

static void check_table_helpers(void)
{
    static struct DotrMiscEntry80 expected[256];
    int row, column;
    memset(require_coordinate, 0x5a, sizeof(require_coordinate));
    memcpy(expected, require_coordinate, sizeof(expected));
    require_coordinate_no[0] = 254;
    assert(BookingGetAnyCoordinate(0x1234) == 1 && BookingGetAnyCoordinate(-9) == 1 && require_coordinate_no[0] == 256);
    expected[254].unknown_00 = 0x1234;
    expected[254].unknown_04 = -1;
    expected[255].unknown_00 = -9;
    expected[255].unknown_04 = -1;
    assert(memcmp(require_coordinate, expected, sizeof(expected)) == 0);
    /* A full or negative count is rejected by the unsigned test. */
    assert(BookingGetAnyCoordinate(1) == 0 && require_coordinate_no[0] == 256);
    require_coordinate_no[0] = -1;
    assert(BookingGetAnyCoordinate(1) == 0 && require_coordinate_no[0] == -1 && memcmp(require_coordinate, expected, sizeof(expected)) == 0);

    for (row = 0; row < 51; row++) {
        for (column = 0; column < 41; column++) {
            gSzData_ConstDeck[row][column] = (unsigned short)(0xf000 | (row * 41 + column));
        }
    }
    assert(SzData_GetConstDeck(-1, 0) == -1 && SzData_GetConstDeck(51, 0) == -1);
    assert(SzData_GetConstDeck(0, -1) == -1 && SzData_GetConstDeck(0, 41) == -1);
    for (row = 0; row < 51; row++) {
        for (column = 0; column < 41; column++) {
            assert(SzData_GetConstDeck(row, column) == row * 41 + column);
        }
    }
}

static void check_bit_words(void)
{
    struct DotrMiscFlagObject object;
    int position;
    /* 0x24c780 sets bit (10 - position) in the low field, or in the field
     * above bit 11 when the second argument is zero; 0x24ea10 reads it. */
    for (position = 0; position <= 10; position++) {
        unsigned low = ConvUnitIDtoFlagList(0x00400000u, 1, position);
        unsigned high = ConvUnitIDtoFlagList(0x00000400u, 0, position);
        assert(low == (0x00400000u | (1u << (10 - position))));
        assert(high == (0x00000400u | (1u << (21 - position))));
        assert(Ta_GetAgreeOwnerIDon22bitFlagLst(0x100 | position, low) == 1 && Ta_GetAgreeOwnerIDon22bitFlagLst(position, high) == 1);
        assert(Ta_GetAgreeOwnerIDon22bitFlagLst(0x100 | position, ~low) == 0 && Ta_GetAgreeOwnerIDon22bitFlagLst(position, ~high) == 0);
    }
    assert(Ta_IsUnitControl2(0) == -1);
    memset(&object, 0, sizeof(object));
    object.team = 9;
    assert(Ta_IsUnitControl2(&object) == 9);
    object.status.unknown_bit_1 = -1;
    assert(Ta_IsUnitControl2(&object) == 0);
    object.team = 0;
    assert(Ta_IsUnitControl2(&object) == 1);
    object.status.unknown_bit_1 = 0;
    object.status.unknown_bit_0 = -1;
    assert(Ta_IsUnitControl2(&object) == 0);
}

struct DotrMiscRows *gpShFaceIndex[2];

static void check_phase_interpolation(void)
{
    /* With these exactly representable synthetic table values, a positive
     * nonspecial folded phase p has result (3*p + 16)/65536. The nonzero
     * table[0] distinguishes the explicit zero result from interpolation. */
    static const struct { int phase; int numerator; } samples[] = {
        {1, 19}, {15, 61}, {16, 64}, {17, 67}, {1023, 3085},
        {16383, 49165}, {16385, 49165}, {32767, 19},
        {-1, -19}, {-17, -67}, {-16385, -49165},
        {65537, 19}, {-65537, -19}, {INT_MAX, -19}
    };
    int i;
    size_t j;
    for (i = 0; i <= 1024; i++) data_00294050[i] = (float)(3 * i + 1) / 4096.0f;
    assert(RhMath_SinI(0) == 0.0f && RhMath_SinI(-32768) == 0.0f);
    assert(!signbit(RhMath_SinI(-32768)));
    assert(RhMath_SinI(INT_MIN) == 0.0f);
    assert(RhMath_SinI(16384) == 1.0f && RhMath_SinI(-16384) == -1.0f);
    for (j = 0; j < sizeof(samples) / sizeof(samples[0]); j++)
        assert(RhMath_SinI(samples[j].phase) == (float)samples[j].numerator / 65536.0f);
    /* The sibling shifts the incoming phase by one quarter before narrowing.
     * Keep this signed addition within the defined host range. */
    {
        static const int shifted_inputs[] = {
            INT_MIN, INT_MAX - 16384, -65537, -32768, -16385, -16384,
            -16383, -1, 0, 1, 16383, 16384, 16385, 32767, 65537
        };
        assert(RhMath_CosI(0) == 1.0f);
        assert(RhMath_CosI(-16384) == 0.0f && RhMath_CosI(16384) == 0.0f);
        assert(!signbit(RhMath_CosI(16384)));
        assert(RhMath_CosI(32768) == -1.0f);
        for (j = 0; j < sizeof(shifted_inputs) / sizeof(shifted_inputs[0]); j++)
            assert(RhMath_CosI(shifted_inputs[j]) == RhMath_SinI(shifted_inputs[j] + 16384));
    }
    /* 0x20b730, branch coverage: zero, both signs, the quarter-turn special case, the fold
     * above a quarter turn, the half turn that narrows to -32768, and interpolated values
     * against the integer-phase sibling. The 0x1a22c0 stub truncates (assumed). */
    {
        static const float radians[] = {
            0.0f, 1.5707964f, -1.5707964f, 3.1415927f, 0.1f, -0.1f, 2.0f, -2.0f, 1.0f, 4.0f, 1.5700f
        };
        assert(RhMath_Sin(0.0f) == 0.0f);
        assert(RhMath_Sin(1.5707964f) == 1.0f);
        assert(RhMath_Sin(3.1415927f) == 0.0f && !signbit(RhMath_Sin(3.1415927f)));
        assert(RhMath_Sin(0.1f) > 0.0f && RhMath_Sin(-0.1f) < 0.0f);
        assert(RhMath_Sin(2.0f) > 0.0f && RhMath_Sin(2.0f) < 1.0f);
        for (j = 0; j < sizeof(radians) / sizeof(radians[0]); j++) {
            float scaled = radians[j] * 65536.0f / 6.2831855f + 0.5f;
            float got = RhMath_Sin(radians[j]);
            assert(a22c0_last == scaled);
            assert(got == RhMath_SinI((short)(int)scaled));
        }
    }
    /* 0x20b620, branch coverage: zero, the quarter turn (special case 0), the half turn
     * (narrows to -32768, folds to -1), both signs, the fold above a quarter turn and
     * interpolated values against the integer-phase sibling. */
    {
        static const float radians[] = {
            0.0f, 1.5707964f, -1.5707964f, 3.1415927f, 0.1f, -0.1f, 2.0f, -2.0f, 1.0f, 4.0f, 1.5700f
        };
        assert(RhMath_Cos(0.0f) == 1.0f);
        assert(RhMath_Cos(-1.5707964f) == 0.0f);
        assert(RhMath_Cos(0.1f) > 0.0f && RhMath_Cos(0.1f) < 1.0f);
        assert(RhMath_Cos(2.0f) < 0.0f && RhMath_Cos(-2.0f) < 0.0f);
        for (j = 0; j < sizeof(radians) / sizeof(radians[0]); j++) {
            float scaled = radians[j] * 65536.0f / 6.2831855f + 0.5f + 16384.0f;
            float got = RhMath_Cos(radians[j]);
            assert(got == RhMath_SinI((short)(int)scaled));
        }
    }
    for (i = 0; i <= 1024; i++)
        assert(data_00294050[i] == (float)(3 * i + 1) / 4096.0f);
}

static void check_mode_value(void)
{
    static const int values[] = {INT_MIN, -1, 0, 1, INT_MAX};
    static const int other_modes[] = {INT_MIN, -1, 0, 3, INT_MAX};
    size_t i, j;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        assert(Ta_GetEOwnerSide(values[i], 1) == values[i]);
        assert(Ta_GetEOwnerSide(values[i], 2) == (values[i] == 0));
        for (j = 0; j < sizeof(other_modes) / sizeof(other_modes[0]); j++)
            assert(Ta_GetEOwnerSide(values[i], other_modes[j]) == -1);
    }
}

static void check_oldest_queue_address(void)
{
    static const int capacities[] = {1, 3, 7};
    struct DotrMiscQueue queue, before;
    size_t c;
    int index, count, capacity, oldest;
    memset(&queue, 0, sizeof(queue));
    queue.unknown_04 = 0x5000;
    memcpy(&before, &queue, sizeof(queue));
    /* An empty queue returns before a zero capacity can reach division. */
    assert(voBufGetTag(&queue) == 0 && memcmp(&queue, &before, sizeof(queue)) == 0);
    for (c = 0; c < sizeof(capacities) / sizeof(capacities[0]); c++) {
        capacity = capacities[c];
        queue.unknown_00 = 0x1000;
        queue.unknown_10 = capacity;
        for (index = 0; index < capacity; index++) {
            queue.unknown_08 = index;
            for (count = 1; count <= capacity; count++) {
                queue.unknown_0c = count;
                memcpy(&before, &queue, sizeof(queue));
                oldest = (index + capacity - count) % capacity;
                assert(voBufGetTag(&queue) == 0x5000 + oldest * 0x4ce40);
                assert(memcmp(&queue, &before, sizeof(queue)) == 0);
            }
        }
    }
}

static void check_ring_queue_rows(void)
{
    static struct DotrMiscLarge ring;
    static union { int words[4]; short halves[8]; signed char bytes[16]; } half_rows, byte_rows;
    static int elements[3][0x4ce40 / 4];
    struct DotrMiscRows rows;
    struct DotrMiscQueue queue;
    struct DotrMiscHalfRow half;
    struct DotrMiscByteRow bytes;
    char *where;
    int i;

    /* Ring: capacity 100, write offset 90, 30 bytes used. */
    ring.unknown_50000 = 90;
    ring.unknown_50004 = 30;
    ring.unknown_50008 = 100;
    where = 0;
    assert(readBufBeginPut(&ring, &where) == 70 && where == (char *)&ring + 90);
    assert(readBufBeginGet(&ring, &where) == 30 && where == (char *)&ring + 60);
    assert(readBufEndPut(&ring, 25) == 25 && ring.unknown_50000 == 15 && ring.unknown_50004 == 55);
    /* Requests are capped by the free and the used byte counts. */
    assert(readBufEndPut(&ring, 1000) == 45 && ring.unknown_50000 == 60 && ring.unknown_50004 == 100);
    where = (char *)&ring;
    assert(readBufBeginPut(&ring, &where) == 0 && where == (char *)&ring);
    assert(readBufEndGet(&ring, 40) == 40 && ring.unknown_50004 == 60);
    assert(readBufEndGet(&ring, 1000) == 60 && ring.unknown_50004 == 0 && ring.unknown_50000 == 60);
    assert(readBufBeginGet(&ring, &where) == 0 && where == (char *)&ring);

    /* Queue header. */
    for (i = 0; i < 3; i++) {
        elements[i][0] = 0x55;
    }
    memset(&queue, 0x5a, sizeof(queue));
    voBufCreate(&queue, 0x1000, (DotrMiscAddress)elements, 3);
    assert(queue.unknown_00 == 0x1000 && queue.unknown_04 == (DotrMiscAddress)elements);
    assert(queue.unknown_08 == 0 && queue.unknown_0c == 0 && queue.unknown_10 == 3);
    assert(elements[0][0] == 0 && elements[1][0] == 0 && elements[2][0] == 0);
    queue.unknown_08 = 2;
    assert(voBufGetData(&queue) == 0x1000 + 2 * 0x195000);
    queue.unknown_0c = 3;
    assert(voBufGetData(&queue) == 0);

    /* Row copies from the two pointers of a set. */
    for (i = 0; i < 8; i++) {
        half_rows.halves[i] = (short)(-300 - i);
    }
    for (i = 0; i < 16; i++) {
        byte_rows.bytes[i] = (signed char)(i - 8);
    }
    rows.unknown_00 = (unsigned char *)&half_rows;
    rows.unknown_04 = (unsigned char *)&byte_rows;
    gpShFaceIndex[0] = 0;
    gpShFaceIndex[1] = &rows;
    ShAnime_GetPartsRect(1, 0x101, &half);
    ShAnime_GetPatInfo(0x101, 0x101, &bytes);
    assert(memcmp(&half, &half_rows.halves[4], sizeof(half)) == 0);
    assert(memcmp(&bytes, &byte_rows.bytes[8], sizeof(bytes)) == 0);
}

static void check_byte_helpers(void)
{
    unsigned char five[5] = {1, 2, 255, 4, 5};
    union { unsigned char bytes[60]; int words[15]; } object, expected;
    struct DotrMiscNibbles nibbles[12];
    struct DotrMiscWordAt8 word;
    int i;
    assert(TaLogic_GetArrayLength(five) == 2);
    five[2] = 3;
    assert(TaLogic_GetArrayLength(five) == 5);
    five[0] = 255;
    assert(TaLogic_GetArrayLength(five) == 0);

    memset(&object, 0x5a, sizeof(object));
    expected = object;
    TaLogic_SetRootTreeDat(object.bytes, 0x1234, 0x3, 0xa);
    expected.bytes[0] = 0xa3;
    for (i = 1; i <= 12; i++) {
        expected.words[i] = 0;
    }
    expected.words[13] = 0x1234;
    assert(memcmp(&object, &expected, sizeof(expected)) == 0);

    memset(nibbles, 0, sizeof(nibbles));
    assert(TaLogic_GetMinGoalCount(nibbles) == 12);
    nibbles[0].col = 15;
    nibbles[5].row = 15;
    nibbles[11].col = 15;
    nibbles[11].row = 15;
    assert(TaLogic_GetMinGoalCount(nibbles) == 9);

    memset(&word, 0, sizeof(word));
    word.goalLoc1 = 0x12345678;
    TaLogic_InitRogicControl(&word, 2);
    assert(word.goalLoc1 == 0x12345678);
    TaLogic_InitRogicControl(&word, 1);
    assert(word.goalLoc1 == 0xffff5678u);
    word.goalLoc1 = 0x12345678;
    TaLogic_InitRogicControl(&word, 0);
    assert(word.goalLoc1 == 0x1234ffffu && word.unknown_00[7] == 0);
}

struct DotrMiscScanEntry gSzText_aItem[1024];
struct DotrMiscAccumulator gSzText_tl[1];

static void check_scan_and_accumulator(void)
{
    static struct DotrMiscScanEntry expected[1024];
    int i;
    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
    assert(SzText_SleepRequest(-1, -1, 1) == 0 && SzText_FreeRequest(-1, -1) == 0);
    /* Entries 1..6 and 1023: active unless the word is zero (4) or bit 0 is
     * clear (5). Field = index & 1, halfword = index / 2. */
    for (i = 1; i < 1024; i++) {
        if (i > 6 && i != 1023) continue;
        gSzText_aItem[i].unknown_00 = i == 4 ? 0 : 0x100 + i;
        gSzText_aItem[i].unknown_08.unknown_bit_0 = i == 5 ? 0 : -1;
        gSzText_aItem[i].unknown_08.unknown_bits_3_15 = (short)(i & 1 ? 5 : 7);
        gSzText_aItem[i].unknown_0a = (short)(i / 2);
    }
    memcpy(expected, gSzText_aItem, sizeof(expected));
    assert(SzText_SleepRequest(8, -1, 1) == 0 && SzText_SleepRequest(7, 9, 1) == 0);
    assert(memcmp(gSzText_aItem, expected, sizeof(expected)) == 0);
    assert(SzText_SleepRequest(5, -1, 0x100) == 1);
    expected[1].unknown_08.unknown_bit_1 = -1;
    expected[3].unknown_08.unknown_bit_1 = -1;
    expected[1023].unknown_08.unknown_bit_1 = -1;
    assert(memcmp(gSzText_aItem, expected, sizeof(expected)) == 0);
    assert(SzText_SleepRequest(-1, 1, 0) == 1);
    expected[3].unknown_08.unknown_bit_1 = 0;
    assert(memcmp(gSzText_aItem, expected, sizeof(expected)) == 0);
    assert(SzText_FreeRequest(7, 3) == 1);
    expected[6].unknown_08.unknown_bit_0 = 0;
    assert(memcmp(gSzText_aItem, expected, sizeof(expected)) == 0 && SzText_FreeRequest(7, 3) == 0);
    assert(SzText_FreeRequest(-1, -1) == 1);
    /* Entry 4 has a zero word and keeps its bit. */
    for (i = 1; i < 1024; i++) {
        if (i != 4) expected[i].unknown_08.unknown_bit_0 = 0;
    }
    assert(memcmp(gSzText_aItem, expected, sizeof(expected)) == 0);

    /* Accumulator: limit = size * 32; requests beyond it return -1. */
    assert(SzText_InitClutManage(0x100, 3) == 96 && gSzText_tl[0].unknown_00 == 0x100);
    assert(gSzText_tl[0].unknown_02 == 96 && gSzText_tl[0].unknown_04 == 0);
    assert(SzText_GetClutGsAddr(90) == 0x100 && SzText_GetClutGsAddr(7) == -1 && gSzText_tl[0].unknown_04 == 90);
    assert(SzText_GetClutGsAddr(6) == 0x100 + 90 && gSzText_tl[0].unknown_04 == 96 && SzText_GetClutGsAddr(1) == -1);
}

static int bit_calls, bit_args[3], bit_result, bit_only4;
static const unsigned char *bit_by_lo;

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    bit_args[0] = a;
    bit_args[1] = b;
    bit_args[2] = bit;
    bit_calls++;
    if (bit_by_lo) return bit_by_lo[b];
    if (bit_only4) return bit == 4;
    return bit_result;
}

static int kind_query_calls, kind_query_value, kind_query_args[2];

int SzDuel_GetUnitConstKind(int a, int b)
{
    kind_query_args[0] = a;
    kind_query_args[1] = b;
    kind_query_calls++;
    return kind_query_value;
}

static int shifted_kind;

int SzDuel_GetUnitKind(int a, int b)
{
    (void)a;
    (void)b;
    return shifted_kind;
}

unsigned int FusionDat[26540];

static int word_calls, word_args[2], word_results[2];

int TaHand_GetCardAPDP(int id)
{
    assert(word_calls < 2);
    word_args[word_calls] = id;
    return word_results[word_calls++];
}

static int wide_args[8], wide_which, notify_args[3], notify_calls;
static int pair_result, single_result, other_result, pair_trace[8], pair_count;

static unsigned short *wide_pointer;
static int wide_result = 0x7001;
int TaLogic_IsPassRoot2(int item, int c, int d, int e, int f, unsigned short *g)
{
    int v[] = {item, c, d, e, f, *g, 0, 0};
    memcpy(wide_args, v, sizeof(v));
    wide_pointer = g;
    wide_which = 1;
    return wide_result;
}

int TaLogic_IsRunPassRoot2(int item, int c, int d, int e, int f, int g, int h)
{
    int v[] = {item, c, d, e, f, g, h, 0};
    memcpy(wide_args, v, sizeof(v));
    wide_which = 2;
    return 0x7002;
}

int TaLogic_GetRoot2(int item, int c, int d, int e, int f, int g, int h, unsigned char i)
{
    int v[] = {item, c, d, e, f, g, h, i};
    memcpy(wide_args, v, sizeof(v));
    wide_which = 3;
    return 0x7003;
}

static int nibble_args[8], nibble_calls, nibble_fill_ok, nibble_zero_bytes;

/* Records the call, checks all twelve bytes arrive as 0xff, then clears the
 * requested number of leading bytes so TaLogic_GetMinGoalCount sees that many pairs. */
int func_002792c0_nibbles(int item, struct DotrMiscNibbles *bytes, int c, int d, int e, int f,
                          unsigned short g, unsigned char h)
{
    int i;
    int v[] = {item, c, d, e, f, g, h};
    memcpy(nibble_args, v, sizeof(v));
    nibble_args[7] = (int)(bytes != NULL);
    nibble_calls++;
    nibble_fill_ok = 1;
    for (i = 0; i < 12; i++) {
        if (((unsigned char *)bytes)[i] != 0xff) {
            nibble_fill_ok = 0;
        }
    }
    memset(bytes, 0, (size_t)nibble_zero_bytes);
    return 0x7004;
}

int TaUtil_DoOpen(int a, int b, int c)
{
    notify_args[0] = a;
    notify_args[1] = b;
    notify_args[2] = c;
    notify_calls++;
    return 0;
}

int Ta_GetHandEnchant(int a, int b) { pair_trace[pair_count++] = 1; pair_trace[pair_count++] = a; pair_trace[pair_count++] = b; return pair_result; }
int TaEnc_IsEncDo(int a) { pair_trace[pair_count++] = 2; pair_trace[pair_count++] = a; return single_result; }
int Fusion(int a, int b) { pair_trace[pair_count++] = 3; pair_trace[pair_count++] = a; pair_trace[pair_count++] = b; return other_result; }

/* Fusion is stubbed above for the 0x2789f0 caller test, so the real
 * body is compiled here under another name. */
#define Fusion real_func_002495a0
#include "../src/ee/small_misc/func_002495a0.c"
#undef Fusion

static int combine(int a, int b, int first, int single, int other)
{
    pair_result = first;
    single_result = single;
    other_result = other;
    pair_count = 0;
    return TaLogic_GetHandFusion((unsigned short)a, (unsigned short)b);
}

/* Forwarders 0x279850, 0x279c40 and 0x279230; 0x24fd10; 0x2789f0; 0x24e880. */
static void check_forwarders_and_words(void)
{
    struct DotrMiscWords4 words;
    struct DotrMiscBitWords bits;
    int source[41];
    int i;

    unsigned short halfword = 14;

    assert(TaLogic_IsPassRoot(2, 9, 10, 11, 12, 13, &halfword) == 0x7001 && wide_which == 1 && wide_pointer == &halfword);
    assert(word_a == 2 && word_b == 9 && wide_args[0] == 0x5540 && wide_args[1] == 10 && wide_args[5] == 14);
    assert(TaLogic_IsRunPassRoot(3, 8, 10, 11, 12, 13, 14, 15) == 0x7002 && wide_which == 2);
    assert(word_a == 3 && word_b == 8 && wide_args[0] == 0x5540 && wide_args[1] == 10 && wide_args[6] == 15);
    assert(TaLogic_GetRoot(4, 7, 10, 11, 12, 13, 14, 15, 0xfe) == 0x7003 && wide_which == 3);
    assert(word_a == 4 && word_b == 7 && wide_args[0] == 0x5540 && wide_args[6] == 15 && wide_args[7] == 0xfe);

    /* 0x24fd10 writes nothing unless the real 0x24fda0 returns exactly one. */
    memset(&words, 0x55, sizeof(words));
    notify_calls = 0;
    gate_result = 2;
    assert(Ta_IsVirus(2, 9, &words) == 0 && words.unknown_00[0] == 0x55555555 && notify_calls == 0);
    gate_result = 1;
    assert(Ta_IsVirus(2, 9, &words) == 1 && word_a == 2 && word_b == 9 && item_seen == 0x5540);
    assert(words.unknown_00[0] == 112 && words.unknown_00[1] == 0 && words.unknown_00[2] == 2 && words.unknown_00[3] == 0);
    assert(notify_calls == 1 && notify_args[0] == 2 && notify_args[1] == 9 && notify_args[2] == 1);
    assert(Ta_IsVirus(0, 10, &words) == 1 && words.unknown_00[2] == 0x800 && notify_calls == 2);
    gate_result = 0xfdd0;

    /* 0x2789f0: either halfword 999 returns 999 without calls. */
    assert(combine(999, 5, 1, 1, 1) == 999 && pair_count == 0);
    assert(combine(5, 999, 1, 1, 1) == 999 && pair_count == 0 && combine(0x103e7, 5, 1, 1, 1) == 999);
    /* A first result within 16 bits is returned when the second call gives one. */
    assert(combine(5, 6, 0xabcd, 1, 0x7777) == 0xabcd && pair_count == 5);
    assert(pair_trace[0] == 1 && pair_trace[1] == 5 && pair_trace[2] == 6 && pair_trace[3] == 2 && pair_trace[4] == 6);
    assert(combine(5, 6, 0xabcd, 2, 0x17777) == 0x7777 && pair_count == 8);
    assert(pair_trace[5] == 3 && pair_trace[6] == 5 && pair_trace[7] == 6);
    /* Bits above 16 (including a negative result) skip the second call. */
    assert(combine(5, 6, 0x10000, 1, -1) == 0xffff && pair_count == 6 && pair_trace[3] == 3);
    assert(combine(5, 6, -1, 1, 0x1234) == 0x1234 && pair_count == 6);

    /* 0x24e880: stops at the first nonnegative word; word +0 is untouched. */
    bits.unknown_00 = 0x1234;
    bits.unknown_04 = bits.unknown_08 = -1;
    source[0] = 0;
    Ta_SetEffectObjByField(&bits, source);
    assert(bits.unknown_00 == 0x1234 && bits.unknown_04 == 0 && bits.unknown_08 == 0);
    /* The test is low byte + 7 * (word >> 8) < 29 with an arithmetic shift.
     * Words 0x800000xx take the first branch and 0xffffffxx with a low byte
     * of at least 36 the second. Host cases keep both shift counts within
     * 0..30; the target instruction would use the count's low five bits. */
    for (i = 0; i < 40; i++) {
        source[i] = i < 20 ? (int)(0x80000000u | (unsigned)i) : -1;
    }
    source[3] = 3; /* nonnegative: ends the scan after three words */
    Ta_SetEffectObjByField(&bits, source);
    assert(bits.unknown_04 == ((1 << 28) | (1 << 27) | (1 << 26)) && bits.unknown_08 == 0);
    source[3] = (int)0xffffff23u; /* 35 - 7 = 28: still the first branch */
    source[40] = 0;
    Ta_SetEffectObjByField(&bits, source);
    /* Indices 0..19 set bits 28..9 of word +4; 20..39 set bits 30..11 of word +8. */
    assert(bits.unknown_04 == 0x1ffffe00 && bits.unknown_08 == 0x7ffff800 && bits.unknown_00 == 0x1234);
}

static int scores[2][11], kind_result, kind_args[2], tail_args[5], tail_calls;
static int fill_argument, fill_calls, attach_args[3], attach_calls;
static struct DotrMiscRecord24 *attach_record;

int SzDuel_GetUnitSummonOrder(int a, int b)
{
    assert(a >= 0 && a < 2 && b >= 0 && b < 11);
    return scores[a][b];
}

static int kind_cell = -1, kind_open_y = -1;
int SzDuel_GetField(int x, int y)
{
    kind_args[0] = x;
    kind_args[1] = y;
    if (kind_cell >= 0 && x * 7 + y == kind_cell) return 8;
    if (y == kind_open_y) return 0;
    return kind_result;
}

static int set_calls[49][3], set_count;
int SzDuel_SetField(int x, int y, int value)
{
    assert(set_count < 49);
    set_calls[set_count][0] = x;
    set_calls[set_count][1] = y;
    set_calls[set_count][2] = value;
    set_count++;
    return 0;
}

static int list_results[8], list_calls[8], list_count, list_shrink = -1;
static DotrMiscByteList *list_owner;
int SzEffect_IsUse(int index)
{
    assert(list_count < 8);
    list_calls[list_count] = index;
    if (list_shrink == list_count) list_owner->count = 1;
    return list_results[list_count++];
}

static int dee_calls[8][2], dee_count;
int SzDuel_AddGrave(int index, int value)
{
    assert(dee_count < 8);
    dee_calls[dee_count][0] = index;
    dee_calls[dee_count][1] = value;
    dee_count++;
    return 0;
}

static float float_a[4], float_b[4];
static int float_log[2], float_log_count;
void *SzView_GetCP(int index)
{
    float_log[float_log_count++] = 0x1f9d30 + index;
    return float_a;
}
float *func_001faf60_floats(int index)
{
    float_log[float_log_count++] = 0x1faf60 + index;
    return float_b;
}

int Ta_Do_MoveEfc(int a, int b, int c, int d, struct DotrMiscWords4 *out)
{
    int v[] = {a, b, c, d, out != 0};
    memcpy(tail_args, v, sizeof(v));
    tail_calls++;
    return 0x4f83;
}

void TaHand_GetNextHand(int a, struct DotrMiscScratch32 *out)
{
    fill_argument = a;
    fill_calls++;
    memset(out, 0x5a, sizeof(*out));
}

void TaLogic_InitTree(struct DotrMiscRecord24 *record, unsigned short d, int e, int c)
{
    attach_record = record;
    attach_args[0] = d;
    attach_args[1] = e;
    attach_args[2] = c;
    attach_calls++;
}

static int rule_check(struct DotrMiscRule *rule, int id, unsigned result)
{
    word_calls = 0;
    word_results[0] = (int)result;
    return TaLogic_IsKeepEnchant(rule, id, 0x1234);
}

/* 0x24e950, 0x24efb0, 0x24f150, 0x24f780, 0x278c80 and 0x279170. */
static void check_moves_and_rules(void)
{
    struct DotrMiscRule rule = {7, 36, 0x8000};
    struct DotrMiscWords4 words;
    unsigned short used = 0, count = 3;
    int word = 0x200401; /* positions 0, 0x100 and 0x10a */
    int i, j;

    /* 0x24e950: modes 0/2 move the low byte down/up, 1/3 the upper part;
     * results below 0 or above 6 give -1, as do other modes. */
    assert(Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 2, 0) == 0x0303 && Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 5, 0) == 0x0300);
    assert(Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 6, 0) == -1 && Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 3, 1) == 0x0005);
    assert(Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 4, 1) == -1 && Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 1, 2) == 0x0306);
    assert(Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 2, 2) == -1 && Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 3, 3) == 0x0605);
    assert(Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 4, 3) == -1 && Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 0, 4) == -1);
    assert(Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, 0, -1) == -1 && Ta_GetLocDistanceFromCenterOnRangeXY(0x0305, -1, 0) == 0x0306);

    /* 0x24efb0 / 0x24f150 over the real bit helpers. */
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 11; j++) {
            scores[i][j] = 100;
        }
    }
    scores[0][0] = 50;
    scores[1][0] = 20;
    scores[1][10] = 20; /* tie: the earlier position wins */
    assert(Ta_GetMinimumOrderUnitFromFlagList(word) == 0x100 && Ta_GetMaximumOrderUnitFromFlagList(word) == 0);
    scores[0][0] = 255;
    scores[1][0] = 255;
    scores[1][10] = 300;
    assert(Ta_GetMinimumOrderUnitFromFlagList(word) == -1 && Ta_GetMaximumOrderUnitFromFlagList(word) == 0x10a);
    scores[0][0] = scores[1][0] = scores[1][10] = -1;
    assert(Ta_GetMaximumOrderUnitFromFlagList(word) == -1 && Ta_GetMinimumOrderUnitFromFlagList(word) == 0);
    assert(Ta_GetMinimumOrderUnitFromFlagList(0) == -1 && Ta_GetMaximumOrderUnitFromFlagList(0) == -1);

    /* 0x24f780: kind 9 and a successful 0x24fd10 return 0x10001; everything
     * else is forwarded to 0x24f830. */
    memset(&words, 0, sizeof(words));
    kind_result = 9;
    gate_result = 1;
    tail_calls = 0;
    assert(Ta_Do_CompMove(2, 9, 30, 40, &words) == 0x10001 && tail_calls == 0);
    assert(kind_args[0] == 30 && kind_args[1] == 40 && words.unknown_00[0] == 112);
    gate_result = 0;
    assert(Ta_Do_CompMove(2, 9, 30, 40, &words) == 0x4f83 && tail_calls == 1);
    assert(tail_args[0] == 2 && tail_args[1] == 9 && tail_args[2] == 30 && tail_args[3] == 40 && tail_args[4] == 1);
    kind_result = 8;
    gate_result = 1;
    item_seen = 0;
    assert(Ta_Do_CompMove(2, 9, 30, 40, &words) == 0x4f83 && tail_calls == 2 && item_seen == 0);
    gate_result = 0xfdd0;

    /* 0x278c80: a nonzero fifth argument or an exhausted pool returns null
     * after the fill call; otherwise the new record is passed on. */
    fill_calls = attach_calls = 0;
    assert(TaLogic_GetFusionTree2(11, -1, 13, 0x1abcd, 1, &used, &count) == 0);
    assert(fill_calls == 1 && fill_argument == 11 && attach_calls == 0 && used == 0 && count == 3);
    assert(TaLogic_GetFusionTree2(11, -1, 13, 0x1abcd, 0, &used, &count) == DOTR_MISC_RECORDS_01ED0000 + 3);
    assert(fill_calls == 2 && attach_calls == 1 && attach_record == DOTR_MISC_RECORDS_01ED0000 + 3);
    assert(attach_args[0] == 0xabcd && attach_args[1] == 0 && attach_args[2] == 13 && used == 24 && count == 4);
    used = 0xffff;
    assert(TaLogic_GetFusionTree2(11, -1, 13, 14, 0, &used, &count) == 0 && attach_calls == 1 && count == 4);

    /* 0x279170: a null rule or another identifier accepts without a call. */
    word_calls = 0;
    assert(TaLogic_IsKeepEnchant(0, 7, 0) == 1 && rule_check(&rule, 8, 0) == 1 && word_calls == 0);
    assert(rule_check(&rule, 7, 0x7fff0000u) == 0 && word_calls == 1 && word_args[0] == 0x1234);
    assert(rule_check(&rule, 7, 0x8000ffffu) == 1 && rule_check(&rule, 7, 0xffff0000u) == 1);
    rule.unknown_02 = 37;
    assert(rule_check(&rule, 7, 0x8001ffffu) == 0 && rule_check(&rule, 7, 0x80000000u) == 1);
    assert(rule_check(&rule, 7, 0) == 1 && word_calls == 1);
    rule.unknown_02 = 35;
    assert(rule_check(&rule, 7, 0) == 0 && word_calls == 0);
    rule.unknown_02 = 38;
    assert(rule_check(&rule, 7, 0) == 0 && word_calls == 0);
}

static int random_value, random_calls;

int func_0019a5c8(void)
{
    random_calls++;
    return random_value;
}

/* Stubs for the callees of ShFace_DrawOne (0x2460f0). */
static struct DotrMiscFaceFix face_fix[8];
static struct DotrMiscFaceAnm face_anm[8];
static int face_fix_calls, face_anm_calls, face_anm_frame[8], face_prim_calls;
static struct {
    struct DotrMiscEntry52 *entry;
    int rect0, x, y;
} face_prim[8];

void ShAnime_GetFixInfo(int set, int row, unsigned char index, struct DotrMiscFaceFix *out)
{
    assert(set == 1 && row == 2 && index == face_fix_calls && index < 8);
    face_fix_calls++;
    *out = face_fix[index];
}

void ShAnime_GetAnmDat(int set, int row, unsigned char index, int frame, struct DotrMiscFaceAnm *out)
{
    assert(set == 1 && row == 2 && index < 8);
    face_anm_frame[index] = frame;
    face_anm_calls++;
    *out = face_anm[index];
}

void ShFace_Prim(struct DotrMiscEntry52 *entry, struct DotrMiscHalfRow *rect, int x, int y)
{
    assert(face_prim_calls < 8);
    face_prim[face_prim_calls].entry = entry;
    face_prim[face_prim_calls].rect0 = rect->unknown_values[0];
    face_prim[face_prim_calls].x = x;
    face_prim[face_prim_calls].y = y;
    face_prim_calls++;
}

static void face_part(int i, int flags, int kind, int count, int delay, int row, int x, int y)
{
    memset(&face_fix[i], 0, sizeof(face_fix[i]));
    face_fix[i].flags = (unsigned char)flags;
    if (flags & 0x80) {
        ((struct DotrMiscFaceFixAnm *)&face_fix[i])->kind = (unsigned short)kind;
        ((struct DotrMiscFaceFixAnm *)&face_fix[i])->count = (unsigned short)count;
        face_anm[i].delay = (unsigned char)delay;
        face_anm[i].row = (unsigned char)row;
        face_anm[i].x = (unsigned short)x;
        face_anm[i].y = (unsigned short)y;
    } else {
        face_fix[i].row = (unsigned char)row;
        face_fix[i].x = (unsigned short)x;
        face_fix[i].y = (unsigned short)y;
    }
}

static void check_face_draw_one(void)
{
    static short halves[8 * 4];
    static unsigned char bytes[4 * 8];
    static struct DotrMiscRows face_rows;
    struct DotrMiscRows *saved = gpShFaceIndex[1];
    struct DotrMiscEntry52 entry;
    unsigned char *timer = &entry.unknown_04[7];
    unsigned char *frame = &entry.unknown_04[23];
    int i;

    for (i = 0; i < 32; i++) {
        halves[i] = (short)((i / 4) * 10 + i % 4);
    }
    face_rows.unknown_00 = (unsigned char *)halves;
    face_rows.unknown_04 = bytes;
    gpShFaceIndex[1] = &face_rows;
    random_value = 0x250; /* (0x250 >> 4) = 37: wait0 = 37 + 37, wait1 = 37 & 7 */

    /* Null entry: only the three random numbers are drawn. */
    random_calls = face_fix_calls = face_anm_calls = face_prim_calls = 0;
    ShFace_DrawOne(0);
    assert(random_calls == 3 && face_fix_calls == 0 && face_prim_calls == 0);

    /* A pattern row with no parts: nothing is asked for or drawn. */
    memset(&entry, 0, sizeof(entry));
    entry.unknown_04[0] = 1;
    entry.unknown_04[1] = 2;
    bytes[2 * 8] = 0;
    random_calls = 0;
    ShFace_DrawOne(&entry);
    assert(random_calls == 3 && face_fix_calls == 0 && face_anm_calls == 0 && face_prim_calls == 0);

    /* Six parts: a fixed one, then five animated ones (kinds 0 and 1 running, kind 2 not). */
    bytes[2 * 8] = 6;
    entry.unknown_04[2] = 3;
    face_part(0, 0x00, 0, 0, 0, 3, 100, 200);
    face_part(1, 0x80, 0, 3, 10, 4, 11, 12);  /* timer 0: frame 0 -> 1; reload = delay */
    face_part(2, 0x80, 1, 2, 20, 5, 21, 22);  /* timer 1 -> 0: frame 1 -> 2 wraps to 0;
                                               * reload = delay + wait1 */
    face_part(3, 0x80, 2, 4, 30, 6, 31, 32);  /* kind not running: frame reset; reload = delay */
    face_part(4, 0x80, 0, 5, 40, 7, 41, 42);  /* timer 5 -> 4: frame kept, no reload */
    face_part(5, 0x80, 0, 0, 50, 1, 51, 52);  /* no frames: frame wraps to 0, nothing drawn */
    timer[0] = 0; frame[0] = 0;
    timer[1] = 1; frame[1] = 1;
    timer[2] = 0; frame[2] = 7;
    timer[3] = 5; frame[3] = 2;
    timer[4] = 0; frame[4] = 9;
    timer[5] = 77; frame[5] = 88;
    random_calls = face_fix_calls = face_anm_calls = face_prim_calls = 0;
    ShFace_DrawOne(&entry);
    assert(random_calls == 3 && face_fix_calls == 6 && face_anm_calls == 4 && face_prim_calls == 5);
    assert(face_prim[0].entry == &entry && face_prim[0].rect0 == 30 && face_prim[0].x == 100 && face_prim[0].y == 200);
    assert(face_prim[1].rect0 == 40 && face_prim[1].x == 11 && face_prim[1].y == 12 && face_anm_frame[1] == 1);
    assert(face_prim[2].rect0 == 50 && face_prim[2].x == 21 && face_prim[2].y == 22 && face_anm_frame[2] == 0);
    assert(face_prim[3].rect0 == 60 && face_prim[3].x == 31 && face_prim[3].y == 32 && face_anm_frame[3] == 0);
    assert(face_prim[4].entry == &entry && face_prim[4].rect0 == 70 && face_prim[4].x == 41 && face_anm_frame[4] == 2);
    assert(timer[0] == 10 && frame[0] == 1);
    assert(timer[1] == 20 + 5 && frame[1] == 0);
    assert(timer[2] == 30 && frame[2] == 0);
    assert(timer[3] == 4 && frame[3] == 2);
    assert(timer[4] == 0 && frame[4] == 0);
    assert(timer[5] == 77 && frame[5] == 88);

    /* One kind 0 part whose frame wraps to 0 at its count: reload = delay + wait0; and the
     * boundary one below the count, which keeps the advanced frame. */
    bytes[2 * 8] = 1;
    face_part(0, 0x80, 0, 1, 10, 4, 11, 12);
    timer[0] = 0; frame[0] = 0;
    random_calls = face_fix_calls = face_anm_calls = face_prim_calls = 0;
    ShFace_DrawOne(&entry);
    assert(face_fix_calls == 1 && face_prim_calls == 1 && face_anm_frame[0] == 0 && frame[0] == 0 && timer[0] == 10 + 74);
    face_part(0, 0x80, 0, 2, 10, 4, 11, 12);
    timer[0] = 1; frame[0] = 0;
    random_calls = face_fix_calls = face_anm_calls = face_prim_calls = 0;
    ShFace_DrawOne(&entry);
    assert(face_prim_calls == 1 && face_anm_frame[0] == 1 && frame[0] == 1 && timer[0] == 10 && timer[1] == 25);
    gpShFaceIndex[1] = saved;
}

static int pick(int word, int value)
{
    random_value = value;
    random_calls = 0;
    return Ta_GetRandomUnit(word);
}

/* 0x24ec50, 0x24ebd0 and 0x278d20 with the real 0x24e910 and 0x278d90. */
static void check_bit_pick_and_insert(void)
{
    static const unsigned char partial[5] = {1, 2, 0xff, 9, 8};
    static const unsigned char full[5] = {1, 2, 3, 4, 5};
    static const unsigned char empty[5] = {0xff, 2, 3, 4, 5};
    unsigned char bytes[7];
    int word = 0x200401; /* bits 21, 10 and 0 */

    assert(Ta_GetUnitOwnerIDFromLstCount(0x200000, 0) == 0 && Ta_GetUnitOwnerIDFromLstCount(0x000800, 0) == 10);
    assert(Ta_GetUnitOwnerIDFromLstCount(0x000400, 0) == 0x100 && Ta_GetUnitOwnerIDFromLstCount(1, 0) == 0x10a);
    assert(Ta_GetUnitOwnerIDFromLstCount(word, 0) == 0 && Ta_GetUnitOwnerIDFromLstCount(word, 1) == 0x100 && Ta_GetUnitOwnerIDFromLstCount(word, 2) == 0x10a);
    assert(Ta_GetUnitOwnerIDFromLstCount(word, 3) == -1 && Ta_GetUnitOwnerIDFromLstCount(word, -1) == -1 && Ta_GetUnitOwnerIDFromLstCount(0, 0) == -1);
    assert(Ta_GetUnitOwnerIDFromLstCount(0x400000, 0) == -1 && Ta_GetUnitOwnerIDFromLstCount(-1, 21) == 0x10a && Ta_GetUnitOwnerIDFromLstCount(-1, 22) == -1);

    /* 0x24ebd0: no set bit returns before the random call. */
    assert(pick(0, 0) == -1 && random_calls == 0 && pick(0x400000, 0) == -1 && random_calls == 0);
    assert(pick(word, 0) == 0 && random_calls == 1 && pick(word, 1) == 0x100 && pick(word, 2) == 0x10a);
    assert(pick(word, 3) == 0 && pick(word, 5) == 0x10a && pick(word, INT_MAX) == 0x100);
    /* A negative remainder selects nothing. */
    assert(pick(word, -1) == -1 && random_calls == 1);

    memset(bytes, 0x77, sizeof(bytes));
    memcpy(bytes + 1, partial, 5);
    TaLogic_AddEmptyCombo(bytes + 1, 0x1ab);
    assert(bytes[0] == 0x77 && bytes[1] == 0xab && bytes[2] == 1 && bytes[3] == 2);
    assert(bytes[4] == 0xff && bytes[5] == 9 && bytes[6] == 0x77);
    memcpy(bytes + 1, full, 5);
    TaLogic_AddEmptyCombo(bytes + 1, 0);
    assert(memcmp(bytes + 1, full, 5) == 0 && bytes[0] == 0x77 && bytes[6] == 0x77);
    memcpy(bytes + 1, empty, 5);
    TaLogic_AddEmptyCombo(bytes + 1, 6);
    assert(bytes[1] == 6 && bytes[2] == 0xff && bytes[3] == 2 && bytes[4] == 3 && bytes[5] == 4);
}

static int forward_args[4];

int TaLogic_IsUnitWin2(int item, int b, int c, int d)
{
    forward_args[0] = item;
    forward_args[1] = b;
    forward_args[2] = c;
    forward_args[3] = d;
    return 0x6677;
}

/* 0x277c60, 0x2781b0, 0x2781f0, 0x2788d0 and 0x279130 with stubbed dependencies. */
static void check_scan_gate_and_records(void)
{
    static const int results[] = {1, 0, 2, -1, INT_MIN};
    struct DotrMiscLevelEntry entries[23];
    unsigned short used, count, id;
    int i;

    assert(sizeof(struct DotrMiscLevelEntry) == 4 && sizeof(struct DotrMiscRecord24) == 24);

    /* Every matching entry is lowered, never raised; the scan passes them. */
    for (i = 0; i < 23; i++) {
        entries[i].unknown_00 = (unsigned short)(100 + i);
        entries[i].unknown_02 = 50;
        entries[i].unknown_03 = 0xee;
    }
    entries[3].unknown_00 = 7;
    entries[9].unknown_00 = 7;
    entries[9].unknown_02 = 5;
    entries[21].unknown_00 = 7;
    entries[22].unknown_00 = 7; /* past the 22 scanned entries */
    assert(TaLogic_IsAdjustUnitLst(entries, 7, 20) == 1);
    assert(entries[3].unknown_02 == 20 && entries[9].unknown_02 == 5 && entries[21].unknown_02 == 20);
    assert(entries[22].unknown_02 == 50 && entries[4].unknown_02 == 50 && entries[3].unknown_03 == 0xee);
    assert(TaLogic_IsAdjustUnitLst(entries, 7, 20) == 1 && entries[3].unknown_02 == 20); /* equal: no store */
    assert(TaLogic_IsAdjustUnitLst(entries, 8, 0) == 0 && TaLogic_IsAdjustUnitLst(entries, 122, 0) == 0);
    /* An unmatched 0xffff entry ends the scan; a matched one does not. */
    entries[5].unknown_00 = 0xffff;
    assert(TaLogic_IsAdjustUnitLst(entries, 7, 10) == 1);
    assert(entries[3].unknown_02 == 10 && entries[9].unknown_02 == 5 && entries[21].unknown_02 == 20);
    entries[6].unknown_00 = 0xffff;
    assert(TaLogic_IsAdjustUnitLst(entries, 0xffff, 1) == 1);
    assert(entries[5].unknown_02 == 1 && entries[6].unknown_02 == 1 && entries[21].unknown_02 == 20);
    assert(TaLogic_IsAdjustUnitLst(entries, -1, 0) == 0); /* the identifier is compared as a full word */
    /* A negative level is below every byte and stores its low byte. */
    assert(TaLogic_IsAdjustUnitLst(entries, 7, -2) == 1 && entries[3].unknown_02 == 0xfe);

    /* 0x2781b0: a nonzero third argument skips the call. */
    bit_calls = 0;
    assert(TaLogic_IsJudgeUnitDanger(11, 22, 1) == 1 && TaLogic_IsJudgeUnitDanger(11, 22, INT_MIN) == 1 && bit_calls == 0);
    for (i = 0; i < 5; i++) {
        bit_calls = 0;
        bit_result = results[i];
        assert(TaLogic_IsJudgeUnitDanger(11, 22, 0) == (results[i] == 1));
        assert(bit_calls == 1 && bit_args[0] == 11 && bit_args[1] == 22 && bit_args[2] == 2);
    }

    /* 0x2781f0: the packed pair is split by an arithmetic shift and a byte mask. */
    assert(TaLogic_IsUnitWin(0x1234, 5, 6, 7) == 0x6677 && word_a == 0x12 && word_b == 0x34);
    assert(forward_args[0] == 0x5540 && forward_args[1] == 5 && forward_args[2] == 6 && forward_args[3] == 7);
    assert(TaLogic_IsUnitWin(-1, 0, 0, 0) == 0x6677 && word_a == -1 && word_b == 0xff);

    /* 0x2788d0: only the address is formed; the records are not accessed. */
    used = 0;
    count = 0;
    assert(TaLogic_TreeAloc(&used, &count) == DOTR_MISC_RECORDS_01ED0000 && used == 24 && count == 1);
    assert(TaLogic_TreeAloc(&used, &count) == DOTR_MISC_RECORDS_01ED0000 + 1 && used == 48 && count == 2);
    used = 0x10000 - 56;
    count = 0xffff;
    assert(TaLogic_TreeAloc(&used, &count) == DOTR_MISC_RECORDS_01ED0000 + 0xffff);
    assert(used == 0x10000 - 32 && count == 0);
    used = 0x10000 - 55;
    count = 9;
    assert(TaLogic_TreeAloc(&used, &count) == 0 && used == 0x10000 - 55 && count == 9);
    used = 0xffff;
    assert(TaLogic_TreeAloc(&used, &count) == 0 && used == 0xffff && count == 9);

    /* 0x279130: upper half of the first result, lower half of the second. */
    id = 0xfedc;
    word_calls = 0;
    word_results[0] = (int)0x89ab1234u;
    word_results[1] = 0x5678cdef;
    assert(TaLogic_GetEnchantQue2(-1, &id) == 0x89abcdefu);
    assert(word_calls == 2 && word_args[0] == 0xfedc && word_args[1] == 0xfedc && id == 0xfedc);
}

/* 0x2508d0, 0x250300, 0x250900, 0x278150 and 0x202e30 on synthetic values. */
static void check_clamps_and_modes(void)
{
    static const int kinds[] = {19, 20, 18, 21, 0, 0xffff};
    static const int modes[] = {0, 2, 10, 1, 3, 9, 11, 0xffff};
    struct DotrMiscFlagObject object;
    struct DotrMiscFormat format;
    int i, j;

    assert(GetParCap(INT_MIN) == 0 && GetParCap(-1) == 0 && GetParCap(0) == 0);
    assert(GetParCap(1) == 1 && GetParCap(9999) == 9999);
    assert(GetParCap(10000) == 9999 && GetParCap(INT_MAX) == 9999);

    /* Positive direction: sum narrowed to 16 bits, then capped at 128. */
    assert(Ta_CalcCtrlRate(100, 20, 1) == 120 && Ta_CalcCtrlRate(100, 28, 1) == 128);
    assert(Ta_CalcCtrlRate(100, 29, INT_MAX) == 128 && Ta_CalcCtrlRate(0xffff, 0, 1) == 128);
    assert(Ta_CalcCtrlRate(0xffff, 2, 1) == 1 && Ta_CalcCtrlRate(5, -3, 1) == 2);
    assert(Ta_CalcCtrlRate(1, -3, 1) == 128); /* 0xfffe after narrowing */
    /* Otherwise: difference when the amount is smaller, else zero. */
    assert(Ta_CalcCtrlRate(100, 20, 0) == 80 && Ta_CalcCtrlRate(100, 99, -1) == 1);
    assert(Ta_CalcCtrlRate(100, 100, 0) == 0 && Ta_CalcCtrlRate(100, 101, INT_MIN) == 0);
    assert(Ta_CalcCtrlRate(0xffff, 0xfffe, 0) == 1 && Ta_CalcCtrlRate(0, 0, 0) == 0);
    assert(Ta_CalcCtrlRate(200, 0, 0) == 200);  /* no cap on this path */
    assert(Ta_CalcCtrlRate(10, -5, 0) == 15 && Ta_CalcCtrlRate(0xffff, -2, 0) == 1);

    /* 0x250860: limits 0 and 1 are lower bounds, five others upper bounds. */
    {
        static const int upper[] = {6, 7, 500, 1500, 9999};
        static const int other[] = {-1, 2, 5, 8, 499, 501, 1499, 9998, 10000, INT_MIN, INT_MAX};
        static const int values[] = {INT_MIN, -1, 0, 1, 2, 6, 7, 8, 500, 1500, 9999, 10000, INT_MAX};
        for (j = 0; j < (int)(sizeof(values) / sizeof(values[0])); j++) {
            int v = values[j];
            assert(GetCap(0, v) == (v < 0 ? 0 : v) && GetCap(1, v) == (v < 1 ? 1 : v));
            for (i = 0; i < 5; i++) {
                assert(GetCap(upper[i], v) == (v > upper[i] ? upper[i] : v));
            }
            for (i = 0; i < 11; i++) {
                assert(GetCap(other[i], v) == 0);
            }
        }
    }

    /* 0x250900: byte +0x45 zero and byte +0x44 zero or one. */
    memset(&object, 0xff, sizeof(object));
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            object.team = (unsigned char)(i == 3 ? 0xff : i);
            object.unknown_45[0] = (unsigned char)(j == 2 ? 0x80 : j);
            assert(TaUtil_IsVarInfoLeader(&object) == (j == 0 && i < 2));
        }
    }
    assert(offsetof(struct DotrMiscFlagObject, unknown_45) == 0x45);

    /* 0x278150: modes 0 and 1 require the equal value; other modes accept. */
    for (i = -1; i <= 3; i++) {
        assert(TaLogic_IsStatusMatch(i, 0) == (i == 0) && TaLogic_IsStatusMatch(i, 1) == (i == 1));
        assert(TaLogic_IsStatusMatch(i, 2) == 1 && TaLogic_IsStatusMatch(i, 3) == 1 && TaLogic_IsStatusMatch(i, -1) == 1);
    }

    /* 0x202e30: shift 4 or 8 by +0x40, scale 4 or 2 by +0x42, product / 256,
     * raised to at least one; unknown selectors give zero. */
    assert(offsetof(struct DotrMiscFormat, unknown_40) == 0x40);
    assert(offsetof(struct DotrMiscFormat, unknown_42) == 0x42);
    memset(&format, 0xff, sizeof(format));
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 8; j++) {
            int expected = 0;
            if (i < 2 && j < 3) {
                expected = i == 1 ? 1 : (j == 0 ? 4 : 2);
            }
            format.unknown_40 = (unsigned short)kinds[i];
            format.unknown_42 = (unsigned short)modes[j];
            assert(SzText_GetClutSize(&format) == expected);
        }
    }
}

static int item_args[4], item_calls, long_args[15], long_calls;
static struct DotrMiscByteFlagEntry *flag_entry;
static int flag_lookup[2];

int TaLogic_IsUnitWin3(int item, int base, int x, int y)
{
    item_args[0] = item;
    item_args[1] = base;
    item_args[2] = x;
    item_args[3] = y;
    item_calls++;
    return 0x4455;
}

int TaLogic_GetNearUnitPickUpTree(int a, int zero, int b, int c, int d, int e, int f, int g,
                  int h, int zero2, int k, int l, int i, int j, int m)
{
    int values[15];
    values[0] = a; values[1] = zero; values[2] = b; values[3] = c; values[4] = d;
    values[5] = e; values[6] = f; values[7] = g; values[8] = h; values[9] = zero2;
    values[10] = k; values[11] = l; values[12] = i; values[13] = j; values[14] = m;
    memcpy(long_args, values, sizeof(long_args));
    long_calls++;
    return 0x7788;
}

struct DotrMiscByteFlagEntry *func_002008a0_bytes(int group, int id)
{
    flag_lookup[0] = group;
    flag_lookup[1] = id;
    return flag_entry;
}

/* 0x278510, 0x2787f0, 0x2770e0 and 0x202bf0 with stubbed dependencies. */
static void check_selectors_and_flags(void)
{
    static const int forwarded[15] = {1, 0, 2, 3, 4, 5, 6, 7, 0xfff8, 0, 11, 12, 9, 10, 13};
    struct DotrMiscPairItem a, b;
    struct DotrMiscByteFlagEntry entry;
    unsigned char *bytes = (unsigned char *)&entry;
    int i;

    /* 0x278510: -1 and a rejected pair use 0x278420 with c; otherwise 0x278250. */
    item_calls = bit_calls = 0;
    memset(forward_args, 0, sizeof(forward_args));
    assert(TaLogic_IsUnitWin4(21, -1, 22, 23, 24, 0) == 0x4455 && item_calls == 1 && bit_calls == 0);
    assert(item_args[0] == 21 && item_args[1] == 22 && item_args[2] == 23 && item_args[3] == 24);
    bit_result = 0;
    assert(TaLogic_IsUnitWin4(21, 0x1234, 22, 23, 24, 0) == 0x4455 && item_calls == 2 && bit_calls == 1);
    assert(bit_args[0] == 0x12 && bit_args[1] == 0x34 && bit_args[2] == 2 && forward_args[0] == 0);
    bit_result = 1;
    assert(TaLogic_IsUnitWin4(21, 0x1234, 22, 23, 24, 0) == 0x6677 && item_calls == 2);
    assert(forward_args[0] == 21 && forward_args[1] == 0x1234 && forward_args[2] == 23 && forward_args[3] == 24);
    bit_result = 0;
    bit_calls = 0;
    assert(TaLogic_IsUnitWin4(21, 0x1234, 22, 23, 24, 1) == 0x6677 && bit_calls == 0 && item_calls == 2);

    /* 0x2787f0: b's entry is tried first, then a's. */
    assert(offsetof(struct DotrMiscPairItem, cardEffects) == 8 && offsetof(struct DotrMiscPairItem, kind) == 0x3e);
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    a.cardEffects.data = 0xa111;
    b.cardEffects.data = 0xb222;
    a.kind = 9;
    b.kind = 17;
    assert(TaLogic_GetActOfs(&a, &b) == 0);
    b.cardEffects.type = (27 << 8) | 0x40 | 9; /* bits above the low six are ignored */
    a.cardEffects.type = (27 << 8) | 17;
    assert(TaLogic_GetActOfs(&a, &b) == 0xb222);
    a.kind = 0x20 | 9; /* upper bits set: b's entry fails, a's is used */
    assert(TaLogic_GetActOfs(&a, &b) == 0xa111);
    b.kind = 0x80 | 17;
    assert(TaLogic_GetActOfs(&a, &b) == 0);
    b.kind = 16;
    assert(TaLogic_GetActOfs(&a, &b) == 0);
    b.kind = 17;
    a.cardEffects.type = (28 << 8) | 17;
    assert(TaLogic_GetActOfs(&a, &b) == 0);
    a.cardEffects.type = 0x80000000u | (27 << 8) | 17; /* unsigned kind */
    assert(TaLogic_GetActOfs(&a, &b) == 0);

    /* 0x2770e0: inserted zeros, narrowed h and reordered stack arguments. */
    long_calls = 0;
    assert(TaLogic_GetNearUnitL2(1, 2, 3, 4, 5, 6, 7, 0xfff8, 9, 10, 11, 12, 13) == 0x7788 && long_calls == 1);
    for (i = 0; i < 15; i++) {
        assert(long_args[i] == forwarded[i]);
    }

    /* 0x202bf0: range guards, missing and high-bit entries, then the bits. */
    flag_entry = &entry;
    memset(&entry, 0xff, sizeof(entry));
    entry.unknown_00 = 0x1234;
    flag_lookup[0] = flag_lookup[1] = -5;
    assert(SzText_LoadRequest(-1, 3) == 0 && SzText_LoadRequest(10, 3) == 0 && SzText_LoadRequest(2, -1) == 0);
    assert(flag_lookup[0] == -5 && bytes[8] == 0xff);
    bytes[8] = 0xfe;
    assert(SzText_LoadRequest(9, 0) == 0x1234 && flag_lookup[0] == 9 && flag_lookup[1] == 0);
    assert(bytes[8] == 0xfd && bytes[9] == 0xff && entry.unknown_04 == -1);
    bytes[8] = 0;
    assert(SzText_LoadRequest(0, 7) == 0x1234 && bytes[8] == 1);
    entry.unknown_00 = (int)0x80001234u;
    bytes[8] = 0xfe;
    assert(SzText_LoadRequest(0, 7) == 0 && bytes[8] == 0xfe);
    flag_entry = 0;
    assert(SzText_LoadRequest(0, 7) == 0);
}

static int pair_sides[2][11], pair_values[2][11];

int func_0024de70_pair(int a, int b)
{
    assert(a >= 0 && a < 2 && b >= 0 && b < 11);
    return pair_sides[a][b];
}

int Ta_GetAPPlusDP(int a, int b)
{
    assert(a >= 0 && a < 2 && b >= 0 && b < 11);
    return pair_values[a][b];
}

/* 0x24dd80, and 0x24f060 and 0x24f200 over the real bit-word routines
 * 0x24e910, 0x24ec50 and 0x24c780. Positions 0, 3 and 12 of the word are
 * the packed pairs 0x000, 0x003 and 0x101. */
static void check_modes_and_position_scans(void)
{
    static const int expected[4][6] = {
        /* mode: 0, 1, 2, 3, 4, -1 */
        {0, 1, 0, 1, 0, 0}, /* a 0, b 0 */
        {0, 0, 1, 1, 0, 0}, /* a 0, b 1 */
        {0, 0, 1, 1, 0, 0}, /* a 1, b 0 */
        {0, 1, 0, 1, 0, 0}, /* a 1, b 1 */
    };
    static const int modes[6] = {0, 1, 2, 3, 4, -1};
    const int word = (1 << 21) | (1 << 18) | (1 << 9);
    int i, m;
    for (i = 0; i < 4; i++) {
        for (m = 0; m < 6; m++) {
            assert(Ta_SideMatch(i >> 1, i & 1, modes[m]) == expected[i][m]);
        }
    }
    assert(Ta_SideMatch(0, 7, 2) == 1 && Ta_SideMatch(1, 7, 2) == 0 && Ta_SideMatch(7, 7, 1) == 1);

    memset(scores, 0, sizeof(scores));
    memset(pair_sides, 0, sizeof(pair_sides));
    scores[0][0] = 9;
    scores[0][3] = 5;
    scores[1][1] = 5;
    pair_sides[1][1] = 1;
    assert(Ta_GetMinimumOrderUnitFromFlagList2(0, 1) == -1 && Ta_GetMinimumOrderUnitFromFlagList2(1 << 22, 1) == -1);
    assert(Ta_GetMinimumOrderUnitFromFlagList2(word, 1) == 0x101); /* equal score, matching side */
    assert(Ta_GetMinimumOrderUnitFromFlagList2(word, 0) == 0x003); /* lower score, side 0 */
    assert(Ta_GetMinimumOrderUnitFromFlagList2(word, 2) == 0x000); /* later positions never match */
    scores[1][1] = 6;
    assert(Ta_GetMinimumOrderUnitFromFlagList2(word, 1) == 0x000);
    scores[0][0] = 256; /* above the initial 255: skipped */
    scores[0][3] = 255;
    scores[1][1] = 255;
    assert(Ta_GetMinimumOrderUnitFromFlagList2(word, 5) == 0x003);
    memset(scores, 0, sizeof(scores));

    memset(pair_values, 0, sizeof(pair_values));
    pair_values[0][0] = 7;
    pair_values[0][3] = 4;
    pair_values[1][1] = 4;
    assert(Ta_GetMinimumParamUnitFromFlagList(0) == 0);
    assert(Ta_GetMinimumParamUnitFromFlagList(word) == ((1 << 18) | (1 << 9)));
    pair_values[1][1] = 2;
    assert(Ta_GetMinimumParamUnitFromFlagList(word) == (1 << 9));
    pair_values[0][0] = 2;
    pair_values[0][3] = 2;
    assert(Ta_GetMinimumParamUnitFromFlagList(word) == word);
    pair_values[0][0] = 0x10000; /* above the initial 0xffff */
    pair_values[0][3] = 0xffff;  /* equal to it: accumulated */
    pair_values[1][1] = 0x10000;
    assert(Ta_GetMinimumParamUnitFromFlagList(word) == (1 << 18));
    pair_values[0][3] = -1;
    assert(Ta_GetMinimumParamUnitFromFlagList(word) == (1 << 18));
}

unsigned short gAnmPat_Bakura[0x10000];
static int grid_args[2], inner_args[6], inner_calls;

static int grid_word = 0x12345, grid_word_calls;
static int grid_pair_on, grid_pair_calls, grid_pair[2], grid_pair_args[2][2];
int SzDuel_GetUnitLoc(int a, int b)
{
    grid_args[0] = a;
    grid_args[1] = b;
    grid_word_calls++;
    if (grid_pair_on) {
        grid_pair_args[grid_pair_calls & 1][0] = a;
        grid_pair_args[grid_pair_calls & 1][1] = b;
        return grid_pair[grid_pair_calls++ & 1];
    }
    return grid_word;
}

static int score_args[4], score_calls;
static const int *score_by_hi;
static int thunk_args[2];
static int thunk_by_lo[8];

int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    score_args[0] = a;
    score_args[1] = b;
    score_args[2] = c;
    score_args[3] = d;
    score_calls++;
    return score_by_hi[a];
}

int func_00250360_pair(int a, int b)
{
    thunk_args[0] = a;
    thunk_args[1] = b;
    return thunk_by_lo[b];
}

int TaLogic_DoGetShortRoot(int a, int *word, unsigned char *byte, int b, int c, int d, int e, int f)
{
    assert(*word == 0 && *byte == 255 && (void *)word != (void *)byte);
    inner_args[0] = a;
    inner_args[1] = b;
    inner_args[2] = c;
    inner_args[3] = d;
    inner_args[4] = e;
    inner_args[5] = f;
    inner_calls++;
    return 0x2468;
}

/* 0x2686f0, 0x2493a0, 0x276ec0, 0x27ac70 and 0x27b150. */
static void check_small_leaves(void)
{
    static const int mapped[4] = {56, 57, 54, 55};
    struct DotrMiscTaggedPair pair;
    unsigned short packed = 0xfe09, used, count;
    int i;
    for (i = 0; i < 4; i++) {
        assert(Ta_GetExzodiaPartsCardNo(i) == mapped[i]);
    }
    assert(Ta_GetExzodiaPartsCardNo(4) == 0 && Ta_GetExzodiaPartsCardNo(-1) == 0);

    for (i = 0; i < 6; i++) {
        EnchantId[100 + i] = (unsigned char)i;
        gAnmPat_Bakura[100 + i] = (unsigned short)(0xff00 + i);
    }
    assert(TaEnc_GetEncScore(100) == 0xff00 && TaEnc_GetEncScore(101) == 1000 && TaEnc_GetEncScore(104) == 1000);
    assert(TaEnc_GetEncScore(102) == 0 && TaEnc_GetEncScore(103) == 0 && TaEnc_GetEncScore(105) == 0);
    EnchantId[102] = 0xff;
    assert(TaEnc_GetEncScore(102) == 0);
    memset(EnchantId + 100, 0, 6);

    assert(offsetof(struct DotrMiscTaggedPair, loc) == 4 && offsetof(struct DotrMiscTaggedPair, atkDefPosition) == 0x10);
    memset(&pair, 0x5a, sizeof(pair));
    TaLogic_GetMinimumTask_DoNone(&packed, -1, &pair);
    assert(grid_args[0] == 0xfe && grid_args[1] == 9);
    assert(pair.unitOwnerId == 0xfe09 && pair.taskType == 6 && pair.unknown_03 == 0x5a);
    assert(pair.loc == 0x2345 && pair.unknown_06[0] == 0x5a && pair.unknown_06[9] == 0x5a && pair.atkDefPosition == 2);

    inner_calls = 0;
    assert(TaLogic_GetShortRoot(1, 2, 3, 4) == 0x2468 && inner_calls == 1);
    assert(inner_args[0] == 1 && inner_args[1] == 2 && inner_args[2] == 3 && inner_args[3] == 4);
    assert(inner_args[4] == 1 && inner_args[5] == 0);

    /* 0x241190 over 0x8078 halfwords and 0x27c670 over 22. */
    {
        static unsigned short block[0x8078 + 1];
        unsigned short list[23];
        memset(block, 0, sizeof(block));
        block[0] = 0x1111;
        block[1] = 0xbeef; /* excluded from the sum */
        block[0x8077] = 0x2222;
        block[0x8078] = 0x7777; /* past the summed range */
        assert(LkCheckData(block) == 0 && block[1] == (0x1111 ^ 0x2222 ^ 0x4c6b));
        assert(LkCheckData(block) == 1 && block[1] == (0x1111 ^ 0x2222 ^ 0x4c6b) && block[0x8078] == 0x7777);
        block[2] ^= 0x8000;
        assert(LkCheckData(block) == 0 && block[1] == (0x1111 ^ 0x2222 ^ 0x4c6b ^ 0x8000));
        for (i = 0; i < 23; i++) {
            list[i] = 0xffff;
        }
        assert(TaLogic_GetUnitCtlLstLen(list) == 0);
        list[0] = 0;
        list[21] = 0xfffe;
        list[22] = 5; /* past the 22 scanned halfwords */
        assert(TaLogic_GetUnitCtlLstLen(list) == 2);
        for (i = 0; i < 22; i++) {
            list[i] = (unsigned short)i;
        }
        assert(TaLogic_GetUnitCtlLstLen(list) == 22);
    }

    /* Addresses are compared only; the literal base is not host memory. */
    assert(sizeof(struct DotrMiscRecord56) == 56);
    used = 0;
    count = 0;
    assert(TaLogic_RootTreeAloc(&used, &count) == DOTR_MISC_RECORDS56_01ED0000 && used == 56 && count == 1);
    assert(TaLogic_RootTreeAloc(&used, &count) == DOTR_MISC_RECORDS56_01ED0000 + 1 && used == 112 && count == 2);
    used = 0x10000 - 56;
    count = 9;
    assert(TaLogic_RootTreeAloc(&used, &count) == DOTR_MISC_RECORDS56_01ED0000 + 9 && used == 0 && count == 10);
    used = 0x10000 - 55;
    assert(TaLogic_RootTreeAloc(&used, &count) == 0 && used == 0x10000 - 55 && count == 10);
}

static int bracket_log[4], bracket_count, port_args[2][4], port_calls, source_args[2];
static struct DotrMiscRequest request_seen;
static int *request_queue;
static int frame_step;
static struct DotrMiscMatrix *frame_local;

void func_0017b920(int unknown)
{
    assert(bracket_count < 4);
    bracket_log[bracket_count++] = unknown;
}

void func_0017b900(int unknown)
{
    assert(bracket_count < 4);
    bracket_log[bracket_count++] = -unknown;
}

void func_00190810(int a, int b, int c, int d)
{
    assert(port_calls < 2);
    port_args[port_calls][0] = a;
    port_args[port_calls][1] = b;
    port_args[port_calls][2] = c;
    port_args[port_calls][3] = d;
    port_calls++;
}

int Ta_IsActiveTrapB(int a, int b)
{
    source_args[0] = a;
    source_args[1] = b;
    return 0x1357;
}

void viBufPutTs(int *queue, struct DotrMiscRequest *request)
{
    request_queue = queue;
    request_seen = *request;
}

static struct DotrMiscFrame frame_fixture;

void RhView_GetWorldScreenMatrix(struct DotrMiscMatrix *m)
{
    assert(frame_step == 0 && m == &frame_fixture.unknown_30);
    frame_step++;
}

void RhGeom_GetScaleRotXYZTransMatrix(struct DotrMiscMatrix *out, struct DotrMiscFrame *frame, float *a, float *b)
{
    assert(frame_step == 1 && frame == &frame_fixture);
    assert(a == frame_fixture.unknown_10 && b == frame_fixture.unknown_20);
    assert((void *)out != (void *)&frame_fixture.unknown_30);
    out->unknown_00[0] = 5.0f;
    frame_local = out;
    frame_step++;
}

static int bc00_record, bc00_calls;
static struct DotrMiscMatrix *bc00_out, *bc00_in;
static float bc00_in_00;
void RhGeom_MulMatrix(struct DotrMiscMatrix *m, struct DotrMiscMatrix *other)
{
    if (bc00_record) {
        bc00_out = m; bc00_in = other; bc00_in_00 = other->unknown_00[0]; bc00_calls++;
        return;
    }
    assert(frame_step == 2 && m == &frame_fixture.unknown_30 && other == frame_local);
    assert(other->unknown_00[0] == 5.0f);
    frame_step++;
}

static int args7_seen[7], args7_calls, args7_ports, args7_owner_word;
static struct DotrMiscStream *args7_stream;

void func_00190810_args7(int a, int b, int c, int d, int e, int f, int g)
{
    int v[] = {a, b, c, d, e, f, g};
    memcpy(args7_seen, v, sizeof(v));
    args7_calls++;
    args7_ports = port_calls;
    args7_owner_word = args7_stream->unknown_00;
}

/* 0x214190 and 0x214210: delta added to nonzero words; flag adds the word before the base and
 * before each target. */
#define CHECK_FIXUP(FN, TYPE, WORDS, TAIL) do { \
    struct { DotrMiscAddress before; TYPE e[2]; } w; \
    DotrMiscAddress tgt[6]; \
    int n, t; \
    for (t = 0; t < 2; t++) { \
        memset(&w, 0, sizeof(w)); \
        for (n = 0; n < 6; n++) tgt[n] = 1000 + n; \
        w.before = 500; \
        w.e[0].unknown_00[0] = (DotrMiscAddress)&tgt[1] - 16; \
        w.e[1].unknown_00[(WORDS) - 1] = (DotrMiscAddress)&tgt[3] - 16; \
        w.e[1].unknown_00[0] = 0; \
        assert(FN(&w.e[0], 16, t == 0 ? 0 : 2, t) == 1); \
        if (t == 0) { \
            assert(w.before == 500 && w.e[0].unknown_00[0] == (DotrMiscAddress)&tgt[1] - 16); \
        } else { \
            assert(w.before == 516 && w.e[0].unknown_00[0] == (DotrMiscAddress)&tgt[1]); \
            assert(w.e[1].unknown_00[(WORDS) - 1] == (DotrMiscAddress)&tgt[3]); \
            assert(tgt[0] == 1016 && tgt[2] == 1018 && tgt[1] == 1001 && tgt[3] == 1003); \
            assert(w.e[0].unknown_00[1] == 0 && w.e[1].unknown_00[0] == 0); \
        } \
    } \
    /* flag clear, count 2: only nonzero words move, zero words and tails do not. */ \
    memset(&w, 0, sizeof(w)); \
    w.e[0].unknown_00[0] = 100; \
    w.e[1].unknown_00[(WORDS) - 1] = 200; \
    w.e[1].TAIL[0] = 77; \
    assert(FN(&w.e[0], 5, 2, 0) == 1); \
    assert(w.e[0].unknown_00[0] == 105 && w.e[1].unknown_00[(WORDS) - 1] == 205 && w.e[0].unknown_00[1] == 0); \
    assert(w.e[1].TAIL[0] == 77 && w.before == 0); \
    /* count 0 with flag still adjusts the word before the base. */ \
    assert(FN(&w.e[0], 5, 0, 1) == 1 && w.before == 5 && w.e[0].unknown_00[0] == 105); \
} while (0)

/* 0x20f250 and 0x213f10. */
static void check_packed_and_packets(void)
{
    struct {
        struct DotrMiscPackedHeader h;
        struct DotrMiscPackedEntry48 e[3];
    } blob;
    struct DotrMiscPacket128 packets[5];
    unsigned values[5] = {7, 7, 9, 9, 7};
    int i;

    memset(&blob, 0, sizeof(blob));
    blob.h.unknown_10 = (int)offsetof(__typeof__(blob), e);
    blob.h.unknown_58 = 2;
    blob.e[0].unknown_00 = 11;
    blob.e[0].unknown_04 = 0x100;
    blob.e[1].unknown_04 = 0x200;
    blob.e[2].unknown_04 = 0x300;
    blob.e[1].unknown_08[0] = 0x5a;
    RhText_BindAddress(&blob.h);
    assert(blob.h.unknown_0c == (DotrMiscAddress)&blob.h);
    assert(blob.e[0].unknown_04 == (DotrMiscAddress)&blob.h + 0x100 && blob.e[0].unknown_00 == 11);
    assert(blob.e[1].unknown_04 == (DotrMiscAddress)&blob.h + 0x200 && blob.e[1].unknown_08[0] == 0x5a);
    assert(blob.e[2].unknown_04 == 0x300);
    blob.h.unknown_0c = 0;
    blob.h.unknown_58 = 0;
    RhText_BindAddress(&blob.h);
    assert(blob.h.unknown_0c == (DotrMiscAddress)&blob.h && blob.e[2].unknown_04 == 0x300);
    assert(blob.e[0].unknown_04 == (DotrMiscAddress)&blob.h + 0x100);
    blob.h.unknown_58 = -1;
    RhText_BindAddress(&blob.h);
    assert(blob.e[0].unknown_04 == (DotrMiscAddress)&blob.h + 0x100);

    /* Run 7,7,9,9,7: the second 7 and second 9 are blanked, the last 7 is kept
     * (the remembered value after the 9s is 9). */
    memset(packets, 0x33, sizeof(packets));
    for (i = 0; i < 5; i++) packets[i].unknown_04 = values[i];
    lRhModelCpu_CleanupPacket(&packets[0], &packets[5]);
    assert(packets[0].unknown_04 == 7 && packets[0].unknown_00 == 0x33333333);
    assert(packets[1].unknown_00 == 0x10000000 && packets[1].unknown_04 == 0 && packets[1].unknown_08 == 0 && packets[1].unknown_0c == 0);
    assert(packets[1].unknown_10[0] == 0x33 && packets[1].unknown_10[0x6f] == 0x33);
    assert(packets[2].unknown_04 == 9 && packets[2].unknown_00 == 0x33333333);
    assert(packets[3].unknown_00 == 0x10000000 && packets[3].unknown_04 == 0);
    assert(packets[4].unknown_04 == 7 && packets[4].unknown_00 == 0x33333333);
    /* Initial remembered value is 0, and an empty range writes nothing. */
    memset(packets, 0x33, sizeof(packets));
    packets[0].unknown_04 = 0;
    lRhModelCpu_CleanupPacket(&packets[0], &packets[0]);
    assert(packets[0].unknown_00 == 0x33333333);
    lRhModelCpu_CleanupPacket(&packets[0], &packets[1]);
    assert(packets[0].unknown_00 == 0x10000000 && packets[1].unknown_00 == 0x33333333);
}

static void check_fixups(void)
{
    CHECK_FIXUP(lNjaReloc_AdjustMData2, struct DotrMiscFixup16, 2, unknown_08);
    CHECK_FIXUP(lNjaReloc_AdjustMData3, struct DotrMiscFixup24, 3, unknown_0c);
}

/* 0x214100 branch coverage (callee fixups run for real, count 1, flag 0). */
static void check_fixup_head(void)
{
    struct { DotrMiscAddress pre; DotrMiscFixupHead h; } hd;
    DotrMiscAddress w[4];
    int delta = 0x10;

    /* kind 3 (also with high bits set): 16-byte routine, two words. */
    memset(&hd, 0, sizeof(hd));
    w[0] = 1; w[1] = 2; w[2] = 3; w[3] = 4;
    hd.h.data = (DotrMiscAddress)w - delta;
    hd.h.kind = 0xffc3;
    hd.pre = 100;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 1, 0) == 1);
    assert(hd.h.data == (DotrMiscAddress)w && hd.pre == 100);
    assert(w[0] == 1 + delta && w[1] == 2 + delta && w[2] == 3 && w[3] == 4);
    /* kind 7: 24-byte routine, three words. */
    w[0] = 1; w[1] = 2; w[2] = 3;
    hd.h.data = (DotrMiscAddress)w - delta;
    hd.h.kind = 7;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 1, 0) == 1);
    assert(w[0] == 1 + delta && w[1] == 2 + delta && w[2] == 3 + delta && w[3] == 4);
    /* zero data stays zero and is not passed on; flag 1 rebases the word before the header. */
    hd.h.data = 0;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 1, 1) == 1);
    assert(hd.h.data == 0 && hd.pre == 100 + delta && w[0] == 1 + delta);
    hd.h.kind = 3;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 1, 0) == 1 && hd.h.data == 0 && hd.pre == 100 + delta);
    /* other kinds change nothing but still return 1. */
    hd.h.data = 0x1234;
    hd.h.kind = 4;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 1, 0) == 1 && hd.h.data == 0x1234);
    hd.h.kind = 0x43;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 0, 0) == 1);
    hd.h.kind = 0x40;
    hd.h.data = 0x1234;
    assert(lNjaReloc_AdjustMotion(&hd.h, delta, 0, 0) == 1 && hd.h.data == 0x1234);
}

/* Ta_InitMazePiece and Ta_IsFinishEffect: branch coverage; TaTuto_Init036 and
 * TaTuto_BkupOriginalView: straight-line coverage. */
static void check_scans_and_copies(void)
{
    DotrMiscByteList list;
    float out[9];
    unsigned bits_a, bits_b;
    int i;

    kind_result = 7;
    set_count = 0;
    Ta_InitMazePiece();
    assert(set_count == 0 && kind_args[0] == 6 && kind_args[1] == 6);
    kind_result = 9;
    Ta_InitMazePiece();
    assert(set_count == 0);
    kind_result = 7;
    kind_cell = 3 * 7 + 5;
    Ta_InitMazePiece();
    assert(set_count == 1 && set_calls[0][0] == 3 && set_calls[0][1] == 5 && set_calls[0][2] == 7);
    kind_result = 8;
    kind_cell = -1;
    set_count = 0;
    Ta_InitMazePiece();
    assert(set_count == 49);
    for (i = 0; i < 49; i++) {
        assert(set_calls[i][0] == i / 7 && set_calls[i][1] == i % 7 && set_calls[i][2] == 7);
    }
    kind_result = 0;

    memset(&list, 0, sizeof(list));
    for (i = 0; i < 0xce; i++) list.entries[i] = (unsigned char)(i + 1);
    list_owner = &list;
    list_count = 0;
    assert(Ta_IsFinishEffect(&list) == 1 && list_count == 0);
    list.count = 3;
    memset(list_results, 0, sizeof(list_results));
    assert(Ta_IsFinishEffect(&list) == 1 && list_count == 3);
    assert(list_calls[0] == 1 && list_calls[1] == 2 && list_calls[2] == 3);
    list_count = 0;
    list_results[1] = 5;
    assert(Ta_IsFinishEffect(&list) == -1 && list_count == 2 && list_calls[1] == 2);
    /* The count is re-read on each iteration. */
    list_count = 0;
    list_results[1] = 0;
    list_shrink = 0;
    assert(Ta_IsFinishEffect(&list) == 1 && list_count == 1);
    list_shrink = -1;
    list.count = 0xce;
    list_count = 0;
    memset(list_results, 0, sizeof(list_results));
    list_results[7] = 1;
    assert(Ta_IsFinishEffect(&list) == -1 && list_count == 8);

    dee_count = 0;
    TaTuto_Init036();
    {
        static const int want[8][2] = {{0, 0}, {0, 3}, {0, 6}, {0, 700}, {1, 0}, {1, 3}, {1, 6}, {1, 699}};
        assert(dee_count == 8);
        for (i = 0; i < 8; i++) assert(dee_calls[i][0] == want[i][0] && dee_calls[i][1] == want[i][1]);
    }

    for (i = 0; i < 4; i++) {
        float_a[i] = (float)(i + 1);
        float_b[i] = (float)(10 + i);
    }
    {
        unsigned nan = 0x7fc00123u;
        memcpy(&float_b[2], &nan, sizeof(nan));
    }
    for (i = 0; i < 9; i++) out[i] = -1.0f;
    TaTuto_BkupOriginalView(out);
    assert(float_log_count == 2 && float_log[0] == 0x1f9d30 && float_log[1] == 0x1faf60);
    assert(memcmp(out, float_a, sizeof(float_a)) == 0 && memcmp(out + 4, float_b, sizeof(float_b)) == 0);
    bits_a = bits_b = 0;
    memcpy(&bits_a, &out[6], 4);
    memcpy(&bits_b, &float_b[2], 4);
    assert(bits_a == bits_b && out[8] == -1.0f);
}

/* 0x1cda10, 0x1cdd40, 0x1cdd90, 0x243230 and 0x243290. */
static void check_tables_and_stream(void)
{
    struct DotrMiscSlot160 slot;
    struct DotrMiscStream stream;
    unsigned char *bytes;
    int i, j, k;

    /* 0x1cda10: every one of the 32 elements is reset through 0x1cda60. */
    memset(gSzDuel_aCardRequest, 0x55, sizeof(gSzDuel_aCardRequest));
    for (i = 0; i < 32; i++) {
        *(int *)(gSzDuel_aCardRequest[i].unknown_bytes + 0x90) = 0x55;
    }
    SzDuel_InitCardRequest();
    assert(node_seen == (struct DotrMiscNode *)&gSzDuel_aCardRequest[31]);
    for (i = 0; i < 32; i++) {
        bytes = gSzDuel_aCardRequest[i].unknown_bytes;
        for (j = 0; j < 160; j++) {
            assert(bytes[j] == ((j >= 0x90 && j < 0x94) ? 0 : 0x55));
        }
    }

    /* 0x1cdd90: null, then every stored field, with the neighbours kept. */
    assert(SzDuel_ClearCardControl(0) == 0);
    memset(&slot, 0xaa, sizeof(slot));
    assert(SzDuel_ClearCardControl(&slot) == 1);
    bytes = (unsigned char *)&slot;
    assert(slot.unknown_00 == 0 && slot.unknown_98 == 0 && slot.unknown_9a == 0);
    for (j = 4; j < 0x98; j++) {
        assert(bytes[j] == 0xaa);
    }
    assert(*(unsigned *)(bytes + 0x9c) == 0xff);

    /* 0x1cdd40: all six elements reset, the following bytes untouched. */
    memset(gSzDuel_aCardControl, 0xaa, sizeof(gSzDuel_aCardControl));
    SzDuel_InitCardControl();
    for (i = 0; i < 6; i++) {
        bytes = gSzDuel_aCardControl[i].unknown_bytes;
        assert(*(int *)bytes == 0 && *(short *)(bytes + 0x98) == 0 && *(short *)(bytes + 0x9a) == 0);
        assert(*(unsigned *)(bytes + 0x9c) == 0xff && bytes[4] == 0xaa && bytes[0x97] == 0xaa);
    }

    /* 0x243230 and 0x243290: same body; 0x243a40 first, then the seven-argument call, then the store. */
    for (k = 0; k < 2; k++) {
        memset(&stream, 0, sizeof(stream));
        stream.unknown_00 = 9;
        stream.unknown_44 = 100;
        stream.unknown_48 = -1;
        stream.unknown_50 = 7;
        args7_stream = &stream;
        port_calls = 0;
        args7_calls = 0;
        if (k == 0) {
            audioDecResume(&stream);
        } else {
            audioDecStart(&stream);
        }
        assert(args7_calls == 1 && args7_ports == 2 && args7_owner_word == 9 && stream.unknown_00 == 2);
        assert(port_args[0][3] == 0x7fff && port_args[1][3] == 0x7fff);
        assert(args7_seen[0] == 1 && args7_seen[1] == 0x80e0 && args7_seen[2] == 0 && args7_seen[3] == 19);
        assert(args7_seen[4] == 100 && args7_seen[5] == -1024 && args7_seen[6] == 107);
        stream.unknown_48 = 0x12345;
        stream.unknown_00 = 0;
        port_calls = 0;
        if (k == 0) {
            audioDecResume(&stream);
        } else {
            audioDecStart(&stream);
        }
        assert(args7_seen[5] == 0x12000);
    }
}

/* 0x244ad0, 0x244b20, 0x26b430, 0x243a40, 0x245160 and 0x27f110. */
static void check_brackets_and_callers(void)
{
    struct DotrMiscSpan span;
    struct DotrMiscOrigin origin;
    int out[5];

    assert(offsetof(struct DotrMiscSpan, unknown_40) == 0x40 && offsetof(struct DotrMiscOrigin, unknown_48) == 0x48);
    assert(offsetof(struct DotrMiscFrame, unknown_30) == 0x30 && sizeof(struct DotrMiscMatrix) == 64);
    memset(&span, 0, sizeof(span));
    span.unknown_10 = 3;
    span.unknown_14 = 100;
    span.unknown_40 = 77;
    bracket_count = 0;
    assert(viBufCount(&span) == 100 + 3 * 2048 && bracket_count == 2);
    assert(bracket_log[0] == 77 && bracket_log[1] == -77 && span.unknown_14 == 100);
    viBufFlush(&span);
    assert(bracket_count == 4 && bracket_log[2] == 77 && bracket_log[3] == -77 && span.unknown_14 == 2048);
    bracket_count = 0;
    viBufFlush(&span);
    assert(span.unknown_14 == 2048);
    span.unknown_14 = 2049;
    bracket_count = 0;
    viBufFlush(&span);
    assert(span.unknown_14 == 4096 && span.unknown_10 == 3);

    out[4] = 0x5a5a;
    assert(Ta_DoUncTrapB(out) == 1 && source_args[0] == 0 && source_args[1] == 46);
    assert(out[0] == 102 && out[1] == 1 && out[2] == 0x1357 && out[3] == 0 && out[4] == 0x5a5a);

    port_calls = 0;
    changeInputVolume(-9);
    assert(port_calls == 2 && port_args[0][0] == 1 && port_args[0][1] == 0x8010 && port_args[0][2] == 3968);
    assert(port_args[1][0] == 1 && port_args[1][1] == 0x8010 && port_args[1][2] == 4224);
    assert(port_args[0][3] == -9 && port_args[1][3] == -9);

    memset(&origin, 0, sizeof(origin));
    origin.unknown_48 = 1000;
    videoDecPutTs(&origin, 0x123456789L, -2L, 1500, 44);
    assert(request_queue == data_003506c8 && request_seen.unknown_00 == 0x123456789L && request_seen.unknown_08 == -2L);
    assert(request_seen.unknown_10 == 500 && request_seen.unknown_14 == 44);

    frame_step = 0;
    Ta_LkSetCoordinate(&frame_fixture);
    assert(frame_step == 3);
    frame_step = 0;
    LkSetCoordinate(&frame_fixture);
    assert(frame_step == 3);
    frame_step = 0;
    Taku_Coord_Set(&frame_fixture);
    assert(frame_step == 3);
}

/* 0x27b200 and 0x27b290. */
static void check_nibble_callers(void)
{
    nibble_calls = 0;
    nibble_zero_bytes = 0;
    assert(TaLogic_IsRootOpen2(5, 10, 11, 0x12, 0x34, 13, 14, 0xfe) == 0);
    assert(nibble_calls == 1 && nibble_fill_ok && nibble_args[7] == 1);
    assert(nibble_args[0] == 5 && nibble_args[1] == 10 && nibble_args[2] == 11);
    assert(nibble_args[3] == 13 && nibble_args[4] == 14 && nibble_args[5] == 0x1234 && nibble_args[6] == 0xfe);

    nibble_zero_bytes = 1;
    assert(TaLogic_IsRootOpen2(5, 10, 11, 0x12, -1, 13, 14, 0xfe) == 0x7004);
    assert(nibble_args[5] == 0xffff && nibble_args[6] == 0xfe);
    nibble_zero_bytes = 12;
    assert(TaLogic_IsRootOpen2(6, 10, 11, 0x12, -2, 13, 14, 1) == 0x7004);
    assert(nibble_args[0] == 6 && nibble_args[5] == (unsigned short)((0x12 << 8) | -2));
    assert(nibble_args[6] == 1);
    assert(TaLogic_IsRootOpen2(6, 10, 11, 0x1ffff, 0x34, 13, 14, 1) == 0x7004);
    assert(nibble_args[5] == 0xff34);

    nibble_zero_bytes = 1;
    assert(TaLogic_IsRootOpen(4, 7, 10, 0x12, 0x34, 13, 14, 15, 0xfe) == 0x7004);
    assert(word_a == 4 && word_b == 7 && nibble_args[0] == 0x5540 && nibble_args[1] == 10);
    assert(nibble_args[2] == 0x12 && nibble_args[3] == 14 && nibble_args[4] == 15);
    assert(nibble_args[5] == ((0x34 << 8) | 13) && nibble_args[6] == 0xfe);
}

/* 0x1c3520 branch coverage; 0x25ecb0 and 0x25f2a0 branch coverage. 0x23bc80 is not
 * executed: it copies to the fixed address 0x1e301e0, which is unmapped on the host. */
static void check_flag_and_cell_scans(void)
{
    int i;

    data_00317490_tail.unknown_6d8 = 0x6d8;
    data_00317490_tail.unknown_6dc = 0x6dc;
    lSzDuel_SortTrade(0);
    assert(t_id == 9 && t_args[0] == 0x6d8 && t_args[1] == 854 && t_args[2] == 2);
    assert(t_callback == lSzDuel_SortFunc1 && data_00317490_tail.unknown_d5c == 0);
    lSzDuel_SortTrade(1);
    assert(t_args[0] == 0x6dc && t_args[1] == 854 && t_args[2] == 2);
    assert(t_callback == lSzDuel_SortFunc2 && data_00317490_tail.unknown_d5c == 1);
    lSzDuel_SortTrade(0x100);
    assert(t_args[0] == 0x6dc && t_callback == lSzDuel_SortFunc2 && data_00317490_tail.unknown_d5c == 0);

    grid_negative_at = -1;
    grid_count = 0;
    assert(Ta_SearchMoveTemp() == -1 && grid_count == 49);
    for (i = 0; i < 49; i++) assert(grid_calls[i][0] == i / 7 && grid_calls[i][1] == i % 7);
    grid_negative_at = 2 * 7 + 5;
    grid_negative_value = -3;
    grid_count = 0;
    assert(Ta_SearchMoveTemp() == 0x502 && grid_count == 2 * 7 + 6);
    grid_negative_at = 0;
    grid_count = 0;
    assert(Ta_SearchMoveTemp() == 0 && grid_count == 1);

    grid_negative_at = -1;
    grid_count = 0;
    assert(Ta_GetStackMapPiece() == -1 && grid_count == 49);
    for (i = 0; i < 49; i++) assert(grid_calls[i][0] == i / 7 && grid_calls[i][1] == i % 7);
    grid_negative_at = 3 * 7 + 4;
    grid_negative_value = -7;
    grid_count = 0;
    assert(Ta_GetStackMapPiece() == -7 && grid_count == 3 * 7 + 5);
    grid_negative_at = -1;
}

/* 0x1b7660, 0x243750, 0x2495a0, 0x272050, 0x27c6d0 branch
 * coverage. 0x22c3c0 is not host-executed (fixed address 0x1ed547c): compile
 * and link only. */
static void check_seventh_batch(void)
{
    int a, b, c, d, i, kind;
    struct DotrMiscRing ring;
    const char *in = "AZaz09?@[/:!";
    const int want[] = {0, 25, 0, 25, 26, 35, 36, -1, -1, -1, -1, -1};

    for (i = 0; i < 12; i++) assert(lSzDuel_GetFontIndex(in[i]) == want[i]);
    assert(lSzDuel_GetFontIndex((char)0xff) == -1);

    memset(&ring, 0, sizeof(ring));
    ring.unknown_44 = 0x1000;
    ring.unknown_48 = 4096;
    ring.unknown_4c = 100;
    /* t = (position + length - offset - 1024) % length, rounded down to 1024 */
    iopGetArea(&a, &b, &c, &d, &ring, 3172);
    assert(a == 0x1000 + 100 && b == 2048 && c == 0 && d == 0);
    iopGetArea(&a, &b, &c, &d, &ring, 1124);
    assert(a == 0x1000 + 100 && b == 0 && c == 0 && d == 0);
    ring.unknown_48 = 8192;
    ring.unknown_4c = 7000;
    iopGetArea(&a, &b, &c, &d, &ring, 11024);
    assert(a == 0x1000 + 7000 && b == 1192 && c == 0x1000 && d == 2048 - 1192);

    memset(FusionDat, 0, sizeof(FusionDat));
    FusionDat[7] = 5u | (3u << 10) | (321u << 20);
    FusionDat[26539] = 9u | (1u << 10) | (77u << 20);
    assert(real_func_002495a0(5, 3) == 321 && real_func_002495a0(3, 5) == 321);
    assert(real_func_002495a0(9, 1) == 77 && real_func_002495a0(1, 9) == 77);
    assert(real_func_002495a0(5, 4) == 999 && real_func_002495a0(4, 4) == 999);

    kind_query_value = 8;
    assert(Ta_IsLD_CurseObj(3, 2) == 1 && Ta_IsLD_CurseObj(3, 1) == 0);
    assert(kind_query_args[0] == 3 && kind_query_args[1] == 0);
    kind_query_value = 0x28;
    assert(Ta_IsLD_CurseObj(3, 2) == 1);
    kind_query_value = 11;
    assert(Ta_IsLD_CurseObj(3, 16) == 1 && Ta_IsLD_CurseObj(3, 2) == 0);
    kind_query_value = 17;
    assert(Ta_IsLD_CurseObj(3, 5) == 1 && Ta_IsLD_CurseObj(3, 6) == 1);
    assert(Ta_IsLD_CurseObj(3, 4) == 0 && Ta_IsLD_CurseObj(3, 7) == 0);
    kind_query_value = 19;
    assert(Ta_IsLD_CurseObj(3, 0) == 1 && Ta_IsLD_CurseObj(3, 1) == 0);
    kind_query_value = 0;
    assert(Ta_IsLD_CurseObj(3, 2) == 0);
    kind_query_value = 0x23;
    assert(Ta_IsLD_CurseObj(3, 0) == 0);

    bit_calls = 0;
    bit_result = 1;
    assert(TaLogic_IsPickUp((5 << 8) | 7) == 1 && bit_calls == 1);
    assert(bit_args[0] == 5 && bit_args[1] == 7 && bit_args[2] == 0);
    bit_result = 0;
    for (kind = 0; kind < 8; kind++) {
        shifted_kind = kind << 5;
        assert(TaLogic_IsPickUp((5 << 8) | 7) == 1);
    }
    /* kinds 3 and 4: result follows the bit-4 query */
    shifted_kind = 3 << 5;
    bit_calls = 0;
    assert(TaLogic_IsPickUp(0x0507) == 1 && bit_calls == 2 && bit_args[2] == 4);
    bit_only4 = 1;
    assert(TaLogic_IsPickUp(0x0507) == 0);
    shifted_kind = 4 << 5;
    assert(TaLogic_IsPickUp(0x0507) == 0);
    shifted_kind = 5 << 5;
    assert(TaLogic_IsPickUp(0x0507) == 1);
    shifted_kind = 3 << 5;
    assert(TaLogic_IsPickUp(-0x100 | 0x7f) == 0 && bit_args[0] == -1 && bit_args[1] == 0x7f);
    bit_only4 = 0;
}

/* 0x214090 branch coverage. Fixture is static (non-PIE, below 4 GiB) so the
 * (int) address arithmetic of the 32-bit original stays meaningful. */
static DotrMiscAddress rebase_buf[64] __attribute__((aligned(16)));
static void check_rebase_head(void)
{
    DotrMiscRebaseHead *head = (DotrMiscRebaseHead *)rebase_buf;
    DotrMiscFixupHead *fix = (DotrMiscFixupHead *)((char *)rebase_buf + 64);
    DotrMiscAddress *w = (DotrMiscAddress *)((char *)rebase_buf + 128);
    DotrMiscAddress base = (DotrMiscAddress)rebase_buf;

    assert(base > 0 && base < 0x40000000);

    /* Wrong magic: nothing is modified. */
    memset(rebase_buf, 0, sizeof(rebase_buf));
    head->magic = 0x1a544f4c;
    head->first = 64;
    head->second = 8;
    assert(NjaReloc_Adjust_M3DMOT(head) == 0 && head->first == 64 && head->second == 8);

    /* Right magic, flag bit set: both offsets rebased, callee runs with flag 1 and count 1. */
    memset(rebase_buf, 0, sizeof(rebase_buf));
    head->magic = 0x1a544f4d;
    head->count = 1;
    head->flags = 1;
    head->first = 64;
    head->second = 8;
    ((DotrMiscAddress *)fix)[-1] = 100;
    fix->kind = 3;
    fix->data = 128;
    /* The element's two nonzero words are offsets of targets inside the buffer: with the flag
     * the word before each rebased target is rebased too, so it must be fixture memory. */
    w[0] = 256; w[1] = 320; w[2] = 3;
    ((DotrMiscAddress *)((char *)rebase_buf + 256))[-1] = 7;
    ((DotrMiscAddress *)((char *)rebase_buf + 320))[-1] = 9;
    assert(NjaReloc_Adjust_M3DMOT(head) == 1);
    assert(head->first == base + 64 && head->second == base + 8);
    assert(((DotrMiscAddress *)fix)[-1] == 100 + base);
    assert(fix->data == (DotrMiscAddress)(long)w);
    assert(w[0] == 256 + base && w[1] == 320 + base && w[2] == 3);
    assert(((DotrMiscAddress *)((char *)rebase_buf + 256))[-1] == 7 + base);
    assert(((DotrMiscAddress *)((char *)rebase_buf + 320))[-1] == 9 + base);

    /* Flag bit clear (0x7f in the byte): the word before the target is untouched. */
    memset(rebase_buf, 0, sizeof(rebase_buf));
    head->magic = 0x1a544f4d;
    head->count = 1;
    head->flags = 0;
    head->unknown_13 = 0x7f;
    head->first = 64;
    ((DotrMiscAddress *)fix)[-1] = 100;
    assert(NjaReloc_Adjust_M3DMOT(head) == 1 && ((DotrMiscAddress *)fix)[-1] == 100);
}

/* 0x25ed20 branch coverage; all callees are stubs. */
static int job_log[16], job_log_count;
static int job_b50_result, job_a30_result;
static DotrMiscJob job_req_seen;
void InitEffectTemp(DotrMiscJobQueue *q) { job_log[job_log_count++] = 0x960; job_log[job_log_count++] = q != 0; }
void TaEffect_LoadModule(DotrMiscJobQueue *q, int id) { (void)q; job_log[job_log_count++] = 0xac0; job_log[job_log_count++] = id; }
int TaEffect_IsLoadModule(DotrMiscJobQueue *q) { (void)q; job_log[job_log_count++] = 0xb50; return job_b50_result; }
int Ta_Do_Effect(DotrMiscJob *req, DotrMiscJobQueue *q) { (void)q; job_log[job_log_count++] = 0xa30; job_req_seen = *req; return job_a30_result; }
int SzDuel_CalcSummonPower(int index, int operation, int value)
{
    job_log[job_log_count++] = 0xde5;
    job_log[job_log_count++] = index;
    job_log[job_log_count++] = operation;
    job_log[job_log_count++] = value;
    return 0;
}
void SzDuel_CloseMsgPopup(void) { job_log[job_log_count++] = 0x453; }

static void check_job_queue(void)
{
    DotrMiscJob in = {0, 11, 22, 33};
    DotrMiscJobQueue q;

    /* mode >= 0 (0 and positive): reset calls, return 1. */
    memset(&q, 0, sizeof(q));
    job_log_count = 0;
    assert(Ta_Eff_034(&in, &q, 0) == 1 && job_log_count == 4);
    assert(job_log[0] == 0x960 && job_log[1] == 1 && job_log[2] == 0xac0 && job_log[3] == 534);
    job_log_count = 0;
    assert(Ta_Eff_034(&in, &q, 5) == 1 && job_log_count == 4);

    /* mode < 0, status != 1: nothing else happens. */
    job_log_count = 0;
    job_b50_result = 0;
    assert(Ta_Eff_034(&in, &q, -1) == 1 && job_log_count == 1 && job_log[0] == 0xb50 && q.count == 0);

    /* status 1, count 0: request built from in->value, slot stored, count incremented. */
    memset(&q, 0, sizeof(q));
    memset(&job_req_seen, 0, sizeof(job_req_seen));
    job_log_count = 0;
    job_b50_result = 1;
    job_a30_result = 0x5a;
    assert(Ta_Eff_034(&in, &q, -1) == 1 && job_log_count == 2 && job_log[1] == 0xa30);
    assert(job_req_seen.unknown_00 == 534 && job_req_seen.first == 0 && job_req_seen.second == 0 && job_req_seen.value == 33);
    assert(q.slot == 0x5a && q.count == 1);

    /* status 1, count != 0, slot check == 1: returns 1 without further calls. */
    list_count = 0;
    list_results[0] = 1;
    job_log_count = 0;
    assert(Ta_Eff_034(&in, &q, -1) == 1 && job_log_count == 1 && list_calls[0] == 0x5a && q.count == 1);

    /* slot check != 1: two SzDuel_CalcSummonPower calls, SzDuel_CloseMsgPopup, returns 0. */
    list_count = 0;
    list_results[0] = 0;
    job_log_count = 0;
    assert(Ta_Eff_034(&in, &q, -1) == 0 && job_log_count == 10);
    assert(job_log[1] == 0xde5 && job_log[2] == 11 && job_log[3] == 4 && job_log[4] == 0);
    assert(job_log[5] == 0xde5 && job_log[6] == 22 && job_log[7] == 0 && job_log[8] == 33 && job_log[9] == 0x453);
}

/* 0x206030 branch coverage: empty, single and multi-item averages. */
static void check_vector_mean(void)
{
    struct DotrMiscVector4 items[3] = {{1, 2, 3, 4}, {3, 6, 9, 12}, {5, 10, 15, 20}};
    struct DotrMiscVector4 out = {9, 9, 9, 9};
    assert(Ex3d_GetPointsAverage(0, items, &out) == 0);
    assert(out.x == 0 && out.y == 0 && out.z == 0 && out.unknown_w == 0);
    out.x = out.y = out.z = out.unknown_w = 9;
    assert(Ex3d_GetPointsAverage(1, items, &out) == 1);
    assert(out.x == 1 && out.y == 2 && out.z == 3 && out.unknown_w == 4);
    assert(Ex3d_GetPointsAverage(3, items, &out) == 1);
    assert(out.x == 3 && out.y == 6 && out.z == 9 && out.unknown_w == 12);
}

/* Test stub for the pool entry lookup used by TaEffect_IsLoadModule2 (state in the low 3 bits). */
static unsigned char job_states[0x10000 >> 8];
static int job_calls, job_args[2];
static int job_null;
struct DotrJobPool *SzEffect_GetModuleInfo(int a, int b)
{
    job_calls++;
    job_args[0] = a;
    job_args[1] = b;
    return job_null ? 0 : (struct DotrJobPool *)&job_states[0];
}

static void check_tenth_batch(void)
{
    struct DotrJobIds job;
    struct DotrSlot16 slots[22], saved[22];
    int i;

    /* 0x255c30 */
    memset(&job, 0, sizeof(job));
    for (i = 0; i < 24; i++) {
        job.ids[i] = (unsigned short)(100 + i);
    }
    job_calls = 0;
    assert(TaEffect_IsLoadModule2(0, 100) == 0 && job_calls == 0);
    assert(TaEffect_IsLoadModule2(&job, 5) == 0 && job_calls == 0);
    job_null = 1;
    assert(TaEffect_IsLoadModule2(&job, 103) == 1 && job_calls == 1 && job_args[0] == 0 && job_args[1] == 103);
    job_null = 0;
    job_states[0] = 0;
    assert(TaEffect_IsLoadModule2(&job, 123) == 0 && job_args[1] == 123);
    job_states[0] = 1;
    assert(TaEffect_IsLoadModule2(&job, 100) == -1);
    job_states[0] = 2;
    assert(TaEffect_IsLoadModule2(&job, 100) == 1);
    job_states[0] = 0xfb;
    assert(TaEffect_IsLoadModule2(&job, 100) == 1);
    /* First matching index only: a duplicate id is looked up once. */
    job.ids[4] = 100;
    job_calls = 0;
    TaEffect_IsLoadModule2(&job, 100);
    assert(job_calls == 1);

    /* 0x2761d0 */
    memset(slots, 0, sizeof(slots));
    for (i = 0; i < 22; i++) {
        slots[i].id = (unsigned short)(i + 1);
        slots[i].tag = (unsigned short)(i + 50);
        slots[i].aux = (unsigned short)(i + 70);
        slots[i].word_08 = i + 9;
        slots[i].flag_lo = 1;
        slots[i].flag_rest = 5;
    }
    memcpy(saved, slots, sizeof(slots));
    TaLogic_DoSortTaskList(slots);
    assert(memcmp(saved, slots, sizeof(slots)) == 0);
    slots[2].id = 0xffff;
    slots[3].id = 0xffff;
    memcpy(saved, slots, sizeof(slots));
    TaLogic_DoSortTaskList(slots);
    /* Slot 4 moves into hole 2, slot 5 into hole 3, and so on down the array. */
    assert(slots[2].id == 5 && slots[2].tag == 54 && slots[2].aux == 74 && slots[2].word_08 == 13);
    assert(slots[3].id == 6 && slots[3].word_08 == 14);
    assert(slots[4].id == 7 && slots[5].id == 8);
    assert(slots[21].id == 0xffff && slots[21].tag == 255 && slots[21].aux == 0xffff && slots[21].word_08 == -1);
    assert(slots[21].flag_lo == 0 && slots[21].flag_rest == 5);
    assert(slots[20].id == 0xffff && slots[19].id == 22);
    /* All holes: unchanged. */
    for (i = 0; i < 22; i++) {
        slots[i].id = 0xffff;
    }
    memcpy(saved, slots, sizeof(slots));
    TaLogic_DoSortTaskList(slots);
    assert(memcmp(saved, slots, sizeof(slots)) == 0);
}

static void check_eleventh_batch(void)
{
    float plane[4] = {0.0f, 0.0f, 1.0f, -2.0f}, a[3] = {1.0f, 2.0f, 0.0f}, b[3] = {3.0f, 4.0f, 4.0f}, out[4];

    /* 0x206430: crossing, parallel clamp, negative denominator */
    Ex3d_IntersectP(plane, a, b, out);
    assert(out[0] == 2.0f && out[1] == 3.0f && out[2] == 2.0f && out[3] == 1.0f);
    b[2] = 0.0f;
    Ex3d_IntersectP(plane, a, b, out);
    assert(out[3] == 1.0f && out[0] > 1.0e6f);
    b[2] = -4.0f;
    Ex3d_IntersectP(plane, a, b, out);
    assert(out[0] == 0.0f && out[1] == 1.0f && out[2] == 2.0f && out[3] == 1.0f);

    /* 0x26d840: callee is asked about (y, x +- 1) */
    grid_negative_at = 3 * 7 + 6;
    grid_negative_value = -1;
    grid_count = 0;
    assert(Ta_GetUnitGravityLocSearchOnZ(99, 9, 3, 5) == ((6 << 8) | 3) && grid_count == 1 && grid_calls[0][0] == 3 && grid_calls[0][1] == 6);
    grid_negative_at = -1;
    grid_count = 0;
    assert(Ta_GetUnitGravityLocSearchOnZ(99, 9, 3, 5) == -1 && grid_count == 1);
    grid_negative_at = 3 * 7 + 4;
    grid_count = 0;
    assert(Ta_GetUnitGravityLocSearchOnZ(99, 1, 3, 5) == ((4 << 8) | 3) && grid_calls[0][1] == 4);
    grid_negative_at = -1;
    grid_count = 0;
    assert(Ta_GetUnitGravityLocSearchOnZ(99, 1, 3, 5) == -1 && grid_count == 1);
    grid_count = 0;
    assert(Ta_GetUnitGravityLocSearchOnZ(99, 5, 3, 5) == -1 && grid_count == 0);
    grid_negative_at = -1;
}

static int flag_log[32][2], flag_log_count;

int Ta_DelUnit(int a, int b)
{
    flag_log[flag_log_count][0] = a;
    flag_log[flag_log_count][1] = b;
    flag_log_count++;
    return 0;
}

static void check_twelfth_batch(void)
{
    static const struct { unsigned char kind; int lo, hi; } ranges[] = {
        {0, 0, 11}, {1, 0, 6}, {2, 6, 11}, {3, 1, 11}, {4, 1, 6}
    };
    DotrMiscByteList list;
    struct DotrMiscFlagWord flags;
    struct DotrMiscVector3 dst, src;
    int i, k;

    /* 0x24e780: every kind's range, a count of the nonzero results, and the rejected kinds. */
    for (k = 0; k < 5; k++) {
        word_calls = 0;
        word_mask = ~0u;
        assert(Ta_GetUnitNum(77, ranges[k].kind) == ranges[k].hi - ranges[k].lo);
        assert(word_calls == ranges[k].hi - ranges[k].lo && word_a == 77);
        for (i = 0; i < word_calls; i++) assert(word_seen[i] == ranges[k].lo + i);
    }
    word_mask = 0x555; /* even indices only */
    assert(Ta_GetUnitNum(1, 0) == 6 && Ta_GetUnitNum(1, 3) == 5 && Ta_GetUnitNum(1, 2) == 3);
    word_mask = 0;
    assert(Ta_GetUnitNum(1, 4) == 0);
    word_mask = ~0u;
    word_calls = 0;
    assert(Ta_GetUnitNum(1, 5) == -1 && Ta_GetUnitNum(1, 255) == -1 && word_calls == 0);

    /* 0x25f670: list failure, no bits, and both halves of the 22-bit flag word. */
    memset(&list, 0, sizeof(list));
    list_owner = &list;
    list_shrink = -1;
    memset(list_results, 0, sizeof(list_results));
    memset(&flags, 0, sizeof(flags));
    flags.unknown_08 = 0x3fffff;
    list.count = 1;
    list_count = 0;
    list_results[0] = 1;
    flag_log_count = 0;
    assert(Ta_IsFinishDestEfc(&flags, &list) == 0 && flag_log_count == 0);
    list.count = 0;
    flags.unknown_08 = 0xffc00000; /* only bits above 21 */
    list_count = 0;
    assert(Ta_IsFinishDestEfc(&flags, &list) == 1 && flag_log_count == 0);
    flags.unknown_08 = (1u << 21) | (1u << 11) | (1u << 10) | 1u;
    assert(Ta_IsFinishDestEfc(&flags, &list) == 1 && flag_log_count == 4);
    assert(flag_log[0][0] == 0 && flag_log[0][1] == 0);
    assert(flag_log[1][0] == 0 && flag_log[1][1] == 10);
    assert(flag_log[2][0] == 1 && flag_log[2][1] == 0);
    assert(flag_log[3][0] == 1 && flag_log[3][1] == 10);

    /* 0x2720f0: each matching (kind, b) pair, neighbouring values, the 0x1f mask and other kinds. */
    kind_query_value = 8;
    assert(Ta_IsLD_DestObj(5, 7) == 1 && Ta_IsLD_DestObj(5, 6) == 0);
    assert(kind_query_args[0] == 5 && kind_query_args[1] == 0);
    kind_query_value = 11;
    assert(Ta_IsLD_DestObj(5, 9) == 1 && Ta_IsLD_DestObj(5, 7) == 0);
    kind_query_value = 17;
    assert(Ta_IsLD_DestObj(5, 2) == 1 && Ta_IsLD_DestObj(5, 19) == 1 && Ta_IsLD_DestObj(5, 10) == 0);
    kind_query_value = 18;
    assert(Ta_IsLD_DestObj(5, 10) == 1 && Ta_IsLD_DestObj(5, 18) == 0);
    kind_query_value = 19;
    assert(Ta_IsLD_DestObj(5, 18) == 1 && Ta_IsLD_DestObj(5, 2) == 0);
    kind_query_value = 0x28; /* masks to 8 */
    assert(Ta_IsLD_DestObj(5, 7) == 1);
    kind_query_value = 0x31; /* masks to 17 */
    assert(Ta_IsLD_DestObj(5, 19) == 1);
    kind_query_value = 0;
    assert(Ta_IsLD_DestObj(5, 7) == 0);
    kind_query_value = 31;
    assert(Ta_IsLD_DestObj(5, 7) == 0);
    kind_query_value = 0;

    /* 0x27f290: n == 0 copies; otherwise dst += (src - dst) / n through the real helpers. */
    dst.x = 1.0f; dst.y = -2.0f; dst.z = 4.0f;
    src.x = 5.0f; src.y = 6.0f; src.z = -4.0f;
    Ta_LkOBJ_TransitVector(&dst, &src, 0);
    assert(dst.x == 5.0f && dst.y == 6.0f && dst.z == -4.0f);
    dst.x = 1.0f; dst.y = -2.0f; dst.z = 4.0f;
    Ta_LkOBJ_TransitVector(&dst, &src, 4);
    assert(dst.x == 2.0f && dst.y == 0.0f && dst.z == 2.0f);
    assert(src.x == 5.0f && src.y == 6.0f && src.z == -4.0f);
    dst.x = 1.0f; dst.y = -2.0f; dst.z = 4.0f;
    Ta_LkOBJ_TransitVector(&dst, &src, -2);
    assert(dst.x == -1.0f && dst.y == -6.0f && dst.z == 8.0f);
}

static void check_euler_matrix(void)
{
    /* Straight-line: one call per angle set, every element checked against the real sine/cosine helpers. */
    static const int sets[2][3] = {{0, 0, 0}, {4096, 9000, -12000}};
    int k, i;
    for (k = 0; k < 2; k++) {
        int ang[3];
        float m[16], cx, sx, cy, sy, cz, sz, e[16];
        memcpy(ang, sets[k], sizeof(ang));
        for (i = 0; i < 16; i++) m[i] = 99.0f;
        RhGeom_GetRotXYZMatrix(m, ang);
        cx = RhMath_CosI(ang[0]); sx = RhMath_SinI(ang[0]);
        cy = RhMath_CosI(ang[1]); sy = RhMath_SinI(ang[1]);
        cz = RhMath_CosI(ang[2]); sz = RhMath_SinI(ang[2]);
        e[0] = cy * cz; e[1] = cy * sz; e[2] = -sy;
        e[4] = cz * (sx * sy) - cx * sz; e[5] = sz * (sx * sy) + cx * cz; e[6] = sx * cy;
        e[8] = cz * (cx * sy) + sx * sz; e[9] = sz * (cx * sy) - sx * cz; e[10] = cx * cy;
        e[3] = e[7] = e[11] = e[12] = e[13] = e[14] = 0.0f; e[15] = 1.0f;
        for (i = 0; i < 16; i++) assert(fabsf(m[i] - e[i]) < 1e-5f);
    }
}

static void check_candidate_list(void)
{
    unsigned short out[20];
    int i;
    memset(pair_sides, 0, sizeof(pair_sides));
    word_mask = (1u << 2) | (1u << 5) | (1u << 9);
    /* mode 3: every nonzero word emits, kind first, then its partner */
    memset(out, 0xff, sizeof(out));
    TaUtil_GetUnitLst(1, 3, out);
    assert(out[0] == 0x102 && out[1] == 0x105 && out[2] == 0x109);
    assert(out[3] == 0x002 && out[4] == 0x005 && out[5] == 0x009 && out[6] == 0xffff);
    memset(out, 0xff, sizeof(out));
    TaUtil_GetUnitLst(0, 3, out);
    assert(out[0] == 0x002 && out[3] == 0x102 && out[5] == 0x109 && out[6] == 0xffff);
    /* mode 0 never emits */
    memset(out, 0xff, sizeof(out));
    TaUtil_GetUnitLst(1, 0, out);
    assert(out[0] == 0xffff);
    /* mode 1 compares kind with the pair value: only slot 5 of side 1 and slot 9 of side 0 */
    for (i = 0; i < 11; i++) {
        pair_sides[1][i] = 0;
        pair_sides[0][i] = 0;
    }
    pair_sides[1][5] = 1;
    pair_sides[0][9] = 1;
    memset(out, 0xff, sizeof(out));
    TaUtil_GetUnitLst(1, 1, out);
    assert(out[0] == 0x105 && out[1] == 0x009 && out[2] == 0xffff);
    word_mask = ~0u;
}

static void fill_list(unsigned short *list, int n, unsigned short e1, unsigned short e2, unsigned short e3)
{
    int i;
    for (i = 0; i < 22; i++) list[i] = 0xffff;
    /* TaLogic_GetUnitCtlLstLen counts the halfwords below 0xffff, so n entries including the count word. */
    list[0] = 0;
    if (n > 1) list[1] = e1;
    if (n > 2) list[2] = e2;
    if (n > 3) list[3] = e3;
}

static void check_best_entry(void)
{
    /* Branch coverage: count bound, acceptance, strictly-greater and tie outcomes for both scanners. */
    static const int scores[8] = {0, 10, 30, 30, 20, 0, 0, 0};
    static const unsigned char accept[8] = {0, 1, 1, 1, 1, 0, 0, 0};
    static const unsigned char none[8];
    unsigned short list[22];
    score_by_hi = scores;
    bit_by_lo = accept;

    score_calls = 0;
    fill_list(list, 1, 0, 0, 0);
    assert(TaLogic_GetMaxApUnit(list) == -1 && score_calls == 0);
    fill_list(list, 2, 0x0101, 0, 0);
    assert(TaLogic_GetMaxApUnit(list) == 0x0101 && score_calls == 1);
    assert(score_args[0] == 1 && score_args[1] == 1 && score_args[2] == 0x45 && score_args[3] == 0x123);
    /* Greater score wins, not the later entry. */
    fill_list(list, 4, 0x0101, 0x0202, 0x0104);
    assert(TaLogic_GetMaxApUnit(list) == 0x0202);
    /* Tie keeps the first. */
    fill_list(list, 4, 0x0101, 0x0202, 0x0303);
    assert(TaLogic_GetMaxApUnit(list) == 0x0202);
    /* Rejected entries are skipped (lo 5 is not accepted). */
    fill_list(list, 3, 0x0205, 0x0105, 0);
    assert(TaLogic_GetMaxApUnit(list) == -1);
    fill_list(list, 3, 0x0205, 0x0102, 0);
    assert(TaLogic_GetMaxApUnit(list) == 0x0102);

    bit_by_lo = none;
    thunk_by_lo[2] = 7;
    fill_list(list, 1, 0, 0, 0);
    assert(TaLogic_GetMaxApUnit2(list, 7) == -1);
    fill_list(list, 3, 0x0201, 0x0102, 0);
    assert(TaLogic_GetMaxApUnit2(list, 7) == 0x0102 && thunk_args[0] == 1 && thunk_args[1] == 2);
    assert(TaLogic_GetMaxApUnit2(list, 8) == -1);
    bit_by_lo = accept;
    fill_list(list, 4, 0x0101, 0x0202, 0x0303);
    assert(TaLogic_GetMaxApUnit2(list, 99) == 0x0202);
    fill_list(list, 4, 0x0101, 0x0204, 0x0105);
    assert(TaLogic_GetMaxApUnit2(list, 99) == 0x0204);
    bit_by_lo = NULL;
}

int Ta_SearchUncTypeNaturalEffectUserNumWithSide(int kind, int mode) { stub_q_kind = kind; stub_q_mode = mode; return stub_q_result; }
int SzDuel_GetUnitLeaderAbility(int a, int b, int c, int d)
{
    stub_g_calls++;
    stub_g_args[0] = a; stub_g_args[1] = b; stub_g_args[2] = c; stub_g_args[3] = d;
    return stub_g_result;
}

static void check_neighbour_step(void)
{
    /* Branch coverage: a == y, a < y and a > y, each with negative/non-negative probe and d == 0 / d != 0.
     * SzDuel_GetUnitLoc stub yields row 0x123, y 0x45; the neighbour probe is cell (x, 0x123). */
    const int y = 0x45, row = 0x123;
    grid_negative_value = -1;

    grid_negative_at = -1;
    grid_count = 0;
    assert(Ta_GetUnitGravityLoc(y, 9, 0x0102, 0) == Ta_GetUnitGravityLocSearchOnZ(y, 9, y, row));
    assert(grid_args[0] == 1 && grid_args[1] == 2);

    grid_negative_at = (y - 1) * 7 + row;
    grid_count = 0;
    assert(Ta_GetUnitGravityLoc(y - 1, 9, 0x0102, 0) == ((row << 8) | (y - 1)) && grid_count == 1);
    assert(grid_calls[0][0] == y - 1 && grid_calls[0][1] == row);
    grid_count = 0;
    assert(Ta_GetUnitGravityLoc(y - 1, 9, 0x0102, 1) == Ta_GetUnitGravityLocSearchOnZ(y - 1, 9, y, row));
    grid_negative_at = -1;
    grid_count = 0;
    assert(Ta_GetUnitGravityLoc(y - 1, 9, 0x0102, 0) == Ta_GetUnitGravityLocSearchOnZ(y - 1, 9, y, row));

    grid_negative_at = (y + 1) * 7 + row;
    grid_count = 0;
    assert(Ta_GetUnitGravityLoc(y + 1, 9, 0x0102, 0) == ((row << 8) | (y + 1)) && grid_calls[0][0] == y + 1);
    grid_count = 0;
    assert(Ta_GetUnitGravityLoc(y + 1, 9, 0x0102, 1) == Ta_GetUnitGravityLocSearchOnZ(y + 1, 9, y, row));
    grid_negative_at = -1;
    assert(Ta_GetUnitGravityLoc(y + 1, 9, 0x0102, 0) == Ta_GetUnitGravityLocSearchOnZ(y + 1, 9, y, row));
}

static void check_gated_forward(void)
{
    /* Branch coverage: positive query blocks, b == 0 picks mode 1, b != 0 picks mode 0. */
    stub_g_calls = 0;
    stub_q_result = 1;
    assert(Ta_IsLDAbl(5, 3) == 0 && stub_q_kind == 88 && stub_q_mode == 0 && stub_g_calls == 0);
    stub_q_result = 0;
    assert(Ta_IsLDAbl(5, 0) == 0x7e57 && stub_q_mode == 1 && stub_g_calls == 1);
    assert(stub_g_args[0] == 0 && stub_g_args[1] == 0 && stub_g_args[2] == 5 && stub_g_args[3] == 0);
    stub_q_result = -1;
    assert(Ta_IsLDAbl(5, 3) == 0x7e57 && stub_q_mode == 0 && stub_g_calls == 2);
    assert(stub_g_args[0] == 3 && stub_g_args[1] == 0 && stub_g_args[2] == 5 && stub_g_args[3] == 0);
}

static int fwd_log[8], fwd_count;
static void *c750_m, *c750_axis; static int c750_phase;
void SzView_GetLookVector(int a, int b) { fwd_log[fwd_count++] = 0xafb0; fwd_log[fwd_count++] = a; fwd_log[fwd_count++] = b; float_b[0] = 7.0f; }
void SzView_GetLookMatrix(int a, int b) { fwd_log[fwd_count++] = 0xb0b0; fwd_log[fwd_count++] = a; fwd_log[fwd_count++] = b; }
void SzView_GetLookMatrixWithRoll(int a, int b) { fwd_log[fwd_count++] = 0xb1d0; fwd_log[fwd_count++] = a; fwd_log[fwd_count++] = b; }
void RhGeom_GetRotVectorMatrix(float *m, float *axis, int phase)
{
    c750_m = m; c750_axis = axis; c750_phase = phase;
    m[0] = 3.0f;
}

static void check_block_forwarders(void)
{
    /* Straight-line leaves: one call each, every forwarded argument and the return value checked. */
    struct DotrMiscMatrix out;
    float axis[3] = {1, 2, 3};
    int ang[3] = {4096, 0, 0};
    float r;

    float_log_count = 0; fwd_count = 0; float_b[0] = 1.0f;
    r = Ex3d_GetLookVector(-5);
    assert(r == 7.0f && float_log_count == 1 && float_log[0] == 0x1faf60);
    assert(fwd_count == 3 && fwd_log[0] == 0xafb0 && fwd_log[1] == 0 && fwd_log[2] == -5);

    fwd_count = 0;
    Ex3d_GetLookMatrix(9);
    assert(fwd_count == 3 && fwd_log[0] == 0xb0b0 && fwd_log[1] == 0 && fwd_log[2] == 9);
    fwd_count = 0;
    Ex3d_GetLookMatrixWithRoll(-2);
    assert(fwd_count == 3 && fwd_log[0] == 0xb1d0 && fwd_log[1] == 0 && fwd_log[2] == -2);

    bc00_record = 1; bc00_calls = 0;
    RhGeom_RotXYZMatrix(&out, ang);
    assert(bc00_calls == 1 && bc00_out == &out && bc00_in != &out && bc00_in_00 == RhMath_CosI(ang[1]) * RhMath_CosI(ang[2]));
    RhGeom_RotVectorMatrix(&out, axis, 11);
    assert(bc00_calls == 2 && bc00_out == &out && bc00_in != &out && bc00_in_00 == 3.0f);
    assert(c750_m == bc00_in && c750_axis == axis && c750_phase == 11);
    bc00_record = 0;
}

/* 0x212d70: the per-entry routine only records its arguments. */
static void *entry_seen[4];
static int entry_arg_seen[4], entry_calls;
void RhGeom_MulMatrixRev(void *entry, int arg)
{
    if (entry_calls < 4) { entry_seen[entry_calls] = entry; entry_arg_seen[entry_calls] = arg; }
    entry_calls++;
}

static void check_entry_loop(void)
{
    unsigned char base[112 * 3];
    int k;

    entry_calls = 0;
    lRhModel_ChangeL2VtoL2S(base, 42, 3);
    assert(entry_calls == 3);
    for (k = 0; k < 3; k++)
        assert(entry_seen[k] == base + 112 * k && entry_arg_seen[k] == 42);
    lRhModel_ChangeL2VtoL2S(base, 7, 0);
    lRhModel_ChangeL2VtoL2S(base, 7, -2);
    assert(entry_calls == 3);
    lRhModel_ChangeL2VtoL2S(base, -1, 1);
    assert(entry_calls == 4 && entry_seen[3] == base && entry_arg_seen[3] == -1);
}

static void check_candidate_pick(void)
{
    /* Branch coverage over the real 0x26d6e0/0x26d840: the 0x1e1090 stub gives y 0x45, row 0x123, so
     * with a == y - 1 variant 0 is 0x12344 when cell (y - 1, row) is negative, and variant 1 is
     * 0x12245 when cell (y, row - 1) is negative (b = 9 < row); otherwise they are -1 or the same. */
    const int y = 0x45, row = 0x123, v0 = (row << 8) | (y - 1), v1 = ((row - 1) << 8) | y;
    int table[23];
    int i;

    grid_negative_value = -1;
    for (i = 0; i < 23; i++) table[i] = 1000 + i;
    /* variant 0 fails */
    grid_negative_at = grid_negative_at2 = -1;
    assert(Ta_RequestGravityReserve(y, 9, 0x0102, table) == -1);
    /* variant 0 not in the table: returned as is; an equal word past 22 entries is ignored */
    grid_negative_at = (y - 1) * 7 + row;
    table[22] = v0;
    assert(Ta_RequestGravityReserve(y - 1, 9, 0x0102, table) == v0);
    /* in the table (first and last slot), variant 1 fails */
    for (i = 0; i < 22; i += 21) {
        table[i] = v0;
        assert(Ta_RequestGravityReserve(y - 1, 9, 0x0102, table) == -1);
        table[i] = 1000 + i;
    }
    /* in the table, variant 1 usable and absent -> variant 1; present (first and last slot) -> -1 */
    grid_negative_at2 = y * 7 + (row - 1);
    table[5] = v0;
    assert(Ta_RequestGravityReserve(y - 1, 9, 0x0102, table) == v1);
    for (i = 0; i < 22; i += 21) {
        table[i] = v1;
        assert(Ta_RequestGravityReserve(y - 1, 9, 0x0102, table) == -1);
        table[i] = 1000 + i;
    }
    assert(Ta_RequestGravityReserve(y - 1, 9, 0x0102, table) == v1);
    grid_negative_at = grid_negative_at2 = -1;
}

static int wide14_args[14], wide14_calls;
void lSzDuel_DrawWindow_ExIcon_Pipe(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k,
                   int l, int m, int n)
{
    int v[14] = {a, b, c, d, e, f, g, h, i, j, k, l, m, n};
    memcpy(wide14_args, v, sizeof(v));
    wide14_calls++;
}

static int close_order[2], close_calls, close_descriptor;
void func_00192058(void) { close_order[0] = ++close_calls; }
int func_0017fd20(int descriptor) { close_descriptor = descriptor; close_order[1] = ++close_calls; return 0; }

static int side_calls, side_args[2], side_results[2];
int lSzDuel_ActCustomDuelProc(int side)
{
    side_args[side_calls] = side;
    return side_results[side_calls++];
}
unsigned char customDuelOpponentId[16];

struct DotrMiscLink data_0034fb00;
static int link_open_args[2], link_which, link_status, link_kind, link_flag;
int func_001948a8(int a, int b) { link_open_args[0] = a; link_open_args[1] = b; link_which = 1; return 0x48a8; }
int func_00194e90(int a, int b) { link_open_args[0] = a; link_open_args[1] = b; link_which = 2; return 0x4e90; }
int func_00194248(int a, int *b, int *c)
{
    assert(a == 1 && b == &data_0034fb00.unknown_20 && c == &data_0034fb00.unknown_24);
    *b = link_kind;
    *c = link_flag;
    return link_status;
}

/* TakuSave_card_format and TakuSave_card_unformat (branch coverage): status not 1, wrong kind, flag
 * zero and non-zero. On the host both symbols name one object. */
static void check_link(void)
{
    int pass;
    for (pass = 0; pass < 2; pass++) {
        int kind = pass == 0 ? 16 : 17;
#define LINK_CALL() (pass == 0 ? TakuSave_card_format() : TakuSave_card_unformat())
        memset(&data_0034fb00, 0, sizeof(data_0034fb00));
        data_0034fb00.unknown_08 = 8;
        data_0034fb00.unknown_0c = 12;
        data_0034fb00.unknown_30 = data_0034fb00.unknown_34 = data_0034fb00.unknown_40 = 99;
        link_status = 2;
        link_kind = kind;
        link_flag = 0;
        assert(LINK_CALL() == 0 && link_which == pass + 1 && link_open_args[0] == 8 && link_open_args[1] == 12);
        assert(data_0034fb00.unknown_00 == (pass == 0 ? 0x48a8 : 0x4e90) && data_0034fb00.unknown_04 == 2);
        link_status = 1;
        link_kind = kind + 1;
        assert(LINK_CALL() == 0 && data_0034fb00.unknown_30 == 99);
        link_kind = kind;
        link_flag = 1;
        assert(LINK_CALL() == 13);
        assert(data_0034fb00.unknown_30 == (pass == 0 ? 99 : 14) && data_0034fb00.unknown_40 == (pass == 0 ? 99 : 0));
        assert(data_0034fb00.unknown_34 == (pass == 0 ? 99 : 0));
        link_flag = 0;
        assert(LINK_CALL() == 12 && data_0034fb00.unknown_30 == 14);
        assert(data_0034fb00.unknown_40 == 0 && data_0034fb00.unknown_34 == 0);
#undef LINK_CALL
    }
}

struct DotrMiscEntry24 data_0031bc50[24];
static int entry_or_calls, entry_or_seen;
int SzEffect_Destroy(int index) { entry_or_calls++; entry_or_seen |= 1 << index; return 1 << (index & 7); }

/* SzEffect_Destroy2 (loop and branch coverage): no filter, kind only, id only, both,
 * and filters that match nothing; first and last entry included. */
static void check_entry_or(void)
{
    int i;
    memset(data_0031bc50, 0, sizeof(data_0031bc50));
    for (i = 0; i < 24; i++) {
        data_0031bc50[i].kind = i & 3;
        data_0031bc50[i].id = 1000 + (i >> 2);
    }
    entry_or_calls = entry_or_seen = 0;
    assert(SzEffect_Destroy2(-1, -1) == 0xff && entry_or_calls == 24 && entry_or_seen == 0xffffff);
    entry_or_calls = entry_or_seen = 0;
    assert(SzEffect_Destroy2(3, -1) == 0x88 && entry_or_calls == 6 && entry_or_seen == 0x888888);
    entry_or_calls = entry_or_seen = 0;
    assert(SzEffect_Destroy2(-1, 1005) == 0xf0 && entry_or_calls == 4 && entry_or_seen == 0xf00000);
    entry_or_calls = entry_or_seen = 0;
    assert(SzEffect_Destroy2(0, 1000) == 1 && entry_or_calls == 1 && entry_or_seen == 1);
    assert(SzEffect_Destroy2(4, -1) == 0 && SzEffect_Destroy2(-1, 999) == 0 && entry_or_calls == 1);
}

static int poll_calls, poll_results[3];
int func_001854e0(int a) { assert(a == 0); return poll_results[poll_calls++ % 3]; }

static int mask_args[2], mask_calls;
unsigned Ta_ConvBitLengthToNumericLength(int n) { mask_args[mask_calls++ & 1] = n; return (1u << n) - 1; }

static void check_solo_batch_5(void)
{
    struct DotrMiscTree nodes[4];
    unsigned x = 0xfffff805u;

    /* startDisplay (loop coverage): two equal results, then a different one. */
    poll_calls = 0;
    poll_results[0] = poll_results[1] = 9;
    poll_results[2] = 8;
    frd[0] = 5;
    isCountVblank[0] = 5;
    isCountVblank[2] = 5;
    startDisplay(9);
    assert(poll_calls == 3 && frd[0] == 0 && isCountVblank[0] == 1 && isCountVblank[2] == 0);
    poll_calls = 2;
    startDisplay(9);
    assert(poll_calls == 3);

    /* ConvUnitIDtoFlagList_OFF (branch coverage): flag zero and non-zero. */
    mask_calls = 0;
    assert(ConvUnitIDtoFlagList_OFF(x, 0, 3) == (((((x >> 11) >> 7) & ~1u) << 18) | (x & ((1u << 18) - 1))));
    assert(mask_calls == 1 && mask_args[0] == 18);
    mask_calls = 0;
    assert(ConvUnitIDtoFlagList_OFF(x, 1, 3) == ((x & ~((1u << 8) - 1)) | (x & ((1u << 7) - 1))));
    assert(mask_calls == 2 && mask_args[0] == 8 && mask_args[1] == 7);

    /* TaLogic_IsConnectRoot (branch and loop coverage): null node, depth reached at the
     * root, found through the last and the first link, target not reached. */
    memset(nodes, 0, sizeof(nodes));
    assert(TaLogic_IsConnectRoot(0, 0, 0) == 0);
    assert(TaLogic_IsConnectRoot(&nodes[0], 2, 2) == 1 && TaLogic_IsConnectRoot(&nodes[0], 0, 1) == 0);
    nodes[0].children[11] = &nodes[1];
    assert(TaLogic_IsConnectRoot(&nodes[0], 0, 1) == 1 && TaLogic_IsConnectRoot(&nodes[0], 0, 2) == 0);
    nodes[1].children[0] = &nodes[2];
    assert(TaLogic_IsConnectRoot(&nodes[0], 0, 2) == 1 && TaLogic_IsConnectRoot(&nodes[0], 0, 3) == 0);
}

static int convert_result, convert_calls, *convert_table;
static void *convert_source;
int Ex3d_RotTransPers(void *a, int *b, unsigned short *out, int *w)
{
    (void)w;
    convert_source = a;
    convert_table = b;
    convert_calls++;
    out[0] = 0x7000;
    out[1] = 0x7ff0;
    return convert_result;
}
int data_00299f90[4];

static int blend_args[5], blend_calls;
void SzColor_RGB2SEP(unsigned char *out, int r, int g, int b, int e, int f)
{
    int v[5] = {r, g, b, e, f};
    memcpy(blend_args, v, sizeof(v));
    blend_calls++;
    out[0] = 0xff;
    out[1] = 0x21;
    out[2] = 0x45;
}

static void check_solo_batch_4(void)
{
    int x, y, source;

    /* Taku_3D_2D_Set (branch coverage): conversion failed and succeeded. */
    convert_result = 0;
    x = y = 5;
    Taku_3D_2D_Set(&x, &y, &source);
    assert(x == -1 && y == -1 && convert_source == &source && convert_table == data_00299f90);
    convert_result = 1;
    Taku_3D_2D_Set(&x, &y, &source);
    assert(x == 64 && y == 223 && convert_calls == 2);

    /* SzColor_CLUT2SEP (branch coverage): low 15 bits zero (with and without bit 15
     * and bit 16), then a conversion with and without bit 15. */
    blend_calls = 0;
    assert(SzColor_CLUT2SEP(0, 9) == 0 && SzColor_CLUT2SEP(0x8000, 9) == 0x8000);
    assert(SzColor_CLUT2SEP(0x10000, 9) == 0x10000 && blend_calls == 0);
    assert(SzColor_CLUT2SEP(0x8000 | (3 << 10) | (2 << 5) | 1, 9) == (0x8000 | (5 << 10) | (1 << 5) | 0x1f));
    assert(blend_calls == 1 && blend_args[0] == 1 && blend_args[1] == 2 && blend_args[2] == 3);
    assert(blend_args[3] == 9 && blend_args[4] == 31);
    assert(SzColor_CLUT2SEP(0x0421, 9) == ((5 << 10) | (1 << 5) | 0x1f) && blend_calls == 2);
}

static struct DotrMiscRect *rect_seen;
static int rect_calls;
void SzDuel_DoCardControl0(struct DotrMiscRect *p) { rect_seen = p; rect_calls++; }

struct DotrMiscControlCb2 data_00317490_cb2;
static int pair_calls_3, pair_log_3[20][2];
int SzDuel_DelUnit(int a, int b)
{
    pair_log_3[pair_calls_3 % 20][0] = a;
    pair_log_3[pair_calls_3 % 20][1] = b;
    pair_calls_3++;
    return 1;
}

char data_0030f354[8];
int data_003503b8[4], videoDec[4], readBuf[4], infile[4], controller_val[4];
static int run_order[3], run_count, run_arg, run_word, *run_a, *run_c;
int data_003503c0[16];
static int screen_calls, screen_args[5], *screen_buffer;
void clearGsMem(int a, int b, int c, int d, int e)
{
    assert(screen_calls++ == 0 && a == 0 && b == 0 && c == 0);
    screen_args[0] = d;
    screen_args[1] = e;
}
void func_001851e8(int *buffer, int a, int b, int c, int d, int e, int f)
{
    assert(screen_calls++ == 1 && a == 0 && d == 0 && e == 0);
    screen_buffer = buffer;
    screen_args[2] = b;
    screen_args[3] = c;
    screen_args[4] = f;
}
void func_0017bb60(int a) { assert(screen_calls++ == 2 && a == 0); }
void initAll(int a) { run_order[0] = ++run_count; run_arg = a; }
void readMpeg(int *a, int b, int *c) { run_order[1] = ++run_count; run_a = a; run_word = b; run_c = c; }
void termAll(void) { run_order[2] = ++run_count; }

static int *kind_seen;
int Ta_Eff_Hand_Fusion(int *p) { kind_seen = p; return 0x30; }
int Ta_Eff_Hand_Enc(int *p) { kind_seen = p; return 0x260; }
int Ta_Eff_Hand_Fail(int *p) { kind_seen = p; return 0xfe70; }

static void check_solo_batch_3(void)
{
    struct DotrMiscRect rect;
    int i, word;

    /* lSzDuel_SetCardCtl (branch coverage): null object, then every stored value. */
    rect_calls = 0;
    assert(lSzDuel_SetCardCtl(0, 1, 2, 3, 4, 5, 6) == 0 && rect_calls == 0);
    memset(&rect, 0xff, sizeof(rect));
    assert(lSzDuel_SetCardCtl(&rect, -1, 2, 3, 4, 5, 6) == 1 && rect_calls == 1 && rect_seen == &rect);
    assert(rect.unknown_10 == -1.0f && rect.unknown_14 == 2.0f && rect.unknown_18 == 0 && rect.unknown_1c == 0);
    assert(rect.unknown_20 == 3.0f && rect.unknown_24 == 4.0f && rect.unknown_28 == 0 && rect.unknown_2c == 0);
    assert(rect.unknown_98 == 6 && rect.unknown_9a == 5);
    assert(rect.unknown_00[0xf] == 0xff && rect.unknown_30[0] == 0xff && rect.unknown_30[0x67] == 0xff);

    /* SzDuel_Term (loop coverage): four bytes, then pairs for i = 1..10. */
    memset(&data_00317490_cb2, 0x55, sizeof(data_00317490_cb2));
    pair_calls_3 = 0;
    SzDuel_Term();
    assert(data_00317490_cb2.unknown_000[0xcb1] == 0x55);
    for (i = 0; i < 4; i++) {
        assert(data_00317490_cb2.unknown_cb2[i] == 24);
    }
    assert(pair_calls_3 == 20);
    for (i = 0; i < 20; i++) {
        assert(pair_log_3[i][0] == (i & 1) && pair_log_3[i][1] == i / 2 + 1);
    }

    /* 0x242580 (straight-line coverage): arguments, order, counter, result. */
    data_003503b8[0] = 41;
    readBuf[0] = 0x788;
    controller_val[0] = 0x5f0;
    run_count = 0;
    assert(func_00242580(0x77) == 0x5f0);
    assert(print_text == data_0030f354 && print_value == 41 && data_003503b8[0] == 42);
    assert(run_order[0] == 1 && run_order[1] == 2 && run_order[2] == 3 && run_arg == 0x77);
    assert(run_a == videoDec && run_word == 0x788 && run_c == infile);

    /* movie_play (straight-line coverage): every stored word, every forwarded
     * argument, and the final call through the real 0x242580. */
    memset(isCountVblank, 0, sizeof(isCountVblank));
    data_003503b8[0] = 41;
    controller_val[0] = 0x5f0;
    run_count = 0;
    run_arg = 0;
    screen_calls = 0;
    movie_play(0x66, 11, 22);
    assert(isWithAudio[0] == 11 && isCountVblank[28] == 22);
    assert(screen_calls == 3 && screen_args[0] == 640 && screen_args[1] == 448);
    assert(screen_buffer == data_003503c0 && screen_args[2] == 640 && screen_args[3] == 224 && screen_args[4] == 1);
    assert(isCountVblank[10] == 0x01200000 && isCountVblank[12] == 0x0152A000 && isCountVblank[14] == 0x015C3C80);
    assert(isCountVblank[16] == 0x015C4CC0 && isCountVblank[18] == 0x0178DE40 && isCountVblank[20] == 0x017DE680);
    assert(isCountVblank[22] == 0x017EA680 && isCountVblank[24] == 0x0186A680 && isCountVblank[26] == 0x0186E680);
    assert(isCountVblank[0] == 0 && isCountVblank[11] == 0 && isCountVblank[27] == 0);
    assert(run_count == 3 && run_arg == 0x66 && run_word == 0x0178E640 && data_003503b8[0] == 42);

    /* Ta_ManageHandEffect (branch coverage): kinds 0, 1, 2 and both neighbours. */
    word = 0;
    assert(Ta_ManageHandEffect(&word) == 0x30 && kind_seen == &word);
    word = 1;
    assert(Ta_ManageHandEffect(&word) == 0x260);
    word = 2;
    assert(Ta_ManageHandEffect(&word) == 0xfe70);
    kind_seen = 0;
    word = 3;
    assert(Ta_ManageHandEffect(&word) == 0);
    word = -1;
    assert(Ta_ManageHandEffect(&word) == 0 && kind_seen == 0);
}

static int mode_args[3], mode_calls;
int Do_Match_AtkEfcObj(int a, int b, int c)
{
    mode_args[0] = a;
    mode_args[1] = b;
    mode_args[2] = c;
    mode_calls++;
    return 0x26f0;
}

static void check_solo_batch_2(void)
{
    struct DotrMiscChain chain[10];
    int i;

    /* TaLogic_GetNextPiece (branch coverage): depth bound 6/7, no successor, successor
     * without successor, one recursion, recursion reaching the bound. */
    memset(chain, 0, sizeof(chain));
    for (i = 0; i < 10; i++) {
        chain[i].loc = (unsigned char)(0x40 + i);
        chain[i].parent = i < 9 ? &chain[i + 1] : 0;
    }
    assert(TaLogic_GetNextPiece(&chain[9], 0) == 255);
    assert(TaLogic_GetNextPiece(&chain[8], 0) == 0x48);
    assert(TaLogic_GetNextPiece(&chain[8], 6) == 0x48 && TaLogic_GetNextPiece(&chain[8], 7) == 255);
    assert(TaLogic_GetNextPiece(&chain[7], 0) == 0x48 && TaLogic_GetNextPiece(&chain[7], 5) == 0x48);
    assert(TaLogic_GetNextPiece(&chain[7], 6) == 255);
    assert(TaLogic_GetNextPiece(&chain[2], 0) == 0x48 && TaLogic_GetNextPiece(&chain[1], 0) == 255);

    /* Match_AtkEfcObj (branch coverage): modes 1 and 2, and both neighbours. */
    mode_calls = 0;
    assert(Match_AtkEfcObj(1, 10, 11, 12, 13, 14) == 0x26f0 && mode_calls == 1);
    assert(mode_args[0] == 10 && mode_args[1] == 11 && mode_args[2] == 12);
    assert(Match_AtkEfcObj(2, 10, 11, 12, 13, 14) == 0x26f0 && mode_calls == 2);
    assert(mode_args[0] == 10 && mode_args[1] == 13 && mode_args[2] == 14);
    assert(Match_AtkEfcObj(0, 10, 11, 12, 13, 14) == 0 && Match_AtkEfcObj(3, 10, 11, 12, 13, 14) == 0);
    assert(mode_calls == 2);
}

static void check_solo_batch(void)
{
    static const int want[14] = {1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 9, 10, 11, 128};
    struct DotrMiscClose closing;
    int i;

    /* lSzDuel_DrawWindow_Icon_Pipe (forwarder coverage): every forwarded argument. */
    wide14_calls = 0;
    lSzDuel_DrawWindow_Icon_Pipe(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    assert(wide14_calls == 1 && memcmp(wide14_args, want, sizeof(want)) == 0);

    /* SzDuel_GetCardRequest (loop coverage): first element free, only the last free, none free. */
    node_want = 0;
    for (i = 0; i < 32; i++) {
        *(int *)(gSzDuel_aCardRequest[i].unknown_bytes + 0x90) = i == 0 || i == 31 ? 0 : 7;
    }
    node_calls = 0;
    assert(SzDuel_GetCardRequest() == (struct DotrMiscNode *)&gSzDuel_aCardRequest[0] && node_calls == 1);
    assert(node_seen == (struct DotrMiscNode *)&gSzDuel_aCardRequest[0]);
    *(int *)(gSzDuel_aCardRequest[0].unknown_bytes + 0x90) = 7;
    assert(SzDuel_GetCardRequest() == (struct DotrMiscNode *)&gSzDuel_aCardRequest[31] && node_calls == 2);
    assert(node_seen == (struct DotrMiscNode *)&gSzDuel_aCardRequest[31]);
    *(int *)(gSzDuel_aCardRequest[31].unknown_bytes + 0x90) = 7;
    assert(SzDuel_GetCardRequest() == 0 && node_calls == 2);
    node_want = 0x55;

    /* strFileClose (branch coverage): non-zero and zero first word. */
    memset(&closing, 0, sizeof(closing));
    closing.unknown_00 = 1;
    closing.unknown_2c = 0x2c2c;
    closing.unknown_30 = 0x3030;
    f_calls[7] = close_calls = close_order[0] = close_order[1] = 0;
    assert(strFileClose(&closing) == 1);
    assert(close_calls == 1 && close_order[0] == 1 && f_calls[7] == 1 && f_args[0] == 0x2c2c);
    closing.unknown_00 = 0;
    assert(strFileClose(&closing) == 1);
    assert(close_calls == 2 && close_order[1] == 2 && close_descriptor == 0x3030 && f_calls[7] == 1);
    f_calls[7] = 0;

    /* SzDuel_ActCustomDuel (branch coverage): first call non-zero; zero with the byte set;
     * zero with the byte clear. */
    side_calls = 0;
    side_results[0] = 1;
    customDuelOpponentId[0] = 0;
    SzDuel_ActCustomDuel();
    assert(side_calls == 1 && side_args[0] == 0);
    side_calls = 0;
    side_results[0] = 0;
    customDuelOpponentId[0] = 1;
    SzDuel_ActCustomDuel();
    assert(side_calls == 1 && side_args[0] == 0);
    side_calls = 0;
    customDuelOpponentId[0] = 0;
    SzDuel_ActCustomDuel();
    assert(side_calls == 2 && side_args[0] == 0 && side_args[1] == 1);
}

static char number_text[17];
static int number_width;
DotrMiscAddress SzDuel_DrawWindow_Font_Pipe(DotrMiscAddress cursor, int x, int y, int a, int flags, int n, const char *text, int c)
{
    assert(cursor == 0x1000 && x == 2 && y == 3 && a == 4 && n == number_width && c == 0);
    assert(flags == 0 || flags == 0x20 || flags == 0xdf);
    memset(number_text, 0, sizeof(number_text));
    memcpy(number_text, text, n > 0 ? (size_t)n : 0);
    return 0x2000 + flags;
}

static void check_number(int flags, int width, int value, const char *expect)
{
    number_width = width;
    assert(SzDuel_DrawWindow_FontDigit_Pipe(0x1000, 2, 3, 4, flags, width, value) == 0x2000 + flags);
    assert(strcmp(number_text, expect) == 0);
}

static void check_number_text(void)
{
    /* Width bound: zero or negative width writes nothing. */
    check_number(0, 0, 123, "");
    check_number(0, -1, 123, "");
    /* Bit 0x20 clear: stops after the last significant digit, rest spaces. */
    check_number(0, 5, 123, "  123");
    check_number(0xdf, 5, 0, "    0");
    check_number(0, 3, 9, "  9");
    check_number(0, 3, 10, " 10");
    /* Width smaller than the number keeps the low digits. */
    check_number(0, 2, 123, "23");
    check_number(0, 16, 1, "               1");
    /* Bit 0x20 set: zero padded to the width. */
    check_number(0x20, 5, 123, "00123");
    check_number(0x20, 1, 0, "0");
}

struct DotrMiscControlCounts data_00317490_counts;
static unsigned char count_first[856], count_second[856];
static int count_calls, count_sides[2];
void lSzDuel_TradeDoSide(int side)
{
    /* Runs after the cancelling loop and before the sum: its change is counted. */
    assert(count_first[0] == 4 && count_second[0] == 0);
    count_sides[count_calls++] = side;
    if (side == 1)
        count_second[853] += 1;
}

/* 0x1c32b0 branch coverage: first zero, second zero, first larger, second
 * larger, equal (takes the else arm), plus the last index 853 and the guard
 * bytes at 854 that must stay untouched. */
static void check_count_cancel(void)
{
    memset(count_first, 0, sizeof(count_first));
    memset(count_second, 0, sizeof(count_second));
    data_00317490_counts.first = count_first;
    data_00317490_counts.second = count_second;
    count_first[0] = 9; count_second[0] = 5;       /* first larger: 4, 0 */
    count_first[1] = 0; count_second[1] = 7;       /* first zero: kept */
    count_first[2] = 6; count_second[2] = 0;       /* second zero: kept */
    count_first[3] = 2; count_second[3] = 250;     /* second larger: 0, 248 */
    count_first[4] = 200; count_second[4] = 200;   /* equal: 0, 0 */
    count_first[853] = 255; count_second[853] = 1; /* last index: 254, 0 */
    count_first[854] = 3; count_second[854] = 2;   /* beyond the bound */
    count_calls = 0;
    assert(lSzDuel_TradeDo() == 4 + 7 + 6 + 248 + 254 + 1);
    assert(count_calls == 2 && count_sides[0] == 0 && count_sides[1] == 1);
    assert(count_first[0] == 4 && count_second[0] == 0 && count_first[1] == 0 && count_second[1] == 7);
    assert(count_first[2] == 6 && count_second[2] == 0 && count_first[3] == 0 && count_second[3] == 248);
    assert(count_first[4] == 0 && count_second[4] == 0);
    assert(count_first[853] == 254 && count_second[853] == 1);
    assert(count_first[854] == 3 && count_second[854] == 2);
}

void viBufGetTs(void *p, long *out)
{
    assert(p == (void *)data_003506c8);
    out[0] = 0x1122334455667788L;
    out[1] = -2L;
    out[2] = 3;
    out[3] = 4;
}

/* 0x2455e0 straight-line: both stored words, the untouched neighbours and the result. */
static void check_position_callback(void)
{
    long out[4] = { 70, 71, 72, 73 };

    assert(mpegTS(0, out) == 1);
    assert(out[0] == 70 && out[1] == 0x1122334455667788L && out[2] == -2L && out[3] == 73);
}

struct DotrMiscSortList gRhZSort_zsl_list;
static int sort_calls;
void RhMath_HeapSortStruct(void *base, int count, int size, int (*compare)(int *, int *))
{
    struct DotrMiscSortEntry *e = base, tmp;

    sort_calls++;
    assert(count == 2 && size == 8 && compare == lRhZSort_ZCompare);
    if (compare(&e[0].key, &e[1].key) > 0) {
        tmp = e[0];
        e[0] = e[1];
        e[1] = tmp;
    }
}

static void check_sorted_emit(void)
{
    struct DotrMiscSortEntry entries[3] = { { 9, 0x111 }, { 4, 0x222 }, { 1, 0x333 } };
    int out[9], a = 5, b = -2, i;

    /* Comparator: straight-line difference of the two keys. */
    assert(lRhZSort_ZCompare(&a, &b) == 7);
    for (i = 0; i < 9; i++) {
        out[i] = 77;
    }
    /* Empty list (first == end): cursor returned, nothing called or stored. */
    gRhZSort_zsl_list.out = 0;
    gRhZSort_zsl_list.first = gRhZSort_zsl_list.end = &entries[2];
    gRhZSort_zsl_list.sorted = 0;
    assert(RhZSort_Finish_Pipe(out) == out && sort_calls == 0 && out[0] == 77);
    assert(gRhZSort_zsl_list.out == 0 && gRhZSort_zsl_list.sorted == 0);
    /* Two entries: sorted through the callee, one four-word record each. */
    gRhZSort_zsl_list.first = entries;
    assert(RhZSort_Finish_Pipe(out) == out + 8 && sort_calls == 1);
    assert(out[0] == 0x50000000 && out[1] == 0x222 && out[2] == 0 && out[3] == 0);
    assert(out[4] == 0x50000000 && out[5] == 0x111 && out[6] == 0 && out[7] == 0 && out[8] == 77);
    assert(gRhZSort_zsl_list.out == out + 8 && gRhZSort_zsl_list.first == &entries[2]);
    assert(gRhZSort_zsl_list.end == &entries[2] && gRhZSort_zsl_list.sorted == 1);
}

void SzDuel_SetHandPhase(int index, int value)
{
    job_log[job_log_count++] = 0xde7;
    job_log[job_log_count++] = index;
    job_log[job_log_count++] = value;
}

/* 0x26be20 branch coverage; same stubs as check_job_queue plus 0x1e1e10. */
static int job_e10_result, job_e10_args[2], job_e10_calls;
int SzDuel_GetUnitOpenRate(int a, int b) { job_e10_args[0] = a; job_e10_args[1] = b; job_e10_calls++; return job_e10_result; }
static void check_job_queue_601(void)
{
    DotrMiscJob in = {0, 11, 0x1234, 33};
    DotrMiscJobQueue q;

    /* mode >= 0 (0 and positive): reset calls and TaUtil_DoOpen(0x12, 0x34, 1), return 1. */
    memset(&q, 0, sizeof(q));
    job_log_count = 0;
    notify_calls = 0;
    job_e10_calls = 0;
    assert(Ta_Eff_101(&in, &q, 0) == 1 && job_log_count == 4 && notify_calls == 1);
    assert(job_log[0] == 0x960 && job_log[1] == 1 && job_log[2] == 0xac0 && job_log[3] == 601);
    assert(notify_args[0] == 0x12 && notify_args[1] == 0x34 && notify_args[2] == 1);
    job_log_count = 0;
    assert(Ta_Eff_101(&in, &q, 5) == 1 && job_log_count == 4 && notify_calls == 2 && job_e10_calls == 0);

    /* mode < 0, status != 1: nothing else happens. */
    job_log_count = 0;
    job_b50_result = 0;
    assert(Ta_Eff_101(&in, &q, -1) == 1 && job_log_count == 1 && job_log[0] == 0xb50 && q.count == 0
        && job_e10_calls == 0);

    /* status 1, 0x1e1e10 != 0xFF: asked with (0x12, 0x34), nothing queued. */
    job_log_count = 0;
    job_b50_result = 1;
    job_e10_result = 0xFE;
    assert(Ta_Eff_101(&in, &q, -1) == 1 && job_log_count == 1 && q.count == 0 && job_e10_calls == 1
        && job_e10_args[0] == 0x12 && job_e10_args[1] == 0x34);

    /* 0xFF, count 0: request 601 carrying second and value, slot stored, count incremented. */
    memset(&job_req_seen, 0x55, sizeof(job_req_seen));
    job_log_count = 0;
    job_e10_result = 0xFF;
    job_a30_result = 0x5b;
    assert(Ta_Eff_101(&in, &q, -1) == 1 && job_log_count == 2 && job_log[1] == 0xa30);
    assert(job_req_seen.unknown_00 == 601 && job_req_seen.first == 0 && job_req_seen.second == 0x1234
        && job_req_seen.value == 33);
    assert(q.slot == 0x5b && q.count == 1);

    /* count != 0, slot check == 1: returns 1 without further calls. */
    list_count = 0;
    list_results[0] = 1;
    job_log_count = 0;
    assert(Ta_Eff_101(&in, &q, -1) == 1 && job_log_count == 1 && list_calls[0] == 0x5b && q.count == 1);

    /* slot check != 1: SzDuel_CloseMsgPopup, returns 0. */
    list_count = 0;
    list_results[0] = 0;
    job_log_count = 0;
    assert(Ta_Eff_101(&in, &q, -1) == 0 && job_log_count == 2 && job_log[1] == 0x453 && notify_calls == 2);
}

/* 0x25ee40 branch coverage; same stubs as check_job_queue. */
static void check_job_queue_533(void)
{
    DotrMiscJob in = {0, 11, 22, 33};
    DotrMiscJobQueue q;

    /* mode >= 0 (0 and positive): reset calls, return 1. */
    memset(&q, 0, sizeof(q));
    job_log_count = 0;
    assert(Ta_Eff_033(&in, &q, 0) == 1 && job_log_count == 4);
    assert(job_log[0] == 0x960 && job_log[1] == 1 && job_log[2] == 0xac0 && job_log[3] == 533);
    job_log_count = 0;
    assert(Ta_Eff_033(&in, &q, 5) == 1 && job_log_count == 4);

    /* mode < 0, status != 1: nothing else happens. */
    job_log_count = 0;
    job_b50_result = 0;
    assert(Ta_Eff_033(&in, &q, -1) == 1 && job_log_count == 1 && job_log[0] == 0xb50 && q.count == 0);

    /* status 1, count 0: all-zero request with id 533, slot stored, count incremented. */
    memset(&q, 0, sizeof(q));
    memset(&job_req_seen, 0x55, sizeof(job_req_seen));
    job_log_count = 0;
    job_b50_result = 1;
    job_a30_result = 0x5b;
    assert(Ta_Eff_033(&in, &q, -1) == 1 && job_log_count == 2 && job_log[1] == 0xa30);
    assert(job_req_seen.unknown_00 == 533 && job_req_seen.first == 0 && job_req_seen.second == 0 && job_req_seen.value == 0);
    assert(q.slot == 0x5b && q.count == 1);

    /* status 1, count != 0, slot check == 1: returns 1 without further calls. */
    list_count = 0;
    list_results[0] = 1;
    job_log_count = 0;
    assert(Ta_Eff_033(&in, &q, -1) == 1 && job_log_count == 1 && list_calls[0] == 0x5b && q.count == 1);

    /* slot check != 1: SzDuel_SetHandPhase(in->second, 1), SzDuel_CloseMsgPopup, returns 0. */
    list_count = 0;
    list_results[0] = 0;
    job_log_count = 0;
    assert(Ta_Eff_033(&in, &q, -1) == 0 && job_log_count == 5);
    assert(job_log[1] == 0xde7 && job_log[2] == 22 && job_log[3] == 1 && job_log[4] == 0x453);
}

int Ta_DelUnit2(int a, int b)
{
    job_log[job_log_count++] = 0xf43;
    job_log[job_log_count++] = a;
    job_log[job_log_count++] = b;
    return 0;
}

/* 0x271630 branch coverage; same stubs as check_job_queue. */
static void check_job_queue_603(void)
{
    DotrMiscJob in = {0, 11, (int)0x80000a07, 33};
    DotrMiscJobQueue q;

    /* mode >= 0 (0 and positive): reset calls, return 1. */
    memset(&q, 0, sizeof(q));
    job_log_count = 0;
    assert(Ta_Eff_Hand_Dest(&in, &q, 0) == 1 && job_log_count == 4);
    assert(job_log[0] == 0x960 && job_log[1] == 1 && job_log[2] == 0xac0 && job_log[3] == 603);
    job_log_count = 0;
    assert(Ta_Eff_Hand_Dest(&in, &q, 5) == 1 && job_log_count == 4);

    /* mode < 0, count 0, status != 1: nothing else happens. */
    job_log_count = 0;
    job_b50_result = 0;
    assert(Ta_Eff_Hand_Dest(&in, &q, -1) == 1 && job_log_count == 1 && job_log[0] == 0xb50 && q.count == 0);

    /* count 0, status 1: request with id 603 and in->second, slot stored, count incremented. */
    memset(&job_req_seen, 0x55, sizeof(job_req_seen));
    job_log_count = 0;
    job_b50_result = 1;
    job_a30_result = 0x5b;
    assert(Ta_Eff_Hand_Dest(&in, &q, -1) == 1 && job_log_count == 2 && job_log[1] == 0xa30);
    assert(job_req_seen.unknown_00 == 603 && job_req_seen.first == 0 && job_req_seen.second == (int)0x80000a07 && job_req_seen.value == 0);
    assert(q.slot == 0x5b && q.count == 1);

    /* count != 0: 0x255b50 is not asked; slot check nonzero (1 and 2) returns 1 without further calls. */
    list_count = 0;
    list_results[0] = 1;
    list_results[1] = 2;
    job_log_count = 0;
    assert(Ta_Eff_Hand_Dest(&in, &q, -1) == 1 && job_log_count == 0 && list_calls[0] == 0x5b && q.count == 1);
    assert(Ta_Eff_Hand_Dest(&in, &q, -1) == 1 && job_log_count == 0 && list_count == 2);

    /* slot check 0: 0x24f430 with the logical high part and the low byte, SzDuel_CloseMsgPopup,
     * returns 0. */
    list_count = 0;
    list_results[0] = 0;
    job_log_count = 0;
    assert(Ta_Eff_Hand_Dest(&in, &q, -1) == 0 && job_log_count == 4);
    assert(job_log[0] == 0xf43 && job_log[1] == 0x80000a && job_log[2] == 7 && job_log[3] == 0x453 && q.count == 1);
}

static int job_780_result, job_5c0_result;
void SzDuel_SetWindowDraw(int value) { job_log[job_log_count++] = 0x7e0; job_log[job_log_count++] = value; }
int SzDuel_GetWindowDisp(void) { job_log[job_log_count++] = 0x780; return job_780_result; }
int SzDuel_GetSummonPower(int index) { job_log[job_log_count++] = 0x5c0; job_log[job_log_count++] = index; return job_5c0_result; }

/* 0x263790 branch coverage; same stubs as check_job_queue. */
static void check_job_queue_534(void)
{
    DotrMiscJob in = {0, 11, 7, 33};
    DotrMiscJobQueue q;

    /* mode >= 0 (0 and positive): flag call, reset calls, return 1. */
    memset(&q, 0, sizeof(q));
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, 0) == 1 && job_log_count == 6 && job_log[0] == 0x7e0 && job_log[1] == 0);
    assert(job_log[2] == 0x960 && job_log[3] == 1 && job_log[4] == 0xac0 && job_log[5] == 534);
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, 5) == 1 && job_log_count == 6);

    /* mode < 0, count 0, status != 1: nothing else happens. */
    job_log_count = 0;
    job_b50_result = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 1 && job_log_count == 3 && job_log[2] == 0xb50 && q.count == 0);

    /* status 1, value above the looked-up word: second 0, value - word. */
    job_log_count = 0;
    job_b50_result = 1;
    job_a30_result = 0x5b;
    job_5c0_result = 20;
    assert(Ta_Eff_065(&in, &q, -1) == 1 && job_log_count == 8 && job_log[3] == 0x5c0 && job_log[4] == 7);
    assert(job_log[5] == 0x5c0 && job_log[6] == 7 && job_log[7] == 0xa30);
    assert(job_req_seen.unknown_00 == 534 && job_req_seen.first == 0 && job_req_seen.second == 0 && job_req_seen.value == 13);
    assert(q.slot == 0x5b && q.count == 1);
    /* equal: still second 0; below: second 1, word - value; the compare is unsigned. */
    q.count = 0;
    in.value = 20;
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 1 && job_req_seen.second == 0 && job_req_seen.value == 0);
    q.count = 0;
    in.value = 19;
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 1 && job_log_count == 8);
    assert(job_req_seen.unknown_00 == 534 && job_req_seen.first == 0 && job_req_seen.second == 1 && job_req_seen.value == 1);
    q.count = 0;
    in.value = -1;
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 1 && job_req_seen.second == 0 && job_req_seen.value == -21 && q.count == 1);

    /* count != 0: slot check 1 returns 1 after the flag call only. */
    in.value = 33;
    list_count = 0;
    list_results[0] = 1;
    list_results[1] = 2;
    list_results[2] = 0;
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 1 && job_log_count == 2 && list_calls[0] == 0x5b && q.count == 1);
    /* slot check not 1 (2 and 0): SzDuel_CalcSummonPower(second, 4, value), then
     * SzDuel_SetWindowDraw(1) only when 0x1de780 is set. */
    job_780_result = 0;
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 0 && job_log_count == 8);
    assert(job_log[2] == 0xde5 && job_log[3] == 7 && job_log[4] == 4 && job_log[5] == 33);
    assert(job_log[6] == 0x780 && job_log[7] == 0x453);
    job_780_result = 1;
    job_log_count = 0;
    assert(Ta_Eff_065(&in, &q, -1) == 0 && job_log_count == 10 && list_count == 3);
    assert(job_log[6] == 0x780 && job_log[7] == 0x7e0 && job_log[8] == 1 && job_log[9] == 0x453 && q.count == 1);
}

static int emit_words[8], emit_positions[8], emit_count;
int func_00202fb0_word(int cursor, int a0, int a1, int *count)
{
    assert(cursor == 0x900 + emit_count);
    emit_words[emit_count] = a0;
    emit_positions[emit_count] = a1;
    *count = 10 + emit_count;
    emit_count++;
    return cursor + 1;
}

/* 0x202b00 branch coverage: zero word, bit 0 clear, bit 1 set, kind mismatch,
 * negative kind, null and non-null output, last entry of the 1024. */
static void check_scan_emit(void)
{
    int out = -1, i;

    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
    for (i = 1; i <= 6; i++) {
        gSzText_aItem[i].unknown_00 = i == 2 ? 0 : 0x100 + i;
        gSzText_aItem[i].unknown_08.unknown_bit_0 = i == 3 ? 0 : -1;
        gSzText_aItem[i].unknown_08.unknown_bit_1 = i == 4 ? -1 : 0;
        gSzText_aItem[i].unknown_08.unknown_bits_3_15 = (short)(i == 5 ? 5 : 7);
    }
    gSzText_aItem[1023].unknown_00 = 0x4ff;
    gSzText_aItem[1023].unknown_08.unknown_bit_0 = -1;
    gSzText_aItem[1023].unknown_08.unknown_bits_3_15 = 7;
    /* kind 7: entries 1, 6 and 1023; positions advance by the callee's counts. */
    emit_count = 0;
    assert(SzText_DoRequest_Pipe(0x900, 7, 0x50, &out) == 0x903 && emit_count == 3 && out == 10 + 11 + 12);
    assert(emit_words[0] == 0x101 && emit_words[1] == 0x106 && emit_words[2] == 0x4ff);
    assert(emit_positions[0] == 0x50 && emit_positions[1] == 0x5a && emit_positions[2] == 0x65);
    /* Negative kind: entry 5 as well; null output is not written. */
    emit_count = 0;
    assert(SzText_DoRequest_Pipe(0x900, -1, 0, 0) == 0x904 && emit_count == 4 && emit_words[1] == 0x105);
    /* kind 0 is compared (not treated as "any"): nothing matches, output 0. */
    emit_count = 0;
    assert(SzText_DoRequest_Pipe(0x900, 0, 0x50, &out) == 0x900 && emit_count == 0 && out == 0);
    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
}

static void check_word_sort(void)
{
    int words[6] = {0x003, 0x101, 0x005, 0x000, 0x10a, 0x002};
    int one[2] = {0x001, 0x002};
    int i;

    memset(scores, 0, sizeof(scores));
    scores[0][3] = 30;
    scores[1][1] = 60;
    scores[0][5] = 10;
    scores[0][0] = 50;
    scores[1][10] = 40;
    scores[0][2] = 20;
    scores[0][1] = 5;
    /* low == high and low > high leave the words alone */
    SortByOrder(one, 1, 1);
    SortByOrder(one, 1, 0);
    assert(one[0] == 0x001 && one[1] == 0x002);
    /* two words: swapped only when the later key is greater */
    SortByOrder(one, 0, 1);
    assert(one[0] == 0x002 && one[1] == 0x001);
    SortByOrder(one, 0, 1);
    assert(one[0] == 0x002 && one[1] == 0x001);
    /* a sub-range sorts by descending key and leaves the last word outside it */
    SortByOrder(words, 0, 4);
    assert(words[0] == 0x101 && words[1] == 0x000 && words[2] == 0x10a);
    assert(words[3] == 0x003 && words[4] == 0x005 && words[5] == 0x002);
    SortByOrder(words, 0, 5);
    assert(words[3] == 0x003 && words[4] == 0x002 && words[5] == 0x005);
    for (i = 0; i < 2; i++) scores[i][0] = 0;
}

static int probe_results[8], probe_args[8][2], probe_count;
int Ta_GetSommonPiecePreference(int a, int i)
{
    assert(probe_count < 8);
    probe_args[probe_count][0] = a;
    probe_args[probe_count][1] = i;
    probe_count++;
    return probe_results[i];
}

static void check_probe_free_cell(void)
{
    int saved_kind = kind_result, saved_cell = kind_cell;
    int saved_at = grid_negative_at, saved_at2 = grid_negative_at2, saved_value = grid_negative_value;
    int i;

    kind_result = 0;
    kind_cell = -1;
    grid_negative_at = grid_negative_at2 = -1;
    grid_negative_value = -1;
    /* every probe negative: all eight indices asked, no cell looked at */
    for (i = 0; i < 8; i++) probe_results[i] = -1;
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePiece(9, 0) == -1);
    assert(probe_count == 8 && grid_count == 0);
    for (i = 0; i < 8; i++) assert(probe_args[i][0] == 9 && probe_args[i][1] == i);
    /* positions pack y << 8 | x; (2,1) is kind 8 and free, (4,3) occupied, (5,0) free */
    probe_results[1] = 0x102;
    probe_results[2] = 0x304;
    probe_results[3] = 0x005;
    kind_cell = 2 * 7 + 1;
    grid_negative_at = 2 * 7 + 1;
    grid_negative_at2 = 5 * 7 + 0;
    /* flag clear: the first free cell wins whatever its kind */
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePiece(4, 0) == 0x102);
    assert(probe_count == 2 && grid_count == 1);
    assert(grid_calls[0][0] == 2 && grid_calls[0][1] == 1);
    /* flag set: kind 8 is skipped before the cell test, occupied is skipped, free returns */
    probe_count = grid_count = 0;
    kind_args[0] = kind_args[1] = -1;
    assert(Ta_GetSummonablePiece(4, 1) == 0x005);
    assert(probe_count == 4 && grid_count == 2);
    assert(grid_calls[0][0] == 4 && grid_calls[0][1] == 3);
    assert(grid_calls[1][0] == 5 && grid_calls[1][1] == 0);
    assert(kind_args[0] == 5 && kind_args[1] == 0);
    /* nothing free: -1 after all eight, for either flag */
    grid_negative_at = grid_negative_at2 = -1;
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePiece(4, 1) == -1);
    assert(probe_count == 8 && grid_count == 2);
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePiece(4, 0) == -1);
    assert(probe_count == 8 && grid_count == 3);
    /* 0x24e300: the idx-th accepted position; (2,1) kind 8 free, (4,3) occupied, (5,0) free */
    grid_negative_at = 2 * 7 + 1;
    grid_negative_at2 = 5 * 7 + 0;
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePieceWithOrder(4, 0, 0) == 0x102);
    assert(probe_count == 2 && grid_count == 1);
    assert(probe_args[1][0] == 4 && probe_args[1][1] == 1);
    assert(grid_calls[0][0] == 2 && grid_calls[0][1] == 1);
    /* idx 1 skips the first free cell and the occupied one */
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePieceWithOrder(4, 1, 0) == 0x005);
    assert(probe_count == 4 && grid_count == 3);
    /* only the low byte of flag counts: 0x100 behaves as clear */
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePieceWithOrder(4, 1, 0x100) == 0x005);
    assert(probe_count == 4 && grid_count == 3);
    /* idx past the accepted count: -1 after all eight */
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePieceWithOrder(4, 2, 0) == -1);
    assert(probe_count == 8 && grid_count == 3);
    /* flag set: kind 8 is rejected before the cell test and is not counted */
    probe_count = grid_count = 0;
    kind_args[0] = kind_args[1] = -1;
    assert(Ta_GetSummonablePieceWithOrder(4, 0, 1) == 0x005);
    assert(probe_count == 4 && grid_count == 2);
    assert(grid_calls[0][0] == 4 && grid_calls[0][1] == 3);
    assert(grid_calls[1][0] == 5 && grid_calls[1][1] == 0);
    assert(kind_args[0] == 5 && kind_args[1] == 0);
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePieceWithOrder(4, 1, 1) == -1);
    assert(probe_count == 8 && grid_count == 2);
    /* every probe negative: no cell looked at */
    for (i = 0; i < 8; i++) probe_results[i] = -1;
    probe_count = grid_count = 0;
    assert(Ta_GetSummonablePieceWithOrder(9, 0, 1) == -1);
    assert(probe_count == 8 && grid_count == 0);
    grid_count = 0;
    kind_result = saved_kind;
    kind_cell = saved_cell;
    grid_negative_at = saved_at;
    grid_negative_at2 = saved_at2;
    grid_negative_value = saved_value;
}

static void check_cell_pick(void)
{
    int saved_kind = kind_result, saved_cell = kind_cell;
    int saved_at = grid_negative_at, saved_at2 = grid_negative_at2, saved_value = grid_negative_value;
    int saved_lo[8];

    memcpy(saved_lo, thunk_by_lo, sizeof(saved_lo));
    grid_negative_at = grid_negative_at2 = -1;
    /* first scan starts at odd y: (1,1) is not kind 8 */
    kind_result = 0;
    kind_cell = -1;
    grid_count = 0;
    assert(TaLogic008_GetWallEmpty(3) == 0x101);
    assert(kind_args[0] == 1 && kind_args[1] == 1 && grid_count == 0);
    /* (1,1) kind 8: the next x on the same row */
    kind_cell = 1 * 7 + 1;
    assert(TaLogic008_GetWallEmpty(3) == 0x102);
    assert(kind_args[0] == 2 && kind_args[1] == 1);
    /* every cell kind 8: the first scan ends at (5,5); all 49 cells owned by side: -1 */
    kind_cell = -1;
    kind_result = 8;
    thunk_by_lo[0] = thunk_by_lo[5] = 3;
    assert(TaLogic008_GetWallEmpty(3) == -1);
    assert(grid_count == 49 && grid_calls[0][0] == 0 && grid_calls[0][1] == 0);
    assert(grid_calls[48][0] == 6 && grid_calls[48][1] == 6);
    assert(kind_args[0] == 5 && kind_args[1] == 5);
    /* another owner at (0,0) but (6,0) is kind 8: -1 */
    thunk_by_lo[0] = 4;
    grid_count = 0;
    assert(TaLogic008_GetWallEmpty(3) == -1);
    assert(grid_count == 49 && kind_args[0] == 6 && kind_args[1] == 0);
    /* (6,0) open, (5,0) kind 8: -1 */
    kind_open_y = 0;
    kind_cell = 5 * 7 + 0;
    grid_count = 0;
    assert(TaLogic008_GetWallEmpty(3) == -1);
    assert(grid_count == 49 && kind_args[0] == 5 && kind_args[1] == 0);
    /* both open: 6 at the first cell; the owner is asked with (hi, lo) of the cell word */
    kind_cell = -1;
    grid_count = 0;
    thunk_args[0] = thunk_args[1] = -1;
    assert(TaLogic008_GetWallEmpty(3) == 6);
    assert(grid_count == 1 && thunk_args[0] == 0 && thunk_args[1] == 0);
    /* a negative cell is skipped */
    grid_negative_at = 0;
    grid_negative_value = -1;
    grid_count = 0;
    assert(TaLogic008_GetWallEmpty(3) == 6);
    assert(grid_count == 2);
    /* column bound: another owner at second index 3 does not count, at 2 it does */
    thunk_by_lo[0] = 3;
    thunk_by_lo[3] = 4;
    grid_negative_value = 0x203;
    grid_negative_at = 2 * 7 + 3;
    grid_count = 0;
    assert(TaLogic008_GetWallEmpty(3) == -1);
    assert(grid_count == 49);
    grid_negative_at = 2 * 7 + 2;
    grid_count = 0;
    assert(TaLogic008_GetWallEmpty(3) == 6);
    assert(grid_count == 2 * 7 + 3 && thunk_args[0] == 2 && thunk_args[1] == 3);
    grid_count = 0;
    kind_open_y = -1;
    kind_result = saved_kind;
    kind_cell = saved_cell;
    grid_negative_at = saved_at;
    grid_negative_at2 = saved_at2;
    grid_negative_value = saved_value;
    memcpy(thunk_by_lo, saved_lo, sizeof(saved_lo));
}

static void check_word_class(void)
{
    /* {low byte, upper part, expected}: every range test at and beside its bounds */
    static const int cases[][3] = {
        {2, 0, 0}, {5, 0, 0}, {1, 0, -1}, {6, 0, 1}, {3, 1, -1},
        {6, 3, 1}, {6, 4, 2}, {6, -1, -1},
        {5, 4, 2}, {4, 4, 3}, {7, 4, -1},
        {4, 5, 3}, {4, 6, 4}, {4, 3, -1},
        {1, 6, 4}, {0, 6, 5}, {5, 6, -1},
        {0, 3, 5}, {0, 2, 6}, {0, 7, -1},
        {1, 2, 6}, {2, 2, 7}, {2, 1, 7}, {2, 3, -1}, {3, 2, -1},
    };
    unsigned i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        grid_word = cases[i][1] * 256 + cases[i][0];
        grid_word_calls = 0;
        grid_args[0] = grid_args[1] = -1;
        assert(TaLogic005_GetUnitOnCycleRoot(3, 4) == cases[i][2]);
        assert(grid_word_calls == 2 && grid_args[0] == 3 && grid_args[1] == 4);
    }
    grid_word = 0x12345;
}

static void check_position_collect(void)
{
    int out[4];
    int saved_gate = gate_result, saved_kind = kind_result, saved_cell = kind_cell;

    kind_cell = -1;
    /* 0x24fda0 not one: nothing collected, output untouched */
    memset(out, 0x55, sizeof(out));
    gate_result = 2;
    kind_result = 9;
    notify_calls = 0;
    assert(Ta_IsVirusAll(out) == 0 && out[0] == 0x55555555 && out[3] == 0x55555555 && notify_calls == 0);
    assert(word_a == 1 && word_b == 10 && grid_args[0] == 1 && grid_args[1] == 10);
    /* kind other than nine: nothing collected */
    gate_result = 1;
    kind_result = 8;
    assert(Ta_IsVirusAll(out) == 0 && out[2] == 0x55555555 && notify_calls == 0);
    assert(kind_args[0] == 0x45 && kind_args[1] == 0x123);
    /* both tests pass for all twenty positions */
    kind_result = 9;
    assert(Ta_IsVirusAll(out) == 1 && notify_calls == 20);
    assert(out[0] == 0x70 && out[1] == 0 && out[2] == 0x1ffbff && out[3] == 0);
    assert(notify_args[0] == 1 && notify_args[1] == 10 && notify_args[2] == 1);
    gate_result = saved_gate;
    kind_result = saved_kind;
    kind_cell = saved_cell;
}

static int veto_a, veto_b, veto_calls[2], veto_args[2];
int SzDuel_GetUnitCurseRest(int a, int b) { veto_calls[0]++; veto_args[0] = a; veto_args[1] = b; return veto_a; }
int SzDuel_GetUnitSpellRest(int a, int b) { veto_calls[1]++; assert(a == veto_args[0] && b == veto_args[1]); return veto_b; }

static void check_best_entry_veto(void)
{
    /* Branch coverage for 0x27b860: count bound, bit acceptance, key acceptance with each veto,
     * score outcomes. */
    static const int scores[8] = {0, 10, 30, 30, 20, 0, 0, 0};
    static const unsigned char accept[8] = {0, 1, 1, 1, 1, 0, 0, 0};
    static const unsigned char none[8];
    unsigned short list[22];
    score_by_hi = scores;
    bit_by_lo = none;
    thunk_by_lo[2] = 7;
    veto_a = veto_b = 0;
    veto_calls[0] = veto_calls[1] = 0;

    score_calls = 0;
    fill_list(list, 1, 0, 0, 0);
    assert(TaLogic_GetMaxApUnit3(list, 7) == -1 && score_calls == 0);
    fill_list(list, 3, 0x0201, 0x0102, 0);
    assert(TaLogic_GetMaxApUnit3(list, 7) == 0x0102 && thunk_args[0] == 1 && thunk_args[1] == 2);
    assert(veto_calls[0] == 1 && veto_calls[1] == 1 && veto_args[0] == 1 && veto_args[1] == 2 && score_calls == 1);
    assert(score_args[0] == 1 && score_args[1] == 2 && score_args[2] == 0x45 && score_args[3] == 0x123);
    /* key mismatch: neither veto is asked */
    assert(TaLogic_GetMaxApUnit3(list, 8) == -1 && veto_calls[0] == 1 && veto_calls[1] == 1);
    /* first veto set: second not asked */
    veto_a = 1;
    assert(TaLogic_GetMaxApUnit3(list, 7) == -1 && veto_calls[0] == 2 && veto_calls[1] == 1);
    veto_a = 0;
    veto_b = 1;
    assert(TaLogic_GetMaxApUnit3(list, 7) == -1 && veto_calls[0] == 3 && veto_calls[1] == 2 && score_calls == 1);
    /* bit acceptance skips the key and the vetoes; greater score wins, a tie keeps the first */
    bit_by_lo = accept;
    fill_list(list, 4, 0x0101, 0x0202, 0x0303);
    assert(TaLogic_GetMaxApUnit3(list, 99) == 0x0202 && veto_calls[0] == 3);
    fill_list(list, 4, 0x0101, 0x0204, 0x0105);
    assert(TaLogic_GetMaxApUnit3(list, 99) == 0x0204);
    bit_by_lo = NULL;
    veto_b = 0;
}

static int slot_update(int id, int key, struct DotrMiscSlotRequest *req)
{
    req->id = (unsigned char)id;
    req->unknown_01 = 0x11;
    req->key = (unsigned char)key;
    req->unknown_03 = 0x33;
    req->unknown_04 = 0x4444;
    req->unknown_06 = 0x6666;
    return ShFace_Request(req);
}

static void slot_expect(int index, int key, int cleared)
{
    struct DotrMiscSlot52 *slot = &gSh_Face_slots[index];
    int i;

    assert(slot->active == 1 && slot->unknown_04 == 0x11 && slot->key == key && slot->unknown_06 == 0x33);
    assert(slot->unknown_2c == 0x4444 && slot->unknown_2e == 0x6666 && slot->unknown_30 == 0x5a5a5a5a);
    assert(slot->unknown_07[0] == 0x5a && slot->unknown_07[3] == 0x5a && slot->unknown_2b == 0x5a);
    for (i = 0; i < 16; i++) {
        assert(slot->unknown_0b[i] == (cleared ? 0 : 0x5a) && slot->unknown_1b[i] == (cleared ? 0 : 0x5a));
    }
}

static void check_slot_update(void)
{
    /* Branch coverage for 0x245be0. */
    struct DotrMiscEntry52 saved[9];
    struct DotrMiscSlotRequest req;
    int i;

    assert(sizeof(struct DotrMiscSlot52) == sizeof(struct DotrMiscEntry52));
    memcpy(saved, gSh_Face, sizeof(saved));
    memset(gSh_Face, 0x5a, sizeof(saved));
    /* 0xff with all eight active: refused, request unchanged */
    assert(slot_update(0xff, 7, &req) == 0 && req.id == 0xff && gSh_Face_slots[0].key == 0x5a);
    /* 0xff takes the first inactive entry (3, then the last one, 7); a new entry's key is 0xff first */
    gSh_Face_slots[3].active = 0;
    gSh_Face_slots[7].active = 0;
    assert(slot_update(0xff, 7, &req) == 1 && req.id == 3);
    slot_expect(3, 7, 1);
    assert(slot_update(0xff, 0xff, &req) == 1 && req.id == 7);
    slot_expect(7, 0xff, 0);
    assert(gSh_Face_slots[8].active == 0x5a5a5a5a && gSh_Face_slots[8].key == 0x5a);
    gSh_Face_slots[0].active = 0;
    assert(slot_update(0xff, 0x5a, &req) == 1 && req.id == 0);
    slot_expect(0, 0x5a, 1);
    /* explicit inactive entry: refused, nothing written */
    gSh_Face_slots[5].active = 0;
    assert(slot_update(5, 7, &req) == 0 && req.id == 5 && gSh_Face_slots[5].key == 0x5a);
    assert(gSh_Face_slots[5].unknown_04 == 0x5a && gSh_Face_slots[5].unknown_2c == 0x5a5a);
    /* explicit active entry: same key keeps the two byte arrays, a new key clears them */
    gSh_Face_slots[2].active = 1;
    assert(slot_update(2, 0x5a, &req) == 1);
    slot_expect(2, 0x5a, 0);
    assert(slot_update(2, 9, &req) == 1 && req.id == 2);
    slot_expect(2, 9, 1);
    for (i = 0; i < 9; i++) gSh_Face[i] = saved[i];
}

static void check_slot_claim(void)
{
    DotrMiscSlot16 table[23];
    int i;

    assert(sizeof(DotrMiscSlot16) == 16);
    memset(table, 0x55, sizeof(table));
    scores[1][3] = 0x1234;
    /* Slot 21 (the last one scanned) is the only free slot. */
    table[21].key = 0xffff;
    table[22].key = 0xffff;
    assert(TaLogic_DoUnitAssign(0x0103, table) == 1);
    assert(table[21].key == 0x0103 && table[21].value == 0x1234 && table[21].ref == -1);
    assert(table[21].link == 0xffff && table[21].mode == 0 && table[21].rest == 0x15);
    assert(table[22].key == 0xffff && table[20].key == 0x5555);
    /* No free slot within 22 entries: slot 22 is not examined. */
    assert(TaLogic_DoUnitAssign(0x0103, table) == 0 && table[22].value == 0x5555);
    /* The first of two free slots wins. */
    table[0].key = 0xffff;
    table[5].key = 0xffff;
    scores[0][10] = 7;
    assert(TaLogic_DoUnitAssign(0x000a, table) == 1 && table[0].key == 0x000a && table[0].value == 7);
    assert(table[5].key == 0xffff && table[5].ref == 0x55555555);
    for (i = 1; i < 5; i++) assert(table[i].key == 0x5555);
    scores[1][3] = scores[0][10] = 0;
}

/* 0x2500c0: every state_grid callee is logged as {address, a, b, c, d}. */
static int level_log[8][5], level_calls, level_cb0, level_d10;
static int level_note(int address, int a, int b, int c, int d)
{
    assert(level_calls < 8);
    level_log[level_calls][0] = address;
    level_log[level_calls][1] = a;
    level_log[level_calls][2] = b;
    level_log[level_calls][3] = c;
    level_log[level_calls][4] = d;
    level_calls++;
    return 0;
}
int SzEffect_SetBusyFlag2(int packed, int flag) { return level_note(0x1f5970, packed, flag, 0, 0); }
int SzDuel_GetUnitCtlRateC(int a, int b) { (void)a; (void)b; return level_cb0; }
int SzDuel_GetUnitCtlRateM(int a, int b) { (void)a; (void)b; return level_d10; }
int SzDuel_SetUnitCtlModeM(int a, int b, int value) { return level_note(0x1e18a0, a, b, value, 0); }
int SzDuel_SetUnitCtlGoalM(int a, int b, int value) { return level_note(0x1e19d0, a, b, value, 0); }
int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value) { return level_note(0x1e1c10, a, b, pair, value); }
int SzDuel_SetUnitCtlModeC(int a, int b, int value) { return level_note(0x1e1840, a, b, value, 0); }
int SzDuel_SetUnitCtlGoalC(int a, int b, int value) { return level_note(0x1e1970, a, b, value, 0); }
int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value) { return level_note(0x1e1b70, a, b, pair, value); }

static int level_is(int n, int address, int a, int b, int c, int d)
{
    return level_log[n][0] == address && level_log[n][1] == a && level_log[n][2] == b &&
           level_log[n][3] == c && level_log[n][4] == d;
}

static int level_case(int pos, int dir, int flags, int d10, int cb0)
{
    level_calls = 0;
    level_d10 = d10;
    level_cb0 = cb0;
    Ta_UnitFade(1, pos, dir, 0x55, flags);
    assert(level_is(0, 0x1f5970, 0x100 | pos, 0, 0, 0));
    return level_calls;
}

static void check_level_moves(void)
{
    shifted_kind = 0;
    bit_by_lo = 0;
    bit_only4 = 0;
    bit_result = 0;
    /* Up, both flags, both levels at 127: bit 4 clear and pos non-zero gives pair 0. */
    assert(level_case(2, 1, 3, 127, 127) == 7);
    assert(level_is(1, 0x1e18a0, 1, 2, 0x55, 0) && level_is(2, 0x1e19d0, 1, 2, 128, 0));
    assert(level_is(3, 0x1e1c10, 1, 2, 0, -1) && bit_args[0] == 1 && bit_args[1] == 2 && bit_args[2] == 4);
    assert(level_is(4, 0x1e1840, 1, 2, 0x55, 0) && level_is(5, 0x1e1970, 1, 2, 128, 0));
    assert(level_is(6, 0x1e1b70, 1, 2, 1, -1));
    /* Bit 4 set, or pos zero, gives pair 1. */
    bit_result = 1;
    assert(level_case(2, 1, 1, 0, 0) == 4 && level_is(3, 0x1e1c10, 1, 2, 1, -1));
    bit_result = 0;
    assert(level_case(0, 1, 1, 0, 0) == 4 && level_is(3, 0x1e1c10, 1, 0, 1, -1));
    /* Up bounds: a level at 128 does not move; each flag selects one level. */
    assert(level_case(2, 1, 3, 128, 128) == 1);
    assert(level_case(2, 1, 3, 128, 127) == 4 && level_is(1, 0x1e1840, 1, 2, 0x55, 0));
    assert(level_case(2, 1, 2, 0, 0) == 4 && level_is(3, 0x1e1b70, 1, 2, 1, -1));
    assert(level_case(2, 1, 0, 0, 0) == 1);
    /* Down (dir 0 and negative): levels above 0 go to 0 with pair -1. */
    assert(level_case(2, 0, 3, 1, 1) == 7);
    assert(level_is(1, 0x1e18a0, 1, 2, 0x55, 0) && level_is(2, 0x1e19d0, 1, 2, 0, 0));
    assert(level_is(3, 0x1e1c10, 1, 2, -1, -1) && level_is(4, 0x1e1840, 1, 2, 0x55, 0));
    assert(level_is(5, 0x1e1970, 1, 2, 0, 0) && level_is(6, 0x1e1b70, 1, 2, -1, -1));
    assert(level_case(2, -1, 3, 0, 0) == 1);
    assert(level_case(2, -1, 3, 0, 1) == 4 && level_is(3, 0x1e1b70, 1, 2, -1, -1));
    assert(level_case(2, -1, 1, 1, 1) == 4 && level_is(3, 0x1e1c10, 1, 2, -1, -1));
    assert(level_case(2, -1, 2, 1, 1) == 4 && level_is(1, 0x1e1840, 1, 2, 0x55, 0));
    /* A non-zero kind at a non-zero pos keeps only flag bit 1; pos 0 skips the kind query. */
    shifted_kind = 32;
    assert(level_case(2, 1, 3, 0, 0) == 4 && level_is(1, 0x1e1840, 1, 2, 0x55, 0));
    assert(level_case(2, 1, 1, 0, 0) == 1);
    assert(level_case(0, -1, 3, 1, 1) == 7);
    shifted_kind = 31;
    assert(level_case(2, 1, 1, 0, 0) == 4);
    shifted_kind = 0;
}

struct DotrMiscControlLists data_00317490_lists;
static int own_calls, own_side, own_kind_bad;
static int own_kind_last;
static unsigned char own_by[854];

int SzData_GetBagCount(int side, int id, int kind)
{
    assert(id == own_calls % 854 && id < 854);
    own_calls++;
    own_side = side;
    own_kind_bad |= kind != 3;
    own_kind_last = kind;
    return own_by[id] == 2 ? -1 : own_by[id];
}

/* lSzDuel_SetupTrade (branch and loop coverage). */
static void run_list(int flag, int mode, int top, int rows)
{
    data_00317490_lists.modes[0] = data_00317490_lists.modes[1] = 9;
    data_00317490_lists.modes[flag] = mode;
    data_00317490_lists.pairs[flag].top = top;
    data_00317490_lists.pairs[flag].count = 0x7777;
    data_00317490_lists.pairs[!flag].top = 0x1111;
    data_00317490_lists.pairs[!flag].count = 0x2222;
    data_00317490_lists.rows[flag] = rows;
    data_00317490_lists.rows[!flag] = 0;
    own_calls = own_kind_bad = 0;
    own_side = -1;
    t_id = 0;
    lSzDuel_SetupTrade(flag);
    assert(data_00317490_lists.pairs[!flag].top == 0x1111 && data_00317490_lists.pairs[!flag].count == 0x2222);
    assert(t_id == 9 && t_args[1] == 854 && t_callback == (flag ? lSzDuel_SortFunc2 : lSzDuel_SortFunc1));
    assert(data_00317490_tail.unknown_d5c == flag);
}

static void check_list_rebuild(void)
{
    static short lists[2][855];
    static unsigned char counts[2][855];
    int flag, i;

    memset(&data_00317490_lists, 0, sizeof(data_00317490_lists));
    for (flag = 0; flag < 2; flag++) {
        struct DotrMiscControlPair *pair = &data_00317490_lists.pairs[flag];

        data_00317490_lists.lists[0] = lists[0];
        data_00317490_lists.lists[1] = lists[1];
        data_00317490_lists.counts[0] = counts[0];
        data_00317490_lists.counts[1] = counts[1];
        memset(lists, 0x55, sizeof(lists));
        memset(counts, 0, sizeof(counts));
        memset(own_by, 0, sizeof(own_by));
        /* own side: entries 0, 7, 853; other side: 1, 2; 0x216ac0: 3..12 (and a negative answer at 20) */
        counts[flag][0] = 1, counts[flag][7] = 255, counts[flag][853] = 3, counts[flag][854] = 1;
        counts[!flag][1] = 1, counts[!flag][2] = 1;
        for (i = 3; i < 13; i++) {
            own_by[i] = 1;
        }
        own_by[20] = 2;
        /* mode 1: length 3 is below 10 + 1, so the first row drops to 0 (3 - 5 is not above 0) */
        run_list(flag, 1, 10, 1);
        assert(own_calls == 0 && pair->count == 3 && pair->top == 0);
        assert(lists[flag][0] == 0 && lists[flag][1] == 999 && lists[flag][7] == 7 && lists[flag][852] == 999);
        assert(lists[flag][853] == 853 && lists[flag][854] == 0x5555 && lists[!flag][0] == 0x5555);
        /* mode 2 reads the other side's bytes; length 2 is not below 1 + 1: first row kept */
        run_list(flag, 2, 1, 1);
        assert(own_calls == 0 && pair->count == 2 && pair->top == 1);
        assert(lists[flag][0] == 999 && lists[flag][1] == 1 && lists[flag][2] == 2 && lists[flag][853] == 999);
        /* one below the bound moves it */
        run_list(flag, 2, 1, 2);
        assert(pair->count == 2 && pair->top == 0);
        /* mode 0 asks 0x216ac0 for all 854 ids with kind 3; length 10 gives first row 5 */
        run_list(flag, 0, 9, 2);
        assert(own_calls == 854 && own_side == flag && !own_kind_bad && pair->count == 10 && pair->top == 5);
        assert(lists[flag][2] == 999 && lists[flag][3] == 3 && lists[flag][12] == 12 && lists[flag][13] == 999);
        assert(lists[flag][20] == 999 && lists[flag][854] == 0x5555);
        /* length 6 gives 1 */
        for (i = 9; i < 13; i++) {
            own_by[i] = 0;
        }
        run_list(flag, 0, 6, 1);
        assert(pair->count == 6 && pair->top == 1);
        /* another mode: the list is left, the length is 0 */
        lists[flag][0] = 0x1234;
        run_list(flag, 3, 4, 0);
        assert(own_calls == 0 && pair->count == 0 && pair->top == 0 && lists[flag][0] == 0x1234);
        run_list(flag, 3, 0, 0);
        assert(pair->count == 0 && pair->top == 0);
    }
}

unsigned char gSzDuel_MapData[46][7][7];

/* SzDuel_SetupMap (branch and loop coverage). */
static void run_grid(int index, const unsigned char *grid)
{
    int i;

    set_count = 0;
    SzDuel_SetupMap(index);
    if (grid == NULL) {
        assert(set_count == 0);
        return;
    }
    assert(set_count == 49);
    for (i = 0; i < 49; i++) {
        assert(set_calls[i][0] == i % 7 && set_calls[i][1] == i / 7 && set_calls[i][2] == grid[i]);
    }
}

static void check_grid_load(void)
{
    struct DotrMiscLargeGrids *first = (struct DotrMiscLargeGrids *)&gSzData_All[0];
    struct DotrMiscLargeGrids *second = (struct DotrMiscLargeGrids *)&gSzData_All[1];
    int i, j;

    assert(offsetof(struct DotrMiscLargeGrids, grids) == 0xfea3 && sizeof(*first) == sizeof(gSzData_All[0]));
    for (i = 0; i < 46; i++) {
        for (j = 0; j < 49; j++) {
            gSzDuel_MapData[i][j / 7][j % 7] = (unsigned char)(i * 5 + j);
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 49; j++) {
            first->grids[i][j / 7][j % 7] = (unsigned char)(200 - i * 3 - j);
            second->grids[i][j / 7][j % 7] = (unsigned char)(90 + i * 7 + j * 2);
        }
    }
    /* SzData_GetData(0) answers the first entry and (1) the second */
    gSzData_bRoseFlag[0] = 0;
    run_grid(0, &gSzDuel_MapData[0][0][0]);
    run_grid(45, &gSzDuel_MapData[45][0][0]);
    run_grid(46, &first->grids[0][0][0]);
    run_grid(50, &first->grids[4][0][0]);
    run_grid(51, &second->grids[0][0][0]);
    run_grid(55, &second->grids[4][0][0]);
    run_grid(56, NULL);
    run_grid(1000, NULL);
    /* with the selector at 1 the two entries swap */
    gSzData_bRoseFlag[0] = 1;
    run_grid(46, &second->grids[0][0][0]);
    run_grid(55, &first->grids[4][0][0]);
    run_grid(45, &gSzDuel_MapData[45][0][0]);
    gSzData_bRoseFlag[0] = 0;
}

/* 0x272210's two outside callees: answers per position, calls counted. */
static int strip_e40[2][11], strip_700[2][11], strip_e40_calls, strip_700_calls;
int Ta_IsLDRangeAvility(int a, int b, int c)
{
    assert(a >= 0 && a < 2 && b >= 0 && b < 11 && c == 11);
    strip_e40_calls++;
    return strip_e40[a][b];
}
int Ta_IsResistAll(int a, int b)
{
    assert(a >= 0 && a < 2 && b >= 0 && b < 11);
    strip_700_calls++;
    return strip_700[a][b];
}
static int run_strip(int word, int side)
{
    strip_e40_calls = strip_700_calls = 0;
    return Ta_RemoveObjFromFlagLst(word, side);
}

static void check_side_strip(void)
{
    /* set positions, in order: (0, 0), (0, 3), (1, 1) */
    const int word = (1 << 21) | (1 << 18) | (1 << 9);
    memset(pair_sides, 0, sizeof(pair_sides));
    memset(strip_e40, 0, sizeof(strip_e40));
    memset(strip_700, 0, sizeof(strip_700));
    /* no set position: no callee past the count */
    assert(run_strip(0, 1) == 0 && strip_e40_calls == 0 && strip_700_calls == 0);
    /* neither test answers: unchanged, both asked for each position */
    assert(run_strip(word, 1) == word && strip_e40_calls == 3 && strip_700_calls == 3);
    /* 0x271e40 answers and the side differs: rebuilt without asking 0x267700 */
    strip_e40[0][3] = 1;
    assert(run_strip(word, 1) == (int)ConvUnitIDtoFlagList_OFF(word, 0, 3) && strip_e40_calls == 3 && strip_700_calls == 2);
    /* 0x271e40 answers but the side is equal: 0x267700 is asked; unchanged whether it answers or not */
    pair_sides[0][3] = 1;
    assert(run_strip(word, 1) == word && strip_700_calls == 3);
    strip_700[0][3] = 1;
    assert(run_strip(word, 1) == word && strip_700_calls == 3);
    /* 0x267700 alone answers and the side differs */
    strip_700[1][1] = 1;
    assert(run_strip(word, 1) == (int)ConvUnitIDtoFlagList_OFF(word, 1, 1) && strip_e40_calls == 3 && strip_700_calls == 3);
    /* two positions rebuilt in turn, the later one on the earlier result */
    assert(run_strip(word, 2) == (int)ConvUnitIDtoFlagList_OFF(ConvUnitIDtoFlagList_OFF(word, 0, 3), 1, 1) && strip_700_calls == 2);
    assert(ConvUnitIDtoFlagList_OFF(word, 0, 3) != (unsigned)word && ConvUnitIDtoFlagList_OFF(word, 1, 1) != (unsigned)word);
    memset(pair_sides, 0, sizeof(pair_sides));
}

static int cue_calls[4][4], cue_count, tint_args[2][7], tint_count, text_calls;
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value)
{
    cue_calls[cue_count][0] = operation;
    cue_calls[cue_count][1] = a;
    cue_calls[cue_count][2] = b;
    cue_calls[cue_count][3] = value;
    cue_count++;
}
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int kind)
{
    int args[7];
    args[0] = x; args[1] = y; args[2] = c; args[3] = reach; args[4] = e; args[5] = mode; args[6] = kind;
    memcpy(tint_args[tint_count++], args, sizeof(args));
}
int SzDuel_GetMsgName(int index)
{
    assert(index == 83);
    text_calls++;
    return 0x777;
}
static int run_strip_cue(int word, int side)
{
    strip_e40_calls = strip_700_calls = cue_count = tint_count = text_calls = grid_word_calls = 0;
    return Ta_RemoveObjFromFlagLst2(word, side);
}

static void check_side_strip_cue(void)
{
    /* set positions, in order: (0, 0), (0, 3), (1, 1) */
    const int word = (1 << 21) | (1 << 18) | (1 << 9);
    memset(pair_sides, 0, sizeof(pair_sides));
    memset(strip_e40, 0, sizeof(strip_e40));
    memset(strip_700, 0, sizeof(strip_700));
    grid_word = 0x0305;
    /* no set position: no callee past the count */
    assert(run_strip_cue(0, 1) == 0 && strip_e40_calls == 0 && grid_word_calls == 0 && cue_count == 0);
    /* neither test answers: unchanged, both asked and the side looked up for each position, no effect */
    assert(run_strip_cue(word, 1) == word && strip_e40_calls == 3 && strip_700_calls == 3 && grid_word_calls == 3);
    assert(grid_args[0] == 0 && grid_args[1] == 0 && cue_count == 0 && tint_count == 0 && text_calls == 0);
    /* 0x271e40 answers and the side differs: cue, tint with the looked-up bytes, rebuilt; 0x267700
     * not asked */
    strip_e40[0][3] = 1;
    assert(run_strip_cue(word, 1) == (int)ConvUnitIDtoFlagList_OFF(word, 0, 3) && strip_700_calls == 2);
    assert(cue_count == 1 && cue_calls[0][0] == 0 && cue_calls[0][1] == -120 && cue_calls[0][2] == 80 && cue_calls[0][3] == 270);
    assert(tint_count == 1 && tint_args[0][0] == 5 && tint_args[0][1] == 3 && tint_args[0][2] == 0);
    assert(tint_args[0][3] == Ta_IsLDAbl(9, 0) && tint_args[0][4] == 1 && tint_args[0][5] == 1 && tint_args[0][6] == 11);
    assert(text_calls == 0);
    /* 0x271e40 answers but the side is equal: 0x267700 is asked; unchanged whether it answers or not */
    pair_sides[0][3] = 1;
    assert(run_strip_cue(word, 1) == word && strip_700_calls == 3 && cue_count == 0);
    strip_700[0][3] = 1;
    assert(run_strip_cue(word, 1) == word && strip_700_calls == 3 && cue_count == 0 && tint_count == 0);
    /* 0x267700 alone answers and the side differs: cue with the SzDuel_GetMsgName(83) value, rebuilt */
    strip_700[1][1] = 1;
    assert(run_strip_cue(word, 1) == (int)ConvUnitIDtoFlagList_OFF(word, 1, 1) && strip_e40_calls == 3 && strip_700_calls == 3);
    assert(cue_count == 1 && cue_calls[0][0] == 0 && cue_calls[0][1] == 0 && cue_calls[0][2] == 80 && cue_calls[0][3] == 0x777);
    assert(text_calls == 1 && tint_count == 0);
    /* two positions rebuilt in turn, the later one on the earlier result */
    assert(run_strip_cue(word, 2) == (int)ConvUnitIDtoFlagList_OFF(ConvUnitIDtoFlagList_OFF(word, 0, 3), 1, 1) && strip_700_calls == 2);
    assert(cue_count == 2 && tint_count == 1 && text_calls == 1 && tint_args[0][3] == Ta_IsLDAbl(9, 1));
    memset(pair_sides, 0, sizeof(pair_sides));
    grid_word = 0x12345;
}

static void check_random_constant(void)
{
    static const int expected[6] = { 35, 79, 44, 92, 379, 394 };
    int i;

    for (i = 0; i < 6; i++) {
        random_value = i;
        random_calls = 0;
        assert(TaEnc_GetRandomElf() == expected[i] && random_calls == 1);
    }
    /* Boundary: the remainder wraps at 6; a negative remainder takes the common return. */
    random_value = 6;
    assert(TaEnc_GetRandomElf() == 35);
    random_value = 11;
    assert(TaEnc_GetRandomElf() == 394);
    random_value = -1;
    assert(TaEnc_GetRandomElf() == 394);
}

static void check_chain_cells(void)
{
    static struct DotrMiscChain cells[2];

    cells[0].loc = 0x34;
    cells[0].parent = 0;
    /* no depth, no object */
    assert(TaLogic_IsRootPast2(&cells[0], 4, 3, 0, 0, 0) == 0);
    assert(TaLogic_IsRootPast2(0, 4, 3, 0, 0, 1) == 0);
    /* the object is on (a, b); column or row alone is not enough */
    assert(TaLogic_IsRootPast2(&cells[0], 4, 3, 0, 0, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 4, 0, 0, 0, 1) == 0);
    assert(TaLogic_IsRootPast2(&cells[0], 0, 3, 9, 9, 1) == 0);
    /* same column, two rows apart: the cell between */
    assert(TaLogic_IsRootPast2(&cells[0], 4, 2, 4, 4, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 4, 4, 4, 2, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 5, 2, 5, 4, 1) == 0);
    assert(TaLogic_IsRootPast2(&cells[0], 4, 4, 4, 6, 1) == 0);
    /* diagonal step: column of (c, d), or row of (c, d) */
    assert(TaLogic_IsRootPast2(&cells[0], 5, 0, 4, 1, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 1, 2, 0, 3, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 1, 0, 0, 1, 1) == 0);
    /* same row, two columns apart */
    assert(TaLogic_IsRootPast2(&cells[0], 3, 3, 5, 3, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 5, 3, 3, 3, 1) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 3, 2, 5, 2, 1) == 0);
    assert(TaLogic_IsRootPast2(&cells[0], 1, 3, 3, 3, 1) == 0);
    /* three steps apart: only the exact cell counts */
    assert(TaLogic_IsRootPast2(&cells[0], 4, 2, 4, 5, 1) == 0);
    /* the next object is reached only while depth lasts */
    cells[0].loc = 0;
    cells[0].parent = &cells[1];
    cells[1].loc = 0x34;
    cells[1].parent = 0;
    assert(TaLogic_IsRootPast2(&cells[0], 4, 3, 9, 9, 2) == 1);
    assert(TaLogic_IsRootPast2(&cells[0], 4, 3, 9, 9, 1) == 0);
    assert(TaLogic_IsRootPast2(&cells[0], 4, 2, 4, 4, 2) == 1);
}

static int rule_args[3], rule_calls;
int func_002464f0_rule(int item, int x, int y)
{
    rule_args[0] = item;
    rule_args[1] = x;
    rule_args[2] = y;
    rule_calls++;
    return 0;
}

/* 0x27b520 over the real forwarder 0x279850; 0x1e1090, 0x1df540, 0x2464f0 and 0x2798c0 are stubs. */
static void check_cell_pair_test(void)
{
    struct DotrMiscCellPair pair;
    int mode;

    pair.unknown_00 = 0x0203;
    pair.unknown_08.word = 0x06070405;
    word_mask = ~0u;
    grid_pair_on = 1;
    grid_pair[0] = 0x0a0b;
    grid_pair[1] = 0x0c0d;
    for (mode = 0; mode < 3; mode++) {
        /* mode 1 takes the upper halfword of +8, any other mode the lower */
        int want = mode == 1 ? 0x0607 : 0x0405;

        grid_pair_calls = 0;
        rule_calls = 0;
        wide_result = 1;
        assert(TaLogic_IsAtkNowToGoal(&pair, mode) == 1);
        assert(grid_pair_calls == 2 && grid_pair_args[0][0] == 2 && grid_pair_args[0][1] == 3);
        assert(grid_pair_args[1][0] == want >> 8 && grid_pair_args[1][1] == (want & 0xff));
        assert(rule_calls == 1 && rule_args[0] == 0x5540 && rule_args[1] == 0x0b && rule_args[2] == 0x0a);
        assert(word_a == 2 && word_b == 3 && wide_args[0] == 0x5540 && wide_args[1] == 0x0b && wide_args[2] == 0x0a);
        assert(wide_args[3] == 0x0d && wide_args[4] == 0x0c && wide_args[5] == want);
    }
    /* any result other than 1 gives 0 */
    wide_result = 0;
    assert(TaLogic_IsAtkNowToGoal(&pair, 1) == 0);
    wide_result = 2;
    assert(TaLogic_IsAtkNowToGoal(&pair, 1) == 0);
    wide_result = 0x7001;
    grid_pair_on = 0;
}

/* TkMc_Load_Main and TkMc_Trade_Load_Main (branch coverage): stubs for the step callees. */
static int step_result, step_reset_calls, step_calls[4], step_arg;
int TakuCard_inti(int a) { assert(a == 0); step_reset_calls++; return 0; }
int TakuLoad_card_kansi(int a) { step_arg = a; step_calls[0]++; return step_result; }
int TakuLoad_card_filename_check(void) { step_calls[1]++; return step_result; }
int TakuLoad_card_file_open(void) { step_calls[2]++; return step_result; }
int TakuCustom_card_file_open(void) { step_calls[3]++; return step_result; }

static void step_setup(int state, int result)
{
    memset(&data_0034fb00, 0, sizeof(data_0034fb00));
    memset(step_calls, 0, sizeof(step_calls));
    step_reset_calls = 0;
    step_arg = -1;
    data_0034fb00.unknown_30 = state;
    data_0034fb00.unknown_34 = data_0034fb00.unknown_40 = 99;
    data_0034fb00.unknown_0c = 12;
    data_0034fb00.unknown_2be = 0x55;
    step_result = result;
}

static void check_step_dispatch(void)
{
    int pass;
    for (pass = 0; pass < 2; pass++) {
        int (*f)(int) = pass == 0 ? TkMc_Load_Main : TkMc_Trade_Load_Main;
        int third = pass == 0 ? 2 : 3;
        /* state 0: result 2 clears the state and is stored */
        step_setup(0, 2);
        assert(f(5) == 2 && step_calls[0] == 1 && step_arg == 5 && step_reset_calls == 0);
        assert(data_0034fb00.unknown_30 == 0 && data_0034fb00.unknown_2be == 2);
        /* state 0: result 0 leaves the stored word */
        step_setup(0, 0);
        assert(f(5) == 0x55 && data_0034fb00.unknown_2be == 0x55);
        /* state 1: result 3 becomes 7 */
        step_setup(1, 3);
        assert(f(0) == 7 && step_calls[1] == 1 && data_0034fb00.unknown_30 == 1);
        /* state 2: the two functions differ only in this callee; 256 is
         * returned as is after being stored */
        step_setup(2, 256);
        assert(f(0) == 256 && step_calls[third] == 1 && step_calls[5 - third] == 0);
        assert(data_0034fb00.unknown_2be == 256);
        /* state 3: result 2 becomes 10, other results pass */
        step_setup(3, 2);
        assert(f(4) == 10 && step_calls[0] == 1 && step_arg == 4 && data_0034fb00.unknown_30 == 3);
        step_setup(3, 5);
        assert(f(4) == 5);
        /* any other state: no callee */
        step_setup(4, 9);
        assert(f(0) == 0x55 && step_calls[0] + step_calls[1] + step_calls[2] + step_calls[3] == 0);
        /* mode 1: reset call, fields cleared, result forced to 16 */
        step_setup(1, 3);
        assert(f(1) == 16 && step_reset_calls == 1 && step_calls[1] == 1);
        assert(data_0034fb00.unknown_30 == 0 && data_0034fb00.unknown_34 == 0 && data_0034fb00.unknown_40 == 0);
        assert(data_0034fb00.unknown_0c == (pass == 0 ? 12 : 0));
        /* 0x23c8b0 only: unknown_08 == 1 sets bit 7 of the stored word */
        step_setup(1, 3);
        data_0034fb00.unknown_08 = 1;
        assert(f(0) == (pass == 0 ? 7 : 0x87));
        step_setup(2, 256);
        data_0034fb00.unknown_08 = 1;
        assert(f(0) == 256 && data_0034fb00.unknown_2be == 256);
    }
}

/* TkMc_Check_Main (branch coverage): each state, each result class, mode 1. */
static int step2_calls[2];
int TakuSave_card_kansi(int a) { assert(a == 1); step2_calls[0]++; return step_result; }
int TakuSave_card_filename_check(int a) { assert(a == 1); step2_calls[1]++; return step_result; }

static void check_step_dispatch_2(void)
{
    static const short first[] = { 0, 14, 11, 3 };
    static const short to_256[] = { 2, 3, 14, 6, 8, 10, 256 };
    static const short kept[] = { 1, 4, 5, 7, 9, 11, 13, 15 };
    unsigned i;
    for (i = 0; i < sizeof(first) / sizeof(first[0]); i++) {
        step_setup(first[i], 5);
        step2_calls[0] = step2_calls[1] = 0;
        assert(TkMc_Check_Main(0) == 5 && step2_calls[0] == 1 && step2_calls[1] == 0);
        assert(data_0034fb00.unknown_30 == first[i] && data_0034fb00.unknown_2be == 5);
    }
    step_setup(1, 5);
    step2_calls[0] = step2_calls[1] = 0;
    assert(TkMc_Check_Main(0) == 5 && step2_calls[0] == 0 && step2_calls[1] == 1);
    /* other states call nothing and return the stored short */
    step_setup(2, 5);
    step2_calls[0] = step2_calls[1] = 0;
    assert(TkMc_Check_Main(0) == 0x55 && step2_calls[0] + step2_calls[1] == 0);
    for (i = 0; i < sizeof(to_256) / sizeof(to_256[0]); i++) {
        step_setup(0, to_256[i]);
        assert(TkMc_Check_Main(0) == 256 && data_0034fb00.unknown_2be == 256);
    }
    for (i = 0; i < sizeof(kept) / sizeof(kept[0]); i++) {
        step_setup(0, kept[i]);
        assert(TkMc_Check_Main(0) == kept[i] && data_0034fb00.unknown_2be == kept[i]);
    }
    step_setup(0, 0);
    assert(TkMc_Check_Main(0) == 0x55 && data_0034fb00.unknown_2be == 0x55);
    /* mode 1: reset call, stored short cleared first, three fields cleared */
    step_setup(1, 0);
    assert(TkMc_Check_Main(1) == 0 && step_reset_calls == 1);
    assert(data_0034fb00.unknown_30 == 0 && data_0034fb00.unknown_34 == 0 && data_0034fb00.unknown_40 == 0);
    step_setup(1, 5);
    assert(TkMc_Check_Main(1) == 5 && data_0034fb00.unknown_0c == 12);
}

/* TkMc_Custom_Main (branch coverage): states 0 and 3 call the stubbed 0x23fce0,
 * 1 and 2 the stubbed 0x23f340 and 0x23ff30; the result mapping depends on
 * the word at +8. */
static int step3_calls, step3_arg;
int TakuCustom_card_kansi(int a) { step3_arg = a; step3_calls++; return step_result; }

static void check_step_dispatch_3(void)
{
    static const short to_300[] = { 3, 5, 6, 8, 10, 7 };
    static const short kept[] = { 1, 4, 9, 11, 16 };
    unsigned i;
    /* state 0: result 2 clears the state (already 0); state 3: 2 becomes 10 */
    step_setup(0, 5);
    step3_calls = 0;
    assert(TkMc_Custom_Main(0) == 5 && step3_calls == 1 && step3_arg == 0);
    assert(data_0034fb00.unknown_2be == 5);
    step_setup(0, 2);
    assert(TkMc_Custom_Main(0) == 2 && data_0034fb00.unknown_30 == 0);
    step_setup(3, 2);
    step3_calls = 0;
    assert(TkMc_Custom_Main(0) == 10 && step3_calls == 1 && data_0034fb00.unknown_30 == 3);
    step_setup(3, 4);
    assert(TkMc_Custom_Main(0) == 4);
    step_setup(1, 5);
    step3_calls = 0;
    assert(TkMc_Custom_Main(0) == 5 && step_calls[1] == 1 && step_calls[3] == 0 && step3_calls == 0);
    step_setup(2, 5);
    assert(TkMc_Custom_Main(0) == 5 && step_calls[1] == 0 && step_calls[3] == 1);
    /* other states call nothing; result 0 keeps the stored short */
    step_setup(4, 5);
    step3_calls = 0;
    assert(TkMc_Custom_Main(0) == 0x55 && step3_calls == 0 && step_calls[1] + step_calls[3] == 0);
    step_setup(0, 0);
    assert(TkMc_Custom_Main(0) == 0x55);
    /* word at +8 not 1: only 3 becomes 7; 256 returns before the or */
    step_setup(0, 3);
    assert(TkMc_Custom_Main(0) == 7 && data_0034fb00.unknown_2be == 7);
    step_setup(0, 256);
    assert(TkMc_Custom_Main(0) == 256 && data_0034fb00.unknown_2be == 256);
    /* word at +8 is 1: mapped values, then 0x80 is ored in */
    for (i = 0; i < sizeof(to_300) / sizeof(to_300[0]); i++) {
        step_setup(1, to_300[i]);
        data_0034fb00.unknown_08 = 1;
        assert(TkMc_Custom_Main(0) == 0x380 && data_0034fb00.unknown_2be == 0x380);
    }
    step_setup(1, 2);
    data_0034fb00.unknown_08 = 1;
    assert(TkMc_Custom_Main(0) == 0x280);
    for (i = 0; i < sizeof(kept) / sizeof(kept[0]); i++) {
        step_setup(1, kept[i]);
        data_0034fb00.unknown_08 = 1;
        assert(TkMc_Custom_Main(0) == (kept[i] | 0x80));
    }
    step_setup(1, 256);
    data_0034fb00.unknown_08 = 1;
    assert(TkMc_Custom_Main(0) == 256 && data_0034fb00.unknown_2be == 256);
    step_setup(1, 0);
    data_0034fb00.unknown_08 = 1;
    assert(TkMc_Custom_Main(0) == (0x55 | 0x80));
    /* mode 1: reset call, fields cleared, result forced to 16 */
    step_setup(3, 5);
    step3_calls = 0;
    assert(TkMc_Custom_Main(1) == 16 && step_reset_calls == 1 && step3_calls == 1 && step3_arg == 1);
    assert(data_0034fb00.unknown_30 == 0 && data_0034fb00.unknown_34 == 0);
    assert(data_0034fb00.unknown_40 == 0 && data_0034fb00.unknown_0c == 0);
}

/* Ta_GetScrificeOrder (branch coverage) over the real bit-word routines 0x24e910,
 * 0x24ec50, 0x24ea10, 0x24c7d0, 0x24f200, 0x24efb0 and 0x24c780, with the
 * stubbed scores and pair values. */
static int pick_sets[3], pick_arg;
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && c == pick_arg && d == 1 && f == 14 && e >= 10 && e < 13);
    return pick_sets[e - 10];
}

#define PICK_BIT(h, l) ((h) == 0 ? 1 << (21 - (l)) : 1 << (10 - (l)))
static void check_three_picks(void)
{
    static int words[3] = { 10, 11, 12 };
    int i, j;

    memset(pair_values, 0, sizeof(pair_values));
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 11; j++) {
            scores[i][j] = 100;
        }
    }
    scores[0][2] = 5;
    scores[1][3] = 9;
    scores[0][5] = 1;
    pick_arg = 7;
    /* three picks: the second set loses the position already taken */
    pick_sets[0] = PICK_BIT(0, 2) | PICK_BIT(1, 3);
    pick_sets[1] = PICK_BIT(0, 2) | PICK_BIT(1, 3);
    pick_sets[2] = PICK_BIT(0, 5);
    assert(Ta_GetScrificeOrder(words, 7) == (PICK_BIT(0, 2) | PICK_BIT(1, 3) | PICK_BIT(0, 5)));
    /* an empty or negative set is skipped: fewer than three picks give -1 */
    pick_sets[2] = 0;
    assert(Ta_GetScrificeOrder(words, 7) == -1);
    pick_sets[2] = -1;
    assert(Ta_GetScrificeOrder(words, 7) == -1);
    /* a set emptied by the removal gives no pick */
    pick_sets[2] = PICK_BIT(0, 2);
    assert(Ta_GetScrificeOrder(words, 7) == -1);
    /* 0x24efb0 returning -1 (no score below 255) adds nothing */
    pick_sets[2] = PICK_BIT(0, 5);
    scores[0][5] = 255;
    assert(Ta_GetScrificeOrder(words, 7) == -1);
    scores[0][5] = 254;
    assert(Ta_GetScrificeOrder(words, 7) == (PICK_BIT(0, 2) | PICK_BIT(1, 3) | PICK_BIT(0, 5)));
}

static int ph_calls, ph_flag, ph_a, ph_b, ph_fix, ph_count, ph_order;
static struct DotrMiscPacket128 *ph_packet, *ph_fix_packet, *ph_fix_end;
static unsigned ph_word0;
int lRhModelCpu_SetupGlobal(int flag) { ph_flag = flag; assert(ph_order++ == 0); return 1; }
struct DotrMiscPacket128 *lRhModelCpu_TraceDynamicBlock(struct DotrMiscPacket128 *packet, int a, int b)
{
    ph_calls++; ph_packet = packet; ph_a = a; ph_b = b;
    /* the head is complete before the records are filled */
    ph_word0 = ((unsigned *)packet)[-12];
    assert(ph_order++ == 1);
    return ph_count < 0 ? 0 : packet + ph_count;
}
void lRhModelCpu_SortPolygon(struct DotrMiscPacket128 *packet, struct DotrMiscPacket128 *end)
{
    ph_fix++; ph_fix_packet = packet; ph_fix_end = end;
    /* runs before the real lRhModelCpu_CleanupPacket blanks the repeated record */
    assert(packet[1].unknown_00 == 0x33333333);
}

/* RhModelCpu_DrawAlpha (branch coverage; lRhModelCpu_CleanupPacket is the real one). */
static void check_packet_head(void)
{
    struct { struct DotrMiscPacketHead head; struct DotrMiscPacket128 packets[2]; } b;

    assert(sizeof(struct DotrMiscPacketHead) == 48);
    /* no end: head answered, neither pass runs */
    memset(&b, 0x33, sizeof(b));
    ph_count = -1; ph_order = 0;
    assert(RhModelCpu_DrawAlpha(&b.head, 11, 22, 33) == (void *)&b.head);
    assert(ph_flag == 33 && ph_calls == 1 && ph_packet == b.packets && ph_a == 11 && ph_b == 22);
    assert(ph_word0 == 0x10000002 && ph_fix == 0);
    assert(b.head.words[0] == 0x10000002 && b.head.words[1] == 0);
    assert(b.head.words[2] == 0x11000000 && b.head.words[3] == 0x51000002);
    assert(b.head.tags[0] == 0x1000000000008001UL && b.head.tags[1] == 14);
    assert(b.head.tags[2] == 115 && b.head.tags[3] == 0);
    assert(b.packets[0].unknown_00 == 0x33333333 && b.packets[1].unknown_00 == 0x33333333);
    /* an end: both passes get the records and the end, which is answered */
    memset(&b, 0x33, sizeof(b));
    b.packets[0].unknown_04 = 7; b.packets[1].unknown_04 = 7;
    ph_count = 2; ph_order = 0;
    assert(RhModelCpu_DrawAlpha(&b.head, 1, 2, 0) == (void *)(b.packets + 2));
    assert(ph_flag == 0 && ph_calls == 2 && ph_fix == 1);
    assert(ph_fix_packet == b.packets && ph_fix_end == b.packets + 2);
    assert(b.head.words[0] == 0x10000002 && b.head.tags[2] == 115);
    assert(b.packets[0].unknown_00 == 0x33333333 && b.packets[1].unknown_00 == 0x10000000);
}

struct DotrViewMatrix { float unknown_values[16]; } InvertView;
static int copy_calls;
void *func_00198790(void *destination, const void *source, int byte_count)
{
    copy_calls++;
    return memcpy(destination, source, (size_t)byte_count);
}

static void entry_fill(int count)
{
    int i, j;

    memset(require_coordinate, 0, sizeof(require_coordinate));
    for (i = 0; i < 6; i++) {
        require_coordinate[i].unknown_00 = 10 + i;
        for (j = 0; j < 16; j++) require_coordinate[i].unknown_10[j] = (float)(100 * i + j);
    }
    for (j = 0; j < 16; j++) InvertView.unknown_values[j] = (float)(-1 - j);
    require_coordinate_no[0] = count;
    bc00_record = 1; bc00_calls = 0; copy_calls = 0;
}

/* GetAnyCoordinate (branch coverage; RhGeom_MulMatrix is the recording stub, which leaves the
 * local copy as it is, and func_00198790 a byte copy). */
static void check_entry_matrix(void)
{
    struct DotrMiscMatrix out;
    int saved_count = require_coordinate_no[0];
    int j;

    assert(offsetof(struct DotrMiscEntry80, unknown_10) == 0x10 && sizeof(struct DotrMiscEntry80) == 80);
    memset(&out, 0, sizeof(out));
    /* count 0, id not positive, no out, no entry with the id */
    entry_fill(0);
    assert(GetAnyCoordinate(10, &out) == 0);
    entry_fill(4);
    assert(GetAnyCoordinate(0, &out) == 0 && GetAnyCoordinate(-3, &out) == 0);
    assert(GetAnyCoordinate(10, 0) == 0);
    assert(GetAnyCoordinate(14, &out) == 0 && GetAnyCoordinate(99, &out) == 0);
    assert(copy_calls == 0 && bc00_calls == 0 && out.unknown_00[0] == 0.0f);
    /* state -1: answer 0, nothing copied */
    require_coordinate[1].unknown_04 = -1;
    assert(GetAnyCoordinate(11, &out) == 0);
    assert(copy_calls == 0 && require_coordinate[1].unknown_04 == -1);
    /* state 0 at the last counted entry: its 64 bytes are copied out */
    assert(GetAnyCoordinate(13, &out) == 1);
    assert(copy_calls == 1 && bc00_calls == 0);
    for (j = 0; j < 16; j++) assert(out.unknown_00[j] == (float)(300 + j));
    /* state 1: cleared, the entry rebuilt from the object at 0x32a730, then copied out */
    entry_fill(4);
    require_coordinate[2].unknown_04 = 1;
    assert(GetAnyCoordinate(12, &out) == 1);
    assert(require_coordinate[2].unknown_04 == 0 && copy_calls == 3 && bc00_calls == 1);
    assert(bc00_out != (struct DotrMiscMatrix *)&InvertView);
    assert(bc00_in == (struct DotrMiscMatrix *)require_coordinate[2].unknown_10 && bc00_in_00 == 200.0f);
    for (j = 0; j < 16; j++) {
        assert(require_coordinate[2].unknown_10[j] == (float)(-1 - j) && out.unknown_00[j] == (float)(-1 - j));
        assert(InvertView.unknown_values[j] == (float)(-1 - j));
    }
    assert(require_coordinate[1].unknown_10[0] == 100.0f && require_coordinate[3].unknown_10[0] == 300.0f);
    /* another state: found (1) but nothing copied */
    entry_fill(4);
    require_coordinate[0].unknown_04 = 2;
    memset(&out, 0, sizeof(out));
    assert(GetAnyCoordinate(10, &out) == 1);
    assert(copy_calls == 0 && out.unknown_00[0] == 0.0f && require_coordinate[0].unknown_04 == 2);
    /* the first entry with the id is the one used */
    entry_fill(4);
    require_coordinate[3].unknown_00 = 11;
    assert(GetAnyCoordinate(11, &out) == 1 && out.unknown_00[0] == 100.0f);
    bc00_record = 0;
    require_coordinate_no[0] = saved_count;
}

/* 0x23b920: only the flag-clear outcome is executed (one call of 0x1e4530). The
 * flag-set branch copies to the fixed address 0x1e301e0, which is unmapped on the
 * host, so its constants and call order are covered by the byte comparison only. */
signed char data_01ed546b;
static int option_override;
int SzData_GetOption(int side, int which) { (void)side; return option_override ? option_override : which; }
int SzGame_SetMenuNum(int index, int value) { (void)index; return value; }
static int const_deck_calls, const_deck_index, const_deck_value;
void SzData_SetupFromConstDeck(int index, int value) { const_deck_calls++; const_deck_index = index; const_deck_value = value; }
int SzData_SetOption(int side, int which, int flag) { (void)side; (void)which; return flag; }
struct DotrTradeWork dotr_trade_work_01ed0000;
int data_003180e8;
unsigned char gSzDuel_NameIndex1[4];
unsigned char gSzDuel_NameIndex0[4];

/* SzDuel_InitTrade (branch and loop coverage): both outcomes of the option test, the first and
 * last entry of both clearing loops, every stored field, and both rebuild calls (the
 * real 0x1c2300 runs: mode 0 asks the bag stub 854 times per side). */
static void check_trade_init(void)
{
    struct DotrMiscControlLists *p = &data_00317490_lists;
    struct DotrTradeWork *w = &dotr_trade_work_01ed0000;
    int option, i, side;

    for (option = 0; option < 2; option++) {
        memset(p, 0x5a, sizeof(*p));
        memset(w, 0x5a, sizeof(*w));
        memset(own_by, 0, sizeof(own_by));
        data_003180e8 = 0;
        option_override = option ? 7 : 0;
        own_calls = own_kind_bad = 0;
        own_side = -1;
        SzDuel_InitTrade();
        option_override = 0;
        assert(data_003180e8 == 0xfffff8);
        assert(p->table == (option ? (void *)gSzDuel_NameIndex1 : (void *)gSzDuel_NameIndex0));
        assert(p->lists[0] == w->lists[0] && p->lists[1] == w->lists[1]);
        assert(p->counts[0] == w->counts[0] && p->counts[1] == w->counts[1]);
        for (side = 0; side < 2; side++) {
            for (i = 0; i < 854; i++)
                assert(w->lists[side][i] == 999 && w->counts[side][i] == 0);
            assert(p->modes[side] == 0 && p->rows[side] == 0);
            assert(p->pairs[side].top == 0 && p->pairs[side].count == 0);
            assert(p->unknown_cbc[side] == 0 && p->unknown_cbe[side] == 1);
        }
        assert(p->unknown_c92 == 0 && p->unknown_c94 == 0 && p->unknown_c96 == 0 && p->unknown_c98 == 0);
        assert(p->unknown_cb6[0] == 0 && p->unknown_cb6[1] == 4 && p->unknown_cb6[2] == 10);
        assert(p->unknown_cb6[3] == 0 && p->unknown_cb6[4] == 4 && p->unknown_cb6[5] == 10);
        assert(p->unknown_c9a[0] == 0x5a && p->unknown_c9a[0x1b] == 0x5a);
        assert(own_calls == 2 * 854 && own_side == 1 && !own_kind_bad);
    }
}

/* lSzDuel_ReBuildBag (branch and loop coverage): kept, zero and negative answers, the first and last
 * entry, a count of 5 and below, and the top below, at and above count - 5. */
static void run_bag(int kept, int top, int want_top)
{
    static short list[855];
    struct DotrMiscControlLists *p = &data_00317490_lists;
    int i;

    memset(p, 0, sizeof(*p));
    memset(own_by, 0, sizeof(own_by));
    memset(list, 0x55, sizeof(list));
    for (i = 0; i < kept - 1; i++) {
        own_by[10 + i] = 1;
    }
    own_by[0] = 2;
    own_by[853] = 255;
    p->lists[0] = list;
    p->side = 1;
    p->pairs[0].top = top;
    p->pairs[0].count = 0x7777;
    p->pairs[1].top = 0x1111;
    p->pairs[1].count = 0x2222;
    own_calls = 0;
    own_side = own_kind_last = 0;
    t_callback = 0;
    lSzDuel_ReBuildBag();
    assert(own_calls == 854 && own_side == 1 && own_kind_last == -1);
    assert(p->pairs[0].count == kept && p->pairs[0].top == want_top);
    assert(p->pairs[1].top == 0x1111 && p->pairs[1].count == 0x2222);
    assert(list[0] == 999 && list[1] == 999 && list[853] == 853 && list[854] == 0x5555);
    for (i = 0; i < kept - 1; i++) {
        assert(list[10 + i] == 10 + i);
    }
    assert(list[10 + i] == 999);
    assert(t_callback == lSzDuel_SortFunc1 && t_args[1] == 854);
}
static void check_rotation_interpolate(void)
{
    /* difference within half a turn: plain weighted sum, rounded by adding 0.5 (the conversion
     * is a cast in the source: truncation toward zero, as the host does) */
    assert(RhGeom_RotationInterpolate(0.5f, 0.5f, 0x1000, 0x3000) == 0x2000);
    assert(RhGeom_RotationInterpolate(0.25f, 0.75f, 0x1000, 0x1001) == 0x1001);
    /* inputs are masked to 16 bits */
    assert(RhGeom_RotationInterpolate(1.0f, 0.0f, 0x12345, 0x2345) == 0x2345);
    /* boundary: a difference of exactly 0x8000 or -0x8000 is not wrapped */
    assert(RhGeom_RotationInterpolate(0.5f, 0.5f, 0, 0x8000) == 0x4000);
    assert(RhGeom_RotationInterpolate(0.5f, 0.5f, 0x8000, 0) == 0x4000);
    /* second more than half a turn ahead: it is taken one turn lower */
    assert(RhGeom_RotationInterpolate(0.5f, 0.5f, 0, 0x8002) == -0x3ffe);
    /* second more than half a turn behind: it is taken one turn higher */
    assert(RhGeom_RotationInterpolate(0.5f, 0.5f, 0x8002, 0) == 0xc001);
}

static void check_bag_rebuild(void)
{
    run_bag(1, 3, 0);
    run_bag(5, 1, 0);
    run_bag(6, 0, 0);
    run_bag(6, 1, 1);
    run_bag(10, 4, 4);
    run_bag(10, 5, 5);
    run_bag(10, 9, 5);
}

static void check_block_replay(void)
{
    unsigned char saved = HELP_flg[0];

    HELP_flg[0] = 0;
    job_log_count = 0;
    TkCamp_Term();
    assert(job_log_count == 1 && job_log[0] == 0x453);
    HELP_flg[0] = saved;
}

/* 0x21c350: n = 1 (neither loop), n = 2 (one pass of each loop), n = 3 with exactly
 * representable coefficients, and n = 0 (still writes the first entry). */
static void check_tridiagonal(void)
{
    float lower[3] = { 99.0f, 1.0f, 2.0f }, diagonal[3] = { 2.0f, 4.0f, 4.0f };
    float upper[3] = { 4.0f, 2.0f, 99.0f }, right[3], out[4];

    out[0] = out[1] = out[2] = out[3] = -7.0f;
    right[0] = 6.0f;
    tri_diag(lower, diagonal, upper, right, out, 1);
    assert(out[0] == 3.0f && out[1] == -7.0f);
    out[0] = -7.0f;
    tri_diag(lower, diagonal, upper, right, out, 0);
    assert(out[0] == 3.0f && out[1] == -7.0f);
    /* 2x + 4y = 10, x + 4y = 9: x = 1, y = 2 */
    right[0] = 10.0f; right[1] = 9.0f;
    tri_diag(lower, diagonal, upper, right, out, 2);
    assert(out[0] == 1.0f && out[1] == 2.0f && out[2] == -7.0f);
    /* 2x + 4y = 10, x + 4y + 2z = 15, 2y + 4z = 16: x = 1, y = 2, z = 3 */
    right[1] = 15.0f; right[2] = 16.0f;
    tri_diag(lower, diagonal, upper, right, out, 3);
    assert(out[0] == 1.0f && out[1] == 2.0f && out[2] == 3.0f && out[3] == -7.0f);
}

struct DotrFontRect gSzDuel_Font[3 * 37];
unsigned char gSzDuel_abFontSize[3][2];

/* Branch coverage: empty text, fixed and proportional advance, unknown
 * character in both modes, newline with and without a new widest line, the
 * final widest-line update, the n bound and both null result pointers. */
static void check_font_size(void)
{
    int w, h;

    memset(gSzDuel_Font, 0, sizeof(gSzDuel_Font));
    gSzDuel_abFontSize[1][0] = 12;
    gSzDuel_abFontSize[1][1] = 20;
    gSzDuel_Font[37 + 0].left = 3;
    gSzDuel_Font[37 + 0].right = 10;
    gSzDuel_Font[37 + 1].left = 0;
    gSzDuel_Font[37 + 1].right = 5;

    w = h = -1;
    lSzDuel_GetFontSize(1, 0, 0, "AB", &w, &h);
    assert(w == 0 && h == 20);
    lSzDuel_GetFontSize(1, 0, 2, "AB", &w, &h);
    assert(w == 24 && h == 20);
    lSzDuel_GetFontSize(1, 1, 2, "AB", &w, &h);
    assert(w == 12 && h == 20);
    lSzDuel_GetFontSize(1, 1, 2, "ABAB", &w, &h);
    assert(w == 12 && h == 20);
    lSzDuel_GetFontSize(1, 1, 3, "A B", &w, &h);
    assert(w == 18 && h == 20);
    lSzDuel_GetFontSize(1, 2, 3, "A B", &w, &h);
    assert(w == 36 && h == 20);
    lSzDuel_GetFontSize(1, 1, 4, "AB\nA", &w, &h);
    assert(w == 12 && h == 40);
    lSzDuel_GetFontSize(1, 1, 4, "A\nAB", &w, &h);
    assert(w == 12 && h == 40);
    lSzDuel_GetFontSize(1, 1, 3, "A\n\n", &w, &h);
    assert(w == 7 && h == 60);
    w = h = -1;
    lSzDuel_GetFontSize(1, 1, 2, "AB", 0, &h);
    assert(w == -1 && h == 20);
    h = -1;
    lSzDuel_GetFontSize(1, 1, 2, "AB", &w, 0);
    assert(w == 12 && h == -1);
}

static void *copy16_args[2], *copy16_result;
static int copy16_count, copy16_calls;
void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    copy16_args[0] = destination; copy16_args[1] = source; copy16_count = count; copy16_calls++;
    return copy16_result;
}

/* 0x202fb0 branch coverage: `used` null and given, null file, no palette, palette that fits,
 * palette after a full allocator (restart), and the packet loop with zero and several
 * packets. The second refusal after a restart needs a palette above 32 blocks, which the
 * real SzText_GetClutSize linked here cannot return (at most 4): that branch is not run. */
static void check_text_load_pipe(void)
{
    static struct { char head[16]; struct DotrMiscFormat f; } file;
    static struct DotrMiscTextPacket pk[4];
    struct DotrMiscAccumulator saved = gSzText_tl[0];
    int used = 77, i;

    copy16_result = pk + 3; copy16_calls = 0;
    assert(SzText_Load_Pipe(pk, 0, 100, &used) == pk && used == 0 && copy16_calls == 0);
    assert(SzText_Load_Pipe(pk, 0, 100, 0) == pk);

    memset(&file, 0, sizeof(file)); memset(pk, 0, sizeof(pk));
    file.f.offset = 0x40; file.f.size = 0x95; file.f.count = 3;
    file.f.unknown_tex0_14 = 0x7fffff; file.f.unknown_tex0_51 = 0x1fff;
    for (i = 0; i < 4; i++) pk[i].unknown_46 = 0x3ffff;
    gSzText_tl[0].unknown_00 = 500; gSzText_tl[0].unknown_02 = 32; gSzText_tl[0].unknown_04 = 10;
    used = 77;
    assert(SzText_Load_Pipe(pk, (char *)&file, 100, &used) == pk + 3 && used == 96);
    assert(copy16_calls == 1 && copy16_args[0] == pk && copy16_args[1] == (char *)&file + 0x40);
    assert(copy16_count == 9);
    assert(file.f.tbp0 == 100 && file.f.cbp == 0);
    assert(file.f.unknown_tex0_14 == 0x7fffff && file.f.unknown_tex0_51 == 0x1fff);
    assert(pk[0].dbp == 100 && pk[1].dbp == 132 && pk[2].dbp == 164 && pk[3].dbp == 0);
    assert(pk[0].unknown_46 == 0x3ffff && gSzText_tl[0].unknown_04 == 10);

    memset(pk, 0, sizeof(pk));
    file.f.unknown_40 = 19; file.f.unknown_42 = 0; file.f.count = 1;
    assert(SzText_Load_Pipe(pk, (char *)&file, 200, 0) == pk + 3);
    assert(file.f.tbp0 == 200 && file.f.cbp == 510 && gSzText_tl[0].unknown_04 == 14);
    assert(pk[0].dbp == 510 && pk[1].dbp == 0);

    memset(pk, 0, sizeof(pk));
    file.f.count = 3; gSzText_tl[0].unknown_04 = 30;
    assert(SzText_Load_Pipe(pk, (char *)&file, 300, &used) == pk + 3 && used == 96);
    assert(gSzText_tl[0].unknown_00 == 300 && gSzText_tl[0].unknown_02 == 32);
    assert(gSzText_tl[0].unknown_04 == 4);
    assert(file.f.tbp0 == 332 && file.f.cbp == 300);
    assert(pk[0].dbp == 300 && pk[1].dbp == 332 && pk[2].dbp == 364 && pk[3].dbp == 0);
    gSzText_tl[0] = saved;
}

struct DotrCustomDuel dotr_custom_duel_00a9c000;
static int custom_log[8], custom_log_count, custom_edit[2], custom_copy_calls;
unsigned char *RhMem_CopyMemory(unsigned char *destination, unsigned char *source, int count)
{
    assert(destination == (unsigned char *)&gSzData_All[1] && source == (unsigned char *)&gSzData_All[0]);
    assert(count == 0xffa0);
    custom_copy_calls++;
    return destination;
}
int SzText_Free(int group, int id) { custom_log[custom_log_count++] = 0x100 + group * 16 + (id & 15); return 0; }
int SzText_LoadMerge(int a, int b) { custom_log[custom_log_count++] = 0x200 + a * 16 + b; return 0; }
int SzData_GetCurrentDeck(int side) { return 40 + side; }
int SzData_GetMapEditFlag(int side) { return custom_edit[side]; }

/* SzDuel_InitCustomDuel (branch and loop coverage): a negative and a non-negative mode, each stored
 * mode of the switch, both outcomes of both map-edit tests, the first and last entry of
 * the three list loops and every stored field; callees are stubs. */
static void check_custom_duel_init(void)
{
    struct DotrCustomDuelControl *c = (struct DotrCustomDuelControl *)&data_00317490_lists;
    struct DotrCustomDuel *d = &dotr_custom_duel_00a9c000;
    struct DotrCustomDuelFlags *f = (struct DotrCustomDuelFlags *)&gSzData_All[1];
    static const int decks[5] = { 101, 22, 25, 24, 23 };
    static const int consts[5] = { 0, 0, 50, 49, 48 };
    int mode, edit, side, i, n;

    assert(sizeof(struct DotrCustomDuelSide) == 0x350);
    assert(offsetof(struct DotrCustomDuelControl, a) == 0xd32);
    assert(offsetof(struct DotrCustomDuel, mode) == 0x88 && offsetof(struct DotrCustomDuel, list) == 0x18);
    gSzData_bRoseFlag[0] = 0;
    for (mode = 0; mode < 6; mode++) {
        for (edit = 0; edit < 4; edit++) {
            memset(&data_00317490_lists, 0x5a, sizeof(data_00317490_lists));
            memset(d, 0x5a, sizeof(*d));
            f->b0 = 31; f->b5 = 1;
            custom_edit[0] = edit & 1; custom_edit[1] = edit >> 1;
            custom_log_count = custom_copy_calls = const_deck_calls = 0;
            SzDuel_InitCustomDuel(mode);
            assert(d->mode == mode);
            assert(custom_copy_calls == (mode == 1) && f->b5 == (mode != 1) && f->b0 == 31);
            assert(const_deck_calls == (mode >= 2 && mode <= 4));
            if (const_deck_calls) assert(const_deck_index == 1 && const_deck_value == consts[mode]);
            assert(custom_log_count == 5 && custom_log[0] == 0x101 && custom_log[1] == 0x100);
            assert(custom_log[2] == 0x11f && custom_log[3] == 0x241 && custom_log[4] == 0x271);
            assert(c->side[0].deck == 100 && c->side[1].deck == (mode < 5 ? decks[mode] : 101));
            for (side = 0; side < 2; side++) {
                assert(c->side[side].lp == 4000 && c->side[side].n == 3);
                assert(d->f0[side] == 0 && d->f2[side] == 0 && d->deck[side] == 40 + side);
                assert(d->lp[side] == 4000 && d->lp2[side] == 4000 && d->f6[side] == 3);
                assert(d->f8[side] == 1 && d->fa[side] == 1 && d->fc[side] == 1 && d->fe[side] == 1);
            }
            assert(d->list[0] == 0x200 && d->list[23] == 0x217);
            n = 24;
            if (edit & 1) {
                assert(d->list[n] == 0 && d->list[n + 4] == 4);
                n += 5;
            }
            assert(d->f8d == ((edit & 1) ? 5 : 0) && d->f8c == ((edit & 1) ? 3 : 0));
            if (edit & 2) {
                assert(d->list[n] == 0x100 && d->list[n + 4] == 0x104);
                n += 5;
            }
            for (i = n; i < 56; i++) assert(d->list[i] == 0x5a5a);
            assert(d->count == n && d->last == n - 2 && d->f8b == 0);
            assert(c->a == 0 && c->b == 0 && c->side[0].unknown_33d[0] == 0x5a);
        }
    }
    /* Negative mode: the stored mode decides the deck, only the decks and the kept
     * life points are written. */
    for (mode = 0; mode < 5; mode++) {
        memset(&data_00317490_lists, 0x5a, sizeof(data_00317490_lists));
        memset(d, 0x5a, sizeof(*d));
        d->mode = (unsigned char)mode; d->lp2[0] = 1234; d->lp2[1] = 4321;
        f->b5 = 1;
        custom_log_count = custom_copy_calls = const_deck_calls = 0;
        SzDuel_InitCustomDuel(-1);
        assert(d->mode == mode && custom_copy_calls == (mode == 1) && f->b5 == (mode != 1));
        assert(const_deck_calls == (mode >= 2));
        assert(c->side[0].deck == 100 && c->side[0].lp == 1234 && c->side[0].n == 0x5a);
        assert(c->side[1].deck == decks[mode] && c->side[1].lp == 4321 && c->side[1].n == 0x5a);
        assert(d->count == 0x5a && d->f0[0] == 0x5a && d->list[0] == 0x5a5a && d->last == 0x5a);
        assert(custom_log_count == 5 && custom_log[4] == 0x271 && c->a == 0 && c->b == 0);
    }
}

int main(void)

{
    check_tridiagonal();
    check_slot_update();
    check_best_entry_veto();
    check_position_collect();
    check_word_sort();
    check_probe_free_cell();
    check_cell_pick();
    check_word_class();
    check_scan_emit();
    check_job_queue_533();
    check_job_queue_601();
    check_sorted_emit();
    check_vector_mean();
    check_job_queue();
    check_rebase_head();
    struct DotrMiscLarge *large = malloc(sizeof(*large));
    struct DotrMiscObject object, expected;
    struct DotrMiscCounter counter;
    struct DotrMiscPair pair = {11, -22};
    unsigned char key = 200;
    int i;
    assert(large != 0);
    assert(offsetof(struct DotrMiscObject, unknown_48) == 0x48);
    assert(offsetof(struct DotrMiscObject, unknown_a8) == 0xa8);
    large->unknown_50000 = large->unknown_50004 = large->unknown_50008 = -1;
    readBufCreate(large);
    assert(large->unknown_50000 == 0 && large->unknown_50004 == 0 && large->unknown_50008 == 0x50000);
    free(large);
    readBufDelete();
    voBufDelete();
    ShFace_Term();

    isCountVblank[0] = frd[0] = 9;
    endDisplay();
    assert(isCountVblank[0] == 0 && frd[0] == 0);
    switchThread();
    assert(tick_calls == 1 && tick_argument == 32);
    /* The endless loop repeats the same call. */
    tick_calls = 0;
    loop_limit = 5;
    if (setjmp(escape) == 0) {
        defMain();
        assert(0);
    }
    assert(tick_calls == 5 && tick_argument == 32);
    loop_limit = 0;
    ErrMessage(-77);
    assert(print_text == data_0030f3ae && print_value == -77);
    assert(mpegError(0, &pair) == 1 && print_text == data_0030f8e9 && print_value == -22);

    memset(&object, 0x6d, sizeof(object));
    object.unknown_48 = 10;
    object.unknown_54 = 9;
    assert(audioDecIsPreset(&object) == 0);
    object.unknown_54 = 10;
    assert(audioDecIsPreset(&object) == 1);
    object.unknown_54 = 11;
    assert(audioDecIsPreset(&object) == 1);
    assert(videoDecBeginPut(&object) == 0x40f0 && adjusted == (void *)&object.unknown_48);
    assert(videoDecEndPut(&object) == 0x41f0 && adjusted == (void *)&object.unknown_48);
    memcpy(&expected, &object, sizeof(expected));
    expected.unknown_a8 = 1;
    videoDecAbort(&object);
    assert(memcmp(&expected, &object, sizeof(expected)) == 0 && videoDecGetState(&object) == 1);
    expected.unknown_a8 = 0;
    videoDecReset(&object);
    assert(memcmp(&expected, &object, sizeof(expected)) == 0 && videoDecGetState(&object) == 0);
    expected.unknown_08 = expected.unknown_0c = 0;
    voBufReset(&object);
    assert(memcmp(&expected, &object, sizeof(expected)) == 0);
    object.unknown_10 = 0;
    assert(voBufIsFull(&object) == 1);
    object.unknown_10 = 1;
    assert(voBufIsFull(&object) == 0);

    counter.unknown_0c = 2;
    voBufDecCount(&counter);
    assert(counter.unknown_0c == 1);
    voBufDecCount(&counter);
    voBufDecCount(&counter);
    assert(counter.unknown_0c == 0);
    counter.unknown_0c = -3;
    voBufDecCount(&counter);
    assert(counter.unknown_0c == -3);

    assert(videoDecSetStream() == 1 && calls[0] == 1);
    tick_calls = 0;
    assert(mpegNodata() == 1 && tick_calls == 1 && calls[1] == 1);
    assert(mpegStopDMA() == 1 && calls[2] == 1 && mpegRestartDMA() == 1 && calls[3] == 1);

    memset(gSh_Face, 0x44, sizeof(gSh_Face));
    ShFace_Close(&key);
    for (i = 0; i < 256; i++) {
        assert(gSh_Face[i].unknown_00 == (i == 200 ? 0 : 0x44444444));
        assert(gSh_Face[i].unknown_04[0] == 0x44);
    }
    for (i = 0; i < 4; i++) {
        data_0029d586[i].unknown_bytes[0] = (unsigned char)(i << 1 | 1);
        data_0029d586[i].unknown_bytes[1] = 0xff;
    }
    assert(Ta_GetToonAgreement(0) == 0 && Ta_GetToonAgreement(1) == 1 && Ta_GetToonAgreement(2) == 0 && Ta_GetToonAgreement(3) == 1);
    /* The index is truncated to 16 bits. */
    EnchantId[5] = 0;
    EnchantId[0xffff] = 7;
    assert(TaEnc_IsActReflexEnc(5) == 1 && TaEnc_GetEncID(5) == 0);
    assert(TaEnc_IsActReflexEnc(-1) == 0 && TaEnc_GetEncID(-1) == 7);
    assert(TaEnc_IsActReflexEnc(0x10005) == 1 && TaEnc_GetEncID(0x1ffff) == 7);
    check_second_batch();
    check_third_batch();
    check_fourth_batch();
    check_gated_comparison();
    check_table_helpers();
    check_bit_words();
    check_ring_queue_rows();
    check_oldest_queue_address();
    check_mode_value();
    check_phase_interpolation();
    check_byte_helpers();
    check_scan_and_accumulator();
    check_clamps_and_modes();
    check_scan_gate_and_records();
    check_selectors_and_flags();
    check_modes_and_position_scans();
    check_small_leaves();
    check_brackets_and_callers();
    check_tables_and_stream();
    check_fixups();
    check_packed_and_packets();
    check_nibble_callers();
    check_bit_pick_and_insert();
    check_forwarders_and_words();
    check_moves_and_rules();
    check_flag_and_cell_scans();
    check_fixup_head();
    check_scans_and_copies();
    check_seventh_batch();
    check_tenth_batch();
    check_eleventh_batch();
    check_twelfth_batch();
    check_candidate_list();
    check_euler_matrix();
    check_block_forwarders();
    check_best_entry();
    check_gated_forward();
    check_neighbour_step();
    check_candidate_pick();
    check_entry_loop();
    check_solo_batch();
    check_solo_batch_2();
    check_solo_batch_3();
    check_solo_batch_4();
    check_solo_batch_5();
    check_entry_or();
    check_link();
    check_number_text();
    check_count_cancel();
    check_position_callback();
    check_slot_claim();
    check_level_moves();
    check_list_rebuild();
    check_grid_load();
    check_side_strip();
    check_side_strip_cue();
    check_job_queue_603();
    check_job_queue_534();
    check_random_constant();
    check_chain_cells();
    check_cell_pair_test();
    check_step_dispatch();
    check_three_picks();
    check_step_dispatch_2();
    check_step_dispatch_3();
    check_packet_head();
    check_entry_matrix();
    check_block_replay();
    check_font_size();
    check_text_load_pipe();
    check_face_draw_one();
    check_trade_init();
    check_bag_rebuild();
    check_rotation_interpolate();
    check_custom_duel_init();
    puts("small_misc: ok");
    return 0;
}
