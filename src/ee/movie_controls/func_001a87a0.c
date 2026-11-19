/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

int SzGame_GetStabilizer(void)
{
    return gSzGame_gl.bits_22e.bit_6 ? 1 : 0;
}
