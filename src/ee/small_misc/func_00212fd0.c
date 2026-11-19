#include "dotr/ee/small_misc.h"

void *RhModelCpu_DrawAlpha(struct DotrMiscPacketHead *head, int a, int b, int flag)
{
    struct DotrMiscPacket128 *packet;
    struct DotrMiscPacket128 *end;

    lRhModelCpu_SetupGlobal(flag);
    head->words[0] = 0x10000002;
    head->words[1] = 0;
    head->words[2] = 0x11000000;
    head->words[3] = 0x51000002;
    head->tags[0] = 0x1000000000008001;
    head->tags[1] = 14;
    head->tags[2] = 115;
    head->tags[3] = 0;
    packet = (struct DotrMiscPacket128 *)(head + 1);
    end = lRhModelCpu_TraceDynamicBlock(packet, a, b);
    if (end == 0) {
        return head;
    }
    lRhModelCpu_SortPolygon(packet, end);
    lRhModelCpu_CleanupPacket(packet, end);
    return end;
}
