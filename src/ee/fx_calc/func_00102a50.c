#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"

/* ShFx_Vib2P (0x102a50): fills count + 1 points from start to end with random offsets.
 * The step is a float array, the output points are walked by a float pointer (three floats a
 * point), shifts go through temporaries and the conversions are explicit func_001a22c0 calls. */
void ShFx_Vib2P(struct DotrFxVib *p)
{
    float steps[3];
    float len;
    float range;
    float *out;
    float *last;
    int i;
    int half;
    int r0;
    int r1;
    int r2;

    steps[0] = (p->end.x - p->start.x) / (float)p->count;
    steps[1] = (p->end.y - p->start.y) / (float)p->count;
    steps[2] = (p->end.z - p->start.z) / (float)p->count;
    len = p->amplitude * RhGeom_GetVectorScalor((struct DotrViewVector *)steps);
    out = (float *)p->out;
    i = 1;
    *(struct DotrFxPoint *)out = p->start;
    range = 200.0f * len;
    last = out + (p->count * 3);
    out += 3;
    *(struct DotrFxPoint *)last = p->end;
    half = func_001a22c0(200.0f * len) >> 1;
    for (; i < p->count;) {
        r0 = func_0019a5c8() >> 4;
        out[0] = p->start.x + (steps[0] * (float)i) + ((float)((r0 % func_001a22c0(range)) - half) / 100.0f);
        r1 = func_0019a5c8() >> 4;
        out[1] = p->start.y + (steps[1] * (float)i) + ((float)((r1 % func_001a22c0(range)) - half) / 100.0f);
        r2 = func_0019a5c8() >> 4;
        out[2] = p->start.z + (steps[2] * (float)i++) + ((float)((r2 % func_001a22c0(range)) - half) / 100.0f);
        out += 3;
    }
}
