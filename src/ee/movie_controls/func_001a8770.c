/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

void SzGame_SetStabilizer(int value)
{
    gSzGame_gl.bits_22e.bit_6 = (signed char)value;
}
