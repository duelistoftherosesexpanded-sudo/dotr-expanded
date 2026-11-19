#include "dotr/ee/movie_operations.h"

/* SzGame_ModeAct: one frame of the current game mode (SzGame_GetMode(0)).
 * The two-bit status in the state's byte 0x22c runs a mode's start-up first: at 3 it waits
 * for SzFile_InqRequestEx, requests the mode's music when the word at 0x190 is set and puts
 * 128 in byte 0x228 when bit 4 of byte 0x22e is set; at 2 it waits for SzSound_IsLoadingBGM
 * when bit 5 of byte 0x22e is set and loads the text sets of mode 1 (4, 1, 2, 3) or of modes
 * 6 and 8 (0); both then lower the status and return. At 1 it waits for byte 0x228 to reach
 * zero, then clears fade pause 1 and the word at 0xc.
 * After that, every frame: the battle entries' and the effects' sound requests while
 * SzSound_IsStandby, SzModel_Motion for the 84 models, SzEffect_TrimModule, and, unless fade
 * 1 is running, the mode's own Act function. Mode 17 is a pad-driven adjustment screen: it
 * toggles bit 0 of byte 0x22f, steps the words at 0x214 / 0x218 and the four bytes at 0x20c,
 * and leaves through the body of SzGame_Pop, which the compiler inlined here.
 * The pointer local and the direct accesses follow the original's addressing. */
void SzGame_ModeAct(void)
{
    struct DotrMovieOperationsStatePrefix *state = &gSzGame_gl_operations;
    int mode = SzGame_GetMode(0);
    int i;
    int step;

    switch (gSzGame_gl_operations.field_22c.field_4) {
    case 3:
        if (SzFile_InqRequestEx()) {
            return;
        }
        if (state->field_190) {
            SzGame_ReqBGM(mode, 1);
        }
        if (state->field_22e.field_4) {
            *(unsigned char *)&state->field_228 = 128;
        }
        state->field_22c.field_4--;
        return;
    case 2:
        if ((gSzGame_gl_operations.field_22e.field_5 ? 1 : 0) && SzSound_IsLoadingBGM()) {
            return;
        }
        switch (mode) {
        case 1:
            SzText_LoadMerge(4, 1);
            SzText_LoadMerge(1, 0);
            SzText_LoadMerge(2, 0);
            SzText_LoadMerge(3, 0);
            break;
        case 6:
        case 8:
            SzText_LoadMerge(0, 0);
            break;
        }
        state->field_22c.field_4--;
        return;
    case 1:
        if (*(unsigned char *)&state->field_228) {
            return;
        }
        SzFade_SetPauseFlag(1, 0);
        state->field_00c = 0;
        break;
    }
    if (SzSound_IsStandby()) {
        for (i = 0; i < 2; i++) {
            int entry = SzBattle_InqEntry(i);

            if (entry >= 0) {
                SzModel_ReqSE(i + 2, SzDuel_GetUnitModelID(entry >> 8, entry & 0xff));
            }
        }
        SzEffect_ReqSE();
    }
    for (i = 0; i < 84; i++) {
        SzModel_Motion(i);
    }
    SzEffect_TrimModule();
    if (SzFade_IsDoing(1)) {
        return;
    }
    switch (mode) {
    case -3:
        break;
    case -2:
    case -1:
        SzGame_ActMenu();
        break;
    case 0:
        SzDebug_Act();
        if (!SzDebug_IsEffect()) {
            for (i = 0; i < 84; i++) {
                SzModel_Act(i);
            }
        }
        SzView_Act(0);
        break;
    case 1:
        while (SzDuel_Act()) {
        }
        break;
    case 5:
        SzDebug_Act();
        if (!SzDebug_IsEffect()) {
            for (i = 0; i < 84; i++) {
                SzModel_Act(i);
            }
        }
        SzView_Act(0);
        break;
    case 6:
        SzDuel_ActBuildDeck();
        break;
    case 7:
        SzDuel_Act3DView();
        break;
    case 8:
        SzDuel_ActTrade();
        break;
    case 9:
        SzDuel_ActCustomDuel();
        break;
    case 36:
        lSzGame_ActMemCheck();
        break;
    case 10:
        lSzGame_ActTitle();
        break;
    case 11:
        TkCamp_Act();
        ShFace_Act();
        break;
    case 12:
        SzDebug_Act();
        if (!SzDebug_IsEffect()) {
            for (i = 0; i < 84; i++) {
                SzModel_Act(i);
            }
        }
        SzView_Act(0);
        break;
    case 13:
        SzDuel_ActMapEdit();
        break;
    case 18:
        SzDuel_ActCardTest();
        break;
    case 19:
        SzView_ActCamera();
        SzView_Act(0);
        break;
    case 14:
        ShFace_Act();
        break;
    case 15:
        ShSlot_Act();
        break;
    case 17:
        step = 1;
        if (SzPad_Check(0, 0, 16)) {
            state->field_22f.field_0 = state->field_22f.field_0 ? 0 : 1;
        }
        if (SzPad_Check(0, 1, 32)) {
            step = 2;
        }
        if (SzPad_Check(0, 1, 128)) {
            step = 2;
        }
        if (SzPad_Check(0, 1, 0xf000)) {
            if (SzPad_Check(0, 1, 0x8000)) {
                state->field_214 -= step;
            }
            if (SzPad_Check(0, 1, 0x2000)) {
                state->field_214 += step;
            }
            if (SzPad_Check(0, 1, 0x1000)) {
                state->field_218 -= step;
            }
            if (SzPad_Check(0, 1, 0x4000)) {
                state->field_218 += step;
            }
        }
        if (SzPad_Check(1, 1, 0x1000)) {
            if (SzPad_Check(1, 2, 128)) {
                if (state->bytes_20c[3] > 0) {
                    state->bytes_20c[3]--;
                }
            }
            if (SzPad_Check(1, 2, 32)) {
                if (state->bytes_20c[3] < 255) {
                    state->bytes_20c[3]++;
                }
            }
        } else {
            if (SzPad_Check(1, 2, 129)) {
                if (state->bytes_20c[2] > 0) {
                    state->bytes_20c[2]--;
                }
            }
            if (SzPad_Check(1, 2, 34)) {
                if (state->bytes_20c[2] < 255) {
                    state->bytes_20c[2]++;
                }
            }
            if (SzPad_Check(1, 2, 65)) {
                if (state->bytes_20c[1] > 0) {
                    state->bytes_20c[1]--;
                }
            }
            if (SzPad_Check(1, 2, 18)) {
                if (state->bytes_20c[1] < 255) {
                    state->bytes_20c[1]++;
                }
            }
            if (SzPad_Check(1, 2, 5)) {
                if (state->bytes_20c[0] > 0) {
                    state->bytes_20c[0]--;
                }
            }
            if (SzPad_Check(1, 2, 10)) {
                if (state->bytes_20c[0] < 255) {
                    state->bytes_20c[0]++;
                }
            }
        }
        if (SzPad_Check(0, 0, 32)) {
            struct DotrMovieOperationsStatePrefix *pop = &gSzGame_gl_operations;

            if (pop->field_222 - 1 >= 0) {
                SzFade_Entry(0, 1, 1, 0, 60, 0);
                pop->field_21f = -2;
                pop->field_220 = ((signed char *)(pop->field_222 + (DotrMovieAddress)pop))[0x1f0];
                pop->field_221 = -1;
                if (((signed char *)(pop->field_222 + (DotrMovieAddress)pop))[0x1f1] < 2)
                    SzGame_ReqBGM(-2, 1);
            }
        }
        break;
    }
}
