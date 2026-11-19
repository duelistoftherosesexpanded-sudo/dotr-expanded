#include "dotr/ee/lk_assign_uv.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* LkAssignUVST: writes the current animation cell into the four texture coordinates of a sprite
 * packet and advances the animation. Flag 1: the cell is in integer texels (u, v from the base
 * plus the frame's column and row in the sheet; 0x800 set in the packet flags; 12.4 uv words);
 * else in float texture units (s, t; the grid is 1 / w by 1 / h). Flag 2 runs frames along rows,
 * else down columns. The timer counts to the delay, then the frame steps; past the last frame it
 * rewinds by the loop length when loop is 999, else loop is set to 1.
 * Compiled warm. An inline helper tests the flag bits. */

static inline int Flag_LkAssignUVST(struct DotrLkAssignAnim *anim, int bit) {
    return (anim->flags & bit) ? 1 : 0;
}

void LkAssignUVST(void *arg0, void *arg1) {
    struct DotrLkAssignStSprite *sprite = arg0;
    struct DotrLkAssignAnim *anim = arg1;

    if (Flag_LkAssignUVST(anim, 1)) {
        if (Flag_LkAssignUVST(anim, 2)) {
            anim->r.i.u = anim->r.i.baseU + anim->r.i.w * (anim->frame % (anim->r.i.texW / anim->r.i.w));
            anim->r.i.v = anim->r.i.baseV + anim->r.i.h * (anim->frame / (anim->r.i.texW / anim->r.i.w));
        } else {
            anim->r.i.u = anim->r.i.baseU + anim->r.i.w * (anim->frame / (anim->r.i.texH / anim->r.i.h));
            anim->r.i.v = anim->r.i.baseV + anim->r.i.h * (anim->frame % (anim->r.i.texH / anim->r.i.h));
        }
        sprite->flags |= 0x800;
        sprite->v[0].tex.uv.x.val = anim->r.i.u << 4;
        sprite->v[0].tex.uv.y.val = anim->r.i.v << 4;
        sprite->v[1].tex.uv.x.val = (anim->r.i.u + anim->r.i.w) << 4;
        sprite->v[1].tex.uv.y.val = anim->r.i.v << 4;
        sprite->v[2].tex.uv.x.val = anim->r.i.u << 4;
        sprite->v[2].tex.uv.y.val = (anim->r.i.v + anim->r.i.h) << 4;
        sprite->v[3].tex.uv.x.val = (anim->r.i.u + anim->r.i.w) << 4;
        sprite->v[3].tex.uv.y.val = (anim->r.i.v + anim->r.i.h) << 4;
    } else {
        if (Flag_LkAssignUVST(anim, 2)) {
            anim->r.f.x = anim->r.f.w * (float)(anim->frame % (short)(1.0f / anim->r.f.w));
            anim->r.f.y = anim->r.f.h * (float)(anim->frame / (short)(1.0f / anim->r.f.w));
        } else {
            anim->r.f.x = anim->r.f.w * (float)(anim->frame / (short)(1.0f / anim->r.f.h));
            anim->r.f.y = anim->r.f.h * (float)(anim->frame % (short)(1.0f / anim->r.f.h));
        }
        sprite->v[0].tex.st.s = anim->r.f.x;
        sprite->v[0].tex.st.t = anim->r.f.y;
        sprite->v[1].tex.st.s = anim->r.f.x + anim->r.f.w;
        sprite->v[1].tex.st.t = anim->r.f.y;
        sprite->v[2].tex.st.s = anim->r.f.x;
        sprite->v[2].tex.st.t = anim->r.f.y + anim->r.f.h;
        sprite->v[3].tex.st.s = anim->r.f.x + anim->r.f.w;
        sprite->v[3].tex.st.t = anim->r.f.y + anim->r.f.h;
    }
    anim->timer++;
    if (anim->timer > anim->delay) {
        anim->timer = 0;
        anim->frame++;
        if (anim->frame > anim->last) {
            if (anim->loop == 999) {
                anim->frame = anim->last - anim->loopLength + 1;
            } else {
                anim->loop = 1;
            }
        }
    }
}
