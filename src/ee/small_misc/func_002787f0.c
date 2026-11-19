#include "dotr/ee/small_misc.h"

/* Returns the halfword of whichever item's kind-27 entry names the other
 * item's low five bits while that item's upper three bits are clear. Two
 * `switch` statements reproduce the branch layout. */
int TaLogic_GetActOfs(struct DotrMiscPairItem *unit1, struct DotrMiscPairItem *unit2)
{
    struct DotrMiscPairEntry *eb = &unit2->cardEffects;
    struct DotrMiscPairEntry *ea;
    switch (eb->type >> 8) {
    case 27: /* 27: bonus against a monster kind */
        if ((eb->type & 0x3f) == (unit1->kind & 0x1f) && (unit1->kind >> 5) == 0) {
            return eb->data;
        }
        break;
    }
    ea = &unit1->cardEffects;
    switch (ea->type >> 8) {
    case 27:
        if ((ea->type & 0x3f) == (unit2->kind & 0x1f) && (unit2->kind >> 5) == 0) {
            return ea->data;
        }
        break;
    }
    return 0;
}
