#include "dotr/ee/sound_command.h"

#if !defined(__MWERKS__)
#include <stdarg.h>
#include <stdint.h>
#endif

#define manager (g_SDValue_commands[0])

int SDRPC_SendCommand(int command, ...)
{
    int i;
    int id;
#if defined(__MWERKS__)
    int count;
    char *arguments;
#else
    va_list arguments;
#endif

    if (!manager) {
        return 0;
    }
    id = command;
#if defined(__MWERKS__)
    /* In this compiler, __builtin_args_info(2) gives the register argument count; next_arg
     * points past the eight 8-byte register slots. These are not recovered SDK macros.
     * MATCHING DEVICE: the local declaration order gives the original's register allocation.
     */
    count = __builtin_args_info(2);
    if (count >= 8) {
        count = 0;
    } else {
        count = (8 - count) * 8;
    }
    arguments = (char *)__builtin_next_arg(command) - count;
    manager->unknown_0080[0] = (int)&manager->unknown_0080[0];
#else
    va_start(arguments, command);
    manager->unknown_0080[0] = (int)(intptr_t)&manager->unknown_0080[0];
#endif
    for (i = 1; i < 7; i++) {
#if defined(__MWERKS__)
        arguments += 8;
        manager->unknown_0080[i] = *(int *)(arguments - 8);
#else
        manager->unknown_0080[i] = va_arg(arguments, int);
#endif
    }
#if !defined(__MWERKS__)
    va_end(arguments);
#endif
    return func_0017ea80(manager->unknown_001c, id, 0, manager->unknown_0080, 64,
                         manager->unknown_0180, 16, 0, 0);
}
