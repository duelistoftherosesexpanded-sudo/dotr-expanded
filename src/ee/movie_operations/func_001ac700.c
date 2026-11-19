/* Evidence: docs/modules/game-modes.md. Names/signatures provisional. */
#include "dotr/ee/movie_operations.h"

/* The target contains the body of SzGame_Push (0x1a8870) three times with a constant mode
 * and a zero second argument; this copy of that source lets the compiler inline it. */
static inline void SzGame_Push_inline(int arg0, int arg1)
{
    struct DotrMovieOperationsStatePrefix *state = &gSzGame_gl_operations;
    if (state->field_222 + 1 < 8) {
        SzFade_Entry(0, 1, 1, 0, 60, 0);
        state->field_21f = -2;
        state->field_220 = arg0;
        state->field_221 = 1;
        ((signed char *)(state->field_221 + (state->field_222 + (DotrMovieAddress)state)))[0x1f9] = arg1;
        if (arg1 < 2)
            SzGame_ReqBGM(-2, 1);
    }
}

void lSzGame_ActTitle(void)
{
    struct DotrMovieOperationsStatePrefix *p = &gSzGame_gl_operations;
    int v;

    switch (SzGame_ActMemCardProc()) {
    case 0:
    case 2:
        return;
    case 1:
        switch (p->field_225) {
        case 1:
            p->field_164 = 1;
            SzGame_Push_inline(11, 0);
            break;
        case 2:
            gSzGame_gl_operations.field_15c = (p->field_21c >> 8) - 1;
            SzGame_Push_inline(9, 0);
            break;
        case 3:
            SzGame_Push_inline(8, 0);
            break;
        }
        SzSound_SetOutput(SzData_GetOption(0, 2));
        return;
    }
    if (p->field_22f.field_1) {
        if (p->field_224 >= 128) {
            p->field_227 += p->field_22c.unknown_0 * 4;
            if (p->field_227 <= 128 || p->field_227 >= 240)
                p->field_22c.unknown_0 = -p->field_22c.unknown_0;
            if (func_00197548(data_002be80f) != 0) {
                if (++p->field_008 >= 900) {
                    gSzGame_gl_operations.field_160 = 0;
                    SzFade_Entry(0, 1, 1, 0, 60, 0);
                    gSzGame_gl_operations.field_21f = -2;
                    gSzGame_gl_operations.field_220 = 10;
                    gSzGame_gl_operations.field_221 = 0;
                    (gSzGame_gl_operations.bytes_1f9 + gSzGame_gl_operations.field_222)[gSzGame_gl_operations.field_221] = 0;
                    SzGame_ReqBGM(-2, 1);
                    return;
                }
            }
            if (SzPad_Check(0, 0, 2048)) {
                SzSound_PlaySE(0, 4);
                p->field_22f.field_1 = 0;
            }
        } else {
            if (SzSound_IsStandby() && !SzSound_IsPlaySE())
                return;
            v = p->field_224 + 4;
            v = v < 128 ? v : 128; p->field_224 = v;
        }
    } else if (p->field_226 >= 128) {
        if (SzPad_Check(0, 2, 20480)) {
            if (SzPad_Check(0, 2, 4096))
                p->field_225 = (p->field_225 + 3) % 4;
            else
                p->field_225 = (p->field_225 + 5) % 4;
            SzSound_PlaySE(0, 3);
            return;
        }
        if (SzPad_Check(0, 0, 192)) {
            SzSound_PlaySE(0, 4);
            switch (p->field_225) {
            case 0:
                p->field_164 = 0;
                SzGame_Push_inline(11, 0);
                SzData_SetOption(0, 0, 1);
                SzData_SetOption(0, 1, 1);
                SzData_SetOption(0, 2, 1);
                break;
            case 1:
                SzGame_InitMemCardProc(0);
                break;
            case 2:
                SzGame_InitMemCardProc(3);
                break;
            case 3:
                SzGame_InitMemCardProc(4);
                break;
            }
            /* MATCHING DEVICE: this return keeps the branch after the switch separate from
             * the function end, as in the target. */
            return;
        }
    } else {
        v = p->field_226 + 4;
        v = v < 128 ? v : 128; p->field_226 = v;
    }
}
