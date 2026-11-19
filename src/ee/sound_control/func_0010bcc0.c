#include "dotr/ee/sound_control.h"

int SD_AddToXferQueue(int a, void *data)
{
    int write = g_SDValue[0]->ring_write;
    int next = (write + 1) & 7;
    if (next == g_SDValue[0]->ring_read) {
        return 0;
    }
    g_SDValue[0]->ring[write].unknown_000 = a;
    func_0019c2b8(g_SDValue[0]->ring[write].data, data, 28);
    g_SDValue[0]->ring_write = next;
    return 1;
}
