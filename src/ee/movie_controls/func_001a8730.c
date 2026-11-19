/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

int SzGame_AddFrameCount(void)
{
    return ++gSzGame_gl.word_004;
}
