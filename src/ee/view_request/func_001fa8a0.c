#include "dotr/ee/view_request.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* SzView_ReqCamera: only view 0 (its block non-null) is served. Takes the first free of the
 * eight request slots (0 when none) and records the start and end value by request type:
 * 0 field of view (converted to an integer) to `value`; 1 roll to `value`, both modulo 0x10000
 * and folded above 0x8000 to negative; 2 direction, 3 position, 4/5 polar angles to the vector
 * at `data` (for 4/5 the end angle y is moved by 2 pi when the two angles, in 0x8000-per-pi
 * units rounded to 0x1000, are 0x8000 or more apart); 6/7 position to `value`; 8/9 the track at
 * `data` to `value`, `count` defaulting to its frame count - 1. Any other type returns 0. A zero
 * count becomes 1; the slot is marked active with its type, owner, target and count, and 1 is
 * returned. Two inline helpers precede it. Compiled warm (float-to-int conversions as casts). */

static inline struct DotrViewReqSlot *findFree_001FA8A0(void) {
    struct DotrViewReqSlot *slot = (struct DotrViewReqSlot *)gSzView_CameraReq_req;
    int i = 0;

    do {
        if (!slot->active) {
            return slot;
        }
        i++;
        slot++;
    } while (i < 8);
    return 0;
}

static inline float toAngle_001FA8A0(float radians) {
    return (32768.0f * radians) / 3.1415927f;
}

int SzView_ReqCamera(int owner, int target, int type, int count, int value, void *data)
{
    struct DotrViewReqCamera *camera;
    struct DotrViewReqSlot *slot;
    float fromAngle;
    float toAngle;
    int fromSign;
    int toSign;
    int delta;

    if (owner < 0 || owner > 0) {
        return 0;
    }
    camera = (struct DotrViewReqCamera *)gSzView_hView_req[owner];
    if (camera == 0) {
        return 0;
    }
    slot = findFree_001FA8A0();
    if (slot == 0) {
        return 0;
    }
    switch (type) {
    case 0:
        slot->from.i = (int)camera->fov;
        slot->to.i = value;
        break;
    case 1:
        slot->from.i = camera->roll % 0x10000;
        if (slot->from.i > 0x8000) {
            slot->from.i = -(0x10000 - slot->from.i);
        }
        slot->to.i = value % 0x10000;
        if (slot->to.i > 0x8000) {
            slot->to.i = -(0x10000 - slot->to.i);
        }
        break;
    case 2:
        slot->from.v = camera->direction;
        slot->to.v = *(struct DotrViewReqVec *)data;
        break;
    case 3:
        slot->from.v = camera->position;
        slot->to.v = *(struct DotrViewReqVec *)data;
        break;
    case 4:
    case 5:
        slot->from.v = camera->polar;
        slot->to.v = *(struct DotrViewReqVec *)data;
        fromAngle = toAngle_001FA8A0(slot->from.v.y);
        fromSign = ((int)fromAngle > 0) ? 0x1000 : -0x1000;
        toAngle = toAngle_001FA8A0(slot->to.v.y);
        toSign = ((int)toAngle > 0) ? 0x1000 : -0x1000;
        delta = (int)toAngle;
        delta = (((delta + (toSign >> 1)) >> 12) - (((int)fromAngle + (fromSign >> 1)) >> 12)) << 12;
        if (delta < -0x8000) {
            slot->to.v.y += 6.2831855f;
        } else if (delta >= 0x8000) {
            slot->to.v.y -= 6.2831855f;
        }
        break;
    case 6:
    case 7:
        slot->from.v = camera->position;
        slot->to.i = value;
        break;
    case 8:
    case 9:
        slot->from.p = data;
        slot->to.i = value;
        if (count == 0) {
            count = (unsigned short)(((struct DotrViewReqTrack *)data)->frames - 1);
        }
        break;
    default:
        return 0;
    }
    if (count == 0) {
        count = (unsigned short)(count + 1);
    }
    slot->active = 1;
    slot->type = type;
    slot->flag = 0;
    slot->other = 0;
    slot->owner = owner;
    slot->target = target;
    slot->count = count;
    slot->timer = 0;
    return 1;
}
