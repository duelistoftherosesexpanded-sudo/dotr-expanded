#include "dotr/ee/obj_vectors.h"

/* Each outcome copies the rectangle out as one 16-byte structure, as the original does. */
#define RECT_OUT() (*out = *(struct DotrObjVector *)&rect)

void RH2_DATA_IN(struct DotrObjVector *out, short kind, short sub)
{
    struct DotrUvRect rect;

    if (kind == 0) {
        rect.u = 64;
        rect.v = 0;
        rect.w = 63;
        rect.h = 63;
        RECT_OUT();
    } else if (kind == 1) {
        rect.u = 240;
        rect.v = sub << 4;
        rect.w = 16;
        rect.h = 16;
        RECT_OUT();
    } else if (kind == 2) {
        rect.w = 31;
        rect.h = 64;
        if (sub == 0) {
            rect.u = 143;
            rect.v = 0;
            RECT_OUT();
        } else {
            rect.u = 143;
            rect.v = 63;
            RECT_OUT();
        }
    } else if (kind == 3) {
        rect.u = 128;
        rect.v = 128;
        rect.w = 127;
        rect.h = 63;
        RECT_OUT();
    } else {
        if (kind == 4) {
            rect.w = 63;
            rect.h = 32;
            if (sub == 0) {
                rect.u = 128;
                rect.v = 128;
                RECT_OUT();
                return;
            } else if (sub == 1) {
                rect.u = 128;
                rect.v = 160;
                RECT_OUT();
                return;
            } else if (sub == 2) {
                rect.u = 192;
                rect.v = 128;
                RECT_OUT();
                return;
            } else if (sub == 3) {
                rect.u = 192;
                rect.v = 160;
                RECT_OUT();
                return;
            }
        } else if (kind == 5) {
            rect.u = 143;
            rect.v = 0;
            rect.w = 31;
            rect.h = 127;
            RECT_OUT();
            return;
        } else if (kind == 6) {
            rect.u = 176;
            rect.v = 0;
            rect.w = 59;
            rect.h = 31;
            RECT_OUT();
            return;
        }
        rect.u = 0;
        rect.v = 0;
        rect.w = 96;
        rect.h = 96;
        RECT_OUT();
    }
}
