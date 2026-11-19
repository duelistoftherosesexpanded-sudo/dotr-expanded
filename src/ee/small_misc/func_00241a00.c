#include "dotr/ee/small_misc.h"

/* Returns color unchanged when its low 15 bits are zero. Otherwise passes
 * the three 5-bit components to SzColor_RGB2SEP with e and 31 and repacks the
 * three result bytes, keeping bit 15 of color. */
int SzColor_CLUT2SEP(int color, int e)
{
    unsigned short c = color;
    unsigned char out[4];
    if ((c & 0xffff7fff) == 0) {
        return color;
    }
    SzColor_RGB2SEP(out, c & 0x1f, (c >> 5) & 0x1f, (c >> 10) & 0x1f, e, 31);
    return (color & 0x8000) | (((out[2] & 0x1f) << 10) | ((out[0] & 0x1f) | ((out[1] & 0x1f) << 5)));
}
