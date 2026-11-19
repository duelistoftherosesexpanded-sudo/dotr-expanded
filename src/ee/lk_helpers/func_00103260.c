#include "dotr/ee/lk_helpers.h"

int LkInitTex2(int *handle, int id)
{
    *handle = SzText_LoadRequest(4, id);
    return 0;
}
