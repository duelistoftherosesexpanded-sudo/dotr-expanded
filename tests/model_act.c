/* Synthetic host tests for the model_act group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/model_act.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

struct DotrModelActObject *gSzModel_ahModel_modelact[0x54];
unsigned char iColF[4], iColT[4];
unsigned short wFrame, wAlphaMode;
int nCameraCurrent, nEffectCurrent, gSzModel_fRefLock_word, gSzModel_nRefLock_word, nBlurUse, nBlurCur, data_0031bb48;
struct DotrModelActDigits4 data_00292f90, data_00292fa0;
struct DotrModelActDigits3 data_00292fb0, data_00292fc0;
char data_002cfa17[4], data_002cfb7d[4];

static int mode_arg = -1;

int SzPad_Check(int arg0, int arg1, int arg2) { return 0; }
int func_0019b648(char *str, const char *fmt, ...) { return 0; }
int SzGame_GetFrameCount(void) { return 0; }
long long SzGame_GetMode(int arg0)
{
    mode_arg = arg0;
    return 0;
}
int SzDuel_SetUnitStatus(int arg0, int arg1, int arg2, int arg3) { return 0; }
void lSzModel_LoadCameraCB(void *arg0, int arg1) { }
int SzModel_BattleSeq(int arg0, int arg1, int arg2) { return 0; }
int SzModel_SetAnim(int arg0, int arg1) { return 0; }
int SzModel_SetAnimIntr(int arg0, int arg1) { return 0; }
int SzModel_SetAnimDirect(int arg0, int arg1) { return 0; }
int SzModel_SetAnimDir(int arg0, long long arg1) { return 0; }
int SzModel_SetAnimSpeed(int arg0, float value) { return 0; }
int SzModel_SetIntensity(int arg0, float *arg1) { return 0; }
int SzModel_SetScale(int idx, void *src) { return 0; }
int SzModel_SetRgba(int arg0, int arg1, void *arg2) { return 0; }
int SzModel_SetFilter(int arg0, int arg1, void *arg2) { return 0; }
void SzEffect_Clear(void) { }
int SzEffect_EntryCallback(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) { return 0; }
int SzEffect_Destroy(int arg0) { return 0; }
int SzEffect_Destroy2(int arg0, int arg1) { return 0; }
int SzView_FreeRequest(int arg0, int arg1) { return 0; }
int SzView_ReqCamera(int owner, int target, int type, int count, int value, void *data) { return 0; }
int SzFile_InqRequest(int arg0) { return 0; }
int SzFile_DoRequestS(void) { return 0; }
unsigned char *SzFile_GetEnv(int unused0) { return NULL; }
int SzText_AddFile(int arg0, int arg1, char *arg2, int arg3) { return 0; }
int SzText_FreeRequest(int arg0, int arg1) { return 0; }
int SzDebug_GetActiveMenu(void) { return 0; }
int SzDebug_GetActiveModel(void) { return 7; }
int SzDebug_IsHide(void) { return 1; }
void SzMem_Free(int unused0) { }
/* Declared without parameters inside SzModel_Act's body. */
int SzModel_ReqMotionBlur(void) { return 0; }
int SzFile_ReadRequestParam(void) { return 0; }

static union {
    struct DotrModelActDbgObject dbg;
    struct DotrModelActObject obj;
    unsigned char bytes[0x400];
} model;

int main(void)
{
    /* SzModel_Act: a model whose viewer page (0) is not the debug viewer's (7) is left alone; 1. */
    gSzModel_ahModel_modelact[3] = &model.obj;
    assert(SzModel_Act(3) == 1 && mode_arg == 1);

    /* SzModel_SetAnimIntrReverseParam: both levels set, so the two levels swap and the frame
     * byte at 0x1fc becomes 20 - 5 * 20 / 10 with the count byte at 0x1fb set to 20. */
    model.obj.lv[0] = 1;
    model.obj.lv[1] = 2;
    model.bytes[0x1FB] = 10;
    model.bytes[0x1FC] = 5;
    assert(SzModel_SetAnimIntrReverseParam(3, 20) == 1);
    assert(model.obj.lv[0] == 2 && model.obj.lv[1] == 1);
    assert(model.bytes[0x1FB] == 20 && model.bytes[0x1FC] == 10 && model.bytes[0x1FD] == 0);
    return 0;
}
