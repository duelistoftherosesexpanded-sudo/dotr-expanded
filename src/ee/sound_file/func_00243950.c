#include "dotr/ee/sound_file.h"

/* Returns 0 for n <= 0. Otherwise builds the four-word block {b, a, n, 0},
 * calls func_0017bb60(0), starts it through func_0017bca0(block, 1) and
 * calls func_0017bc80 on the result until it returns a negative value.
 * Returns n. */
int sendToIOP(int a, int b, int n)
{
    int block[4];
    int id;
    if (n <= 0) {
        return 0;
    }
    block[0] = b;
    block[1] = a;
    block[2] = n;
    block[3] = 0;
    func_0017bb60(0);
    id = func_0017bca0(block, 1);
    while (func_0017bc80(id) >= 0) {
    }
    return n;
}
