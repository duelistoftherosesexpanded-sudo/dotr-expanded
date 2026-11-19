/* Synthetic host tests of the call order and guards of the sound_lifecycle group; the command
 * callees are stubs. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/sound_lifecycle.h"

struct DotrSoundLifecycleManager *g_SDValue_lifecycle[1];
char *g_SDBuffer[1];
char s_szReadPCMListAtFirst[] = "<name>";
static struct DotrSoundLifecycleManager block;
static char enabled;
static int trace[24], count, expected_table_index;
static const int values[12] = {
    0x5010, 0x9f810, 0x199810, 0x1cb810, 0x1fd810, 0x8f810,
    0x9f810, 0xaf810, 0xbf810, 0xd7810, 0xef810, 0x107810
};

int SDRPC_Init(void) { trace[count++] = -1; return -99; }
void SD_vInitSDThread(void) { trace[count++] = -3; }
void SD_vInitializeAutoDMA(void) { trace[count++] = -4; }

int SDFS_ReadToEEBlock(char *name, char *destination, int offset, int size)
{
    assert(name == s_szReadPCMListAtFirst && destination == (char *)&block + 0x210);
    assert(offset == 0 && size == 0x1000);
    trace[count++] = -2;
    return -7;
}

/* Reads only the argument words these call sites pass. */
int SDRPC_SendCommand(int command, ...)
{
    va_list list;
    int index;
    trace[count++] = command;
    va_start(list, command);
    if (command == 0xf002) {
        assert(va_arg(list, int) == 0xc1800);
        block.unknown_0180 = 0x7654321; /* Assumed reply of the stubbed command. */
    } else if (command == 0x5f10) {
        index = va_arg(list, int);
        assert(index == expected_table_index++ && index >= 0 && index < 12);
        assert(va_arg(list, int) == values[index]);
        assert(trace[count - 2] == (index ? 0x5f10 : 0xf002) ||
               (index == 0 && trace[count - 2] == 0xf005));
    } else if (command == 0x5f12) {
        assert(va_arg(list, int) == 0x150000 && va_arg(list, int) == 0x38000);
    } else {
        assert(command == 0xf005);
    }
    va_end(list);
    return -1;
}

static void check(int preloaded)
{
    struct DotrSoundLifecycleManager expected;
    int i, start;
    memset(&block, 0x5a, sizeof(block));
    block.unknown_0000 = preloaded ? 0x20 : 0;
    block.unknown_0004 = 0x10;
    expected = block;
    expected.unknown_0000 |= 0xa1;
    expected.unknown_0004 |= 14;
    expected.unknown_2a6c = 0;
    if (!preloaded) {
        expected.unknown_0014 = expected.unknown_0180 = 0x7654321;
    }
    count = 0;
    expected_table_index = 0;
    SD_InitializeDevice();
    assert(memcmp(&block, &expected, sizeof(block)) == 0);
    assert(count == (preloaded ? 18 : 19));
    assert(expected_table_index == 12);
    assert(trace[0] == -1 && trace[1] == 0xf005);
    start = preloaded ? 2 : 3;
    if (!preloaded) {
        assert(trace[2] == 0xf002);
    }
    for (i = 0; i < 12; i++) {
        assert(trace[start + i] == 0x5f10);
    }
    assert(trace[start + 12] == 0x5f12 && trace[start + 13] == -2);
    assert(trace[start + 14] == -3 && trace[start + 15] == -4);
    count = 0;
    block.unknown_0000 &= ~0x80;
    SD_InitializeDevice();
    assert(count == 0 && block.unknown_0000 == expected.unknown_0000);
}

int main(void)
{
    struct DotrSoundLifecycleManager saved;
    assert(offsetof(struct DotrSoundLifecycleManager, unknown_0014) == 0x14);
    assert(offsetof(struct DotrSoundLifecycleManager, unknown_0180) == 0x180);
    assert(offsetof(struct DotrSoundLifecycleManager, unknown_2a6c) == 0x2a6c);
    memset(&block, 0x5a, sizeof(block));
    saved = block;
    g_SDBuffer[0] = &enabled;
    SD_InitializeDevice();
    g_SDValue_lifecycle[0] = &block;
    g_SDBuffer[0] = 0;
    SD_InitializeDevice();
    assert(count == 0 && memcmp(&block, &saved, sizeof(block)) == 0);
    g_SDBuffer[0] = &enabled;
    check(0);
    check(1);
    count = 0;
    SD_vInitializeFileHandler();
    assert(count == 1 && trace[0] == -2);
    puts("sound_lifecycle: ok");
    return 0;
}
