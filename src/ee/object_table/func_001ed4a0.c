#include "dotr/ee/object_table.h"

unsigned char SzModel_Motion(int modelId)
{
    struct DotrObject *pModel;
    struct DotrObjectVector3 *a, *b;
    signed char *current, *previous, *next;
    float frameSpeed, last, step;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    a = &pModel->unknown_198[0];
    b = &pModel->unknown_198[1];
    current = &pModel->unknown_1e0[0];
    previous = &pModel->unknown_1e0[1];
    next = &pModel->unknown_1e0[3];
    frameSpeed = func_001a8760_int();
    if (pModel->modelId < 0) {
        return 0;
    }
    if (pModel->modelBaseData == 0) {
        return 0;
    }
    if (pModel->drawFlag.unknown_bit_3) {
        return 0;
    }
    if (!pModel->drawFlag.unknown_bit_1) {
        return 0;
    }
    if (pModel->stateFlags.unknown_bit_0) {
        return 1;
    }
    if (*previous >= 0 && pModel->animationDataPointers[*previous] != 0) {
        last = (float)pModel->animationFrameCountArray[*previous] - 1.0f;
        step = b->x * frameSpeed;
        if (pModel->animDir.unknown_bits_6_7 == 1) {
            if (b->y >= last) {
                b->y = 1.0f;
            } else {
                b->y += step;
                if (b->y > last) {
                    b->y = last;
                }
            }
        } else if (pModel->animDir.unknown_bits_6_7 == -1) {
            if (b->y <= 1.0f) {
                b->y = last;
            } else {
                b->y -= step;
                if (b->y < 1.0f) {
                    b->y = 1.0f;
                }
            }
        }
        a->z = (float)(unsigned)pModel->blendTickCurrent / (float)(unsigned)pModel->blendTickTotal;
        if (++pModel->blendTickCurrent >= pModel->blendTickTotal) {
            *previous = -1;
            a->z = 1.0f;
            pModel->blendTickCurrent = 0;
        }
        b->z = 1.0f - a->z;
    }
    if (pModel->animationDataPointers[*current] != 0) {
        last = (float)pModel->animationFrameCountArray[*current] - 1.0f;
        step = a->x * frameSpeed;
        if (pModel->animDir.unknown_bits_6_7 != 0) {
            switch (*current) {
            case 6:
                if (a->y >= 10.0f && pModel->motionDelay < 15) {
                    pModel->motionDelay++;
                    step = 0.0f;
                }
                break;
            case 3:
            case 4:
                if (a->y >= last) {
                    pModel->animDir.unknown_bits_6_7 = 0;
                }
                break;
            }
        }
        if (pModel->animDir.unknown_bits_6_7 == 1) {
            if (a->y >= last) {
                a->y = 1.0f;
                if (*next >= 0 && *current != *next) {
                    *b = *a;
                    *previous = -1;
                    *current = *next;
                    pModel->motionDelay = 0;
                }
            } else {
                a->y += step;
                if (a->y > last) {
                    a->y = last;
                }
            }
        } else if (pModel->animDir.unknown_bits_6_7 == -1) {
            if (a->y <= 1.0f) {
                a->y = last;
                if (*next >= 0 && *current != *next) {
                    *b = *a;
                    *previous = -1;
                    *current = *next;
                    a->y = (float)pModel->animationFrameCountArray[*current] - 1.0f;
                    pModel->motionDelay = 0;
                }
            } else {
                a->y -= step;
                if (a->y < 1.0f) {
                    a->y = 1.0f;
                }
            }
        }
    }
    if (pModel->drawFlag.unknown_bit_4) {
        if (pModel->deformRatio < 32) {
            pModel->deformRatio++;
        }
    } else if (pModel->deformRatio > 0) {
        pModel->deformRatio--;
    }
    return 1;
}
