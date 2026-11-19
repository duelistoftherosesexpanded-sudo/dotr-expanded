#include "dotr/ee/sacrifice_pick.h"

int Ta_GetSacrificeUnit(int *sacrifices, int side)
{
    int results[6];
    int scores[6];
    int shuffledSacrifices[3];
    int i;
    int j;
    int n;
    int best;
    int r;

    shuffledSacrifices[2] = -1;
    shuffledSacrifices[1] = -1;
    shuffledSacrifices[0] = -1;
    for (i = 0; i < 6; i++) {
        scores[i] = -1;
        results[i] = -1;
    }
    for (i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            shuffledSacrifices[0] = sacrifices[0]; shuffledSacrifices[1] = sacrifices[1]; shuffledSacrifices[2] = sacrifices[2];
            break;
        case 1:
            shuffledSacrifices[0] = sacrifices[0]; shuffledSacrifices[1] = sacrifices[2]; shuffledSacrifices[2] = sacrifices[1];
            break;
        case 2:
            shuffledSacrifices[0] = sacrifices[1]; shuffledSacrifices[1] = sacrifices[0]; shuffledSacrifices[2] = sacrifices[2];
            break;
        case 3:
            shuffledSacrifices[0] = sacrifices[1]; shuffledSacrifices[1] = sacrifices[2]; shuffledSacrifices[2] = sacrifices[0];
            break;
        case 4:
            shuffledSacrifices[0] = sacrifices[2]; shuffledSacrifices[1] = sacrifices[0]; shuffledSacrifices[2] = sacrifices[1];
            break;
        case 5:
            shuffledSacrifices[0] = sacrifices[2]; shuffledSacrifices[1] = sacrifices[1]; shuffledSacrifices[2] = sacrifices[0];
            break;
        default:
            shuffledSacrifices[0] = sacrifices[0]; shuffledSacrifices[1] = sacrifices[1]; shuffledSacrifices[2] = sacrifices[2];
            break;
        }
        r = Ta_GetScrificeOrder(shuffledSacrifices, side);
        results[i] = r;
        if (r >= 0) {
            scores[i] = 0;
            n = Ta_GetUnitNumFrom22bitFlagList(results[i]);
            if (n == 3) {
                for (j = 0; j < n; j++) {
                    int v = Ta_GetUnitOwnerIDFromLstCount(results[i], j);
                    scores[i] += Ta_GetAPPlusDP(v >> 8, v & 0xff);
                }
            } else {
                scores[i] = -1;
            }
        }
    }
    /* MATCHING DEVICE: the last loop counts in `n`, keeps its lowest score in `j` and reads
     * each score into `r`; with `i`, `min` and `s` three registers rotate. */
    best = 255;
    j = 0xffff;
    for (n = 5; n >= 0; n--) {
        r = scores[n];
        if (r < 0) {
            continue;
        }
        if (r <= j) {
            j = r;
            best = n;
        }
    }
    if (best == 255) {
        return -1;
    }
    return results[best];
}
