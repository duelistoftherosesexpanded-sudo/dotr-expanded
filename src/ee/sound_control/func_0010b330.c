#include "dotr/ee/sound_control.h"

void SD_vInitSDThread(void)
{
    if (!(g_SDValue[0]->unknown_0000 & 0x10)) {
        g_SDValue[0]->unknown_0000 |= 0x10;
    }
}
