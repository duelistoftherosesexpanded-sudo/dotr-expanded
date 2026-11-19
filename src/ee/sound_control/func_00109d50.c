#include "dotr/ee/sound_control.h"

int SD_Term(void)
{
    return SDRPC_SendCommand(0xf004, g_SDValue[0]->unknown_0014);
}
