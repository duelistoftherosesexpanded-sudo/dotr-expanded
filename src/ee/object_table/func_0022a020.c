#include "dotr/ee/object_table.h"

/* Threads the cursor through SzModel_Draw_Pipe for indices 34..43, passing a local copy of
 * the 16 bytes at 0x29abb0 (second word replaced by the short at 0x1ed53f4) when the
 * byte at 0x1ed53b3 is 1 and a null pointer otherwise, then calls
 * SzModel_SetDrawFlag(i, 0) for indices 30 and 31. Returns the cursor. */
int Taku_Camp_STON(int cursor)
{
    struct DotrObjectVector16 v = data_0029abb0;
    struct DotrObjectBank *bank = DOTR_OBJECT_BANK;
    short i;
    short j;

    v.unknown_words[1] = data_01ed53f4;
    for (i = 0; i <= 9; i++) {
        switch (bank->unknown_53b3) {
        case 1:
            cursor = SzModel_Draw_Pipe(cursor, i + 34, 0, &v, 0, 0, 0);
            break;
        default:
            cursor = SzModel_Draw_Pipe(cursor, i + 34, 0, 0, 0, 0, 0);
            break;
        }
    }
    for (j = 0; j < 2; j++) {
        SzModel_SetDrawFlag(j + 30, 0);
    }
    return cursor;
}
