/* Synthetic host checks for the text helpers, the result callback, the name
 * setter and the module-path loader of the resource chain.
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined(DOTR_TEST_MODULE_PATH)
#include "dotr/ee/resource_boot.h"

/* 0x200220/0x200230 with the real uppercase helper linked in. The string
 * and loader dependencies are stubs whose semantics are assumptions.
 */
struct DotrResourceBootState dotr_ee_resource_boot_state_0031c2f0;
char data_002d0670[] = "<root>";
char data_002d067d[] = "<suffix>";

enum { COPY = 1, APPEND, LOAD };
static int trace[16], trace_count, load_failures, load_size;
static const char *load_arguments;
static char loaded_path[128];

static char *path_buffer(void)
{
    return (char *)dotr_ee_resource_boot_state_0031c2f0.unknown_000;
}

char *func_0019bdd8(char *destination, const char *source)
{
    assert(destination == path_buffer() && source == data_002d0670);
    trace[trace_count++] = COPY;
    return strcpy(destination, source);
}

char *func_0019b9d0(char *destination, const char *source)
{
    assert(destination == path_buffer());
    trace[trace_count++] = APPEND;
    return strcat(destination, source);
}

/* Ends the original unbounded retry after a controlled number of failures. */
int func_00183538(const char *path, int argument_size, const char *arguments)
{
    assert(path == path_buffer());
    trace[trace_count++] = LOAD;
    strcpy(loaded_path, path);
    load_size = argument_size;
    load_arguments = arguments;
    return load_failures-- > 0 ? -1 : 7;
}

int main(void)
{
    static const int once[] = {COPY, APPEND, APPEND, LOAD};
    static const int retried[] = {COPY, APPEND, APPEND, LOAD, LOAD, LOAD};
    char name[] = "mod_9.irx";
    char other[] = "Second.Irx";
    char arguments[] = "args";
    int flag;

    /* The thunk passes no arguments; the name is uppercased in place and the
     * path is root + name + suffix. Any nonnegative load result ends the loop.
     */
    load_failures = 0;
    assert(RhFile_LoadIopModuleWrapper(name, 0) == 1);
    assert(trace_count == 4 && memcmp(trace, once, sizeof(once)) == 0);
    assert(strcmp(name, "MOD_9.IRX") == 0);
    assert(strcmp(loaded_path, "<root>MOD_9.IRX<suffix>") == 0);
    assert(load_size == 0 && load_arguments == 0);

    /* Negative results retry with the same path and arguments; both flag
     * values build the same path.
     */
    for (flag = 0; flag < 2; flag++) {
        strcpy(other, "Second.Irx");
        trace_count = 0;
        load_failures = 2;
        assert(RhFile_LoadIopModuleWrapperEx(other, 5, arguments, flag) == 1);
        assert(trace_count == 6 && memcmp(trace, retried, sizeof(retried)) == 0);
        assert(strcmp(loaded_path, "<root>SECOND.IRX<suffix>") == 0);
        assert(load_size == 5 && load_arguments == arguments);
    }
    puts("resource_module_path: ok");
    return 0;
}
#else
#include "dotr/ee/resource_process.h"

struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;
char data_002d0655[] = "<fallback>";
char data_002d0680[] = "<label>";
char data_002d0686[] = "<format>";

static char *lookup_result;
static const char *format_first, *format_second;
static char *format_destination;
static int format_calls, attach_calls;

char *func_00197548(const char *label)
{
    assert(label == data_002d0680);
    return lookup_result;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list arguments;
    assert(format == data_002d0686);
    va_start(arguments, format);
    format_first = va_arg(arguments, const char *);
    format_second = va_arg(arguments, const char *);
    va_end(arguments);
    format_destination = destination;
    strcpy(destination, "<path>");
    format_calls++;
    return 0;
}

int SzMem_StrDup(const char *name)
{
    /* The formatted text is complete before it is handed over. */
    assert(format_calls == attach_calls + 1);
    assert(name == format_destination && strcmp(name, "<path>") == 0);
    attach_calls++;
    return 0x4321;
}

static void check_text(void (*function)(char *), const char *input, const char *expected)
{
    char buffer[40];
    memset(buffer, '#', sizeof(buffer));
    strcpy(buffer + 1, input);
    function(buffer + 1);
    assert(strcmp(buffer + 1, expected) == 0);
    /* Nothing before the text or after its terminator is written. */
    assert(buffer[0] == '#' && buffer[strlen(input) + 2] == '#');
}

int main(void)
{
    static char found[] = "<found>";
    static char name[] = "<name>";

    /* 0x200300: only 'a'..'z' change; the neighbours '`' and '{' do not. */
    check_text(lRhFile_StrUpper, "", "");
    check_text(lRhFile_StrUpper, "az`{AZ09_./\\", "AZ`{AZ09_./\\");
    check_text(lRhFile_StrUpper, "\x80\xff" "m", "\x80\xff" "M");

    /* 0x1ff250: uppercase, then make separators uniform. A leading "//" or
     * "\\\\" selects backslashes; anything else selects forward slashes.
     */
    check_text(lSzFile_StrNormalize, "", "");
    check_text(lSzFile_StrNormalize, "dir\\sub/file.bin", "DIR/SUB/FILE.BIN");
    check_text(lSzFile_StrNormalize, "/dir\\file", "/DIR/FILE");
    check_text(lSzFile_StrNormalize, "\\dir/file", "/DIR/FILE");
    check_text(lSzFile_StrNormalize, "//host/a\\b", "\\\\HOST\\A\\B");
    check_text(lSzFile_StrNormalize, "\\\\host/a\\b", "\\\\HOST\\A\\B");
    check_text(lSzFile_StrNormalize, "/\\x/y", "//X/Y");
    check_text(lSzFile_StrNormalize, "\\/x\\y", "//X/Y");
    check_text(lSzFile_StrNormalize, "/", "/");

    /* 0x1fff60: only the low bit of the flag's low byte reaches result bit 0;
     * the record is unused and bit 1 is preserved.
     */
    state->flags.bit_1 = -1;
    lSzFile_ReadCB(0, 1);
    assert(state->flags.bit_0 == -1 && state->flags.bit_1 == -1);
    lSzFile_ReadCB(0, 0);
    assert(state->flags.bit_0 == 0 && state->flags.bit_1 == -1);
    lSzFile_ReadCB(0, 2);
    assert(state->flags.bit_0 == 0);
    lSzFile_ReadCB(0, 0x103);
    assert(state->flags.bit_0 == -1);
    state->flags.bit_1 = 0;
    lSzFile_ReadCB(0, 0x100);
    assert(state->flags.bit_0 == 0 && state->flags.bit_1 == 0);

    /* 0x200360: format directory and name, store the returned word, return 0. */
    lookup_result = found;
    state->cached_result = 99;
    assert(RhFile_OpenFile(name) == 0);
    assert(format_first == found && format_second == name);
    assert(state->unknown_6684 == 0x4321 && state->cached_result == 99 && attach_calls == 1);
    lookup_result = 0;
    state->unknown_6684 = 0;
    assert(RhFile_OpenFile(name) == 0);
    assert(format_first == data_002d0655 && state->unknown_6684 == 0x4321 && attach_calls == 2);

    puts("resource_paths: ok");
    return 0;
}
#endif
