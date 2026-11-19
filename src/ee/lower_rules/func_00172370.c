#include "dotr/ee/lower_rules.h"

int L022(int side, struct DotrRuleSet *set)
{
    struct DotrRuleStep step;
    int i;
    int id;
    unsigned short inputCount;

    step.position = 0xffff;
    step.kind = 0xff;
    step.loc = 0xffff;
    step.shouldFlipUp = 0;
    step.atkDefPosition = 0xff;
    set->inputCount++;
    inputCount = set->inputCount;
    if (inputCount == 600) {
        return 0x20;
    }
    if (inputCount == 601) {
        return 0x20;
    }
    if (inputCount == 602) {
        return 0x20;
    }
    if (inputCount >= 603) {
        return 0x800;
    }
    if (set->mode == 0) {
        func_00275e80_set(set);
        if (TaLogic_GetMinimumTask__0016d140(set, &step, side) == 0) {
            return 0x800;
        }
        if (TaLogic_GetInputQueFromMinimumTask(set, &step, side) < 0) {
            return 0;
        }
    }
    id = set->ids[set->mode - 1];
    if (id == 0xffff) {
        return 0;
    }
    if (set->ids[(unsigned char)set->mode] == 0xffff) {
        set->mode = 0;
        for (i = 0; i < 32; i++) {
            set->ids[i] = 0xffff;
        }
    } else {
        set->mode++;
    }
    return id & 0xffff;
}
