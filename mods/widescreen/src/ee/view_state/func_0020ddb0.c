#include "dotr/ee/view_state.h"

int RhView_Init(void)
{
    int w;
    int h;

    gRhView_nStackIndex[0] = 0;
    gRhView_FakeMirror[0].unknown_44 = 0;
    RhBase_GetScreenSize(&w, &h);
    gRhView_avl[0].unknown_160 = 400.0f;
    gRhView_avl[0].unknown_164 = (float)w;
    gRhView_avl[0].unknown_168 = (float)h;
    gRhView_avl[0].unknown_150 = gRhView_avl[0].unknown_164 / 2.0f;
    gRhView_avl[0].unknown_154 = gRhView_avl[0].unknown_168 / 2.0f;
    gRhView_avl[0].unknown_158 = 0.75f; /* widescreen: 3/4 of the original 1.0f, as in RhView_SetAspect */
    gRhView_avl[0].unknown_15c = 0.95f;
    gRhView_avl[0].unknown_16c = 1.0f;
    gRhView_avl[0].unknown_170 = 10000.0f;
    gRhView_avl[0].unknown_174 = gRhView_avl[0].unknown_170 - 1000.0f;
    gRhView_avl[0].unknown_178 = gRhView_avl[0].unknown_170;
    gRhView_avl[0].unknown_17c = 0.0f;
    gRhView_avl[0].unknown_180 = 1000.0f;
    /* The four 255 bytes of the white fog colour as one word: makes room for the second constant. */
    *(unsigned int *)&gRhView_avl[0].unknown_184 = 0xffffffff;
    RhView_Reset((struct DotrViewFrame *)&gRhView_avl[0].unknown_0c0);
    return 1;
}
