#include "dotr/ee/small_misc.h"

void ShFace_Close(unsigned char *p)
{
    gSh_Face[*p].unknown_00 = 0;
}
