#include "dotr/ee/card_scale_on.h"

int CARD_SCL_ON_1(int card, float step)
{
    struct DotrCardScaleVec v;
    if (func_001f5af0_card(card)) {
        /* The original passes the vector before it is set (see the header). */
        func_001f5d10_card(card, &v);
        v.x += step;
        if (v.x > 0.1) {
            v.x = 0.1f;
        }
        v.y += step;
        if (v.y > 0.1) {
            v.y = 0.1f;
        }
        v.z += step;
        if (v.z > 0.1) {
            v.z = 0.1f;
        }
        func_001f5d10_card(card, &v);
    }
    return 0;
}
