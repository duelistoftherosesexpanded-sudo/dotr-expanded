#include "dotr/ee/small_misc.h"

int Ta_IsFinishDestEfc(struct DotrMiscFlagWord *flags, DotrMiscByteList *list)
{
    int i;
    if (Ta_IsFinishEffect(list) == 1) {
        for (i = 0; i < 22; i++) {
            if (((flags->unknown_08 >> (21 - i)) & 1) == 1) {
                if (i < 11) {
                    Ta_DelUnit(0, i);
                } else {
                    Ta_DelUnit(1, i - 11);
                }
            }
        }
        return 1;
    }
    return 0;
}
