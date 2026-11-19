#include "dotr/ee/pool_table.h"

/* The body of 0x1f3ea0, which 0x1f40d0 contains inline (as 0x1f4cb0 does); the original
 * helper form is unknown. */
static inline struct DotrPoolEntry8 *dotr_pool_find_small(int a, int b)
{
    struct DotrPoolEntry8 *e = gSzEffect_el_pool.unknown_000;
    int i;
    for (i = 0; i < 32L; i++, e++) {
        if (a >= 0 && e->unknown_bits_3_5 != a) {
            continue;
        }
        if (b < 0 || e->unknown_bits_6_15 == b) {
            return e;
        }
    }
    return 0;
}

void SzEffect_Draw(int type)
{
    struct DotrPoolEntry8 *base;
    struct DotrPoolEntry24 *q;
    int i;
    int dir;
    long address;
    struct DotrPool *pool;

    base = gSzEffect_el_pool.unknown_000;
    q = gSzEffect_el_pool.unknown_100;
    SzGame_GetMode(1);
    RhGeom_PushMatrix(0);
    for (i = 0, address = 0x1e10000; i < 24; i++, q++, address += 0x8000) {
        DotrPoolAddr module;
        DotrPoolAddr work;
        int arg;
        int result;
        int kind;
        int id;

        if (type >= 0 && q->unknown_02_bits_0_2 != type) {
            continue;
        }
        arg = -1;
        work = 0x1ed0000;
        module = 0;
        if (q->unknown_00_bits_0_2 != 1) {
            continue;
        }
        if (q->unknown_02_bit_13) {
            arg = -2;
        }
        if (q->unknown_02_bit_14) {
            arg = -3;
        }
        switch (type) {
        case 1:
            module = SzModel_GetOverlayB(SzDuel_GetUnitModelID(q->unknown_06 >> 8, q->unknown_06 & 0xff));
            break;
        case 2:
            module = SzModel_GetOverlayM(SzDuel_GetUnitModelID(q->unknown_06 >> 8, q->unknown_06 & 0xff));
            break;
        }
        if (module == 0) {
            struct DotrPoolEntry8 *e = dotr_pool_find_small(q->unknown_02_bits_0_2, q->unknown_02_bits_3_12);
            if (e == 0) {
                continue;
            }
            if (e->unknown_bits_0_2 != 2) {
                continue;
            }
            module = (DotrPoolAddr)(void *)(((long)(e - base) << 15) + 0x1ee0000);
        }
        pool = &gSzEffect_el_pool;
        id = q->unknown_02_bits_3_12;
        kind = q->unknown_02_bits_0_2;
        if (pool->unknown_340 != kind || pool->unknown_344 != id) {
            RhMem_CopyMemory16(DOTR_POOL_OVERLAY, (void *)module, 2048);
            func_0017bb90(0);
            mwOverlayInit(DOTR_POOL_OVERLAY, DOTR_POOL_OVERLAY[7] - 0x1e00000);
            pool->unknown_340 = kind;
            pool->unknown_344 = id;
        }
        SzEffect_SetTarget(q->unknown_04);
        SzEffect_SetFrom(q->unknown_06);
        if (q->unknown_02_bits_0_2 != 1) {
            work = (DotrPoolAddr)(void *)address;
        }
        q->unknown_10 = result = DOTR_POOL_EFFECT_FUNCTION(gSzEffect_aEffFuncTbl[q->unknown_02_bits_0_2].unknown_list[q->unknown_02_bits_3_12])(work, arg);
        q->unknown_00_bits_3_15++;
        if (q->unknown_02_bits_0_2 == 1) {
            int motion;
            int x;
            int y;
            int model;
            int anim;
            int param;
            float frame;
            x = q->unknown_04 >> 8;
            y = q->unknown_04 & 0xff;
            /* MATCHING DEVICE: an empty two-armed test (always true: x comes from an
             * unsigned halfword) ends the block here, so that both values are computed
             * before the next call and the status fix-up below keeps its target shape. */
            if (x >= 0) {
            } else {
            }
            motion = SzEffect_GetPlayMotion(0);
            model = SzDuel_GetUnitModelID(x, y);
            anim = SzModel_GetAnim(model);
            param = SzModel_GetAnimParam(model, 1);
            dir = SzModel_GetAnimDir(model);
            frame = SzModel_GetAnimFrame(model);

            if (result == 1) {
                if (motion == 6) {
                    result = 6;
                } else {
                    result = 0;
                }
            }
            if (result == 3 || result == 4 || result == 8) {
                if (!q->unknown_02_bit_15) {
                    float start;
                    if (motion == anim) {
                        start = SzModel_GetAnimFrame(model);
                    } else {
                        start = 0.0f;
                    }
                    work = SzModel_GetAnimFrameMax(model, motion) - func_001a22c0(start);
                    SzView_FreeRequest(0, 1);
                    if (work < 30) {
                        work = 30;
                    }
                    SzView_ReqCamera(0, 2, 0, work, 800, 0);
                    SzView_ReqCamera(0, 2, 1, work, 0, 0);
                    SzView_ReqCamera(0, 2, 6, work, model, 0);
                    q->unknown_02_bit_15 = -1;
                }
                if (result == 4) {
                    result = 0;
                }
            }
            if (motion == 4 && result == 5) {
                SzModel_SetStoneFlag(model, 1);
            }
            if (result == 2 || result == 3 || result == 5 || result == 6) {
                if (anim == 3 || anim == 4 || anim == 6) {
                    if (motion == 6) {
                        SzModel_SetMotionDelay(model, 0);
                    }
                    if (result == 6 && dir == 1 && !(frame < 11.0f)) {
                        int hit = 0;
                        if (!(frame < 16.0f)) {
                            hit = 1;
                        } else if (func_001a22c0(3.0f * ((float)func_0019a5c8() / 2147483648.0f))) {
                            hit = 1;
                        }
                        if (hit) {
                            SzModel_SetAnimFrame(model, frame - 2.0f);
                        }
                    }
                } else {
                    SzModel_SetAnimSpeed(model, 1.0f);
                    if (param >= 0) {
                        SzModel_SetAnimIntrReverse(model);
                    } else {
                        if (motion == 3) {
                            motion = 4;
                        }
                        SzModel_SetAnimIntr(model, motion);
                    }
                    SzModel_SetAnimSpeed(model, 1.0f);
                }
            } else if (result == 7) {
                if (anim == 3 || anim == 4) {
                    SzModel_SetAnimSpeed(model, 0.0f);
                    if (param >= 0) {
                        SzModel_SetAnimIntrReverse(model);
                    } else {
                        SzModel_SetAnimIntrParam(model, 0, 16);
                    }
                    SzModel_SetAnimSpeed(model, 1.0f);
                }
            } else if (result == 0) {
                if (motion == 3 && anim == 4 && !(frame < 16.0f)) {
                    SzModel_SetAnimSpeed(model, 0.0f);
                    if (param >= 0) {
                        SzModel_SetAnimIntrReverse(model);
                    } else {
                        SzModel_SetAnimIntrParam(model, 0, 16);
                    }
                    SzModel_SetAnimSpeed(model, 1.0f);
                }
            } else if (result == 8) {
                if (motion == 3 && anim == 4) {
                    SzModel_SetAnimSpeed(model, 0.0f);
                    if (param >= 0) {
                        SzModel_SetAnimIntrReverse(model);
                    } else {
                        SzModel_SetAnimIntrParam(model, 0, 16);
                    }
                    SzModel_SetAnimSpeed(model, 1.0f);
                }
                SzEffect_Destroy(i);
            }
        } else if (arg == -2) {
            if (result == 8) {
                q->unknown_00_bits_0_2 = 2;
            }
        } else if (result == 8) {
            SzEffect_Destroy(i);
        }
    }
    RhGeom_PopMatrix();
}
