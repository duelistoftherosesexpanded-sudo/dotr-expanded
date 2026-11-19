#include "dotr/ee/model_setup.h"

/* Walks a node and its `next` chain; pad is the scratchpad work area (only nodes and matrices
 * are read). Per node: the parent's scale (16 bytes) is copied to a
 * 16-aligned local; unless the node's type (flags & 3) is 3, a matrix is pushed and built
 * from the node's rotation (null with flag bit 1) and its translation multiplied by the
 * parent's scale (null with flag bit 0); without flag bit 2 the local scale is multiplied by
 * the node's; a non-zero matrix index takes the current matrix into that 16-byte unit of the
 * model's matrix block and scales it by the local scale. The first require_coordinate entry
 * whose index equals require_coordinate_index gets flag 1 and the current matrix, the index counts up,
 * the child (if any) is walked with the local scale as its parent, and the pushed matrix is
 * popped.
 * The type test is written out twice: a `type` variable takes the register the original
 * gives to the flags. */
void lRhModel_SetupNoMotionRecursive(struct DotrSetupPad *pad, struct DotrSetupNode *node, float *parent)
{
    unsigned char raw[0x20];
    float *scale = (float *)(((DotrSetupWord)raw + 15) & -16);
    unsigned short flags;
    char *m;
    unsigned int i;
    struct DotrSetupCoordinate *e;

    do {
        RhMem_CopyMemory16(scale, parent, 1);
        flags = node->flags;
        if ((flags & 3) != 3) {
            struct DotrSetupVec3 t = data_00295080;
            t.x = node->trans[0] * parent[0];
            t.y = node->trans[1] * parent[1];
            t.z = node->trans[2] * parent[2];
            RhGeomEx_PushMatrix();
            RhGeomEx_ScaleRotXYZTransMatrix(0, (flags & 2) ? 0 : node->rot, (flags & 1) ? 0 : &t);
        }
        if (!(flags & 4)) {
            scale[0] = scale[0] * node->scale[0];
            scale[1] = scale[1] * node->scale[1];
            scale[2] = scale[2] * node->scale[2];
        }
        if (node->matrix != 0) {
            m = pad->matrices + (node->matrix << 4);
            RhGeomEx_GetMatrix(m);
            RhGeom_ScaleMatrix(m, scale);
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
            lRhModel_SetupNoMotionRecursive(pad, (struct DotrSetupNode *)(pad->nodes + (node->child & 0x0fffffff)),
                                            scale);
        }
        if ((flags & 3) != 3) {
            RhGeomEx_PopMatrix();
        }
        if (node->next == 0) {
            break;
        }
        node = (struct DotrSetupNode *)(pad->nodes + (node->next & 0x0fffffff));
    } while (1);
}
