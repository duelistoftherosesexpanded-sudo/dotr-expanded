#include "dotr/ee/view3d_draw.h"

/* Per-frame control of the duel's 3D model view. Requests sound 29 on channel 2; pad
 * button 32 (trigger mode 0) plays sound 5 and pops the game state. Three deltas are then
 * collected: with an analog pad, the stick bytes 3, 2 and 1 (centred at 128, dead zone
 * below 48, the dead zone subtracted) scaled by 1/40; then either the pressure values of
 * buttons 1/0, 2/3 and 8/9 scaled by 1/64 (pressure pad) or fixed steps for the digital
 * buttons 0x8000/0x2000, 0x1000/0x4000 and 4/8. If any delta exceeds 1e-6 in size the
 * look vector of view 0 is updated: x (distance) clamped between the model's camera
 * distance and 300, y (angle) wrapped through func_001a6c30 with 2*pi, z (elevation)
 * clamped to -pi/8 .. 7*pi/16; the point light then takes view 0's SP vector as position.
 * The pressure value converts to float as unsigned (the target has the unsigned
 * conversion sequence), spelled as a cast because small_state declares the callee int. */
#define VIEW3D_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define VIEW3D_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define VIEW3D_ABS(a) (((a) >= 0) ? (a) : -(a))
/* pi / 128, the rotation step per frame at full deflection. */
#define VIEW3D_STEP 0.024543693f

void SzDuel_Act3DView(void)
{
    float rx = 0.0f;
    float ry = 0.0f;
    float dz = 0.0f;
    struct DotrView3dVector look;

    SzModel_ReqSE(2, 29);
    if (SzPad_Check(0, 0, 32)) {
        SzSound_PlaySE(0, 5);
        SzGame_Pop();
    }
    if (SzPad_IsAnalog(0)) {
        int a3 = (SzPad_GetAnalogInfo(0, 3) & 0xff) - 128;
        int a2 = (SzPad_GetAnalogInfo(0, 2) & 0xff) - 128;
        int a1 = (SzPad_GetAnalogInfo(0, 1) & 0xff) - 128;

        SzPad_GetAnalogInfo(0, 0);
        if (VIEW3D_ABS(a3) >= 48) {
            a3 -= (a3 > 0) ? 48 : -48;
        } else {
            a3 = 0;
        }
        if (VIEW3D_ABS(a2) >= 48) {
            a2 -= (a2 > 0) ? 48 : -48;
        } else {
            a2 = 0;
        }
        if (VIEW3D_ABS(a1) >= 48) {
            a1 -= (a1 > 0) ? 48 : -48;
        } else {
            a1 = 0;
        }
        rx -= VIEW3D_STEP * (float)a2 / 40.0f;
        ry -= VIEW3D_STEP * (float)a3 / 40.0f;
        dz += 2.0f * (float)a1 / 40.0f;
    }
    if (SzPad_IsPress(0)) {
        rx += VIEW3D_STEP * (float)(unsigned int)SzPad_GetPressInfo(0, 1) / 64.0f;
        rx -= VIEW3D_STEP * (float)(unsigned int)SzPad_GetPressInfo(0, 0) / 64.0f;
        ry += VIEW3D_STEP * (float)(unsigned int)SzPad_GetPressInfo(0, 2) / 64.0f;
        ry -= VIEW3D_STEP * (float)(unsigned int)SzPad_GetPressInfo(0, 3) / 64.0f;
        dz += 2.0f * (float)(unsigned int)SzPad_GetPressInfo(0, 8) / 64.0f;
        dz -= 2.0f * (float)(unsigned int)SzPad_GetPressInfo(0, 9) / 64.0f;
    } else {
        if (SzPad_Check(0, 1, 0x8000)) {
            rx += VIEW3D_STEP;
        }
        if (SzPad_Check(0, 1, 0x2000)) {
            rx -= VIEW3D_STEP;
        }
        if (SzPad_Check(0, 1, 0x1000)) {
            ry += VIEW3D_STEP;
        }
        if (SzPad_Check(0, 1, 0x4000)) {
            ry -= VIEW3D_STEP;
        }
        if (SzPad_Check(0, 1, 4)) {
            dz += 2.0f;
        }
        if (SzPad_Check(0, 1, 8)) {
            dz -= 2.0f;
        }
    }
    if (!(VIEW3D_ABS(rx) <= 0.000001f) || !(VIEW3D_ABS(ry) <= 0.000001f) ||
        !(VIEW3D_ABS(dz) <= 0.000001f)) {
        float dist;

        look = *(struct DotrView3dVector *)SzView_GetLook(0);
        dist = (float)SzDuel_GetCamDist(SzDuel_Xax2New(SzModel_GetModelNo(29)));
        look.x = VIEW3D_MIN(VIEW3D_MAX(look.x + dz, dist), 300.0f);
        look.y = func_001a6c30(6.2831855f + (look.y + rx), 6.2831855f);
        look.z = VIEW3D_MIN(VIEW3D_MAX(look.z + ry, -0.3926991f), 1.3744469f);
        SzView_SetLookSP(0, &look);
        gSzDuel_PointLight.position = *(struct DotrView3dVector *)SzView_GetSP(0);
        SzView_SetPointLight(0, 4, &gSzDuel_PointLight);
    }
}
