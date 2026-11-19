#include "dotr/ee/draw_quad_block.h"

/* Card quad centred on the origin: 120x180, or 256x368 for a non-zero mode. */
int SzDuel_GetCardNodeUnit(int mode, struct DotrDrawQuadVertex *out)
{
    float width = 120.0f;
    float height = 180.0f;

    if (mode) {
        width = 256.0f;
        height = 368.0f;
    }

    out[0].color[0] = 0x80;
    out[0].color[1] = 0x80;
    out[0].color[2] = 0x80;
    out[0].color[3] = 0x80;
    out[0].q = 1.0f;
    out[0].x = -(width / 2.0f);
    out[0].y = height / 2.0f;
    out[0].z = 0.0f;
    out[0].w = 1.0f;
    out[0].u = 0.0f;
    out[0].v = 0.0f;
    out[1].color[0] = 0x80;
    out[1].color[1] = 0x80;
    out[1].color[2] = 0x80;
    out[1].color[3] = 0x80;
    out[1].q = 1.0f;
    out[1].x = -(width / 2.0f);
    out[1].y = -(height / 2.0f);
    out[1].z = 0.0f;
    out[1].w = 1.0f;
    out[1].u = 0.0f;
    out[1].v = height / 512.0f;
    out[2].color[0] = 0x80;
    out[2].color[1] = 0x80;
    out[2].color[2] = 0x80;
    out[2].color[3] = 0x80;
    out[2].q = 1.0f;
    out[2].x = width / 2.0f;
    out[2].y = -(height / 2.0f);
    out[2].z = 0.0f;
    out[2].w = 1.0f;
    out[2].u = width / 512.0f;
    out[2].v = height / 512.0f;
    out[3].color[0] = 0x80;
    out[3].color[1] = 0x80;
    out[3].color[2] = 0x80;
    out[3].color[3] = 0x80;
    out[3].q = 1.0f;
    out[3].x = width / 2.0f;
    out[3].y = height / 2.0f;
    out[3].z = 0.0f;
    out[3].w = 1.0f;
    out[3].u = width / 512.0f;
    out[3].v = 0.0f;
    return 4;
}
