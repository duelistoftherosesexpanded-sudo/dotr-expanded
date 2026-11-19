#include "dotr/ee/model_func_draw.h"

/* 0x1ee6c0. Compiled with #pragma global_optimizer off, set directly before the function and
 * reset after it. The absolute-value helper precedes it; the model table is the view
 * gSzModel_ahModel_funcdraw. */
#ifndef __MWERKS__
/* Host build only: the code passes addresses as 32-bit words, which the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Woverflow" /* one-bit signed fields set to 1 */
#endif

static inline float abs_001EE6C0(float x)
{
    return (x >= 0.0f) ? x : -x;
}

#ifdef __MWERKS__
#pragma global_optimizer off
#endif
int SzModel_DrawWithFunc_Pipe(int arg0, int modelId, int translationRelated, int rotationRelated, int scaleRelated,
                 int arg5, int zbufferPtr, void (*preRenderCallback)(void), int postRenderCallback)
{
    DotrFuncDrawMatrix matrix;
    DotrFuncDrawVector origin;
    DotrFuncDrawVector offset;
    DotrFuncDrawTransform transform;
    DotrFuncDrawEffect effect0;
    DotrFuncDrawModel model;
    DotrFuncDrawVector position;
    DotrFuncDrawEffect effect1;
    DotrFuncDrawPrimitive primitive;
    int overlay;
    int enabled;
    DotrFuncDrawObject *pModel;
    float scale;
    float scaledY;
    unsigned int depth;
    void *packet;
    int k;
    int draw;
    int i;
    unsigned char *p;
    DotrFuncDrawFlags *fl;
    DotrFuncDrawFlags *f0;
    float *sx;
    float *sy;
    float *sz;
    DotrFuncDrawPrimitiveFlags *pflags;
    DotrFuncDrawXyzf *pv0;
    DotrFuncDrawXyzf *pv1;
    DotrFuncDrawXyzf *sv0;
    DotrFuncDrawXyzf *sv1;
    DotrFuncDrawSprite sprite __attribute__((aligned(8)));

    origin = data_00292fd0;
    offset = data_00292fe0;
    enabled = 1;
    overlay = 0;
    draw = 0;
    if (!(SzEffect_IsUse2(0, -1) & 1) && !(SzEffect_IsUse2(1, -1) & 1) &&
        !(SzEffect_IsUse2(2, -1) & 1)) {
        enabled = 0;
    }
    if (arg5) {
        DotrFuncDrawHeader *header = (DotrFuncDrawHeader *)arg5;
        header->type = 7;
        header->count = 0;
        header->tag = 0;
        header->pad04 = 0;
    }
    if (modelId < 0 || modelId >= 0x54) {
        return arg0;
    }
    pModel = gSzModel_ahModel_funcdraw[modelId];
    if (pModel == NULL) {
        return arg0;
    }
    if (*(signed char *)((unsigned char *)pModel + 0x1F2) < 0) {
        return arg0;
    }
    if (*(int *)((unsigned char *)pModel + 0x138) == 0) {
        return arg0;
    }
    if (((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b3) {
        return arg0;
    }
    if (!((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b1) {
        return arg0;
    }
    if (arg5 == 0) {
        int type;
        if (pModel->pad040[0xEF] == 0) {
            return arg0;
        }
        type = *(signed char *)((unsigned char *)pModel + 0x1F3);
        if (type != 4 && type != 5 && type != 6) {
            if (abs_001EE6C0(*(float *)((unsigned char *)pModel + 0x128)) <= 0.000001f) {
                return arg0;
            }
            if (abs_001EE6C0(*(float *)((unsigned char *)pModel + 0x120)) <= 0.000001f &&
                abs_001EE6C0(*(float *)((unsigned char *)pModel + 0x124)) <= 0.000001f) {
                return arg0;
            }
        }
    }
    RhMem_CopyMemory16(&transform, pModel, 4);
    if (0 != translationRelated) {
        transform.position.f[0] += ((float *)translationRelated)[0];
        transform.position.f[1] += ((float *)translationRelated)[1];
        transform.position.f[2] += ((float *)translationRelated)[2];
    }
    if (rotationRelated != 0) {
        transform.rotation[0] += ((int *)rotationRelated)[0];
        transform.rotation[1] += ((int *)rotationRelated)[1];
        transform.rotation[2] += ((int *)rotationRelated)[2];
    }
    if (0 != scaleRelated) {
        transform.scale.f[0] *= ((float *)scaleRelated)[0];
        transform.scale.f[1] *= ((float *)scaleRelated)[1];
        transform.scale.f[2] *= ((float *)scaleRelated)[2];
    }
    sx = &transform.scale.f[0];
    sy = &transform.scale.f[1];
    sz = &transform.scale.f[2];
    scale = *sx;
    scaledY = 1.0638298f * *sy;
    scale = (scale > scaledY) ? scale : scaledY;
    if (scale < *sz) {
        scale = *sz;
    }
    if (abs_001EE6C0(scale) <= 0.000001f) {
        return arg0;
    }
    if (((DotrFuncDrawTextureFlags *)((unsigned char *)pModel + 0x1F8))->overlay && pModel->pad040[0xF3]) {
        overlay = 1;
    }
    if (arg5 || overlay || gSzModel_nBlurMode_word || ((DotrFuncDrawTextureFlags *)((unsigned char *)pModel + 0x1F8))->base ||
        pModel->pad040[0xEE] != 0x80 || pModel->pad040[0xED] != 0x80 ||
        pModel->pad040[0xEC] != 0x80 || pModel->pad040[0xEF] != 0x80) {
        if (enabled) {
            draw = RhBase_AssignRenderFrame(0, 2, 0x32);
        } else {
            draw = RhBase_AssignRenderFrame(0, 0xA, 0x31);
        }
    }
    if (draw) {
        if (gSzModel_nBlurMode_word != 1) {
            data_0031bb10 = 0;
        }
        arg0 = (int)RhBase_EnableRenderFrame((void *)arg0);
        arg0 = (int)RhBase_ClearRenderFrame((unsigned char *)arg0, 0);
        if (enabled && RhView_PushStack()) {
            RhView_GetCurView(&effect0);
            RhView_SetClipZ(1.0f, 1024.0f);
            RhView_Modify(&effect0);
            arg0 = RhView_SendViewParameters(arg0);
        }
        if (preRenderCallback) {
            arg0 = ((int (*)(int))preRenderCallback)(arg0);
        }
    }
    arg0 = SzView_SetLight_Pipe((int)SzView_SetFog_Pipe((void *)arg0, 0, *(signed char *)((unsigned char *)pModel + 0x1F3),
                        *(float *)((unsigned char *)pModel + 0x128)), 0, *(signed char *)((unsigned char *)pModel + 0x1F3));
    if (data_0031bb10 != *(int *)((unsigned char *)pModel + 0x138)) {
        int result;
        arg0 = SzModel_LoadTexture_Pipe(arg0, modelId, *(void **)((unsigned char *)pModel + 0x138), &result);
        data_0031bb10 = *(int *)((unsigned char *)pModel + 0x138);
    }
    {
        signed char *lv = &pModel->lv[0];
        signed char *lvNext = &pModel->lv[1];
        float *slotpos = (float *)&pModel->slots[8];
        float *slotpos2 = (float *)&pModel->slots[11];
        RhMem_FillMemory4(&model, 0, 0x18);
        model.model = *(int *)((unsigned char *)pModel + 0x138);
        if (*(DotrFuncDrawSlot **)((*lv << 2) + (unsigned int)pModel + 0x178) != NULL) {
            ((DotrFuncDrawFlags *)&model.flags0)->b2 = 1;
            ((DotrFuncDrawFlags *)&model.flags0)->b4 = 1;
            model.slot0 = *(DotrFuncDrawSlot **)((*lv << 2) + (unsigned int)pModel + 0x178);
            model.value0 = slotpos[1];
            model.value2 = slotpos[2];
            if (*lvNext >= 0 && *lvNext != *lv && *(DotrFuncDrawSlot **)((*lvNext << 2) + (unsigned int)pModel + 0x178) != NULL) {
                ((DotrFuncDrawFlags *)&model.flags0)->b3 = 1;
                model.slot1 = *(DotrFuncDrawSlot **)((*lvNext << 2) + (unsigned int)pModel + 0x178);
                model.value1 = slotpos2[1];
                model.value3 = slotpos2[2];
            }
        }
    }
    if (((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b7) {
        model.flags0.b1 = SzView_IsFogZ(0, *(signed char *)((unsigned char *)pModel + 0x1F3));
        model.flags1.b2 = SzView_IsFogY(0, *(signed char *)((unsigned char *)pModel + 0x1F3));
    }
    fl = &model.flags1;
    if (!fl->b2 && SzView_InqPointLight(0, *(signed char *)((unsigned char *)pModel + 0x1F3))) {
        model.flags0.b0 = 1;
    }
    fl->b3 = 1;
    f0 = (DotrFuncDrawFlags *)&model.flags0;
    f0->b7 = 1;
    model.values[0] = *(float *)((unsigned char *)pModel + 0x120) * *(float *)((unsigned char *)pModel + 0x128);
    model.values[1] = *(float *)((unsigned char *)pModel + 0x124) * *(float *)((unsigned char *)pModel + 0x128);
    model.values[2] = 1.0f;
    if (*(signed char *)((unsigned char *)pModel + 0x1F3) == 4 || *(signed char *)((unsigned char *)pModel + 0x1F3) == 5 ||
        *(signed char *)((unsigned char *)pModel + 0x1F3) == 6) {
        GetAnyCoordinateInit();
        if (*(unsigned char *)((unsigned char *)pModel + 0x1FE) &&
            ((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b6) {
            BookingGetAnyCoordinate(1);
            if (0 == gSzModel_nBlurMode_word) {
                if (*(unsigned char *)((unsigned char *)pModel + 0x1F0) < 0xFF) {
                    BookingGetAnyCoordinate(*(unsigned char *)((unsigned char *)pModel + 0x1F0));
                }
                *(unsigned short *)((unsigned char *)pModel + 0x1F0) &= 0x7FFF;
                i = 0;
                p = (unsigned char *)pModel;
                do {
                    if (*(unsigned char *)(p + 0x1B0) < 0xFF) {
                        BookingGetAnyCoordinate(*(unsigned char *)(0x1B0 + p));
                    }
                    *(unsigned short *)(p + 0x1B0) &= 0x7FFF;
                    p += 2;
                    i++;
                } while (i < 8);
            }
            if (((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b5) {
                RhGeom_PushMatrix(0);
                *(struct DotrFuncDrawVector4f *)&position = *(struct DotrFuncDrawVector4f *)((unsigned char *)pModel + 0x80);
                RhGeom_MulMatrixVector(&position, NULL, &position);
                fl->b4 = 1;
                RhGeom_PopMatrix();
            }
            ((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b5 = 0;
        }
    }
    RhGeom_PushMatrix(0);
    *(float *)(*(unsigned char **)((unsigned char *)pModel + 0x134) + 0x3C) =
        *(float *)((unsigned char *)pModel + 0x1E4) * scale;
    if (*(signed char *)((unsigned char *)pModel + 0x1F3) == 4 || *(signed char *)((unsigned char *)pModel + 0x1F3) == 5 || *(signed char *)((unsigned char *)pModel + 0x1F3) == 6) {
        int bounds;
        bounds = lSzModel_GetGNode(modelId, 1, NULL);
        if (bounds) {
            float denominator;
            float *o1 = &origin.f[1];
            float *o2 = &origin.f[2];
            denominator = *(float *)(*(unsigned char **)((unsigned char *)pModel + 0x13C) + 0x54);
            origin.f[0] = *(float *)((unsigned char *)bounds + 0x20) / denominator;
            *o1 = *(float *)((unsigned char *)bounds + 0x24) / denominator;
            *o2 = *(float *)((unsigned char *)bounds + 0x28) / denominator;
            offset.f[0] = origin.f[0];
            offset.f[1] = *o1;
            offset.f[2] = *o2;
            origin.f[0] *= -1.0f;
            *o1 *= -1.0f;
            *o2 *= -1.0f;
        }
        if (*(unsigned char *)((unsigned char *)pModel + 0x1FF)) {
            float blend = (float)(unsigned int)*(unsigned char *)((unsigned char *)pModel + 0x1FF) / 32.0f;
            float factor;
            float remainder;
            factor = 1.0f + (*(float *)((unsigned char *)pModel + 0x1E8) - 1.0f) * blend;
            *(float *)(*(unsigned char **)((unsigned char *)pModel + 0x134) + 0x3C) *= factor;
            remainder = 1.0f - blend;
            *sx *= factor;
            *sy *= factor;
            *sz *= factor;
            offset.f[0] *= remainder;
            offset.f[2] *= remainder;
        }
        RhGeom_ScaleRotXYZTransMatrix(0, 0, (int *)&transform.extra, transform.position.f);
        RhGeom_GetScaleRotXYZTransMatrix(&matrix, (float *)&transform.scale, transform.rotation, offset.f);
        RhGeom_MulMatrix(0, &matrix);
        RhGeom_GetTransMatrix(&matrix, origin.f);
        RhGeom_MulMatrix(0, &matrix);
        packet = RhModel_Draw((void *)arg0, &model);
        if (*(unsigned char *)((unsigned char *)pModel + 0x1FE) &&
            ((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b6) {
            if (f0->b2 && GetAnyCoordinate(1, &matrix)) {
                *(float *)((unsigned char *)pModel + 0x80) = matrix.f[12];
                *(float *)((unsigned char *)pModel + 0x84) = matrix.f[13];
                *(float *)((unsigned char *)pModel + 0x88) = matrix.f[14];
                *(float *)((unsigned char *)pModel + 0x8C) = 1.0f;
            } else {
                RhView_GetWorldViewMatrix(&matrix);
                RhGeom_InvertMatrix(&matrix, &matrix);
                RhGeom_MulMatrixRev(0, (int)&matrix);
                RhGeom_MulMatrixVector(&pModel->pad040[0x40], NULL, &pModel->pad040[0x50]);
            }
            if (!gSzModel_nBlurMode_word && f0->b2) {
                if (*(unsigned char *)((unsigned char *)pModel + 0x1F0) < 0xFF &&
                    GetAnyCoordinate(*(unsigned char *)((unsigned char *)pModel + 0x1F0), pModel->pad040)) {
                    *(unsigned short *)((unsigned char *)pModel + 0x1F0) |= 0x8000;
                }
                k = 0;
                i = (int)pModel;
                p = (unsigned char *)pModel;
                do {
                    if (*(unsigned char *)(i + 0x1B0) < 0xFF &&
                        GetAnyCoordinate(*(unsigned char *)(i + 0x1B0), &matrix)) {
                        *(unsigned short *)(0x1B0 + i) |= 0x8000;
                        *(float *)(p + 0xA0) = matrix.f[12];
                        *(float *)(p + 0xA4) = matrix.f[13];
                        *(float *)(0xA8 + p) = matrix.f[14];
                        *(float *)(0xAC + p) = 1.0f;
                    }
                    k++;
                    i += 2;
                    p += 0x10;
                } while (k < 8);
            }
            ((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b5 = 1;
        }
    } else {
        RhGeom_ScaleRotXYZTransMatrix(0, 0, (int *)&transform.extra, transform.position.f);
        RhGeom_GetScaleRotXYZTransMatrix(&matrix, (float *)&transform.scale, transform.rotation, NULL);
        RhGeom_MulMatrix(0, &matrix);
        packet = RhModel_Draw((void *)arg0, &model);
        if (((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b6) {
            RhView_GetWorldViewMatrix(&matrix);
            RhGeom_InvertMatrix(&matrix, &matrix);
            RhGeom_MulMatrixRev(0, (int)&matrix);
            RhGeom_MulMatrixVector(&pModel->pad040[0x40], NULL, &pModel->pad040[0x50]);
            ((DotrFuncDrawObjectFlags *)((unsigned char *)pModel + 0x20C))->b5 = 1;
        }
    }
    RhGeom_PopMatrix();
    *(int *)&model.depth = depth = Ex3d_GetSampleZ(&pModel->pad040[0x40], 0.0f);
    if (zbufferPtr && !(zbufferPtr & 0x80000000)) {
        *(int *)zbufferPtr = *(int *)&model.depth;
        if (draw == 0 && *(int *)(*(unsigned char **)((unsigned char *)pModel + 0x13C) + 0x1C) == 0) {
            *(int *)zbufferPtr = 0;
        }
    }
    if (draw != 0) {
        if (0x80000000 & zbufferPtr) {
            depth = zbufferPtr & 0x7FFFFFFF;
        }
        if (overlay) {
            ((DotrFuncDrawHeader *)&sprite.header)->type = 6;
            ((DotrFuncDrawHeader *)&sprite.header)->count = 3;
            ((DotrFuncDrawHeader *)&sprite.header)->tag = 0x441;
            ((DotrFuncDrawHeader *)&sprite.header)->pad04 = 0;
            ExAssignAlpha(&sprite, ExGetAlphaVal(((DotrFuncDrawTextureFlags *)((unsigned char *)pModel + 0x1F8))->overlay));
            sprite.r = pModel->pad040[0xF2];
            sprite.g = pModel->pad040[0xF1];
            sprite.b = pModel->pad040[0xF0];
            sprite.a = func_001a23b0((float)(unsigned int)pModel->pad040[0xF3] *
                                         *(float *)((unsigned char *)pModel + 0x128));
            sprite.q = 1.0f;
            (sv0 = (DotrFuncDrawXyzf *)&sprite.v0)->x = 0x6C00;
            sprite.v0.y = 0x7200;
            sv0->z = 0xFFFFFF;
            sprite.v0.fog = 0;
            (sv1 = (DotrFuncDrawXyzf *)&sprite.v1)->x = 0x9400;
            sprite.v1.y = 0x8E00;
            sv1->z = 0xFFFFFF;
            sprite.v1.fog = 0;
            packet = RhBase_MaskFBuffer((void *)ExSortPrim_Pipe((int)RhBase_MaskFBuffer(packet, 0xFF000000), (int)&sprite, 0, 0), 0);
        }
        if (postRenderCallback) {
            packet = ((void *(*)(void *))postRenderCallback)(packet);
        }
        ((DotrFuncDrawHeader *)&primitive.header)->type = 0x26;
        ((DotrFuncDrawHeader *)&primitive.header)->count = 5;
        ((DotrFuncDrawHeader *)&primitive.header)->tag = 0x42421;
        ((DotrFuncDrawHeader *)&primitive.header)->pad04 = 0;
        ExAssignAlpha(&primitive, ExGetAlphaVal(((DotrFuncDrawTextureFlags *)((unsigned char *)pModel + 0x1F8))->base));
        primitive.r = pModel->pad040[0xEE];
        primitive.g = pModel->pad040[0xED];
        primitive.b = pModel->pad040[0xEC];
        primitive.a = pModel->pad040[0xEF];
        primitive.q = 1.0f;
        (pv0 = (DotrFuncDrawXyzf *)&primitive.v0)->x = 0x6BF8;
        primitive.v0.y = 0x71F8;
        pv0->z = depth;
        primitive.v0.fog = 0;
        (pv1 = (DotrFuncDrawXyzf *)&primitive.v1)->x = 0x93F8;
        primitive.v1.y = 0x8DF8;
        pv1->z = depth;
        primitive.v1.fog = 0;
        primitive.s0 = 0.0f;
        primitive.t0 = 0.0f;
        primitive.s1 = 0.625f;
        primitive.t1 = 0.875f;
        pflags = (DotrFuncDrawPrimitiveFlags *)&primitive.extra[2];
        RhBase_GetTextureInfoForRender(&primitive.extra[1], pflags);
        pflags->flag = pflags->type = 0;
        packet = (void *)RhText_SetTextureFlush((int)RhBase_DisableRenderFrame(packet));
        if (enabled && RhView_PopStack()) {
            RhView_GetCurView(&effect1);
            RhView_Modify(&effect1);
            packet = (void *)RhView_SendViewParameters((int)packet);
        }
        RhBase_FreeRenderFrame();
        if (arg5 != 0) {
            *(DotrFuncDrawPrimitive *)arg5 = primitive;
        } else {
            packet = ExDitherMode_Pipe((void *)RhBase_SetZBufferMode(ExSortPrim_Pipe(RhBase_SetZBufferMode((int)ExDitherMode_Pipe(packet, 0), 0), (int)&primitive, 0, 0), 1), 1);
        }
        if (1 != gSzModel_nBlurMode_word) {
            data_0031bb10 = 0;
        }
    }
    return (int)packet;
}
#ifdef __MWERKS__
#pragma global_optimizer reset
#endif
