/* Synthetic host checks for the sound read/request ring writers. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/sound_queue.h"
#include "dotr/ee/sound_control.h"

static struct DotrSoundQueueManager storage;
struct DotrSoundQueueManager *g_SDValue[1] = {&storage};

static const char *copied_source;

/* Assumed to copy a zero-terminated text, like the uses elsewhere. */
char *func_0019bdd8(char *destination, const char *source)
{
    copied_source = source;
    return strcpy(destination, source);
}

enum { FLUSH = 1, START, WAKE, BIND };
static int trace[16], trace_count, start_block[4], bind_calls, bind_after, wake_argument;
/* 0x17bc80's answer; the step cases set it negative. */
static int wake_result = 0x55;

void func_0017bb60(int unknown)
{
    assert(unknown == 0);
    trace[trace_count++] = FLUSH;
}

int func_0017bca0(int *block, int unknown)
{
    assert(block == storage.unknown_0044 && unknown == 1);
    memcpy(start_block, block, sizeof(start_block));
    trace[trace_count++] = START;
    return 0x77;
}

int func_0017bc80(int unknown)
{
    wake_argument = unknown;
    trace[trace_count++] = WAKE;
    return wake_result;
}

/* Never returns a negative result here: the original then loops forever. */
int func_0017e8b0(void *block, int number, int unknown)
{
    assert(block == storage.unknown_001c && number == 0x5730601 && unknown == 0);
    trace[trace_count++] = BIND;
    if (++bind_calls == bind_after) {
        storage.unknown_0040 = 1;
    }
    return 0;
}

static void service_cases(void)
{
    assert(offsetof(struct DotrSoundQueueManager, unknown_001c) == 0x1c);
    assert(offsetof(struct DotrSoundQueueManager, unknown_0040) == 0x40);
    assert(offsetof(struct DotrSoundQueueManager, unknown_0044) == 0x44);
    assert(offsetof(struct DotrSoundQueueManager, unknown_0064) == 0x64);

    /* 0x10c800: flush, fill the four-word block (last word zero), start, keep
     * and return the result.
     */
    memset(&storage, 0x33, sizeof(storage));
    trace_count = 0;
    assert(SDRPC_StartTransferToIop(11, 22, 33) == 0x77);
    assert(trace_count == 2 && trace[0] == FLUSH && trace[1] == START);
    assert(start_block[0] == 11 && start_block[1] == 22 && start_block[2] == 33 && start_block[3] == 0);
    assert(storage.unknown_0064 == 0x77 && storage.unknown_0040 == 0x33333333);

    /* 0x10c8a0: forwards the stored result and returns the callee's value. */
    trace_count = 0;
    assert(SDRPC_TranserToIopStatus() == 0x55 && wake_argument == 0x77 && trace_count == 1);

    /* 0x10c8b0: repeats the bind (with its fixed delay loop) until the word at
     * +0x40 is nonzero, then flushes once and returns 1.
     */
    storage.unknown_0040 = 0;
    bind_calls = 0;
    bind_after = 3;
    trace_count = 0;
    assert(SDRPC_Init() == 1);
    assert(bind_calls == 3 && trace_count == 4 && trace[3] == FLUSH);
    assert(trace[0] == BIND && trace[1] == BIND && trace[2] == BIND);
}

/* Stubs of SD_vDoFsCommandQueue's callees SDFS_IsUsingNow (0x10c2c0) and SDFS_ReadToEE
 * (0x10c2d0), which the sound_file group defines. */
int s_iHowManyTimesCDAreUsed[1];
static int busy_v, busy_calls, read_calls, read_args[3];
static char *read_name;

int SDFS_IsUsingNow(void)
{
    busy_calls++;
    return busy_v;
}

int func_0010c2d0_word(char *name, int destination, int offset, int size)
{
    read_name = name;
    read_args[0] = destination;
    read_args[1] = offset;
    read_args[2] = size;
    read_calls++;
    return 1;
}

static void step_reset(void)
{
    memset(&storage, 0, sizeof(storage));
    storage.unknown_0000 = 0x1000;
    s_iHowManyTimesCDAreUsed[0] = 40;
    busy_v = 0;
    busy_calls = read_calls = trace_count = 0;
    wake_result = 0x55;
}

/* SD_vDoFsCommandQueue (branch coverage: each state of both sides, each callee answer,
 * the empty counts and the index wrap at 16). 0x10c8a0 and 0x10c800 are the
 * real functions over the 0x17bc80 / 0x17bb60 / 0x17bca0 stubs. */
static void step_cases(void)
{
    struct DotrSoundQueueRead *read;

    /* both sides idle with empty counts: nothing changes, no callee */
    step_reset();
    SD_vDoFsCommandQueue();
    assert(storage.unknown_171c == 0 && storage.unknown_182c == 0 && storage.read_count == 0 && storage.request_count == 0);
    assert(busy_calls == 0 && trace_count == 0 && storage.unknown_0000 == 0x1000);

    /* idle with counts: state 1, count taken */
    step_reset();
    storage.read_count = 3;
    storage.request_count = 2;
    SD_vDoFsCommandQueue();
    assert(storage.unknown_171c == 1 && storage.read_count == 2 && storage.unknown_182c == 1 && storage.request_count == 1);
    assert(busy_calls == 0 && trace_count == 0);

    /* state 1, callees not ready (0x10c2c0 not 0, 0x10c8a0 not negative): unchanged */
    step_reset();
    storage.unknown_171c = 1;
    storage.unknown_182c = 1;
    busy_v = 5;
    wake_result = 0;
    SD_vDoFsCommandQueue();
    assert(storage.unknown_171c == 1 && storage.unknown_182c == 1 && read_calls == 0 && storage.unknown_0000 == 0x1000);
    assert(busy_calls == 1 && trace_count == 1 && trace[0] == WAKE);

    /* state 1, ready: the indexed entries are forwarded, states step, bits set */
    step_reset();
    storage.unknown_171c = 1;
    storage.unknown_1718 = 15;
    read = &storage.reads[15];
    read->destination = 0x5000;
    read->offset = 0x1800;
    read->length = 0x800;
    storage.unknown_182c = 1;
    storage.unknown_1828 = 15;
    storage.requests[15].unknown_000 = 11;
    storage.requests[15].unknown_004 = 22;
    storage.requests[15].unknown_008 = 33;
    storage.unknown_0064 = 0x66;
    wake_result = -1;
    SD_vDoFsCommandQueue();
    assert(read_calls == 1 && read_name == read->name && read_args[0] == 0x5000 && read_args[1] == 0x1800 && read_args[2] == 0x800);
    assert(storage.unknown_171c == 2 && storage.unknown_182c == 2 && storage.unknown_0000 == 0x8801000);
    assert(trace_count == 3 && trace[0] == WAKE && wake_argument == 0x66 && trace[1] == FLUSH && trace[2] == START);
    assert(start_block[0] == 11 && start_block[1] == 22 && start_block[2] == 33 && start_block[3] == 0);
    assert(storage.unknown_1718 == 15 && storage.unknown_1828 == 15 && s_iHowManyTimesCDAreUsed[0] == 40);

    /* state 2, not ready: unchanged */
    step_reset();
    storage.unknown_0000 = 0x8801000;
    storage.unknown_171c = 2;
    storage.unknown_182c = 2;
    busy_v = 1;
    SD_vDoFsCommandQueue();
    assert(storage.unknown_171c == 2 && storage.unknown_182c == 2 && storage.unknown_0000 == 0x8801000 && s_iHowManyTimesCDAreUsed[0] == 40);

    /* state 2, ready: counter, bits cleared, indexes advance (15 wraps to 0, 3 to 4), states 3 */
    step_reset();
    storage.unknown_0000 = 0x8801000;
    storage.unknown_171c = 2;
    storage.unknown_1718 = 15;
    storage.unknown_182c = 2;
    storage.unknown_1828 = 3;
    wake_result = -5;
    SD_vDoFsCommandQueue();
    assert(storage.unknown_171c == 3 && storage.unknown_182c == 3 && storage.unknown_0000 == 0x1000 && s_iHowManyTimesCDAreUsed[0] == 41);
    assert(storage.unknown_1718 == 0 && storage.unknown_1828 == 4 && read_calls == 0 && trace_count == 1);

    /* any other state returns to 0 without a callee, whatever the counts */
    storage.read_count = 1;
    storage.request_count = 1;
    busy_calls = trace_count = 0;
    SD_vDoFsCommandQueue();
    assert(storage.unknown_171c == 0 && storage.unknown_182c == 0 && busy_calls == 0 && trace_count == 0);
    assert(storage.read_count == 1 && storage.request_count == 1);
    wake_result = 0x55;
}

/* Stubs and data for 0x10a4d0. */
char *g_SDBuffer[1];
char data_002b9090[] = "<ambience %d>";
/* Synthetic nonnull base word; it is only used arithmetically. */
#define AMB_BASE ((char *)0x1000)
static int cd_busy, sif_busy, cd_calls, sif_calls, file_exists, exist_calls;
static int format_calls, format_number, sent_calls, sent[5];

int SD_DoesExistAnyCDReq(void)
{
    cd_calls++;
    return cd_busy;
}

int SD_DoesExistAnySIFReq(void)
{
    sif_calls++;
    return sif_busy;
}

/* Assumed to format like the standard routine; only the number is recorded. */
int func_0019b648(char *destination, const char *format, ...)
{
    va_list list;

    assert(destination == storage.ambience_name && format == data_002b9090);
    va_start(list, format);
    format_number = va_arg(list, int);
    va_end(list);
    strcpy(destination, "<file>");
    format_calls++;
    return 0;
}

int RhFile_ExistFile(char *name)
{
    assert(name == storage.ambience_name && format_calls == exist_calls + 1);
    exist_calls++;
    return file_exists;
}

int SDRPC_SendCommand(int command, ...)
{
    va_list list;

    sent[0] = command;
    va_start(list, command);
    sent[1] = va_arg(list, int);
    sent[2] = va_arg(list, int);
    if (command == 0x5102) {
        sent[3] = va_arg(list, int);
        sent[4] = va_arg(list, int);
    }
    va_end(list);
    sent_calls++;
    return 0;
}

static void ambience_reset(int step0, int step1)
{
    memset(&storage, 0, sizeof(storage));
    g_SDValue[0] = &storage;
    g_SDBuffer[0] = AMB_BASE;
    storage.unknown_0000 = 0x8001;
    storage.unknown_0004 = 8;
    storage.unknown_0014 = 0x100;
    storage.ambience_step[0] = step0;
    storage.ambience_step[1] = step1;
    storage.ambience_slot[0] = 1;
    storage.ambience_slot[1] = 0;
    storage.ambience_file[0] = 41;
    storage.ambience_file[1] = 42;
    storage.unknown_2258 = -1;
    storage.unknown_225c = -1;
    cd_busy = sif_busy = cd_calls = sif_calls = file_exists = exist_calls = 0;
    format_calls = sent_calls = 0;
}

/* The outcome every early return shares: both steps and bit 15 as they were. */
static int ambience_untouched(int step0, int step1)
{
    return storage.ambience_step[0] == step0 && storage.ambience_step[1] == step1 &&
           storage.unknown_0000 == 0x8001 && storage.read_count == 0 && storage.request_count == 0 &&
           sent_calls == 0 && storage.unknown_2258 == -1 && storage.unknown_225c == -1;
}

/* SD_vRunAmbienceOnIopProc (branch coverage): the two guards, every step value 0..5 and an unknown one,
 * each early return inside a step, the loop reaching the second slot, and both outcomes
 * of the bit-21 test after the loop. */
static void ambience_cases(void)
{
    unsigned char *byte_58 = (unsigned char *)&storage.unknown_0054[1];

    assert(offsetof(struct DotrSoundQueueManager, unknown_204c) == 0x204c);
    assert(offsetof(struct DotrSoundQueueManager, ambience_name) == 0x21c8);
    assert(offsetof(struct DotrSoundQueueManager, ambience_step) == 0x2208);
    assert(offsetof(struct DotrSoundQueueManager, ambience_file) == 0x2218);
    assert(offsetof(struct DotrSoundQueueManager, unknown_2248) == 0x2248);
    assert((char *)byte_58 - (char *)&storage == 0x58);

    /* guards: null manager; +4 bit 3 clear */
    ambience_reset(9, 9);
    g_SDValue[0] = 0;
    SD_vRunAmbienceOnIopProc();
    g_SDValue[0] = &storage;
    assert(ambience_untouched(9, 9));
    storage.unknown_0004 = ~8;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(9, 9));

    /* steps outside 1..5 are cleared in both slots; then bit 15 goes unless bit 21 is set */
    ambience_reset(9, 0);
    SD_vRunAmbienceOnIopProc();
    assert(storage.ambience_step[0] == 0 && storage.ambience_step[1] == 0 && storage.unknown_0000 == 1);
    assert(cd_calls == 0 && sif_calls == 0);
    ambience_reset(6, -1);
    storage.unknown_0000 = 0x208001;
    SD_vRunAmbienceOnIopProc();
    assert(storage.ambience_step[0] == 0 && storage.ambience_step[1] == 0 && storage.unknown_0000 == 0x208001);

    /* step 1: a pending CD request, the byte at +0x58 or the word at +0x204c hold it back */
    ambience_reset(1, 9);
    cd_busy = 1;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(1, 9) && cd_calls == 1 && format_calls == 0);
    ambience_reset(1, 9);
    *byte_58 = 1;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(1, 9) && cd_calls == 1 && format_calls == 0);
    ambience_reset(1, 9);
    storage.unknown_204c = 1;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(1, 9) && cd_calls == 1 && format_calls == 0);
    /* step 1, no such file: the slot is cleared and the loop goes on to the next slot */
    ambience_reset(1, 9);
    SD_vRunAmbienceOnIopProc();
    assert(format_calls == 1 && format_number == 41 && exist_calls == 1);
    assert(storage.ambience_step[0] == 0 && storage.ambience_step[1] == 0 && storage.unknown_0000 == 1);
    assert(storage.read_count == 0);
    /* step 1, file present: 4096 bytes are queued to the slot's buffer; first and second slot */
    ambience_reset(1, 9);
    file_exists = 1;
    SD_vRunAmbienceOnIopProc();
    assert(format_number == 41 && storage.read_count == 1 && strcmp(storage.reads[0].name, "<file>") == 0);
    assert(storage.reads[0].destination == 0x1000 + 0x101800 && storage.reads[0].offset == 0);
    assert(storage.reads[0].length == 0x1000);
    assert(storage.ambience_step[0] == 2 && storage.ambience_step[1] == 9 && storage.unknown_0000 == 0x8001);
    ambience_reset(0, 1);
    file_exists = 1;
    SD_vRunAmbienceOnIopProc();
    assert(format_number == 42 && storage.read_count == 1);
    assert(storage.reads[0].destination == 0x1000 + 0x1000 + 0x101800 && storage.reads[0].length == 0x1000);
    assert(storage.ambience_step[0] == 0 && storage.ambience_step[1] == 2 && storage.unknown_0000 == 0x8001);

    /* step 2: waits for the CD requests */
    ambience_reset(2, 9);
    cd_busy = 1;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(2, 9) && cd_calls == 1);
    ambience_reset(2, 9);
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(3, 9) && cd_calls == 1);

    /* step 3: waits for the SIF requests, then queues the buffer for the IOP */
    ambience_reset(3, 9);
    sif_busy = 1;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(3, 9) && sif_calls == 1);
    ambience_reset(0, 3);
    SD_vRunAmbienceOnIopProc();
    assert(sif_calls == 1 && storage.request_count == 1);
    assert(storage.requests[0].unknown_000 == 0x1000 + 0x1000 + 0x101800);
    assert(storage.requests[0].unknown_004 == 0x100 + 0xc0800 && storage.requests[0].unknown_008 == 0x1000);
    assert(storage.ambience_step[0] == 0 && storage.ambience_step[1] == 4 && storage.unknown_0000 == 0x8001);

    /* step 4: waits for the SIF requests, then sends 0x5401 */
    ambience_reset(4, 9);
    sif_busy = 1;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(4, 9) && sif_calls == 1);
    ambience_reset(4, 9);
    SD_vRunAmbienceOnIopProc();
    assert(sent_calls == 1 && sent[0] == 0x5401 && sent[1] == 1 && sent[2] == 0x100 + 0xc0800);
    assert(storage.ambience_step[0] == 5 && storage.ambience_step[1] == 9 && storage.unknown_0000 == 0x8001);

    /* step 5: waits for bit 3 of the byte at +0x58, then records the file number */
    ambience_reset(5, 9);
    *byte_58 = 8;
    SD_vRunAmbienceOnIopProc();
    assert(ambience_untouched(5, 9));
    ambience_reset(5, 9);
    *byte_58 = 0xf7;
    SD_vRunAmbienceOnIopProc();
    assert(storage.ambience_step[0] == 0 && storage.ambience_step[1] == 9 && storage.unknown_0000 == 0x8001);
    assert(storage.unknown_2258 == -1 && storage.unknown_225c == 41);
    ambience_reset(0, 5);
    SD_vRunAmbienceOnIopProc();
    assert(storage.ambience_step[1] == 0 && storage.unknown_2258 == 42 && storage.unknown_225c == -1);
    memset(&storage, 0, sizeof(storage));
}

/* Stubs for SD_vAutoDMASequentialWatchDog (assumed semantics: each only records its call). */
static char wd_log[32];
static int wd_n, wd_volume, wd_load[3], wd_decode_arg, wd_decode_c0, wd_decode_clears;
int SD_iStopAutoDMA(void) { wd_log[wd_n++] = 'S'; return 0; }
int SD_iStartAutoDMA(void) { wd_log[wd_n++] = 'A'; return 0; }
int SD_BGMGetVolume(void) { wd_log[wd_n++] = 'V'; return wd_volume; }
void _vLoadBGMFileToEE(int id, int position, int length)
{
    wd_log[wd_n++] = 'L';
    wd_load[0] = id; wd_load[1] = position; wd_load[2] = length;
}
void _vDecodeLPCMThenSendToIOP(int unused)
{
    wd_log[wd_n++] = 'D';
    wd_decode_arg = unused;
    wd_decode_c0 = storage.unknown_01c0;
    if (wd_decode_clears) {
        storage.unknown_0000 &= ~0x100;
    }
}
static void wd_reset(int step)
{
    memset(&storage, 0, sizeof storage);
    g_SDValue[0] = &storage;
    storage.unknown_0004 = 2;
    storage.unknown_0000 = 0x70;
    storage.unknown_01ec = 1;
    storage.unknown_01e4 = step;
    storage.unknown_01dc = 77;
    cd_busy = sif_busy = 0;
    memset(wd_log, 0, sizeof wd_log);
    wd_n = 0; wd_volume = 0; wd_decode_arg = -1; wd_decode_clears = 0;
}
static int wd_is(const char *calls) { return strcmp(wd_log, calls) == 0; }

/* SD_vAutoDMASequentialWatchDog (branch coverage: one assertion per branch outcome and case). */
static void watchdog_cases(void)
{
    int bit;

    /* gates: +4 bit 1, then +0 bits 4, 5, 6; nothing happens, not even the stop */
    wd_reset(101); storage.unknown_0004 = ~2; storage.unknown_01ec = 0;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 101);
    for (bit = 0x10; bit <= 0x40; bit <<= 1) {
        wd_reset(101); storage.unknown_0000 = ~bit; storage.unknown_01ec = 0;
        SD_vAutoDMASequentialWatchDog();
        assert(wd_is("") && storage.unknown_01e4 == 101);
    }
    /* +0x1ec zero: stop first; nonzero: no stop. 101-103 count up */
    wd_reset(101); storage.unknown_01ec = 0;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("S") && storage.unknown_01e4 == 102);
    wd_reset(102); SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 103);
    wd_reset(103); SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 104);
    /* 100: waits for volume 0 */
    wd_reset(100); wd_volume = 5; storage.unknown_01c8 = 9;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("V") && storage.unknown_01e4 == 100 && storage.unknown_01c8 == 9);
    wd_reset(100); storage.unknown_01c8 = 9; storage.unknown_01cc = 9; storage.unknown_01d0 = 9;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("VS") && storage.unknown_01e4 == 101);
    assert(storage.unknown_01c8 == 0 && storage.unknown_01cc == 0 && storage.unknown_01d0 == 0);
    /* 104: id 9127 ends the sequence and clears bits 8, 9, 13; another id goes to 1 */
    wd_reset(104); storage.unknown_2248 = 9127; storage.unknown_0000 = 0x70 | 0x100 | 0x200 | 0x2000 | 0x400;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_0000 == (0x70 | 0x400));
    assert(storage.unknown_01dc == 9127 && storage.unknown_01e4 == 0);
    wd_reset(104); storage.unknown_2248 = 31;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01dc == 31 && storage.unknown_01e4 == 1);
    /* 1: load, length by +0x1fc bit 1; returns before the tail (+0x1c4 kept) */
    wd_reset(1); storage.unknown_01fc = 2; storage.unknown_0000 |= 0x100; storage.unknown_0018 = 0x1000000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("L") && wd_load[0] == 77 && wd_load[1] == 0 && wd_load[2] == 0x90000);
    assert(storage.unknown_01e4 == 2 && storage.unknown_01c4 == 0);
    wd_reset(1);
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("L") && wd_load[0] == 77 && wd_load[1] == 0 && wd_load[2] == 0x100000);
    assert(storage.unknown_01e4 == 2);
    /* 2: waits for the CD queue, then clears four words and decodes */
    wd_reset(2); cd_busy = 1; storage.unknown_0018 = 3;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 2 && storage.unknown_0018 == 3);
    wd_reset(2); storage.unknown_0018 = 3; storage.unknown_01c4 = 3; storage.unknown_01c0 = 3; storage.unknown_01d8 = 3;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("D") && wd_decode_arg == 0 && storage.unknown_01e4 == 3);
    assert(storage.unknown_0018 == 0 && storage.unknown_01c4 == 0 && storage.unknown_01c0 == 0);
    assert(storage.unknown_01d8 == 0);
    /* 3: waits for the SIF queue, then decodes */
    wd_reset(3); sif_busy = 1;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 3);
    wd_reset(3);
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("D") && wd_decode_arg == 0 && storage.unknown_01e4 == 4);
    /* 4: waits for the SIF queue; +0x1fc bit 1 decides between going on and ending */
    wd_reset(4); sif_busy = 1; storage.unknown_01fc = 2;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 4 && storage.unknown_01fc == 2);
    wd_reset(4); storage.unknown_01fc = 7;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 5 && storage.unknown_01fc == 5);
    wd_reset(4); storage.unknown_01fc = 1; storage.unknown_0000 |= 0x2000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 0 && storage.unknown_01fc == 4);
    assert(storage.unknown_0000 == 0x70);
    /* 5: second load, stop, start */
    wd_reset(5);
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("LSA") && wd_load[0] == 77 && wd_load[1] == 0x90000 && wd_load[2] == 0x70000);
    assert(storage.unknown_01e4 == 6);
    /* 6: waits for the CD queue, then ends and clears bit 13 */
    wd_reset(6); cd_busy = 1; storage.unknown_0000 |= 0x2000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 6 && storage.unknown_0000 == (0x70 | 0x2000));
    wd_reset(6); storage.unknown_0000 |= 0x2000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 0 && storage.unknown_0000 == 0x70);
    /* any other value becomes 0 */
    wd_reset(7); SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 0);
    wd_reset(99); SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01e4 == 0);

    /* tail. Bit 8 clear: nothing */
    wd_reset(0); storage.unknown_0018 = 0x1000000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01c4 == 0);
    /* bit 24 equal on both words: nothing */
    wd_reset(0); storage.unknown_0000 |= 0x100; storage.unknown_0018 = 0x1000005; storage.unknown_01c4 = 0x1000000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("") && storage.unknown_01c4 == 0x1000000);
    /* bit 24 now set: copy, +0x1d8 = 0, decode, load at +0x1cc (+0x1c8 == 0) */
    wd_reset(0); storage.unknown_0000 |= 0x100; storage.unknown_0018 = 0x1000005;
    storage.unknown_01d8 = 9; storage.unknown_01cc = 0x4000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("DL") && wd_decode_arg == 0 && wd_decode_c0 == 0x1000005);
    assert(storage.unknown_01c4 == 0x1000005 && storage.unknown_01d8 == 0 && storage.unknown_01c0 == 0);
    assert(wd_load[0] == 77 && wd_load[1] == 0x4000 && wd_load[2] == 0x80000);
    /* bit 24 now clear: +0x1d8 = 1; +0x1c8 == 0xa0000 also loads */
    wd_reset(0); storage.unknown_0000 |= 0x100; storage.unknown_0018 = 6; storage.unknown_01c4 = 0x1000000;
    storage.unknown_01c8 = 0xa0000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("DL") && storage.unknown_01c4 == 6 && storage.unknown_01d8 == 1 && storage.unknown_01c0 == 0);
    /* another +0x1c8: no load */
    wd_reset(0); storage.unknown_0000 |= 0x100; storage.unknown_0018 = 0x1000000; storage.unknown_01c8 = 0x10000;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("D") && storage.unknown_01c0 == 0 && storage.unknown_01c4 == 0x1000000);
    /* bit 8 cleared during the decode: no load, +0x1c0 still cleared */
    wd_reset(0); storage.unknown_0000 |= 0x100; storage.unknown_0018 = 0x1000000; wd_decode_clears = 1;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("D") && storage.unknown_01c0 == 0);
    /* a waiting step (4, SIF busy) still reaches the tail */
    wd_reset(4); sif_busy = 1; storage.unknown_0000 |= 0x100; storage.unknown_0018 = 0x1000000;
    storage.unknown_01c8 = 1;
    SD_vAutoDMASequentialWatchDog();
    assert(wd_is("D") && storage.unknown_01e4 == 4 && storage.unknown_01c4 == 0x1000000);
}

static _Alignas(16) char xfer_region[0x101810];
static int fetch_calls, fetch_result, kick_calls, kick_number;
static char *fetch_text, *kick_text;

int SD_FetchFromXferQueue(int *unknown_000, void *data)
{
    fetch_calls++;
    fetch_text = data;
    *unknown_000 = 0x77;
    return fetch_result;
}

void SD_KickSEGroupReadSequence(int unknown_20b4, char *text)
{
    kick_calls++;
    kick_number = unknown_20b4;
    kick_text = text;
}

static void xfer_reset(int step)
{
    memset(&storage, 0, sizeof(storage));
    g_SDValue[0] = &storage;
    g_SDBuffer[0] = xfer_region;
    storage.unknown_0000 = 0x4041;
    storage.unknown_0004 = 4;
    storage.unknown_0014 = 0x1000;
    storage.unknown_20b0 = step;
    storage.unknown_20b4 = 0x66;
    strcpy(storage.unknown_2070, "grp");
    busy_v = sif_busy = 0;
    fetch_calls = fetch_result = kick_calls = sent_calls = 0;
}

static int xfer_request(int length)
{
    return storage.request_count == 1 &&
           storage.requests[0].unknown_000 == (int)(DotrSoundControlWord)(xfer_region + 0x101800) &&
           storage.requests[0].unknown_004 == 0x41000 && storage.requests[0].unknown_008 == length;
}

static void xfer_size(int value)
{
    memcpy(xfer_region + 0x101804, &value, sizeof(value));
}

/* 0x10b6c0: one assertion group per branch outcome and boundary. */
static void transfer_cases(void)
{
    int step;

    /* No manager; flag clear. */
    xfer_reset(0);
    g_SDValue[0] = 0;
    SD_vTransferSEGrpToIopProc();
    assert(fetch_calls == 0);
    xfer_reset(5);
    storage.unknown_0004 = ~4;
    SD_vTransferSEGrpToIopProc();
    assert(fetch_calls == 0 && storage.unknown_20b0 == 5);

    /* Values outside the steps (0, between the runs, past the last): cleared, queue polled. */
    for (step = 0; step < 3; step++) {
        static const int other[3] = {0, 4, 37};
        xfer_reset(other[step]);
        SD_vTransferSEGrpToIopProc();
        assert(storage.unknown_20b0 == 0 && fetch_calls == 1 && kick_calls == 0);
    }
    xfer_reset(21);
    fetch_result = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 0 && fetch_calls == 1 && kick_calls == 1);
    assert(kick_number == 0x77 && kick_text == fetch_text);

    /* 1 */
    xfer_reset(1);
    busy_v = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 1 && storage.request_count == 0);
    busy_v = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 2 && xfer_request(0x80800) && fetch_calls == 0);

    /* 2 */
    xfer_reset(2);
    sif_busy = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 2 && sent_calls == 0);
    sif_busy = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 3 && sent_calls == 1);
    assert(sent[0] == 0x5100 && sent[1] == 0x66 && sent[2] == 0x41000);

    /* 3 */
    xfer_reset(3);
    ((unsigned char *)&storage)[0x5a] = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 3 && storage.unknown_0000 == 0x4041);
    ((unsigned char *)&storage)[0x5a] = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 0 && storage.unknown_0000 == 0x41 && fetch_calls == 0);

    /* 20, 30, 34: wait for the CD jobs (real SD_IsAnyJobOnCD). */
    for (step = 0; step < 3; step++) {
        static const int from[3] = {20, 30, 34}, to[3] = {1, 31, 35};
        xfer_reset(from[step]);
        storage.read_count = 1;
        SD_vTransferSEGrpToIopProc();
        assert(storage.unknown_20b0 == from[step]);
        storage.read_count = 0;
        SD_vTransferSEGrpToIopProc();
        assert(storage.unknown_20b0 == to[step]);
    }

    /* 31: size word plus 0x800; the part is capped at 0x80800. */
    xfer_reset(31);
    xfer_size(0x100);
    busy_v = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 31 && storage.request_count == 0);
    busy_v = 0;
    storage.unknown_20bc = 5;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 32 && xfer_request(0x900));
    assert(storage.unknown_20b8 == 0 && storage.unknown_20bc == 0x900 && storage.unknown_20c0 == 0x900);
    xfer_reset(31);
    xfer_size(0x80000);
    SD_vTransferSEGrpToIopProc();
    assert(xfer_request(0x80800) && storage.unknown_20b8 == 0 && storage.unknown_20bc == 0x80800);
    xfer_reset(31);
    xfer_size(0x80001);
    SD_vTransferSEGrpToIopProc();
    assert(xfer_request(0x80800) && storage.unknown_20b8 == 1 && storage.unknown_20bc == 0x80800);
    assert(storage.unknown_20c0 == 0x80800);

    /* 32 */
    xfer_reset(32);
    storage.unknown_20c0 = 0x900;
    storage.unknown_20b8 = 1;
    sif_busy = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 32 && sent_calls == 0);
    sif_busy = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 33 && sent_calls == 1 && sent[0] == 0x5102 && sent[1] == 0x66);
    assert(sent[2] == 0x41000 && sent[3] == 0x900 && sent[4] == 0);
    xfer_reset(32);
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 3 && sent_calls == 1);

    /* 33: read of the next part through the real SD_AddReadToEeReq. */
    xfer_reset(33);
    storage.unknown_20b8 = 0x1000;
    storage.unknown_20bc = 0x80800;
    ((unsigned char *)&storage)[0x5a] = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 33 && storage.read_count == 0);
    ((unsigned char *)&storage)[0x5a] = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 34 && storage.read_count == 1 && storage.unknown_20c0 == 0x1000);
    assert(!strcmp(storage.reads[0].name, "grp") && storage.reads[0].offset == 0x80800);
    assert(storage.reads[0].destination == (int)(DotrSoundControlWord)(xfer_region + 0x101800));
    assert(storage.reads[0].length == 0x1000 && storage.unknown_20b8 == 0x1000);
    xfer_reset(33);
    storage.unknown_20b8 = 0x80800;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20c0 == 0x80800 && storage.reads[0].length == 0x80800);
    xfer_reset(33);
    storage.unknown_20b8 = 0x80801;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20c0 == 0x80800 && storage.reads[0].length == 0x80800);

    /* 35 */
    xfer_reset(35);
    storage.unknown_20b8 = 0x1100;
    storage.unknown_20bc = 0x80800;
    storage.unknown_20c0 = 0x1000;
    busy_v = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 35 && storage.request_count == 0);
    busy_v = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 36 && xfer_request(0x1000));
    assert(storage.unknown_20b8 == 0x100 && storage.unknown_20bc == 0x81800);

    /* 36 */
    xfer_reset(36);
    storage.unknown_20b8 = 0x100;
    storage.unknown_20bc = 0x81800;
    storage.unknown_20c0 = 0x1000;
    sif_busy = 1;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 36 && sent_calls == 0);
    sif_busy = 0;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 33 && sent_calls == 1 && sent[0] == 0x5102 && sent[1] == 0x66);
    assert(sent[2] == 0x41000 && sent[3] == 0x1000 && sent[4] == 0x80000);
    xfer_reset(36);
    storage.unknown_20bc = 0x1800;
    storage.unknown_20c0 = 0x1000;
    SD_vTransferSEGrpToIopProc();
    assert(storage.unknown_20b0 == 3 && sent[4] == 0);
}

int main(void)
{
    static char name[] = "<name>";
    struct DotrSoundQueueRead *read;
    int i;

    step_cases();
    ambience_cases();

    /* Offsets of the minimum layout. */
    assert(offsetof(struct DotrSoundQueueManager, reads) == 0x1250);
    assert(offsetof(struct DotrSoundQueueManager, read_count) == 0x1710);
    assert(offsetof(struct DotrSoundQueueManager, unknown_171c) == 0x171c);
    assert(offsetof(struct DotrSoundQueueManager, requests) == 0x1720);
    assert(offsetof(struct DotrSoundQueueManager, request_count) == 0x1820);

    /* 0x10c290: nonzero if the read count or the word at +0x171c is nonzero. */
    memset(&storage, 0, sizeof(storage));
    assert(SD_IsAnyJobOnCD() == 0);
    storage.read_count = 3;
    assert(SD_IsAnyJobOnCD() == 1);
    storage.read_count = 0;
    storage.unknown_171c = -1;
    assert(SD_IsAnyJobOnCD() == 1);
    storage.unknown_1718 = 9; /* not consulted */
    storage.unknown_171c = 0;
    assert(SD_IsAnyJobOnCD() == 0);

    /* 0x10c110: the entry at the write index receives the name and the
     * sector-aligned window; the second word moves back by the remainder.
     */
    memset(&storage, 0x11, sizeof(storage));
    storage.read_write = 15;
    storage.read_count = 4;
    SD_AddReadToEeReq(name, 0x5000, 0x1805, 100);
    read = &storage.reads[15];
    assert(copied_source == name && strcmp(read->name, "<name>") == 0);
    assert(read->offset == 0x1800 && read->destination == 0x5000 - 5);
    assert(read->length == 0x800); /* 100 + 5, rounded up */
    assert(storage.read_write == 0 && storage.read_count == 5); /* index wraps at 16 */
    assert(storage.reads[14].offset == 0x11111111 && storage.reads[0].offset == 0x11111111);

    /* Already aligned input is stored unchanged; exact multiples stay put. */
    SD_AddReadToEeReq(name, 0x6000, 0x2000, 0x1000);
    read = &storage.reads[0];
    assert(read->destination == 0x6000 && read->offset == 0x2000 && read->length == 0x1000);
    assert(storage.read_write == 1 && storage.read_count == 6);
    SD_AddReadToEeReq(name, 0, 2047, 1);
    assert(storage.reads[1].destination == -2047 && storage.reads[1].offset == 0);
    assert(storage.reads[1].length == 0x800);
    SD_AddReadToEeReq(name, 0, 2047, 2);
    assert(storage.reads[2].length == 0x1000);

    /* 0x10c220: three words per request; the fourth is untouched. */
    memset(&storage, 0x22, sizeof(storage));
    storage.request_write = 0;
    storage.request_count = 0;
    for (i = 0; i < 17; i++) {
        SD_AddEeToIopReq(i, i + 100, i + 200);
    }
    assert(storage.request_write == 1 && storage.request_count == 17);
    assert(storage.requests[0].unknown_000 == 16 && storage.requests[0].unknown_008 == 216);
    assert(storage.requests[15].unknown_000 == 15 && storage.requests[15].unknown_004 == 115);
    assert(storage.requests[15].unknown_00c == 0x22222222);
    /* Neither count is bounded: a 17th write overwrites the oldest slot. */
    assert(storage.read_count == 0x22222222);

    service_cases();
    watchdog_cases();
    transfer_cases();
    puts("sound_queue: ok");
    return 0;
}
