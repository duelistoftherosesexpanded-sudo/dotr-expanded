#include "dotr/ee/custom_duel_proc.h"

/* MATCHING DEVICE: the other player's menu byte and the life-point halfwords are reached
 * through the same index-as-pointer view as `w` below. */
#define MENU(i) (DOTR_CUSTOM_PROC_BYTES_AT(i)->menu)
#define LP(i) (DOTR_CUSTOM_PROC_WORDS_AT(i)->lp)
#define LP2(i) (DOTR_CUSTOM_PROC_WORDS_AT(i)->lp2)

/* MATCHING DEVICE: these spellings (each evaluates its arguments twice). */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(v, lo, hi) MIN(MAX(v, lo), hi)

/* Assumed: a copy of lSzDuel_IsDeckReady (0x1ba3b0, in the side_table group), which the
 * original has inlined here. MATCHING DEVICE: the empty test in the 999 branch. */
static inline int DotrCustomProcIsDeckReady(int side, int list)
{
    int count = SzData_GetDeckCount(side, list);
    int value = SzData_GetLeader(side, list);

    if (count >= 40) {
        if (value == 999) {
            if (!list && !count) {
            }
        } else {
            return 1;
        }
    }
    return 0;
}

int lSzDuel_ActCustomDuelProc(int side)
{
    struct DotrCustomProcControl *c = (struct DotrCustomProcControl *)data_00317490_customproc;
    int deck = SzData_GetCurrentDeck(side);
    unsigned char *reload = side ? &c->reload[1] : &c->reload[0];
    /* MATCHING DEVICE: the player index itself is the pointer (custom_duel_proc.h); the host
     * build adds it to its fixture's address. */
#ifdef __MWERKS__
    struct DotrCustomProcAt *w = (struct DotrCustomProcAt *)side;
#else
    struct DotrCustomProcAt *w = DOTR_CUSTOM_PROC_BYTES_AT(side);
#endif
    int m;
    int old;
    int x;
    int y;
    int v;
    int row;

    if (SzFile_InqRequestEx() == 0 && *reload) {
        SzText_LoadMerge(side + 5, 1);
        *reload = 0;
        if (DOTR_CUSTOM_PROC->mode == 1) {
            SzText_LoadMerge(6, 1);
        }
    }
    /* MATCHING DEVICE: the menu byte is read once; the levels are a switch inside this test
     * with its own return, and the browsing level is a second switch on the whole byte. */
    m = w->menu;
    if (m & 0xf0) {
    switch (m & 0xf0) {
    case 0x10:
        switch (m & 0xf) {
        case 0:
            if (SzPad_Check(side, 2, 0xa000) || SzPad_Check(side, 2, 12)) {
                if (SzPad_Check(side, 2, 0x8004)) {
                    w->deck = (w->deck + 2) % 3;
                } else {
                    w->deck = (w->deck + 4) % 3;
                }
                SzSound_PlaySE(0, 3);
            } else if (SzPad_Check(side, 0, 192)) {
                if (DotrCustomProcIsDeckReady(side, w->deck)) {
                    if (deck != w->deck) {
                        SzSound_PlaySE(0, 4);
                        SzData_SetCurrentDeck(side, w->deck);
                        SzText_Free(2, side);
                        *reload = 1;
                        if (DOTR_CUSTOM_PROC->mode == 1) {
                            SzData_SetCurrentDeck(1, w->deck);
                            SzText_Free(2, 1);
                        }
                    }
                } else {
                    SzSound_PlaySE(0, 6);
                }
            } else if (SzPad_Check(side, 0, 32)) {
                SzSound_PlaySE(0, 5);
                w->menu &= 0xf;
            }
            break;
        case 1:
            if (SzPad_Check(side, 2, 0xa000) || SzPad_Check(side, 2, 12)) {
                short *p = &LP(side);

                old = *p;
                if (SzPad_Check(side, 2, 0x8004)) {
                    *p = ((*p + 50) / 100 - 1) * 100;
                } else {
                    *p = ((*p + 50) / 100 + 1) * 100;
                }
                *p = CLAMP(*p, 1, 9999);
                if (*p == old) {
                    SzSound_PlaySE(0, 6);
                } else {
                    SzSound_PlaySE(0, 3);
                }
            } else if (SzPad_Check(side, 0, 192)) {
                short *p = &LP(side);

                if (*p != SzDuel_GetLP(side)) {
                    SzSound_PlaySE(0, 4);
                    SzDuel_CalcLP(side, 4, *p);
                    LP2(side) = *p;
                    if (DOTR_CUSTOM_PROC->mode) {
                        /* MATCHING DEVICE: the value is computed, the index flipped in
                         * place, and the maximum written out a second time. */
                        v = MAX(*p, 4000);
                        side ^= 1;
                        SzDuel_CalcLP(side, 4, v);
                        LP2(side) = MAX(*p, 4000);
                    }
                }
            } else if (SzPad_Check(side, 0, 32)) {
                SzSound_PlaySE(0, 5);
                w->menu &= 0xf;
            }
            break;
        case 2:
            if (SzPad_Check(side, 2, 0xa000) || SzPad_Check(side, 2, 12)) {
                old = w->charge;
                if (SzPad_Check(side, 2, 0x8004)) {
                    w->charge = MAX(w->charge - 1, 1);
                } else {
                    w->charge = MIN(w->charge + 1, 12);
                }
                if (w->charge == old) {
                    SzSound_PlaySE(0, 6);
                } else {
                    SzSound_PlaySE(0, 3);
                }
            } else if (SzPad_Check(side, 0, 192)) {
                if (w->charge != SzDuel_GetSummonCharge(side)) {
                    SzSound_PlaySE(0, 4);
                    SzDuel_SetSummonCharge(side, w->charge);
                    if (DOTR_CUSTOM_PROC->mode) {
                        SzDuel_SetSummonCharge(side ^ 1, (unsigned char)MAX(w->charge, 3));
                    }
                }
            } else if (SzPad_Check(side, 0, 32)) {
                SzSound_PlaySE(0, 5);
                w->menu &= 0xf;
            }
            break;
        case 3:
            if (SzPad_Check(side, 2, 0xa000)) {
                old = w->f8;
                if (SzPad_Check(side, 2, 0x8000)) {
                    w->f8 = 0;
                } else {
                    w->f8 = 1;
                }
                if (w->f8 != old) {
                    SzSound_PlaySE(0, 3);
                }
            } else if (SzPad_Check(side, 0, 192)) {
                if (w->f8 != w->fa) {
                    SzSound_PlaySE(0, 4);
                    w->fa = w->f8;
                }
            } else if (SzPad_Check(side, 0, 32)) {
                SzSound_PlaySE(0, 5);
                w->menu &= 0xf;
            }
            break;
        case 4:
            if (SzPad_Check(side, 2, 0xa000)) {
                old = w->fc;
                if (SzPad_Check(side, 2, 0x8000)) {
                    w->fc = 0;
                } else {
                    w->fc = 1;
                }
                if (w->fc != old) {
                    SzSound_PlaySE(0, 3);
                }
            } else if (SzPad_Check(side, 0, 192)) {
                if (w->fc != w->fe) {
                    SzSound_PlaySE(0, 4);
                    w->fe = w->fc;
                }
            } else if (SzPad_Check(side, 0, 32)) {
                SzSound_PlaySE(0, 5);
                w->menu &= 0xf;
            }
            break;
        case 5:
            w->menu &= 0xf;
            break;
        case 6:
            if (SzGame_ActMemCardProc() < 0) {
                w->menu &= 0xf;
            }
            break;
        case 7:
            if (SzPad_Check(side, 2, 0x5000)) {
                if (SzPad_Check(side, 2, 0x1000)) {
                    DOTR_CUSTOM_PROC->map = (DOTR_CUSTOM_PROC->map + DOTR_CUSTOM_PROC->maps - 1) % DOTR_CUSTOM_PROC->maps;
                } else {
                    DOTR_CUSTOM_PROC->map = (DOTR_CUSTOM_PROC->map + DOTR_CUSTOM_PROC->maps + 1) % DOTR_CUSTOM_PROC->maps;
                }
                SzSound_PlaySE(0, 3);
            } else if (SzPad_Check(side, 0, 192)) {
                struct DotrCustomProcSave *d = SzData_GetData(side);
                int n = (DOTR_CUSTOM_PROC->map + 2) % DOTR_CUSTOM_PROC->maps;

                for (y = 0; y < 7; y++) {
                    for (x = 0; x < 7; x++) {
                        SzDuel_SetField(x, y, d->map[n][y][x]);
                    }
                }
                SzSound_PlaySE(0, 4);
                SzGame_SetMenuNum(13, 0);
                SzGame_Push(13, 2);
            } else if (SzPad_Check(side, 0, 32)) {
                SzSound_PlaySE(0, 5);
                w->menu &= 0xf;
            }
            break;
        }
        break;
    case 0x20:
        if (SzPad_Check(side, 0, 32)) {
            SzSound_PlaySE(0, 5);
            w->menu &= 0xf;
        }
        break;
    case 0x30:
        if (SzPad_Check(side, 0, 0xa000)) {
            old = DOTR_CUSTOM_PROC->yes;
            if (SzPad_Check(side, 0, 0x8000)) {
                DOTR_CUSTOM_PROC->yes = 1;
            } else {
                DOTR_CUSTOM_PROC->yes = 0;
            }
            if (DOTR_CUSTOM_PROC->yes != old) {
                SzSound_PlaySE(0, 3);
            }
        } else if (SzPad_Check(side, 0, 192)) {
            SzSound_PlaySE(0, 4);
            if (DOTR_CUSTOM_PROC->yes) {
                SzGame_Pop();
            } else {
                DOTR_CUSTOM_PROC->menu[0] &= 0xf;
                DOTR_CUSTOM_PROC->menu[1] &= 0xf;
            }
        } else if (SzPad_Check(side, 0, 32)) {
            SzSound_PlaySE(0, 5);
            DOTR_CUSTOM_PROC->menu[0] &= 0xf;
            DOTR_CUSTOM_PROC->menu[1] &= 0xf;
        }
        break;
    case 0x40:
        if (SzPad_Check(side, 2, 12)) {
            if (SzPad_Check(side, 2, 4)) {
                DOTR_CUSTOM_PROC->last = (DOTR_CUSTOM_PROC->last + DOTR_CUSTOM_PROC->count - 5) % DOTR_CUSTOM_PROC->count;
            } else {
                DOTR_CUSTOM_PROC->last = (DOTR_CUSTOM_PROC->last + DOTR_CUSTOM_PROC->count + 5) % DOTR_CUSTOM_PROC->count;
            }
            SzSound_PlaySE(0, 3);
        } else if (SzPad_Check(side, 2, 0x5000)) {
            if (SzPad_Check(side, 2, 0x1000)) {
                DOTR_CUSTOM_PROC->last = (DOTR_CUSTOM_PROC->last + DOTR_CUSTOM_PROC->count - 1) % DOTR_CUSTOM_PROC->count;
            } else {
                DOTR_CUSTOM_PROC->last = (DOTR_CUSTOM_PROC->last + DOTR_CUSTOM_PROC->count + 1) % DOTR_CUSTOM_PROC->count;
            }
            SzSound_PlaySE(0, 3);
        } else if (SzPad_Check(side, 0, 192)) {
            /* MATCHING DEVICE: the class goes through `row`, the number stays in `v`. */
            v = DOTR_CUSTOM_PROC->list[(DOTR_CUSTOM_PROC->last + 2) % DOTR_CUSTOM_PROC->count];
            row = v >> 8;
            v = v & 0xff;
            if (row < 2) {
                v += row * 5 + 46;
            } else {
                v += 22;
            }
            SzGame_SetMenuNum(2, 0);
            SzGame_SetMenuNum(1, v);
            SzGame_SetMenuNum(3, c->side[0].deck + 1000);
            SzGame_SetMenuNum(4, c->side[1].deck + 1000);
            SzGame_SetMenuNum(28, (DOTR_CUSTOM_PROC->f8[0] ? 1 : 0) | (DOTR_CUSTOM_PROC->f8[1] ? 2 : 0));
            SzGame_SetMenuNum(29, (DOTR_CUSTOM_PROC->fc[0] ? 1 : 0) | (DOTR_CUSTOM_PROC->fc[1] ? 2 : 0));
            SzGame_Push(1, 1);
            SzSound_PlaySE(0, 4);
            return 1;
        } else if (SzPad_Check(side, 0, 32)) {
            SzSound_PlaySE(0, 5);
            DOTR_CUSTOM_PROC->menu[0] &= 0xf;
            DOTR_CUSTOM_PROC->menu[1] &= 0xf;
            if (DOTR_CUSTOM_PROC->mode == 0) {
                MENU(side ^ 1) |= 0x20;
            }
        }
        break;
    }
    return 0;
    }
    switch (m) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (SzPad_Check(side, 2, 0xf000)) {
                /* MATCHING DEVICE: block locals in this order, assigned row, col, grid. */
                int row;
                int col;
                unsigned char (*t)[5];

                row = w->cur / 5;
                col = w->cur % 5;
                t = SzDuel_CustomDuelMenu_customproc[DOTR_CUSTOM_PROC->mode ? 1 : 0];
                do {
                    if (SzPad_Check(side, 2, 0x1000)) {
                        col = (col + 4) % 5;
                    } else if (SzPad_Check(side, 2, 0x4000)) {
                        col = (col + 6) % 5;
                    } else if (SzPad_Check(side, 2, 0x8000)) {
                        row = (row + 1) % 2;
                    } else if (SzPad_Check(side, 2, 0x2000)) {
                        row = (row + 3) % 2;
                    }
                    v = t[row][col];
                } while (v == 255 || (v == 7 && !SzData_GetMapEditFlag(side)));
                if (w->menu != v) {
                    w->cur = col + row * 5;
                    w->menu = v;
                    SzSound_PlaySE(0, 3);
                }
            } else if (SzPad_Check(side, 0, 2048)) {
                unsigned char *q = &MENU(side ^ 1);

                if ((*q & 0xf0) == 0x20) {
                    SzSound_PlaySE(0, 4);
                    *q &= 0xf;
                    DOTR_CUSTOM_PROC->menu[0] |= 0x40;
                    DOTR_CUSTOM_PROC->menu[1] |= 0x40;
                } else {
                    SzSound_PlaySE(0, 4);
                    if (DOTR_CUSTOM_PROC->mode) {
                        DOTR_CUSTOM_PROC->menu[0] |= 0x40;
                        DOTR_CUSTOM_PROC->menu[1] |= 0x40;
                    } else {
                        w->menu |= 0x20;
                    }
                }
            } else if (SzPad_Check(side, 0, 192)) {
                SzSound_PlaySE(0, 4);
                switch (w->menu) {
                case 5:
                    SzGame_SetMenuNum(6, 0);
                    SzGame_Push(6, 1);
                    break;
                case 6:
                    SzGame_InitMemCardProc(1);
                    break;
                }
                w->menu |= 0x10;
            } else if (SzPad_Check(side, 0, 32)) {
                if (MENU(side ^ 1) < 8) {
                    SzSound_PlaySE(0, 5);
                    DOTR_CUSTOM_PROC->menu[0] |= 0x30;
                    DOTR_CUSTOM_PROC->menu[1] |= 0x30;
                    DOTR_CUSTOM_PROC->yes = 0;
                } else {
                    SzSound_PlaySE(0, 6);
                }
            }
            break;
        }
    return 0;
}
