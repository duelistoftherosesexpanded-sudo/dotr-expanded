/* Evidence: docs/modules/game-modes.md. Bit-field spelling reproduces
 * observed masks and accesses, without establishing the original declarations.
 */
#include "dotr/ee/movie_operations.h"

extern void SzGame_ModeTerm(int);
extern void SzGame_ModeInit(int);

void SzGame_SetMode(int arg0, int arg1)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;
    int flag = 1;
    unsigned int prior = state->bytes_1f9[state->field_222];
    state->field_21e = state->bytes_1f1[state->field_222];
    state->field_222 += arg1;
    state->bytes_1f1[state->field_222] = arg0;
    state->field_21f = arg0;
    if (state->bytes_1f9[state->field_222] == 0 || arg1 <= 0)
        SzGame_ModeTerm(state->field_21e);
    else
        flag = 0;
    SzGame_ComInit();
    if (prior == 0 || arg1 >= 0)
        SzGame_ModeInit(state->field_21f);
    else
        flag = 0;
    switch (arg0) {
    case -3:
    case -2:
    case -1:
    case 10:
    case 36:
        flag = 0;
        break;
    case 15:
        if (state->field_21e == 1)
            flag = 0;
        break;
    }
    state->field_221 = 0;
    state->field_22c.field_4 = 3;
    state->field_22e.field_4 = (signed char)flag;
    state->field_00c = 0;
    state->field_229 = 0;
    state->field_228 = 0;
    state->field_22c.field_2 = 1;
}
