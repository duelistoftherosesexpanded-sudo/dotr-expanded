#include "dotr/ee/fx_calc.h"

void ShFx_SetUV(int *out, int index, unsigned int kind)
{
    int w, h, cols, rows;

    switch (kind) {
    case 0: w = h = 32; cols = 4; rows = 2; break;
    case 1: w = h = 32; cols = 2; rows = 4; break;
    case 2: w = h = 32; cols = rows = 4; break;
    case 3: w = 32; h = 64; cols = 8; rows = 1; break;
    case 4: w = h = 64; cols = 2; rows = 1; break;
    case 5: w = h = 64; cols = rows = 2; break;
    case 9: w = h = 64; cols = rows = 1; break;
    case 6: w = 32; h = 128; cols = rows = 1; break;
    case 7: w = 128; h = 64; cols = rows = 1; break;
    case 8: w = h = 128; cols = rows = 1; break;
    }
    if (cols * rows - 1 < index) {
        out[5] = 0;
        out[4] = 0;
        out[3] = 0;
        out[2] = 0;
    } else {
        out[2] = (index % cols) * w;
        out[3] = out[2] + w;
        out[4] = (index / cols) * h;
        out[5] = out[4] + h;
    }
}
