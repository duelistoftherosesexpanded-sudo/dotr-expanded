#include "dotr/ee/small_misc.h"

int BookingGetAnyCoordinate(int value)
{
    unsigned count = require_coordinate_no[0];
    if (count >= 256) {
        return 0;
    }
    require_coordinate[count].unknown_00 = value;
    require_coordinate[count].unknown_04 = -1;
    require_coordinate_no[0]++;
    return 1;
}
