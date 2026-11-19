#include "dotr/ee/slot_table.h"

DotrDrawWord SzMsg_DrawX_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3, int a4)
{
    struct DotrSlotTableEntry *entry = SzMsg_GetInfo(a0);
    union DotrSlotArgsUnion args;
    struct DotrSlotTableWords *w = &args.words;
    *(DotrSlotWord128 *)w = *(DotrSlotWord128 *)&gSzMsg_defRect[0];
    if (entry == 0) {
        return cursor;
    }
    args.words.words[2] = entry->unknown_004[2];
    args.words.words[3] = entry->unknown_004[3];
    return ((DotrSlotDrawFn)(DotrDrawWord)a4)(cursor, a0, a1, a2, *w, a3);
}
