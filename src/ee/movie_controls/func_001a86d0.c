/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

int SzGame_InqSoftFocus(void)
{
    return gSzGame_gl.bits_22e.bit_5 ? 1 : 0;
}
