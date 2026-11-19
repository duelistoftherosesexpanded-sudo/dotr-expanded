#include "dotr/ee/object_table.h"

int SzModel_ReqMotionBlur(int modelId, int wFrame, int mode, struct DotrObjectQuad *a, struct DotrObjectQuad *b, int flag)
{
    struct DotrObject *pModel = gSzModel_ahModel[modelId];
    struct DotrObjectCopy112 *e;
    int i;
    int j;

    if (pModel == 0) {
        return 0;
    }
    if (pModel->type != 4 && pModel->type != 5) {
        return 0;
    }
    for (i = 1, e = &data_0031ad00_copy[1]; i < 32; i++, e++) {
        if (e->index < 0) {
            for (j = 0; j < 2; j++) {
                e->unknown_40[j] = pModel->unknown_198[j];
                e->unknown_58[j] = pModel->unknown_1e0[j];
            }
            RhMem_CopyMemory16(e, pModel, 4);
            if (a != 0) {
                e->unknown_5a = *a;
            } else {
                e->unknown_5a = pModel->rgba;
                e->unknown_5e.unknown_3 = 0x80;
            }
            if (b != 0) {
                e->unknown_5e = *b;
            } else {
                e->unknown_5e = pModel->rgba;
                e->unknown_5e.unknown_3 = 0;
            }
            e->mode = mode >= 0 ? mode : (mode = pModel->alphaMode.unknown_bits_0_1);
            e->unknown_62 = wFrame;
            e->unknown_64 = 0;
            e->flag = (signed char)flag;
            e->index = modelId;
            return 1;
        }
    }
    return 0;
}
