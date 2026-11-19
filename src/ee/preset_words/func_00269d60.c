#include "dotr/ee/preset_words.h"

int Ta_Eff_036_LoadEffect(struct DotrPresetKindView *obj, struct DotrPresetObject *target)
{
    switch (obj->unknown_0c >> 14) {
    case 536:
        return TaEffect_LoadModule(target, 536);
    case 540:
        return TaEffect_LoadModule(target, 540);
    case 541:
        return TaEffect_LoadModule(target, 541);
    case 542:
        return TaEffect_LoadModule(target, 542);
    default:
        return TaEffect_LoadModule(target, 536);
    }
}
