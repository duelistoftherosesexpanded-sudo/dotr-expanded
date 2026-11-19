#include "dotr/ee/sound_queue.h"

int SD_IsAnyJobOnCD(void)
{
    return g_SDValue[0]->read_count != 0 || g_SDValue[0]->unknown_171c != 0;
}
