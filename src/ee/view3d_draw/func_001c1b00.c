#include "dotr/ee/view3d_draw.h"

/* Draws the duel's 3D view. When model 29 is displayed (SzModel_IsModelDisp) it first
 * calls SzView_GetLook(0) and SzModel_GetRadius(29), ignoring both results. On the first
 * pass (control byte +0xd35 zero) and with SzModel_GetCenterOfs succeeding, the model is
 * moved to minus its centre offset, the look vector (data_00291480 with x replaced by the
 * camera distance of the model's number) is set, and the point light gets that distance at
 * +0x20, twice it at +0x24 and view 0's SP vector as position; the byte is then
 * incremented whether or not the centre was read. On later passes, with SzModel_GetCenter
 * succeeding, the camera point's y moves toward the centre's y by at most 1 per call.
 * Then, whatever the model's state: model 38 at y = 2500 down to -2500 in steps of 500;
 * model 36 at the same eleven heights with twelve rotations (y angle j * 0x10000 / 12);
 * model 37 at y 0 with six rotations; ten fade boards (alpha 16, 1500 x 1500) at y 2500
 * down to 2100 and -2500 up to -2100 in steps of 100; models 34 and 35 at y -500; and
 * model 29 through SzModel_Draw_Pipe, whose result is returned. The cursor is threaded
 * through every draw. */
#define VIEW3D_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define VIEW3D_MIN(a, b) (((a) < (b)) ? (a) : (b))

int SzDuel_Draw3DView_Pipe(int cursor)
{
    struct DotrView3dControl *control = &data_00317490;
    struct DotrView3dVector pos = data_00291460;
    struct DotrView3dAngles rot = data_00291470;
    struct DotrView3dVector centre;
    struct DotrView3dVector cp;
    struct DotrView3dVector look;
    float dist;
    int i;
    int j;

    if (SzModel_IsModelDisp(29)) {
        SzView_GetLook(0);
        SzModel_GetRadius(29);
        if (control->unknown_d35 != 0) {
            if (SzModel_GetCenter(29, &centre)) {
                cp = *(struct DotrView3dVector *)SzView_GetCP(0);
                cp.y += VIEW3D_MIN(VIEW3D_MAX(centre.y - cp.y, -1.0f), 1.0f);
                SzView_SetCP(0, &cp);
            }
        } else {
            if (SzModel_GetCenterOfs(29, &centre)) {
                dist = (float)SzDuel_GetCamDist(SzDuel_Xax2New(SzModel_GetModelNo(29)));
                look = data_00291480;
                centre.x *= -1.0f;
                centre.y *= -1.0f;
                centre.z *= -1.0f;
                look.x = dist;
                SzModel_SetPos(29, &centre);
                SzView_SetLookSP(0, &look);
                gSzDuel_PointLight.unknown_20 = dist;
                gSzDuel_PointLight.unknown_24 = 2.0f * dist;
                gSzDuel_PointLight.position = *(struct DotrView3dVector *)SzView_GetSP(0);
                SzView_SetPointLight(0, 4, &gSzDuel_PointLight);
            }
            control->unknown_d35++;
        }
    }
    for (i = 5; i >= -5; i--) {
        pos.y = (float)(i * 500);
        cursor = SzModel_DrawWithOutCalc_Pipe(cursor, 38, &pos, 0, 0, 0, 0);
    }
    for (i = 5; i >= -5; i--) {
        pos.y = (float)(i * 500);
        for (j = 0; j < 12; j++) {
            rot.y = j * 0x10000 / 12;
            cursor = SzModel_DrawWithOutCalc_Pipe(cursor, 36, &pos, &rot, 0, 0, 0);
        }
    }
    pos.y = 0.0f;
    for (j = 0; j < 6; j++) {
        rot.y = j * 0x10000 / 6;
        cursor = SzModel_DrawWithOutCalc_Pipe(cursor, 37, &pos, &rot, 0, 0, 0);
    }
    for (i = 0; i < 5; i++) {
        pos.y = (float)(2500 - i * 100);
        cursor = lSzDuel_DrawFadeBoard_Pipe(cursor, &pos, 16, 1500.0f, 1500.0f);
    }
    for (i = 0; i < 5; i++) {
        pos.y = (float)(-2500 + i * 100);
        cursor = lSzDuel_DrawFadeBoard_Pipe(cursor, &pos, 16, 1500.0f, 1500.0f);
    }
    pos.y = -500.0f;
    cursor = SzModel_DrawWithOutCalc_Pipe(cursor, 34, &pos, 0, 0, 0, 0);
    cursor = SzModel_DrawWithOutCalc_Pipe(cursor, 35, &pos, 0, 0, 0, 0);
    return SzModel_Draw_Pipe(cursor, 29, 0, 0, 0, 0, 0);
}
