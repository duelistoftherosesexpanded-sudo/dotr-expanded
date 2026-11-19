#include "dotr/ee/small_misc.h"


void TaLogic_DoSortTaskList(struct DotrSlot16 *taskList)
{
    int i;
    int j;
    struct DotrSlot16 *dst;
    for (i = 0, dst = taskList; i < 22; i++, dst++) {
        if (dst->id == 0xffff) {
            for (j = i + 1; j < 22; j++) {
                struct DotrSlot16 *src = &taskList[j];
                if (src->id != 0xffff) {
                    *(struct DotrSlotF4 *)dst = *(struct DotrSlotF4 *)src;
                    j = 22;
                    src->id = 0xffff;
                    src->tag = 255;
                    src->word_08 = -1;
                    src->aux = 0xffff;
                    src->flag_lo = 0;
                }
            }
        }
    }
}
