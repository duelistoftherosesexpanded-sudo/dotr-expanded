/* Evidence: docs/modules/game-modes.md. Names and prototypes are assumed. */
#include "dotr/ee/movie_operations.h"

void SzGame_Pop(void)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;
    if (state->field_222 - 1 >= 0) {
        SzFade_Entry(0, 1, 1, 0, 60, 0);
        state->field_21f = -2;
        state->field_220 = ((signed char *)(state->field_222 + (DotrMovieAddress)state))[0x1f0];
        state->field_221 = -1;
        if (((signed char *)(state->field_222 + (DotrMovieAddress)state))[0x1f1] < 2)
            SzGame_ReqBGM(-2, 1);
    }
}
