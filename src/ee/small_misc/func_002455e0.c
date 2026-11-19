#include "dotr/ee/small_misc.h"

int mpegTS(void *unused, long *out)
{
    long v[4];

    (void)unused;
    viBufGetTs(data_003506c8, v);
    out[1] = v[0];
    out[2] = v[1];
    return 1;
}
