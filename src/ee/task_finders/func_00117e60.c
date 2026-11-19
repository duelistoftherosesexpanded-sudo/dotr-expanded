#include "dotr/ee/task_finders.h"

static inline int CRun_val(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int done)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_0011a4d0_fnd(unit, side, task, first);
        done = 1;
    }
    return done;
}

static inline int CMove_val(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, int done)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_0011b680_fnd(unit, side, task, first, e);
        done = 1;
    }
    return done;
}

static inline int CEnc_val(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int done)
{
    func_0027d020_fnd(unit, side, task, first, 0, func_0011aa10_fnd(side), 3, 1);
    done = 1;
    return done;
}

static inline void CRun_void(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int *pd)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_0011a4d0_fnd(unit, side, task, first);
        *pd = 1;
    }
}

static inline void CMove_void(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, int *pd)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_0011b680_fnd(unit, side, task, first, e);
        *pd = 1;
    }
}

static inline void CEnc_void(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int *pd)
{
    func_0027d020_fnd(unit, side, task, first, 0, func_0011aa10_fnd(side), 3, 1);
    *pd = 1;
}

int TaLogic_GetMinimumTask__00117e60(struct DotrFinderUnit *list, struct DotrFinderTask *task, int owner)
{
    int side = func_001de540_fnd();
    int done = 0;
    int r;
    int idx;

    (void)owner;
    if ((r = func_00118510_fnd(list, side, task)) < 0) {
        return 0;
    }
    idx = func_00276c80_fnd(list, r & 0xffff);
    if (idx < 0) {
        return 0;
    }
    if ((r >> 16) == 1) {
        if (((list[idx].wPurpose >> 8) & 0xff) == 10) {
            func_0027b1b0_fnd(&list[idx], 1);
        } else {
            func_0027b1b0_fnd(&list[idx], 0);
        }
        return 1;
    }
    func_00118330_fnd(&list[idx]);
    switch ((list[idx].wPurpose >> 8) & 0xff) {
    case 5:
        CRun_void(&list[idx], side, task, 1, &done);
        break;
    case 2:
        if (func_001e2100_fnd(list[idx].wUnit >> 8, list[idx].wUnit & 0xff, 0) == 0) {
            if (func_0011aa70_fnd(&list[idx], side, task, 1) < 0) {
                func_0011c3e0_fnd(&list[idx], 1);
                return 1;
            }
            done = 1;
        }
        break;
    case 10:
        if (func_00119300_fnd(list[idx].wUnit, side, task) < 0) {
            func_0011c3e0_fnd(&list[idx], 1);
            task->bPurpose = 255;
            task->bOpen = 0;
            break;
        }
        func_0011c3e0_fnd(&list[idx], 1);
        done = 1;
        break;
    case 11:
        func_0011b0e0_fnd(&list[idx], side, task, 1);
        done = 1;
        break;
    case 6:
    case 15:
    case 16:
        CMove_void(&list[idx], side, task, 1, 0, &done);
        break;
    case 9:
        CEnc_void(&list[idx], side, task, 1, &done);
        break;
    case 14:
        func_00276ec0_fnd(&list[idx], side, task);
        done = 1;
        break;
    default:
        done = 0;
        break;
    }
    if (done == 1) {
        return 1;
    }
    done = 0;
    switch (list[idx].wPurpose & 0xff) {
    case 5:
        done = CRun_val(&list[idx], side, task, 0, done);
        break;
    case 2:
        if (func_001e2100_fnd(list[idx].wUnit >> 8, list[idx].wUnit & 0xff, 0) == 0) {
            if (func_0011aa70_fnd(&list[idx], side, task, 0) < 0) {
                func_0011a4d0_fnd(&list[idx], side, task, 0);
            }
            done = 1;
        }
        break;
    case 10:
        if (func_00119300_fnd(list[idx].wUnit, side, task) < 0) {
            func_0011c3e0_fnd(&list[idx], 0);
            task->bPurpose = 255;
            done = 1;
            task->bOpen = 0;
            break;
        }
        func_0027b1b0_fnd(&list[idx], 0);
        done = 1;
        break;
    case 11:
        func_0011b0e0_fnd(&list[idx], side, task, 0);
        done = 1;
        break;
    case 6:
    case 15:
    case 16:
        CMove_void(&list[idx], side, task, 0, 0, &done);
        break;
    case 9:
        done = CEnc_val(&list[idx], side, task, 0, done);
        break;
    case 14:
        func_00276ec0_fnd(&list[idx], side, task);
        done = 1;
        break;
    default:
        return 0;
    }
    if (done == 1) {
        return 1;
    }
    return 0;
}
