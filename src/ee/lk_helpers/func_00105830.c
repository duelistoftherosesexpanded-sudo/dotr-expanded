#include "dotr/ee/lk_helpers.h"

void LkAssignUV2(struct DotrLkUvPacket *packet, struct DotrLkUvAnim *anim, int mode)
{
    /* MATCHING DEVICE: the explicit 1/0 gives the original's li/movz before the branch. */
    if ((anim->flags & 2) ? 1 : 0) {
        anim->u = anim->u0 + anim->w * (anim->frame % (anim->sheet_w / anim->w));
        anim->v = anim->v0 + anim->h * (anim->frame / (anim->sheet_w / anim->w));
    } else {
        anim->u = anim->u0 + anim->w * (anim->frame / (anim->sheet_h / anim->h));
        anim->v = anim->v0 + anim->h * (anim->frame % (anim->sheet_h / anim->h));
    }
    if (mode == 0) {
        packet->flags |= 0x800;
        packet->vertex[0].u = anim->u << 4;
        packet->vertex[0].v = anim->v << 4;
        packet->vertex[1].u = (anim->u + anim->w - 1) << 4;
        packet->vertex[1].v = anim->v << 4;
        packet->vertex[2].u = anim->u << 4;
        packet->vertex[2].v = (anim->v + anim->h / 2 - 1) << 4;
        packet->vertex[3].u = (anim->u + anim->w - 1) << 4;
        packet->vertex[3].v = (anim->v + anim->h / 2 - 1) << 4;
    } else {
        packet->flags |= 0x800;
        packet->vertex[0].u = anim->u << 4;
        packet->vertex[0].v = (anim->v + anim->h / 2 - 1) << 4;
        packet->vertex[1].u = (anim->u + anim->w - 1) << 4;
        packet->vertex[1].v = (anim->v + anim->h / 2 - 1) << 4;
        packet->vertex[2].u = anim->u << 4;
        packet->vertex[2].v = (anim->v + anim->h - 1) << 4;
        packet->vertex[3].u = (anim->u + anim->w - 1) << 4;
        packet->vertex[3].v = (anim->v + anim->h - 1) << 4;
    }
    if (mode == 1) {
        anim->wait++;
        if (anim->wait > anim->wait_max) {
            anim->wait = 0;
            anim->frame++;
            if (anim->frame > anim->frame_max) {
                if (anim->state == 999) {
                    anim->frame = anim->frame_max - anim->loop_frames + 1;
                } else {
                    anim->state = 1;
                }
            }
        }
    }
}
