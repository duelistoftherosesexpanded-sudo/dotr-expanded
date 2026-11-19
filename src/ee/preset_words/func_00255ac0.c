#include "dotr/ee/preset_words.h"

int TaEffect_LoadModule(struct DotrPresetObject *obj, int moduleId)
{
    if (func_001f3910_result(0, moduleId, 0)) {
        obj->unknown_82[obj->unknown_80.unknown_bits_1_5] = moduleId;
        obj->unknown_80.unknown_bits_1_5++;
        return 1;
    }
    return -1;
}
