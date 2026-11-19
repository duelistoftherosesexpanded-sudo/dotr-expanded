#include "dotr/ee/land_agreement.h"

/* MATCHING DEVICE: one result variable, `break` in every case and a single return; the outer
 * cases in the order 9, 7/8, 6, 3, 0, 2, 1, 4, 5 (two jump tables). */
int Do_CheckLandAgreement(int land, struct jp_64b6d *vInfo)
{
    int kind;
    int ret;

    if ((vInfo->bVarKind >> 5) != 0) {
        return 0;
    }
    kind = vInfo->bVarKind & 0x1f;
    switch (land) {
    case 9: /* crush */
        if (kind == 20) {
            ret = 1;
        } else {
            ret = 0;
        }
        break;
    case 7: /* normal */
    case 8: /* labyrinth */
        ret = 0;
        break;
    case 6: /* toon */
        if (Ta_GetToonAgreement(vInfo->wCardNo) != 0) {
            ret = 1;
        } else {
            ret = 2;
        }
        break;
    case 3: /* meadow */
        switch (kind) {
        case 3:
        case 4:
            ret = 1;
            break;
        case 1:
            ret = 2;
            break;
        default:
            ret = 0;
            break;
        }
        break;
    case 0: /* forest */
        switch (kind) {
        case 4:
        case 5:
        case 9:
        case 17:
        case 19:
            ret = 1;
            break;
        case 7:
            ret = 2;
            break;
        default:
            ret = 0;
            break;
        }
        break;
    case 2: /* mountain */
        switch (kind) {
        case 0:
        case 6:
        case 8:
        case 15:
            ret = 1;
            break;
        case 2:
            ret = 2;
            break;
        default:
            ret = 0;
            break;
        }
        break;
    case 1: /* wasteland */
        switch (kind) {
        case 2:
        case 10:
        case 14:
        case 18:
            ret = 1;
            break;
        case 19:
        case 12:
        case 13:
        case 16:
            ret = 2;
            break;
        default:
            ret = 0;
            break;
        }
        break;
    case 4: /* sea */
        switch (kind) {
        case 12:
        case 13:
        case 15:
        case 16:
            ret = 1;
            break;
        case 14:
        case 17:
            ret = 2;
            break;
        default:
            ret = 0;
            break;
        }
        break;
    case 5: /* yami */
        switch (kind) {
        case 2:
        case 1:
        case 7:
            ret = 1;
            break;
        case 8:
            ret = 2;
            break;
        default:
            ret = 0;
            break;
        }
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}
