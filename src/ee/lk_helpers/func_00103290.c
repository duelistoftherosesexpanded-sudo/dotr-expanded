#include "dotr/ee/lk_helpers.h"

int LkInitTexBtl(int *handle, int id)
{
    *handle = SzText_LoadRequest(5, id);
    return 0;
}
