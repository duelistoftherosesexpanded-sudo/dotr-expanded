#include "dotr/ee/draw_state.h"

int RhBase_SetVSyncCallback(int value)
{
    data_003261e4[0] = value;
    return 1;
}
