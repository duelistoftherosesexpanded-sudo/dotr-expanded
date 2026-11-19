#include "dotr/ee/trade_proc.h"

/* MATCHING DEVICE: the clamp macros evaluate their arguments twice. */
#define DOTR_TRADE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_TRADE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define DOTR_TRADE_CLAMP(v, lo, hi) DOTR_TRADE_MIN(DOTR_TRADE_MAX(v, lo), hi)

/* Assumed helper (name invented): the copies one side offers, summed over its 854 count bytes
 * through its own pointer to the control block; the original rebuilds the block's address at
 * each of the two sums. */
static inline int lSzDuel_TradeTotal(int which)
{
    struct DotrTradeProcControl *q = &DOTR_TRADE_PROC;
    unsigned char *p = which ? q->counts[1] : q->counts[0];
    int total = 0;
    int i;

    for (i = 0; i < 854; i++) {
        total += *p++;
    }
    return total;
}

int lSzDuel_ActTradeProc(int which)
{
    struct DotrTradeProcControl *c;
    unsigned short *list;
    unsigned char *cnt;
    unsigned char *keys;
    unsigned short *idle;
    unsigned short *top;
    unsigned short *count;
    unsigned char *rows;
    int id;
    int hold;
    int old;
    int d;
    int v;
    int n;
    int x;
    int max;
    int other;

    /* MATCHING DEVICE: the seven per-side pointers are declared without initialisers and
     * assigned in this order as `which ? X[1] : X[0]`; state, slide, tab, the count byte and the
     * sort key are array expressions throughout (no pointer variables); the saved old value is
     * held in `other` (count byte), `max` (sort key) and `hold` (tab); the comparisons keep
     * their operand order; the analog value goes through `v`; the add/remove button mask is
     * written `(unsigned short)(which ? 0x80 : 0x40)`. */
    c = &DOTR_TRADE_PROC;
    list = which ? c->lists[1] : c->lists[0];
    cnt = which ? c->counts[1] : c->counts[0];
    keys = which ? c->keys[1] : c->keys[0];
    idle = which ? &c->idle[1] : &c->idle[0];
    top = which ? &c->pairs[1].top : &c->pairs[0].top;
    count = which ? &c->pairs[1].count : &c->pairs[0].count;
    rows = which ? &c->rows[1] : &c->rows[0];
    if (c->state[which] < 4) {
        if (SzPad_Check(which, 1, 0xffff)) {
            *idle = 90;
        } else if (*idle) {
            (*idle)--;
        }
    }
    switch (c->state[which]) {
    case 0:
        c->slide[which] = DOTR_TRADE_MAX(c->slide[which] - 45, 0);
        if (c->slide[which] <= 0) {
            c->state[which] = 1;
        }
        break;
    case 1:
        id = list[*top + *rows];
        hold = SzPad_Check(which, 1, 0x100) ? 1 : 0;
        old = *rows;
        d = 0;
        if (*count) {
            if (SzPad_Check(which, 2, 0xc)) {
                if (SzPad_Check(which, 2, 4)) {
                    d = -5;
                } else {
                    d = 5;
                }
            } else if (SzPad_Check(which, 2, 0x5000)) {
                if (SzPad_Check(which, 2, 0x1000)) {
                    if (*rows > 0) {
                        (*rows)--;
                    } else {
                        d = -1;
                    }
                } else {
                    if (*rows < 4) {
                        (*rows)++;
                    } else {
                        d = 1;
                    }
                }
            }
            if (*rows != old) {
                SzSound_PlaySE(0, 3);
                break;
            }
            if (d == 0 && SzPad_IsAnalog(which)) {
                v = SzPad_GetAnalogInfo(which, 3) & 0xff;
                x = v - 0x80;
                if ((x >= 0 ? x : -x) > 0x30) {
                    if (x > 0) {
                        x -= 0x30;
                    } else {
                        x += 0x30;
                    }
                    d = x * 15 >> 7;
                }
            }
            if (d) {
                max = DOTR_TRADE_MAX(*count - 5, 0);
                old = *top;
                x = old + d;
                *top = DOTR_TRADE_CLAMP(x, 0, max);
                if (*top != old) {
                    SzSound_PlaySE(0, 3);
                }
                *idle = 90;
                break;
            }
            if (SzPad_Check(which, 2, 0xc0)) {
                if (id == 999 || c->modes[which] == 2) {
                    SzSound_PlaySE(0, 6);
                    break;
                }
                n = SzData_GetBagCount(which, id, 3);
                other = cnt[id];
                if (SzPad_Check(which, 2, (unsigned short)(which ? 0x80 : 0x40))) {
                    if (cnt[id] < n) {
                        cnt[id]++;
                    }
                } else {
                    if (cnt[id] > 0) {
                        cnt[id]--;
                    }
                }
                if (other == cnt[id]) {
                    SzSound_PlaySE(0, 6);
                } else {
                    SzSound_PlaySE(0, 3);
                }
                break;
            }
            if (SzPad_Check(which, 0, 0x10)) {
                if (id == 999) {
                    SzSound_PlaySE(0, 6);
                    break;
                }
                c->state[which] = 2;
                SzSound_PlaySE(0, 16);
                break;
            }
        }
        if (SzPad_Check(which, 0, 0x600)) {
            if (SzPad_Check(which, 0, 0x200)) {
                c->modes[which] = (c->modes[which] + 2) % 3;
            } else {
                c->modes[which] = (c->modes[which] + 4) % 3;
            }
            lSzDuel_SetupTrade(which);
            *top = 0;
            SzSound_PlaySE(0, 19);
        } else if (SzPad_Check(which, 0, 0x800)) {
            other = which ^ 1;
            if (c->state[other] == 4) {
                if (lSzDuel_TradeTotal(which) > 0 || lSzDuel_TradeTotal(other) > 0) {
                    SzSound_PlaySE(0, 4);
                    c->state[0] = 6;
                    c->state[1] = 6;
                    c->confirm = 0;
                } else {
                    SzSound_PlaySE(0, 6);
                }
            } else {
                SzSound_PlaySE(0, 4);
                c->state[which] = 4;
            }
        } else if (SzPad_Check(which, 2, 3)) {
            max = keys[hold];
            if (SzPad_Check(which, 2, 1)) {
                keys[hold] = (keys[hold] + 9) % 10;
            } else {
                keys[hold] = (keys[hold] + 11) % 10;
            }
            if (max != keys[hold]) {
                lSzDuel_SortTrade(which);
                SzSound_PlaySE(0, 19);
            }
        } else if (SzPad_Check(which, 0, 0x20)) {
            if (c->state[which ^ 1] == 1) {
                SzSound_PlaySE(0, 5);
                c->state[0] = 5;
                c->state[1] = 5;
                c->confirm = 0;
            } else {
                SzSound_PlaySE(0, 6);
            }
        }
        break;
    case 2:
        c->slide[which] = DOTR_TRADE_MIN(c->slide[which] + 45, 320);
        if (c->slide[which] >= 320) {
            c->state[which] = 3;
        }
        break;
    case 3:
        if (SzPad_Check(which, 2, 0x5000)) {
            hold = c->tab[which];
            if (SzPad_Check(which, 2, 0x1000)) {
                c->tab[which] = (c->tab[which] + 3) % 4;
            } else {
                c->tab[which] = (c->tab[which] + 5) % 4;
            }
            if (c->tab[which] != hold) {
                SzSound_PlaySE(0, 19);
            }
        } else if (SzPad_Check(which, 0, 0x30)) {
            c->state[which] = 0;
            SzSound_PlaySE(0, 17);
        }
        break;
    case 4:
        if (SzPad_Check(which, 0, 0x20)) {
            SzSound_PlaySE(0, 5);
            c->state[which] = 1;
        }
        break;
    case 5:
        if (SzPad_Check(which, 0, 0xa000)) {
            old = c->confirm;
            if (SzPad_Check(which, 0, 0x8000)) {
                c->confirm = 1;
            } else {
                c->confirm = 0;
            }
            if (c->confirm != old) {
                SzSound_PlaySE(0, 3);
            }
        } else if (SzPad_Check(which, 0, 0xc0)) {
            SzSound_PlaySE(0, 4);
            if (c->confirm) {
                SzGame_Pop();
            } else {
                c->state[0] = 1;
                c->state[1] = 1;
            }
        } else if (SzPad_Check(which, 0, 0x20)) {
            SzSound_PlaySE(0, 5);
            c->state[0] = 1;
            c->state[1] = 1;
        }
        break;
    case 6:
        if (SzPad_Check(which, 0, 0xa000)) {
            old = c->confirm;
            if (SzPad_Check(which, 0, 0x8000)) {
                c->confirm = 1;
            } else {
                c->confirm = 0;
            }
            if (c->confirm != old) {
                SzSound_PlaySE(0, 3);
            }
        } else if (SzPad_Check(which, 0, 0xc0)) {
            SzSound_PlaySE(0, 4);
            if (c->confirm) {
                if (lSzDuel_TradeDo() > 0) {
                    c->state[0] = 8;
                    c->state[1] = 8;
                } else {
                    c->state[0] = 7;
                    c->state[1] = 7;
                    SzGame_InitMemCardProc(5);
                    return 1;
                }
            } else {
                c->state[which] = 1;
                c->state[which ^ 1] = 4;
            }
        } else if (SzPad_Check(which, 0, 0x20)) {
            SzSound_PlaySE(0, 5);
            c->state[which] = 1;
            c->state[which ^ 1] = 4;
        }
        break;
    case 7:
        SzGame_Pop();
        break;
    case 8:
        if (SzPad_Check(which, 0, 0xf0)) {
            SzSound_PlaySE(0, 4);
            SzGame_Pop();
        }
        break;
    }
    return 0;
}
