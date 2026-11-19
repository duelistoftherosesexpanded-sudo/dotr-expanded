/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

void SzGame_SetAutoBlur(int value)
{
    struct DotrMovieControlPrefix *state = &gSzGame_gl;
    if (value >= 0 && (unsigned int)value < state->word_0f4)
        state->byte_22b = value;
}
