#include "dotr/ee/small_state.h"

/* SzPad_Status: steps each of the two pad entries (0x5c bytes from +0x200 of gSzPad_pl) through
 * the six-bit phase at +0x5b. State 0 from func_00192bb8 resets the entry and the phase. With
 * state 2 or 6: phase 0 resets, stores the mode count (func_00192f98 term 4, offset -1) in the
 * high nibble of +0x5a and, once term 1 answers non-zero, the id at +0x58 (term 2 replaces it
 * when positive) and goes to phase 1, or to 5 when the count is zero; phase 1 looks for mode id
 * 7 and locks it (func_001930d0), going to 2, else 5; phase 2 waits on func_00192cd0 (0: bit 6
 * set, phase 3; 1: phase 1); phase 3 goes to 4 when func_00193488 and func_001934e8 both answer
 * 1, else 5; phase 4 waits again (0: bit 7 set, phase 5; 1: phase 3); phase 5 stays.
 * func_00192cd0 uses the prototype in small_state.h. The two signed one-bit fields are set with
 * -1 rather than 1 (same bit; the host compiler rejects 1). */

static inline void SzPad_ResetEntry(struct DotrSmallPadEntry *pad) {
    int i;

    for (i = 0; i < 3; i++) {
        pad->buttons[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        pad->press[i] = -pad->base;
    }
    pad->pressed = 0;
    for (i = 0; i < 4; i++) {
        pad->act[i] = 0x80;
    }
    pad->ready = 0;
    for (i = 0; i < 12; i++) {
        pad->data[i] = 0;
    }
}

void SzPad_Status(void) {
    int port;
    struct DotrSmallPadEntry *pad;
    unsigned char *base;
    int state;
    int id;
    int offs;

    base = (unsigned char *)&gSzPad_pl;
    for (port = 0; port < 2; port++, base += 0x5C) {
        pad = (struct DotrSmallPadEntry *)(base + 0x200);
        state = func_00192bb8(port, 0);
        if (state == 0) {
            SzPad_ResetEntry(pad);
            pad->state = 0;
        }
        if (state == 2 || state == 6) {
            switch (pad->state) {
            case 0:
                for (state = 0; state < 3; state++) {
                    pad->buttons[state] = 0;
                }
                for (state = 0; state < 16; state++) {
                    pad->press[state] = -pad->base;
                }
                pad->pressed = 0;
                for (state = 0; state < 4; state++) {
                    pad->act[state] = 0x80;
                }
                pad->ready = 0;
                for (state = 0; state < 12; state++) {
                    pad->data[state] = 0;
                }
                pad->count = func_00192f98(port, 0, 4, -1);
                id = func_00192f98(port, 0, 1, 0);
                if (id == 0) {
                    break;
                }
                pad->mode = id;
                id = func_00192f98(port, 0, 2, 0);
                if (id > 0) {
                    pad->mode = id;
                }
                if (pad->count != 0) {
                    pad->state = 1;
                } else {
                    pad->state = 5;
                }
                break;
            case 1:
                pad->state = 5;
                for (offs = 0; offs < pad->count; offs++) {
                    id = func_00192f98(port, 0, 4, offs);
                    if (id == 7 && func_001930d0(port, 0, offs, 3) == 1) {
                        pad->mode = id;
                        pad->state = 2;
                        break;
                    }
                }
                break;
            case 2:
                id = func_00192cd0(port, 0);
                if (id != 0) {
                    if (id == 1) {
                        pad->state = 1;
                    }
                } else {
                    pad->pressed = -1; /* the one-bit field is signed: -1 sets the bit */
                    pad->state = 3;
                }
                break;
            case 3:
                pad->state = 5;
                if (func_00193488(port, 0) == 1 && func_001934e8(port, 0) == 1) {
                    pad->state = 4;
                }
                break;
            case 4:
                id = func_00192cd0(port, 0);
                if (id != 0) {
                    if (id == 1) {
                        pad->state = 3;
                    }
                } else {
                    pad->ready = -1; /* likewise */
                    pad->state = 5;
                }
                break;
            case 5:
                break;
            }
        }
    }
}
