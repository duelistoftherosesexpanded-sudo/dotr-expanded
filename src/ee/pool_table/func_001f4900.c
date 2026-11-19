#include "dotr/ee/pool_table.h"

int SzEffect_EntryCallback(int moduleType, int id, int arg, int target, int from, void (*callback)(int), int callback_arg)
{
    struct DotrPoolEntry8 *e;
    struct DotrPoolEntry8 *base;
    struct DotrPoolEntry24 *q;
    DotrPoolAddr module;
    int result;
    int i;
    struct DotrPool *pool;

    base = gSzEffect_el_pool.unknown_000;
    q = gSzEffect_el_pool.unknown_100;
    module = 0;
    if (moduleType < 0 || moduleType >= 3) {
        return -1;
    }
    if (id < 0 || id >= gSzEffect_aEffFuncTbl[moduleType].unknown_count) {
        return -1;
    }
    if (moduleType == 1) {
        if (SzEffect_IsUse2(1, -1)) {
            return -1;
        }
    }
    switch (moduleType) {
    case 1:
        module = SzModel_GetOverlayB(SzDuel_GetUnitModelID(from >> 8, from & 0xff));
        break;
    case 2:
        module = SzModel_GetOverlayM(SzDuel_GetUnitModelID(from >> 8, from & 0xff));
        break;
    }
    if (module == 0) {
        e = SzEffect_LoadModule(moduleType, id, 1);
        if (e == 0) {
            return -1;
        }
        /* The 64-bit shift and add are kept by the conversion through a pointer. */
        module = (DotrPoolAddr)(void *)(((long)(e - base) << 15) + 0x1ee0000);
    }
    for (i = 0; i < 24; i++, q++) {
        DotrPoolAddr address = 0x1ed0000;
        if (q->unknown_00_bits_0_2 != 0) {
            continue;
        }
        pool = &gSzEffect_el_pool;
        if (pool->unknown_340 != moduleType || pool->unknown_344 != id) {
            RhMem_CopyMemory16(DOTR_POOL_OVERLAY, (void *)module, 2048);
            func_0017bb90(0);
            mwOverlayInit(DOTR_POOL_OVERLAY, DOTR_POOL_OVERLAY[7] - 0x1e00000);
            pool->unknown_340 = moduleType;
            pool->unknown_344 = id;
        }
        SzEffect_SetTarget(target);
        SzEffect_SetFrom(from);
        RhGeom_PushMatrix(0);
        if (moduleType != 1) {
            address = (DotrPoolAddr)(void *)(((long)i << 15) + 0x1e10000);
        }
        result = DOTR_POOL_EFFECT_FUNCTION(gSzEffect_aEffFuncTbl[moduleType].unknown_list[id])(address, arg);
        RhGeom_PopMatrix();
        q->unknown_00_bits_0_2 = 1;
        q->unknown_02_bits_0_2 = moduleType;
        q->unknown_02_bits_3_12 = id;
        q->unknown_14 = arg;
        q->unknown_04 = target;
        q->unknown_06 = from;
        q->unknown_02_bit_13 = 0;
        q->unknown_02_bit_14 = 0;
        q->unknown_02_bit_15 = 0;
        q->unknown_00_bits_3_15 = 0;
        q->unknown_10 = 0;
        q->unknown_08 = callback;
        q->unknown_0c = callback_arg;
        if (result == 8) {
            SzEffect_Destroy(i);
            i = -1;
        }
        return i;
    }
    return -1;
}
