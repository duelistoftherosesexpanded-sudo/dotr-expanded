/* Evidence: docs/modules/game-modes.md. Dependency names and signatures
 * are provisional. */
#include "dotr/ee/movie_update.h"

extern char data_002be80f[];
extern char data_002be809[];
extern const char *data_0028ff10[];
extern char *func_00197548(const char *);
extern char *SzFile_GetEnv(const char *);
extern int func_0019b648(char *, const char *, ...);
extern int SzGame_PlayMovie(const char *, int, int);
extern void SzModel_LoadMerge(int, int, int);
extern void SzDebug_SetHideFlag(int);
extern int SzData_GetCurrentDeck(int);
extern int SzData_GetLeader(int, int);
extern void SzDuel_Initialize(void);
extern void SzDuel_AddUnit(int, int, int, int, int);
extern void SzBattle_Init(int);
extern void SzBattle_Entry(int, int);
extern void SzDuel_SetBusyFlag(int, int, int);
extern void SzView_InitCamera(int);
extern void SzText_LoadMerge(int, int);
extern void ShSlot_Init(int);
extern void ShFace_Init(int);
extern void SzDuel_InitMapEdit(int);
extern void SzEvent_Init(int);
extern void SzData_SetRoseFlag(int);
extern void SzDuel_SetUnitStatus(int, int, int, int);
extern void SzDuel_SetUnitOpenRate(int, int, int, int);
extern void SzDuel_SetUnitCtlRateM(int, int, int, int);
extern void TkCamp_Init(int);
extern void lSzGame_InitTitle(int);
extern void SzSound_TransferSEGrp(int, int, int);
extern void lSzGame_InitMemCheck(void);
extern void SzDuel_InitCustomDuel(int);
extern void SzDuel_InitTrade(void);
extern void SzDuel_Init3DView(int);
extern void SzDuel_InitBuildDeck(int);
extern void SzModel_SetDrawFlag(int, int);
extern void SzDuel_SetupDuel(int, int);
extern void SzDuel_SetPlayer(int, unsigned int);
extern void SzDuel_Init(int, int, int);

void SzGame_ModeInit(int GameMode)
{
    struct MovieUpdatePrefix *state = &dotr_ee_movie_update_state_00317250;
    int value_0, value_1, result_0, result_1;
    int i, is_one;
    char path[128];
    switch (GameMode) {
    case -1:
        for (i = 0; i < 84; ++i)
            SzModel_SetDrawFlag(i, 0);
        state->bits_22e.bit_7 = -1;
        SzSound_TransferSEGrp(0, 0, 0);
        break;
    case 0:
        SzText_LoadMerge(1, 0);
        /* falls through */
    case 1:
        if (GameMode == 1)
            is_one = 1;
        else
            is_one = 0;
        SzData_SetRoseFlag(state->menu_nums[2]);
        SzText_LoadMerge(11, 0);
        SzText_LoadMerge(12, 0);
        SzDuel_SetupDuel(1, state->menu_nums[1]);
        SzDuel_SetPlayer(0, state->menu_nums[3] % 1000);
        SzDuel_SetPlayer(1, state->menu_nums[4] % 1000);
        SzDuel_Init(state->menu_nums[1], state->menu_nums[2], is_one);
        break;

    case 5: {
        SzText_LoadMerge(12, 0);
        SzModel_LoadMerge(1, 0, 0);
        SzModel_LoadMerge(2, state->menu_nums[GameMode], 0);
        value_0 = SzData_GetCurrentDeck(0);
        value_1 = SzData_GetCurrentDeck(1);
        result_0 = SzData_GetLeader(0, value_0);
        result_1 = SzData_GetLeader(1, value_1);
        if (result_0 == 999)
            result_0 = 0;
        if (result_1 == 999)
            result_1 = 0;
        SzDuel_Initialize();
        SzDuel_AddUnit(0, 3, 6, result_0, 1);
        SzDuel_AddUnit(1, 3, 0, result_1, 1);
        SzBattle_Init(state->menu_nums[GameMode]);
        SzBattle_Entry(0, 0);
        SzDuel_SetBusyFlag(0, 0, 0);
        SzBattle_Entry(1, 256);
        SzDuel_SetBusyFlag(1, 0, 0);
        break;
    }
    case 6: {
        SzData_SetRoseFlag(0);
        SzText_LoadMerge(11, 0);
        SzText_LoadMerge(4, 1);
        SzDuel_InitBuildDeck(state->menu_nums[GameMode]);
        break;
    }
    case 7: {
        SzModel_LoadMerge(4, 1, 0);
        if (state->byte_21e == -1)
            SzDuel_Init3DView(state->menu_nums[GameMode]);
        SzDuel_Init3DView(-1);
        break;
    }
    case 8: {
        SzData_SetRoseFlag(0);
        SzText_LoadMerge(11, 0);
        SzDuel_InitTrade();
        break;
    }
    case 9: {
        SzData_SetRoseFlag(0);
        SzText_LoadMerge(11, 0);
        SzDuel_InitCustomDuel(state->menu_nums[GameMode]);
        break;
    }
    case 36: {
        SzText_LoadMerge(11, 0);
        lSzGame_InitMemCheck();
        break;
    }
    case 10: {
        SzText_LoadMerge(11, 0);
        SzText_LoadMerge(15, 0);
        if (func_00197548(data_002be80f) != 0 && state->menu_nums[GameMode] == 0) {
            int selected = dotr_ee_movie_update_state_00317250.byte_230;
            char *directory = SzFile_GetEnv(data_002be80f);
            func_0019b648(path, data_002be809, directory, data_0028ff10[selected]);
            SzGame_PlayMovie(path, 1, 1);
            dotr_ee_movie_update_state_00317250.byte_230 =
                (dotr_ee_movie_update_state_00317250.byte_230 + 1) % 2;
        }
        lSzGame_InitTitle(state->menu_nums[GameMode]);
        SzModel_LoadMerge(5, 3, 1);
        SzSound_TransferSEGrp(0, 0, 0);
        break;
    }
    case 11: {
        SzData_SetRoseFlag(0);
        dotr_ee_movie_update_state_00317250.menu_nums[28] = 3;
        dotr_ee_movie_update_state_00317250.menu_nums[29] = 3;
        SzText_LoadMerge(11, 0);
        SzText_LoadMerge(16, 0);
        SzText_LoadMerge(13, 0);
        SzText_LoadMerge(14, 0);
        SzText_LoadMerge(5, 1);
        SzDuel_Initialize();
        SzDuel_AddUnit(0, 3, 6, 0, 1);
        SzDuel_SetUnitStatus(0, 0, 4, 1);
        SzDuel_SetUnitOpenRate(0, 0, 0, 255);
        SzDuel_SetUnitCtlRateM(0, 0, 1, 0);
        SzEvent_Init(2);
        TkCamp_Init(state->menu_nums[GameMode]);
        ShFace_Init(0);
        break;
    }
    case 12: {
        SzModel_LoadMerge(3, state->menu_nums[GameMode], 0);
        SzEvent_Init(state->menu_nums[GameMode]);
        break;
    }
    case 13: {
        SzModel_LoadMerge(6, 0, 0);
        SzModel_LoadMerge(0, 0, 0);
        SzDuel_InitMapEdit(state->menu_nums[GameMode]);
        break;
    }
    case 14: {
        SzText_LoadMerge(13, 0);
        ShFace_Init(1);
        break;
    }
    case 15: {
        SzText_LoadMerge(11, 0);
        SzText_LoadMerge(12, 0);
        SzText_LoadMerge(17, 0);
        if (state->menu_nums[GameMode] != 0)
            SzText_LoadMerge(0, 0);
        ShSlot_Init(state->menu_nums[GameMode]);
        state->menu_nums[GameMode] = 0;
        break;
    }
    case 18: {
        SzText_LoadMerge(11, 0);
        SzText_LoadMerge(0, 0);
        break;
    }
    case 19: {
        SzModel_LoadMerge(1, 0, 0);
        SzModel_LoadMerge(2, state->menu_nums[5], 0);
        SzDebug_SetHideFlag(0);
        value_0 = SzData_GetCurrentDeck(0);
        value_1 = SzData_GetCurrentDeck(1);
        result_0 = SzData_GetLeader(0, value_0);
        result_1 = SzData_GetLeader(1, value_1);
        if (result_0 == 999)
            result_0 = 0;
        if (result_1 == 999)
            result_1 = 0;
        SzDuel_Initialize();
        SzDuel_AddUnit(0, 3, 6, result_0, 1);
        SzDuel_AddUnit(1, 3, 0, result_1, 1);
        SzBattle_Init(state->menu_nums[5]);
        SzBattle_Entry(0, 0);
        SzDuel_SetBusyFlag(0, 0, 0);
        SzBattle_Entry(1, 256);
        SzDuel_SetBusyFlag(1, 0, 0);
        SzView_InitCamera(state->menu_nums[GameMode]);
        break;
    }
    default:
        break;
    }
}
