#include "dotr/ee/sound_file.h"

int SDFS_IsUsingNow(void)
{
    return s_iUsingNow.busy;
}
