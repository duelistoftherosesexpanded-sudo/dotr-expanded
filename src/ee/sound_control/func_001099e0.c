#include "dotr/ee/sound_control.h"

void Card_V_IN(struct DotrSoundControlVector4 *result, struct DotrSoundControlVector4 *value,
                   struct DotrSoundControlPair *pair, float divisor)
{
    struct DotrSoundControlVector4 local;
    local.x = (value->x - (pair->first.x + pair->second.x)) / divisor;
    local.z = (value->z - (pair->first.z + pair->second.z)) / divisor;
    local.y = (value->y - (pair->first.y + pair->second.y)) / divisor;
    local.w = 0;
    *result = local;
}
