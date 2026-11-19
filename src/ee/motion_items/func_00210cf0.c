#include "dotr/ee/motion_items.h"

/* 0x210cf0 lRhModel_GetInterpolateMotionItem (name from the Japanese build's symbols).
 * Three key searches of the same shape; the first is spelled without the loop pointer `k` and
 * the third with explicit time locals because the original's registers differ that way. */
unsigned int lRhModel_GetInterpolateMotionItem(void *data, float frame, unsigned int flags, float *position, int *rotation, float *scale, float rate)
{
    struct DotrMotionAngleKey *rkey;
    struct DotrMotionAngleKey *rnext;
    struct DotrMotionItem *item;
    unsigned int *counts;

    /* MATCHING DEVICE: the original copies the pointer in both branches; one assignment after
     * the test compiles to a single copy. */
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
                if (!(frame < (float)keys[mid + 1].time)) {
                    low = mid;
                } else {
                    if (!(frame < (float)keys[mid].time))
                        break;
                    high = mid;
                }
                mid = (low + high) >> 1;
            }
            key = &keys[mid];
            if (!((unsigned int)(mid + 1) < counts[0])) {
                RhMem_CopyMemory4((int *)position, (int *)key->value, 3);
                position[0] *= rate;
                position[1] *= rate;
                position[2] *= rate;
            } else {
                float span;
                float a;
                float b;

                next = &keys[mid + 1];
                span = (float)(next->time - key->time);
                a = ((float)next->time - frame) / span;
                b = (frame - (float)key->time) / span;
                position[0] = a * key->value[0] + b * next->value[0];
                position[1] = a * key->value[1] + b * next->value[1];
                position[2] = a * key->value[2] + b * next->value[2];
                position[0] *= rate;
                position[1] *= rate;
                position[2] *= rate;
            }
        }
    }
    }
    {
    struct DotrMotionAngleKey *k;
    struct DotrMotionAngleKey *keys = (struct DotrMotionAngleKey *)item->keys[1];
    if (keys) {
        int high = counts[1];
        int low;
        int mid;

        flags &= ~2;
        if (high < 3) {
            RhMem_CopyMemory4((int *)rotation, (int *)keys->value, 3);
        } else {
            mid = high >> 1;
            low = 0;
            while (low < mid) {
                k = &keys[mid];
                if (!(frame < (float)k[1].time)) {
                    low = mid;
                } else {
                    if (!(frame < (float)k->time))
                        break;
                    high = mid;
                }
                mid = (low + high) >> 1;
            }
            rkey = &keys[mid];
            if (!((unsigned int)(mid + 1) < counts[1])) {
                RhMem_CopyMemory4((int *)rotation, (int *)rkey->value, 3);
            } else {
                float span;
                float a;
                float b;

                rnext = &keys[mid + 1];
                span = (float)(rnext->time - rkey->time);
                a = ((float)rnext->time - frame) / span;
                b = (frame - (float)rkey->time) / span;
                rotation[0] = RhGeom_RotationInterpolate(a, b, rkey->value[0], rnext->value[0]);
                rotation[1] = RhGeom_RotationInterpolate(a, b, rkey->value[1], rnext->value[1]);
                rotation[2] = RhGeom_RotationInterpolate(a, b, rkey->value[2], rnext->value[2]);
            }
        }
    }
    }
    if (DOTR_MOTION_PAD->flags & 4) {
    {
    struct DotrMotionKey *k;
    struct DotrMotionKey *keys = (struct DotrMotionKey *)item->keys[2];
    if (keys) {
        int high = counts[2];
        int low;
        int mid;
        unsigned int nt;
        unsigned int kt;
        struct DotrMotionKey *key;
        struct DotrMotionKey *next;

        flags &= ~4;
        if (high < 3) {
            RhMem_CopyMemory4((int *)scale, (int *)keys->value, 3);
        } else {
            mid = high >> 1;
            low = 0;
            while (low < mid) {
                k = &keys[mid];
                if (!(frame < (float)k[1].time)) {
                    low = mid;
                } else {
                    if (!(frame < (float)k->time))
                        break;
                    high = mid;
                }
                mid = (low + high) >> 1;
            }
            key = &keys[mid];
            if (!((unsigned int)(mid + 1) < counts[2])) {
                RhMem_CopyMemory4((int *)scale, (int *)key->value, 3);
            } else {
                float span;
                float a;
                float b;

                next = &keys[mid + 1];
                kt = key->time;
                nt = next->time;
                span = (float)(nt - kt);
                a = ((float)nt - frame) / span;
                b = (frame - (float)kt) / span;
                scale[0] = a * key->value[0] + b * next->value[0];
                scale[1] = a * key->value[1] + b * next->value[1];
                scale[2] = a * key->value[2] + b * next->value[2];
            }
        }
    }
    }
    }
    return flags;
}
