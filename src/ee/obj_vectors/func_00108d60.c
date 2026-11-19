#include "dotr/ee/obj_vectors.h"

#define SET_INMAP4(X, Y) \
    x = (float)(X); \
    y = (float)(Y); \
    v[0] = x; \
    v[1] = y; \
    v[2] = 0.0f; \
    v[4] = 0.0f; \
    v[5] = y; \
    v[6] = 0.0f; \
    v[8] = x; \
    v[9] = 0.0f; \
    v[10] = 0.0f; \
    v[12] = 0.0f; \
    v[13] = 0.0f; \
    v[14] = 0.0f;

int PAINT_VECTOR_4TG_SET_INMAP4(float *v, short w, short h, short mode)
{
    float x, y;

    if (mode == 0) {
        SET_INMAP4(-w, h)
    } else if (mode == 1) {
        SET_INMAP4(w, h)
    } else if (mode == 2) {
        SET_INMAP4(-w, -h)
    } else if (mode == 3) {
        SET_INMAP4(w, -h)
    }
    return 0;
}
