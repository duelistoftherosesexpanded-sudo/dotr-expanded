#include "dotr/ee/view_act.h"

/* SzView_Act (0x1f7790): debug pad editing of the active view's lights, fog ranges and
 * radius, then pad-driven camera rolls.
 * Compiled with `#pragma conditional_move off` (docs/DECOMPILATION.md): the
 * original selects with branches where the compiler would otherwise use movn/movz. */
#ifdef __MWERKS__
#pragma conditional_move off
#endif
int SzView_Act(int index)
{
    int debugActiveMenu;
    long long gameMode;

    debugActiveMenu = SzDebug_GetActiveMenu();
    gameMode = SzGame_GetMode(1);
    if (index < 0 || index > 0) {
        return 0;
    } else {
        struct DotrViewActState *view;
        int modelType;
        struct DotrViewActLight *temp_s2;
        struct DotrViewActRange *temp_s3;
        struct DotrViewActRange *temp_s4;
        struct DotrViewActColor *temp_s5;
        int spBC;
        int spC0;
        int spD0;
        int spE0;
        struct DotrViewActReset lightDirection;
        float var_f20;
        float var_f21;
        float var_f22;
        int var_v0;
        int temp_s2_2;

        view = (struct DotrViewActState *) gSzView_hView_act[index];
        if (view == 0) {
            return 0;
        }
        spBC = SzDebug_GetActiveModel();
        modelType = SzModel_GetModelType(spBC);
        spC0 = (view->lightFlags & (1 << modelType)) ? modelType : 9;
        spD0 = (view->fogFlags & (1 << modelType)) ? modelType : 9;
        spE0 = (view->secondFlags & (1 << modelType)) ? modelType : 9;
        var_v0 = 9;
        if (spD0 != 9 || spE0 != 9) {
            var_v0 = modelType;
        }
        temp_s5 = (struct DotrViewActColor *) ((unsigned char *) view + var_v0 * 4 + 0x880);
        temp_s2 = (struct DotrViewActLight *) ((unsigned char *) view + spC0 * 0x90 + 0x60);
        temp_s3 = (struct DotrViewActRange *) ((unsigned char *) view + spD0 * 8 + 0x7E0);
        temp_s4 = (struct DotrViewActRange *) ((unsigned char *) view + spE0 * 8 + 0x830);
        if (gameMode == 0 || gameMode == 5 || gameMode == 12) {
            if (debugActiveMenu == 0xC) {
                if (SzPad_Check(0, 0, 0x4000) != 0) {
                    lightDirection = data_00293050_act;
                    SzView_SetStraightLightDirection(0, spC0, nCurLight_act, &lightDirection);
                }
                if (SzPad_Check(0, 2, 0xA000) != 0) {
                    if (SzPad_Check(0, 2, 0x8000) != 0) {
                        nCurLight_act = (nCurLight_act + 2) % 3;
                    } else {
                        nCurLight_act = (nCurLight_act + 4) % 3;
                    }
                    }
                if (SzPad_Check(0, 2, 0x80) != 0) {
                    temp_s2[nCurLight_act].y -= 0x100;
                }
                if (SzPad_Check(0, 2, 0x20) != 0) {
                    temp_s2[nCurLight_act].y += 0x100;
                }
                if (SzPad_Check(0, 2, 0x40) != 0) {
                    temp_s2[nCurLight_act].x -= 0x100;
                }
                if (SzPad_Check(0, 2, 0x10) != 0) {
                    temp_s2[nCurLight_act].x += 0x100;
                }
                if (SzPad_Check(0, 2, 0xFFFF) != 0) {
                    SzView_SetStraightLightDirection(0, spC0, nCurLight_act, 0);
                }
            }
            if (debugActiveMenu == 0xD) {
                if (SzPad_Check(0, 0, 0x4000) != 0) {
                    SzView_SetStraightLightColor(index, spC0, nCurLight_act, 0);
                }
                if (SzPad_Check(0, 2, 0xA000) != 0) {
                    if (SzPad_Check(0, 2, 0x8000) != 0) {
                        nCurLight_act = (nCurLight_act + 2) % 3;
                    } else {
                        nCurLight_act = (nCurLight_act + 4) % 3;
                    }
                    }
                if (SzPad_Check(0, 2, 0x81) != 0) {
                    temp_s2[nCurLight_act].rotation[0] -= 0.01f;
                }
                if (SzPad_Check(0, 2, 0x22) != 0) {
                    temp_s2[nCurLight_act].rotation[0] += 0.01f;
                }
                if (SzPad_Check(0, 2, 0x41) != 0) {
                    temp_s2[nCurLight_act].rotation[1] -= 0.01f;
                }
                if (SzPad_Check(0, 2, 0x12) != 0) {
                    temp_s2[nCurLight_act].rotation[1] += 0.01f;
                }
                if (SzPad_Check(0, 2, 5) != 0) {
                    temp_s2[nCurLight_act].rotation[2] -= 0.01f;
                }
                if (SzPad_Check(0, 2, 0xA) != 0) {
                    temp_s2[nCurLight_act].rotation[2] += 0.01f;
                }
            }
            if (debugActiveMenu == 0xE) {
                if (SzPad_Check(0, 0, 0x4000) != 0) {
                    temp_s5->r = 0xFFU;
                    temp_s5->g = 0xFFU;
                    temp_s5->b = 0xFFU;
                }
                if (SzPad_Check(0, 2, 0x81) != 0) {
                    if (temp_s5->b > 0) {
                        temp_s5->b--;
                    }
                }
                if (SzPad_Check(0, 2, 0x22) != 0) {
                    if (temp_s5->b < 0xFF) {
                        temp_s5->b++;
                    }
                }
                if (SzPad_Check(0, 2, 0x41) != 0) {
                    if (temp_s5->g > 0) {
                        temp_s5->g--;
                    }
                }
                if (SzPad_Check(0, 2, 0x12) != 0) {
                    if (temp_s5->g < 0xFF) {
                        temp_s5->g++;
                    }
                }
                if (SzPad_Check(0, 2, 5) != 0) {
                    if (temp_s5->r > 0) {
                        temp_s5->r--;
                    }
                }
                if (SzPad_Check(0, 2, 0xA) != 0) {
                    if (temp_s5->r < 0xFF) {
                        temp_s5->r++;
                    }
                }
            }
            if (debugActiveMenu == 0xF) {
                if (SzPad_Check(0, 0, 0x4000) != 0) {
                    temp_s3->near = 2000.0f;
                    temp_s3->far = 4000.0f;
                }
                if (SzPad_Check(0, 0, 0x8000) != 0) {
                    view->fogFlags &= ~(1 << spD0);
                }
                if (SzPad_Check(0, 0, 0x2000) != 0) {
                    view->fogFlags |= 1 << spD0;
                }
                if (SzPad_Check(0, 2, 0x80) != 0) {
                    temp_s3->near -= 10.0f;
                }
                if (SzPad_Check(0, 2, 0x20) != 0) {
                    temp_s3->near += 10.0f;
                }
                if (SzPad_Check(0, 2, 0x40) != 0) {
                    temp_s3->far -= 10.0f;
                }
                if (SzPad_Check(0, 2, 0x10) != 0) {
                    temp_s3->far += 10.0f;
                }
            }
            if (debugActiveMenu == 0x10) {
                if (SzPad_Check(0, 0, 0x4000) != 0) {
                    temp_s4->near = 2000.0f;
                    temp_s4->far = 4000.0f;
                }
                if (SzPad_Check(0, 0, 0x8000) != 0) {
                    view->secondFlags &= ~(1 << spE0);
                }
                if (SzPad_Check(0, 0, 0x2000) != 0) {
                    view->secondFlags |= 1 << spE0;
                }
                if (SzPad_Check(0, 2, 0x80) != 0) {
                    temp_s4->near -= 10.0f;
                }
                if (SzPad_Check(0, 2, 0x20) != 0) {
                    temp_s4->near += 10.0f;
                }
                if (SzPad_Check(0, 2, 0x40) != 0) {
                    temp_s4->far -= 10.0f;
                }
                if (SzPad_Check(0, 2, 0x10) != 0) {
                    temp_s4->far += 10.0f;
                }
            }
            if (debugActiveMenu == 0x11) {
                if (SzPad_Check(0, 2, 0xC) != 0) {
                    temp_s2_2 = SzView_GetMode(0);
                    if (SzPad_Check(0, 2, 4) != 0) {
                        SzView_SetMode(0, (int) (temp_s2_2 + 2) % 3);
                    } else {
                        SzView_SetMode(0, (int) (temp_s2_2 + 4) % 3);
                    }
                }
                if (SzPad_Check(0, 2, 0x4000) != 0) {
                    view->radius = 800.0f;
                }
                if (SzPad_Check(0, 2, 0x80) != 0) {
                    view->radius -= 10.0f;
                    if (view->radius < 10.0f) {
                        view->radius = 10.0f;
                    }
                }
                if (SzPad_Check(0, 2, 0x20) != 0) {
                    view->radius += 10.0f;
                    if (!(view->radius <= 10000.0f)) {
                        view->radius = 10000.0f;
                    }
                }
            }
        }
        var_f20 = 0.0f;
        var_f21 = 0.0f;
        var_f22 = 0.0f;
        if (SzView_InqRequest(-1, -1) == 0) {
            if ((unsigned int) (modelType - 4) < 2U) {
                SzView_SetViewCamera(index, spBC, 0, view->mode, 1.0f);
            }
        }
        if (SzPad_IsPress(1) != 0) {
            var_f20 += (0.024543693f * (float) SzPad_GetPressInfo(1, 1)) / 64.0f;
            var_f20 -= (0.024543693f * (float) SzPad_GetPressInfo(1, 0)) / 64.0f;
            var_f21 += (0.024543693f * (float) SzPad_GetPressInfo(1, 2)) / 64.0f;
            var_f21 -= (0.024543693f * (float) SzPad_GetPressInfo(1, 3)) / 64.0f;
            var_f22 += (2.0f * (float) SzPad_GetPressInfo(1, 8)) / 64.0f;
            var_f22 -= (2.0f * (float) SzPad_GetPressInfo(1, 9)) / 64.0f;
            SzView_RollCamera(index, 0, 1, var_f20, var_f21, var_f22);
            var_f22 = 0.0f;
            var_f21 = 0.0f;
            var_f20 = 0.0f;
            var_f20 -= (0.024543693f * (float) SzPad_GetPressInfo(1, 7)) / 64.0f;
            var_f20 += (0.024543693f * (float) SzPad_GetPressInfo(1, 5)) / 64.0f;
            var_f21 -= (0.024543693f * (float) SzPad_GetPressInfo(1, 4)) / 64.0f;
            var_f21 += (0.024543693f * (float) SzPad_GetPressInfo(1, 6)) / 64.0f;
            var_f22 -= (2.0f * (float) SzPad_GetPressInfo(1, 0xA)) / 64.0f;
            var_f22 += (2.0f * (float) SzPad_GetPressInfo(1, 0xB)) / 64.0f;
            SzView_RollCamera(index, 0, -1, var_f20, var_f21, var_f22);
        } else {
            if (SzPad_Check(1, 1, 0x8000) != 0) {
                var_f20 += 0.024543693f;
            }
            if (SzPad_Check(1, 1, 0x2000) != 0) {
                var_f20 -= 0.024543693f;
            }
            if (SzPad_Check(1, 1, 0x1000) != 0) {
                var_f21 += 0.024543693f;
            }
            if (SzPad_Check(1, 1, 0x4000) != 0) {
                var_f21 -= 0.024543693f;
            }
            if (SzPad_Check(1, 1, 4) != 0) {
                var_f22 += 2.0f;
            }
            if (SzPad_Check(1, 1, 8) != 0) {
                var_f22 -= 2.0f;
            }
            SzView_RollCamera(index, 0, 1, var_f20, var_f21, var_f22);
            var_f22 = 0.0f;
            var_f21 = 0.0f;
            var_f20 = 0.0f;
            if (SzPad_Check(1, 1, 0x80) != 0) {
                var_f20 -= 0.024543693f;
            }
            if (SzPad_Check(1, 1, 0x20) != 0) {
                var_f20 += 0.024543693f;
            }
            if (SzPad_Check(1, 1, 0x10) != 0) {
                var_f21 -= 0.024543693f;
            }
            if (SzPad_Check(1, 1, 0x40) != 0) {
                var_f21 += 0.024543693f;
            }
            if (SzPad_Check(1, 1, 1) != 0) {
                var_f22 -= 2.0f;
            }
            if (SzPad_Check(1, 1, 2) != 0) {
                var_f22 += 2.0f;
            }
            SzView_RollCamera(index, 0, -1, var_f20, var_f21, var_f22);
        }
        if ((gameMode == 0) || (gameMode == 5) || (gameMode == 0xC)) {
            SzDebug_IsHide();
        }
        SzDebug_IsHide();
        return 1;
    }
}
#ifdef __MWERKS__
#pragma conditional_move reset
#endif
