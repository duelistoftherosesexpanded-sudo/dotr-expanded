#include "dotr/ee/map_edit.h"

#define MAX0(v) ((v) > 0 ? (v) : 0)
#define CLAMP(v) (MAX0(v) < 0x8000 ? MAX0(v) : 0x8000)

void SzDuel_ActMapEdit(void)
{
    struct DotrControlDrift *control = &data_00317490_drift;
    struct DotrControlDrift *first = &data_00317490_drift;
    struct DotrControlDrift *second;
    int sum;
    int step;
    struct DotrMapEditGrids *data;
    int x;
    int y;
    unsigned char field;
    unsigned short pad;

    if (data_00317490_drift.restart) {
        first->step_c86 = 1;
    }
    sum = first->unknown_c86 + (first->step_c86 << 9);
    first->unknown_c86 = CLAMP(sum);
    if (first->unknown_c86 == 0 || first->unknown_c86 >= 0x8000) {
        first->step_c86 = -first->step_c86;
    }
    step = data_00317490_drift.step_c88 << 11;
    second = &data_00317490_drift;
    second->unknown_c88 = CLAMP(second->unknown_c88 + step);
    if (second->unknown_c88 == 0 || second->unknown_c88 >= 0x8000) {
        second->step_c88 = -second->step_c88;
    }
    if (DOTR_MAP_EDIT->unknown_91 != 0) {
        DOTR_MAP_EDIT->unknown_91--;
    }
    if (SzPad_Check(0, 0, 0x8c0)) {
        /* The grid index is applied to the entry pointer once, before the loops. */
        data = (struct DotrMapEditGrids *)((unsigned char *)SzData_GetData(0) + (DOTR_MAP_EDIT->unknown_8c + 2) % DOTR_MAP_EDIT->unknown_8d * 49);
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                data->grids[0][y][x] = SzDuel_GetField(x, y);
            }
        }
        SzSound_PlaySE(0, 4);
        SzGame_Pop();
        return;
    }
    if (SzPad_Check(0, 0, 0x20)) {
        SzSound_PlaySE(0, 5);
        SzGame_Pop();
        return;
    }
    if (SzPad_Check(0, 2, 0xc)) {
        field = SzDuel_GetField(DOTR_MAP_EDIT->unknown_8f, DOTR_MAP_EDIT->unknown_90);
        if (SzPad_Check(0, 2, 4)) {
            field = (field + 9) % 10;
        }
        if (SzPad_Check(0, 2, 8)) {
            field = (field + 11) % 10;
        }
        SzSound_PlaySE(0, 3);
        SzDuel_SetField(DOTR_MAP_EDIT->unknown_8f, DOTR_MAP_EDIT->unknown_90, field);
    }
    if (DOTR_MAP_EDIT->unknown_91 != 0) {
        return;
    }
    pad = SzPad_Check(0, 1, 0xffff);
    if ((pad & 0xf000) != 0) {
    if (((pad & 0x8000) && DOTR_MAP_EDIT->unknown_8f <= 0) || ((pad & 0x2000) && DOTR_MAP_EDIT->unknown_8f >= 6)) {
        pad &= ~0xa000;
    }
    if (((pad & 0x1000) && DOTR_MAP_EDIT->unknown_90 <= 0) || ((pad & 0x4000) && DOTR_MAP_EDIT->unknown_90 >= 6)) {
        pad &= ~0x5000;
    }
    control->restart = 60;
    if (DOTR_MAP_EDIT->unknown_92_bit_0) {
        if (pad & 0x5000) {
            pad &= 0x5000;
        } else {
            DOTR_MAP_EDIT->unknown_92_bit_0 = 0;
        }
    }
    if (pad & 0xa000) {
        if (pad & 0x8000) {
            DOTR_MAP_EDIT->unknown_8f--;
        } else {
            DOTR_MAP_EDIT->unknown_8f++;
        }
        if (pad & 0x5000) {
            DOTR_MAP_EDIT->unknown_92_bit_0 = 1;
        }
    } else if (pad & 0x5000) {
        if (pad & 0x1000) {
            DOTR_MAP_EDIT->unknown_90--;
        } else {
            DOTR_MAP_EDIT->unknown_90++;
        }
        DOTR_MAP_EDIT->unknown_92_bit_0 = 0;
    } else {
        DOTR_MAP_EDIT->unknown_92_bit_0 = 0;
        return;
    }
    SzSound_PlaySE(0, 3);
    DOTR_MAP_EDIT->unknown_91 = 8;
        /* MATCHING DEVICE: the explicit return closing this conditional block makes the
         * compiler fill branch delay slots throughout the function with useful instructions
         * instead of nops, and drop the originals of instructions it copies into them. */
        return;
    }
}
