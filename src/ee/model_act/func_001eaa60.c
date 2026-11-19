#include "dotr/ee/model_act.h"

/* 0x1eaa60. Two callees are declared inside the function body; the model table is the view
 * gSzModel_ahModel_modelact. */
#ifndef __MWERKS__
/* Host build only: the code passes addresses as 32-bit words, which the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

int SzModel_Act(int modelId)
{
    extern int SzModel_ReqMotionBlur();
    extern int SzFile_ReadRequestParam();
    long long mode;
    struct DotrModelActDbgObject *pModel;
    int page;
    float last;
    int i;
    int sel;
    int snd;
    int num;
    int newnum;
    int sub;
    int dir;
    int flag;
    int flag2;
    int n;
    float *anim;
    int pos;
    signed char *cur;
    signed char *lvp;
    void *ap;

    mode = SzGame_GetMode(1);
    if (modelId < 0 || modelId >= 0x54) {
        return 0;
    }
    pModel = (struct DotrModelActDbgObject *)gSzModel_ahModel_modelact[modelId];
    if (pModel == NULL) {
        return 0;
    }
    if (pModel->page == SzDebug_GetActiveModel()) {
        anim = &pModel->speed;
        lvp = &pModel->lv[0];
        if (modelId == 0) lvp = &pModel->lv[0]; /* MATCHING DEVICE: the repeated assignment keeps
                                           * lvp in its own register (s5) */
        cur = &pModel->lv[2];
        page = SzDebug_GetActiveMenu();
        last = (float)pModel->vals[*lvp] - 1.0f;
        if (page == 1) {
            if (pModel->slots[*lvp] != NULL) {
                if (!(DOTR_MODEL_ACT_PAD(1, 3) & 0xFFFF)) {
                    if (DOTR_MODEL_ACT_PAD(2, 8) != 0) {
                        do {
                            *cur = (*cur + 9) % 8;
                        } while (pModel->slots[*cur] == NULL);
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                        do {
                            *cur = (*cur + 7) % 8;
                        } while (pModel->slots[*cur] == NULL);
                    }
                }
                if (DOTR_MODEL_ACT_PAD(1, 0x10) != 0) {
                    if (DOTR_MODEL_ACT_PAD(0, 0x10) != 0) {
                        SzModel_SetAnim(modelId, *cur);
                    } else {
                        if (DOTR_MODEL_ACT_PAD(2, 2) != 0) {
                            SzModel_SetAnimSpeed(modelId, anim[0] + 0.1f);
                        }
                        if (DOTR_MODEL_ACT_PAD(2, 1) != 0) {
                            SzModel_SetAnimSpeed(modelId, anim[0] - 0.1f);
                        }
                    }
                }
                if (DOTR_MODEL_ACT_PAD(0, 0xF000) != 0) {
                    sel = 0;
                    if (DOTR_MODEL_ACT_PAD(0, 0x1000) != 0) {
                        sel = 0;
                    } else if (DOTR_MODEL_ACT_PAD(0, 0x2000) != 0) {
                        sel = 1;
                    } else if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                        sel = 2;
                    } else if (DOTR_MODEL_ACT_PAD(0, 0x8000) != 0) {
                        sel = 3;
                    }
                    if (DOTR_MODEL_ACT_PAD(1, 0x10) != 0) {
                        sel += 4;
                    }
                    SzModel_SetAnimIntr(modelId, sel);
                }
                if (DOTR_MODEL_ACT_PAD(1, 0x40) != 0 || pModel->loop == 0) {
                    if (DOTR_MODEL_ACT_PAD(2, 0x20) != 0) {
                        pModel->loop = 1;
                        if (!(anim[1] < last)) {
                            anim[1] = 1.0f;
                        } else {
                            anim[1] += anim[0];
                            if (!(anim[1] <= last)) {
                                anim[1] = last;
                            }
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x80) != 0) {
                        pModel->loop = -1;
                        if (anim[1] <= 1.0f) {
                            anim[1] = last;
                        } else {
                            anim[1] -= anim[0];
                            if (anim[1] < 1.0f) {
                                anim[1] = 1.0f;
                            }
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(1, 0x40) != 0) {
                        pModel->loop = 0;
                    }
                } else {
                    if (DOTR_MODEL_ACT_PAD(0, 0x20) != 0) {
                        pModel->loop = 1;
                    }
                    if (DOTR_MODEL_ACT_PAD(0, 0x80) != 0) {
                        pModel->loop = -1;
                    }
                }
            }
        }
        if (page == 2) {
            switch (nBlurCur) {
            case 0:
                flag = 0;
                flag2 = 0;
                if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                    wAlphaMode = 0;
                    wFrame = 6;
                }
                if (!(DOTR_MODEL_ACT_PAD(1, 3) & 0xFFFF)) {
                    if (DOTR_MODEL_ACT_PAD(1, 0x1000) != 0) {
                        if (DOTR_MODEL_ACT_PAD(2, 0xC) != 0) {
                            if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                                wAlphaMode = (wAlphaMode + 3) % 4;
                            } else {
                                wAlphaMode = (wAlphaMode + 5) % 4;
                            }
                        }
                    } else if (DOTR_MODEL_ACT_PAD(2, 0xC) != 0) {
                        if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                            n = wFrame - 1;
                            wFrame = (n > 1) ? n : 1;
                        } else {
                            n = wFrame + 1;
                            wFrame = (n < 0x63) ? n : 0x63;
                        }
                    }
                }
                if (DOTR_MODEL_ACT_PAD(1, 0x10) != 0) {
                    if (SzGame_GetFrameCount() & 1) {
                        flag = 1;
                        flag2 = 1;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(1, 0x80) != 0) {
                    if (SzGame_GetFrameCount() & 1) {
                        flag2 = 1;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(0, 0x20) != 0) {
                    flag2 = 1;
                }
                if (flag2 != 0) {
                    SzModel_ReqMotionBlur(modelId, wFrame, wAlphaMode, iColF, iColT, flag);
                }
                break;
            case 1:
                if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                    iColF[0] = 0x80;
                    iColF[1] = 0x80;
                    iColF[2] = 0x80;
                    iColF[3] = 0x40;
                }
                if (DOTR_MODEL_ACT_PAD(1, 0x1000) != 0) {
                    if (DOTR_MODEL_ACT_PAD(2, 0x80) != 0) {
                        if (iColF[3] > 0) {
                            iColF[3]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x20) != 0 && iColF[3] < 0xFF) {
                        iColF[3]++;
                    }
                } else {
                    if (DOTR_MODEL_ACT_PAD(2, 0x81) != 0) {
                        if (iColF[2] > 0) {
                            iColF[2]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x22) != 0) {
                        if (iColF[2] < 0xFF) {
                            iColF[2]++;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x41) != 0) {
                        if (iColF[1] > 0) {
                            iColF[1]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x12) != 0) {
                        if (iColF[1] < 0xFF) {
                            iColF[1]++;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 5) != 0) {
                        if (iColF[0] > 0) {
                            iColF[0]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0xA) != 0 && iColF[0] < 0xFF) {
                        iColF[0]++;
                    }
                }
                break;
            case 2:
                if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                    iColT[0] = 0x80;
                    iColT[1] = 0x80;
                    iColT[2] = 0x80;
                    iColT[3] = 0;
                }
                if (DOTR_MODEL_ACT_PAD(1, 0x1000) != 0) {
                    if (DOTR_MODEL_ACT_PAD(2, 0x80) != 0) {
                        if (iColT[3] > 0) {
                            iColT[3]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x20) != 0 && iColT[3] < 0xFF) {
                        iColT[3]++;
                    }
                } else {
                    if (DOTR_MODEL_ACT_PAD(2, 0x81) != 0) {
                        if (iColT[2] > 0) {
                            iColT[2]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x22) != 0) {
                        if (iColT[2] < 0xFF) {
                            iColT[2]++;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x41) != 0) {
                        if (iColT[1] > 0) {
                            iColT[1]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0x12) != 0) {
                        if (iColT[1] < 0xFF) {
                            iColT[1]++;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 5) != 0) {
                        if (iColT[0] > 0) {
                            iColT[0]--;
                        }
                    }
                    if (DOTR_MODEL_ACT_PAD(2, 0xA) != 0 && iColT[0] < 0xFF) {
                        iColT[0]++;
                    }
                }
                break;
            case 3:
                if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                    nBlurUse = 2;
                }
                if (DOTR_MODEL_ACT_PAD(0, 0x10) != 0) {
                    nBlurUse = 2;
                }
                if (DOTR_MODEL_ACT_PAD(0, 0xC0) != 0) {
                    nBlurUse = 1;
                }
                if (DOTR_MODEL_ACT_PAD(0, 0x40) != 0) {
                    nBlurUse = 0;
                }
                break;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0xA000) != 0) {
                if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                    nBlurCur = (nBlurCur + 3) % 4;
                } else {
                    nBlurCur = (nBlurCur + 5) % 4;
                }
            }
        }
        if (page == 3) {
            struct DotrModelActDigits4 dig;
            char path[0x80];

            sub = modelId - 0x1E;
            num = pModel->num;
            dig = data_00292f90;
            newnum = num;
            dig.d0 = num / 1000 % 10;
            dig.d1 = num / 100 % 10;
            dig.d2 = num / 10 % 10;
            dig.d3 = num % 10;
            if (DOTR_MODEL_ACT_PAD(0, 0xC0) != 0) {
                if (pModel->anim != NULL) {
                    flag = DOTR_MODEL_ACT_PAD(1, 3) != 0 ? 1 : 0;
                    dir = DOTR_MODEL_ACT_PAD(0, 0x80) != 0 ? 1 : 0;
                    SzView_FreeRequest(0, 1);
                    snd = pModel->sound;
                    if (snd >= 0) {
                        SzEffect_Destroy(snd);
                    }
                    SzModel_SetAnimDirect(0x1E, 0);
                    SzModel_SetAnimDirect(0x1F, 0);
                    SzModel_SetAnimDir(0x1E, 1);
                    SzModel_SetAnimDir(0x1F, 1);
                    SzDuel_SetUnitStatus(sub ^ 1, 0, 5, dir);
                    SzModel_BattleSeq(sub, 1, flag > 0);
                }
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x600) != 0) {
                if (pModel->anim != NULL) {
                    flag = DOTR_MODEL_ACT_PAD(0, 0x200) != 0 ? 1 : 0;
                    SzView_FreeRequest(0, 1);
                    SzModel_SetAnimDirect(0x1E, 0);
                    SzModel_SetAnimDir(0x1E, 1);
                    SzModel_SetAnimIntr(modelId, 5);
                    ap = *(void *volatile *)&pModel->anim; /* loaded before f202, as in the original */
                    SzView_ReqCamera(0, 1, flag + 8, pModel->f202, modelId, ap);
                }
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x20) != 0) {
                SzView_FreeRequest(0, 1);
                snd = pModel->sound;
                if (snd >= 0) {
                    SzEffect_Destroy(snd);
                }
                SzModel_SetAnimDirect(0x1E, 0);
                SzModel_SetAnimDirect(0x1F, 0);
                SzModel_SetAnimDir(0x1E, 1);
                SzModel_SetAnimDir(0x1F, 1);
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x10) != 0) {
                SzView_FreeRequest(0, 1);
                if (pModel->model->flags & 0x80000) {
                    pModel->anim = NULL;
                } else if (pModel->anim != NULL) {
                    SzMem_Free((int)pModel->anim);
                    pModel->anim = NULL;
                }
                snd = pModel->sound;
                if (snd >= 0) {
                    SzEffect_Destroy(snd);
                }
                func_0019b648(path, data_002cfb7d, SzFile_GetEnv((int)data_002cfa17), num, num);
                SzFile_ReadRequestParam(0x400, 1, path, 0, -1, 0, -1, lSzModel_LoadCameraCB, modelId);
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x5000) != 0) {
                if (DOTR_MODEL_ACT_PAD(2, 0x1000) != 0) {
                    (&dig.d0)[nCameraCurrent] = ((&dig.d0)[nCameraCurrent] + 11) % 10;
                } else {
                    (&dig.d0)[nCameraCurrent] = ((&dig.d0)[nCameraCurrent] + 9) % 10;
                }
                newnum = (&dig.d0)[0] * 1000 + (&dig.d0)[1] * 100 + (&dig.d0)[2] * 10 + (&dig.d0)[3];
            }
            if (!(DOTR_MODEL_ACT_PAD(1, 3) & 0xFFFF)) {
                if (DOTR_MODEL_ACT_PAD(2, 8) != 0) {
                    if (pModel->f202 < 0x3E7) {
                        pModel->f202++;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                    if (pModel->f202 > 0) {
                        pModel->f202--;
                    }
                }
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                nCameraCurrent = (nCameraCurrent + 3) % 4;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                nCameraCurrent = (nCameraCurrent + 5) % 4;
            }
            if (newnum != num) {
                pModel->num = newnum;
                SzView_FreeRequest(0, 1);
                if (pModel->model->flags & 0x80000) {
                    pModel->anim = NULL;
                } else if (pModel->anim != NULL) {
                    SzMem_Free((int)pModel->anim);
                    pModel->anim = NULL;
                }
                snd = pModel->sound;
                if (snd >= 0) {
                    SzEffect_Destroy(snd);
                }
            }
        }
        if (page == 4) {
            struct DotrModelActDigits4 dig2;
            char path2[0x80];
            int num;
            int i;
            int flag;
            int newnum;

            num = pModel->num;
            dig2 = data_00292fa0;
            newnum = num;
            dig2.d0 = num / 1000 % 10;
            dig2.d1 = num / 100 % 10;
            dig2.d2 = num / 10 % 10;
            dig2.d3 = num % 10;
            if (DOTR_MODEL_ACT_PAD(0, 0xC0) != 0) {
                flag = DOTR_MODEL_ACT_PAD(1, 3) != 0 ? 1 : 0;
                DOTR_MODEL_ACT_PAD(0, 0x80);
                SzView_FreeRequest(0, 1);
                SzEffect_Destroy2(0, 0x3D);
                SzEffect_Destroy2(0, 0x3E);
                SzEffect_Destroy2(0, 0x3F);
                SzModel_SetAnimDirect(0x20, 0);
                SzModel_SetAnimDir(0x20, 1);
                SzText_FreeRequest(5, -1);
                for (i = 0; i < 8; i++) {
                    SzText_AddFile(5, i, NULL, pModel->parts[i]);
                }
                SzEffect_EntryCallback(0, 0x3D, 0, 0, 0, 0, 0);
                SzEffect_EntryCallback(0, 0x3E, 0, 0, 0, 0, 0);
                SzEffect_EntryCallback(0, 0x3F, 0, 0, 0, 0, 0);
                if (pModel->anim != NULL) {
                    SzView_ReqCamera(0, 1, flag + 8, pModel->f202, modelId, pModel->anim);
                }
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x20) != 0) {
                SzView_FreeRequest(0, 1);
                SzEffect_Destroy2(0, 0x3D);
                SzEffect_Destroy2(0, 0x3E);
                SzEffect_Destroy2(0, 0x3F);
                SzModel_SetAnimDirect(0x20, 0);
                SzModel_SetAnimDir(0x20, 1);
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x10) != 0) {
                SzView_FreeRequest(0, 1);
                if (pModel->model->flags & 0x80000) {
                    pModel->anim = NULL;
                } else if (pModel->anim != NULL) {
                    SzMem_Free((int)pModel->anim);
                    pModel->anim = NULL;
                }
                SzEffect_Destroy2(0, 0x3D);
                SzEffect_Destroy2(0, 0x3E);
                SzEffect_Destroy2(0, 0x3F);
                func_0019b648(path2, data_002cfb7d, SzFile_GetEnv((int)data_002cfa17), num, num);
                SzFile_ReadRequestParam(0x400, 1, path2, 0, -1, 0, -1, lSzModel_LoadCameraCB, modelId);
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x5000) != 0) {
                if (DOTR_MODEL_ACT_PAD(2, 0x1000) != 0) {
                    (&dig2.d0)[nCameraCurrent] = ((&dig2.d0)[nCameraCurrent] + 11) % 10;
                } else {
                    (&dig2.d0)[nCameraCurrent] = ((&dig2.d0)[nCameraCurrent] + 9) % 10;
                }
                newnum = (&dig2.d0)[0] * 1000 + (&dig2.d0)[1] * 100 + (&dig2.d0)[2] * 10 + (&dig2.d0)[3];
            }
            if (!(DOTR_MODEL_ACT_PAD(1, 3) & 0xFFFF)) {
                if (DOTR_MODEL_ACT_PAD(2, 8) != 0) {
                    if (pModel->f202 < 0x3E7) {
                        pModel->f202++;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                    if (pModel->f202 > 0) {
                        pModel->f202--;
                    }
                }
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                nCameraCurrent = (nCameraCurrent + 3) % 4;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                nCameraCurrent = (nCameraCurrent + 5) % 4;
            }
            if (newnum != num) {
                pModel->num = newnum;
                SzView_FreeRequest(0, 1);
                if (pModel->model->flags & 0x80000) {
                    pModel->anim = NULL;
                } else if (pModel->anim != NULL) {
                    SzMem_Free((int)pModel->anim);
                    pModel->anim = NULL;
                }
                snd = pModel->sound;
                if (snd >= 0) {
                    SzEffect_Destroy(snd);
                }
            }
        }
        if (page == 5) {
            struct DotrModelActDigits3 d3;
            struct DotrModelActDigits3 d3b;

            sub = modelId - 0x1E;
            d3 = data_00292fb0;
            d3.d0 = pModel->f206 / 100 % 10;
            d3.d1 = pModel->f206 / 10 % 10;
            d3.d2 = pModel->f206 % 10;
            d3b = data_00292fc0;
            d3b.d0 = pModel->f208 / 100 % 10;
            d3b.d1 = pModel->f208 / 10 % 10;
            d3b.d2 = pModel->f208 % 10;
            if (DOTR_MODEL_ACT_PAD(0, 0xC0) != 0) {
                dir = DOTR_MODEL_ACT_PAD(0, 0x80) != 0 ? 1 : 0;
                snd = pModel->sound;
                if (snd >= 0) {
                    SzEffect_Destroy(snd);
                }
                SzModel_SetAnimDirect(0x1E, 0);
                SzModel_SetAnimDirect(0x1F, 0);
                SzModel_SetAnimDir(0x1E, 1);
                SzModel_SetAnimDir(0x1F, 1);
                SzDuel_SetUnitStatus(sub ^ 1, 0, 5, dir);
                SzModel_BattleSeq(sub, 0, 0);
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x40) != 0) {
                snd = pModel->sound;
                if (snd >= 0) {
                    SzEffect_Destroy(snd);
                }
                SzModel_SetAnimDirect(0x1E, 0);
                SzModel_SetAnimDirect(0x1F, 0);
                SzModel_SetAnimDir(0x1E, 1);
                SzModel_SetAnimDir(0x1F, 1);
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x10) != 0) {
                while (SzFile_InqRequest(0x800) != 0) {
                    SzFile_DoRequestS();
                }
                SzEffect_Destroy2(-1, -1);
                SzEffect_Clear();
            }
            switch (data_0031bb48) {
            case 0:
                if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                    if (++nEffectCurrent > 2) {
                        nEffectCurrent = 0;
                        data_0031bb48 = 1;
                    }
                } else if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                    if (--nEffectCurrent < 0) {
                        data_0031bb48 = 1;
                        nEffectCurrent = 2;
                    }
                } else if (DOTR_MODEL_ACT_PAD(2, 0x5000) != 0) {
                    if (DOTR_MODEL_ACT_PAD(2, 0x1000) != 0) {
                        (&d3.d0)[nEffectCurrent] = ((&d3.d0)[nEffectCurrent] + 11) % 10;
                    } else {
                        (&d3.d0)[nEffectCurrent] = ((&d3.d0)[nEffectCurrent] + 9) % 10;
                    }
                    pModel->f206 = (&d3.d0)[0] * 100 + (&d3.d0)[1] * 10 + (&d3.d0)[2];
                }
                break;
            case 1:
                if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                    if (++nEffectCurrent > 2) {
                        data_0031bb48 = 0;
                        nEffectCurrent = 0;
                    }
                } else if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                    if (--nEffectCurrent < 0) {
                        data_0031bb48 = 0;
                        nEffectCurrent = 2;
                    }
                } else if (DOTR_MODEL_ACT_PAD(2, 0x5000) != 0) {
                    if (DOTR_MODEL_ACT_PAD(2, 0x1000) != 0) {
                        (&d3b.d0)[nEffectCurrent] = ((&d3b.d0)[nEffectCurrent] + 11) % 10;
                    } else {
                        (&d3b.d0)[nEffectCurrent] = ((&d3b.d0)[nEffectCurrent] + 9) % 10;
                    }
                    pModel->f208 = (&d3b.d0)[0] * 100 + (&d3b.d0)[1] * 10 + (&d3b.d0)[2];
                }
                break;
            }
        }
        if (page == 6) {
            if (DOTR_MODEL_ACT_PAD(0, 0xC0) != 0) {
                gSzModel_fRefLock_word = 1;
            }
            if (DOTR_MODEL_ACT_PAD(0, 0x40) != 0) {
                gSzModel_fRefLock_word = 0;
            }
            if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                gSzModel_nRefLock_word = (gSzModel_nRefLock_word + pModel->count - 1) % pModel->count;
            }
            if (DOTR_MODEL_ACT_PAD(2, 8) != 0) {
                gSzModel_nRefLock_word = (gSzModel_nRefLock_word + pModel->count + 1) % pModel->count;
            }
        }
        if (page == 7) {
            if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                pModel->pos[0] -= 0.5f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                pModel->pos[0] += 0.5f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x4000) != 0) {
                pModel->pos[1] -= 0.5f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x1000) != 0) {
                pModel->pos[1] += 0.5f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                pModel->pos[2] -= 0.5f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 8) != 0) {
                pModel->pos[2] += 0.5f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x80) != 0) {
                pModel->rot[1] = (pModel->rot[1] + 0xFF00) % 0x10000;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x20) != 0) {
                pModel->rot[1] = (pModel->rot[1] + 0x10100) % 0x10000;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x40) != 0) {
                pModel->rot[0] = (pModel->rot[0] + 0xFF00) % 0x10000;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x10) != 0) {
                pModel->rot[0] = (pModel->rot[0] + 0x10100) % 0x10000;
            }
            if (DOTR_MODEL_ACT_PAD(2, 1) != 0) {
                pModel->rot[2] = (pModel->rot[2] + 0xFF00) % 0x10000;
            }
            if (DOTR_MODEL_ACT_PAD(2, 2) != 0) {
                pModel->rot[2] = (pModel->rot[2] + 0x10100) % 0x10000;
            }
        }
        if (page == 8) {
            if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                SzModel_SetScale(modelId, NULL);
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x81) != 0) {
                pModel->v.x -= 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x22) != 0) {
                pModel->v.x += 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x41) != 0) {
                pModel->v.y -= 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x12) != 0) {
                pModel->v.y += 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 5) != 0) {
                pModel->v.z -= 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0xA) != 0) {
                pModel->v.z += 0.01f;
            }
        }
        if (page == 9) {
            if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                SzModel_SetIntensity(modelId, NULL);
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x81) != 0) {
                pModel->light[0] -= 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x22) != 0) {
                pModel->light[0] += 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x41) != 0) {
                pModel->light[1] -= 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x12) != 0) {
                pModel->light[1] += 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 4) != 0) {
                pModel->light[2] -= 0.01f;
            }
            if (DOTR_MODEL_ACT_PAD(2, 8) != 0) {
                pModel->light[2] += 0.01f;
            }
        }
        if (page == 10) {
            if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                SzModel_SetRgba(modelId, 0, NULL);
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                pModel->fog0 = (pModel->fog0 + 3) % 4;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                pModel->fog0 = (pModel->fog0 + 5) % 4;
            }
            if (DOTR_MODEL_ACT_PAD(1, 0x1000) != 0) {
                if (DOTR_MODEL_ACT_PAD(2, 0x80) != 0) {
                    if (pModel->col0[3] > 0) {
                        pModel->col0[3]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x20) != 0) {
                    if (pModel->col0[3] < 0xFF) {
                        pModel->col0[3]++;
                    }
                }
            } else {
                if (DOTR_MODEL_ACT_PAD(2, 0x81) != 0) {
                    if (pModel->col0[2] > 0) {
                        pModel->col0[2]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x22) != 0) {
                    if (pModel->col0[2] < 0xFF) {
                        pModel->col0[2]++;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x41) != 0) {
                    if (pModel->col0[1] > 0) {
                        pModel->col0[1]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x12) != 0) {
                    if (pModel->col0[1] < 0xFF) {
                        pModel->col0[1]++;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 5) != 0) {
                    if (pModel->col0[0] > 0) {
                        pModel->col0[0]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0xA) != 0) {
                    if (pModel->col0[0] < 0xFF) {
                        pModel->col0[0]++;
                    }
                }
            }
        }
        if (page == 11) {
            if (DOTR_MODEL_ACT_PAD(0, 0x4000) != 0) {
                SzModel_SetFilter(modelId, 1, NULL);
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x8000) != 0) {
                pModel->fog1 = (pModel->fog1 + 3) % 4;
            }
            if (DOTR_MODEL_ACT_PAD(2, 0x2000) != 0) {
                pModel->fog1 = (pModel->fog1 + 5) % 4;
            }
            if (DOTR_MODEL_ACT_PAD(1, 0x1000) != 0) {
                if (DOTR_MODEL_ACT_PAD(2, 0x80) != 0) {
                    if (pModel->col1[3] > 0) {
                        pModel->col1[3]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x20) != 0) {
                    if (pModel->col1[3] < 0xFF) {
                        pModel->col1[3]++;
                    }
                }
            } else {
                if (DOTR_MODEL_ACT_PAD(2, 0x81) != 0) {
                    if (pModel->col1[2] > 0) {
                        pModel->col1[2]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x22) != 0) {
                    if (pModel->col1[2] < 0xFF) {
                        pModel->col1[2]++;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x41) != 0) {
                    if (pModel->col1[1] > 0) {
                        pModel->col1[1]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0x12) != 0) {
                    if (pModel->col1[1] < 0xFF) {
                        pModel->col1[1]++;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 5) != 0) {
                    if (pModel->col1[0] > 0) {
                        pModel->col1[0]--;
                    }
                }
                if (DOTR_MODEL_ACT_PAD(2, 0xA) != 0) {
                    if (pModel->col1[0] < 0xFF) {
                        pModel->col1[0]++;
                    }
                }
            }
        }
        if (mode == 5 && nBlurUse != 0 && (SzGame_GetFrameCount() & 1)) {
            SzModel_ReqMotionBlur(modelId, wFrame, wAlphaMode, iColF, iColT, nBlurUse == 1 ? 0 : 1);
        }
        if (SzDebug_IsHide() == 0) {
            signed char kind = pModel->kind;

            if (kind == 5 && mode == 5) {
                if (page == 3) {
                    pos = 1000;
                    i = nCameraCurrent;
                    while (i-- != 0) {
                        pos /= 10;
                    }
                }
                if (page == 5) {
                    pos = 100;
                    i = nEffectCurrent;
                    while (i-- != 0) {
                        pos /= 10;
                    }
                }
            }
            if (kind == 6 && mode == 0 && page == 4) {
                pos = 1000;
                i = nCameraCurrent;
                while (i-- != 0) {
                    pos /= 10;
                }
            }
        }
    }
    return 1;
}
