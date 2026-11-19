#include "dotr/ee/card_control.h"

/* 0x1cdeb0 SzDuel_DoCardControl0 (name from the Japanese build's symbols).
 * The pinned compiler builds `% 0x10000` on a signed int with `addiu v1,v1,0` where the
 * negative correction belongs, as the original has it: for a negative value that is not a
 * multiple of 0x10000 the machine code keeps the low sixteen bits, unlike host C. */
int SzDuel_DoCardControl0(struct DotrCardControl *c)
{
    struct DotrCardControlOut *out = c->out;
    float t;
    float inv;
    if (out != 0) {
        t = (float)c->count / (float)c->total;
        inv = 1.0f - t;
        if (c->do_pos) {
            out->pos[0] = c->pos[0][0] * inv + c->pos[1][0] * t;
            out->pos[1] = c->pos[0][1] * inv + c->pos[1][1] * t;
            out->pos[2] = c->pos[0][2] * inv + c->pos[1][2] * t;
            out->pos[3] = c->pos[0][3] * inv + c->pos[1][3] * t;
        }
        if (c->do_rot) {
            out->rot[0] = func_001a22c0((float)c->rot[0][0] * inv + (float)c->rot[1][0] * t) % 0x10000;
            out->rot[1] = func_001a22c0((float)c->rot[0][1] * inv + (float)c->rot[1][1] * t) % 0x10000;
            out->rot[2] = func_001a22c0((float)c->rot[0][2] * inv + (float)c->rot[1][2] * t) % 0x10000;
            out->rot[3] = func_001a22c0((float)c->rot[0][3] * inv + (float)c->rot[1][3] * t) % 0x10000;
        }
        if (c->do_rot2) {
            out->rot2[0] = func_001a22c0((float)c->rot2[0][0] * inv + (float)c->rot2[1][0] * t) % 0x10000;
            out->rot2[1] = func_001a22c0((float)c->rot2[0][1] * inv + (float)c->rot2[1][1] * t) % 0x10000;
            out->rot2[2] = func_001a22c0((float)c->rot2[0][2] * inv + (float)c->rot2[1][2] * t) % 0x10000;
            out->rot2[3] = func_001a22c0((float)c->rot2[0][3] * inv + (float)c->rot2[1][3] * t) % 0x10000;
        }
        if (c->do_scale) {
            out->scale[0] = c->scale[0][0] * inv + c->scale[1][0] * t;
            out->scale[1] = c->scale[0][1] * inv + c->scale[1][1] * t;
            out->scale[2] = c->scale[0][2] * inv + c->scale[1][2] * t;
            out->scale[3] = c->scale[0][3] * inv + c->scale[1][3] * t;
        }
        if (c->do_color) {
            out->color[2] = func_001a22c0((float)c->color[0][2] * inv + (float)c->color[1][2] * t);
            out->color[1] = func_001a22c0((float)c->color[0][1] * inv + (float)c->color[1][1] * t);
            out->color[0] = func_001a22c0((float)c->color[0][0] * inv + (float)c->color[1][0] * t);
            out->color[3] = func_001a22c0((float)c->color[0][3] * inv + (float)c->color[1][3] * t);
        }
        return 1;
    }
    return 0;
}
