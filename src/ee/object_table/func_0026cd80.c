#include "dotr/ee/object_table.h"

void Ta109_BlackOutForExz(int direction)
{
    struct DotrObjectVector3 v;

    if (direction < 0) {
        SzModel_GetIntensity(1, &v);
        if (v.x - 0.08f < 0.0f) { v.x = 0.0f; } else { v.x -= 0.08f; }
        if (v.y - 0.08f < 0.0f) { v.y = 0.0f; } else { v.y -= 0.08f; }
        SzModel_SetIntensity(1, &v);
        SzModel_GetIntensity(5, &v);
        if (v.x - 0.08f < 0.0f) { v.x = 0.0f; } else { v.x -= 0.08f; }
        if (v.y - 0.08f < 0.0f) { v.y = 0.0f; } else { v.y -= 0.08f; }
        SzModel_SetIntensity(5, &v);
        SzModel_GetIntensity(6, &v);
        if (v.x - 0.08f < 0.0f) { v.x = 0.0f; } else { v.x -= 0.08f; }
        if (v.y - 0.08f < 0.0f) { v.y = 0.0f; } else { v.y -= 0.08f; }
        SzModel_SetIntensity(6, &v);
    } else {
        SzModel_GetIntensity(1, &v);
        v.x = 0.8f;
        v.y = 0.8f;
        SzModel_SetIntensity(1, &v);
        SzModel_GetIntensity(5, &v);
        v.x = 0.8f;
        v.y = 0.8f;
        SzModel_SetIntensity(5, &v);
        SzModel_GetIntensity(6, &v);
        v.x = 0.8f;
        v.y = 0.8f;
        SzModel_SetIntensity(6, &v);
    }
}
