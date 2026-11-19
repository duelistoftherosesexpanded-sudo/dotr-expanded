#include "dotr/ee/sound_file.h"

void audioDecBeginPut(struct DotrSoundRing *ring, int *start1, int *count1, int *start2, int *count2)
{
    int span;
    if (ring->mode == 0) {
        *start1 = (DotrSoundAddress)&ring->unknown_004[0] + ring->unknown_2c;
        *count1 = 40 - ring->unknown_2c;
        *start2 = ring->unknown_30;
        *count2 = ring->unknown_3c;
    } else {
        span = ring->unknown_3c - ring->unknown_38;
        if (!(ring->unknown_3c - ring->unknown_34 < span)) {
            *start1 = ring->unknown_30 + ring->unknown_34;
            *count1 = span;
            *start2 = 0;
            *count2 = 0;
        } else {
            *start1 = ring->unknown_30 + ring->unknown_34;
            *count1 = ring->unknown_3c - ring->unknown_34;
            *start2 = ring->unknown_30;
            *count2 = span - (ring->unknown_3c - ring->unknown_34);
        }
    }
}
