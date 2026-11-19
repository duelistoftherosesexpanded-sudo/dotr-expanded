#include "dotr/ee/motion_items.h"

/* 0x2118f0 lRhModel_GetPulseMotionItem (name from the Japanese build's symbols).
 * The integer-frame twin of 0x210cf0: the same three key searches, with an exact key-time hit
 * copied instead of interpolated. Which locals sit in the outer block of each search (none,
 * `k high`, `k high low`) and the time locals of the last two are spelled as they are because
 * the original's registers differ that way between the three searches. */
unsigned int lRhModel_GetPulseMotionItem(void *data, unsigned int frame, unsigned int flags, float *position, int *rotation, float *scale, float rate)
{
    struct DotrMotionAngleKey *rnext;
    struct DotrMotionItem *item;
    unsigned int *counts;
    struct DotrMotionAngleKey *rkey;

    if (DOTR_MOTION_PAD->flags & 4) {
        counts = ((struct DotrMotionItem *)data)->counts;
        item = (struct DotrMotionItem *)data;
    } else {
        counts = ((struct DotrMotionShortItem *)data)->counts;
        item = (struct DotrMotionItem *)data;
    }
    {
    struct DotrMotionKey *keys = (struct DotrMotionKey *)item->keys[0];
    if (keys) {
        int low;
        int high = counts[0];
        int mid;
        struct DotrMotionKey *key;
        struct DotrMotionKey *next;

        flags &= ~1;
        if (high < 3) {
            RhMem_CopyMemory4((int *)position, (int *)keys->value, 3);
            position[0] *= rate;
            position[1] *= rate;
            position[2] *= rate;
        } else {
            mid = high >> 1;
            low = 0;
            while (low < mid) {
                if (!(frame < keys[mid + 1].time)) {
                    low = mid;
                } else {
                    if (!(frame < keys[mid].time))
                        break;
                    high = mid;
                }
                mid = (low + high) >> 1;
            }
            key = &keys[mid];
            if (frame == key->time || !((unsigned int)(mid + 1) < counts[0])) {
                RhMem_CopyMemory4((int *)position, (int *)key->value, 3);
            } else {
                float span;
                float a;
                float b;

                next = &keys[mid + 1];
                span = (float)(next->time - key->time);
                a = (float)(next->time - frame);
                b = (float)(frame - key->time);
                position[0] = (a * key->value[0] + b * next->value[0]) / span;
                position[1] = (a * key->value[1] + b * next->value[1]) / span;
                position[2] = (a * key->value[2] + b * next->value[2]) / span;
            }
            position[0] *= rate;
            position[1] *= rate;
            position[2] *= rate;
        }
    }
    }
    {
    struct DotrMotionAngleKey *k;
    int high;
    struct DotrMotionAngleKey *keys = (struct DotrMotionAngleKey *)item->keys[1];
    if (keys) {
        int low;
        int mid;
        unsigned int nt;
        unsigned int kt;

        high = counts[1];
        flags &= ~2;
        if (high < 3) {
            RhMem_CopyMemory4((int *)rotation, (int *)keys->value, 3);
        } else {
            mid = high >> 1;
            low = 0;
            while (low < mid) {
                k = &keys[mid];
                if (!(frame < k[1].time)) {
                    low = mid;
                } else {
                    if (!(frame < k->time))
                        break;
                    high = mid;
                }
                mid = (low + high) >> 1;
            }
            rkey = &keys[mid];
            kt = rkey->time;
            if (frame == kt || !((unsigned int)(mid + 1) < counts[1])) {
                RhMem_CopyMemory4((int *)rotation, (int *)rkey->value, 3);
            } else {
                float span;
                float a;
                float b;

                rnext = &keys[mid + 1];
                nt = rnext->time;
                span = (float)(nt - kt);
                a = (float)(nt - frame) / span;
                b = (float)(frame - kt) / span;
                rotation[0] = RhGeom_RotationInterpolate(a, b, rkey->value[0], rnext->value[0]);
                rotation[1] = RhGeom_RotationInterpolate(a, b, rkey->value[1], rnext->value[1]);
                rotation[2] = RhGeom_RotationInterpolate(a, b, rkey->value[2], rnext->value[2]);
            }
        }
    }
    }
    if (DOTR_MOTION_PAD->flags & 4) {
    struct DotrMotionKey *k;
    int high;
    int low;
    struct DotrMotionKey *keys = (struct DotrMotionKey *)item->keys[2];
    if (keys) {
        int mid;
        unsigned int nt;
        unsigned int kt;
        struct DotrMotionKey *key;
        struct DotrMotionKey *next;

        high = counts[2];
        flags &= ~4;
        if (high < 3) {
            RhMem_CopyMemory4((int *)scale, (int *)keys->value, 3);
        } else {
            mid = high >> 1;
            low = 0;
            while (low < mid) {
                k = &keys[mid];
                if (!(frame < k[1].time)) {
                    low = mid;
                } else {
                    if (!(frame < k->time))
                        break;
                    high = mid;
                }
                mid = (low + high) >> 1;
            }
            key = &keys[mid];
            kt = key->time;
            if (frame == kt || !((unsigned int)(mid + 1) < counts[2])) {
                RhMem_CopyMemory4((int *)scale, (int *)key->value, 3);
            } else {
                float span;
                float a;
                float b;

                next = &keys[mid + 1];
                nt = next->time;
                span = (float)(nt - kt);
                a = (float)(nt - frame) / span;
                b = (float)(frame - kt) / span;
                scale[0] = a * key->value[0] + b * next->value[0];
                scale[1] = a * key->value[1] + b * next->value[1];
                scale[2] = a * key->value[2] + b * next->value[2];
            }
        }
    }
    }
    return flags;
}
