#include "dotr/ee/view_requests.h"

/* 0x1fa220. Compiled warm (see its build entry). */
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

static inline int lerp_001FA220(int from, int to, float t) {
    int d = to - from;

    return from + (int)(d * t);
}

static inline int setFov_001FA220(int index, int fov) {
    struct DotrViewReqCamera *camera;

    if (index < 0 || index > 0) {
        return 0;
    }
    camera = (struct DotrViewReqCamera *)gSzView_hView_viewreq[index];
    if (camera == NULL) {
        return 0;
    }
    camera->fov = fov;
    return 1;
}

static inline int setRoll_001FA220(int index, int roll) {
    struct DotrViewReqCamera *camera;

    if (index < 0 || index > 0) {
        return 0;
    }
    camera = (struct DotrViewReqCamera *)gSzView_hView_viewreq[index];
    if (camera == NULL) {
        return 0;
    }
    camera->roll = (int)(float)roll;
    return 1;
}

static inline int setPosition_001FA220(int index, struct DotrViewReqVector4 *position) {
    struct DotrViewReqCamera *camera;

    if (index < 0 || index > 0) {
        return 0;
    }
    camera = (struct DotrViewReqCamera *)gSzView_hView_viewreq[index];
    if (camera == NULL) {
        return 0;
    }
    if (position == NULL) {
        position = &gSzView_pntSPDef;
    }
    camera->position = *position;
    lSzView_CalcRel(index);
    lSzView_CalcLook(index);
    return 1;
}

static inline int setTarget_001FA220(int index, struct DotrViewReqVector4 *target) {
    struct DotrViewReqCamera *camera;

    if (index < 0 || index > 0) {
        return 0;
    }
    camera = (struct DotrViewReqCamera *)gSzView_hView_viewreq[index];
    if (camera == NULL) {
        return 0;
    }
    if (target == NULL) {
        target = (struct DotrViewReqVector4 *)&gSzView_pntCPDef;
    }
    camera->target = *target;
    lSzView_CalcRel(index);
    lSzView_CalcLook(index);
    return 1;
}

void SzView_DoRequest(int owner, int target)
{
    extern struct DotrViewReqSlot gSzView_CameraReq[];
    extern int SzGame_GetFrameSpeed();
    struct DotrViewReqSlot *slot;
    int delta;
    int i;
    int done;
    int timer;
    int d;
    int nt;
    float t;
    struct DotrViewReqVector4 pos;
    struct DotrViewReqVector4 polar;

    slot = gSzView_CameraReq;
    delta = SzGame_GetFrameSpeed();
    i = 0;
    do {
        if (slot->active && !slot->flag && (owner < 0 || slot->owner == owner) &&
            (target < 0 || slot->target == target)) {
            done = 0;
            timer = slot->timer + delta;
            if (timer < 0xFFFF) {
                nt = timer;
            } else {
                nt = 0xFFFF;
            }
            slot->timer = nt;
            if (slot->timer >= slot->count) {
                done = 1;
            }
            t = (float)slot->timer / (float)slot->count;
            switch (slot->type) {
            case 0:
                d = slot->to.i - slot->from.i;
                setFov_001FA220(slot->owner, slot->from.i + (int)(d * t));
                break;
            case 1:
                d = slot->to.i - slot->from.i;
                setRoll_001FA220(slot->owner, slot->from.i + (int)(d * t));
                break;
            case 2:
            case 3:
                pos.x = slot->from.v.x + t * (slot->to.v.x - slot->from.v.x);
                pos.y = slot->from.v.y + t * (slot->to.v.y - slot->from.v.y);
                pos.z = slot->from.v.z + t * (slot->to.v.z - slot->from.v.z);
                if (slot->type == 2) {
                    setPosition_001FA220(slot->owner, &pos);
                } else {
                    setTarget_001FA220(slot->owner, &pos);
                }
                break;
            case 4:
            case 5:
                polar.x = slot->from.v.x + t * (slot->to.v.x - slot->from.v.x);
                polar.y = slot->from.v.y + t * (slot->to.v.y - slot->from.v.y);
                polar.z = slot->from.v.z + t * (slot->to.v.z - slot->from.v.z);
                if (slot->type == 4) {
                    SzView_SetLookSP(slot->owner, &polar.x);
                } else {
                    SzView_SetLookCP(slot->owner, &polar.x);
                }
                break;
            case 6:
                SzView_SetViewCamera(slot->owner, slot->to.i, &slot->from.v.x, 1, t);
                break;
            case 7:
                SzView_SetViewCamera(slot->owner, slot->to.i, &slot->from.v.x, 2, t);
                break;
            case 8:
            case 9:
                SzView_SetInterpCameraMotion(slot->owner, slot->from.p, slot->count, slot->timer, slot->to.i, slot->type == 9);
                break;
            }
            if (done) {
                slot->active = 0;
            }
        }
        i++;
        slot++;
    } while (i < 8);
}
