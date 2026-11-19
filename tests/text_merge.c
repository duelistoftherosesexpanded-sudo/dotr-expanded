/* Synthetic host tests for the text_merge group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/text_merge.h"

struct DotrTextMergeItem gSzText_aItem[0x400];
int anOfs_TEXETC[4] = { 0x100, 0x900, 0x2000, 0x2800 };
static char file_names[10][8] = { "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9" };
char *gSzText_apchMerge[10] = {
    file_names[0], file_names[1], file_names[2], file_names[3], file_names[4],
    file_names[5], file_names[6], file_names[7], file_names[8], file_names[9]
};
char data_002d074b[] = "ENV";
char data_002d0750[] = "%s/%s";
static unsigned char env_dir[] = "dir";

enum { F_PRESET = 1, F_DEL, F_FREE, F_LOAD, F_EXCH };

struct call {
    int fn;
    int a[7];
    char name[32];
};

static struct call trace[256];
static int ntrace, nformat, load_ret, leader, deck_ids[40], const_ids[17];

static struct call *add(int fn, int a, int b, int c)
{
    struct call *k;

    assert(ntrace < 256);
    k = &trace[ntrace++];
    memset(k, 0, sizeof *k);
    k->fn = fn;
    k->a[0] = a;
    k->a[1] = b;
    k->a[2] = c;
    return k;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    assert(0);
    return NULL;
}

unsigned char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002d074b);
    return env_dir;
}
int func_0019b648(char *destination, const char *format, ...)
{
    va_list ap;
    int n;

    assert(format == data_002d0750);
    nformat++;
    va_start(ap, format);
    n = vsprintf(destination, format, ap);
    va_end(ap);
    return n;
}
int SzModel_Preset(int index, int value, int unknown) { add(F_PRESET, index, value, unknown); return 0; }
int SzDuel_DelUnit(int side, int index) { add(F_DEL, side, index, 0); return 0; }
int SzText_Free(int group, int id) { add(F_FREE, group, id, 0); return 0; }
int SzText_LoadFileEx(int group, int id, char *name, int address, int byte_size, int byte_offset, int read_size)
{
    struct call *k = add(F_LOAD, group, id, address);

    k->a[3] = byte_size;
    k->a[4] = byte_offset;
    k->a[5] = read_size;
    strcpy(k->name, name);
    return load_ret;
}
int SzData_GetCurrentDeck(int side) { return 0x40 + side; }
int SzData_GetLeader(int side, int deck)
{
    assert(deck == 0x40 + side || deck < 3);
    return deck < 3 ? 0x300 + side * 3 + deck : leader;
}
int SzDuel_InqDeck(int index, int position)
{
    assert(index == 0 || index == 1);
    return deck_ids[position];
}
int SzData_ExchSpecial(int side, int id)
{
    add(F_EXCH, side, id, 0);
    return id == 0x2b0 ? 0x2c0 : id;
}
int SzData_GetConstDeck(int row, int column)
{
    assert(column == 0);
    return const_ids[row];
}

static void reset(void)
{
    int i;

    memset(gSzText_aItem, 0, sizeof gSzText_aItem);
    ntrace = 0;
    nformat = 0;
    load_ret = 0;
    leader = 0x2b0;
    for (i = 0; i < 40; i++) {
        deck_ids[i] = 400 - i * 3;
    }
    for (i = 0; i < 17; i++) {
        const_ids[i] = 50 - i;
    }
}

static void put_item(int slot, int cat, int type)
{
    gSzText_aItem[slot].ptr = &gSzText_aItem[slot];
    gSzText_aItem[slot].cat = (short)cat;
    gSzText_aItem[slot].type = (short)type;
}

static void check_range_and_mode0(void)
{
    int k;

    reset();
    assert(SzText_LoadMerge(-1, 1) == 0 && SzText_LoadMerge(18, 1) == 0 && ntrace == 0 && nformat == 0);

    /* Mode 0 with only its own item present and no force: nothing to do. */
    reset();
    put_item(5, 0, 0);
    assert(SzText_LoadMerge(0, 0) == 1 && ntrace == 0);
    /* Another group's item makes it reload; a free entry (null pointer) does not count. */
    reset();
    put_item(5, 0, 0);
    put_item(9, 0, 6);
    gSzText_aItem[7].cat = 0;
    gSzText_aItem[7].type = 1;
    load_ret = 4;
    assert(SzText_LoadMerge(0, 0) == 4);
    assert(count(F_PRESET) == 82 && count(F_DEL) == 20 && count(F_FREE) == 15 && count(F_LOAD) == 9);
    assert(nth(F_PRESET, 0)->a[0] == 0 && nth(F_PRESET, 0)->a[1] == -1 && nth(F_PRESET, 0)->a[2] == 999);
    assert(nth(F_PRESET, 81)->a[0] == 0x53);
    assert(nth(F_DEL, 19)->a[0] == 1 && nth(F_DEL, 19)->a[1] == 10);
    assert(nth(F_FREE, 5)->a[0] == 8 && nth(F_FREE, 5)->a[1] == -1);
    for (k = 0; k <= 8; k++) {
        struct call *c = nth(F_LOAD, k);
        int records = k < 8 ? 100 : 71;

        assert(c->a[0] == 0 && c->a[1] == k * 0x10 && c->a[2] == 0xb1b000 + k * 0x1a9640);
        assert(c->a[3] == records * 0x4800 && c->a[4] == k * 0x1c2000 && c->a[5] == records * 0x4800);
        assert(strcmp(c->name, "dir/f0") == 0);
        assert(nth(F_FREE, 6 + k)->a[0] == 0 && nth(F_FREE, 6 + k)->a[1] == k * 0x10);
    }
}

static void check_fixed_files(void)
{
    struct call *c;

    reset();
    load_ret = 7;
    assert(SzText_LoadMerge(1, 0) == 7 && count(F_FREE) == 7);
    c = nth(F_LOAD, 0);
    assert(c->a[0] == 0 && c->a[1] == 1 && c->a[2] == 0xb1b000 && c->a[3] == -1 && c->a[4] == 0 && c->a[5] == -1);
    assert(strcmp(c->name, "dir/f1") == 0);

    reset();
    put_item(0, 0, 3);
    assert(SzText_LoadMerge(11, 0) == 1 && ntrace == 0);
    assert(SzText_LoadMerge(11, 1) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 3 && c->a[2] == 0x912000 && c->a[3] == 0x11e800 && c->a[5] == -1);
    assert(strcmp(c->name, "dir/f3") == 0);

    reset();
    assert(SzText_LoadMerge(12, 0) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 4 && c->a[2] == 0xa30800 && c->a[3] == 0x44800 && strcmp(c->name, "dir/f4") == 0);
    reset();
    assert(SzText_LoadMerge(13, 0) == 0 && count(F_FREE) == 5);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 5 && c->a[2] == 0xb63540 && c->a[3] == 0x16a800 && strcmp(c->name, "dir/f5") == 0);
    reset();
    assert(SzText_LoadMerge(14, 0) == 0 && count(F_PRESET) == 32 && count(F_DEL) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 6 && c->a[2] == 0xccdd40 && c->a[3] == 0x47f000 && strcmp(c->name, "dir/f6") == 0);

    /* 15..17: byte ranges between the anOfs_TEXETC neighbours. */
    reset();
    assert(SzText_LoadMerge(15, 0) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 7 && c->a[2] == 0xa75000 && c->a[3] == 0x800 && c->a[4] == 0x100 && c->a[5] == 0x800);
    assert(strcmp(c->name, "dir/f7") == 0 && nth(F_FREE, 3)->a[0] == 9);
    reset();
    assert(SzText_LoadMerge(16, 0) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 8 && c->a[3] == 0x1700 && c->a[4] == 0x900 && strcmp(c->name, "dir/f8") == 0);
    reset();
    put_item(3, 0, 9);
    assert(SzText_LoadMerge(17, 0) == 1 && ntrace == 0);
    put_item(4, 0, 8);
    assert(SzText_LoadMerge(17, 0) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 9 && c->a[3] == 0x800 && c->a[4] == 0x2000 && strcmp(c->name, "dir/f9") == 0);
}

static void check_decks(void)
{
    struct call *c;
    int i;

    /* Mode 2: 40 deck cards and the leader, sorted, each loaded unless present. */
    reset();
    put_item(20, 1, 400 - 39 * 3); /* the smallest id: already loaded */
    assert(SzText_LoadMerge(2, 0) == 0);
    assert(count(F_EXCH) == 41 && count(F_LOAD) == 40);
    for (i = 0; i < 40; i++) {
        c = nth(F_LOAD, i);
        assert(c->a[0] == 1 && c->a[2] == 0xb1b000 + (0xdf + 1 + i) * 0x4410);
        assert(c->a[3] == 0x4800 && c->a[4] == c->a[1] * 0x4800 && c->a[5] == 0x4800);
    }
    assert(nth(F_LOAD, 0)->a[1] == 400 - 38 * 3 && nth(F_LOAD, 38)->a[1] == 400);
    assert(nth(F_LOAD, 39)->a[1] == 0x2c0); /* the leader 0x2b0, exchanged */
    assert(strcmp(nth(F_LOAD, 0)->name, "dir/f0") == 0);

    /* Side 1 starts at record 0xdf + 41; 999 is never exchanged. */
    reset();
    deck_ids[0] = 999;
    assert(SzText_LoadMerge(3, 0) == 0);
    assert(count(F_EXCH) == 40 && nth(F_LOAD, 0)->a[2] == 0xb1b000 + (0xdf + 41) * 0x4410);
    assert(nth(F_LOAD, 40)->a[1] == 999);

    /* Mode 10: the 17 constant-deck cards, sorted, from record 0. */
    reset();
    load_ret = 2;
    assert(SzText_LoadMerge(10, 0) == 2 && count(F_FREE) == 3 && count(F_LOAD) == 17);
    assert(nth(F_LOAD, 0)->a[1] == 34 && nth(F_LOAD, 0)->a[2] == 0xb1b000);
    assert(nth(F_LOAD, 16)->a[1] == 50 && nth(F_LOAD, 16)->a[2] == 0xb1b000 + 16 * 0x4410);
}

static void check_leaders(void)
{
    struct call *c;

    /* Mode 5: side 0's leader picture; an exchanged id reads from (id - 0xab) << 12. */
    reset();
    load_ret = 3;
    assert(SzText_LoadMerge(5, 0) == 3);
    assert(nth(F_FREE, 0)->a[0] == 2 && nth(F_FREE, 0)->a[1] == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[0] == 2 && c->a[1] == 0 && c->a[2] == 0xb19000 && c->a[3] == 0x1000);
    assert(c->a[4] == (0x2c0 - 0xab) << 12 && c->a[5] == 0x1000 && strcmp(c->name, "dir/f2") == 0);
    reset();
    leader = 0x50;
    assert(SzText_LoadMerge(6, 0) == 0);
    c = nth(F_LOAD, 0);
    assert(c->a[1] == 1 && c->a[2] == 0xb19000 + 0x1000 && c->a[4] == 0x50 << 12);
    reset();
    leader = 999;
    assert(SzText_LoadMerge(5, 0) == 0 && count(F_FREE) == 1 && count(F_LOAD) == 0);
    reset();
    put_item(1, 2, 0);
    assert(SzText_LoadMerge(5, 0) == 1 && ntrace == 0);

    /* Mode 4 runs 5 then 6 and ORs their results. */
    reset();
    load_ret = 1;
    assert(SzText_LoadMerge(4, 0) == 1 && count(F_LOAD) == 2);
    assert(nth(F_LOAD, 0)->a[1] == 0 && nth(F_LOAD, 1)->a[1] == 1);

    /* Modes 8/9 load a side's three leaders as items 10.. / 20..; mode 7 runs both. */
    reset();
    assert(SzText_LoadMerge(8, 0) == 0 && count(F_FREE) == 7 && count(F_LOAD) == 3);
    c = nth(F_LOAD, 2);
    assert(c->a[0] == 2 && c->a[1] == 12 && c->a[2] == 0xa75000 + (2 << 12) && c->a[4] == 0x302 << 12);
    reset();
    assert(SzText_LoadMerge(7, 0) == 0 && count(F_LOAD) == 6);
    c = nth(F_LOAD, 3);
    assert(c->a[1] == 20 && c->a[2] == 0xa75000 + (3 << 12) && c->a[4] == 0x303 << 12);
}

int main(void)
{
    check_range_and_mode0();
    check_fixed_files();
    check_decks();
    check_leaders();
    return 0;
}
