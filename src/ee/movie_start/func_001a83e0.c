/* Evidence: docs/modules/game-modes.md. All names and signatures provisional. */
#include "dotr/ee/movie_start.h"
extern int SzSound_IsStandby(void);
extern int SzFile_InqRequestEx(void);
extern int SzFile_DoRequestS(void);
extern void SD_StopAll(void);
extern void SD_SuspendSoundSystem(int);
extern void SzSound_Term(void);
extern int SzModel_GetModelNo(int);
extern void SzModel_Preset(int, int, int);
extern void RhBase_FlipBuffer(void);
extern void RhBase_ResetGsEnv(int);
extern int movie_play(const char *, int, int);
extern void func_001854e0(int);
extern void SzGame_ClearGsMem(int, int, int, int, int, int);
extern void SzSound_Init(void);

int SzGame_PlayMovie(const char *path, int arg1, int arg2)
{
    int active = SzSound_IsStandby();
    int i;
    int result;
    while (SzFile_InqRequestEx() != 0) {
        SzFile_DoRequestS();
    }
    if (active != 0) {
        SD_StopAll();
        SD_SuspendSoundSystem(1);
        SzSound_Term();
    }
    for (i = 0; i < 10; i++) {
        SzModel_Preset(i + 20, -1, SzModel_GetModelNo(i + 20));
    }
    RhBase_FlipBuffer();
    RhBase_ResetGsEnv(1);
    result = movie_play(path, arg1, arg2);
    func_001854e0(0);
    RhBase_ResetGsEnv(0);
    SzGame_ClearGsMem(10, 0, 0, 0, 1024, 1024);
    if (active != 0) {
        SD_SuspendSoundSystem(0);
        SzSound_Init();
    }
    return result;
}
