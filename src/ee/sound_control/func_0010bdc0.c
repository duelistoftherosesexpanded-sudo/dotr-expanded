#include "dotr/ee/sound_control.h"

void SD_ClearXferQueue(void)
{
    g_SDValue[0]->ring_read = 0;
    g_SDValue[0]->ring_write = 0;
}
