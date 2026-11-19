#include "dotr/ee/movie_controls.h"

int SzGame_GetMode(int direction)
{
    struct DotrMovieControlPrefix *p = &gSzGame_gl;
    signed char currentMode;
    if (direction > 0) {
        currentMode = p->levels_1f1[p->byte_222];
    } else if (direction < 0) {
        currentMode = p->level_21e;
    } else {
        currentMode = p->level_21f;
    }
    return currentMode;
}
