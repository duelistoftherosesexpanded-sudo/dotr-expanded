#include "dotr/ee/kind_pairs.h"

/* For attributes 0-5 (light, dark, fire, earth, water, wind): -1 when `attrB` is the one
 * attribute listed first for `attrA`, 1 when it is the one listed second, otherwise 0; 0 for
 * any other `attrA`. One jump table at 0x30fb70. */
int ChkAttrAdv(unsigned int attrA, int attrB)
{
    switch (attrA) {
    case 0: /* light */
        if (attrB == 1) {
            return -1;
        }
        if (attrB == 4) {
            return 1;
        }
        return 0;
    case 1: /* dark */
        if (attrB == 5) {
            return -1;
        }
        if (attrB == 0) {
            return 1;
        }
        return 0;
    case 2: /* fire */
        if (attrB == 4) {
            return -1;
        }
        if (attrB == 3) {
            return 1;
        }
        return 0;
    case 3: /* earth */
        if (attrB == 2) {
            return -1;
        }
        if (attrB == 5) {
            return 1;
        }
        return 0;
    case 4: /* water */
        if (attrB == 0) {
            return -1;
        }
        if (attrB == 2) {
            return 1;
        }
        return 0;
    case 5: /* wind */
        if (attrB == 3) {
            return -1;
        }
        if (attrB == 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}
