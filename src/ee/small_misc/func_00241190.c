#include "dotr/ee/small_misc.h"

/* XORs 0x8078 halfwords with the second one taken as zero, XORs 0x4c6b,
 * stores the result in the second halfword and reports whether it equals
 * the value found there. The sum is declared before the saved value. */
int LkCheckData(unsigned short *words)
{
    unsigned short sum;
    unsigned short saved;
    unsigned short i;
    saved = words[1];
    sum = 0;
    words[1] = 0;
    for (i = 0; i < 0x8078; i++) {
        sum ^= words[i];
    }
    sum ^= 0x4c6b;
    words[1] = sum;
    if (sum == saved) {
        return 1;
    }
    return 0;
}
