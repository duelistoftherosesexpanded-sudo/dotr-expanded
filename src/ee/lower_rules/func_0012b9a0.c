#include "dotr/ee/lower_rules.h"

int L006(int side, struct DotrRuleSet *set)
{
    struct DotrRuleStep step;
    int i;
    int id;
    unsigned short count;

    step.position = 0xffff;
    step.kind = 0xff;
    step.loc = 0xffff;
    step.shouldFlipUp = 0;
    step.atkDefPosition = 0xff;
    if (set == 0) {
        return 0x800;
    }
    for (i = 0; i < 5; i++) {
        step.slots[i] = 0xff;
    }
    set->inputCount++;
    count = set->inputCount;
    if (count == 600) {
        return 0x20;
    }
    if (count == 601) {
        return 0x20;
    }
    if (count == 602) {
        return 0x20;
    }
    if (count >= 603) {
        return 0x800;
    }
    if (set->mode == 0) {
        func_00275e80_set(set);
        if (TaLogic_GetMinimumTask__00126c80(set, &step, side) == 0) {
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
