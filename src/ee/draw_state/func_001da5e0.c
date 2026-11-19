#include "dotr/ee/draw_state.h"

int SzDuel_GetCardNodeUnitOnScreen(int mode, float cx, struct DotrDrawQuadVertex *out, struct DotrDrawQuadScale *scale, float cy)
{
    float width = (float)(mode ? 256 : 120);
    float height = (float)(mode ? 368 : 180);
    float hw = (scale ? width * scale->x : width) / 2.0f;
    float hh = (scale ? height * scale->y : height) / 2.0f;
    float z = SzView_GetScreenDistance(0);

    out[0].color[0] = 0x80;
    out[0].color[1] = 0x80;
    out[0].color[2] = 0x80;
    out[0].color[3] = 0x80;
    out[0].q = 1.0f;
    out[0].x = cx - hw;
    out[0].y = cy - hh;
    out[0].z = z;
    out[0].w = 1.0f;
    out[0].u = 0.0f;
    out[0].v = 0.0f;
    out[1].color[0] = 0x80;
    out[1].color[1] = 0x80;
    out[1].color[2] = 0x80;
    out[1].color[3] = 0x80;
    out[1].q = 1.0f;
    out[1].x = cx - hw;
    out[1].y = cy + hh;
    out[1].z = z;
    out[1].w = 1.0f;
    out[1].u = 0.0f;
    out[1].v = height / 512.0f;
    out[2].color[0] = 0x80;
    out[2].color[1] = 0x80;
    out[2].color[2] = 0x80;
    out[2].color[3] = 0x80;
    out[2].q = 1.0f;
    out[2].x = cx + hw;
    out[2].y = cy + hh;
    out[2].z = z;
    out[2].w = 1.0f;
    out[2].u = width / 512.0f;
    out[2].v = height / 512.0f;
    out[3].color[0] = 0x80;
    out[3].color[1] = 0x80;
    out[3].color[2] = 0x80;
    out[3].color[3] = 0x80;
    out[3].q = 1.0f;
    out[3].x = cx + hw;
    out[3].y = cy - hh;
    out[3].z = z;
    out[3].w = 1.0f;
    out[3].u = width / 512.0f;
    out[3].v = 0.0f;
    return 4;
}
