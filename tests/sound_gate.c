/* Synthetic tests of the sound gate wrappers. Every sound-control routine and
 * the two outside dependencies are recording stubs with assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/sound_gate.h"

struct DotrSoundGate data_0034f6e0_gate[1];
char data_002d0930[] = "label";
static char format_one[] = "A%d";
static char format_two[] = "B%d-%d";
char *apchSEGrp[4] = {format_one, format_one, format_two, format_two};

static int log_count, log_entries[16][4];
static long status_word;
static int found, busy, send_result;
static char sent_text[128];

static void record(int tag, int a, int b, int c)
{
    assert(log_count < 16);
    log_entries[log_count][0] = tag;
    log_entries[log_count][1] = a;
    log_entries[log_count][2] = b;
    log_entries[log_count][3] = c;
    log_count++;
}

static int logged(int n, int tag, int a, int b, int c)
{
    const int *e = log_entries[n];
    return n < log_count && e[0] == tag && e[1] == a && e[2] == b && e[3] == c;
}

void RhMem_FillMemory(void *destination, int value, int size)
{
    record(1, value, size, destination == (void *)data_0034f6e0_gate);
    memset(destination, value, (size_t)size);
}

char *func_00197548(const char *label)
{
    assert(label == data_002d0930);
    return found ? data_002d0930 : 0;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list arguments;
    int n;
    va_start(arguments, format);
    n = vsprintf(destination, format, arguments);
    va_end(arguments);
    return n;
}

int SD_Init(struct DotrSoundQueueManager *block)
{
    record(2, block == (struct DotrSoundQueueManager *)0x400000, 0, 0);
    return 0;
}
int SD_Term(void) { record(3, 0, 0, 0); return 0; }
int SD_IsLoadingNow(void) { record(4, 0, 0, 0); return busy; }
void SD_SetOutputType(int value) { record(5, value, 0, 0); }
void SD_BGMStop(void) { record(6, 0, 0, 0); }
void SD_BGMFadeOut(short value) { record(7, value, 0, 0); }
void SD_BGMSetSourceLevel(int a, int b) { record(8, a, b, 0); }
void SD_SEPlay2(int a, int b, int c) { record(9, a, b, c); }
void SD_SEPlayOnlyOnce(int a, int b, int c) { record(10, a, b, c); }
void SD_SEStop(int a, int b) { record(11, a, b, 0); }
long SD_iGetSDStatus(void) { return status_word; }
int SD_iTransferSEGrpToIop(int unknown_000, void *data)
{
    record(12, unknown_000, 0, 0);
    strcpy(sent_text, (const char *)data);
    return send_result;
}

static void enable(void)
{
    int i;
    memset(data_0034f6e0_gate, 0, sizeof(data_0034f6e0_gate));
    data_0034f6e0_gate[0].unknown_08.unknown_bit_0 = -1;
    for (i = 0; i < 4; i++) {
        data_0034f6e0_gate[0].unknown_00[i] = (short)(i == 2 ? -1 : 20 + i);
    }
    status_word = 0;
    log_count = 0;
}

static void check_disabled(void)
{
    struct DotrSoundGate before;
    memset(data_0034f6e0_gate, 0, sizeof(data_0034f6e0_gate));
    data_0034f6e0_gate[0].unknown_08.unknown_bits_2_7 = 5;
    before = data_0034f6e0_gate[0];
    log_count = 0;
    SzSound_Term();
    assert(SzSound_TransferSEGrp(0, 1, 1) == 0 && SzSound_IsPlaySE() == 0 && SzSound_PlaySE(0, 1) == 0);
    assert(SzSound_StereoPlaySE(0, 1, 2) == 0 && SzSound_StopSE(0, 1) == 0 && SzSound_IsLoadingBGM() == 0);
    assert(SzSound_StopBGM(1) == 0 && SzSound_SetOutput(1) == 0);
    assert(log_count == 0 && memcmp(data_0034f6e0_gate, &before, sizeof(before)) == 0);
}

static void check_start_stop(void)
{
    int i;
    memset(data_0034f6e0_gate, 0xff, sizeof(data_0034f6e0_gate));
    found = 0;
    log_count = 0;
    SzSound_Init();
    assert(log_count == 1 && logged(0, 1, 0, 12, 1));
    for (i = 0; i < 12; i++) {
        assert(((unsigned char *)data_0034f6e0_gate)[i] == 0);
    }
    found = 1;
    log_count = 0;
    SzSound_Init();
    assert(log_count == 4 && logged(1, 2, 1, 0, 0) && logged(2, 8, 128, 128, 0) && logged(3, 5, 0, 0, 0));
    assert(data_0034f6e0_gate[0].unknown_08.unknown_bit_0 && data_0034f6e0_gate[0].unknown_08.unknown_bit_1);
    for (i = 0; i < 4; i++) {
        assert(data_0034f6e0_gate[0].unknown_00[i] == -1);
    }
    /* 0x217a70 stores the flag and passes its complement on. */
    log_count = 0;
    assert(SzSound_SetOutput(0) == 1 && logged(0, 5, 1, 0, 0) && !data_0034f6e0_gate[0].unknown_08.unknown_bit_1);
    assert(SzSound_SetOutput(0x100) == 1 && logged(1, 5, 0, 0, 0) && data_0034f6e0_gate[0].unknown_08.unknown_bit_1);
    log_count = 0;
    SzSound_Term();
    assert(log_count == 1 && logged(0, 3, 0, 0, 0) && !data_0034f6e0_gate[0].unknown_08.unknown_bit_0);
    assert(data_0034f6e0_gate[0].unknown_08.unknown_bit_1);
}

static void check_guarded(void)
{
    static const int bad_index[] = {-1, 4};
    size_t b;
    enable();
    for (b = 0; b < 2; b++) {
        assert(SzSound_TransferSEGrp(bad_index[b], 1, 1) == 0 && SzSound_PlaySE(bad_index[b], 1) == 0);
        assert(SzSound_StereoPlaySE(bad_index[b], 1, 2) == 0 && SzSound_StopSE(bad_index[b], 1) == 0);
    }
    /* Slot 2 holds a negative identifier. */
    assert(SzSound_PlaySE(2, 1) == 0 && SzSound_StereoPlaySE(2, 1, 2) == 0 && SzSound_StopSE(2, 1) == 0);
    status_word = 0x4000;
    assert(SzSound_PlaySE(1, 1) == 0 && SzSound_StereoPlaySE(1, 1, 2) == 0 && SzSound_StopSE(1, 1) == 0);
    assert(SzSound_IsPlaySE() == 0 && log_count == 0);
    status_word = 0x7fffbfffL;
    assert(SzSound_IsPlaySE() == 1);
    assert(SzSound_PlaySE(1, 0x8123) == 1 && logged(0, 10, 1, 0x0123, 1));
    assert(SzSound_PlaySE(3, 0x17fff) == 1 && logged(1, 10, 3, 0x7fff, 0));
    assert(SzSound_StereoPlaySE(0, 0x18001, -1) == 1 && logged(2, 9, 0, 1, 0x7fff));
    assert(SzSound_StopSE(3, -9) == 1 && logged(3, 11, 3, -9, 0) && log_count == 4);

    log_count = 0;
    busy = 0;
    assert(SzSound_IsLoadingBGM() == 0);
    busy = 7;
    assert(SzSound_IsLoadingBGM() == 1 && log_count == 2);
    assert(SzSound_StopBGM(0) == 1 && logged(2, 6, 0, 0, 0) && SzSound_StopBGM(-3) == 1 && logged(3, 6, 0, 0, 0));
    assert(SzSound_StopBGM(0x12345) == 1 && logged(4, 7, 0x2345, 0, 0));
}

static void check_select(void)
{
    enable();
    send_result = 0x55;
    /* Unchanged identifier without force: nothing is sent. */
    assert(SzSound_TransferSEGrp(1, 21, 0) == 1 && log_count == 0);
    assert(SzSound_TransferSEGrp(1, 21, 1) == 0x55 && log_count == 2 && logged(0, 11, 1, -1, 0) && logged(1, 12, 1, 0, 0));
    assert(strcmp(sent_text, "A21") == 0);
    log_count = 0;
    assert(SzSound_TransferSEGrp(0, 7, 0) == 0x55 && data_0034f6e0_gate[0].unknown_00[0] == 7 && strcmp(sent_text, "A7") == 0);
    assert(SzSound_TransferSEGrp(3, 9, 0) == 0x55 && data_0034f6e0_gate[0].unknown_00[3] == 9 && strcmp(sent_text, "B9-9") == 0);
    assert(SzSound_TransferSEGrp(2, 4, 0) == 0x55 && strcmp(sent_text, "B4-4") == 0 && log_count == 6);
}

static void check_value_switch(void)
{
    static const unsigned int key[5] = {536, 540, 541, 542, 7};
    static const int value[5] = {59, 75, 78, 79, 74};
    unsigned int words[4] = {0, 0, 0, 0};
    int i;

    for (i = 0; i < 5; i++) {
        enable();
        log_count = 0;
        words[3] = (key[i] << 14) | 0x3fff;
        assert(Ta_Eff_036_GetSound(words) == 1 && logged(0, 10, 1, value[i], 0));
    }
}

/* Ta_Eff_036_ManageSound (branch coverage): every counter case, the limit and its neighbours, for each key
 * and for the 999 word. */
static void check_step_value(unsigned int word, int old, int expect)
{
    unsigned int words[4] = {0, 0, 0, 0};
    struct DotrSoundStep step;

    memset(&step, 0, sizeof(step));
    enable();
    log_count = 0;
    words[3] = word;
    step.count_d0 = old;
    Ta_Eff_036_ManageSound(words, &step);
    assert(step.count_d0 == old + 1 && words[3] == word);
    if (expect < 0) {
        assert(log_count == 0);
    } else {
        assert(log_count == 1 && logged(0, 10, 1, expect, 0));
    }
}

static void check_step(void)
{
    static const unsigned int key[5] = {536, 540, 541, 542, 7};
    static const int value[5] = {59, 75, 78, 79, 74};
    static const int limit[5] = {64, 66, 64, 72, 48};
    int i;

    for (i = 0; i < 5; i++) {
        unsigned int word = (key[i] << 14) | 0x3fff;

        check_step_value(word, 0, 56);
        check_step_value(word, 20, 57);
        check_step_value(word, 30, 76);
        check_step_value(word, 36, value[i]);
        check_step_value(word, 37, key[i] == 541 || key[i] == 542 ? 59 : -1);
        check_step_value(word, limit[i] - 1, 60);
        check_step_value(word, limit[i] - 2, -1);
        check_step_value(word, limit[i], -1);
        check_step_value(word, 1, -1);
        check_step_value(word, 19, -1);
        check_step_value(word, 21, -1);
        check_step_value(word, 35, -1);
        check_step_value(word, 38, -1);
    }
    /* the word 999 has key 0: limit 48, no case 37 */
    check_step_value(999, 0, 56);
    check_step_value(999, 20, 57);
    check_step_value(999, 30, 76);
    check_step_value(999, 36, 74);
    check_step_value(999, 37, -1);
    check_step_value(999, 47, 60);
    check_step_value(999, 46, -1);
    check_step_value(999, 48, -1);
    check_step_value(999, 63, -1);
    /* a neighbouring word takes the other arm */
    check_step_value(998, 37, -1);
    check_step_value(998, 47, 60);
    check_step_value((541u << 14) | 999, 37, 59);
}

int main(void)
{
    check_disabled();
    check_start_stop();
    check_guarded();
    check_select();
    check_value_switch();
    check_step();
    puts("sound_gate: ok");
    return 0;
}
