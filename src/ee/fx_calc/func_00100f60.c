#include "dotr/ee/fx_calc.h"

/* The two temporaries are one two-element local: the original keeps the
 * address of the second in a saved register and recomputes the first.
 * MATCHING DEVICE: the first scale call names the first element as the
 * whole array cast to float *; written as v[0] the constant load moves
 * ahead of the address. */
void GGet_TriangleCenter(float *out, float *a, float *b, float *c)
{
    float v[2][4];

    func_00187b00(v[0], a, b);
    func_00187b48((float *)v, (float *)v, 0.5f);
    func_00187b18(v[1], c, v[0]);
    func_00187b48(v[1], v[1], 0.33333334f);
    func_00187b00(out, v[1], v[0]);
}
