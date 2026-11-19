/* Synthetic host coverage of C behavior, not the EE variadic register ABI. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/sound_command.h"

struct DotrSoundCommandManager *g_SDValue_commands[1];
static struct DotrSoundCommandManager block;
static int calls, expected_command, result;
static int expected_words[6];

int func_0017ea80(void *rpc, int command, int mode, void *send, int send_size,
                  void *receive, int receive_size, void *callback, void *argument)
{
    int i;
    calls++;
    assert(rpc == block.unknown_001c && command == expected_command && mode == 0);
    assert(send == block.unknown_0080 && send_size == 64);
    assert(receive == block.unknown_0180 && receive_size == 16);
    assert(callback == 0 && argument == 0);
    assert(block.unknown_0080[0] == (int)(intptr_t)block.unknown_0080);
    for (i = 0; i < 6; i++) {
        assert(block.unknown_0080[i + 1] == expected_words[i]);
    }
    return result;
}

static void check(int command, int dependency_result)
{
    struct DotrSoundCommandManager expected;
    int i;
    memset(&block, 0x5a, sizeof(block));
    expected = block;
    expected.unknown_0080[0] = (int)(intptr_t)block.unknown_0080;
    for (i = 0; i < 6; i++) {
        expected.unknown_0080[i + 1] = expected_words[i];
    }
    expected_command = command;
    result = dependency_result;
    assert(SDRPC_SendCommand(command, expected_words[0], expected_words[1], expected_words[2],
                         expected_words[3], expected_words[4], expected_words[5]) == result);
    assert(memcmp(&block, &expected, sizeof(block)) == 0);
}

int main(void)
{
    assert(offsetof(struct DotrSoundCommandManager, unknown_001c) == 0x1c);
    assert(offsetof(struct DotrSoundCommandManager, unknown_0080) == 0x80);
    assert(offsetof(struct DotrSoundCommandManager, unknown_0180) == 0x180);
    assert(SDRPC_SendCommand(123) == 0 && calls == 0);
    if ((intptr_t)block.unknown_0080 != (int)(intptr_t)block.unknown_0080) {
        puts("skipped: buffer self-address does not fit a signed 32-bit word");
        return 77;
    }
    g_SDValue_commands[0] = &block;
    expected_words[0] = INT_MIN;
    expected_words[1] = INT_MAX;
    expected_words[2] = -1;
    expected_words[3] = 0;
    expected_words[4] = 0x12345678;
    expected_words[5] = -123;
    check(0x5003, -77);
    check(-1, 0x4321);
    assert(calls == 2);
    g_SDValue_commands[0] = 0;
    assert(SDRPC_SendCommand(0) == 0 && calls == 2);
    puts("sound_command: ok");
    return 0;
}
