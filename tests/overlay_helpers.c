/* Synthetic host tests for the overlay helpers (overlay_initialize.c, initializer_list.c). Fixtures
 * are host data, not game data; stubbed callees have assumed semantics. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/overlay.h"

static int stage, expected_stage, callback_count, callback_order[3];
static unsigned char *expected_destination;
static unsigned int expected_zero_fill;

void func_0017bb60(int mode)
{
    assert(stage == 0 && mode == 2);
    stage = 1;
}

void *func_00198948(void *destination, int fill, unsigned int count)
{
    assert(stage == 1);
    assert(destination == expected_destination);
    assert(fill == 0 && count == expected_zero_fill);
    stage = 2;
    return memset(destination, fill, count);
}

static void callback_first(void)
{
    assert(stage == expected_stage);
    callback_order[callback_count++] = 1;
}

static void callback_second(void)
{
    assert(stage == expected_stage);
    callback_order[callback_count++] = 2;
}

static void callback_third(void)
{
    assert(stage == expected_stage);
    callback_order[callback_count++] = 3;
}

int main(void)
{
    struct Fixture {
        struct DotrOverlayInitPrefix header;
        unsigned char bytes[256];
    } state;
    DotrOverlayInitializer callbacks[3] = {callback_first, callback_second, callback_third};
    unsigned int sizes[3] = {0, 1, 128};
    unsigned int i, byte;
    int count;

    /* Empty and reversed ranges within one array must call nothing. */
    callback_count = 0;
    __initialize_cpp_rts(callbacks, callbacks, INT_MIN, INT_MAX);
    __initialize_cpp_rts(callbacks + 2, callbacks + 1, 7, 9);
    assert(callback_count == 0);

    for (i = 0; i < 3; i++) {
        for (count = 0; count <= 3; count++) {
            memset(&state, 0x5a, sizeof(state));
            state.header.zero_fill_size = sizes[i];
            state.header.initializers_begin = callbacks;
            state.header.initializers_end = callbacks + count;
            expected_destination = state.bytes + 32;
            expected_zero_fill = sizes[i];
            stage = 0;
            expected_stage = sizes[i] ? 2 : 1;
            callback_count = 0;
            mwOverlayInit(&state, (int)(offsetof(struct Fixture, bytes) + 32));
            assert(stage == expected_stage && callback_count == count);
            for (byte = 0; byte < sizeof(state.bytes); byte++) {
                unsigned char expected = byte >= 32 && byte < 32 + sizes[i] ? 0 : 0x5a;
                assert(state.bytes[byte] == expected);
            }
            for (byte = 0; byte < (unsigned int)count; byte++) {
                assert(callback_order[byte] == (int)byte + 1);
            }
        }
    }
    return 0;
}
