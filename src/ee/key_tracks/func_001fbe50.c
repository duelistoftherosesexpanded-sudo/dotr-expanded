#include "dotr/ee/key_tracks.h"

void lSzView_CalcInterpCameraMotion(struct DotrKeyOut *outputPosition, struct DotrKeyTriple *keys, int keyframeCount,
                                    float currentTime)
{
    int lo;
    int mid;
    int hi;
    struct DotrKeyTriple *currentKeyframe;
    struct DotrKeyTriple *nextKeyframe;
    float d;
    float w0;
    float w1;

    mid = 0;
    if (keyframeCount > 2) {
        lo = 0;
        hi = keyframeCount;
        for (;;) {
            mid = (lo + hi) >> 1;
            if (lo >= mid) {
                break;
            }
            if (!(currentTime < (float)keys[mid + 1].t)) {
                lo = mid;
                continue;
            }
            if (currentTime < (float)keys[mid].t) {
                hi = mid;
                continue;
            }
            break;
        }
    }
    currentKeyframe = &keys[mid];
    if (mid + 1 >= keyframeCount) {
        outputPosition->x = currentKeyframe->x;
        outputPosition->y = currentKeyframe->y;
        outputPosition->z = currentKeyframe->z;
        return;
    }
    nextKeyframe = &keys[mid + 1];
    d = (float)(nextKeyframe->t - currentKeyframe->t);
    w0 = ((float)nextKeyframe->t - currentTime) / d;
    w1 = (currentTime - (float)currentKeyframe->t) / d;
    outputPosition->x = w0 * currentKeyframe->x + w1 * nextKeyframe->x;
    outputPosition->y = w0 * currentKeyframe->y + w1 * nextKeyframe->y;
    outputPosition->z = w0 * currentKeyframe->z + w1 * nextKeyframe->z;
}
