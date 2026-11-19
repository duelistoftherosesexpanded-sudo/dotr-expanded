/* Synthetic host checks for the sound-file block reader (0x10c3d0) and its
 * chained-read callback (0x10c580). Every dependency is a stub whose semantics
 * are assumptions; the scratch address is recorded, never dereferenced.
 */
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/sound_file.h"

struct DotrSoundFileRead s_iUsingNow;
char gSdfs_sa[64] = "<stored name>";
char data_002b90da[] = "<enter>", data_002b9102[] = "<open>", data_002b9112[] = "<block>";
char data_002b9127[] = "<at once>", data_002b9135[] = "<close>", data_002b9141[] = "<finished>";
char data_002b9168[] = "<next block>";
char data_002b90c0[] = "<start>";

#define SCRATCH ((char *)0x4000000)
enum { PRINT = 1, OPEN, READ, COPY, CLOSE, SUBMIT };
struct Event {
    int kind;
    const void *p, *q;
    int a, b, c, d;
};
static struct Event trace[32];
static int trace_count, open_result, submit_result = 1;

static struct Event *add(int kind)
{
    assert(trace_count < 32);
    memset(&trace[trace_count], 0, sizeof(trace[0]));
    trace[trace_count].kind = kind;
    return &trace[trace_count++];
}

char data_0030f600[] = "<no block>", data_0030f61c[] = "<block at>";
char _0_buf[0x800];
char data_0030f645[] = "<head>", data_0030f676[] = "<head words>", data_0030f7b1[] = "<tag>";
static int print_words[4][2], print_word_calls;
static int feed_prints, feed_args[16];
static int dec_stop_prints;
int func_00199b50(const char *format, ...)
{
    if (format == data_0030f8b0) {
        dec_stop_prints++;
        return 0;
    }
    if (format == data_0030f645 || format == data_0030f676 || format == data_0030f7b1) {
        int count = format == data_0030f645 ? 0 : format == data_0030f676 ? 11 : 5;
        int base = format == data_0030f7b1 ? 11 : 0, i;
        va_list args;

        assert(feed_prints++ == (format == data_0030f645 ? 0 : format == data_0030f676 ? 1 : 2));
        va_start(args, format);
        for (i = 0; i < count; i++) {
            feed_args[base + i] = va_arg(args, int);
        }
        va_end(args);
        return 0;
    }
    add(PRINT)->p = format;
    if (format == data_0030f600 || format == data_0030f61c) {
        va_list args;

        va_start(args, format);
        print_words[print_word_calls & 3][0] = va_arg(args, int);
        print_words[print_word_calls & 3][1] = format == data_0030f61c ? va_arg(args, int) : -1;
        print_word_calls++;
        va_end(args);
    }
    return 0;
}

int RhFile_OpenFile(char *name)
{
    add(OPEN)->p = name;
    return open_result;
}

int RhFile_ReadAndBlock(int unknown, int byte_offset, int byte_size, int unknown_014)
{
    struct Event *event = add(READ);
    event->a = unknown;
    event->b = byte_offset;
    event->c = byte_size;
    event->d = unknown_014;
    return 1;
}

void *func_00198790(void *destination, const void *source, int byte_count)
{
    struct Event *event = add(COPY);
    event->p = destination;
    event->q = source;
    event->a = byte_count;
    return destination;
}

int RhFile_CloseFile(int unknown)
{
    add(CLOSE)->a = unknown;
    return 1;
}

int RhFile_ReadAndCallbackByFileName(char *name, int byte_offset, int byte_size, int unknown_014,
                  void (*function)(int), int unknown_01c, int flag)
{
    struct Event *event = add(SUBMIT);
    assert(function == lSDFS_FinishCallback);
    event->p = name;
    event->a = byte_offset;
    event->b = byte_size;
    event->c = unknown_014;
    event->d = unknown_01c * 1000 + flag;
    return submit_result;
}

static void expect_kinds(const int *kinds, int count)
{
    int i;
    assert(trace_count == count);
    for (i = 0; i < count; i++) {
        assert(trace[i].kind == kinds[i]);
    }
}

static void expect_read(int position, int handle, int offset, int size, int destination)
{
    assert(trace[position].kind == READ && trace[position].a == handle);
    assert(trace[position].b == offset && trace[position].c == size && trace[position].d == destination);
}

static void expect_copy(int position, const void *destination, const void *source, int size)
{
    assert(trace[position].kind == COPY && trace[position].p == destination);
    assert(trace[position].q == source && trace[position].a == size);
}

static void reader_cases(void)
{
    static char name[] = "<name>";
    static _Alignas(16) char buffer[32];
    static const int refused[] = {PRINT, PRINT, OPEN};
    static const int at_once[] = {PRINT, PRINT, OPEN, PRINT, READ, PRINT, CLOSE};
    static const int one_block[] = {PRINT, PRINT, OPEN, PRINT, READ, COPY, PRINT, CLOSE};
    static const int no_block[] = {PRINT, PRINT, OPEN, PRINT, CLOSE};
    static const int three_blocks[] = {PRINT, PRINT, OPEN, PRINT, READ, COPY, PRINT, READ, COPY,
                                       PRINT, READ, COPY, PRINT, CLOSE};

    /* A negative open result returns 0 without reading or closing. */
    trace_count = 0;
    open_result = -1;
    assert(SDFS_ReadToEEBlock(name, buffer, 0, 2048) == 0);
    expect_kinds(refused, 3);
    assert(trace[0].p == data_002b90da && trace[1].p == data_002b9102 && trace[2].p == name);

    /* 16-byte aligned destination and sector-aligned offset and size: one
     * direct read into the destination. A negative multiple of 2048 counts.
     */
    trace_count = 0;
    open_result = 3;
    assert(SDFS_ReadToEEBlock(name, buffer, -2048, 4096) == 1);
    expect_kinds(at_once, 7);
    expect_read(4, 3, -2048, 4096, (int)(intptr_t)buffer);
    assert(trace[3].p == data_002b9127 && trace[5].p == data_002b9135 && trace[6].a == 3);

    /* Unaligned offset: read the containing sectors to the scratch address and
     * copy the requested bytes from inside them.
     */
    trace_count = 0;
    assert(SDFS_ReadToEEBlock(name, buffer, 0x805, 100) == 1);
    expect_kinds(one_block, 8);
    expect_read(4, 3, 0x800, 0x800, 0x4000000);
    expect_copy(5, buffer, SCRATCH + 5, 100);

    /* A misaligned destination alone also selects the scratch path. */
    trace_count = 0;
    assert(SDFS_ReadToEEBlock(name, buffer + 1, 0x800, 0x800) == 1);
    expect_kinds(one_block, 8);
    expect_read(4, 3, 0x800, 0x800, 0x4000000);
    expect_copy(5, buffer + 1, SCRATCH, 0x800);

    /* An unaligned request with nothing to read still closes and returns 1. */
    trace_count = 0;
    assert(SDFS_ReadToEEBlock(name, buffer, 1, 0) == 1);
    expect_kinds(no_block, 5);
    trace_count = 0;
    assert(SDFS_ReadToEEBlock(name, buffer, 1, -5) == 1);
    expect_kinds(no_block, 5);

    /* Blocks are limited to 0x80000 bytes including the leading skip; the
     * destination advances by the bytes copied.
     */
    trace_count = 0;
    assert(SDFS_ReadToEEBlock(name, buffer + 1, 0x10, 0x100000) == 1);
    expect_kinds(three_blocks, 14);
    expect_read(4, 3, 0, 0x80000, 0x4000000);
    expect_copy(5, buffer + 1, SCRATCH + 0x10, 0x7fff0);
    expect_read(7, 3, 0x80000, 0x80000, 0x4000000);
    expect_copy(8, buffer + 1 + 0x7fff0, SCRATCH, 0x80000);
    expect_read(10, 3, 0x100000, 0x800, 0x4000000);
    expect_copy(11, buffer + 1 + 0xffff0, SCRATCH, 0x10);
}

static void callback_cases(void)
{
    static char buffer[16];
    static const int idle[] = {PRINT};
    static const int last[] = {PRINT, COPY};
    static const int chained[] = {PRINT, COPY, PRINT, SUBMIT};
    static const int first[] = {PRINT, PRINT, SUBMIT};

    /* Not busy: only the message. The argument is unused. */
    memset(&s_iUsingNow, 0, sizeof(s_iUsingNow));
    s_iUsingNow.remaining = 5;
    trace_count = 0;
    lSDFS_FinishCallback(123);
    expect_kinds(idle, 1);
    assert(trace[0].p == data_002b9141 && s_iUsingNow.remaining == 5);

    /* Final block: copy it out and clear the busy word. */
    s_iUsingNow.busy = 1;
    s_iUsingNow.destination = buffer;
    s_iUsingNow.skip = 7;
    s_iUsingNow.chunk = 9;
    s_iUsingNow.remaining = 0;
    trace_count = 0;
    lSDFS_FinishCallback(0);
    expect_kinds(last, 2);
    expect_copy(1, buffer, SCRATCH + 7, 9);
    assert(s_iUsingNow.busy == 0 && s_iUsingNow.destination == buffer);

    /* More than one block left: copy, advance, queue a full 0x80000 block. */
    s_iUsingNow.busy = 1;
    s_iUsingNow.offset = 0x1000;
    s_iUsingNow.remaining = 0x80001;
    trace_count = 0;
    lSDFS_FinishCallback(0);
    expect_kinds(chained, 4);
    assert(trace[2].p == data_002b9168 && trace[3].p == gSdfs_sa);
    assert(trace[3].a == 0x1000 && trace[3].b == 0x80000 && trace[3].c == 0x4000000);
    assert(trace[3].d == 32);
    assert(s_iUsingNow.destination == buffer + 9 && s_iUsingNow.skip == 0);
    assert(s_iUsingNow.chunk == 0x80000 && s_iUsingNow.offset == 0x81000);
    assert(s_iUsingNow.remaining == 1 && s_iUsingNow.busy == 1);

    /* Last partial block: rounded up to whole sectors; offset and remaining clear. */
    s_iUsingNow.destination = 0; /* no copy without a destination */
    s_iUsingNow.chunk = 0;
    s_iUsingNow.remaining = 100;
    trace_count = 0;
    lSDFS_FinishCallback(0);
    expect_kinds(first, 3);
    assert(trace[2].a == 0x81000 && trace[2].b == 0x800);
    assert(s_iUsingNow.chunk == 100 && s_iUsingNow.offset == 0 && s_iUsingNow.remaining == 0);
    assert(s_iUsingNow.busy == 1);
}

static void start_cases(void)
{
    static char name[] = "<name>";
    static _Alignas(16) char buffer[32];
    static const int started[] = {PRINT, SUBMIT};

    /* 0x10c2c0 returns the busy word. */
    memset(&s_iUsingNow, 0, sizeof(s_iUsingNow));
    assert(SDFS_IsUsingNow() == 0);
    s_iUsingNow.busy = 5;
    assert(SDFS_IsUsingNow() == 5);

    /* Aligned request: clear the five chain words, queue one read straight
     * into the destination with the chained-read callback, mark busy.
     */
    memset(&s_iUsingNow, 0x55, sizeof(s_iUsingNow));
    s_iUsingNow.busy = 0;
    trace_count = 0;
    assert(SDFS_ReadToEE(name, buffer, 0x800, 0x1000) == 1);
    expect_kinds(started, 2);
    assert(trace[0].p == data_002b90c0 && trace[1].p == name);
    assert(trace[1].a == 0x800 && trace[1].b == 0x1000 && trace[1].c == (int)(intptr_t)buffer);
    assert(trace[1].d == 32 && s_iUsingNow.busy == 1);
    assert(s_iUsingNow.destination == 0 && s_iUsingNow.skip == 0 && s_iUsingNow.chunk == 0);
    assert(s_iUsingNow.offset == 0 && s_iUsingNow.remaining == 0);

    /* A refused submission returns -1 and leaves the busy word alone. */
    s_iUsingNow.busy = 0;
    submit_result = 0;
    trace_count = 0;
    assert(SDFS_ReadToEE(name, buffer, 0, 0) == -1);
    expect_kinds(started, 2);
    assert(s_iUsingNow.busy == 0);
    submit_result = 1;

    /* Unaligned input: nothing is queued or cleared,
     * yet the busy word is set and 1 is returned.
     */
    s_iUsingNow.remaining = 9;
    trace_count = 0;
    assert(SDFS_ReadToEE(name, buffer + 1, 0, 0) == 1 && trace_count == 0);
    assert(s_iUsingNow.busy == 1 && s_iUsingNow.remaining == 9);
    s_iUsingNow.busy = 0;
    assert(SDFS_ReadToEE(name, buffer, 1, 0) == 1 && trace_count == 0 && s_iUsingNow.busy == 1);
    s_iUsingNow.busy = 0;
    assert(SDFS_ReadToEE(name, buffer, 0, 2047) == 1 && trace_count == 0 && s_iUsingNow.busy == 1);
}

/* 0x242f20 and its byte-identical sibling 0x245630: one case per path; each copy is recorded
 * as (destination, source, count). */
static void check_split(const struct Event *e, int index, const void *dest, const void *src, int count)
{
    assert(trace[index].kind == COPY && trace[index].p == dest && trace[index].q == src);
    assert(trace[index].a == count);
    (void)e;
}

typedef int (*SplitFn)(char *, int, char *, int, char *, int, char *, int);

static void split_cases(SplitFn split)
{
    char d1[1], d2[1], a[1], b[1];

    trace_count = 0;
    assert(split(d1, 4, d2, 3, a, 5, b, 3) == 0 && trace_count == 0);
    assert(split(d1, 4, d2, 4, a, 5, b, 3) == 8 && trace_count == 3);

    trace_count = 0;
    assert(split(d1, 4, d2, 6, a, 5, b, 3) == 8 && trace_count == 3);
    check_split(0, 0, d1, a, 4);
    check_split(0, 1, d2, a + 4, 1);
    check_split(0, 2, d2 + 1, b, 3);

    trace_count = 0;
    assert(split(d1, 4, d2, 6, a, 4, b, 2) == 6 && trace_count == 3);
    check_split(0, 0, d1, a, 4);
    check_split(0, 1, d2, a + 4, 0);
    check_split(0, 2, d2, b, 2);

    trace_count = 0;
    assert(split(d1, 6, d2, 6, a, 2, b, 6) == 8 && trace_count == 3);
    check_split(0, 0, d1, a, 2);
    check_split(0, 1, d1 + 2, b, 4);
    check_split(0, 2, d2, b + 4, 2);

    trace_count = 0;
    assert(split(d1, 6, d2, 6, a, 2, b, 4) == 6 && trace_count == 3);
    check_split(0, 0, d1, a, 2);
    check_split(0, 1, d1 + 2, b, 4);
    check_split(0, 2, d2, b + 4, 0);

    trace_count = 0;
    assert(split(d1, 6, d2, 6, a, 2, b, 3) == 5 && trace_count == 2);
    check_split(0, 0, d1, a, 2);
    check_split(0, 1, d1 + 2, b, 3);
}

/* 0x243390, branch coverage: mode 0, mode != 0 without and with wrap. */
static void ring_cases(void)
{
    struct DotrSoundRing ring;
    int s1, c1, s2, c2;
    memset(&ring, 0, sizeof(ring));
    ring.mode = 0;
    ring.unknown_2c = 12;
    ring.unknown_30 = 0x1000;
    ring.unknown_3c = 64;
    audioDecBeginPut(&ring, &s1, &c1, &s2, &c2);
    assert(s1 == (int)((intptr_t)&ring.unknown_004[0] + 12) && c1 == 28 && s2 == 0x1000 && c2 == 64);
    ring.mode = 1;
    ring.unknown_34 = 8;
    ring.unknown_38 = 40;
    audioDecBeginPut(&ring, &s1, &c1, &s2, &c2);
    assert(s1 == 0x1008 && c1 == 24 && s2 == 0 && c2 == 0);
    ring.unknown_38 = 8; /* span 56, tail 56: equal does not wrap */
    audioDecBeginPut(&ring, &s1, &c1, &s2, &c2);
    assert(s1 == 0x1008 && c1 == 56 && s2 == 0 && c2 == 0);
    ring.unknown_38 = 4; /* span 60 > tail 56: wraps */
    audioDecBeginPut(&ring, &s1, &c1, &s2, &c2);
    assert(s1 == 0x1008 && c1 == 56 && s2 == 0x1000 && c2 == 4);
}

static int port_args[4][4], port_calls;
void func_00190810(int a, int b, int c, int d)
{
    int v[4] = {a, b, c, d};
    memcpy(port_args[port_calls++ & 3], v, sizeof(v));
}

/* The stream and voice views overlay the same host storage. */
static union {
    struct DotrSoundStream s;
    struct DotrSoundVoice v;
} stream_u;
#define stream stream_u.s
static int lock_calls, lock_arg, lock_word, unlock_calls, unlock_arg, unlock_word;
static long unlock_total;
void func_0017b920(int unknown) { lock_calls++; lock_arg = unknown; lock_word = stream.unknown_14; }
void func_0017b900(int unknown)
{
    assert(lock_calls == unlock_calls + 1);
    unlock_calls++;
    unlock_arg = unknown;
    unlock_word = stream.unknown_14;
    unlock_total = stream.unknown_48;
}

static int sema_ok, sema_calls, setup_calls, setup_word;
static long setup_total;
int func_0017b8e0(struct DotrSoundSema *sema)
{
    sema_ok = sema->unknown_4 == 1 && sema->unknown_8 == 1;
    sema_calls++;
    return 0x5e;
}
unsigned long dotr_sound_host_memory[0x2000];
volatile int dotr_sound_host_regs[0x1000];
#define SOUND_REG(a) dotr_sound_host_regs[((a) & 0xffff) >> 4]
static int setup_tag_addr, setup_ctrl, setup_enable;
static int delete_calls, delete_arg, delete_ctrl, delete_tag;
void func_0017b8f0(int unknown)
{
    delete_calls++;
    delete_arg = unknown;
    delete_ctrl = dotr_sound_host_regs[0xb40];
    delete_tag = setup_tag_addr;
}
static int big_enter, big_leave, big_enter_count, big_leave_index;

/* viBufCreate (straight-line coverage): every stored word, the address
 * conversion, call order and the result. */
static void stream_init_case(void)
{
    unsigned long *tags = (unsigned long *)((char *)dotr_sound_host_memory + 0x4560);
    struct DotrSoundSlot *slots = (struct DotrSoundSlot *)((char *)dotr_sound_host_memory + 0x100);
    int i;

    /* viBufCreate (straight-line coverage) with the real 0x243f60 behind it (branch and loop
     * coverage: three blocks and two slots here, no block and no slot below). The stores
     * to 0x1000f590 before the control write are not observable on the host. */
    memset(&stream, 0xff, sizeof(stream));
    memset(dotr_sound_host_memory, 0x55, sizeof(dotr_sound_host_memory));
    sema_calls = setup_calls = 0;
    big_enter = big_leave = 0;
    SOUND_REG(0x1000b420) = 0x77;
    SOUND_REG(0x1000b400) = 0x77;
    SOUND_REG(0x1000f520) = 0x10001;
    assert(viBufCreate(&stream, 7, 0xf1234560u, 3, 0x100, 2) == 1);
    assert(stream.unknown_00 == 7 && stream.unknown_04 == 0x21234560 && stream.unknown_08 == 3);
    assert(stream.unknown_18 == 3 << 11 && stream.unknown_50 == 0x100 && stream.unknown_54 == 2);
    assert(sema_calls == 1 && sema_ok && stream.unknown_40 == 0x5e);
    assert(setup_calls == 1 && setup_word == 0x5e && setup_total == -1L && stream.unknown_48 == 0);
    assert(stream.unknown_0c == 0 && stream.unknown_10 == 0 && stream.unknown_14 == 0);
    assert(stream.unknown_44 == 1 && stream.unknown_58 == 0 && stream.unknown_5c == 0);
    for (i = 0; i < 2; i++) {
        assert(slots[i].unknown_00 == -1L && slots[i].unknown_08 == -1L);
        assert(slots[i].unknown_10 == 0 && slots[i].unknown_14 == 0);
    }
    assert(slots[2].unknown_00 == 0x5555555555555555L && slots[2].unknown_10 == 0x55555555);
    assert(tags[0] == ((7UL << 32) | 0x30000080) && tags[2] == ((0x807UL << 32) | 0x30000080));
    assert(tags[4] == ((0x1007UL << 32) | 0x30000080) && tags[6] == ((0x1234560UL << 32) | 0x20000000));
    assert(tags[1] == 0x5555555555555555UL && tags[5] == 0x5555555555555555UL && tags[8] == 0x5555555555555555UL);
    assert(SOUND_REG(0x1000b420) == 0 && SOUND_REG(0x1000b410) == 7 && SOUND_REG(0x1000b430) == 0x1234560);
    assert(setup_tag_addr == 0x1234560 && setup_ctrl == 0x77 && SOUND_REG(0x1000b400) == 5);
    assert(setup_enable == 1 && big_enter == 1 && big_leave == 1);

    memset(dotr_sound_host_memory, 0x55, sizeof(dotr_sound_host_memory));
    stream.unknown_00 = (int)0xf0000010;
    stream.unknown_08 = 0;
    stream.unknown_54 = 0;
    stream.unknown_0c = stream.unknown_5c = 9;
    SOUND_REG(0x1000f520) = 0;
    assert(viBufReset(&stream) == 1);
    assert(slots[0].unknown_00 == 0x5555555555555555L && stream.unknown_0c == 0 && stream.unknown_5c == 0);
    assert(tags[0] == ((0x1234560UL << 32) | 0x20000000) && tags[2] == 0x5555555555555555UL);
    assert(SOUND_REG(0x1000b410) == 0x10 && setup_enable == 0 && big_leave == 2);

    /* viBufStopDMA (straight-line coverage for the stores and call order; the wait loop is
     * taken only with bits 4-7 of 0x10002010 clear: a set bit never clears on the host). */
    memset(&stream, 0xff, sizeof(stream));
    stream.unknown_40 = 0x5e;
    lock_calls = unlock_calls = 0;
    setup_calls = 0;
    SOUND_REG(0x1000b410) = 0x11;
    SOUND_REG(0x1000b430) = 0x12;
    SOUND_REG(0x1000b420) = 0x13;
    SOUND_REG(0x1000b400) = 0x77;
    SOUND_REG(0x1000b010) = 0x21;
    SOUND_REG(0x1000b020) = 0x22;
    SOUND_REG(0x1000b000) = 0x23;
    SOUND_REG(0x10002020) = 0x31;
    SOUND_REG(0x10002010) = 0x70f;
    SOUND_REG(0x1000f520) = 0x10004;
    assert(viBufStopDMA(&stream) == 1);
    assert(lock_calls == 1 && unlock_calls == 1 && lock_arg == 0x5e && stream.unknown_44 == 0);
    assert(setup_calls == 2 && big_enter == 4 && big_leave == 4 && setup_enable == 4);
    assert(stream.unknown_1c[0] == 0x11 && stream.unknown_1c[1] == 0x12 && stream.unknown_1c[2] == 0x13);
    assert(stream.unknown_1c[3] == 5 && stream.unknown_1c[4] == 0x21 && stream.unknown_1c[5] == 0x22);
    assert(stream.unknown_1c[6] == 0 && stream.unknown_1c[7] == 0x31 && stream.unknown_1c[8] == 0x70f);
    assert(SOUND_REG(0x1000b000) == 0 && stream.unknown_18 == -1 && stream.unknown_40 == 0x5e);

    /* viBufDelete (straight-line coverage). */
    SOUND_REG(0x1000b400) = 0x77;
    SOUND_REG(0x1000f520) = 0x10008;
    delete_calls = 0;
    assert(viBufDelete(&stream) == 1);
    assert(delete_calls == 1 && delete_arg == 0x5e && delete_ctrl == 5 && delete_tag == 0x12);
    assert(big_enter == 5 && big_leave == 5 && setup_ctrl == 0x77 && setup_enable == 8);
    assert(SOUND_REG(0x1000b420) == 0 && SOUND_REG(0x1000b410) == 0 && SOUND_REG(0x1000b430) == 0);
    big_enter = big_leave = 0;
}

static int job_log[8], job_count, job_block[4], job_flag, job_waits, job_wait_arg;
static int job_blocks[4][3], job_starts;
void func_0017bb60(int unknown) { job_log[job_count++ & 7] = 1; job_flag = unknown; }
int func_0017bca0(int *block, int unknown)
{
    assert(unknown == 1);
    memcpy(job_block, block, sizeof(job_block));
    memcpy(job_blocks[job_starts++ & 3], block, sizeof(job_blocks[0]));
    job_log[job_count++ & 7] = 2;
    return 0x33;
}
int func_0017bc80(int unknown)
{
    static const int results[3] = {0, 3, -1};
    job_wait_arg = unknown;
    job_log[job_count++ & 7] = 3;
    return results[job_waits++ % 3];
}

static int pos_calls, pos_state, pos_args[6], play_args[7], play_calls;
void changeInputVolume(int value) { assert(value == 0); pos_calls++; pos_state = stream.unknown_00; }
unsigned func_00190810_ret6(int a, int b, int c, int d, int e, int f)
{
    int v[6] = {a, b, c, d, e, f};
    assert(pos_calls == 1 && play_calls == 0);
    memcpy(pos_args, v, sizeof(v));
    return 0xab000500u;
}
void func_00190810_args7(int a, int b, int c, int d, int e, int f, int g)
{
    int v[7] = {a, b, c, d, e, f, g};
    assert(stream.unknown_50 == 0x400);
    memcpy(play_args, v, sizeof(v));
    play_calls++;
}

/* Set by the viBufRestartDMA cases, which write other channel-4 control values. */
static int leave_any_ctrl;
static struct DotrSoundBig big_items[2];
static struct DotrSoundBigQueue big_queue;
void func_001843e0(void)
{
    big_enter++;
    big_enter_count = big_queue.count;
    setup_calls++;
    setup_word = stream.unknown_40;
    setup_total = stream.unknown_48;
    setup_tag_addr = SOUND_REG(0x1000b430);
    setup_ctrl = SOUND_REG(0x1000b400);
}
void func_00184428(void)
{
    assert(big_enter == big_leave + 1);
    big_leave++;
    big_leave_index = big_queue.index;
    setup_enable = SOUND_REG(0x1000f590);
    assert(leave_any_ctrl || SOUND_REG(0x1000b400) == 5);
}

static void stream_more_cases(void)
{
    static const int pos_want[6] = {1, 0x80e0, 0, 2, 0, 0};
    static const int play_want[7] = {1, 0x80d0, 0, 0, 0x58, 16384, 2048};

    /* sendToIOP (branch and loop coverage): n = 0 and -1 rejected; n = 5
     * waits until the first negative result (third call). */
    job_count = job_waits = 0;
    assert(sendToIOP(10, 20, 0) == 0 && sendToIOP(10, 20, -1) == 0 && job_count == 0);
    job_flag = -1;
    assert(sendToIOP(10, 20, 5) == 5 && job_count == 5 && job_waits == 3);
    assert(job_log[0] == 1 && job_log[1] == 2 && job_log[2] == 3 && job_log[4] == 3 && job_flag == 0);
    assert(job_block[0] == 20 && job_block[1] == 10 && job_block[2] == 5 && job_block[3] == 0);
    assert(job_wait_arg == 0x33);

    /* audioDecPause (straight-line coverage): stores, call order and arguments. */
    memset(&stream, 0, sizeof(stream));
    stream.unknown_44 = 0x100;
    stream.unknown_58 = 0x58;
    pos_calls = play_calls = 0;
    audioDecPause(&stream);
    assert(pos_calls == 1 && pos_state == 3 && stream.unknown_00 == 3 && stream.unknown_50 == 0x400);
    assert(memcmp(pos_args, pos_want, sizeof(pos_want)) == 0);
    assert(play_calls == 1 && memcmp(play_args, play_want, sizeof(play_want)) == 0);

    /* voBufIncCount (straight-line coverage, plus the index wrap): last item
     * wraps to 0, first item advances to 1. */
    big_queue.items = big_items;
    big_queue.index = 1;
    big_queue.count = 7;
    big_queue.capacity = 2;
    big_enter = big_leave = 0;
    voBufIncCount(&big_queue);
    assert(big_items[1].state == 2 && big_items[0].state == 0 && big_queue.count == 8 && big_queue.index == 0);
    assert(big_enter == 1 && big_leave == 1 && big_enter_count == 7 && big_leave_index == 0);
    voBufIncCount(&big_queue);
    assert(big_items[0].state == 2 && big_queue.count == 9 && big_queue.index == 1 && big_leave == 2);
}

/* audioDecReset (straight-line coverage): the 0x2431b0 sequence, then the
 * cleared words; untouched neighbours keep their value. */
static void voice_case(void)
{
    struct DotrSoundVoice *voice = &stream_u.v;
    memset(&stream_u, 0x11, sizeof(stream_u));
    voice->unknown_44 = 0x100;
    voice->unknown_58 = 0x58;
    pos_calls = play_calls = 0;
    audioDecReset(voice);
    assert(pos_calls == 1 && pos_state == 3 && play_calls == 1 && play_args[4] == 0x58);
    assert(voice->unknown_00 == 0 && voice->unknown_2c == 0 && voice->unknown_34 == 0 && voice->unknown_38 == 0);
    assert(voice->unknown_40 == 0 && voice->unknown_54 == 0 && voice->unknown_4c == 0 && voice->unknown_50 == 0);
    assert(voice->unknown_30 == 0x11111111 && voice->unknown_3c == 0x11111111 && voice->unknown_48 == 0x11111111);
    assert(voice->unknown_44 == 0x100 && voice->unknown_58 == 0x58);
}

static void stream_cases(void)
{
    static const int want[4][4] = {
        {1, 0x8010, 0x980, 77}, {1, 0x8010, 0xa80, 77}, {1, 0x8010, 0x981, 77}, {1, 0x8010, 0xa81, 77}};

    /* changeMasterVolume (loop coverage): both iterations, in order. */
    port_calls = 0;
    changeMasterVolume(77);
    assert(port_calls == 4 && memcmp(port_args, want, sizeof(want)) == 0);

    /* viBufEndPut (straight-line coverage): both stores happen between the two
     * calls; a negative n is sign-extended into the 64-bit total. */
    memset(&stream, 0, sizeof(stream));
    stream.unknown_14 = 100;
    stream.unknown_40 = 0x4040;
    stream.unknown_48 = 0x100000000L;
    lock_calls = unlock_calls = 0;
    viBufEndPut(&stream, -1);
    assert(lock_calls == 1 && lock_arg == 0x4040 && lock_word == 100);
    assert(unlock_calls == 1 && unlock_arg == 0x4040 && unlock_word == 99 && unlock_total == 0xffffffffL);
    assert(stream.unknown_14 == 99 && stream.unknown_48 == 0xffffffffL && stream.unknown_40 == 0x4040);
}

/* 0x2440f0, branch coverage: data that fits before the ring end, the boundary
 * where it ends exactly at the ring end, and wrapped data (second span). */
static void span_case(int n0c, int n10, int n14, int n08, const int expect[4])
{
    int out[4] = { -1, -1, -1, -1 };

    memset(&stream_u, 0, sizeof(stream_u));
    stream.unknown_00 = 0x100000;
    stream.unknown_08 = n08;
    stream.unknown_0c = n0c;
    stream.unknown_10 = n10;
    stream.unknown_14 = n14;
    stream.unknown_18 = 0x4000; /* eight 2048-byte blocks */
    stream.unknown_40 = 0x55;
    lock_calls = unlock_calls = 0;
    viBufBeginPut(&stream, &out[0], &out[1], &out[2], &out[3]);
    assert(lock_calls == 1 && unlock_calls == 1 && lock_arg == 0x55 && unlock_arg == 0x55);
    assert(memcmp(out, expect, sizeof(out)) == 0);
}

static void span_cases(void)
{
    static const int fits[4] = { 0x100000 + 0x1810, 0xff0, 0, 0 };
    static const int exact[4] = { 0x100000 + 0x3000, 0x1000, 0, 0 };
    static const int wraps[4] = { 0x100000 + 0x3800, 0x800, 0x100000, 0x800 };
    static const int modulo[4] = { 0x100000 + 0x800, 0x800, 0, 0 };

    span_case(1, 2, 0x10, 6, fits);   /* pos 0x1810, avail 2 * 2048 - 0x10 */
    span_case(4, 2, 0, 6, exact);     /* avail 0x1000 == size - pos */
    span_case(5, 2, 0, 6, wraps);     /* avail 0x1000 > size - pos 0x800 */
    span_case(7, 2, 0, 5, modulo);    /* (7 + 2) * 2048 % 0x4000 */
}

static int grant_results[2], grant_sizes[2], grant_calls, fill_calls;
int func_001828c0(int size)
{
    grant_sizes[grant_calls & 1] = size;
    return grant_results[grant_calls++ & 1];
}
void *func_00198948(void *destination, int value, int byte_count)
{
    assert(destination == _0_buf && value == 0 && byte_count == 0x800);
    fill_calls++;
    return destination;
}

/* One call per outcome of the two sign tests in audioDecCreate. */
static void stream_open_case(int first, int second, int expect)
{
    int p[24], i;

    for (i = 0; i < 24; i++) {
        p[i] = 0x7700 + i;
    }
    grant_results[0] = first;
    grant_results[1] = second;
    grant_calls = fill_calls = print_word_calls = port_calls = job_count = 0;
    trace_count = 0;
    assert(audioDecCreate(p, 0x21, 0x32, 0x4300) == expect);
    assert(p[0] == 0 && p[11] == 0 && p[12] == 0x21 && p[13] == 0 && p[14] == 0 && p[15] == 0x32);
    assert(p[16] == 0 && p[17] == first && p[18] == 0x4300 && p[19] == 0 && p[20] == 0 && p[21] == 0);
    assert(p[1] == 0x7701 && p[10] == 0x770a && p[23] == 0x7717 && grant_sizes[0] == 0x4300);
    if (first < 0) {
        assert(grant_calls == 1 && print_word_calls == 1 && p[22] == 0x7716);
        assert(trace[0].p == data_0030f600 && print_words[0][0] == first);
    } else {
        assert(grant_calls == 2 && grant_sizes[1] == 0x800 && p[22] == second && print_word_calls == 2);
        assert(trace[0].p == data_0030f61c && print_words[0][0] == first && print_words[0][1] == 0x4300);
        assert(print_words[1][0] == second);
        if (second < 0) {
            assert(trace[1].p == data_0030f600);
        } else {
            assert(trace[1].p == data_0030f61c && print_words[1][1] == 0x800);
        }
    }
    if (expect) {
        assert(fill_calls == 1 && job_block[0] == (int)(DotrSoundAddress)_0_buf);
        assert(job_block[1] == second && job_block[2] == 0x800 && port_calls == 4);
        assert(port_args[3][2] == 0xa81 && port_args[3][3] == 0x3fff);
    } else {
        assert(fill_calls == 0 && job_count == 0 && port_calls == 0);
    }
}

/* One 0x2437f0 call with destinations 1000/10 and 2000/20 and sources 3000 and 4000;
 * expect lists (dst, src, n) for each started copy. */
static void split_copy_case(int slen1, int slen2, int total, int copies, const int expect[3][3])
{
    int i;

    job_starts = job_waits = 0;
    assert(sendToIOP2area(1000, 10, 2000, 20, 3000, slen1, 4000, slen2) == total);
    assert(job_starts == copies);
    for (i = 0; i < copies; i++) {
        assert(job_blocks[i][1] == expect[i][0] && job_blocks[i][0] == expect[i][1] && job_blocks[i][2] == expect[i][2]);
    }
}

/* 0x2437f0, branch coverage. */
static void split_copy_cases(void)
{
    static const int wide[3][3] = { { 1000, 3000, 10 }, { 2000, 3010, 5 }, { 2005, 4000, 5 } };
    static const int even[3][3] = { { 1000, 3000, 10 }, { 2000, 4000, 5 } };
    static const int three[3][3] = { { 1000, 3000, 4 }, { 1004, 4000, 6 }, { 2000, 4006, 2 } };
    static const int exact[3][3] = { { 1000, 3000, 4 }, { 1004, 4000, 6 } };
    static const int two[3][3] = { { 1000, 3000, 4 }, { 1004, 4000, 3 } };
    static const int trimmed[3][3] = { { 1000, 3000, 10 }, { 2000, 3010, 15 }, { 2015, 4000, 5 } };
    static const int first_only[3][3] = { { 1000, 3000, 10 }, { 2000, 3010, 20 } };

    split_copy_case(15, 5, 20, 3, wide);        /* first source reaches past the first destination */
    split_copy_case(10, 5, 15, 2, even);        /* equal lengths: the middle copy is empty */
    split_copy_case(4, 8, 12, 3, three);        /* second source spans both destinations */
    split_copy_case(4, 6, 10, 2, exact);        /* second source ends with the first destination */
    split_copy_case(4, 3, 7, 2, two);           /* second source stays inside the first destination */
    split_copy_case(25, 5, 30, 3, trimmed);     /* totals equal: no clamp */
    split_copy_case(25, 10, 30, 3, trimmed);    /* excess 5 below the second length: second trimmed */
    split_copy_case(30, 7, 30, 2, first_only);  /* excess equals the second length */
    split_copy_case(38, 2, 30, 2, first_only);  /* excess above it: first trimmed, second dropped */
}

struct DotrSoundReadMode data_0029b2f8;
int isWithAudio;
struct DotrSoundStream data_00350620_sound;
static int rd_log[24], rd_count, rd_errors, rd_states[4], rd_state_at, rd_seeks[4], rd_seek_at;
static int rd_args[3], rd_position, rd_mode_ok, rd_direct[3];
int func_00192090(int sectors, int buffer, int unknown, int *error)
{
    rd_log[rd_count++] = 2;
    rd_args[0] = sectors; rd_args[1] = buffer; rd_args[2] = unknown;
    *error = rd_errors > 0 ? rd_errors-- : 0;
    return 3;
}
void func_00192058(void) { rd_log[rd_count++] = 3; }
int func_00191a58(int unknown) { assert(unknown == 0); rd_log[rd_count++] = 4; return rd_states[rd_state_at++]; }
int func_00191fc0(int position, struct DotrSoundReadMode *mode)
{
    rd_log[rd_count++] = 5;
    rd_position = position;
    rd_mode_ok = memcmp(mode, "\x01\x02\x03\x04", 4) == 0;
    return rd_seeks[rd_seek_at++];
}
void func_00243230_sound(struct DotrSoundStream *p) { assert(p == &data_00350620_sound); rd_log[rd_count++] = 6; }
int func_001800d8(int descriptor, int buffer, int byte_count)
{
    rd_direct[0] = descriptor; rd_direct[1] = buffer; rd_direct[2] = byte_count;
    return 0x777;
}

/* strFileRead (branch and loop coverage): the descriptor path; a first read
 * without error; one error with the pause word clear (state 1 then 2, seek 0
 * then state 2, seek 1); one error with the pause word set. */
static void sector_read_cases(void)
{
    static const int quiet[] = {2, 3, 4, 4, 5, 4, 5, 2};
    static const int paused[] = {2, 3, 4, 5, 6, 2};
    struct DotrSoundReader reader;
    int i;

    memset(&reader, 0, sizeof(reader));
    memcpy(&data_0029b2f8, "\x01\x02\x03\x04", 4);
    reader.descriptor = 9;
    job_count = rd_count = 0;
    assert(strFileRead(&reader, 0x1000, 0x1800, 44) == 0x777 && rd_count == 0 && job_count == 0);
    assert(rd_direct[0] == 9 && rd_direct[1] == 0x1000 && rd_direct[2] == 0x1800);

    reader.by_sector = 1;
    rd_errors = 0;
    assert(strFileRead(&reader, 0x1000, 0x1fff, 44) == 3 << 11 && job_count == 1 && job_flag == 0);
    assert(rd_count == 1 && rd_args[0] == 3 && rd_args[1] == 0x1000 && rd_args[2] == 1);

    job_count = rd_count = rd_state_at = rd_seek_at = 0;
    rd_errors = 1;
    rd_states[0] = 1; rd_states[1] = 2; rd_states[2] = 2;
    rd_seeks[0] = 0; rd_seeks[1] = 1;
    isWithAudio = 0;
    assert(strFileRead(&reader, 0x1000, 0x800, 44) == 3 << 11 && job_count == 2);
    assert(rd_count == 8 && rd_position == 44 && rd_mode_ok && rd_args[0] == 1);
    for (i = 0; i < 8; i++) {
        assert(rd_log[i] == quiet[i]);
    }

    job_count = rd_count = rd_state_at = rd_seek_at = 0;
    rd_errors = 1;
    rd_states[0] = 2;
    rd_seeks[0] = 1;
    isWithAudio = 1;
    memset(&data_00350620_sound, 0, sizeof(data_00350620_sound));
    pos_calls = 0;
    stream.unknown_50 = 0x400; /* the 0x190810 stub checks the 0x2431b0 fixture */
    assert(strFileRead(&reader, 0x1000, 0x800, 44) == 3 << 11 && job_count == 2);
    /* the real audioDecPause ran on the 0x350620 object before the wait */
    assert(pos_calls == 1 && data_00350620_sound.unknown_00 == 3);
    assert(rd_count == 6);
    for (i = 0; i < 6; i++) {
        assert(rd_log[i] == paused[i]);
    }
}

/* audioDecEndPut (branch coverage): state already set; count below, equal to and
 * above what the head still needs; the head at 39 -> 40 prints once with
 * every argument; the position wraps at the size. */
static void feed_cases(void)
{
    static const int want[16] = {-1, 2, -3, 4, 8, 9, 10, 11, 12, 13, 14, -5, 6, -7, 8, 0x28};
    struct DotrSoundFeed feed;
    int i;

    memset(&feed, 0, sizeof(feed));
    feed.state = 1;
    feed.have = 7;
    feed.position = 90;
    feed.size = 100;
    feed.total = 1000;
    feed.sum = 2000;
    feed_prints = 0;
    audioDecEndPut(&feed, 25);
    assert(feed.have == 7 && feed.position == 15 && feed.total == 1025 && feed.sum == 2025 && feed_prints == 0);

    feed.state = 0;
    feed.have = 30;
    audioDecEndPut(&feed, 9); /* all nine go to the head, which stays short */
    assert(feed.have == 39 && feed.state == 0 && feed.position == 15 && feed.total == 1025 && feed_prints == 0);

    feed.head[0] = -1; feed.head[1] = 2; feed.head[2] = -3; feed.head[3] = 4;
    for (i = 0; i < 7; i++) {
        feed.unknown_08[i] = 8 + i;
    }
    feed.tag[0] = -5; feed.tag[1] = 6; feed.tag[2] = -7; feed.tag[3] = 8;
    feed.unknown_28 = 0x28;
    audioDecEndPut(&feed, 1); /* exactly fills the head: nothing left over */
    assert(feed.have == 40 && feed.state == 1 && feed_prints == 3 && feed.position == 15 && feed.total == 1025);
    assert(memcmp(feed_args, want, sizeof(want)) == 0);

    feed.state = 0;
    feed.have = 36;
    feed_prints = 0;
    audioDecEndPut(&feed, 10); /* four to the head, six counted */
    assert(feed.have == 40 && feed.state == 1 && feed_prints == 3);
    assert(feed.position == 21 && feed.total == 1031 && feed.sum == 2031 && feed.unknown_30 == 0);
}

/* decBs0: one assertion per branch outcome (end at once, state 1, a wait for
 * a free item, a failed picture, tags rebuilt / not rebuilt, loop bound). */
struct DotrSoundBigQueue data_00350600_sound;
char data_0030f8b0[] = "<dec stop>", data_0030f8c8[] = "<dec error>";
static struct DotrSoundVideoDec dec;
static int dec_end_after, dec_end_calls, dec_reset_calls, dec_wait, dec_switches, dec_picture_result;
static int dec_pictures, dec_errors, dec_tag_calls;
static unsigned int *dec_tag[4];
static void *dec_image[4];
static int dec_tag_args[4][3];
int func_0018ee58(struct DotrSoundVideoDec *p) { assert(p == &dec); return dec_end_calls++ >= dec_end_after; }
int func_0018ee78(struct DotrSoundVideoDec *p) { assert(p == &dec); dec_reset_calls++; return 0; }
int func_0018ed40(struct DotrSoundVideoDec *p, DotrSoundAddress data, int size)
{
    assert(p == &dec && data == 0x5150 && size == 0x654);
    dec_pictures++;
    return dec_picture_result;
}
DotrSoundAddress voBufGetData(struct DotrSoundBigQueue *queue)
{
    assert(queue == &data_00350600_sound);
    if (dec_wait > 0) {
        dec_wait--;
        return 0;
    }
    return 0x5150;
}
void switchThread(void) { dec_switches++; }
void ErrMessage(DotrSoundAddress text) { assert((char *)text == data_0030f8c8); dec_errors++; }
void setImageTag(unsigned int *tags, void *image, int index, int w, int h)
{
    assert(dec_tag_calls < 4);
    dec_tag[dec_tag_calls] = tags;
    dec_image[dec_tag_calls] = image;
    dec_tag_args[dec_tag_calls][0] = index;
    dec_tag_args[dec_tag_calls][1] = w;
    dec_tag_args[dec_tag_calls][2] = h;
    dec_tag_calls++;
}
static void dec_reset(int end_after)
{
    static unsigned char images[2][0x195000];
    memset(&dec, 0, sizeof dec);
    data_00350600_sound.images = images;
    data_00350600_sound.items = big_items;
    data_00350600_sound.index = 0;
    data_00350600_sound.count = 0;
    data_00350600_sound.capacity = 2;
    dec_end_after = end_after;
    dec_end_calls = dec_reset_calls = dec_wait = dec_switches = dec_picture_result = 0;
    dec_pictures = dec_errors = dec_tag_calls = dec_stop_prints = 0;
}
static void dec_cases(void)
{
    assert(sizeof(struct DotrSoundBig) == 0x4ce40);
    assert((char *)big_items[0].tags[1] - (char *)&big_items[0] == 0x40 + 0x26700);

    /* The stream has already ended: nothing but the final call. */
    dec_reset(0);
    assert(decBs0(&dec) == 1);
    assert(dec_end_calls == 1 && dec_reset_calls == 1 && dec_pictures == 0 && dec_switches == 0);

    /* State 1: message, result -1, no picture. */
    dec_reset(5);
    dec.state = 1;
    assert(decBs0(&dec) == -1);
    assert(dec_stop_prints == 1 && dec_end_calls == 1 && dec_reset_calls == 1 && dec_pictures == 0);

    /* One picture after two waits; frame count zero rebuilds both items' tags. */
    dec_reset(1);
    dec.width = 640;
    dec.height = 448;
    dec_wait = 2;
    assert(decBs0(&dec) == 1);
    assert(dec_switches == 3 && dec_pictures == 1 && dec_errors == 0 && dec_tag_calls == 4);
    assert(dec_tag[0] == big_items[0].tags[0] && dec_tag[1] == big_items[0].tags[1]);
    assert(dec_tag[2] == big_items[1].tags[0] && dec_tag[3] == big_items[1].tags[1]);
    assert(dec_image[0] == data_00350600_sound.images[0] && dec_image[1] == data_00350600_sound.images[0]);
    assert(dec_image[2] == data_00350600_sound.images[1] && dec_image[3] == data_00350600_sound.images[1]);
    assert(dec_tag_args[0][0] == 0 && dec_tag_args[1][0] == 1 && dec_tag_args[2][0] == 0 && dec_tag_args[3][0] == 1);
    assert(dec_tag_args[3][1] == 640 && dec_tag_args[3][2] == 448);
    assert(data_00350600_sound.count == 1 && data_00350600_sound.index == 1 && dec_reset_calls == 1);

    /* Two pictures, both failing, frame count non-zero: errors, no tags. */
    dec_reset(2);
    dec.frame_count = 3;
    dec_picture_result = -1;
    assert(decBs0(&dec) == 1);
    assert(dec_pictures == 2 && dec_errors == 2 && dec_tag_calls == 0 && dec_switches == 2 && dec_end_calls == 3);
    assert(data_00350600_sound.count == 2);

    /* Capacity one bounds the tag loop at one item (voBufIncCount divides
     * by the capacity, so zero is not a host case). */
    dec_reset(1);
    data_00350600_sound.capacity = 1;
    assert(decBs0(&dec) == 1 && dec_tag_calls == 2);
}

/* 0x2435a0: stubs for the position query and the area split. */
static int area_calls, area_position, area_out[4], remote_args[3];
static struct DotrSoundDec *area_ring;

unsigned func_00190810_ret3(int a, int b, int c)
{
    remote_args[0] = a;
    remote_args[1] = b;
    remote_args[2] = c;
    return 0xab001234u;
}

void iopGetArea(int *a, int *b, int *c, int *d, struct DotrSoundDec *ring, int position)
{
    *a = area_out[0];
    *b = area_out[1];
    *c = area_out[2];
    *d = area_out[3];
    area_ring = ring;
    area_position = position;
    area_calls++;
}

static void send_reset(struct DotrSoundDec *ad, int state, int put, int count)
{
    memset(ad, 0, sizeof(*ad));
    ad->state = state;
    ad->data = 0x5000;
    ad->put = put;
    ad->count = count;
    ad->size = 0x1000;
    ad->iopBuff = 0x1000;
    ad->iopBuffSize = 0x2000;
    ad->iopLastPos = 0x1900;
    ad->totalBytesSent = 0x100;
    area_calls = 0;
    job_starts = job_waits = 0;
}

static void send_cases(void)
{
    struct DotrSoundDec ad;

    assert(sizeof(ad) == 0x58);
    /* States 0 and 3: nothing sent, nothing changed. */
    send_reset(&ad, 0, 0x900, 0x805);
    assert(audioDecSendToIOP(&ad) == 0 && ad.count == 0x805 && ad.totalBytesSent == 0x100 && ad.iopLastPos == 0x1900);
    send_reset(&ad, 3, 0x900, 0x805);
    assert(audioDecSendToIOP(&ad) == 0 && ad.count == 0x805 && ad.totalBytesSent == 0x100 && job_starts == 0);

    /* State 1: one destination from the bytes sent so far; whole kilobytes of the ring in one piece. */
    send_reset(&ad, 1, 0x900, 0x805);
    assert(audioDecSendToIOP(&ad) == 0x800 && area_calls == 0 && job_starts == 1);
    assert(job_blocks[0][0] == 0x50fb && job_blocks[0][1] == 0x1100 && job_blocks[0][2] == 0x800);
    assert(ad.count == 5 && ad.totalBytesSent == 0x900 && ad.iopLastPos == 0x100);

    /* State 1 with less than a kilobyte in the ring: no copy, modulo still stored. */
    send_reset(&ad, 1, 0x900, 0x3ff);
    ad.iopLastPos = 0x2100;
    assert(audioDecSendToIOP(&ad) == 0 && job_starts == 0);
    assert(ad.count == 0x3ff && ad.totalBytesSent == 0x100 && ad.iopLastPos == 0x100);
    /* Exactly a kilobyte is sent. */
    send_reset(&ad, 1, 0x900, 0x400);
    assert(audioDecSendToIOP(&ad) == 0x400 && job_starts == 1 && job_blocks[0][0] == 0x5500);

    /* State 2: position query masked to 24 bits less the IOP base; areas from iopGetArea.
     * Destination total 1023: no copy. */
    send_reset(&ad, 2, 0x100, 0x800);
    area_out[0] = 0x1200; area_out[1] = 0x3ff; area_out[2] = 0x1000; area_out[3] = 0;
    assert(audioDecSendToIOP(&ad) == 0 && area_calls == 1 && area_ring == &ad && area_position == 0x1234 - 0x1000);
    assert(remote_args[0] == 1 && remote_args[1] == 0x8100 && remote_args[2] == 0 && job_starts == 0);
    /* Destination total 1024 and a ring that wraps: sources 0x700 at the tail, 0x100 at the start. */
    send_reset(&ad, 2, 0x100, 0x800);
    area_out[0] = 0x1200; area_out[1] = 0x400; area_out[2] = 0x1000; area_out[3] = 0;
    assert(audioDecSendToIOP(&ad) == 0x400 && job_starts == 1);
    assert(job_blocks[0][0] == 0x5900 && job_blocks[0][1] == 0x1200 && job_blocks[0][2] == 0x400);
    assert(ad.count == 0x400 && ad.totalBytesSent == 0x500 && ad.iopLastPos == 0x1d00);
    send_reset(&ad, 2, 0x100, 0x800);
    area_out[0] = 0x1200; area_out[1] = 0x2000; area_out[2] = 0x1000; area_out[3] = 0x10;
    assert(audioDecSendToIOP(&ad) == 0x800 && job_starts == 2);
    assert(job_blocks[0][0] == 0x5900 && job_blocks[0][1] == 0x1200 && job_blocks[0][2] == 0x700);
    assert(job_blocks[1][0] == 0x5000 && job_blocks[1][2] == 0x100);
}

/* viBufRestartDMA: data area 0x1000, tags 0x30009000 (high bits dropped by the 28-bit mask),
 * four elements. Each case gives the saved channel-4 address, the two IPU FIFO counts, the
 * saved control word and the DMA start index and count, then the expected channel-4 address,
 * count, tag address and control word and the start index and count afterwards. The saved
 * count is 7 and the saved tag address 0x9010 throughout. Expected values are derived
 * from the routine's logic, not recorded from a run. */
static void restart_case(unsigned madr, int fp, int ifc, unsigned chcr, int start, int count,
                         unsigned want_madr, unsigned want_qwc, unsigned want_tadr, unsigned want_chcr,
                         int want_start, int want_count)
{
    struct DotrSoundStream s;
    struct DotrSoundViBuf *f = (struct DotrSoundViBuf *)&s;
    int enter = big_enter;

    memset(&s, 0, sizeof(s));
    memset((void *)dotr_sound_host_regs, 0, sizeof(dotr_sound_host_regs));
    f->data = 0x1000; f->tag = 0x30009000; f->n = 4;
    f->dmaStart = start; f->dmaN = count;
    f->d4madr = madr; f->d4tadr = 0x9010; f->d4qwc = 7; f->d4chcr = chcr;
    f->ipubp = 0x25 | (ifc << 8) | (fp << 16) | 0x40000;
    f->ipuctrl = 0x1234; f->sema = 0x77;
    SOUND_REG(0x1000f520) = 0x10201;
    SOUND_REG(0x1000b400) = -1;
    SOUND_REG(0x10002000) = -1;
    lock_calls = unlock_calls = 0;
    assert(viBufRestartDMA(&s) == 1);
    assert(lock_calls == 1 && unlock_calls == 1 && lock_arg == 0x77 && unlock_arg == 0x77);
    assert((unsigned)SOUND_REG(0x1000b410) == want_madr && (unsigned)SOUND_REG(0x1000b420) == want_qwc);
    assert((unsigned)SOUND_REG(0x1000b430) == want_tadr);
    assert(f->dmaStart == want_start && f->dmaN == want_count && f->isActive == 1);
    assert(SOUND_REG(0x10002010) == 0x1234);
    /* Channel 3 is left alone here (saved address and count are 0). */
    assert(SOUND_REG(0x1000b000) == 0 && SOUND_REG(0x1000b010) == 0 && SOUND_REG(0x1000b020) == 0);
    if (want_count != 0) {
        /* Pending elements: IPU cleared to bit position 0x25, channel 4 restarted. */
        assert(SOUND_REG(0x10002000) == 0x25 && (unsigned)SOUND_REG(0x1000b400) == want_chcr);
        assert(big_enter == enter + 1 && SOUND_REG(0x1000f590) == 0x201);
    } else {
        assert(SOUND_REG(0x10002000) == -1 && SOUND_REG(0x1000b400) == -1 && big_enter == enter);
    }
}

static void restart_channel3(unsigned madr, unsigned qwc, int written)
{
    struct DotrSoundStream s;
    struct DotrSoundViBuf *f = (struct DotrSoundViBuf *)&s;
    int enter = big_enter;

    memset(&s, 0, sizeof(s));
    memset((void *)dotr_sound_host_regs, 0, sizeof(dotr_sound_host_regs));
    f->data = 0x1000; f->tag = 0x9000; f->n = 4;
    f->d4madr = 0x1800; f->d4chcr = 4;
    f->d3madr = madr; f->d3qwc = qwc; f->d3chcr = 0x41;
    SOUND_REG(0x1000f520) = 0x10201;
    lock_calls = unlock_calls = 0;
    assert(viBufRestartDMA(&s) == 1);
    if (written) {
        assert((unsigned)SOUND_REG(0x1000b010) == madr && (unsigned)SOUND_REG(0x1000b020) == qwc);
        assert(SOUND_REG(0x1000b000) == 0x141 && big_enter == enter + 1 && SOUND_REG(0x1000f590) == 0x201);
    } else {
        assert(SOUND_REG(0x1000b010) == 0 && SOUND_REG(0x1000b020) == 0 && SOUND_REG(0x1000b000) == 0);
        assert(big_enter == enter);
    }
    /* Nothing pending and no index change: channel 4 keeps its values and is not restarted. */
    assert(SOUND_REG(0x1000b410) == 0x1800 && SOUND_REG(0x1000b420) == 0 && SOUND_REG(0x1000b430) == 0);
    assert(SOUND_REG(0x1000b400) == 0 && f->dmaN == 0 && f->isActive == 1);
}

static void restart_cases(void)
{
    leave_any_ctrl = 1;
    /* same element before and after: only the count grows, nothing pending */
    restart_case(0x1820, 1, 1, 0x30000004, 1, 0, 0x1800, 0x9, 0x9010, 0x30000104, 1, 0);
    /* same element, elements pending */
    restart_case(0x1820, 1, 1, 0x30000004, 1, 2, 0x1800, 0x9, 0x9010, 0x30000104, 1, 2);
    /* wrap from the data start, element 0 outside the region */
    restart_case(0x1000, 0, 1, 0x30000004, 1, 2, 0x2ff0, 0x1, 0x9000, 0x104, 3, 3);
    /* wrap from the data start, element 0 inside the region */
    restart_case(0x1000, 2, 0, 0x70000004, 3, 2, 0x2fe0, 0x2, 0x9000, 0x104, 3, 2);
    /* wrap from inside the first element (tag id 3), negative distance */
    restart_case(0x1010, 3, 15, 0x10000004, 9, 2, 0x2ef0, 0x11, 0x9000, 0x30000104, 3, 3);
    /* element change on an element boundary that is the write position, next outside */
    restart_case(0x2000, 0, 1, 0x30000004, 3, 3, 0x1ff0, 0x1, 0x9020, 0x104, 3, 3);
    /* element change, not the write position, next inside */
    restart_case(0x2010, 1, 2, 0x10000004, 1, 2, 0x1fe0, 0x2, 0x9020, 0x30000104, 1, 2);
    /* the end tag's address counts as element 0 */
    restart_case(0x9050, 0, 1, 0x10000004, 0, 0, 0x9040, 0xffff7fc, 0x9000, 0x30000104, 16, 1);
    /* element change with a negative distance */
    restart_case(0x2000, 0, 1, 0x10000004, 9, 0, 0x1ff0, 0x1, 0x9020, 0x30000104, 1, 1);
    restart_channel3(0, 5, 0);
    restart_channel3(0x4000, 0, 0);
    restart_channel3(0x4000, 5, 1);
    leave_any_ctrl = 0;
}

int main(void)
{
    feed_cases();
    sector_read_cases();
    split_copy_cases();
    stream_open_case(-1, 5, 0);
    stream_open_case(0, -2, 0);
    stream_open_case(0x40, 0, 1);
    ring_cases();
    span_cases();
    start_cases();
    reader_cases();
    callback_cases();
    split_cases(copy2area);
    split_cases(cpy2area);
    stream_cases();
    stream_init_case();
    stream_more_cases();
    voice_case();
    dec_cases();
    send_cases();
    restart_cases();
    puts("sound_file: ok");
    return 0;
}
