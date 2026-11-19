#include "dotr/ee/camp_leader_model.h"

/* MATCHING DEVICE (each of these is needed for the exact bytes; none is a claim about the
 * original source):
 * - the board is read through `signed char *w` with the indexes 199 and 200, not through the
 *   structure's members;
 * - the locals are declared in the order rot, scale, p, q, out, and rot and scale have four
 *   members; `x0` and `z0` are declared in that order after `idx` and assigned after the first
 *   0x1a6c18 call of the reverse branch;
 * - `a = 0; b = 5; fwd = 0; rev = 0;` first, the height byte into the int `lv`, the three scale
 *   stores, then `h = (float)lv` and `from` as a `signed char` local that is reused as an index;
 * - the statement order inside the cases: `b, a, fwd` for from 1; `rev, b, fwd` for the reverse
 *   pairs except 3>2, which is `b, fwd, rev`; the inner cases in ascending order;
 * - the comparison operand orders `0 < show`, `t <= 1024`, `0 <= idx`, `n <= idx`, and the empty
 *   `else if (n - 1 <= idx) { }` before the blend;
 * - `(short)` on the state in the reverse branch's first test;
 * - some location records are indexed with the local `from` and others with the board byte;
 * - `angle + 32767 + 1`; the stored angle cleared between SzModel_SetAlpha and
 *   SzModel_SetDrawFlag; the view point copied by a structure assignment. */
int Taku_Camp_3D_LeaderModel(int cursor)
{
    struct DotrLeaderModelRot rot;
    struct DotrLeaderModelScale scale;
    struct DotrLeaderModelVec p;
    struct DotrLeaderModelVec q;
    struct DotrLeaderModelVec out;
    signed char *w = (signed char *)&data_00299e10_leadermodel;
    short a, b, fwd, rev;
    short idx;
    float *x0;
    float *z0;
    float h;
    float dx2, dz2, dx, dz;
    float px, pz;
    signed char from;
    int lv;

    a = 0;
    b = 5;
    fwd = 0;
    rev = 0;
    lv = data_00299a30_leadermodel[SzData_GetLeader(0, (short)SzData_GetCurrentDeck(0))];
    scale.x = 0.5f;
    scale.y = 0.5f;
    scale.z = 0.5f;
    h = (float)lv;
    from = DOTR_LEADER_FROM;
    switch (from) {
    case 1:
        switch (DOTR_LEADER_TO) {
        case 2:
            b = 1;
            a = 0;
            fwd = 1;
            break;
        case 8:
            b = 2;
            a = 0;
            fwd = 1;
            break;
        }
        break;
    case 2:
        switch (DOTR_LEADER_TO) {
        case 3:
            b = 3;
            fwd = 1;
            break;
        case 5:
            b = 4;
            fwd = 1;
            break;
        case 7:
            b = 12;
            fwd = 1;
            break;
        }
        break;
    case 3:
        switch (DOTR_LEADER_TO) {
        case 2:
            b = 3;
            fwd = 1;
            rev = 1;
            break;
        }
        break;
    case 5:
        switch (DOTR_LEADER_TO) {
        case 2:
            rev = 1;
            b = 4;
            fwd = 1;
            break;
        case 6:
            rev = 1;
            b = 10;
            fwd = 1;
            break;
        case 7:
            b = 13;
            fwd = 1;
            break;
        }
        break;
    case 6:
        switch (DOTR_LEADER_TO) {
        case 5:
            b = 10;
            fwd = 1;
            break;
        }
        break;
    case 7:
        switch (DOTR_LEADER_TO) {
        case 11:
            b = 14;
            fwd = 1;
            break;
        }
        break;
    case 8:
        switch (DOTR_LEADER_TO) {
        case 6:
            b = 5;
            fwd = 1;
            break;
        case 7:
            b = 11;
            fwd = 1;
            break;
        }
        break;
    case 9:
        switch (DOTR_LEADER_TO) {
        case 8:
            rev = 1;
            b = 6;
            fwd = 1;
            break;
        }
        break;
    case 11:
        switch (DOTR_LEADER_TO) {
        case 7:
            rev = 1;
            b = 14;
            fwd = 1;
            break;
        }
        break;
    }
    if (rev == 0 && DOTR_LEADER_WORK->state != 0 && fwd && 0 < DOTR_LEADER_WORK->show && DOTR_LEADER_WORK->t <= 1024) {
        idx = DOTR_LEADER_WORK->idx;
        dx2 = DOTR_LEADER_X(2) - DOTR_LEADER_X(1);
        dz2 = DOTR_LEADER_Z(2) - DOTR_LEADER_Z(1);
        func_001a6c18(dx2, dz2);
        dx = DOTR_LEADER_X(1) - DOTR_LEADER_X(0);
        dz = DOTR_LEADER_Z(1) - DOTR_LEADER_Z(0);
        DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(dx, dz);
        if (DOTR_LEADER_WORK->state == 3) {
            DOTR_LEADER_WORK->pos[0] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].x;
            DOTR_LEADER_WORK->pos[1] = h + DOTR_LEADER_SPOT[DOTR_LEADER_TO].y;
            DOTR_LEADER_WORK->pos[2] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].z;
        } else {
            px = DOTR_LEADER_X(0);
            DOTR_LEADER_WORK->pos[0] = px + dx * (float)DOTR_LEADER_WORK->t / 1024.0f;
            DOTR_LEADER_WORK->pos[1] = DOTR_LEADER_SPOT[0].y + h;
            pz = DOTR_LEADER_Z(0);
            DOTR_LEADER_WORK->pos[2] = pz + dz * (float)DOTR_LEADER_WORK->t / 1024.0f;
        }
        if (DOTR_LEADER_N <= idx) {
            DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(DOTR_LEADER_X(0) - DOTR_LEADER_X(-1), DOTR_LEADER_Z(0) - DOTR_LEADER_Z(-1));
        } else if (DOTR_LEADER_N - 1 <= idx) {
        } else {
            short t = DOTR_LEADER_WORK->t;
            DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(dx * (float)(1024 - t) / 1024.0f + dx2 * (float)t / 1024.0f,
                             dz * (float)(1024 - t) / 1024.0f + dz2 * (float)t / 1024.0f);
        }
        if (DOTR_LEADER_WORK->t < 1024 && DOTR_LEADER_WORK->idx < DOTR_LEADER_N) {
            DOTR_LEADER_WORK->t += 256;
            if (DOTR_LEADER_WORK->t >= 1024) {
                DOTR_LEADER_WORK->idx++;
                if (DOTR_LEADER_N <= DOTR_LEADER_WORK->idx) {
                    DOTR_LEADER_WORK->t = 0;
                    DOTR_LEADER_WORK->state = 3;
                    DOTR_LEADER_WORK->idx = DOTR_LEADER_N;
                } else {
                    DOTR_LEADER_WORK->t = 0;
                }
            }
        }
        if (DOTR_LEADER_FROM == DOTR_LEADER_TO) {
            DOTR_LEADER_WORK->t = 0;
            DOTR_LEADER_WORK->state = 3;
            DOTR_LEADER_WORK->idx = DOTR_LEADER_N;
        }
    } else if (rev && DOTR_LEADER_WORK->state != 0 && fwd && 0 < DOTR_LEADER_WORK->show && DOTR_LEADER_WORK->t <= 1024) {
        if ((short)DOTR_LEADER_WORK->state == 1) {
            DOTR_LEADER_WORK->idx = DOTR_LEADER_N - 1;
            DOTR_LEADER_WORK->state = 2;
        }
        idx = DOTR_LEADER_WORK->idx;
        dx2 = DOTR_LEADER_X(1) - DOTR_LEADER_X(2);
        dz2 = DOTR_LEADER_Z(1) - DOTR_LEADER_Z(2);
        func_001a6c18(dx2, dz2);
        x0 = &DOTR_LEADER_X(0);
        z0 = &DOTR_LEADER_Z(0);
        dx = *x0 - DOTR_LEADER_X(1);
        dz = *z0 - DOTR_LEADER_Z(1);
        DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(dx, dz);
        if (DOTR_LEADER_WORK->state == 3) {
            DOTR_LEADER_WORK->pos[0] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].x;
            DOTR_LEADER_WORK->pos[1] = h + DOTR_LEADER_SPOT[DOTR_LEADER_TO].y;
            DOTR_LEADER_WORK->pos[2] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].z;
        } else {
            px = DOTR_LEADER_X(1);
            DOTR_LEADER_WORK->pos[0] = px + dx * (float)DOTR_LEADER_WORK->t / 1024.0f;
            DOTR_LEADER_WORK->pos[1] = DOTR_LEADER_SPOT[0].y + h;
            pz = DOTR_LEADER_Z(1);
            DOTR_LEADER_WORK->pos[2] = pz + dz * (float)DOTR_LEADER_WORK->t / 1024.0f;
        }
        if (DOTR_LEADER_N <= idx) {
            DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(*x0 - DOTR_LEADER_X(-1), *z0 - DOTR_LEADER_Z(-1));
        } else if (DOTR_LEADER_N - 1 <= idx) {
        } else {
            short t = DOTR_LEADER_WORK->t;
            DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(dx2 * (float)(1024 - t) / 1024.0f + dx * (float)t / 1024.0f,
                             dz2 * (float)(1024 - t) / 1024.0f + dz * (float)t / 1024.0f);
        }
        if (DOTR_LEADER_WORK->state < 3 && DOTR_LEADER_WORK->t < 1024 && 0 <= DOTR_LEADER_WORK->idx) {
            DOTR_LEADER_WORK->t += 256;
            if (DOTR_LEADER_WORK->t >= 1024) {
                DOTR_LEADER_WORK->idx--;
                if (DOTR_LEADER_WORK->idx < 0) {
                    DOTR_LEADER_WORK->t = 0;
                    DOTR_LEADER_WORK->state = 3;
                    DOTR_LEADER_WORK->idx = 0;
                } else {
                    DOTR_LEADER_WORK->t = 0;
                }
            }
        }
        if (DOTR_LEADER_FROM == DOTR_LEADER_TO) {
            DOTR_LEADER_WORK->t = 0;
            DOTR_LEADER_WORK->state = 3;
            DOTR_LEADER_WORK->idx = 0;
        }
    } else if (DOTR_LEADER_WORK->state != 0 && 0 < DOTR_LEADER_WORK->show && DOTR_LEADER_WORK->t <= 1024) {
        dz = DOTR_LEADER_SPOT[DOTR_LEADER_TO].x - DOTR_LEADER_SPOT[from].x;
        dx = DOTR_LEADER_SPOT[DOTR_LEADER_TO].z - DOTR_LEADER_SPOT[from].z;
        DOTR_LEADER_WORK->angle = DOTR_LEADER_ANGLE(dz, dx);
        if (DOTR_LEADER_WORK->state == 3) {
            DOTR_LEADER_WORK->pos[0] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].x;
            DOTR_LEADER_WORK->pos[1] = h + DOTR_LEADER_SPOT[DOTR_LEADER_TO].y;
            DOTR_LEADER_WORK->pos[2] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].z;
        } else {
            DOTR_LEADER_WORK->pos[0] = DOTR_LEADER_SPOT[DOTR_LEADER_FROM].x + dz * (float)DOTR_LEADER_WORK->t / 1024.0f;
            DOTR_LEADER_WORK->pos[1] = h + DOTR_LEADER_SPOT[DOTR_LEADER_FROM].y;
            DOTR_LEADER_WORK->pos[2] = DOTR_LEADER_SPOT[DOTR_LEADER_FROM].z + dx * (float)DOTR_LEADER_WORK->t / 1024.0f;
        }
        DOTR_LEADER_WORK->t += 16;
        if (DOTR_LEADER_WORK->t >= 1024) {
            DOTR_LEADER_WORK->t = 0;
            DOTR_LEADER_WORK->state = 3;
            DOTR_LEADER_WORK->idx = 0;
            DOTR_LEADER_WORK->pos[0] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].x;
            DOTR_LEADER_WORK->pos[1] = h + DOTR_LEADER_SPOT[DOTR_LEADER_TO].y;
            DOTR_LEADER_WORK->pos[2] = DOTR_LEADER_SPOT[DOTR_LEADER_TO].z;
        }
        if (DOTR_LEADER_FROM == DOTR_LEADER_TO) {
            DOTR_LEADER_WORK->t = 0;
            DOTR_LEADER_WORK->state = 3;
            DOTR_LEADER_WORK->idx = 0;
        }
    } else {
        DOTR_LEADER_WORK->pos[0] = DOTR_LEADER_SPOT[from].x;
        DOTR_LEADER_WORK->pos[1] = h + DOTR_LEADER_SPOT[DOTR_LEADER_FROM].y;
        DOTR_LEADER_WORK->pos[2] = DOTR_LEADER_SPOT[DOTR_LEADER_FROM].z;
        p.x = (float)(DOTR_LEADER_WORK->mx - 320);
        p.y = (float)(DOTR_LEADER_WORK->my - 224);
        p.z = 1.0f;
        p.w = 1.0f;
        q.x = (float)(DOTR_LEADER_WORK->mx - 320);
        q.y = (float)(DOTR_LEADER_WORK->my - 224);
        q.z = 100.0f;
        q.w = 1.0f;
        func_00205cb0_leadermodel(&p, p.x, p.y);
        q = *(struct DotrLeaderModelVec *)SzView_GetSP(0);
        func_00206430_leadermodel(DOTR_LEADER_WORK->plane, &p, &q, &out);
        DOTR_LEADER_WORK->angle += DOTR_LEADER_ANGLE(out.x - DOTR_LEADER_WORK->pos[0], out.z - DOTR_LEADER_WORK->pos[2]);
    }
    SzModel_SetDeform(30, 1);
    SzModel_SetDeformRatio(30, 32);
    func_001f25d0_leadermodel(30, &scale);
    func_001f22b0_leadermodel(30, DOTR_LEADER_WORK->pos);
    rot.x = 0;
    rot.y = DOTR_LEADER_WORK->angle + 32767 + 1;
    rot.z = 0;
    func_001f24c0_leadermodel(30, &rot);
    SzModel_SetAlpha(30, DOTR_LEADER_WORK->alpha);
    DOTR_LEADER_WORK->angle = 0;
    SzModel_SetDrawFlag(30, 1);
    return cursor;
}
