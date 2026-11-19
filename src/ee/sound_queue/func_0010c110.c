#include "dotr/ee/sound_queue.h"

void SD_AddReadToEeReq(char *name, int destination, int offset, int length)
{
    struct DotrSoundQueueRead *read = &g_SDValue[0]->reads[g_SDValue[0]->read_write];
    func_0019bdd8(read->name, name);
    read->destination = destination;
    read->offset = offset;
    read->length = length;
    read->offset &= ~2047;
    read->destination -= offset - read->offset;
    read->length += offset - read->offset;
    read->length += 2047;
    read->length &= ~2047;
    g_SDValue[0]->read_write = (g_SDValue[0]->read_write + 1) % 16;
    g_SDValue[0]->read_count++;
}
