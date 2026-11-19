#include "dotr/ee/effect_module.h"

struct DotrPoolEntry8 *SzEffect_LoadModule(int type, int id, int c)
{
    char name[128];
    struct DotrPoolEntry8 *tbl;
    struct DotrPoolEntry8 *e;
    int i;
    int k;
    unsigned char pos;
    int n;
    int idx;
    void *address;
    char letter;

    /* A slot already holding this module: ready (2) or kept after its last use (3). */
    tbl = gSzEffect_el_pool.unknown_000;
    e = gSzEffect_el_pool.unknown_000;
    for (i = 0; i < 32L; i++, e++) {
        if (e->unknown_bits_3_5 == type && e->unknown_bits_6_15 == id) {
            if (e->unknown_bits_0_2 == 2) {
                return e;
            }
            if (e->unknown_bits_0_2 == 3) {
                e->unknown_bits_0_2 = 2;
                return e;
            }
        }
    }
    /* Otherwise the first empty slot, or failing that the first kept slot from the rotating start. */
    e = tbl;
    for (i = 0; i < 32L; i++, e++) {
        if (e->unknown_bits_0_2 == 0) {
            break;
        }
    }
    if (i >= 32L) {
        pos = data_0031bea8;
        for (k = 0; k < 32L; k++) {
            idx = (pos + k) % 32L;
            e = &tbl[idx];
            if (e->unknown_bits_0_2 == 3) {
                i = idx;
                break;
            }
        }
        data_0031bea8 = (pos + 1) % 32L;
    }
    if (i < 32L) {
        address = (void *)(i * 0x8000L + 0x1ee0000);
        letter = '?';
        if (e->unknown_04 != 0) {
            SzMem_Free(e->unknown_04);
            e->unknown_04 = 0;
        }
        switch (type) {
        case 0:
            letter = 'C';
            break;
        case 1:
            letter = 'B';
            break;
        case 2:
            letter = 'M';
            break;
        }
        if (c != 0) {
            return 0;
        }
        if (id < 0 || id >= 854) {
            return 0;
        }
        func_0019b648(name, data_002cfbb0, SzFile_GetEnv(data_002cfbc6), letter);
        if ((unsigned int)type < 3) {
            n = id << 15;
            if (type == 1 || type == 2) {
                idx = SzDuel_Xax2New(id);
                if (idx >= 683) {
                    return 0;
                }
                n = idx << 15;
            }
            if (func_001fecb0_module(0x800, 0, name, address, 0x8000, n, 0x8000, lSzEffect_LoadOverlayCB, i) == 0) {
                return 0;
            }
            e->unknown_bits_3_5 = type;
            e->unknown_bits_6_15 = id;
            e->unknown_bits_0_2 = 1;
        } else {
            return 0;
        }
        return e;
    }
    return 0;
}
