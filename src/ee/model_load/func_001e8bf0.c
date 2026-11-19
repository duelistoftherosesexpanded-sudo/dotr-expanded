#include "dotr/ee/model_load.h"

void lSzModel_LoadCB(int index)
{
    struct DotrModelLoadObject *pModel;
    struct DotrModelLoadNode *node;
    int i;
    index &= 0x3ff;
    if (index < 0 || index >= 84) return;
    pModel = gSzModel_ahModel[index];
    if (pModel == 0) return;
    pModel->loading = 0;
    if (pModel->base == 0) return;
    pModel->index = index;
    /* MATCHING DEVICE: three separate tests and a goto; `a || b || c` is merged into a
     * range test by the compiler, which the original does not have. */
    if (pModel->kind != 4) if (pModel->kind != 5) if (pModel->kind != 6) goto skip;
    {
        pModel->loaded_id = pModel->id;
        pModel->look = gSzDuel_Camera[AlternateCardIds[pModel->id]] & 0x3fff;
    }
skip:
    node = (struct DotrModelLoadNode *)(pModel->base + pModel->desc->node_offset);
    pModel->node = node;
    pModel->level = pModel->desc->flags >> 6;
    lSzModel_GetGNode(index, -1, &pModel->desc->gnode_flag);
    pModel->pos[0] = node->x;
    pModel->pos[1] = node->y;
    pModel->pos[2] = node->z;
    pModel->pos[3] = 1.0f;
    pModel->radius = node->radius;
    pModel->scale = 40.0f / pModel->radius;
    if (pModel->scale > 1.0f) pModel->scale = 1.0f;
    if (pModel->scale < 0.3f) pModel->scale = 0.3f;
    if (pModel->heads[pModel->motion[0]] == 0) {
        pModel->motion[0] = 0;
        for (i = 0; i < 8; i++) {
            if (pModel->heads[i] != 0) {
                pModel->motion[0] = i;
                break;
            }
        }
    }
    for (i = 1; i < 4; i++) {
        if (pModel->motion[i] >= 0 && pModel->heads[pModel->motion[i]] == 0) {
            pModel->motion[i] = pModel->motion[0];
        }
    }
    if (pModel->kind != 4) if (pModel->kind != 5) if (pModel->kind != 6) return;
    {
        SzFile_GetEnv(data_002cfa17);
        SzFile_GetEnv(data_002cfa2a);
        ((struct DotrModelLoadHeader *)pModel->base)->flags &= ~0x400;
        SzEffect_LoadModule(1, pModel->id, 0);
    }
}
