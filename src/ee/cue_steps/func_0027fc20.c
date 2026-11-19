#include "dotr/ee/cue_steps.h"

/* One step of the phase word at +0x160. 0xffff starts at `first`. At `first`: once
 * TaTuto_OpenMsgPopup reports 1 and SzPad_Check(0, 0, 240) is set, cue 4 and phase 0xfffe when
 * first == last, else cue 3 and the next phase. At 0xfffe: once TaTuto_CloseMsgPopup reports 1 the
 * phase returns to 0xffff, +0x1a2 is incremented, +0x1a4 cleared and 1 returned. Any other
 * phase: SzDuel_OpenMsgPopup(1, 0, 0, TaTuto_GetCardNameMsgID()) and the same cue step against `last`.
 * Returns 0 otherwise. */
int TaTuto_OpenTutoMsg(int first, int last, struct DotrCueStepRecord *rec)
{
    if (rec->unknown_160 == 0xffff) {
        rec->unknown_160 = first;
    }
    if (rec->unknown_160 == first) {
        if (func_0027fb30_id(rec->unknown_160, &rec->unknown_1a4) == 1 && SzPad_Check(0, 0, 240) != 0) {
            if (first == last) {
                SzSound_PlaySE(0, 4);
                rec->unknown_160 = 0xfffe;
            } else {
                SzSound_PlaySE(0, 3);
                rec->unknown_160++;
            }
        }
    } else if (rec->unknown_160 == 0xfffe) {
        if (func_0027fba0_id(last, &rec->unknown_1a4) == 1) {
            rec->unknown_160 = 0xffff;
            rec->unknown_1a2++;
            rec->unknown_1a4 = 0;
            return 1;
        }
    } else {
        SzDuel_OpenMsgPopup(1, 0, 0, func_0027fda0_void());
        if (SzPad_Check(0, 0, 240) != 0) {
            if (rec->unknown_160 == last) {
                SzSound_PlaySE(0, 4);
                rec->unknown_160 = 0xfffe;
            } else {
                SzSound_PlaySE(0, 3);
                rec->unknown_160++;
            }
        }
    }
    return 0;
}
