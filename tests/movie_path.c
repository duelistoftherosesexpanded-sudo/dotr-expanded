/* Synthetic host tests for movie_path.c. Every callee is a host stub; no game data is included. */
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void SzGame_PlayMovieNo(int, int, int);

char data_002be80f[] = "synthetic-directory-label";
char data_002be809[] = "%s|%s";
const char *data_0028ff10[] = {"first", "second", "third", "fourth"};
static char directory[] = "synthetic-root";
static int calls;
static const char *expected_file;
static int expected_arg1, expected_arg2;

char *SzFile_GetEnv(const char *label)
{
    assert(calls == 0);
    assert(label == data_002be80f);
    calls++;
    return directory;
}

int func_0019b648(char *path, const char *format, ...)
{
    va_list args;
    const char *root, *file;
    int result;
    assert(calls == 1);
    assert(format == data_002be809);
    va_start(args, format);
    root = va_arg(args, char *);
    file = va_arg(args, const char *);
    va_end(args);
    assert(root == directory);
    assert(file == expected_file);
    result = snprintf(path, 128, format, root, file);
    assert(result >= 0 && result < 128);
    calls++;
    return result;
}

int SzGame_PlayMovie(const char *path, int arg1, int arg2)
{
    char expected[128];
    assert(calls == 2);
    assert(arg1 == expected_arg1 && arg2 == expected_arg2);
    assert(snprintf(expected, sizeof(expected), "%s|%s", directory, expected_file) > 0);
    assert(strcmp(path, expected) == 0);
    calls++;
    return 37;
}

int main(void)
{
    int index;
    for (index = 0; index < 4; index++) {
        expected_file = data_0028ff10[index];
        expected_arg1 = (index & 1) ? INT_MIN : INT_MAX;
        expected_arg2 = index - 2;
        calls = 0;
        SzGame_PlayMovieNo(index, expected_arg1, expected_arg2);
        assert(calls == 3);
    }
    return 0;
}
