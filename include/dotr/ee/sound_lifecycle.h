#ifndef DOTR_EE_SOUND_LIFECYCLE_H
#define DOTR_EE_SOUND_LIFECYCLE_H

/* Offset view of the accessed fields only; aliases the pointer word at EE 0x30ff40 used by the
 * queue, control and command views. Structure layouts and prototypes are inferred, not the
 * original declarations.
 * Evidence: docs/modules/other.md.
 */
struct DotrSoundLifecycleManager {
    int unknown_0000;
    int unknown_0004;
    unsigned char unknown_0008[0xc];
    int unknown_0014;
    unsigned char unknown_0018[0x168];
    int unknown_0180;
    unsigned char unknown_0184[0x28e8];
    int unknown_2a6c;
};
typedef char DotrSoundLifecycleIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];
typedef char DotrSoundLifecycleSizeMustBe2a70[
    (sizeof(struct DotrSoundLifecycleManager) == 0x2a70) ? 1 : -1];
extern struct DotrSoundLifecycleManager *g_SDValue_lifecycle[];
extern char *g_SDBuffer[];
extern char s_szReadPCMListAtFirst[];

extern int SDRPC_Init(void);
extern int SDRPC_SendCommand(int command, ...);
extern int SDFS_ReadToEEBlock(char *name, char *destination, int offset, int size);
extern void SD_vInitSDThread(void);
extern void SD_vInitializeAutoDMA(void);
void SD_InitializeDevice(void);
void SD_vInitializeFileHandler(void);

#endif
