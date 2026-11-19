/* Evidence: docs/modules/game-modes.md. */
#include "dotr/ee/movie_controls.h"

unsigned int SzGame_GetHSyncCount(void)
{
    return *(volatile unsigned int *)0x10000800;
}
