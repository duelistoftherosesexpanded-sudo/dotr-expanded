#include "dotr/ee/texture_callbacks.h"

void lSzModel_LoadTexAnimFileCB(struct DotrTexCallbackRecord *req, int ok)
{
    int index = req->user;
    struct DotrTexCallbackEntry *pTextureEntry = DOTR_TEX_CALLBACK_LIST(index);
    char *text;
    index >>= 26;
    if (ok) {
        pTextureEntry += index;
        pTextureEntry->anim = req->data;
        text = pTextureEntry->anim;
        if (text) {
            text[req->size - 1] = 0;
            pTextureEntry->anim = func_001e9340_texcb(pTextureEntry->tex, text);
            SzMem_Free(text);
        }
    }
}
