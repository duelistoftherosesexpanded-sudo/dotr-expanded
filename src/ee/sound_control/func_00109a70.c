#include "dotr/ee/sound_control.h"

int VEC_POI_IN(struct DotrSoundControlVector *destination, struct DotrSoundControlVector *source)
{
    destination->x = source->x;
    destination->y = source->y;
    destination->z = source->z;
    return 0;
}
