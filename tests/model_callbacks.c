/* Synthetic host tests for the model_callbacks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "dotr/ee/model_callbacks.h"

struct DotrObject *gSzModel_ahModel[84];
char *gSzModel_apchBase[8];
char data_002cfa17[] = "DBDIR";
char data_002cfb94[] = "%s/%s/%s%%d-%%d%%c";

static int clean_calls, clean_arg, bind_calls, texture_calls, texture_arg, effect_calls, effect_arg;
static int motion_calls, motion_types[8], motion_slots[8], group_calls, group_args[2], label_calls;
static void *bind_arg;
static char motion_names[8][64];

int lSzModel_Clean(int index) { clean_calls++; clean_arg = index; return 0; }
int RhModel_BindAddress(void *header) { bind_calls++; bind_arg = header; return 0; }
int SzModel_LoadTextureFile(int index) { texture_calls++; texture_arg = index; return 0; }
int SzModel_LoadEffectTextureFile(int index) { effect_calls++; effect_arg = index; return 1; }
void lSzModel_LoadCB(int index) { (void)index; }

char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002cfa17);
    label_calls++;
    return "root";
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;
    int n;

    va_start(args, format);
    n = vsnprintf(destination, 64, format, args);
    va_end(args);
    return n;
}

int lSzModel_LoadMotion(int type, int slot, char *name)
{
    assert(motion_calls < 8);
    motion_types[motion_calls] = type;
    motion_slots[motion_calls] = slot;
    strcpy(motion_names[motion_calls++], name);
    return 1;
}

int SzFile_EntryGroup(int selector, int value)
{
    group_calls++;
    group_args[0] = selector;
    group_args[1] = value;
    return 0;
}

static void reset(void)
{
    clean_calls = bind_calls = texture_calls = effect_calls = motion_calls = group_calls = label_calls = 0;
}

/* 0x1f01a0 branch coverage: index below 0 and at the bound, empty slot, read not completed,
 * and a completed read for every kind 0..7 plus one kind on each side of the table. */
static void check_load_model_callback(void)
{
    static const int motions[10] = { 0, 1, 0, 0, 1, 8, 8, 8, 1, 0 };
    static struct DotrObject obj;
    /* the record carries the loaded address in 32 bits: use a low page */
    int *file = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    struct DotrResourceSubmitRecord rec;
    char expected[64];
    int kind;
    int i;

    assert(file != MAP_FAILED);
    memset(&rec, 0, sizeof(rec));
    memset(&obj, 0, sizeof(obj));
    for (i = 0; i < 8; i++) gSzModel_apchBase[i] = "kd";
    gSzModel_apchBase[5] = "five";
    rec.unknown_014 = (int)(intptr_t)file;
    reset();
    rec.unknown_01c = -1; lSzModel_LoadModelCB(&rec, 1);
    rec.unknown_01c = 84; lSzModel_LoadModelCB(&rec, 1);
    rec.unknown_01c = 9; lSzModel_LoadModelCB(&rec, 1);
    assert(clean_calls == 0 && bind_calls == 0 && group_calls == 0);

    gSzModel_ahModel[9] = &obj;
    *(signed char *)&obj.drawFlag = -1;
    lSzModel_LoadModelCB(&rec, 0);
    assert(obj.drawFlag.unknown_bit_3 == 0 && *(unsigned char *)&obj.drawFlag == 0xf7);
    assert(clean_calls == 0 && bind_calls == 0 && group_calls == 0 && obj.modelBaseData == 0);

    for (kind = -1; kind <= 8; kind++) {
        reset();
        memset(&obj, 0, sizeof(obj));
        file[1] = 77;
        obj.type = (signed char)kind;
        obj.modelNo = 12;
        lSzModel_LoadModelCB(&rec, 1);
        assert(clean_calls == 1 && clean_arg == 9);
        assert((void *)obj.modelBaseData == (void *)file && *(char **)obj.unknown_13c == (char *)file + 16);
        assert(file[1] == 0 && bind_calls == 1 && bind_arg == (void *)file);
        assert(texture_calls == 1 && texture_arg == 9);
        assert(motion_calls == motions[kind + 1] && label_calls == (motions[kind + 1] != 0));
        assert(effect_calls == (motions[kind + 1] == 8) && (effect_calls == 0 || effect_arg == 9));
        for (i = 0; i < motion_calls; i++) {
            sprintf(expected, "root/%s/%s12-12%c", gSzModel_apchBase[kind], gSzModel_apchBase[kind], 'a' + i);
            assert(motion_types[i] == 9 && motion_slots[i] == i && strcmp(motion_names[i], expected) == 0);
        }
        assert(group_calls == 1 && group_args[0] == 9 && group_args[1] == (int)(intptr_t)lSzModel_LoadCB);
        assert(obj.drawFlag.unknown_bit_3 == 0);
    }
    gSzModel_ahModel[9] = 0;
    munmap(file, 4096);
}

int main(void)
{
    check_load_model_callback();
    puts("model_callbacks: ok");
    return 0;
}
