/* Synthetic host checks for the sound-manager control routines. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/sound_control.h"

static struct DotrSoundQueueManager storage;
struct DotrSoundQueueManager *g_SDValue[1];
char *g_SDBuffer[1];
char data_002b90b0[] = "<format>";
char data_002b90a0[] = "<bgm format>";
/* Synthetic nonnull base word; it is only used arithmetically. */
#define BASE ((char *)0x1000)

enum { COMMAND = 1, FORWARD, COPY16, STEP_A, STEP_B, STEP_C, STEP_D, ALTERNATE };
static int command_result, commands[4], command_count;
static int trace[8], trace_count, command, arguments[4];

/* Reads only the arguments each command passes. */
int SDRPC_SendCommand(int number, ...)
{
    va_list list;
    int count = 0, i;
    switch (number) {
    case 0xf003: case 0x5003: count = 1; break;
    case 0x5005: case 0x5201: case 0x5220: count = 2; break;
    case 0x5210: count = 4; break;
    case 0x5202: case 0x1021: case 0x5000: case 0x5002: count = 0; break;
    case 0xf004: case 0x5004: case 0xf006: count = 1; break;
    case 0x1020: count = 3; break;
    default: assert(0);
    }
    command = number;
    memset(arguments, 0, sizeof(arguments));
    va_start(list, number);
    for (i = 0; i < count; i++) {
        arguments[i] = va_arg(list, int);
    }
    va_end(list);
    trace[trace_count++] = COMMAND;
    if (command_count < 4) {
        commands[command_count++] = number;
    }
    return command_result;
}

void *func_00198790(void *destination, const void *source, int byte_count)
{
    assert(byte_count == 16);
    trace[trace_count++] = COPY16;
    return memcpy(destination, source, 16);
}

static int clear_size, setup_calls, ring_cleared_before_setup;

/* Assumed to fill bytes like memset. */
void *func_00198948(void *destination, int value, int byte_count)
{
    assert(destination == &storage && value == 0);
    clear_size = byte_count;
    return memset(destination, value, (size_t)byte_count);
}

void SD_InitializeDevice(void)
{
    ring_cleared_before_setup = storage.ring_read == 0 && storage.ring_write == 0;
    setup_calls++;
}

static int flush_calls;

void func_0017bb60(int unknown)
{
    assert(unknown == 0);
    flush_calls++;
}

void SD_vDoFsCommandQueue(void) { trace[trace_count++] = STEP_A; }
void SD_vAutoDMASequentialWatchDog(void) { trace[trace_count++] = STEP_B; }
void SD_vTransferSEGrpToIopProc(void) { trace[trace_count++] = STEP_C; }
void SD_vRunAmbienceOnIopProc(void) { trace[trace_count++] = STEP_D; }

static int format_calls, format_ids[8];
/* Assumed text and byte copies, like the standard routines. */
int func_0019b648(char *destination, const char *format, ...)
{
    va_list list;
    va_start(list, format);
    if (format == data_002b90a0) {
        assert(destination == (char *)&storage + 0x1210);
        format_ids[format_calls++ & 7] = va_arg(list, int);
        va_end(list);
        strcpy(destination, "bgm");
        return 0;
    }
    assert(format == data_002b90b0);
    strcpy(destination, va_arg(list, const char *));
    va_end(list);
    return 0;
}

char *func_0019bdd8(char *destination, const char *source)
{
    return strcpy(destination, source);
}

void *func_0019c2b8(void *destination, const void *source, int byte_count)
{
    assert(byte_count == 28);
    return memcpy(destination, source, (size_t)byte_count);
}

static int block_calls, block_args[3];

void SD_AddEeToIopReq(int unknown_000, int unknown_004, int unknown_008)
{
    block_args[0] = unknown_000;
    block_args[1] = unknown_004;
    block_args[2] = unknown_008;
    block_calls++;
}

static void set(int pointer, int enabled, int word0, int word4)
{
    command_result = 0;
    command_count = 0;
    memset(&storage, 0, sizeof(storage));
    storage.unknown_0000 = word0;
    storage.unknown_0004 = word4;
    storage.unknown_2a6c = 0x7777;
    g_SDValue[0] = pointer ? &storage : 0;
    g_SDBuffer[0] = enabled ? BASE : 0;
    trace_count = 0;
    command = 0;
}

static void all_guarded(void)
{
    assert(SD_BGMGetVolume() == 0);
    assert(SD_IsLoadingNow() == 0);
    SD_SetOutputType(5);
    SD_StopAll();
    SD_BGMStop();
    SD_BGMFadeOut(5);
    SD_BGMSetSourceLevel(1, 2);
    SD_SEPlay2(1, 2, 3);
    SD_SEStop(1, 2);
    SD_SEStopAll();
    assert(trace_count == 0 && storage.unknown_2a6c == 0x7777 && storage.unknown_0000 == -1);
}

static void state_cases(void)
{
    /* 0x10a0f0: bit 8 gates the signed word; higher flag bits do not interfere. */
    set(1, 1, 0x80, 0);
    storage.unknown_01ec = -123;
    assert(SD_BGMGetVolume() == 0);
    storage.unknown_0000 = 0x100;
    assert(SD_BGMGetVolume() == -123);
    storage.unknown_0000 = (int)0x80000100u;
    storage.unknown_01ec = 456;
    assert(SD_BGMGetVolume() == 456 && trace_count == 0);

    assert(offsetof(struct DotrSoundQueueManager, unknown_01fc) == 0x1fc);
    assert(offsetof(struct DotrSoundQueueManager, items) == 0x210);
    assert(offsetof(struct DotrSoundQueueManager, unknown_01c8) == 0x1c8);
    assert(sizeof(struct DotrSoundQueueItem) == 16);
    assert(offsetof(struct DotrSoundQueueManager, reads) == 0x1250);
    assert(offsetof(struct DotrSoundQueueManager, unknown_2254) == 0x2254);

    /* 0x10b1f0. */
    set(1, 0, 0x01, 0);
    storage.unknown_01fc = 0x0f;
    storage.unknown_01d4 = 9;
    storage.unknown_01dc = 0x321;
    storage.unknown_0014 = 0x654;
    assert(SD_iStartAutoDMA() == 1);
    assert(storage.unknown_0000 == 0x101 && storage.unknown_01fc == 0x0b && storage.unknown_01d4 == 0);
    assert(storage.unknown_2254 == 0x321 && trace_count == 1 && command == 0x1020);
    assert(arguments[0] == 0x654 && arguments[1] == 0x40000 && arguments[2] == 1);

    /* 0x10b270: W4 bit 1 required; nothing to do without bit 0x100. */
    set(1, 0, 0x300, 0);
    assert(SD_iStopAutoDMA() == 0 && storage.unknown_0000 == 0x300 && trace_count == 0);
    set(1, 0, 0x200, 2);
    assert(SD_iStopAutoDMA() == 1 && storage.unknown_0000 == 0x200 && trace_count == 0);
    set(1, 0, 0x301, 2);
    storage.unknown_0018 = storage.unknown_01c0 = storage.unknown_01c4 = 5;
    storage.unknown_01d4 = storage.unknown_01d8 = 5;
    assert(SD_iStopAutoDMA() == 1 && storage.unknown_0000 == 1 && command == 0x1021);
    assert(storage.unknown_0018 == 0 && storage.unknown_01c0 == 0 && storage.unknown_01c4 == 0);
    assert(storage.unknown_01d4 == 0 && storage.unknown_01d8 == 0 && storage.unknown_2254 == -1);

    /* 0x10b330 and 0x10b360. */
    set(1, 0, 0, 0);
    SD_vInitSDThread();
    assert(storage.unknown_0000 == 0x10 && trace_count == 0);
    SD_vInitSDThread();
    assert(storage.unknown_0000 == 0x10);
    SD_vInitializeAutoDMA();
    assert(storage.unknown_0000 == 0x50 && command == 0x5000 && storage.unknown_01e8 == -1);
    storage.unknown_01e8 = 3;
    SD_vInitializeAutoDMA();
    assert(trace_count == 1 && storage.unknown_01e8 == 3);

    /* 0x10b3c0: found id, the special id 9127, and a missing id. */
    set(1, 0, 0, 0);
    assert(SD_iPreparePreBlockOnIOP(5) == 0 && trace_count == 0 && storage.unknown_2248 == 0);
    set(1, 1, 0, 2);
    storage.items[255].unknown_004 = 5;
    assert(SD_iPreparePreBlockOnIOP(5) == 1);
    assert(command == 0x5003 && arguments[0] == 256 && storage.unknown_2248 == 5);
    assert(storage.unknown_01fc == 1 && storage.unknown_01e4 == 100 && storage.unknown_0000 == 0x2000);
    set(1, 1, 0, 2);
    assert(SD_iPreparePreBlockOnIOP(6) == 0 && storage.unknown_2248 == 6 && storage.unknown_0000 == 0);
    assert(SD_iPreparePreBlockOnIOP(9127) == 1 && storage.unknown_0000 == 0x2000);
    /* Without the enable word the volume command is skipped but the rest still runs. */
    set(1, 0, 0, 2);
    assert(SD_iPreparePreBlockOnIOP(0) == 1 && trace_count == 0);
}

static void initializer_cases(void)
{
    struct DotrSoundControlVector4 value = {10.0f, 20.0f, 30.0f, 99.0f}, result;
    struct DotrSoundControlPair pair = {{1.0f, 2.0f, 3.0f, 0.0f}, {1.0f, 2.0f, 3.0f, 0.0f}};

    /* 0x109ca0: both words, the clear, the eight -1 words and the call order. */
    g_SDValue[0] = 0;
    g_SDBuffer[0] = 0;
    memset(&storage, 0x5a, sizeof(storage));
    assert(SD_Init(&storage) == 1);
    assert(g_SDValue[0] == &storage && g_SDBuffer[0] == (char *)&storage + 0x2a80);
    assert(clear_size == 0x2a80 && setup_calls == 1 && ring_cleared_before_setup);
    assert(storage.unknown_01dc == -1 && storage.unknown_01f4 == -1 && storage.unknown_2248 == -1);
    assert(storage.unknown_224c[0] == -1 && storage.unknown_224c[1] == -1 && storage.unknown_2254 == -1);
    assert(storage.unknown_2258 == -1 && storage.unknown_225c == -1);
    assert(storage.unknown_0000 == 0 && storage.unknown_01f0 == 0 && storage.unknown_2a6c == 0);

    /* 0x1099e0 with exactly representable values. */
    Card_V_IN(&result, &value, &pair, 4.0f);
    assert(result.x == 2.0f && result.y == 4.0f && result.z == 6.0f && result.w == 0.0f);
}

static void update_cases(void)
{
    static const int refreshed[] = {COMMAND, COPY16, STEP_A, STEP_B, STEP_C, STEP_D};
    static const int stale[] = {COMMAND, STEP_A, STEP_B, STEP_C, STEP_D};
    struct DotrSoundControlVector source = {1.5f, -2.0f, 3.25f}, copy = {0, 0, 0};
    int words[4] = {11, 22, 33, 44};

    assert(offsetof(struct DotrSoundQueueManager, unknown_0054) == 0x54);
    assert(offsetof(struct DotrSoundQueueManager, unknown_0180) == 0x180);
    assert(offsetof(struct DotrSoundQueueManager, unknown_01ec) == 0x1ec);

    /* 0x109a70. */
    assert(VEC_POI_IN(&copy, &source) == 0);
    assert(copy.x == 1.5f && copy.y == -2.0f && copy.z == 3.25f);

    /* 0x109dc0: guard, then W0 bit 0. */
    set(0, 1, 1, 0);
    SD_CallEveryVSync();
    set(1, 0, 1, 0);
    SD_CallEveryVSync();
    set(1, 1, 0, 0);
    SD_CallEveryVSync();
    assert(trace_count == 0);

    /* Zero command result: refresh 16 bytes, propagate two words, run the four steps. */
    set(1, 1, 1, 0);
    memcpy(storage.unknown_0180, words, sizeof(words));
    storage.unknown_0018 = 5;
    SD_CallEveryVSync();
    assert(trace_count == 6 && memcmp(trace, refreshed, sizeof(refreshed)) == 0);
    assert(commands[0] == 0x5002 && memcmp(storage.unknown_0054, words, sizeof(words)) == 0);
    assert(storage.unknown_01ec == 44 && storage.unknown_0018 == 33);
    /* A zero third word leaves +0x18 alone. */
    set(1, 1, 1, 0);
    words[2] = 0;
    memcpy(storage.unknown_0180, words, sizeof(words));
    storage.unknown_0018 = 5;
    SD_CallEveryVSync();
    assert(storage.unknown_0018 == 5 && storage.unknown_01ec == 44);
    /* Nonzero command result: no refresh, the steps still run in order. */
    set(1, 1, 1, 0);
    command_result = 1;
    SD_CallEveryVSync();
    assert(trace_count == 5 && memcmp(trace, stale, sizeof(stale)) == 0 && storage.unknown_01ec == 0);

    /* 0x109e80. */
    set(0, 1, 1, 2);
    assert(SD_BGMPreLoad(5, 1) == 0);
    set(1, 1, 1, 0);
    assert(SD_BGMPreLoad(5, 1) == 1 && trace_count == 0);
    set(1, 1, 0, 2);
    assert(SD_BGMPreLoad(5, 1) == 1 && trace_count == 0);
    /* Flag set: the real 0x10b490 runs (it leaves +0x1fc at 3, 0x10b3c0 at 1). */
    set(1, 1, 1, 2);
    storage.items[3].unknown_004 = 5;
    assert(SD_BGMPreLoad(5, 1) == 1 && storage.unknown_2248 == 5 && storage.unknown_01fc == 3);
    assert(storage.unknown_0000 == 0x2001);
    set(1, 1, 1, 2);
    assert(SD_BGMPreLoad(9127, 0) == 1 && storage.unknown_2248 == 9127 && storage.unknown_0000 == 0x2001);
    assert(storage.unknown_01fc == 1);

    /* 0x109d50 and 0x109d70. */
    set(1, 0, 0, 0);
    storage.unknown_0014 = 0x654;
    command_result = 0x31;
    assert(SD_Term() == 0x31 && command == 0xf004 && arguments[0] == 0x654);
    set(1, 1, 1, 2);
    SD_SuspendSoundSystem(0);
    assert(command_count == 1 && commands[0] == 0xf006 && arguments[0] == 0);
    set(1, 1, 1, 2);
    SD_SuspendSoundSystem(7);
    assert(command_count == 3 && commands[0] == 0x5005 && commands[1] == 0x5004 && commands[2] == 0xf006);
    assert(arguments[0] == 7);
}

/* Region standing for the storage after the manager: a 0x100000-byte source
 * ring at +0x800 and the output blocks at +0x181800.
 */
static _Alignas(16) char region[0x181800 + 128 * 1024];

static short source_value(int pair, int second)
{
    return (short)(pair * 7 + (second ? -3000 : 11));
}

static void sample_cases(void)
{
    short *ring = (short *)(region + 0x800);
    short *output = (short *)(region + 0x181800);
    int start = 0x100000 - 0x400; /* the copy wraps around the ring end */
    int mono, pair;

    for (pair = 0; pair < 0x40000; pair++) {
        ring[pair * 2] = source_value(pair, 0);
        ring[pair * 2 + 1] = source_value(pair, 1);
    }
    for (mono = 0; mono < 2; mono++) {
        set(1, 1, 0, 0);
        g_SDBuffer[0] = region;
        storage.unknown_2a6c = mono;
        flush_calls = 0;
        memset(output, 0x55, 128 * 1024);
        /* An odd position is rounded down to the pair boundary. */
        assert(_iDecodeLPCM(start + 1) == (start + 1 + 128 * 512) % 0x100000);
        assert(flush_calls == 1);
        for (pair = 0; pair < 128 * 128; pair++) {
            int index = (start / 4 + pair) % 0x40000;
            short first = source_value(index, 0), second = source_value(index, 1);
            short *block = output + (pair / 128) * 512;
            int slot = (pair % 128) * 2;
            if (mono) {
                first = second = (short)((first + second) >> 1);
            }
            assert(block[slot] == first && block[slot + 1] == first);
            assert(block[256 + slot] == second && block[256 + slot + 1] == second);
        }
    }
    /* The source ring itself is not modified. */
    assert(ring[0] == source_value(0, 0) && ring[0x7ffff] == source_value(0x3ffff, 1));
}

static void ring_cases(void)
{
    unsigned char payload[28], out[28];
    int word, i;

    assert(offsetof(struct DotrSoundQueueManager, unknown_2070) == 0x2070);
    assert(offsetof(struct DotrSoundQueueManager, ring_write) == 0x20c4);
    assert(offsetof(struct DotrSoundQueueManager, ring) == 0x20c8);
    assert(sizeof(struct DotrSoundQueueMessage) == 32);

    /* Empty ring: pop fails and writes nothing. */
    set(1, 1, 0, 0);
    storage.ring_write = 5;
    storage.ring_read = 5;
    word = 123;
    assert(SD_FetchFromXferQueue(&word, out) == 0 && word == 123);

    /* Seven pushes fit; the eighth is refused; order and payloads are kept
     * across the index wrap.
     */
    for (i = 0; i < 7; i++) {
        memset(payload, i + 1, sizeof(payload));
        assert(SD_AddToXferQueue(100 + i, payload) == 1);
    }
    assert(storage.ring_write == 4 && SD_AddToXferQueue(999, payload) == 0);
    assert(storage.ring_write == 4 && storage.ring_read == 5);
    for (i = 0; i < 7; i++) {
        assert(SD_FetchFromXferQueue(&word, out) == 1 && word == 100 + i);
        assert(out[0] == i + 1 && out[27] == i + 1);
    }
    assert(storage.ring_read == 4 && SD_FetchFromXferQueue(&word, out) == 0);

    /* 0x10bdc0 clears both indices. */
    SD_ClearXferQueue();
    assert(storage.ring_read == 0 && storage.ring_write == 0);

    /* 0x10bbc0: gated push; the enable word is not consulted. */
    set(0, 1, 1, 4);
    assert(SD_iTransferSEGrpToIop(1, payload) == 0);
    set(1, 0, 1, 0);
    assert(SD_iTransferSEGrpToIop(1, payload) == 0);
    set(1, 0, 0, 4);
    assert(SD_iTransferSEGrpToIop(1, payload) == 0 && storage.ring_write == 0);
    set(1, 0, 1, 4);
    assert(SD_iTransferSEGrpToIop(77, payload) == 1 && storage.ring_write == 1);
    assert(storage.ring[0].unknown_000 == 77);

    /* Status pairs: first word if nonzero, else the second. */
    set(1, 1, 0, 0);
    assert(SD_DoesExistAnyCDReq() == 0 && SD_DoesExistAnySIFReq() == 0);
    storage.unknown_171c = 6;
    storage.unknown_182c = 8;
    assert(SD_DoesExistAnyCDReq() == 6 && SD_DoesExistAnySIFReq() == 8);
    storage.read_count = 2;
    storage.request_count = 3;
    assert(SD_DoesExistAnyCDReq() == 2 && SD_DoesExistAnySIFReq() == 3);

    /* 0x10bc30: store, format, queue one aligned read at base + 0x101800,
     * set bit 0x4000 and store 30.
     */
    set(1, 1, 1, 0);
    storage.read_write = 2;
    SD_KickSEGroupReadSequence(0x55, "song");
    assert(storage.unknown_20b4 == 0x55 && strcmp(storage.unknown_2070, "song") == 0);
    assert(strcmp(storage.reads[2].name, "song") == 0);
    assert(storage.reads[2].destination == 0x1000 + 0x101800);
    assert(storage.reads[2].offset == 0 && storage.reads[2].length == 0x80800);
    assert(storage.read_write == 3 && storage.read_count == 1);
    assert(storage.unknown_0000 == 0x4001 && storage.unknown_20b0 == 30);
}

/* 0x10ac20 with the real 0x10a780, 0x10a090 and 0x10b270 linked in. The item
 * words are synthetic; their meanings are unknown. */
static void advance(int id, int word0, int word8, int wordc, int position, int half)
{
    set(1, 1, 0x100, 2);
    g_SDBuffer[0] = region;
    storage.unknown_0014 = 0x1000;
    storage.unknown_01c8 = 0x800;
    storage.unknown_01d4 = position;
    storage.unknown_01d8 = half;
    storage.unknown_01dc = id;
    storage.unknown_2248 = 1;
    storage.unknown_2254 = 5;
    storage.items[3].unknown_004 = 77; /* never reached: index 2 matches first */
    storage.items[2].unknown_000 = word0;
    storage.items[2].unknown_004 = 77;
    storage.items[2].unknown_008 = word8;
    storage.items[2].unknown_00c = wordc;
    storage.items[3].unknown_000 = 0x7fffffff;
    storage.items[3].unknown_00c = 0x7fffffff;
    flush_calls = 0;
    block_calls = 0;
    _vDecodeLPCMThenSendToIOP(0);
}

static void assert_block(int half)
{
    assert(block_calls == 1 && block_args[0] == (int)(DotrSoundControlWord)(region + 0x181800));
    assert(block_args[1] == 0x1000 + (half << 17) && block_args[2] == 0x20000);
    assert(storage.unknown_01d8 == 1 - half && flush_calls == 2);
    assert(storage.unknown_01c8 == 0x800 + 128 * 512 && command_count == 0);
}

static void step_cases(void)
{
    /* Identifier 9127: one command and the stored identifier, nothing else. */
    advance(9127, 0, 0, 0, 10, 0);
    assert(flush_calls == 1 && command == 0x5003 && arguments[0] == 256 && command_count == 1);
    assert(storage.unknown_2248 == 9127 && storage.unknown_01d4 == 10 && block_calls == 0);
    assert(storage.unknown_01c8 == 0x800 && storage.unknown_01d8 == 0);

    /* Missing identifier: only the leading call. */
    advance(78, 0, 0, 0, 10, 0);
    assert(flush_calls == 1 && command_count == 0 && block_calls == 0);
    assert(storage.unknown_01c8 == 0x800 && storage.unknown_01d4 == 10 && storage.unknown_2248 == 1);

    /* Nonzero word +0xc: position past it is reduced by it and raised by word +8. */
    advance(77, 0, 300, 0x4000 + 99, 100, 0);
    assert(storage.unknown_01d4 == 301);
    assert_block(0);
    advance(77, 0, 300, 0x4000 + 100, 100, 1); /* equal: unchanged */
    assert(storage.unknown_01d4 == 0x4000 + 100);
    assert_block(1);

    /* Zero word +0xc: position past a quarter of word +0 calls 0x10b270 and stops. */
    advance(77, (0x4000 + 99) * 4 + 3, 300, 0, 100, 0);
    assert(flush_calls == 2 && block_calls == 0 && storage.unknown_01d8 == 0);
    assert(command == 0x1021 && command_count == 1 && storage.unknown_2254 == -1);
    assert(storage.unknown_01d4 == 0 && storage.unknown_0000 == 0);
    assert(storage.unknown_01c8 == 0x800 + 128 * 512);
    advance(77, (0x4000 + 100) * 4, 300, 0, 100, 1); /* equal: continues */
    assert(storage.unknown_01d4 == 0x4000 + 100 && storage.unknown_2254 == 5);
    assert_block(1);
}

/* SD_iPlayBGMByAutoDMA (branch and loop coverage). */
static void alternate_cases(void)
{
    /* +4 bit 1 clear; id neither in the table nor 9127: 0, nothing stored. */
    set(1, 1, 0, 0);
    storage.items[0].unknown_004 = 5;
    assert(SD_iPlayBGMByAutoDMA(5) == 0 && storage.unknown_2248 == 0 && trace_count == 0);
    set(1, 1, 0, 2);
    assert(SD_iPlayBGMByAutoDMA(6) == 0 && storage.unknown_2248 == 0 && trace_count == 0);
    assert(storage.unknown_01fc == 0);

    /* Current id with +0 bit 8: 0x5004 below 32767 only, bit 13 cleared. */
    set(1, 1, 0x2100, 2);
    storage.items[0].unknown_004 = 5;
    storage.unknown_01dc = 5;
    storage.unknown_01ec = 32766;
    assert(SD_iPlayBGMByAutoDMA(5) == 1 && storage.unknown_2248 == 5);
    assert(trace_count == 1 && command == 0x5004 && arguments[0] == 256);
    assert(storage.unknown_0000 == 0x100 && storage.unknown_01fc == 0);
    set(1, 1, 0x100, 2);
    storage.items[255].unknown_004 = 5;
    storage.unknown_01dc = 5;
    storage.unknown_01ec = 32767;
    assert(SD_iPlayBGMByAutoDMA(5) == 1 && trace_count == 0 && storage.unknown_0000 == 0x100);
    assert(storage.unknown_2248 == 5 && storage.unknown_01fc == 0);

    /* Bit 8 clear, +0x1fc exactly 4, current id: the 0x1020 path. */
    set(1, 1, 0x2000, 2);
    storage.items[7].unknown_004 = 5;
    storage.unknown_01dc = 5;
    storage.unknown_01fc = 4;
    storage.unknown_01d4 = 9;
    storage.unknown_0014 = 0x1414;
    storage.unknown_2254 = -1;
    assert(SD_iPlayBGMByAutoDMA(5) == 1 && storage.unknown_0000 == 0x2100 && storage.unknown_01fc == 0);
    assert(storage.unknown_01d4 == 0 && storage.unknown_2254 == 5 && storage.unknown_2248 == 5);
    assert(trace_count == 1 && command == 0x1020);
    assert(arguments[0] == 0x1414 && arguments[1] == 0x40000 && arguments[2] == 1);

    /* +0x1fc is 6, not 4: the inline 0x10b3c0 body, then bit 1. */
    set(1, 1, 0, 2);
    storage.items[7].unknown_004 = 5;
    storage.unknown_01dc = 5;
    storage.unknown_01fc = 6;
    storage.unknown_2254 = -1;
    assert(SD_iPlayBGMByAutoDMA(5) == 1 && storage.unknown_01fc == 3 && storage.unknown_01e4 == 100);
    assert(storage.unknown_0000 == 0x2000 && storage.unknown_2254 == -1 && storage.unknown_2248 == 5);
    assert(trace_count == 1 && command == 0x5003 && arguments[0] == 256);
    /* +0x1fc is 4 but another id is current; the special id without a table entry. */
    set(1, 1, 0, 2);
    storage.unknown_01dc = 5;
    storage.unknown_01fc = 4;
    assert(SD_iPlayBGMByAutoDMA(9127) == 1 && storage.unknown_01fc == 3 && storage.unknown_2248 == 9127);
    assert(storage.unknown_0000 == 0x2000 && command == 0x5003);
}

static void load_setup(int size, int loop_start, int loop_end, int ring)
{
    memset(&storage, 0, sizeof(storage));
    memset(region, 0x55, 0x100800 + 0x100);
    g_SDValue[0] = &storage;
    g_SDBuffer[0] = region;
    storage.items[3].unknown_000 = size;
    storage.items[3].unknown_004 = 77;
    storage.items[3].unknown_008 = loop_start;
    storage.items[3].unknown_00c = loop_end;
    storage.unknown_01d0 = ring;
    storage.unknown_01cc = -1;
    format_calls = 0;
}

static int read_is(int call, int ring, int offset, int length)
{
    /* Through the real SD_AddReadToEeReq, which aligns the request to 2048-byte sectors. */
    struct DotrSoundQueueRead *read = &storage.reads[call];
    int remainder = offset & 2047;
    return !strcmp(read->name, "bgm") &&
           read->destination == (int)(DotrSoundControlWord)(region + 0x800 + ring) - remainder &&
           read->offset == offset + 0x800 - remainder &&
           read->length == ((length + remainder + 2047) & ~2047);
}

static int all_bytes(int from, int count, int value)
{
    int i;
    for (i = 0; i < count; i++) {
        if ((unsigned char)region[0x800 + from + i] != value) {
            return 0;
        }
    }
    return 1;
}

/* One assertion group per branch outcome and boundary of 0x10a8d0. */
static void load_cases(void)
{
    /* Unknown id: nothing. */
    load_setup(0x1008, 0, 0, 0x40);
    _vLoadBGMFileToEE(78, 0, 0x100);
    assert(format_calls == 0 && storage.read_count == 0 && storage.unknown_01cc == -1);
    /* The last table index is still searched. */
    storage.items[255].unknown_004 = 78;
    storage.items[255].unknown_000 = 0x2000;
    _vLoadBGMFileToEE(78, 0x10, 0x100);
    assert(format_calls == 2 && format_ids[0] == 78 && format_ids[1] == 78);
    assert(storage.read_count == 1 && read_is(0, 0x40, 0x10, 0x100));
    assert(storage.unknown_01d0 == 0x140 && storage.unknown_01cc == 0x110);

    /* Position at the rounded size: nothing. One block below it, ending at the size: one read, no fill. */
    load_setup(0x1008, 0, 0, 0x40);
    _vLoadBGMFileToEE(77, 0x1000, 0x10);
    assert(format_calls == 0 && storage.read_count == 0 && storage.unknown_01cc == -1);
    _vLoadBGMFileToEE(77, 0xff0, 0x10);
    assert(format_calls == 2 && storage.read_count == 1 && read_is(0, 0x40, 0xff0, 0x10));
    assert(storage.unknown_01d0 == 0x50 && storage.unknown_01cc == 0x1000);
    assert(all_bytes(0, 0x200, 0x55));

    /* Ring end: below it one read; reaching it exactly one read and position 0; past it two reads. */
    load_setup(0x4000, 0, 0, 0xfff00);
    _vLoadBGMFileToEE(77, 0x20, 0xf0);
    assert(storage.read_count == 1 && read_is(0, 0xfff00, 0x20, 0xf0) && storage.unknown_01d0 == 0xffff0);
    load_setup(0x4000, 0, 0, 0xfff00);
    _vLoadBGMFileToEE(77, 0x20, 0x100);
    assert(storage.read_count == 1 && read_is(0, 0xfff00, 0x20, 0x100) && storage.unknown_01d0 == 0);
    assert(storage.unknown_01cc == 0x120);
    load_setup(0x4000, 0, 0, 0xfff00);
    _vLoadBGMFileToEE(77, 0x20, 0x180);
    assert(storage.read_count == 2 && read_is(0, 0xfff00, 0x20, 0x100) && read_is(1, 0, 0x120, 0x80));
    assert(storage.unknown_01d0 == 0x80 && storage.unknown_01cc == 0x1a0);

    /* Past the end without a loop: the excess is not read and is zeroed after the data. */
    load_setup(0x1000, 0, 0, 0x40);
    _vLoadBGMFileToEE(77, 0xf00, 0x200);
    assert(storage.read_count == 1 && read_is(0, 0x40, 0xf00, 0x100));
    assert(all_bytes(0, 0x140, 0x55) && all_bytes(0x140, 0x100, 0) && all_bytes(0x240, 0x100, 0x55));
    assert(storage.unknown_01d0 == 0x140 && storage.unknown_01cc == 0x1000);
    /* Excess of one block: one store; excess below one block: none. */
    load_setup(0x1000, 0, 0, 0x40);
    _vLoadBGMFileToEE(77, 0xf00, 0x110);
    assert(all_bytes(0x140, 0x10, 0) && all_bytes(0x150, 0x100, 0x55) && read_is(0, 0x40, 0xf00, 0x100));
    load_setup(0x1000, 0, 0, 0x40);
    _vLoadBGMFileToEE(77, 0xf00, 0x108);
    assert(all_bytes(0, 0x400, 0x55) && storage.read_count == 1 && read_is(0, 0x40, 0xf00, 0x100));
    assert(storage.unknown_01cc == 0x1000);
    /* The fill is capped at 0x20000 bytes (excess 0x20000 itself is not capped). */
    load_setup(0x1000, 0, 0, 0);
    _vLoadBGMFileToEE(77, 0, 0x1000 + 0x20000);
    assert(all_bytes(0x1000, 0x20000, 0) && all_bytes(0x21000, 0x100, 0x55));
    load_setup(0x1000, 0, 0, 0);
    _vLoadBGMFileToEE(77, 0, 0x1000 + 0x20010);
    assert(all_bytes(0x1000, 0x20000, 0) && all_bytes(0x21000, 0x100, 0x55));
    assert(storage.read_count == 1 && read_is(0, 0, 0, 0x1000) && storage.unknown_01cc == 0x1000);
    /* The fill wraps at the ring end. */
    load_setup(0x1000, 0, 0, 0xffe00);
    _vLoadBGMFileToEE(77, 0xf00, 0x300);
    assert(all_bytes(0xfff00, 0x100, 0) && all_bytes(0, 0x100, 0) && all_bytes(0x100, 0x100, 0x55));
    assert(all_bytes(0xffe00, 0x100, 0x55) && all_bytes(0x100000, 0x100, 0x55));
    assert(storage.read_count == 1 && read_is(0, 0xffe00, 0xf00, 0x100) && storage.unknown_01d0 == 0xfff00);

    /* Loop end 0x1000, loop start 0x400 (item words times four, rounded down to 16). */
    load_setup(0x1000, 0x103, 0x403, 0x40);
    _vLoadBGMFileToEE(77, 0xe00, 0x200);
    assert(format_calls == 2 && storage.read_count == 1 && read_is(0, 0x40, 0xe00, 0x200));
    assert(storage.unknown_01cc == 0x1000);
    load_setup(0x1000, 0x103, 0x403, 0x40);
    _vLoadBGMFileToEE(77, 0xe00, 0x500);
    assert(format_calls == 3 && storage.read_count == 2);
    assert(read_is(0, 0x40, 0xe00, 0x200) && read_is(1, 0x240, 0x400, 0x300));
    assert(storage.unknown_01d0 == 0x540 && storage.unknown_01cc == 0x700);
    assert(all_bytes(0, 0x800, 0x55));
    load_setup(0x1000, 0x103, 0x403, 0x40);
    _vLoadBGMFileToEE(77, 0xe00, 0xf00);
    assert(format_calls == 4 && storage.read_count == 3 && read_is(0, 0x40, 0xe00, 0x200));
    assert(read_is(1, 0x240, 0x400, 0xc00) && read_is(2, 0xe40, 0x400, 0x100));
    assert(storage.unknown_01d0 == 0xf40 && storage.unknown_01cc == 0x500);
}

int main(void)
{
    assert(offsetof(struct DotrSoundQueueManager, unknown_0004) == 4);
    assert(offsetof(struct DotrSoundQueueManager, unknown_2a6c) == 0x2a6c);

    /* Guard: a null pointer or a zero enable word suppresses everything, even
     * with every bit set.
     */
    set(0, 1, -1, -1);
    all_guarded();
    set(1, 0, -1, -1);
    all_guarded();

    /* 0x10a8a0 and 0x109f10. */
    set(1, 1, 0x2000, 0);
    assert(SD_iGetSDStatus() == 0x2000 && SD_IsLoadingNow() == 1);
    set(1, 1, ~0x2000, 0);
    assert(SD_IsLoadingNow() == 0 && SD_iGetSDStatus() == ~0x2000L);

    /* 0x109f60: command then stored value, with no bit condition. */
    set(1, 1, 0, 0);
    SD_SetOutputType(-9);
    assert(trace_count == 1 && command == 0xf003 && arguments[0] == -9 && storage.unknown_2a6c == -9);

    /* 0x10a010: needs W4 bit 1; the stop call needs W0 bit 0; bit 0x200 is cleared. */
    set(1, 1, 0x201, 0);
    SD_BGMStop();
    assert(trace_count == 0 && storage.unknown_0000 == 0x201);
    set(1, 1, 0x200, 2);
    SD_BGMStop();
    assert(trace_count == 0 && storage.unknown_0000 == 0);
    set(1, 1, 0x7fffffff, 2);
    SD_BGMStop();
    /* Through the real 0x10b270: bit 0x100 is set, so it sends 0x1021. */
    assert(trace_count == 1 && command == 0x1021 && storage.unknown_0000 == (0x7fffffff & ~0x300));
    assert(storage.unknown_2254 == -1);

    /* 0x10a090: 16-bit signed argument. */
    set(1, 1, 0, 0);
    SD_BGMFadeOut(7);
    assert(trace_count == 0);
    set(1, 1, 0, 2);
    SD_BGMFadeOut((short)0xfffe);
    assert(command == 0x5003 && arguments[0] == -2);

    /* 0x10a160: both bits, absolute value of the second argument only. */
    set(1, 1, 1, 0);
    SD_BGMSetSourceLevel(3, 4);
    set(1, 1, 0, 2);
    SD_BGMSetSourceLevel(3, 4);
    assert(trace_count == 0);
    set(1, 1, 1, 2);
    SD_BGMSetSourceLevel(-3, -4);
    assert(command == 0x5005 && arguments[0] == -3 && arguments[1] == 4);
    SD_BGMSetSourceLevel(3, 9);
    assert(arguments[1] == 9 && trace_count == 2);

    /* 0x10a1d0, 0x10a3b0, 0x10a450: W4 bit 2, W0 bit 0 and W0 bit 0x80. */
    set(1, 1, 0x81, 0);
    SD_SEPlay2(1, 2, 3); SD_SEStop(1, 2); SD_SEStopAll();
    set(1, 1, 0x80, 4);
    SD_SEPlay2(1, 2, 3); SD_SEStop(1, 2); SD_SEStopAll();
    assert(trace_count == 0);
    set(1, 1, 0x01, 4);
    SD_SEPlay2(1, 2, 3); SD_SEStop(1, 2); SD_SEStopAll();
    assert(trace_count == 0);
    set(1, 1, 0x81, 4);
    SD_SEPlay2(11, 22, 33);
    assert(command == 0x5210 && arguments[0] == 11 && arguments[1] == 22);
    assert(arguments[2] == 33 && arguments[3] == 0);
    SD_SEStop(44, 55);
    assert(command == 0x5201 && arguments[0] == 44 && arguments[1] == 55);
    SD_SEStopAll();
    assert(command == 0x5202 && trace_count == 3);

    /* 0x109fc0: 0x10a010 first (stop command, clears), then 0x10a450. */
    set(1, 1, 0x381, 6);
    SD_StopAll();
    assert(trace_count == 2 && command == 0x5202 && storage.unknown_0000 == 0x81);
    assert(storage.unknown_2254 == -1);

    /* 0x10a280: same guard and conditions as 0x10a1d0; two packed words,
     * low byte first. 0x10a390 passes zeros for the middle three arguments.
     */
    set(1, 1, 0x81, 0);
    SD_SEPlayWithParam6(1, 2, 3, 4, 5, 6);
    set(1, 0, 0x81, 4);
    SD_SEPlayWithParam6(1, 2, 3, 4, 5, 6);
    assert(trace_count == 0);
    set(1, 1, 0x81, 4);
    SD_SEPlayWithParam6(0x101, 0x22, 0xff, 0x85, 0x33, 0x44);
    assert(command == 0x5220 && arguments[0] == 0x33222201 && arguments[1] == 0x44057f01);
    SD_SEPlayOnlyOnce(7, 8, 9);
    assert(trace_count == 2 && arguments[0] == 0x00080807 && arguments[1] == 0x09000000);

    initializer_cases();
    state_cases();
    update_cases();
    sample_cases();
    ring_cases();
    step_cases();
    load_cases();
    puts("sound_control: ok");
    alternate_cases();
    return 0;
}
