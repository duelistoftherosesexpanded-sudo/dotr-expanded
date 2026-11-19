#include "dotr/ee/debug_menu.h"

/* Per-frame input of the debug menu's effect page. Nothing is edited while both low bits of
 * SzPad_Check(0, 1, 3) are set. Otherwise the row selects which of six values the pad edits
 * (two three-digit numbers, then two pairs of a 0..1 and a 0..10 value) and further buttons
 * clear, destroy, enter, erase and sleep or wake effects with those values. The closing
 * division loop has no effect; it is presumably what is left of removed debug output.
 * `> 2` is the spelling that reproduces the original column comparison. */
void lSzDebug_ActEffect(void)
{
    struct DotrSmallFlagsOwner *p = &data_00325c80_owner;

    if ((SzPad_Check(0, 1, 3) & 0xffff) != 3) {
    struct DotrDebugDigits a = data_00293d38;
    int no = p->unknown_00[DOTR_DEBUG_EFF_NO];
    struct DotrDebugDigits b;
    int arg;
    /* MATCHING DEVICE: as in SzDebug_Act, the digits are stored through a walking pointer. */
    {
        int *e = a.d;
        *e++ = (no / 100) % 10;
        *e++ = (no / 10) % 10;
        *e = no % 10;
    }
    b = data_00293d48;
    arg = p->unknown_00[DOTR_DEBUG_EFF_ARG];
    {
        int *e = b.d;
        *e++ = (arg / 100) % 10;
        *e++ = (arg / 10) % 10;
        *e = arg % 10;
    }
    switch (p->unknown_00[DOTR_DEBUG_EFF_ROW]) {
    case 0:
        if (SzPad_Check(0, 2, 0x2000)) {
            if (++p->unknown_00[DOTR_DEBUG_EFF_COLUMN] > 2) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 1;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x8000)) {
            if (--p->unknown_00[DOTR_DEBUG_EFF_COLUMN] < 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 5;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            int i = p->unknown_00[DOTR_DEBUG_EFF_COLUMN];
            if (SzPad_Check(0, 2, 0x1000)) {
                a.d[i] = (a.d[i] + 11) % 10;
            } else {
                a.d[i] = (a.d[i] + 9) % 10;
            }
            p->unknown_00[DOTR_DEBUG_EFF_NO] = a.d[0] * 100 + a.d[1] * 10 + a.d[2];
        }
        break;
    case 1:
        if (SzPad_Check(0, 2, 0x2000)) {
            if (++p->unknown_00[DOTR_DEBUG_EFF_COLUMN] > 2) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 2;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x8000)) {
            if (--p->unknown_00[DOTR_DEBUG_EFF_COLUMN] < 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 0;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 2;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            int i = p->unknown_00[DOTR_DEBUG_EFF_COLUMN];
            if (SzPad_Check(0, 2, 0x1000)) {
                b.d[i] = (b.d[i] + 11) % 10;
            } else {
                b.d[i] = (b.d[i] + 9) % 10;
            }
            p->unknown_00[DOTR_DEBUG_EFF_ARG] = b.d[0] * 100 + b.d[1] * 10 + b.d[2];
        }
        break;
    case 2:
        if (SzPad_Check(0, 2, 0x2000)) {
            if (++p->unknown_00[DOTR_DEBUG_EFF_COLUMN] > 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 3;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x8000)) {
            if (--p->unknown_00[DOTR_DEBUG_EFF_COLUMN] < 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 1;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 2;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            if (SzPad_Check(0, 2, 0x1000)) {
                p->unknown_00[DOTR_DEBUG_EFF_A_HI] = (p->unknown_00[DOTR_DEBUG_EFF_A_HI] + 3) % 2;
            } else {
                p->unknown_00[DOTR_DEBUG_EFF_A_HI] = (p->unknown_00[DOTR_DEBUG_EFF_A_HI] + 1) % 2;
            }
        }
        break;
    case 3:
        if (SzPad_Check(0, 2, 0x2000)) {
            if (++p->unknown_00[DOTR_DEBUG_EFF_COLUMN] > 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 4;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x8000)) {
            if (--p->unknown_00[DOTR_DEBUG_EFF_COLUMN] < 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 2;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            if (SzPad_Check(0, 2, 0x1000)) {
                p->unknown_00[DOTR_DEBUG_EFF_A_LO] = (p->unknown_00[DOTR_DEBUG_EFF_A_LO] + 12) % 11;
            } else {
                p->unknown_00[DOTR_DEBUG_EFF_A_LO] = (p->unknown_00[DOTR_DEBUG_EFF_A_LO] + 10) % 11;
            }
        }
        break;
    case 4:
        if (SzPad_Check(0, 2, 0x2000)) {
            if (++p->unknown_00[DOTR_DEBUG_EFF_COLUMN] > 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 5;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x8000)) {
            if (--p->unknown_00[DOTR_DEBUG_EFF_COLUMN] < 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 3;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            if (SzPad_Check(0, 2, 0x1000)) {
                p->unknown_00[DOTR_DEBUG_EFF_B_HI] = (p->unknown_00[DOTR_DEBUG_EFF_B_HI] + 3) % 2;
            } else {
                p->unknown_00[DOTR_DEBUG_EFF_B_HI] = (p->unknown_00[DOTR_DEBUG_EFF_B_HI] + 1) % 2;
            }
        }
        break;
    case 5:
        if (SzPad_Check(0, 2, 0x2000)) {
            if (++p->unknown_00[DOTR_DEBUG_EFF_COLUMN] > 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 0;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x8000)) {
            if (--p->unknown_00[DOTR_DEBUG_EFF_COLUMN] < 0) {
                p->unknown_00[DOTR_DEBUG_EFF_ROW] = 4;
                p->unknown_00[DOTR_DEBUG_EFF_COLUMN] = 0;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            if (SzPad_Check(0, 2, 0x1000)) {
                p->unknown_00[DOTR_DEBUG_EFF_B_LO] = (p->unknown_00[DOTR_DEBUG_EFF_B_LO] + 12) % 11;
            } else {
                p->unknown_00[DOTR_DEBUG_EFF_B_LO] = (p->unknown_00[DOTR_DEBUG_EFF_B_LO] + 10) % 11;
            }
        }
        break;
    }
    if (SzPad_Check(0, 2, 8)) {
        p->unknown_00[DOTR_DEBUG_EFF_A_LO] = (p->unknown_00[DOTR_DEBUG_EFF_A_LO] + 12) % 11;
    }
    if (SzPad_Check(0, 2, 4)) {
        p->unknown_00[DOTR_DEBUG_EFF_A_LO] = (p->unknown_00[DOTR_DEBUG_EFF_A_LO] + 10) % 11;
    }
    if (SzPad_Check(0, 0, 0x10)) {
        SzEffect_Clear();
    }
    if (SzPad_Check(0, 0, 0x40)) {
        SzEffect_Destroy2(0, p->unknown_00[DOTR_DEBUG_EFF_NO]);
    }
    if (SzPad_Check(0, 0, 0x20)) {
        SzEffect_Entry(0, p->unknown_00[DOTR_DEBUG_EFF_NO], p->unknown_00[DOTR_DEBUG_EFF_ARG],
                       (p->unknown_00[DOTR_DEBUG_EFF_A_HI] << 8) | p->unknown_00[DOTR_DEBUG_EFF_A_LO],
                       (p->unknown_00[DOTR_DEBUG_EFF_B_HI] << 8) | p->unknown_00[DOTR_DEBUG_EFF_B_LO]);
    }
    if (SzPad_Check(0, 0, 0x80)) {
        SzEffect_Erase2(0, -1);
    }
    if (SzPad_Check(0, 0, 0x800)) {
        SzEffect_Sleep2(0, p->unknown_00[DOTR_DEBUG_EFF_NO], 0);
    }
    if (SzPad_Check(0, 0, 0x100)) {
        SzEffect_Sleep2(0, p->unknown_00[DOTR_DEBUG_EFF_NO], 1);
    }
    }
    {
        int i = p->unknown_00[DOTR_DEBUG_EFF_COLUMN];
        int k = 100;
        while (i-- != 0) {
            k /= 10;
        }
    }
}
