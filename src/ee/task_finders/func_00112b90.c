#include "dotr/ee/task_finders.h"

static inline int CRun_val(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int done)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_00115850_fnd(unit, side, task, first);
        done = 1;
    }
    return done;
}

static inline int CMove_val(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, int done)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_001162f0_fnd(unit, side, task, first, e);
        done = 1;
    }
    return done;
}

static inline int CEnc_val(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int done)
{
    func_0027d020_fnd(unit, side, task, first, 0, func_00117c30_fnd(side), 5, 1);
    done = 1;
    return done;
}

static inline void CRun_void(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int *pd)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_00115850_fnd(unit, side, task, first);
        *pd = 1;
    }
}

static inline void CMove_void(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int e, int *pd)
{
    if (func_001e2100_fnd(unit->wUnit >> 8, unit->wUnit & 0xff, 0) == 0) {
        func_001162f0_fnd(unit, side, task, first, e);
        *pd = 1;
    }
}

static inline void CEnc_void(struct DotrFinderUnit *unit, int side, struct DotrFinderTask *task, int first, int *pd)
{
    func_0027d020_fnd(unit, side, task, first, 0, func_00117c30_fnd(side), 5, 1);
    *pd = 1;
}

int TaLogic_GetMinimumTask__00112b90(struct DotrFinderSet *list, struct DotrFinderTask *task, int owner)
{
    int side = func_001de540_fnd();
    int done = 0;
    int r;
    int idx;

    (void)owner;
    list->m = func_00117340_fnd();
    if ((r = func_00113370_fnd(list, side, list->m, task)) < 0) {
        return 0;
    }
    idx = func_00276c80_fnd2(list, r & 0xffff);
    if (idx < 0) {
        return 0;
    }
    if ((r >> 16) == 1) {
        if (((list->unit[idx].wPurpose >> 8) & 0xff) == 10) {
            func_0027b1b0_fnd(&list->unit[idx], 1);
        } else {
            func_0027b1b0_fnd(&list->unit[idx], 0);
        }
        return 1;
    }
    func_00113140_fnd(&list->unit[idx], list->m);
    switch ((list->unit[idx].wPurpose >> 8) & 0xff) {
    case 5:
        CRun_void(&list->unit[idx], side, task, 1, &done);
        break;
    case 2:
        if (func_001e2100_fnd(list->unit[idx].wUnit >> 8, list->unit[idx].wUnit & 0xff, 0) == 0) {
            if (func_00115e20_fnd(&list->unit[idx], side, task, 1) < 0) {
                func_0027b1b0_fnd(&list->unit[idx], 1);
                return 1;
            }
            done = 1;
        }
        break;
    case 10:
        if (func_00114080_fnd(list->unit[idx].wUnit, side, task) < 0) {
            func_0027b1b0_fnd(&list->unit[idx], 1);
            task->bPurpose = 255;
            task->bOpen = 0;
            break;
        }
        func_0027b1b0_fnd(&list->unit[idx], 1);
        done = 1;
        break;
    case 6:
        CMove_void(&list->unit[idx], side, task, 1, 0, &done);
        break;
    case 3:
        CMove_void(&list->unit[idx], side, task, 1, 1, &done);
        break;
    case 9:
        CEnc_void(&list->unit[idx], side, task, 1, &done);
        break;
    case 11:
        func_00116b60_fnd(&list->unit[idx], side, task, 1);
        done = 1;
        break;
    case 12:
        func_001170a0_fnd(&list->unit[idx], side, task, 1);
        done = 1;
        break;
    case 14:
        func_00276ec0_fnd(&list->unit[idx], side, task);
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
    switch (list->unit[idx].wPurpose & 0xff) {
    case 5:
        done = CRun_val(&list->unit[idx], side, task, 0, done);
        break;
    case 2:
        if (func_001e2100_fnd(list->unit[idx].wUnit >> 8, list->unit[idx].wUnit & 0xff, 0) == 0) {
            if (func_00115e20_fnd(&list->unit[idx], side, task, 0) < 0) {
                func_00115850_fnd(&list->unit[idx], side, task, 0);
            }
            done = 1;
        }
        break;
    case 10:
        if (func_00114080_fnd(list->unit[idx].wUnit, side, task) < 0) {
            func_0027b1b0_fnd(&list->unit[idx], 0);
            task->bPurpose = 255;
            done = 1;
            task->bOpen = 0;
            break;
        }
        func_0027b1b0_fnd(&list->unit[idx], 0);
        done = 1;
        break;
    case 6:
        done = CMove_val(&list->unit[idx], side, task, 0, 0, done);
        break;
    case 3:
        done = CMove_val(&list->unit[idx], side, task, 0, 1, done);
        break;
    case 9:
        done = 1;
        func_0027d020_fnd(&list->unit[idx], side, task, 0, 0, func_00117c30_fnd(side), 5, 1);
        break;
    case 11:
        done = 1;
        func_00116b60_fnd(&list->unit[idx], side, task, 0);
        break;
    case 12:
        done = 1;
        func_001170a0_fnd(&list->unit[idx], side, task, 0);
        break;
    case 14:
        func_00276ec0_fnd(&list->unit[idx], side, task);
        break;
    default:
        return 0;
    }
    if (done == 1) {
        return 1;
    }
    return 0;
}
