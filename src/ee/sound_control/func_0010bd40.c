#include "dotr/ee/sound_control.h"

int SD_FetchFromXferQueue(int *a, void *data)
{
    int read = g_SDValue[0]->ring_read;
    if (read == g_SDValue[0]->ring_write) {
        return 0;
    }
    g_SDValue[0]->ring_read = (read + 1) & 7;
    *a = g_SDValue[0]->ring[read].unknown_000;
    func_0019c2b8(data, g_SDValue[0]->ring[read].data, 28);
    return 1;
}
