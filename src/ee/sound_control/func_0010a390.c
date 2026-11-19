#include "dotr/ee/sound_control.h"

void SD_SEPlayOnlyOnce(int a, int b, int c)
{
    SD_SEPlayWithParam6(a, b, 0, 0, 0, c);
}
