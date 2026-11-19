#include "dotr/ee/draw_state.h"
#include "dotr/ee/draw_quad_block.h"
#include "dotr/ee/block_table.h"

struct DotrDrawTag *RhVu1Mac_SetStraightLightToVu1_Ex(struct DotrDrawTag *tag, struct DotrBlockTriple *light,
                                                      struct DotrBlockVector4 *dir, int slot)
{
    char buf[64 + 15];
    float *m = (float *)(((DotrDrawWord)buf + 15) & -16);

    if (slot >= 0 && slot < 3) {
        if (light == 0 || dir == 0) {
            return tag;
        }
    }
    switch (slot) {
    case 0:
        gRhVu1Mac_matCol[0] = light->unknown_00[0].y;
        gRhVu1Mac_matCol[1] = light->unknown_00[0].z;
        gRhVu1Mac_matCol[2] = light->unknown_00[0].unknown_w;
        gRhVu1Mac_matDir[0] = dir->x;
        gRhVu1Mac_matDir[4] = dir->y;
        gRhVu1Mac_matDir[8] = dir->z;
        break;
    case 1:
        gRhVu1Mac_matCol[4] = light->unknown_00[0].y;
        gRhVu1Mac_matCol[5] = light->unknown_00[0].z;
        gRhVu1Mac_matCol[6] = light->unknown_00[0].unknown_w;
        gRhVu1Mac_matDir[1] = dir->x;
        gRhVu1Mac_matDir[5] = dir->y;
        gRhVu1Mac_matDir[9] = dir->z;
        break;
    case 2:
        gRhVu1Mac_matCol[8] = light->unknown_00[0].y;
        gRhVu1Mac_matCol[9] = light->unknown_00[0].z;
        gRhVu1Mac_matCol[10] = light->unknown_00[0].unknown_w;
        gRhVu1Mac_matDir[2] = dir->x;
        gRhVu1Mac_matDir[6] = dir->y;
        gRhVu1Mac_matDir[10] = dir->z;
        break;
    }
    if (tag != 0) {
        RhMem_CopyMemory16(m, gRhVu1Mac_matCol, 4);
        RhGeom_MulMatrix(m, gRhVu1Mac_matDir);
        tag->unknown_00 = 0x10000004;
        tag->unknown_04 = 0;
        tag->unknown_08 = 0;
        tag->unknown_0c = 0x6c04003f;
        RhMem_CopyMemory16(tag + 1, m, 4);
        tag = (struct DotrDrawTag *)((char *)(tag + 1) + 64);
    } else if (slot < 0) {
        func_00198948(gRhVu1Mac_matDir, 0, 64);
        func_00198948(gRhVu1Mac_matCol, 0, 64);
    }
    return tag;
}
