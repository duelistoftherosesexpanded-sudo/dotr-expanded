/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

int SzGame_VSync(void)
{
    ++gSzGame_gl.word_000;
    return 1;
}
