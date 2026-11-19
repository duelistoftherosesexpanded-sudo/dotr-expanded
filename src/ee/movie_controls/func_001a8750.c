/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

void SzGame_SetFrameSpeed(unsigned char value)
{
    gSzGame_gl.byte_22a = value;
}
