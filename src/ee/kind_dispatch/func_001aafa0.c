#include "dotr/ee/kind_dispatch.h"

void SzGame_InitMemCardProc(int kind)
{
    struct DotrKindStateView *view = (struct DotrKindStateView *)&gSzGame_gl_digits;

    switch (kind) {
    case 0:
        TkMc_Load_Main(1);
        view->state = 2;
        break;
    case 1: {
        char name[128];
        func_0019b648(name, data_002be821, SzFile_GetEnv(data_002be7d0));
        SzFile_ReadRequestParam(0, 0, name, 0x1e301e0, -1, 0, -1, 0, 0);
        view->state = 1;
        break;
    }
    case 2: {
        char name[128];
        func_0019b648(name, data_002be821, SzFile_GetEnv(data_002be7d0));
        SzFile_ReadRequestParam(0, 0, name, 0x1e301e0, -1, 0, -1, 0, 0);
        view->state = 1;
        break;
    }
    case 3:
        TkMc_Custom_Main(1);
        view->state = 1;
        break;
    case 4:
        TkMc_Trade_Load_Main(1);
        view->state = 1;
        break;
    case 5:
        TkMc_Trade_Save_Main(1);
        view->state = 1;
        break;
    case 6:
        TkMc_Check_Main(1);
        view->state = 1;
        break;
    }
    view->word_21c = 0;
    view->kind = kind;
    view->second = kind == 2 ? 1 : 0;
}
