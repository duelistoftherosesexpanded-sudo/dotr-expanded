#include "dotr/ee/view_state.h"

/* Inlined three times below: divides x, y, z by the length and copies the fourth component. */
static inline void normalize_into(struct DotrViewVector *out, struct DotrViewVector *in)
{
    float length = func_001a6c60(in->x * in->x + in->y * in->y + in->z * in->z);
    out->x = in->x / length;
    out->y = in->y / length;
    out->z = in->z / length;
    out->unknown_w = in->unknown_w;
}
/* 0x20d5e0 RhGeom_CreateCameraMatrix2 (name from the Japanese build's symbols). The three
 * work vectors are the function's static locals vecX, vecY, vecZ there; vecZ is filled
 * with one 128-bit copy. */
void RhGeom_CreateCameraMatrix2(struct DotrViewMatrix *out, float *pos, float *dir, int roll)
{
    int ang[4];
    struct DotrViewMatrix rot;

    data_003269f0.x = dir[2];
    data_003269f0.y = 0.0f;
    data_003269f0.z = -dir[0];
    data_00326a00.x = -dir[1] * dir[0];
    data_00326a00.y = dir[0] * dir[0] + dir[2] * dir[2];
    data_00326a00.z = -dir[1] * dir[2];
    *(DotrViewWord128 *)&data_00326a10 = *(DotrViewWord128 *)dir;
    normalize_into(&data_003269f0, &data_003269f0);
    normalize_into(&data_00326a00, &data_00326a00);
    normalize_into(&data_00326a10, &data_00326a10);
    out->unknown_values[0] = -data_003269f0.x;
    out->unknown_values[1] = -data_003269f0.y;
    out->unknown_values[2] = -data_003269f0.z;
    out->unknown_values[3] = 0.0f;
    out->unknown_values[4] = -data_00326a00.x;
    out->unknown_values[5] = -data_00326a00.y;
    out->unknown_values[6] = -data_00326a00.z;
    out->unknown_values[7] = 0.0f;
    out->unknown_values[8] = data_00326a10.x;
    out->unknown_values[9] = data_00326a10.y;
    out->unknown_values[10] = data_00326a10.z;
    out->unknown_values[11] = 0.0f;
    out->unknown_values[12] = pos[0];
    out->unknown_values[13] = pos[1];
    out->unknown_values[14] = pos[2];
    out->unknown_values[15] = 1.0f;
    ang[0] = 0;
    ang[1] = 0;
    ang[2] = roll;
    RhGeom_GetRotXYZMatrix(rot.unknown_values, ang);
    RhGeom_MulMatrix(out, &rot);
    RhGeom_InvertMatrix(out, out);
}
