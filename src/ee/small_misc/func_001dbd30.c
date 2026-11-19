#include "dotr/ee/small_misc.h"

int SzDuel_DrawCard_Pipe(int a,struct DotrMiscSource *b)
{
    return ExSortNode_Pipe(a,b->unknown_48, b->unknown_82_bits_0_11, b->unknown_78,0,0);
}
