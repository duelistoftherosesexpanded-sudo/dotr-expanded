/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

unsigned char SzGame_GetFrameSpeed(void)
{
    return gSzGame_gl.byte_22a;
}
