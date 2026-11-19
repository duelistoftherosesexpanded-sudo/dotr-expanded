/* Synthetic host tests for the face_act group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/face_act.h"

int gSh_FaceMode[2] = {0, 77};

int main(void)
{
    /* Whatever the mode, the call returns and changes nothing. */
    ShFace_Act();
    assert(gSh_FaceMode[0] == 0 && gSh_FaceMode[1] == 77);
    gSh_FaceMode[0] = 3;
    ShFace_Act();
    assert(gSh_FaceMode[0] == 3 && gSh_FaceMode[1] == 77);
    gSh_FaceMode[0] = -1;
    ShFace_Act();
    assert(gSh_FaceMode[0] == -1 && gSh_FaceMode[1] == 77);
    puts("face_act: ok");
    return 0;
}
