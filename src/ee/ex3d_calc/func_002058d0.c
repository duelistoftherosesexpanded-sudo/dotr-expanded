#include "dotr/ee/ex3d_calc.h"

int Ex3d_RotTransPersAutoCalcQ(void *src DOTR_EX3D_CALC_ALIGN16, float *vertices, DotrEx3dCalcWord matrix,
                               DotrEx3dCalcWord packet, DotrEx3dCalcWord average) {
    extern long long func_001a23b0(float);
    float local_matrix[16];
    float *input = src;
    unsigned long long *type_word = &((struct DotrEx3dCalcPacket *)packet)->types;
    float *attribute1 = NULL;
    float *attribute2 = NULL;
    unsigned char *entry = ((struct DotrEx3dCalcPacket *)packet)->data;
    unsigned long long types = *type_word;
    unsigned short count = ((struct DotrEx3dCalcPacket *)packet)->count;
    float total = 0.0f;
    int scale_attributes = 0;
    int vertex_count = 0;
    unsigned short flags = ((struct DotrEx3dCalcPacket *)packet)->flags;
    int i;

    if (!(flags & 0x800) && (flags & 0x20) && (flags & 0x10)) {
        scale_attributes = 1;
    }
    if (matrix == 0) {
        RhView_GetWorldScreenMatrix(local_matrix);
        matrix = (DotrEx3dCalcWord)local_matrix;
    }
    for (i = 0; i < (unsigned short)count; i++) {
        if ((types & 0xF) == 4) {
            input[3] = 1.0f;
            input += 4;
            vertex_count++;
        }
        types >>= 4;
    }
    RhGeom_PushMatrix(matrix);
    RhGeom_MulMatrixVectorArray((DotrEx3dCalcWord)vertices, 0, (DotrEx3dCalcWord)src, vertex_count);
    RhGeom_PopMatrix();
    types = *type_word;
    for (i = 0; i < (unsigned short)count; i++, entry += 8, types >>= 4) {
        int kind = types & 0xF;
        switch (kind) {
        case 1:
            attribute1 = (float *)entry;
            break;
        case 2:
            attribute2 = (float *)entry;
            break;
        case 4: {
            float reciprocal = 1.0f / vertices[3];
            float x, y, z;

            if (scale_attributes && attribute1 != NULL && attribute2 != NULL) {
                attribute1[1] = reciprocal;
                attribute1 = NULL;
                attribute2[0] *= reciprocal;
                attribute2[1] *= reciprocal;
                attribute2 = NULL;
            }
            vertices[0] *= reciprocal;
            vertices[1] *= reciprocal;
            z = vertices[2] * reciprocal;
            vertices[2] = z;
            if (z < 1.0f || !(z <= 16777215.0f) ||
                (x = vertices[0]) < 0.0f || !(x <= 4095.0f) ||
                (y = vertices[1]) < 0.0f || !(y <= 4095.0f)) {
                return 0;
            }
            *(short *)(entry + 0) = func_001a23b0(16.0f * x);
            *(short *)(entry + 2) = func_001a23b0(16.0f * vertices[1]);
            ((struct DotrEx3dCalcPacked *)entry)->z = func_001a23b0(vertices[2]);
            total += vertices[3];
            vertices += 4;
            break;
        }
        }
    }
    if (average != 0) {
        *(float *)average = total / (float)vertex_count;
    }
    return 1;
}
