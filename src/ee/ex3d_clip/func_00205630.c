#include "dotr/ee/ex3d_clip.h"

/* Every type-4 entry gets w = 1 and is transformed in one call into `work`; each result is
 * divided by its w, rejected outside the screen and depth range (the function then returns
 * 0 with the earlier entries already stored) and written to its packet word. `avg`, when
 * given, receives the mean of the w values. */
/* The locals are declared without initialisers, `p` before `types`: with initialised
 * declarations in the order of use the two swap their saved registers. */
int Ex3d_RotTransPersAuto(struct DotrEx3dAutoIn *in, float *work, float *matrix, struct DotrEx3dAutoPacket *packet,
                          float *avg)
{
    float local[16];
    struct DotrEx3dAutoIn *p;
    unsigned short n;
    struct DotrEx3dAutoXyz *xyz;
    float sum;
    unsigned long *ptypes;
    unsigned long types;
    int count;
    int i;
    unsigned long types2;
    int j;
    float w;

    types = packet->types;
    n = packet->count;
    xyz = packet->xyz;
    sum = 0.0f;
    ptypes = &packet->types;
    p = in;
    count = 0;
    if (matrix == 0) {
        RhView_GetWorldScreenMatrix(local);
        matrix = local;
    }
    for (i = 0; i < n; i++, types >>= 4) {
        if ((types & 0xf) == 4) {
            p->w = 0x3f800000;
            p++;
            count++;
        }
    }
    RhGeom_PushMatrix(matrix);
    RhGeom_MulMatrixVectorArray(work, 0, in, count);
    RhGeom_PopMatrix();
    types2 = *ptypes;
    for (j = 0; j < n; j++, xyz++, types2 >>= 4) {
        if ((types2 & 0xf) == 4) {
            w = 1.0f / work[3];
            work[0] *= w;
            work[1] *= w;
            work[2] *= w;
            if (work[2] < 1.0f || work[2] > 16777215.0f || work[0] < 0.0f || work[0] > 4095.0f || work[1] < 0.0f ||
                work[1] > 4095.0f) {
                return 0;
            }
            xyz->x = func_001a23b0(work[0] * 16.0f);
            xyz->y = func_001a23b0(work[1] * 16.0f);
            ((struct DotrEx3dAutoBits *)xyz)->z = (unsigned)func_001a23b0(work[2]);
            sum += work[3];
            work += 4;
        }
    }
    if (avg != 0) {
        *avg = sum / (float)count;
    }
    return 1;
}
