/* Synthetic host tests for the debug_capture group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/debug_capture.h"

unsigned short dotr_debug_host_frame[640 * 448];
int data_00325cb0;
char data_002d07fa[] = "<env label>", data_002d0806[] = "<name format>";
static char env_text[] = "<env>";
static int pending, do_calls, copy_calls, exists_until, create_fd, probe_count, last_count;
static int close_log[8], close_count, write_count, row_count;
static unsigned char file_header[14], info_header[40], first_row[1920], last_row[1920];
static unsigned char *path_buffer;

int SzFile_InqRequest(int a) { assert(a == -1); return pending; }
int SzFile_DoRequestS(void) { pending--; do_calls++; return 0; }
void RhBase_CopyFrameBuffer(unsigned long x) { assert(x == 0x4000000 && pending == 0); copy_calls++; }
char *SzFile_GetEnv(const char *label) { assert(label == data_002d07fa); return env_text; }
int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;

    assert(format == data_002d0806);
    va_start(args, format);
    assert(va_arg(args, char *) == env_text);
    last_count = va_arg(args, int);
    va_end(args);
    path_buffer = (unsigned char *)destination;
    return 0;
}
int func_0017fa98(const char *path, int flags)
{
    assert((unsigned char *)path == path_buffer);
    if (flags == 1) {
        probe_count++;
        return last_count < exists_until ? 7 : -1;
    }
    assert(flags == 0x202 && last_count == exists_until && data_00325cb0 == exists_until + 1);
    return create_fd;
}
int func_0017fd20(int descriptor) { close_log[close_count++] = descriptor; return 0; }
int func_00180348(int descriptor, void *data, int size)
{
    assert(descriptor == create_fd);
    if (write_count == 0) {
        assert(size == 14);
        memcpy(file_header, data, 14);
    } else if (write_count == 1) {
        assert(size == 40);
        memcpy(info_header, data, 40);
    } else {
        assert(size == 1920 && data == path_buffer);
        if (row_count == 0) {
            memcpy(first_row, data, 1920);
        }
        memcpy(last_row, data, 1920);
        row_count++;
    }
    write_count++;
    return size;
}
static void reset(int requests, int count, int until, int fd)
{
    pending = requests;
    data_00325cb0 = count;
    exists_until = until;
    create_fd = fd;
    do_calls = copy_calls = probe_count = close_count = write_count = row_count = 0;
}
/* SzDebug_OutputFrameBuffer (branch coverage: pending requests or none, existing names skipped or not,
 * create failing or succeeding, both loop bounds through the first and last row). */
static void check_capture(void)
{
    static const unsigned char expect_file[14] = {'B', 'M', 0x36, 0x20, 0x0d, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    static const int expect_info[10] = {40, 640, 448, 0x180001, 0, 0xd2000, 1, 1, 0, 0};

    assert(sizeof(struct DotrDebugFileHeader) == 14 && sizeof(struct DotrDebugInfoHeader) == 40);
    dotr_debug_host_frame[447 * 640] = 0x7c00;       /* first row written, first pixel */
    dotr_debug_host_frame[447 * 640 + 639] = 0x03e0; /* first row written, last pixel */
    dotr_debug_host_frame[0] = 0x001f;               /* last row written */
    dotr_debug_host_frame[1] = 0x7fff;
    dotr_debug_host_frame[639] = 0x5a96;

    /* Two pending requests, names 3 and 4 exist, 5 is created. */
    reset(2, 3, 5, 9);
    SzDebug_OutputFrameBuffer();
    assert(do_calls == 2 && copy_calls == 1 && probe_count == 3 && data_00325cb0 == 6);
    assert(close_count == 3 && close_log[0] == 7 && close_log[1] == 7 && close_log[2] == 9);
    assert(write_count == 450 && row_count == 448);
    assert(memcmp(file_header, expect_file, 14) == 0 && memcmp(info_header, expect_info, 40) == 0);
    assert(first_row[0] == 0xff && first_row[1] == 0 && first_row[2] == 0);
    assert(first_row[1917] == 0 && first_row[1918] == 0xff && first_row[1919] == 0);
    assert(last_row[0] == 0 && last_row[1] == 0 && last_row[2] == 0xff);
    assert(last_row[3] == 0xff && last_row[4] == 0xff && last_row[5] == 0xff);
    /* 0x5a96: blue 0x16, green 0x14, red 0x16, each as (v << 3) | (v >> 2). */
    assert(last_row[1917] == 0xb5 && last_row[1918] == 0xa5 && last_row[1919] == 0xb5);

    /* Nothing pending, the first name is free, create fails: counter moves, no writes. */
    reset(0, 8, 8, -1);
    SzDebug_OutputFrameBuffer();
    assert(do_calls == 0 && copy_calls == 1 && probe_count == 1 && data_00325cb0 == 9);
    assert(close_count == 0 && write_count == 0);
}

int main(void)
{
    check_capture();
    return 0;
}
