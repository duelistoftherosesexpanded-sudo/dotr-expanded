/* Synthetic host tests for the effect_module group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/effect_module.h"

struct DotrPool gSzEffect_el_pool;
unsigned char data_0031bea8;
char data_002cfbb0[] = "format";
char data_002cfbc6[] = "label";

static void *released[4];
static int release_calls;
static int format_calls;
static char format_letter;
static char *format_destination;
static int convert_calls;
static int convert_result;
static int request_calls;
static void *request_result;
static void *request_address;
static int request_offset;
static int request_slot;

void SzMem_Free(void *pointer)
{
    assert(release_calls < 4);
    released[release_calls++] = pointer;
}

char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002cfbc6);
    return data_002cfbc6 + 1;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    assert(format == data_002cfbb0 && va_arg(args, char *) == data_002cfbc6 + 1);
    format_letter = (char)va_arg(args, int);
    va_end(args);
    format_calls++;
    format_destination = destination;
    return 0;
}

int SzDuel_Xax2New(int value)
{
    convert_calls++;
    assert(value == 100);
    return convert_result;
}

void lSzEffect_LoadOverlayCB(struct DotrPoolOverlayRecord *record, int completed)
{
    (void)record;
    (void)completed;
}

void *func_001fecb0_module(int type, int priority, char *name, void *address, int byte_size, int byte_offset,
                           int unknown_010, void (*callback)(struct DotrPoolOverlayRecord *record, int completed),
                           int slot)
{
    request_calls++;
    assert(type == 0x800 && priority == 0 && name == format_destination);
    assert(byte_size == 0x8000 && unknown_010 == 0x8000 && callback == lSzEffect_LoadOverlayCB);
    request_address = address;
    request_offset = byte_offset;
    request_slot = slot;
    return request_result;
}

/* Every slot ready (state 2) with a module no test asks for. */
static void reset(void)
{
    int i;

    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    for (i = 0; i < 32; i++) {
        gSzEffect_el_pool.unknown_000[i].unknown_bits_0_2 = 2;
        gSzEffect_el_pool.unknown_000[i].unknown_bits_3_5 = 5;
        gSzEffect_el_pool.unknown_000[i].unknown_bits_6_15 = 900;
    }
    data_0031bea8 = 0;
    release_calls = format_calls = convert_calls = request_calls = 0;
    format_letter = 0;
    convert_result = 42;
    request_result = &request_result;
}

static void set(int slot, int state, int type, int id)
{
    gSzEffect_el_pool.unknown_000[slot].unknown_bits_0_2 = state;
    gSzEffect_el_pool.unknown_000[slot].unknown_bits_3_5 = type;
    gSzEffect_el_pool.unknown_000[slot].unknown_bits_6_15 = id;
}

static void check_already_loaded(void)
{
    struct DotrPoolEntry8 *e = &gSzEffect_el_pool.unknown_000[9];

    reset();
    set(9, 2, 1, 100);
    assert(SzEffect_LoadModule(1, 100, 0) == e && e->unknown_bits_0_2 == 2);
    set(9, 3, 1, 100);
    assert(SzEffect_LoadModule(1, 100, 0) == e && e->unknown_bits_0_2 == 2);
    /* The third argument is not looked at on this path. */
    assert(SzEffect_LoadModule(1, 100, 1) == e);
    assert(format_calls == 0 && request_calls == 0 && release_calls == 0);
    /* A slot still loading (1) is not answered; with no free slot nothing is. */
    set(9, 1, 1, 100);
    assert(SzEffect_LoadModule(1, 100, 0) == 0 && data_0031bea8 == 1);
}

static void check_free_slot(void)
{
    struct DotrPoolEntry8 *e = &gSzEffect_el_pool.unknown_000[5];
    int block;

    reset();
    set(5, 0, 0, 0);
    e->unknown_04 = &block;
    assert(SzEffect_LoadModule(1, 100, 0) == e);
    assert(release_calls == 1 && released[0] == &block && e->unknown_04 == 0);
    assert(format_calls == 1 && format_letter == 'B' && convert_calls == 1 && request_calls == 1);
    assert(request_address == (void *)(0x1ee0000L + 5 * 0x8000L) && request_offset == 42 << 15 && request_slot == 5);
    assert(e->unknown_bits_0_2 == 1 && e->unknown_bits_3_5 == 1 && e->unknown_bits_6_15 == 100);
    assert(data_0031bea8 == 0);

    reset();
    set(0, 0, 0, 0);
    assert(SzEffect_LoadModule(2, 100, 0) == &gSzEffect_el_pool.unknown_000[0]);
    assert(format_letter == 'M' && convert_calls == 1 && request_offset == 42 << 15 && request_slot == 0);
    assert(request_address == (void *)0x1ee0000L && release_calls == 0);

    /* Table 0 uses the id itself as the position in the file; the largest id is 853. */
    reset();
    set(31, 0, 0, 0);
    e = &gSzEffect_el_pool.unknown_000[31];
    assert(SzEffect_LoadModule(0, 853, 0) == e);
    assert(format_letter == 'C' && convert_calls == 0 && request_offset == 853 << 15 && request_slot == 31);
    assert(e->unknown_bits_0_2 == 1 && e->unknown_bits_3_5 == 0 && e->unknown_bits_6_15 == 853);
}

static void check_refusals(void)
{
    struct DotrPoolEntry8 *e = &gSzEffect_el_pool.unknown_000[5];
    int block;

    /* Third argument set: the slot's block is still released first. */
    reset();
    set(5, 0, 0, 0);
    e->unknown_04 = &block;
    assert(SzEffect_LoadModule(1, 100, 1) == 0);
    assert(release_calls == 1 && e->unknown_04 == 0 && format_calls == 0 && e->unknown_bits_0_2 == 0);

    reset();
    set(5, 0, 0, 0);
    assert(SzEffect_LoadModule(0, 854, 0) == 0 && SzEffect_LoadModule(0, -1, 0) == 0);
    assert(format_calls == 0 && request_calls == 0);

    /* The converted number of tables 1 and 2 must be below 683. */
    convert_result = 683;
    assert(SzEffect_LoadModule(1, 100, 0) == 0 && format_calls == 1 && request_calls == 0);
    convert_result = 682;
    assert(SzEffect_LoadModule(2, 100, 0) == e && request_offset == 682 << 15);

    /* A table number above 2 gets as far as the file name. */
    reset();
    set(5, 0, 0, 0);
    assert(SzEffect_LoadModule(3, 100, 0) == 0);
    assert(format_calls == 1 && format_letter == '?' && convert_calls == 0 && request_calls == 0);

    /* The request refused: the slot stays empty. */
    request_result = 0;
    assert(SzEffect_LoadModule(1, 100, 0) == 0 && request_calls == 1 && e->unknown_bits_0_2 == 0);
}

static void check_reuse(void)
{
    struct DotrPoolEntry8 *e;

    /* No empty slot: the first kept slot (3) counting from the rotating start, which then advances. */
    reset();
    set(2, 3, 0, 7);
    set(31, 3, 0, 8);
    data_0031bea8 = 30;
    e = &gSzEffect_el_pool.unknown_000[31];
    assert(SzEffect_LoadModule(1, 100, 0) == e && request_slot == 31 && data_0031bea8 == 31);
    assert(e->unknown_bits_0_2 == 1 && e->unknown_bits_6_15 == 100);
    e = &gSzEffect_el_pool.unknown_000[2];
    assert(SzEffect_LoadModule(0, 9, 0) == e && request_slot == 2 && data_0031bea8 == 0);
    assert(request_address == (void *)(0x1ee0000L + 2 * 0x8000L));

    /* Nothing empty and nothing kept. */
    assert(SzEffect_LoadModule(0, 10, 0) == 0 && data_0031bea8 == 1 && request_calls == 2);
}

int main(void)
{
    check_already_loaded();
    check_free_slot();
    check_refusals();
    check_reuse();
    puts("effect_module: ok");
    return 0;
}
