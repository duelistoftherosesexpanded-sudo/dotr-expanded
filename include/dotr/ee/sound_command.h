#ifndef DOTR_EE_SOUND_COMMAND_H
#define DOTR_EE_SOUND_COMMAND_H

/* A separate minimal view of the manager reached through EE 0x30ff40.
 * It aliases the sound_queue pointer word; neither view is an original type.
 * Evidence: docs/modules/other.md.
 */
struct DotrSoundCommandManager {
    int unknown_0000;
    unsigned char unknown_0004[0x18];
    unsigned char unknown_001c[0x64];
    int unknown_0080[16];
    unsigned char unknown_00c0[0xc0];
    unsigned char unknown_0180[0x40];
};
typedef char DotrSoundCommandIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];
typedef char DotrSoundCommandManagerSizeMustBe1c0[
    (sizeof(struct DotrSoundCommandManager) == 0x1c0) ? 1 : -1];

extern struct DotrSoundCommandManager *g_SDValue_commands[];

/* Assumed prototype of an RPC-style library routine; the command meanings are unknown. */
extern int func_0017ea80(void *block, int command, int mode, void *send, int send_size,
                         void *receive, int receive_size, void *callback, void *argument);
int SDRPC_SendCommand(int command, ...);

#endif
