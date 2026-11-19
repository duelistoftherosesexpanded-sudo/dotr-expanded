#include "dotr/ee/sound_file.h"

/* Accounts for count more bytes. While the state word is 0 the first bytes
 * fill the 40-byte head: once it is full the state becomes 1 and the head is
 * printed. What is left of count advances the position modulo the size and
 * is added to the two totals. */
void audioDecEndPut(struct DotrSoundFeed *feed, int count)
{
    unsigned take;

    if (feed->state == 0) {
        take = 40 - feed->have;
        take = take < (unsigned)count ? take : (unsigned)count;
        feed->have += take;
        if (feed->have >= 40) {
            feed->state = 1;
            func_00199b50(data_0030f645);
            func_00199b50(data_0030f676, feed->head[0], feed->head[1], feed->head[2], feed->head[3], feed->unknown_08[0],
                          feed->unknown_08[1], feed->unknown_08[2], feed->unknown_08[3], feed->unknown_08[4],
                          feed->unknown_08[5], feed->unknown_08[6]);
            func_00199b50(data_0030f7b1, feed->tag[0], feed->tag[1], feed->tag[2], feed->tag[3], feed->unknown_28);
        }
        count -= take;
    }
    feed->position = (feed->position + count) % feed->size;
    feed->total += count;
    feed->sum += count;
}
