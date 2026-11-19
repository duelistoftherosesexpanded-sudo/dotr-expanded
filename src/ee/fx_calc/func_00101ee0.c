#include "dotr/ee/draw_state.h"
#include "dotr/ee/fx_calc.h"

/* Texture coordinates, clamp, texture and alpha of a four-vertex packet (alpha only for a non-zero mode). */
void ShFx_SetTexGT4(struct DotrFxGT4Packet *packet, struct DotrFxAlphaSource *source, int *texture)
{
    unsigned char alpha[4];

    packet->flags |= 0x800;
    packet->vertex[0].uv.u = source->left << 4;
    packet->vertex[0].uv.v = source->top << 4;
    packet->vertex[1].uv.u = source->right << 4;
    packet->vertex[1].uv.v = source->top << 4;
    packet->vertex[2].uv.u = source->left << 4;
    packet->vertex[2].uv.v = source->bottom << 4;
    packet->vertex[3].uv.u = source->right << 4;
    packet->vertex[3].uv.v = source->bottom << 4;
    ExAssignClamp((struct DotrDrawPacket *)packet,
                  (long)2 | ((long)2 << 2) | ((long)source->left << 4) | ((long)(source->right - 1) << 14) |
                      ((long)source->top << 24) | ((long)(source->bottom - 1) << 34));
    ExAssignTexture((struct DotrDrawPacket *)packet, texture, 1);
    if (source->mode != 0) {
        ShFx_GetAlpha(source, alpha);
        ExAssignAlpha((struct DotrDrawPacket *)packet,
                      (long)alpha[0] | ((long)alpha[1] << 2) | ((long)alpha[2] << 4) | ((long)alpha[3] << 6));
    }
}
