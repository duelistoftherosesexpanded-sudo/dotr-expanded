/* Synthetic host tests for the view_requests group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/view_requests.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

struct DotrViewReqSlot gSzView_CameraReq[8];
void *gSzView_hView_viewreq[1];
struct DotrViewReqVector4 gSzView_pntSPDef, gSzView_pntCPDef;
static struct DotrViewReqCamera camera;

int SzGame_GetFrameSpeed(void) { return 2; }
int lSzView_CalcRel(int arg0) { return 0; }
int lSzView_CalcLook(int arg0) { return 0; }
int SzView_SetLookSP(int index, float *src) { return 0; }
int SzView_SetLookCP(int index, float *src) { return 0; }
int SzView_SetViewCamera(int index, int otherIndex, float *target, int mode, float weight) { return 0; }
int SzView_SetInterpCameraMotion(int index, void *motion, int duration, int frame, int parent, int mirror) { return 0; }

int main(void)
{
    int v[4];
    float f[4] = { 7.9f, 3.1415927f, -1.5707964f, -2.5f };

    /* SzView_DoRequest: a field-of-view request (type 0) from 100 to 200 over 4 frames at
     * timer 2 advances by the frame speed 2 to its end: fov 200 and the slot is released. */
    gSzView_hView_viewreq[0] = &camera;
    gSzView_CameraReq[0].active = -1; /* the one-bit signed flag set */
    gSzView_CameraReq[0].type = 0;
    gSzView_CameraReq[0].count = 4;
    gSzView_CameraReq[0].timer = 2;
    gSzView_CameraReq[0].from.i = 100;
    gSzView_CameraReq[0].to.i = 200;
    SzView_DoRequest(-1, -1);
    assert(camera.fov == 200.0f && gSzView_CameraReq[0].timer == 4 && gSzView_CameraReq[0].active == 0);

    /* SzView_V2IV: components 0 and 3 truncated, 1 and 2 as angles (pi -> 32768). */
    SzView_V2IV(f, v);
    assert(v[0] == 7 && v[1] == 32768 && v[2] == -16384 && v[3] == -2);
    return 0;
}
