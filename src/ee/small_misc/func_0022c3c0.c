#include "dotr/ee/small_misc.h"

/* Steps the fade level by 4: dir 0 fades down to 0, dir 1 up to 128.
 * Returns 1 once the limit is reached, otherwise 0. */
int My_LedarModel_a_set(int dir)
{
    switch (dir) {
    case 0:
        DOTR_MISC_FADE_LEVEL = DOTR_MISC_FADE_LEVEL - 4;
        if (DOTR_MISC_FADE_LEVEL <= 0) { DOTR_MISC_FADE_LEVEL = 0; return 1; }
        break;
    case 1:
        DOTR_MISC_FADE_LEVEL = DOTR_MISC_FADE_LEVEL + 4;
        if (DOTR_MISC_FADE_LEVEL >= 128) { DOTR_MISC_FADE_LEVEL = 128; return 1; }
        break;
    }
    return 0;
}
