#include "dotr/ee/object_table.h"

int SzModel_Create(int modelId)
{
    struct DotrObject *model;

    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    if ((model = SzMem_Alloc(sizeof(struct DotrObject))) == 0) {
        return 0;
    }
    gSzModel_ahModel[modelId] = model;
    RhMem_FillMemory4((int *)model, 0, sizeof(struct DotrObject) / 4);
    model->unknown_1e0[1] = -1;
    model->animDir.unknown_bits_6_7 = 1;
    model->modelId = -1;
    model->modelNo = -1;
    *(int *)&model->unknown_1e8[4] = -1;
    model->scale.x = 1.0f;
    model->scale.y = 1.0f;
    model->scale.z = 1.0f;
    model->scale.unknown_w = 1.0f;
    model->intensity.x = 0.8f;
    model->intensity.y = 0.8f;
    model->intensity.z = 1.0f;
    model->rgba.unknown_2 = 0x80;
    model->rgba.unknown_1 = 0x80;
    model->rgba.unknown_0 = 0x80;
    model->rgba.unknown_3 = 0x80;
    model->alphaMode.unknown_bits_2_3 = 1;
    model->filters.unknown_2 = 0;
    model->filters.unknown_1 = 0;
    model->filters.unknown_0 = 0;
    model->filters.unknown_3 = 0;
    model->drawFlag.unknown_bit_0 = -1;
    model->drawFlag.unknown_bit_1 = -1;
    model->drawFlag.unknown_bit_2 = 0;
    model->drawFlag.unknown_bit_3 = 0;
    model->drawFlag.unknown_bit_4 = 0;
    model->drawFlag.unknown_bit_5 = 0;
    model->drawFlag.unknown_bit_6 = -1;
    model->drawFlag.unknown_bit_7 = -1;
    lSzModel_Clean(modelId);
    return 1;
}
