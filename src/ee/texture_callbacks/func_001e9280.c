#include "dotr/ee/texture_callbacks.h"

void lSzModel_LoadTextureFileCB(struct DotrTexCallbackRecord *req, int ok)
{
    int index = req->user;
    struct DotrTexCallbackEntry *pTextureEntry = DOTR_TEX_CALLBACK_LIST(index);
    index >>= 26;
    if (ok) {
        pTextureEntry += index;
        pTextureEntry->tex = req->data;
        RhText_BindAddress(pTextureEntry->tex);
    }
}
