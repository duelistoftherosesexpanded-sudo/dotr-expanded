#include "dotr/ee/packed_item.h"

/* Presets out to (0, 0, 0, 1), rejects a packed position outside 3 x 11 or without an
 * object, then forwards by posType: 0 the grid vector, 1 the object's vector, 2 the vector of
 * object 8. The lookup result reuses the `cell` parameter (it stays in a1). */
int SzEffect_GetPos2(int posType, int cell, struct DotrObjectVector4 *out)
{
    int x;
    int y;

    out->z = 0.0f;
    out->y = 0.0f;
    out->x = 0.0f;
    x = cell >> 8;
    y = cell & 0xff;
    out->unknown_w = 1.0f;
    if (x < 0 || x >= 3 || y < 0 || y >= 11) {
        return 0;
    }
    cell = SzDuel_GetUnitModelID(x, y);
    if (cell < 0) {
        return 0;
    }
    switch (posType) {
    case 0:
        return SzDuel_GetCardPos(x, y, (struct DotrStateVector *)out);
    case 1:
        return SzModel_GetCenter(cell, out);
    case 2:
        return SzModel_GetPos(8, out);
    }
    return 0;
}
