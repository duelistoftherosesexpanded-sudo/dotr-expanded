#include "dotr/ee/motion_items.h"

/* 0x2106e0 lRhModel_GetMultiMotionItem (name from the Japanese build's symbols). */
unsigned int lRhModel_GetMultiMotionItem(void *item0, void *item1, float frame0, float rate0, float frame1, float rate1,
                                         unsigned int flags, float *position, int *rotation, float *scale, float rate)
{
    RhMem_CopyMemory16(&DOTR_MOTION_PAD->first, DOTR_MOTION_PAD, 3);
    RhMem_CopyMemory16(&DOTR_MOTION_PAD->second, DOTR_MOTION_PAD, 3);
    flags = lRhModel_GetInterpolateMotionItem(item0, frame0, flags, DOTR_MOTION_PAD->first.position, DOTR_MOTION_PAD->first.rotation, DOTR_MOTION_PAD->first.scale, rate);
    flags = lRhModel_GetInterpolateMotionItem(item1, frame1, flags, DOTR_MOTION_PAD->second.position, DOTR_MOTION_PAD->second.rotation, DOTR_MOTION_PAD->second.scale, rate);
    position[0] = DOTR_MOTION_PAD->first.position[0] * rate0 + DOTR_MOTION_PAD->second.position[0] * rate1;
    position[1] = DOTR_MOTION_PAD->first.position[1] * rate0 + DOTR_MOTION_PAD->second.position[1] * rate1;
    position[2] = DOTR_MOTION_PAD->first.position[2] * rate0 + DOTR_MOTION_PAD->second.position[2] * rate1;
    RhGeom_RoationComplexInterpolate(rotation, DOTR_MOTION_PAD->first.rotation, DOTR_MOTION_PAD->second.rotation, rate0, rate1);
    if (!(flags & 4)) {
        scale[0] = DOTR_MOTION_PAD->first.scale[0] * rate0 + DOTR_MOTION_PAD->second.scale[0] * rate1;
        scale[1] = DOTR_MOTION_PAD->first.scale[1] * rate0 + DOTR_MOTION_PAD->second.scale[1] * rate1;
        scale[2] = DOTR_MOTION_PAD->first.scale[2] * rate0 + DOTR_MOTION_PAD->second.scale[2] * rate1;
    }
    return flags;
}
