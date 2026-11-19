/* Synthetic host tests for the debug_menu group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/debug_menu.h"

struct DotrSmallFlagsOwner data_00325c80_owner;
struct DotrDebugDigits data_00293d28;
struct DotrDebugDigits data_00293d38;
struct DotrDebugDigits data_00293d48;
static int clear_calls, destroy_calls, destroy_args[2], eff_entry_calls, eff_entry_args[5], erase_calls, erase_args[2],
    sleep_calls, sleep_args[3];
static char name_storage[10][2];
char *gBaseStr_ptrs[10];

static int pad[2][3];
static int standby_result, type_result, mode_result, view_mode, view_set[2], view_set_calls, effect_calls, se_calls,
    se_args[2], model_no, loading_result, card_args[2], card_calls, xax_arg, const_arg, const_calls, preset_args[3],
    preset_calls, unit_calls, unit_args[2], load_args[4], load_calls, load_result, entry_args[2], entry_calls,
    busy_args[3], busy_calls, ctl_mode_args[3], ctl_mode_calls, ctl_rate_args[4], ctl_rate_calls, deform_args[4],
    deform_calls, stone_args[2], stone_calls, hide_result, ready_result, ready_calls, disp_calls, get_deform_calls,
    name_calls, loading_calls, type_calls;
static const char *name_arg;
static struct DotrDebugCard card, *card_result;
static struct DotrDebugCardConst card_const, *const_result;
static void *unit_result;

#define ST (&data_00325c80_owner)

/* The mask (0, 1, 3) is only used by lSzDebug_ActEffect, which is linked for real: it counts the calls. */
int SzPad_Check(int index, int mode, int mask)
{
    if (index == 0 && mode == 1 && mask == 3) {
        effect_calls++;
    }
    return pad[index][mode] & mask;
}
void SzEffect_Clear(void) { clear_calls++; }
int SzEffect_Destroy2(int a, int b) { destroy_args[0] = a; destroy_args[1] = b; destroy_calls++; return 0; }
int SzEffect_Entry(int a, int b, int c, int d, int e)
{
    eff_entry_args[0] = a; eff_entry_args[1] = b; eff_entry_args[2] = c; eff_entry_args[3] = d; eff_entry_args[4] = e;
    eff_entry_calls++;
    return 0;
}
int SzEffect_Erase2(int a, int b) { erase_args[0] = a; erase_args[1] = b; erase_calls++; return 0; }
int SzEffect_Sleep2(int a, int b, int flag) { sleep_args[0] = a; sleep_args[1] = b; sleep_args[2] = flag; sleep_calls++; return 0; }
int SzDebug_GetActiveMenu(void) { return ST->unknown_00[DOTR_DEBUG_MENU]; }
int SzDebug_IsHide(void) { return hide_result; }
int SzModel_IsModelStandby(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); return standby_result; }
int SzModel_GetModelType(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); type_calls++; return type_result; }
int SzGame_GetMode(int a) { assert(a == 1); return mode_result; }
int SzView_GetMode(int index) { assert(index == 0); return view_mode; }
int SzView_SetMode(int index, int value) { view_set[0] = index; view_set[1] = value; view_set_calls++; return 0; }
int SzSound_PlaySE(int index, int value) { se_args[0] = index; se_args[1] = value; se_calls++; return 0; }
int SzModel_GetModelNo(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); return model_no; }
int SzModel_IsModelLoading(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); loading_calls++; return loading_result; }
struct DotrDebugCard *SzDuel_GetCardInfo(int a, int b) { card_args[0] = a; card_args[1] = b; card_calls++; return card_result; }
int SzDuel_Xax2New(int value) { xax_arg = value; return value + 0x1000; }
struct DotrDebugCardConst *SzDuel_GetCardConstInfo(int id) { const_arg = id; const_calls++; return const_result; }
int SzModel_Preset(int index, int value, int unknown)
{
    preset_args[0] = index; preset_args[1] = value; preset_args[2] = unknown; preset_calls++;
    return 0;
}
void *SzDuel_GetUnit(int a, int b) { unit_args[0] = a; unit_args[1] = b; unit_calls++; return unit_result; }
int SzModel_Load(int a, int b, int c, int d)
{
    load_args[0] = a; load_args[1] = b; load_args[2] = c; load_args[3] = d; load_calls++;
    return load_result;
}
void SzBattle_Entry(int side, int packed) { entry_args[0] = side; entry_args[1] = packed; entry_calls++; }
int SzDuel_SetBusyFlag(int a, int b, int flag) { busy_args[0] = a; busy_args[1] = b; busy_args[2] = flag; busy_calls++; return 0; }
int SzDuel_SetUnitCtlModeM(int a, int b, int value)
{
    ctl_mode_args[0] = a; ctl_mode_args[1] = b; ctl_mode_args[2] = value; ctl_mode_calls++;
    return 0;
}
int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value)
{
    ctl_rate_args[0] = a; ctl_rate_args[1] = b; ctl_rate_args[2] = pair; ctl_rate_args[3] = value; ctl_rate_calls++;
    return 0;
}
int SzModel_SetDeform(int index, int value)
{
    assert(deform_calls < 2);
    deform_args[deform_calls * 2] = index; deform_args[deform_calls * 2 + 1] = value; deform_calls++;
    return 0;
}
int SzModel_SetStoneFlag(int index, int value) { stone_args[0] = index; stone_args[1] = value; stone_calls++; return 0; }
int func_0019bef0(const char *string) { name_arg = string; name_calls++; return 0; }
int SzModel_IsModelReady(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); ready_calls++; return ready_result; }
int SzModel_IsModelDisp(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); disp_calls++; return 1; }
int SzModel_GetDeform(int index) { assert(index == ST->unknown_00[DOTR_DEBUG_MODEL]); get_deform_calls++; return 0; }

/* Clears the fixtures: no pad input, a standing-by model of type 0 and number 123, hidden output. */
static void reset(void)
{
    int i;

    memset(&data_00325c80_owner, 0, sizeof(data_00325c80_owner));
    memset(pad, 0, sizeof(pad));
    memset(&card, 0, sizeof(card));
    for (i = 0; i < 10; i++) {
        gBaseStr_ptrs[i] = name_storage[i];
    }
    standby_result = 1; type_result = 0; mode_result = 0; view_mode = 0; model_no = 123; loading_result = 0;
    card_result = 0; const_result = 0; unit_result = &card; load_result = 0; hide_result = 1; ready_result = 0;
    view_set_calls = effect_calls = se_calls = card_calls = const_calls = preset_calls = unit_calls = load_calls = 0;
    entry_calls = busy_calls = ctl_mode_calls = ctl_rate_calls = deform_calls = stone_calls = ready_calls = 0;
    disp_calls = get_deform_calls = name_calls = loading_calls = type_calls = 0;
    xax_arg = const_arg = -1;
    name_arg = 0;
    clear_calls = destroy_calls = eff_entry_calls = erase_calls = sleep_calls = 0;
}

/* One frame of the effect page: the cursor, the edited value and the two pad words. */
static void eff(int row, int column, int index, int value, int pad2, int pad0)
{
    reset();
    ST->unknown_00[DOTR_DEBUG_EFF_ROW] = row; ST->unknown_00[DOTR_DEBUG_EFF_COLUMN] = column;
    ST->unknown_00[index] = value;
    pad[0][2] = pad2; pad[0][0] = pad0;
    lSzDebug_ActEffect();
}
#define CUR(row, column) (ST->unknown_00[DOTR_DEBUG_EFF_ROW] == (row) && ST->unknown_00[DOTR_DEBUG_EFF_COLUMN] == (column))

static void check_effect_page(void)
{
    static const int single[4] = { DOTR_DEBUG_EFF_A_HI, DOTR_DEBUG_EFF_A_LO, DOTR_DEBUG_EFF_B_HI, DOTR_DEBUG_EFF_B_LO };
    static const int next_row[4] = { 3, 4, 5, 0 }, prev_row[4] = { 1, 2, 3, 4 }, prev_column[4] = { 2, 0, 0, 0 };
    int i;

    /* Both low bits of pad word (0, 1): nothing is edited and no effect call is made. */
    reset();
    pad[0][1] = 3; pad[0][2] = 0x2000; pad[0][0] = 0x10; ST->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 2;
    lSzDebug_ActEffect();
    assert(CUR(0, 2) && clear_calls == 0);
    /* One of the two bits is not enough to skip. */
    reset();
    pad[0][1] = 1; pad[0][0] = 0x10;
    lSzDebug_ActEffect();
    assert(clear_calls == 1);

    /* Rows 0 and 1: three digit columns; the cursor leaves the row past column 2 or below 0. */
    eff(0, 1, DOTR_DEBUG_EFF_NO, 0, 0x2000, 0); assert(CUR(0, 2));
    eff(0, 2, DOTR_DEBUG_EFF_NO, 0, 0x2000, 0); assert(CUR(1, 0));
    eff(0, 1, DOTR_DEBUG_EFF_NO, 0, 0x8000, 0); assert(CUR(0, 0));
    eff(0, 0, DOTR_DEBUG_EFF_NO, 0, 0x8000, 0); assert(CUR(5, 0));
    eff(0, 2, DOTR_DEBUG_EFF_NO, 129, 0x1000, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_NO] == 120 && CUR(0, 2));
    eff(0, 0, DOTR_DEBUG_EFF_NO, 129, 0x1000, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_NO] == 229);
    eff(0, 1, DOTR_DEBUG_EFF_NO, 109, 0x4000, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_NO] == 199);
    eff(0, 1, DOTR_DEBUG_EFF_NO, 109, 0, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_NO] == 109 && CUR(0, 1));
    eff(1, 1, DOTR_DEBUG_EFF_ARG, 0, 0x2000, 0); assert(CUR(1, 2));
    eff(1, 2, DOTR_DEBUG_EFF_ARG, 0, 0x2000, 0); assert(CUR(2, 0));
    eff(1, 1, DOTR_DEBUG_EFF_ARG, 0, 0x8000, 0); assert(CUR(1, 0));
    eff(1, 0, DOTR_DEBUG_EFF_ARG, 0, 0x8000, 0); assert(CUR(0, 2));
    eff(1, 2, DOTR_DEBUG_EFF_ARG, 129, 0x1000, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_ARG] == 120);
    eff(1, 0, DOTR_DEBUG_EFF_ARG, 129, 0x1000, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_ARG] == 229);
    eff(1, 1, DOTR_DEBUG_EFF_ARG, 109, 0x4000, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_ARG] == 199);
    eff(1, 1, DOTR_DEBUG_EFF_ARG, 109, 0, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_ARG] == 109 && CUR(1, 1));

    /* Rows 2..5: one column; a 0..1 value (rows 2, 4) or a 0..10 value (rows 3, 5). */
    for (i = 0; i < 4; i++) {
        int row = i + 2, wide = i & 1, top = wide ? 10 : 1;

        eff(row, 0, single[i], 0, 0x2000, 0); assert(CUR(next_row[i], 0));
        eff(row, -5, single[i], 0, 0x2000, 0); assert(CUR(row, -4));
        eff(row, 0, single[i], 0, 0x8000, 0); assert(CUR(prev_row[i], prev_column[i]));
        eff(row, 1, single[i], 0, 0x8000, 0); assert(CUR(row, 0));
        eff(row, 0, single[i], top, 0x1000, 0); assert(ST->unknown_00[single[i]] == 0 && CUR(row, 0));
        eff(row, 0, single[i], 0, 0x1000, 0); assert(ST->unknown_00[single[i]] == 1);
        eff(row, 0, single[i], 0, 0x4000, 0); assert(ST->unknown_00[single[i]] == top);
        eff(row, 0, single[i], top, 0x4000, 0); assert(ST->unknown_00[single[i]] == top - 1);
        eff(row, 0, single[i], top, 0, 0); assert(ST->unknown_00[single[i]] == top && CUR(row, 0));
    }
    /* A row outside 0..5 edits nothing. */
    eff(6, 1, DOTR_DEBUG_EFF_NO, 129, 0xf000, 0); assert(CUR(6, 1) && ST->unknown_00[DOTR_DEBUG_EFF_NO] == 129);

    /* Masks 8 and 4 of pad word (0, 2) step the 0..10 value of row 3 on any row. */
    eff(6, 0, DOTR_DEBUG_EFF_A_LO, 10, 8, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_A_LO] == 0);
    eff(6, 0, DOTR_DEBUG_EFF_A_LO, 0, 4, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_A_LO] == 10);
    eff(6, 0, DOTR_DEBUG_EFF_A_LO, 5, 0, 0); assert(ST->unknown_00[DOTR_DEBUG_EFF_A_LO] == 5);

    /* Pad word (0, 0): one effect call per mask, none without it. */
    eff(6, 0, DOTR_DEBUG_EFF_NO, 321, 0, 0);
    assert(clear_calls == 0 && destroy_calls == 0 && eff_entry_calls == 0 && erase_calls == 0 && sleep_calls == 0);
    eff(6, 0, DOTR_DEBUG_EFF_NO, 321, 0, 0x10);
    assert(clear_calls == 1 && destroy_calls == 0 && eff_entry_calls == 0 && erase_calls == 0 && sleep_calls == 0);
    eff(6, 0, DOTR_DEBUG_EFF_NO, 321, 0, 0x40);
    assert(clear_calls == 0 && destroy_calls == 1 && destroy_args[0] == 0 && destroy_args[1] == 321);
    reset();
    for (i = 0; i < 6; i++) {
        ST->unknown_00[DOTR_DEBUG_EFF_NO + i] = i + 1;
    }
    ST->unknown_00[DOTR_DEBUG_EFF_ROW] = 6; pad[0][0] = 0x20;
    lSzDebug_ActEffect();
    assert(eff_entry_calls == 1 && eff_entry_args[0] == 0 && eff_entry_args[1] == 1 && eff_entry_args[2] == 2);
    assert(eff_entry_args[3] == 0x304 && eff_entry_args[4] == 0x506 && destroy_calls == 0 && erase_calls == 0);
    eff(6, 0, DOTR_DEBUG_EFF_NO, 321, 0, 0x80);
    assert(erase_calls == 1 && erase_args[0] == 0 && erase_args[1] == -1 && eff_entry_calls == 0 && sleep_calls == 0);
    eff(6, 0, DOTR_DEBUG_EFF_NO, 321, 0, 0x800);
    assert(sleep_calls == 1 && sleep_args[0] == 0 && sleep_args[1] == 321 && sleep_args[2] == 0 && erase_calls == 0);
    eff(6, 0, DOTR_DEBUG_EFF_NO, 321, 0, 0x100);
    assert(sleep_calls == 1 && sleep_args[0] == 0 && sleep_args[1] == 321 && sleep_args[2] == 1);
}

/* Runs one frame with no pad input and returns the menu page left in the state. */
static int settle(int menu, int standby, int type, int mode)
{
    reset();
    ST->unknown_00[DOTR_DEBUG_MENU] = menu; standby_result = standby; type_result = type; mode_result = mode;
    SzDebug_Act();
    return ST->unknown_00[DOTR_DEBUG_MENU];
}

/* Runs one frame with one menu-step button and returns the page reached. */
static int step(int mask, int menu, int standby, int type, int mode)
{
    reset();
    pad[0][2] = mask; ST->unknown_00[DOTR_DEBUG_MENU] = menu; standby_result = standby; type_result = type; mode_result = mode;
    SzDebug_Act();
    assert(se_calls == 1 && se_args[0] == 0 && se_args[1] == 0);
    return ST->unknown_00[DOTR_DEBUG_MENU];
}

static void check_menu_rules(void)
{
    /* pages 1..11 need a model that is standing by */
    assert(settle(1, 0, 0, 0) == 0 && settle(11, 0, 0, 0) == 0);
    assert(settle(12, 0, 0, 0) == 12 && settle(0, 0, 0, 0) == 0 && settle(1, 1, 0, 0) == 1);
    /* page 2 needs type 4 or 5 */
    assert(settle(2, 1, 4, 0) == 2 && settle(2, 1, 5, 0) == 2 && settle(2, 1, 6, 0) == 0);
    /* pages 3, 5 and 6 need type 5 in game mode 5 */
    assert(settle(3, 1, 5, 5) == 3 && settle(5, 1, 5, 5) == 5 && settle(6, 1, 5, 5) == 6);
    assert(settle(3, 1, 4, 5) == 0 && settle(5, 1, 5, 4) == 0 && settle(6, 1, 6, 5) == 0);
    /* page 4 needs type 6 in game mode 0 */
    assert(settle(4, 1, 6, 0) == 4 && settle(4, 1, 5, 0) == 0 && settle(4, 1, 6, 1) == 0);
    assert(settle(7, 1, 0, 0) == 7);
    /* a page left standing skips the page-0 editor; a cleared one runs it (it asks for the card) */
    reset();
    ST->unknown_00[DOTR_DEBUG_MENU] = 7; pad[0][2] = 2;
    SzDebug_Act();
    assert(card_calls == 0 && preset_calls == 0);

    /* stepping back (0x100) and forward (0x800) skips the pages the same rules refuse */
    assert(step(0x100, 0, 1, 0, 0) == 17 && step(0x800, 17, 1, 0, 0) == 0);
    assert(step(0x100, 12, 0, 0, 0) == 0 && step(0x800, 0, 0, 0, 0) == 12);
    assert(step(0x800, 1, 1, 4, 0) == 2 && step(0x800, 2, 1, 4, 0) == 7 && step(0x800, 1, 1, 0, 0) == 7);
    assert(step(0x800, 2, 1, 5, 5) == 3 && step(0x800, 3, 1, 5, 5) == 5 && step(0x800, 5, 1, 5, 5) == 6);
    assert(step(0x800, 2, 1, 5, 4) == 7 && step(0x800, 1, 1, 6, 0) == 4 && step(0x800, 1, 1, 6, 1) == 7);
    assert(step(0x100, 7, 1, 5, 5) == 6 && step(0x100, 7, 1, 6, 0) == 4 && step(0x100, 7, 1, 0, 0) == 1);
}

static void check_toggles(void)
{
    /* effect menu not available: the button cycles the view mode */
    reset();
    pad[1][0] = 0x800; view_mode = 2;
    SzDebug_Act();
    assert(view_set_calls == 1 && view_set[0] == 0 && view_set[1] == 0 && effect_calls == 0);
    reset();
    pad[1][0] = 0x800; view_mode = 0;
    SzDebug_Act();
    assert(view_set_calls == 1 && view_set[1] == 1 && !ST->unknown_2c.unknown_bit_2);
    /* available: the button switches the effect menu on, which takes over the frame */
    reset();
    ST->unknown_2c.unknown_bit_1 = -1; pad[1][0] = 0x800; hide_result = 0;
    SzDebug_Act();
    assert(view_set_calls == 0 && ST->unknown_2c.unknown_bit_2 && effect_calls == 1 && name_calls == 0);
    pad[1][0] = 0;
    SzDebug_Act();
    assert(effect_calls == 2 && ST->unknown_2c.unknown_bit_2);
    pad[1][0] = 0x800;
    SzDebug_Act();
    assert(effect_calls == 2 && !ST->unknown_2c.unknown_bit_2 && ST->unknown_2c.unknown_bit_1 && name_calls == 1);
    /* available, button not pressed: nothing changes */
    reset();
    ST->unknown_2c.unknown_bit_1 = -1;
    SzDebug_Act();
    assert(view_set_calls == 0 && effect_calls == 0 && !ST->unknown_2c.unknown_bit_2);
    /* the hide bit toggles both ways and leaves the other bits alone */
    reset();
    pad[1][0] = 0x100; ST->unknown_2c.unknown_bit_1 = -1;
    SzDebug_Act();
    assert(ST->unknown_2c.unknown_bit_0 && ST->unknown_2c.unknown_bit_1 && !ST->unknown_2c.unknown_bit_2);
    SzDebug_Act();
    assert(!ST->unknown_2c.unknown_bit_0 && ST->unknown_2c.unknown_bit_1);
}

/* Runs page 0 with the given buttons and returns the number handed to SzModel_Preset (-1: none). */
static int edit(int mask, int no, int digit)
{
    reset();
    pad[0][2] = mask; model_no = no; ST->unknown_00[DOTR_DEBUG_DIGIT] = digit; ST->unknown_00[DOTR_DEBUG_MODEL] = 9;
    SzDebug_Act();
    if (preset_calls == 0) {
        return -1;
    }
    assert(preset_calls == 1 && preset_args[0] == 9 && preset_args[1] == -1);
    assert(card_calls == 1 && card_args[0] == -1 && card_args[1] == 9);
    return preset_args[2];
}

static void check_editor(void)
{
    /* digit up (0x1000 with 0x5000) and down (0x4000 alone), with wrap-around in the digit */
    assert(edit(0x1000, 123, 0) == 223 && edit(0x1000, 123, 1) == 133 && edit(0x1000, 129, 2) == 120);
    assert(edit(0x4000, 123, 0) == 23 && edit(0x4000, 103, 1) == 193 && edit(0x4000, 123, 2) == 122);
    /* whole number up (2) and down (1), modulo 1000 */
    assert(edit(2, 123, 0) == 124 && edit(2, 999, 0) == 0 && edit(1, 123, 0) == 122 && edit(1, 0, 0) == 999);
    assert(edit(0, 123, 0) == -1 && loading_calls == 0 && type_calls == 2);
    /* digit cursor right (0x2000) and left (0x8000), modulo 3 */
    assert(edit(0x2000, 123, 0) == -1 && ST->unknown_00[DOTR_DEBUG_DIGIT] == 1);
    assert(edit(0x2000, 123, 2) == -1 && ST->unknown_00[DOTR_DEBUG_DIGIT] == 0);
    assert(edit(0x8000, 123, 0) == -1 && ST->unknown_00[DOTR_DEBUG_DIGIT] == 2);
    assert(edit(0x8000, 123, 2) == -1 && ST->unknown_00[DOTR_DEBUG_DIGIT] == 1);
    /* number unchanged: model slot up (8) and down (4), modulo 44, then the type is read again */
    reset();
    pad[0][2] = 8; ST->unknown_00[DOTR_DEBUG_MODEL] = 43;
    SzDebug_Act();
    assert(ST->unknown_00[DOTR_DEBUG_MODEL] == 0 && type_calls == 2);
    reset();
    pad[0][2] = 4;
    SzDebug_Act();
    assert(ST->unknown_00[DOTR_DEBUG_MODEL] == 43);
    reset();
    pad[0][2] = 8; ST->unknown_00[DOTR_DEBUG_MODEL] = 5;
    SzDebug_Act();
    assert(ST->unknown_00[DOTR_DEBUG_MODEL] == 6);
    reset();
    pad[0][2] = 4; ST->unknown_00[DOTR_DEBUG_MODEL] = 5;
    SzDebug_Act();
    assert(ST->unknown_00[DOTR_DEBUG_MODEL] == 4);
    /* number changed while the model is loading: nothing is preset */
    reset();
    pad[0][2] = 2; loading_result = 1;
    SzDebug_Act();
    assert(preset_calls == 0 && card_calls == 0 && loading_calls == 1 && type_calls == 1);
    /* number changed: the card record takes the new id and, when there is one, its constant bytes */
    reset();
    pad[0][2] = 2; card_result = &card; card.unknown_3e = 0x77; card.unknown_3f = 0x66;
    SzDebug_Act();
    assert(xax_arg == 124 && card.unknown_3c == 0x107c && const_arg == 0x107c && card.info == 0);
    assert(card.unknown_3e == 0x77 && card.unknown_3f == 0x66 && preset_calls == 1 && preset_args[2] == 124);
    reset();
    pad[0][2] = 2; card_result = &card; const_result = &card_const;
    card_const.unknown_00 = 0x21; card_const.unknown_02 = 0xa5;
    SzDebug_Act();
    assert(card.info == &card_const && card.unknown_3e == 0x21 && card.unknown_3f == 5 && preset_calls == 1);
}

static void check_load(void)
{
    /* either load button, model busy loading: nothing */
    reset();
    pad[0][0] = 0x20; loading_result = 1;
    SzDebug_Act();
    assert(load_calls == 0 && loading_calls == 1);
    /* no button: the loading state is not even asked */
    reset();
    SzDebug_Act();
    assert(load_calls == 0 && loading_calls == 0);
    /* type outside 4..5: loads without asking for the unit; 0x10 selects the last argument */
    reset();
    pad[0][0] = 0x20; ST->unknown_00[DOTR_DEBUG_MODEL] = 3; load_result = 1;
    SzDebug_Act();
    assert(load_calls == 1 && load_args[0] == 3 && load_args[1] == -1 && load_args[2] == 123 && load_args[3] == 0);
    assert(unit_calls == 0 && card_calls == 0 && ctl_mode_calls == 0);
    reset();
    pad[0][0] = 0x10; type_result = 6;
    SzDebug_Act();
    assert(load_calls == 1 && load_args[3] == 1 && unit_calls == 0);
    /* type 4 or 5 without a unit: no load */
    reset();
    pad[0][0] = 0x30; type_result = 4; unit_result = 0; ST->unknown_00[DOTR_DEBUG_MODEL] = 7;
    SzDebug_Act();
    assert(load_calls == 0 && unit_calls == 1 && unit_args[0] == -1 && unit_args[1] == 7);
    /* type 5 with a unit, load refused: no follow-up */
    reset();
    pad[0][0] = 0x30; type_result = 5; card_result = &card; card.info = &card_const;
    SzDebug_Act();
    assert(load_calls == 1 && load_args[3] == 1 && card_calls == 0 && ctl_mode_calls == 0);
    /* load accepted, no card record or no constant record: no follow-up */
    reset();
    pad[0][0] = 0x30; type_result = 5; load_result = 1;
    SzDebug_Act();
    assert(card_calls == 1 && ctl_mode_calls == 0);
    reset();
    pad[0][0] = 0x30; type_result = 5; load_result = 1; card_result = &card;
    SzDebug_Act();
    assert(card_calls == 1 && ctl_mode_calls == 0 && entry_calls == 0);
    /* type 4, or type 5 outside game mode 5: control mode and rate only */
    reset();
    pad[0][0] = 0x30; type_result = 4; mode_result = 5; load_result = 1; card_result = &card; ST->unknown_00[DOTR_DEBUG_MODEL] = 8;
    card.info = &card_const;
    SzDebug_Act();
    assert(entry_calls == 0 && busy_calls == 0 && ctl_mode_calls == 1 && ctl_rate_calls == 1);
    assert(ctl_mode_args[0] == -1 && ctl_mode_args[1] == 8 && ctl_mode_args[2] == 7);
    assert(ctl_rate_args[0] == -1 && ctl_rate_args[1] == 8 && ctl_rate_args[2] == 1 && ctl_rate_args[3] == 0);
    reset();
    pad[0][0] = 0x30; type_result = 5; mode_result = 4; load_result = 1; card_result = &card;
    card.info = &card_const;
    SzDebug_Act();
    assert(entry_calls == 0 && ctl_mode_calls == 1);
    /* type 5 in game mode 5: the battle entry and the busy flag first */
    reset();
    pad[0][0] = 0x30; type_result = 5; mode_result = 5; load_result = 1; card_result = &card;
    card.info = &card_const; card.unknown_44 = 3; card.unknown_45 = 0x12;
    SzDebug_Act();
    assert(entry_calls == 1 && entry_args[0] == 3 && entry_args[1] == 0x312);
    assert(busy_calls == 1 && busy_args[0] == 3 && busy_args[1] == 0x12 && busy_args[2] == 0);
    assert(ctl_mode_calls == 1 && ctl_rate_calls == 1);
}

static void check_deform_and_tail(void)
{
    /* deform and stone buttons act only for types 4 and 5 */
    reset();
    pad[0][0] = 0x6c0; type_result = 6;
    SzDebug_Act();
    assert(deform_calls == 0 && stone_calls == 0);
    reset();
    pad[0][0] = 0x6c0; type_result = 4; ST->unknown_00[DOTR_DEBUG_MODEL] = 2;
    SzDebug_Act();
    assert(deform_calls == 2 && deform_args[0] == 2 && deform_args[1] == 1 && deform_args[2] == 2 && deform_args[3] == 0);
    assert(stone_calls == 1 && stone_args[0] == 2 && stone_args[1] == 1);
    reset();
    pad[0][0] = 0x80; type_result = 5;
    SzDebug_Act();
    assert(deform_calls == 1 && deform_args[1] == 1 && stone_calls == 0);
    reset();
    pad[0][0] = 0x40; type_result = 5;
    SzDebug_Act();
    assert(deform_calls == 1 && deform_args[1] == 0 && stone_calls == 0);
    reset();
    pad[0][0] = 0x200; type_result = 5;
    SzDebug_Act();
    assert(deform_calls == 0 && stone_calls == 1);

    /* hidden: the closing queries are skipped */
    reset();
    SzDebug_Act();
    assert(name_calls == 0 && get_deform_calls == 0);
    /* shown: the type's string is passed on; the state queries stop at the first that decides */
    reset();
    hide_result = 0; type_result = 6; ST->unknown_00[DOTR_DEBUG_DIGIT] = 2;
    SzDebug_Act();
    assert(name_calls == 1 && name_arg == name_storage[6] && unit_calls == 0);
    assert(ready_calls == 1 && loading_calls == 1 && disp_calls == 1 && get_deform_calls == 1);
    reset();
    hide_result = 0; ready_result = 1;
    SzDebug_Act();
    assert(ready_calls == 1 && loading_calls == 0 && disp_calls == 0 && get_deform_calls == 1);
    reset();
    hide_result = 0; ST->unknown_00[DOTR_DEBUG_MENU] = 12; loading_result = 1;
    SzDebug_Act();
    assert(ready_calls == 1 && loading_calls == 1 && disp_calls == 0 && get_deform_calls == 1);
    reset();
    hide_result = 0; ST->unknown_00[DOTR_DEBUG_MENU] = 12; standby_result = 0;
    SzDebug_Act();
    assert(ready_calls == 1 && loading_calls == 1 && disp_calls == 0 && get_deform_calls == 1);
    /* type 4 or 5 without a unit: no state queries */
    reset();
    hide_result = 0; type_result = 5; unit_result = 0;
    SzDebug_Act();
    assert(name_arg == name_storage[5] && unit_calls == 1 && ready_calls == 0 && get_deform_calls == 1);
    reset();
    hide_result = 0; type_result = 4;
    SzDebug_Act();
    assert(unit_calls == 1 && ready_calls == 1 && disp_calls == 1);
}

int main(void)
{
    check_menu_rules();
    check_toggles();
    check_editor();
    check_load();
    check_deform_and_tail();
    check_effect_page();
    return 0;
}
