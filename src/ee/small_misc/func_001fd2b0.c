#include "dotr/ee/small_misc.h"

unsigned char*SzMsg_Initialize(void)
{
    return RhMem_FillMemory(gSzMsg_ml,0,636);
}
