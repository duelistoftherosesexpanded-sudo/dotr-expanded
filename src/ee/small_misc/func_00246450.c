#include "dotr/ee/small_misc.h"

void ShFace_Init(int value)
{
    int i;
    gSh_FaceMode[0] = value;
    for (i = 0; i < 8; i++) {
        gSh_Face[i].unknown_00 = 0;
    }
    gSh_pbVif1[0] = 0;
}
