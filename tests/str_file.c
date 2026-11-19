/* Synthetic host tests for the str_file group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics (host C library equivalents and recorders). The
 * string constants below are host fixtures, not the game's text. */
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/str_file.h"

char data_0030f7f0[] = "cdrom0";
char data_0030f7f7[] = "";
char data_0030f7f8[] = ";1";
char data_0030f7fb[] = "%s%s";
char data_0030f800[] = "%s:%s";
char data_0030f806[] = "host0";
char data_0030f80c[] = "name %s\n";
char data_0030f816[] = "search %s\n";
char data_0030f839[] = "open %s\n";
char data_0030f854[] = "end %s %d\n";
char data_0030f86f[] = "start %s\n";

static const char *messages[4];
static char message_paths[4][64];
static int message_value;
static int message_count;
static int ready_calls;
static int alloc_size;
static int setup_args[3];
static int setup_calls;
static void *search_file;
static char search_name[64];
static int search_result;
static int seek_position;
static unsigned char seek_mode[3];
static int seek_calls;
static char open_path[64];
static int open_flags;
static int open_result;
static int lseek_args[2][3];
static int lseek_results[2];
static int lseek_calls;
static int close_descriptor;
static int close_calls;

char *func_001976d8(const char *string, int c) { return strchr(string, c); }
void *func_0019c2b8(void *destination, const void *source, int byte_count)
{
    return memcpy(destination, source, (size_t)byte_count);
}
int func_0019bc90(const char *a, const char *b) { return strcmp(a, b); }
int func_0019bef0(const char *string) { return (int)strlen(string); }
int func_0019d9e0(int c) { return toupper(c); }
int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;
    int count;
    va_start(args, format);
    count = vsprintf(destination, format, args);
    va_end(args);
    return count;
}
char *func_0019bdd8(char *destination, const char *source) { return strcpy(destination, source); }
int func_00199b50(const char *format, ...)
{
    va_list args;
    assert(message_count < 4);
    va_start(args, format);
    messages[message_count] = format;
    strcpy(message_paths[message_count], va_arg(args, char *));
    if (format == data_0030f854) {
        message_value = va_arg(args, int);
    }
    va_end(args);
    message_count++;
    return 0;
}
int func_00191a58(int mode)
{
    assert(mode == 0);
    return ready_calls++ == 0 ? 6 : 2;
}
int func_001828c0(int size)
{
    alloc_size = size;
    return 0x1001;
}
int func_00191f90(int a, int b, int c)
{
    setup_args[0] = a; setup_args[1] = b; setup_args[2] = c;
    setup_calls++;
    return 0;
}
int func_00190ff0(void *file, const char *name)
{
    search_file = file;
    strcpy(search_name, name);
    if (search_result != 0) {
        ((int *)file)[0] = 77;
        ((int *)file)[1] = 1234;
    }
    return search_result;
}
int func_00191fc0(int position, void *mode)
{
    seek_position = position;
    memcpy(seek_mode, mode, 3);
    seek_calls++;
    return 1;
}
int func_0017fa98(const char *path, int flags)
{
    strcpy(open_path, path);
    open_flags = flags;
    return open_result;
}
int func_0017fea0(int descriptor, int offset, int origin)
{
    assert(lseek_calls < 2);
    lseek_args[lseek_calls][0] = descriptor;
    lseek_args[lseek_calls][1] = offset;
    lseek_args[lseek_calls][2] = origin;
    return lseek_results[lseek_calls++];
}
int func_0017fd20(int descriptor)
{
    close_descriptor = descriptor;
    close_calls++;
    return 0;
}

static struct DotrStrFile file;

static void reset(void)
{
    memset(&file, 0x55, sizeof(file));
    message_count = 0;
    ready_calls = 0;
    alloc_size = 0;
    setup_calls = 0;
    search_file = NULL;
    search_result = 1;
    seek_calls = 0;
    memset(seek_mode, 0xff, sizeof(seek_mode));
    open_path[0] = 0;
    open_result = 5;
    lseek_results[0] = 100;
    lseek_results[1] = 0;
    lseek_calls = 0;
    close_calls = 0;
}

static void check_layout(void)
{
    assert(offsetof(struct DotrStrFile, size) == 4);
    assert(offsetof(struct DotrStrFile, cd_position) == 8);
    assert(offsetof(struct DotrStrFile, cd_size) == 12);
    assert(offsetof(struct DotrStrFile, cd_buffer) == 0x2c);
    assert(offsetof(struct DotrStrFile, descriptor) == 0x30);
}

/* Device prefix equal to the constant at 0x30f7f0: the path after the colon is rewritten in
 * place ('/' to '\\', upper case) and searched on the CD. */
static void check_cd(void)
{
    char name[] = "cdrom0:/dir/file.bin";
    char versioned[] = "cdrom0:a;1";
    char empty[] = "cdrom0:";

    reset();
    assert(strFileOpen(&file, name) == 1);
    assert(strcmp(name, "cdrom0:\\DIR\\FILE.BIN") == 0);
    assert(file.by_sector == 1);
    assert(message_count == 1 && messages[0] == data_0030f80c);
    assert(strcmp(message_paths[0], "\\DIR\\FILE.BIN;1") == 0);
    assert(ready_calls == 2);
    assert(alloc_size == 0x28010 && file.cd_buffer == 0x1001);
    assert(setup_calls == 1 && setup_args[0] == 80 && setup_args[1] == 5);
    assert(setup_args[2] == 0x1010);
    assert(search_file == &file.cd_position);
    assert(strcmp(search_name, "\\DIR\\FILE.BIN;1") == 0);
    assert(file.size == 1234 && file.cd_position == 77);
    assert(seek_calls == 1 && seek_position == 77);
    assert(seek_mode[0] == 0 && seek_mode[1] == 0 && seek_mode[2] == 0);
    assert(file.descriptor == 0x55555555 && lseek_calls == 0 && open_path[0] == 0);

    /* A ';' in the name selects the other suffix; a failed search reports and returns 0. */
    reset();
    search_result = 0;
    assert(strFileOpen(&file, versioned) == 0);
    assert(strcmp(versioned, "cdrom0:A;1") == 0);
    assert(strcmp(search_name, "A;1") == 0);
    assert(message_count == 2 && messages[1] == data_0030f816);
    assert(strcmp(message_paths[1], "A;1") == 0);
    assert(file.by_sector == 1 && file.size == 0x55555555 && seek_calls == 0);

    /* Nothing after the colon: the rewrite loop does not run. */
    reset();
    assert(strFileOpen(&file, empty) == 1);
    assert(strcmp(empty, "cdrom0:") == 0);
    assert(strcmp(search_name, ";1") == 0);
}

/* Any other device, or no device at all (the default from 0x30f806): descriptor path. */
static void check_descriptor(void)
{
    char other[] = "host1:/x/y";
    char plain[] = "plain.bin";

    reset();
    assert(strFileOpen(&file, other) == 1);
    assert(strcmp(other, "host1:/x/y") == 0);
    assert(file.by_sector == 0);
    assert(message_count == 1 && strcmp(message_paths[0], "host1:/x/y") == 0);
    assert(strcmp(open_path, "host1:/x/y") == 0 && open_flags == 1);
    assert(file.descriptor == 5 && file.size == 100);
    assert(lseek_calls == 2);
    assert(lseek_args[0][0] == 5 && lseek_args[0][1] == 0 && lseek_args[0][2] == 2);
    assert(lseek_args[1][0] == 5 && lseek_args[1][1] == 0 && lseek_args[1][2] == 0);
    assert(close_calls == 0 && ready_calls == 0 && file.cd_buffer == 0x55555555);

    /* Zero results are still successes. */
    reset();
    open_result = 0;
    lseek_results[0] = 0;
    assert(strFileOpen(&file, plain) == 1);
    assert(strcmp(open_path, "host0:plain.bin") == 0);
    assert(file.by_sector == 0 && file.descriptor == 0 && file.size == 0);

    reset();
    open_result = -1;
    assert(strFileOpen(&file, plain) == 0);
    assert(message_count == 2 && messages[1] == data_0030f839);
    assert(file.descriptor == -1 && lseek_calls == 0 && close_calls == 0);

    reset();
    lseek_results[0] = -1;
    assert(strFileOpen(&file, plain) == 0);
    assert(message_count == 2 && messages[1] == data_0030f854 && message_value == -1);
    assert(lseek_calls == 1 && close_calls == 1 && close_descriptor == 5);

    reset();
    lseek_results[1] = -1;
    assert(strFileOpen(&file, plain) == 0);
    assert(message_count == 2 && messages[1] == data_0030f86f);
    assert(strcmp(message_paths[1], "host0:plain.bin") == 0);
    assert(lseek_calls == 2 && close_calls == 1 && close_descriptor == 5 && file.size == 100);
}

int main(void)
{
    check_layout();
    check_cd();
    check_descriptor();
    puts("str_file: ok");
    return 0;
}
