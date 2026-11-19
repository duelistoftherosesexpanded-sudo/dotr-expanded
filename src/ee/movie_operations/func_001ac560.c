#include "dotr/ee/movie_operations.h"

void lSzGame_ActMemCheck(void)
{
    if (SzGame_ActMemCardProc() < 0) {
        gSzGame_gl_operations.field_160 = 0;
        SzFade_Entry(0, 1, 1, 0, 60, 0);
        gSzGame_gl_operations.field_21f = -2;
        gSzGame_gl_operations.field_220 = 10;
        gSzGame_gl_operations.field_221 = 0;
        (gSzGame_gl_operations.bytes_1f9 + gSzGame_gl_operations.field_222)[gSzGame_gl_operations.field_221] = 0;
        SzGame_ReqBGM(-2, 1);
    }
}
