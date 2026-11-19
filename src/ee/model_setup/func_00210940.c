#include "dotr/ee/model_setup.h"

/* Float-motion walker: lRhModel_GetInterpolateMotionItem with the first item table and frame.
 * Per node of the `next` chain: the parent's scale is copied to a 16-aligned local and the
 * node's first 48 bytes to the scratchpad. While index is below the pad's count the motion
 * item of that index (24 bytes each with motion flag bit 2, else 16) is evaluated into the
 * pad's position, rotation and scale with the node's flags; the call's result replaces the
 * flags. The first node (index 0) uses rate 1 instead of the pad's rate, gets its three
 * scale words copied back from the node when motion flag bit 2 is set, and its position is
 * copied to the pad's root pointer when that is not null. Then as lRhModel_SetupNoMotionRecursive
 * (push and build unless type 3, scale, matrix index, require_coordinate entry, child, pop),
 * with one addition before the entry search: unless node flag bit 15 is set the node's sphere
 * is copied to the pad's position (w = 1), the current matrix is scaled in a pushed copy and
 * RhView_CheckViewVolume(position, radius * rate * largest scale) is or-ed into the pad's
 * visible word. Returns the index after the chain.
 * The rate is chosen in a statement before the call and the callee's flags parameter is an
 * unsigned short placed before the frame (see the LIMITATION note in the header). */
int lRhModel_SetupFloatMotionRecursive(struct DotrSetupNode *node, int index, float *parent)
{
    unsigned char raw[0x20];
    struct DotrSetupPad *pad = DOTR_SETUP_PAD;
    float *scale = (float *)(((DotrSetupWord)raw + 15) & -16);
    unsigned int flags;
    int type;
    char *m;
    unsigned int i;
    struct DotrSetupCoordinate *e;
    char *item0;
    float radius, max, rate;

    do {
        RhMem_CopyMemory16(scale, parent, 1);
        flags = node->flags;
        RhMem_CopyMemory16(pad, node, 3);
        if (index < pad->count) {
            item0 = pad->item0;
            if (pad->motion_flags & 4) {
                item0 += index * 24;
            } else {
                item0 += index * 16;
            }
            if (index == 0) {
                rate = 1.0f;
            } else {
                rate = pad->rate;
            }
            flags = lRhModel_GetInterpolateMotionItem(item0, node->flags, pad->frame0, &pad->position.x, pad->rot, pad->scale,
                                                      rate);
            if (index == 0 && (pad->motion_flags & 4)) {
                RhMem_CopyMemory4((int *)pad, (int *)node, 3);
            }
        }
        if (index == 0) {
            if (pad->root != 0) {
                *pad->root = pad->position;
            }
        }
        type = flags & 3;
        index++;
        if (type != 3) {
            pad->position.x = pad->position.x * parent[0];
            pad->position.y = pad->position.y * parent[1];
            pad->position.z = pad->position.z * parent[2];
            RhGeomEx_PushMatrix();
            RhGeomEx_ScaleRotXYZTransMatrix(0, (flags & 2) ? 0 : pad->rot, (flags & 1) ? 0 : &pad->position);
        }
        if (!(flags & 4)) {
            scale[0] = scale[0] * pad->scale[0];
            scale[1] = scale[1] * pad->scale[1];
            scale[2] = scale[2] * pad->scale[2];
        }
        if (node->matrix != 0) {
            m = pad->matrices + (node->matrix << 4);
            RhGeomEx_GetMatrix(m);
            RhGeom_ScaleMatrix(m, scale);
        }
        if (!(node->flags & 0x8000)) {
            radius = node->sphere.w;
            max = scale[0];
            if (max < scale[1]) {
                max = scale[1];
            }
            if (max < scale[2]) {
                max = scale[2];
            }
            max = pad->rate * max;
            *(DotrSetupQuad *)&pad->position = *(DotrSetupQuad *)&node->sphere;
            pad->position.w = 1.0f;
            radius = radius * max;
            RhGeomEx_PushMatrix();
            RhGeom_ScaleMatrix(0, scale);
            pad->visible |= RhView_CheckViewVolume(&pad->position, radius);
            RhGeomEx_PopMatrix();
        }
        for (i = 0, e = require_coordinate; i < (unsigned int)require_coordinate_no[0]; e++, i++) {
            if (require_coordinate_index == e->index) {
                require_coordinate[i].flag = 1;
                RhGeom_GetMatrix(require_coordinate[i].matrix);
                break;
            }
        }
        require_coordinate_index++;
        if (node->child != 0) {
            index = lRhModel_SetupFloatMotionRecursive((struct DotrSetupNode *)(pad->nodes + (node->child & 0x0fffffff)), index,
                                                       scale);
        }
        if (type != 3) {
            RhGeomEx_PopMatrix();
        }
        if (node->next == 0) {
            return index;
        }
        node = (struct DotrSetupNode *)(pad->nodes + (node->next & 0x0fffffff));
    } while (1);
}
