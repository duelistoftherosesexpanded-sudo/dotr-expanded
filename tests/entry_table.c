/* Synthetic host checks for the 1,024-entry table initializer and lookups. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/entry_table.h"

struct DotrEntryTableEntry gSzText_aItem[1024];
unsigned char gSzText_tl[16];
static int clear_calls;

/* Assumed to fill bytes like memset. */
void *RhMem_FillMemory(void *destination, int value, int byte_count)
{
    assert(destination == gSzText_tl && value == 0 && byte_count == 6);
    clear_calls++;
    return memset(destination, value, (size_t)byte_count);
}

static void use(int index, int group, int id, int word)
{
    gSzText_aItem[index].pointerToTextureSource = word;
    gSzText_aItem[index].group = (short)group;
    gSzText_aItem[index].id = (short)id;
}

static int name_calls, drop_calls, drop_args[2];
static const char *name_arg;

int SzMem_StrDup(const char *name)
{
    name_calls++;
    name_arg = name;
    return 0x5151;
}

void SzMem_Free(int address)
{
    assert(drop_calls < 2);
    drop_args[drop_calls++] = address;
}

/* 0x2029b0: one assertion per filter outcome (unused, group and id wildcard
 * and mismatch, bit 31) and per release branch (word at +4, flag_2). */
static void check_release(void)
{
    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
    drop_calls = 0;
    assert(SzText_Free(-1, -1) == 0 && drop_calls == 0);

    use(3, 2, 6, 0x111);
    use(4, 2, 7, 0x222);
    use(1023, 5, 6, 0x333);
    use(8, 2, 6, (int)0x80000001u);
    gSzText_aItem[3].fileNameItCameFrom = 0x444;
    gSzText_aItem[4].flag_2 = -1;
    assert(SzText_Free(4, 6) == 0 && SzText_Free(2, 8) == 0 && drop_calls == 0);

    /* Group and id given: only entry 3; its +4 word is released, +0 is not (flag_2 clear). */
    assert(SzText_Free(2, 6) == 1 && drop_calls == 1 && drop_args[0] == 0x444);
    assert(gSzText_aItem[3].pointerToTextureSource == 0 && gSzText_aItem[3].fileNameItCameFrom == 0);
    assert(gSzText_aItem[4].pointerToTextureSource == 0x222 && gSzText_aItem[8].pointerToTextureSource == (int)0x80000001u);

    /* Group wildcard: entry 4 by id; flag_2 set releases the first word only. */
    drop_calls = 0;
    assert(SzText_Free(-1, 7) == 1 && drop_calls == 1 && drop_args[0] == 0x222);
    assert(gSzText_aItem[4].pointerToTextureSource == 0 && gSzText_aItem[1023].pointerToTextureSource == 0x333);

    /* Id wildcard: the last entry by group; neither word is released. */
    drop_calls = 0;
    assert(SzText_Free(5, -1) == 1 && drop_calls == 0 && gSzText_aItem[1023].pointerToTextureSource == 0);

    /* Only the bit-31 entry is left: never released. */
    assert(SzText_Free(-1, -1) == 0 && gSzText_aItem[8].pointerToTextureSource == (int)0x80000001u);
}

/* 0x202810: one assertion set per rejected bound, for the found, bit-31 and
 * absent lookups, for a full table and for a null and non-null name. */
static void check_store(void)
{
    static const char name[] = "n";
    int i;

    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
    name_calls = drop_calls = 0;
    assert(SzText_AddFile(-1, 0, name, 1) == 0 && SzText_AddFile(10, 0, name, 1) == 0);
    assert(SzText_AddFile(0, -1, name, 1) == 0 && name_calls == 0 && drop_calls == 0);

    /* Absent: the first free entry is taken; boundary group 9 and id 0. */
    gSzText_aItem[0].pointerToTextureSource = 1;
    gSzText_aItem[1].flag_0 = -1;
    gSzText_aItem[1].flag_1 = -1;
    gSzText_aItem[1].flag_2 = -1;
    assert(SzText_AddFile(9, 0, name, 77) == 1);
    assert(name_calls == 1 && name_arg == name && drop_calls == 0);
    assert(gSzText_aItem[1].pointerToTextureSource == 77 && gSzText_aItem[1].fileNameItCameFrom == 0x5151);
    assert(gSzText_aItem[1].flag_0 == 0 && gSzText_aItem[1].flag_1 == 0 && gSzText_aItem[1].flag_2 == 0);
    assert(gSzText_aItem[1].group == 9 && gSzText_aItem[1].id == 0);

    /* Found: 0x2029b0 releases the entry (its +4 word goes to 0x217b50) and
     * the same entry is rewritten; null name. */
    assert(SzText_AddFile(9, 0, 0, 78) == 1);
    assert(name_calls == 1 && drop_calls == 1 && drop_args[0] == 0x5151);
    assert(gSzText_aItem[1].pointerToTextureSource == 78 && gSzText_aItem[1].fileNameItCameFrom == 0);
    assert(gSzText_aItem[2].pointerToTextureSource == 0);

    /* Found with bit 31 set: treated as absent, so a free entry is taken. */
    gSzText_aItem[1].pointerToTextureSource = (int)0x80000000u;
    assert(SzText_AddFile(9, 0, 0, 79) == 1 && drop_calls == 1);
    assert(gSzText_aItem[2].pointerToTextureSource == 79 && gSzText_aItem[2].group == 9 && gSzText_aItem[2].id == 0);

    /* Full table: nothing is stored. */
    for (i = 0; i < 1024; i++) {
        gSzText_aItem[i].pointerToTextureSource = 1;
        gSzText_aItem[i].group = 0;
        gSzText_aItem[i].id = 0;
    }
    assert(SzText_AddFile(3, 4, name, 80) == 0 && name_calls == 1 && drop_calls == 1);
    gSzText_aItem[1023].pointerToTextureSource = 0;
    assert(SzText_AddFile(3, 4, name, 80) == 1 && gSzText_aItem[1023].pointerToTextureSource == 80);
    assert(gSzText_aItem[1023].group == 3 && gSzText_aItem[1023].id == 4 && name_calls == 2);
}

/* SzText_LoadFileEx (0x202220). */
char data_002bd0c1[256];
char data_002d074b[] = "ENV";
char data_002d0760[] = "%s/%s";
static int load_calls, load_result, load_args[7], copy_calls, format_calls, env_calls;
static char load_path[128];
static void *load_cookie;
static void (*load_callback)(void *record, int completed);
void lSzText_LoadFileCB(void *record, int completed) { (void)record; (void)completed; }
char *func_0019bdd8(char *destination, const char *source)
{
    copy_calls++;
    return strcpy(destination, source);
}
int func_0019b648(char *destination, const char *format, ...)
{
    va_list list;
    const char *a, *b;

    assert(format == data_002d0760);
    va_start(list, format);
    a = va_arg(list, const char *);
    b = va_arg(list, const char *);
    va_end(list);
    format_calls++;
    strcpy(destination, a);
    strcat(destination, "/");
    strcat(destination, b);
    return 0;
}
char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002d074b);
    env_calls++;
    return "root";
}
void *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
                              int unknown_010, void (*callback)(void *record, int completed), void *unknown_01c)
{
    load_calls++;
    load_args[0] = type;
    load_args[1] = priority;
    load_args[2] = unknown_014;
    load_args[3] = byte_size;
    load_args[4] = byte_offset;
    load_args[5] = unknown_010;
    strcpy(load_path, name);
    load_callback = callback;
    load_cookie = unknown_01c;
    return load_result ? &load_result : 0;
}

static void load_reset(void)
{
    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
    load_calls = copy_calls = format_calls = env_calls = name_calls = 0;
    load_result = 1;
}

static void check_load_file(void)
{
    static const char drive[] = "c:file";
    static const char plain[] = "cfile";
    static const char digit[] = "1:file";
    struct DotrEntryTableEntry *e;
    int i;

    data_002bd0c1['c'] = 2;
    data_002bd0c1['1'] = 4;
    /* Group and id bounds: nothing is called. */
    load_reset();
    assert(SzText_LoadFileEx(-1, 0, drive, 1, 2, 3, 4) == 0 && SzText_LoadFileEx(10, 0, drive, 1, 2, 3, 4) == 0);
    assert(SzText_LoadFileEx(0, -1, drive, 1, 2, 3, 4) == 0);
    assert(load_calls == 0 && name_calls == 0);
    /* An entry for the pair already exists. */
    use(1023, 9, 7, 5);
    assert(SzText_LoadFileEx(9, 7, drive, 1, 2, 3, 4) == 0 && load_calls == 0);
    /* No free entry. */
    for (i = 0; i < 1024; ++i)
        use(i, 1, i, 1);
    assert(SzText_LoadFileEx(2, 0, drive, 1, 2, 3, 4) == 0 && load_calls == 0);
    /* Drive-style name: copied as it is. The first free entry is taken; a zero address sets flag 2. */
    load_reset();
    use(0, 3, 3, 1);
    e = &gSzText_aItem[1];
    e->flag_0 = -1;
    e->flag_1 = -1;
    assert(SzText_LoadFileEx(9, 0, drive, 0, 22, 33, 44) == 1);
    assert(copy_calls == 1 && format_calls == 0 && env_calls == 0 && load_calls == 1 && name_calls == 1);
    assert(strcmp(load_path, "c:file") == 0 && name_arg == drive);
    assert(load_args[0] == 0x1009 && load_args[1] == 0 && load_args[2] == 0 && load_args[3] == 22);
    assert(load_args[4] == 33 && load_args[5] == 44 && load_callback == lSzText_LoadFileCB && load_cookie == e);
    assert(e->pointerToTextureSource == (int)0x80000000u && e->fileNameItCameFrom == 0x5151);
    assert(e->flag_0 == 0 && e->flag_1 == 0 && e->flag_2 == -1 && e->group == 9 && e->id == 0);
    /* Other names go through the format with the environment value; a non-zero address clears flag 2. */
    load_reset();
    e = &gSzText_aItem[0];
    e->flag_2 = -1;
    assert(SzText_LoadFileEx(0, 0x7fff, plain, 0x100000, 0, 0, 0) == 1);
    assert(copy_calls == 0 && format_calls == 1 && env_calls == 1 && strcmp(load_path, "root/cfile") == 0);
    assert(load_args[0] == 0x1000 && load_args[2] == 0x100000 && load_cookie == e);
    assert(e->flag_2 == 0 && e->group == 0 && e->id == 0x7fff);
    load_reset();
    assert(SzText_LoadFileEx(4, 5, digit, 1, 0, 0, 0) == 1);
    assert(copy_calls == 0 && format_calls == 1 && strcmp(load_path, "root/1:file") == 0);
    /* A refused request leaves the entry free. */
    load_reset();
    load_result = 0;
    assert(SzText_LoadFileEx(4, 5, drive, 1, 0, 0, 0) == 0);
    assert(load_calls == 1 && name_calls == 0 && gSzText_aItem[0].pointerToTextureSource == 0 && gSzText_aItem[0].group == 0);
    memset(gSzText_aItem, 0, sizeof(gSzText_aItem));
}

int main(void)
{
    int i;

    /* 0x2007d0: six header bytes cleared, every entry reset. */
    memset(gSzText_aItem, 0x5a, sizeof(gSzText_aItem));
    memset(gSzText_tl, 0x5a, sizeof(gSzText_tl));
    SzText_Init();
    assert(clear_calls == 1 && gSzText_tl[5] == 0 && gSzText_tl[6] == 0x5a);
    for (i = 0; i < 1024; i++) {
        struct DotrEntryTableEntry *entry = &gSzText_aItem[i];
        assert(entry->pointerToTextureSource == 0 && entry->fileNameItCameFrom == 0);
        assert(entry->flag_0 == 0 && entry->flag_1 == 0 && entry->flag_2 == 0);
        assert(entry->group == -1 && entry->id == -1);
    }

    /* Argument limits: groups 0..9, nonnegative ids. */
    use(5, 9, 7, 1);
    assert(SzText_GetItem(9, 7) == &gSzText_aItem[5]);
    assert(SzText_GetItem(-1, 7) == 0 && SzText_GetItem(10, 7) == 0 && SzText_GetItem(9, -1) == 0);

    /* Unused entries never match, even with equal keys; the first used match wins. */
    use(2, 3, 4, 0);
    use(900, 3, 4, 1);
    use(1023, 3, 4, 1);
    assert(SzText_GetItem(3, 4) == &gSzText_aItem[900]);
    assert(SzText_GetItem(3, 5) == 0 && SzText_GetItem(4, 4) == 0);
    use(900, 0, 0, 0);
    assert(SzText_GetItem(3, 4) == &gSzText_aItem[1023]);
    use(0, 0, 0, -5);
    assert(SzText_GetItem(0, 0) == &gSzText_aItem[0]);
    /* A stored negative group can never equal a valid argument. */
    use(1, -1, 3, 1);
    assert(SzText_GetItem(0x1fff, 3) == 0);

    /* 0x200930: the same search, rejected when bit 31 of the first word is set. */
    assert(SzText_GetReadyItem(9, 7) == &gSzText_aItem[5]);
    assert(SzText_GetReadyItem(0, 0) == 0); /* first word -5 */
    use(5, 9, 7, (int)0x80000000u);
    assert(SzText_GetItem(9, 7) == &gSzText_aItem[5] && SzText_GetReadyItem(9, 7) == 0);
    use(5, 9, 7, 0x7fffffff);
    assert(SzText_GetReadyItem(9, 7) == &gSzText_aItem[5]);
    assert(SzText_GetReadyItem(10, 7) == 0 && SzText_GetReadyItem(9, -1) == 0 && SzText_GetReadyItem(8, 8) == 0);

    check_release();
    check_store();
    check_load_file();
    puts("entry_table: ok");
    return 0;
}
