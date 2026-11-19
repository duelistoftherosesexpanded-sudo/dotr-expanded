/* Synthetic host tests for the movie_start group: synthetic arguments, stubbed callees with
 * assumed results, no game data. */
#include <assert.h>
#include <limits.h>
#include "dotr/ee/movie_start.h"

enum Event { ACTIVE, WAIT, WAIT_STEP, PREPARE, ENABLE, ENTER, QUERY, RESET,
    BEFORE_PLAY, PLAY_ENABLE, PLAY, AFTER_PLAY, PLAY_DISABLE, PACKET,
    DISABLE, LEAVE };
static enum Event expected[80];
static int expected_count, cursor, active, waits, query_index, reset_index;
static int arg1, arg2, result;
static const char *path;

static void event(enum Event value)
{
    assert(cursor < expected_count);
    assert(expected[cursor++] == value);
}

int SzSound_IsStandby(void) { event(ACTIVE); return active; }
int SzFile_InqRequestEx(void) { event(WAIT); return waits-- > 0; }
int SzFile_DoRequestS(void) { event(WAIT_STEP); return -37; }
void SD_StopAll(void) { event(PREPARE); }
void SD_SuspendSoundSystem(int value) { event(value ? ENABLE : DISABLE); }
void SzSound_Term(void) { event(ENTER); }
int SzModel_GetModelNo(int value)
{
    event(QUERY);
    assert(value == query_index++ + 20);
    return -value * 3;
}
void SzModel_Preset(int index, int value, int previous)
{
    event(RESET);
    assert(index == reset_index++ + 20);
    assert(value == -1 && previous == -index * 3);
}
void RhBase_FlipBuffer(void) { event(BEFORE_PLAY); }
void RhBase_ResetGsEnv(int value) { event(value ? PLAY_ENABLE : PLAY_DISABLE); }
int movie_play(const char *file, int first, int second)
{
    event(PLAY);
    assert(file == path && first == arg1 && second == arg2);
    return result;
}
void func_001854e0(int value) { event(AFTER_PLAY); assert(value == 0); }
void SzGame_ClearGsMem(int a, int b, int c, int d, int e, int f)
{
    event(PACKET);
    assert(a == 10 && b == 0 && c == 0 && d == 0 && e == 1024 && f == 1024);
}
void SzSound_Init(void) { event(LEAVE); }

static void append(enum Event value) { expected[expected_count++] = value; }

int main(void)
{
    int a, w, i;
    path = "synthetic/movie";
    for (a = 0; a < 2; a++) {
        for (w = 0; w < 4; w++) {
            active = a ? -7 : 0;
            waits = w;
            arg1 = a ? INT_MIN : INT_MAX;
            arg2 = w - 2;
            result = w ? INT_MIN + w : INT_MAX;
            cursor = expected_count = query_index = reset_index = 0;
            append(ACTIVE); append(WAIT);
            for (i = 0; i < w; i++) { append(WAIT_STEP); append(WAIT); }
            if (a) { append(PREPARE); append(ENABLE); append(ENTER); }
            for (i = 0; i < 10; i++) { append(QUERY); append(RESET); }
            append(BEFORE_PLAY); append(PLAY_ENABLE); append(PLAY);
            append(AFTER_PLAY); append(PLAY_DISABLE); append(PACKET);
            if (a) { append(DISABLE); append(LEAVE); }
            assert(SzGame_PlayMovie(path, arg1, arg2) == result);
            assert(cursor == expected_count && query_index == 10 && reset_index == 10);
        }
    }
    return 0;
}
