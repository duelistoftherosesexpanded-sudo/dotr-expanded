#include "dotr/ee/lk_assign_uv.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* LkAssignUVST3: LkAssignUVST for one of sixteen instances (arg2) of a state record: writes the
 * cell of that instance's frame into the packet's four texture coordinates (integer texels with
 * the far edges at size - 1 and 0x800 set in the flags, or float s/t), then advances its timer
 * and frame; past the last frame, with flag 4 the instance restarts at the base position plus a
 * random offset within +-randomX / +-randomY (a negative randomY adds rand % randomY), and the
 * frame steps back by the rewind count.
 * Compiled warm. An inline helper tests the flag bits. */

static inline int Flag_LkAssignUVST3(struct DotrLkAssignState3 *s, int bit) {
    return (s->flags & bit) ? 1 : 0;
}

void LkAssignUVST3(void *arg0, void *arg1, int arg2) {
    struct DotrLkAssignState3 *state = arg1;
    unsigned char *packet = arg0;

    if (Flag_LkAssignUVST3(state, 1)) {
        if (Flag_LkAssignUVST3(state, 2)) {
            state->uv.fixed.u = state->uv.fixed.baseU + state->uv.fixed.width *
                (state->frames[arg2].index % (state->uv.fixed.imageWidth / state->uv.fixed.width));
            state->uv.fixed.v = state->uv.fixed.baseV + state->uv.fixed.height *
                (state->frames[arg2].index / (state->uv.fixed.imageWidth / state->uv.fixed.width));
        } else {
            state->uv.fixed.u = state->uv.fixed.baseU + state->uv.fixed.width *
                (state->frames[arg2].index / (state->uv.fixed.imageHeight / state->uv.fixed.height));
            state->uv.fixed.v = state->uv.fixed.baseV + state->uv.fixed.height *
                (state->frames[arg2].index % (state->uv.fixed.imageHeight / state->uv.fixed.height));
        }
        *(unsigned short *)(packet + 2) |= 0x800;
        ((struct DotrLkAssignUv3 *)(packet + 0x18))->u = state->uv.fixed.u << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x18))->v = state->uv.fixed.v << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x30))->u = (state->uv.fixed.u + state->uv.fixed.width - 1) << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x30))->v = state->uv.fixed.v << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x48))->u = state->uv.fixed.u << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x48))->v = (state->uv.fixed.v + state->uv.fixed.height - 1) << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x60))->u = (state->uv.fixed.u + state->uv.fixed.width - 1) << 4;
        ((struct DotrLkAssignUv3 *)(packet + 0x60))->v = (state->uv.fixed.v + state->uv.fixed.height - 1) << 4;
    } else {
        if (Flag_LkAssignUVST3(state, 2)) {
            state->uv.floating.u = state->uv.floating.width *
                (state->frames[arg2].index % (short)(1.0f / state->uv.floating.width));
            state->uv.floating.v = state->uv.floating.height *
                (state->frames[arg2].index / (short)(1.0f / state->uv.floating.width));
        } else {
            state->uv.floating.u = state->uv.floating.width *
                (state->frames[arg2].index / (short)(1.0f / state->uv.floating.height));
            state->uv.floating.v = state->uv.floating.height *
                (state->frames[arg2].index % (short)(1.0f / state->uv.floating.height));
        }
        *(float *)(packet + 0x18) = state->uv.floating.u;
        *(float *)(packet + 0x1C) = state->uv.floating.v;
        *(float *)(packet + 0x30) = state->uv.floating.u + state->uv.floating.width;
        *(float *)(packet + 0x34) = state->uv.floating.v;
        *(float *)(packet + 0x48) = state->uv.floating.u;
        *(float *)(packet + 0x4C) = state->uv.floating.v + state->uv.floating.height;
        *(float *)(packet + 0x60) = state->uv.floating.u + state->uv.floating.width;
        *(float *)(packet + 0x64) = state->uv.floating.v + state->uv.floating.height;
    }

    state->frames[arg2].timer++;
    if (state->frames[arg2].timer > state->delay) {
        state->frames[arg2].timer = 0;
        state->frames[arg2].index++;
        if (state->frames[arg2].index > state->end) {
            if (Flag_LkAssignUVST3(state, 4)) {
                short range;
                state->frames[arg2].x = state->x;
                state->frames[arg2].y = state->y;
                range = state->randomX;
                if (range != 0) {
                    state->frames[arg2].x = state->x + (func_0019a5c8() % (range * 2) - range);
                }
                range = state->randomY;
                if (range != 0) {
                    state->frames[arg2].y = state->y + (func_0019a5c8() % (range * 2) - range);
                }
                if (state->randomY < 0) {
                    state->frames[arg2].y = state->y + func_0019a5c8() % state->randomY;
                }
            }
            state->frames[arg2].index -= state->rewind;
        }
    }
}
