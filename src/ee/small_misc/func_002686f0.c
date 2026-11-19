#include "dotr/ee/small_misc.h"

/* Maps limb 0-3 to card 56 (right arm of the Forbidden One), 57 (left arm), 54 (right leg)
 * and 55 (left leg); anything else to 0. */
int Ta_GetExzodiaPartsCardNo(int limbId)
{
    switch (limbId) {
    case 0:
        return 56;
    case 1:
        return 57;
    case 2:
        return 54;
    case 3:
        return 55;
    }
    return 0;
}
