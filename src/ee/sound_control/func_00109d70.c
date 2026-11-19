#include "dotr/ee/sound_control.h"

void SD_SuspendSoundSystem(int value)
{
    if (value) {
        SD_BGMSetSourceLevel(0, 0);
        SDRPC_SendCommand(0x5004, 32767);
    }
    SDRPC_SendCommand(0xf006, value);
}
