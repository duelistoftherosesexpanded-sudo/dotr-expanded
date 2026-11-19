#include "dotr/ee/small_misc.h"

int mpegRestartDMA(void)
{
    viBufRestartDMA(data_003506c8);
    return 1;
}
