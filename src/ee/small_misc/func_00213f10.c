#include "dotr/ee/small_misc.h"

/* Blanks each record (marker 0x10000000, three zero words) whose word at +0x04
 * equals the last kept value, starting from 0. */
void lRhModelCpu_CleanupPacket(struct DotrMiscPacket128 *packet, struct DotrMiscPacket128 *end)
{
    unsigned previous = 0;
    unsigned *word;
    unsigned current;
    while (packet < end) {
        word = &packet->unknown_04;
        current = *word;
        if (current == previous) {
            packet->unknown_00 = 0x10000000;
            *word = 0;
            packet->unknown_08 = 0;
            packet->unknown_0c = 0;
        } else {
            previous = current;
        }
        packet++;
    }
}
