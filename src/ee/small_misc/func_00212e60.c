#include "dotr/ee/small_misc.h"

int GetAnyCoordinate(int id, struct DotrMiscMatrix *out)
{
    struct DotrMiscMatrix local;
    unsigned i;

    if (require_coordinate_no[0] == 0 || id <= 0 || out == 0) {
        return 0;
    }
    for (i = 0; i < (unsigned)require_coordinate_no[0]; i++) {
        if (require_coordinate[i].unknown_00 == id) {
            if (require_coordinate[i].unknown_04 == -1) {
                return 0;
            }
            if (require_coordinate[i].unknown_04 == 1) {
                require_coordinate[i].unknown_04 = 0;
                func_00198790(&local, &InvertView, 64);
                RhGeom_MulMatrix(&local, (struct DotrMiscMatrix *)require_coordinate[i].unknown_10);
                func_00198790(require_coordinate[i].unknown_10, &local, 64);
            }
            if (require_coordinate[i].unknown_04 == 0) {
                func_00198790(out, require_coordinate[i].unknown_10, 64);
            }
            break;
        }
    }
    if (i == (unsigned)require_coordinate_no[0]) {
        return 0;
    }
    return 1;
}
