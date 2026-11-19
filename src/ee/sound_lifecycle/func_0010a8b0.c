#include "dotr/ee/sound_lifecycle.h"

void SD_vInitializeFileHandler(void)
{
    SDFS_ReadToEEBlock(s_szReadPCMListAtFirst, (char *)g_SDValue_lifecycle[0] + 0x210,
                 0, 0x1000);
}
