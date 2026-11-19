#include "dotr/ee/lk_helpers.h"

void LkInitCoordinate(struct DotrLkCoordinate *coordinate)
{
    coordinate->rows[0][3] = 0x3f800000;
    coordinate->rows[0][2] = 0x3f800000;
    coordinate->rows[0][1] = 0x3f800000;
    coordinate->rows[0][0] = 0x3f800000;
    coordinate->rows[1][3] = 0;
    coordinate->rows[1][2] = 0;
    coordinate->rows[1][1] = 0;
    coordinate->rows[1][0] = 0;
    coordinate->rows[2][3] = 0;
    coordinate->rows[2][2] = 0;
    coordinate->rows[2][1] = 0;
    coordinate->rows[2][0] = 0;
}
