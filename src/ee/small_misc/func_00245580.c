#include "dotr/ee/small_misc.h"

int mpegStopDMA(void)
{
    viBufStopDMA(data_003506c8);
    return 1;
}
