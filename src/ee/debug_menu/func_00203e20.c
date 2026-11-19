#include "dotr/ee/debug_menu.h"

/* Per-frame debug menu input. The menu page is cleared, or skipped while stepping, when the
 * current model's state, type or the game mode does not allow it; page 0 edits the model number
 * digit by digit and loads it. Calls whose results are dropped (func_0019bef0, the model state
 * queries at the end, the closing division loop) are presumably what is left of removed debug
 * output; that is an inference. `<= 11` and `!(a != 3 && a != 5 && a != 6)` are the spellings
 * that reproduce the original comparisons. */
void SzDebug_Act(void)
{
    struct DotrSmallFlagsOwner *p = &data_00325c80_owner;
    int standby = SzModel_IsModelStandby(p->unknown_00[DOTR_DEBUG_MODEL]);
    int type = SzModel_GetModelType(p->unknown_00[DOTR_DEBUG_MODEL]);
    int menu = SzDebug_GetActiveMenu();
    int mode = SzGame_GetMode(1);

    if (standby == 0 && p->unknown_00[DOTR_DEBUG_MENU] > 0 && p->unknown_00[DOTR_DEBUG_MENU] < 12) {
        p->unknown_00[DOTR_DEBUG_MENU] = 0;
        menu = 0;
    }
    if (p->unknown_00[DOTR_DEBUG_MENU] == 2 && type != 4 && type != 5) {
        p->unknown_00[DOTR_DEBUG_MENU] = 0;
        menu = 0;
    }
    if (!(p->unknown_00[DOTR_DEBUG_MENU] != 3 && p->unknown_00[DOTR_DEBUG_MENU] != 5 && p->unknown_00[DOTR_DEBUG_MENU] != 6) && (type != 5 || mode != 5)) {
        p->unknown_00[DOTR_DEBUG_MENU] = 0;
        menu = 0;
    }
    if (p->unknown_00[DOTR_DEBUG_MENU] == 4 && (type != 6 || mode != 0)) {
        p->unknown_00[DOTR_DEBUG_MENU] = 0;
        menu = 0;
    }
    if (p->unknown_2c.unknown_bit_1) {
        if (SzPad_Check(1, 0, 0x800)) {
            p->unknown_2c.unknown_bit_2 = p->unknown_2c.unknown_bit_2 ? 0 : 1;
        }
    } else if (SzPad_Check(1, 0, 0x800)) {
        SzView_SetMode(0, (SzView_GetMode(0) + 1) % 3);
    }
    if (SzPad_Check(1, 0, 0x100)) {
        p->unknown_2c.unknown_bit_0 = p->unknown_2c.unknown_bit_0 ? 0 : 1;
    }
    if (p->unknown_2c.unknown_bit_2) {
        lSzDebug_ActEffect();
        return;
    }
    if (SzPad_Check(0, 2, 0x100)) {
        for (;;) {
            p->unknown_00[DOTR_DEBUG_MENU] = (p->unknown_00[DOTR_DEBUG_MENU] + 17) % 18;
            if (standby == 0 && p->unknown_00[DOTR_DEBUG_MENU] > 0 && p->unknown_00[DOTR_DEBUG_MENU] <= 11) {
                continue;
            }
            if (p->unknown_00[DOTR_DEBUG_MENU] == 2 && type != 4 && type != 5) {
                continue;
            }
            if (!(p->unknown_00[DOTR_DEBUG_MENU] != 3 && p->unknown_00[DOTR_DEBUG_MENU] != 5 && p->unknown_00[DOTR_DEBUG_MENU] != 6) && (type != 5 || mode != 5)) {
                continue;
            }
            if (p->unknown_00[DOTR_DEBUG_MENU] == 4 && (type != 6 || mode != 0)) {
                continue;
            }
            break;
        }
        SzSound_PlaySE(0, 0);
    }
    if (SzPad_Check(0, 2, 0x800)) {
        for (;;) {
            p->unknown_00[DOTR_DEBUG_MENU] = (p->unknown_00[DOTR_DEBUG_MENU] + 19) % 18;
            if (standby == 0 && p->unknown_00[DOTR_DEBUG_MENU] > 0 && p->unknown_00[DOTR_DEBUG_MENU] <= 11) {
                continue;
            }
            if (p->unknown_00[DOTR_DEBUG_MENU] == 2 && type != 4 && type != 5) {
                continue;
            }
            if (!(p->unknown_00[DOTR_DEBUG_MENU] != 3 && p->unknown_00[DOTR_DEBUG_MENU] != 5 && p->unknown_00[DOTR_DEBUG_MENU] != 6) && (type != 5 || mode != 5)) {
                continue;
            }
            if (p->unknown_00[DOTR_DEBUG_MENU] == 4 && (type != 6 || mode != 0)) {
                continue;
            }
            break;
        }
        SzSound_PlaySE(0, 0);
    }
    if (menu == 0) {
        int no = SzModel_GetModelNo(p->unknown_00[DOTR_DEBUG_MODEL]);
        struct DotrDebugDigits dd = data_00293d28;
        int n;
        /* MATCHING DEVICE: the digits are stored through a walking pointer; indexed stores make
         * the compiler keep the addresses of dd.d[1] and dd.d[2] in saved registers. */
        {
            int *e = dd.d;
            *e++ = (no / 100) % 10;
            *e++ = (no / 10) % 10;
            *e = no % 10;
        }
        n = no;
        if (SzPad_Check(0, 2, 0x5000)) {
            if (SzPad_Check(0, 2, 0x1000)) {
                dd.d[p->unknown_00[DOTR_DEBUG_DIGIT]] = (dd.d[p->unknown_00[DOTR_DEBUG_DIGIT]] + 11) % 10;
            } else {
                dd.d[p->unknown_00[DOTR_DEBUG_DIGIT]] = (dd.d[p->unknown_00[DOTR_DEBUG_DIGIT]] + 9) % 10;
            }
            n = dd.d[0] * 100 + dd.d[1] * 10 + dd.d[2];
        }
        if (SzPad_Check(0, 2, 2)) {
            n = (n + 1001) % 1000;
        }
        if (SzPad_Check(0, 2, 1)) {
            n = (n + 999) % 1000;
        }
        if (SzPad_Check(0, 2, 0x2000)) {
            p->unknown_00[DOTR_DEBUG_DIGIT] = (p->unknown_00[DOTR_DEBUG_DIGIT] + 4) % 3;
        }
        if (SzPad_Check(0, 2, 0x8000)) {
            p->unknown_00[DOTR_DEBUG_DIGIT] = (p->unknown_00[DOTR_DEBUG_DIGIT] + 2) % 3;
        }
        if (n == no) {
            if (SzPad_Check(0, 2, 8)) {
                p->unknown_00[DOTR_DEBUG_MODEL] = (p->unknown_00[DOTR_DEBUG_MODEL] + 45) % 44;
            }
            if (SzPad_Check(0, 2, 4)) {
                p->unknown_00[DOTR_DEBUG_MODEL] = (p->unknown_00[DOTR_DEBUG_MODEL] + 43) % 44;
            }
            type = SzModel_GetModelType(p->unknown_00[DOTR_DEBUG_MODEL]);
        } else if (!SzModel_IsModelLoading(p->unknown_00[DOTR_DEBUG_MODEL])) {
            struct DotrDebugCard *q = SzDuel_GetCardInfo(-1, p->unknown_00[DOTR_DEBUG_MODEL]);
            if (q != 0) {
                q->unknown_3c = SzDuel_Xax2New(n);
                q->info = SzDuel_GetCardConstInfo(q->unknown_3c);
                if (q->info != 0) {
                    q->unknown_3e = q->info->unknown_00;
                    q->unknown_3f = q->info->unknown_02 & 0xf;
                }
            }
            SzModel_Preset(p->unknown_00[DOTR_DEBUG_MODEL], -1, n);
        }
        if (SzPad_Check(0, 0, 0x30) && !SzModel_IsModelLoading(p->unknown_00[DOTR_DEBUG_MODEL])) {
            int flag = SzPad_Check(0, 0, 0x10) ? 1 : 0;
            int none = 0;
            if ((type == 4 || type == 5) && SzDuel_GetUnit(-1, p->unknown_00[DOTR_DEBUG_MODEL]) == 0) {
                none = 1;
            }
            if (!none && SzModel_Load(p->unknown_00[DOTR_DEBUG_MODEL], -1, n, flag) && (type == 4 || type == 5)) {
                struct DotrDebugCard *q = SzDuel_GetCardInfo(-1, p->unknown_00[DOTR_DEBUG_MODEL]);
                if (q != 0 && q->info != 0) {
                    if (mode == 5 && type == 5) {
                        SzBattle_Entry(q->unknown_44, (q->unknown_44 << 8) | q->unknown_45);
                        SzDuel_SetBusyFlag(q->unknown_44, q->unknown_45, 0);
                    }
                    SzDuel_SetUnitCtlModeM(-1, p->unknown_00[DOTR_DEBUG_MODEL], 7);
                    SzDuel_SetUnitCtlRateM(-1, p->unknown_00[DOTR_DEBUG_MODEL], 1, 0);
                }
            }
        }
        if (type == 4 || type == 5) {
            if (SzPad_Check(0, 0, 0x80)) {
                SzModel_SetDeform(p->unknown_00[DOTR_DEBUG_MODEL], 1);
            }
            if (SzPad_Check(0, 0, 0x40)) {
                SzModel_SetDeform(p->unknown_00[DOTR_DEBUG_MODEL], 0);
            }
            if (SzPad_Check(0, 0, 0x600)) {
                SzModel_SetStoneFlag(p->unknown_00[DOTR_DEBUG_MODEL], 1);
            }
        }
    }
    if (SzDebug_IsHide()) {
        return;
    }
    SzModel_GetModelNo(p->unknown_00[DOTR_DEBUG_MODEL]);
    func_0019bef0(gBaseStr_ptrs[type]);
    {
    int none = 0;
    if ((type == 4 || type == 5) && SzDuel_GetUnit(-1, p->unknown_00[DOTR_DEBUG_MODEL]) == 0) {
        none = 1;
    }
    if (!none && !SzModel_IsModelReady(p->unknown_00[DOTR_DEBUG_MODEL]) && !SzModel_IsModelLoading(p->unknown_00[DOTR_DEBUG_MODEL]) && SzModel_IsModelStandby(p->unknown_00[DOTR_DEBUG_MODEL])) {
        SzModel_IsModelDisp(p->unknown_00[DOTR_DEBUG_MODEL]);
    }
    }
    SzModel_GetDeform(p->unknown_00[DOTR_DEBUG_MODEL]);
    if (menu == 0) {
        int i = p->unknown_00[DOTR_DEBUG_DIGIT];
        int k = 100;
        while (i-- != 0) {
            k /= 10;
        }
    }
}
