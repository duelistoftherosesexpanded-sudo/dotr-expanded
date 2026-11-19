#include "dotr/ee/state_grid.h"
#include "dotr/ee/object_table.h"

int lSzBattle_DrawWasteland_Pipe(int cursor, int index, int count)
{
    /* The second word of the copied block is replaced by an integer. */
    union {
        struct DotrObjectVector4 vector;
        int words[4];
    } v;
    cursor = SzModel_DrawWithOutCalc_Pipe(cursor, index, 0, 0, 0, 0, 0);
    if (count < 9) {
        SzModel_GetRot(index, &v.vector);
        v.words[1] = 0x8000;
        cursor = SzModel_DrawWithOutCalc_Pipe(cursor, index, 0, &v, 0, 0, 0);
    }
    return cursor;
}
