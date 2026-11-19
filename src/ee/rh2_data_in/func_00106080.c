#include "dotr/ee/rh2_data_in.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* RH2_DATA_IN2 (0x106080): the rectangle of sheet `arg1`, entry `arg2`, copied to arg0.
 * Compiled warm (see its build entry).
 * Sheets and entries without a case leave the local rectangle unset, as in the original; the
 * copy still happens. */
void RH2_DATA_IN2(void *arg0, short arg1, short arg2) {
    struct DotrRh2Rect rect;

    if (arg1 == 0x8C) {
        if (arg2 == 0) {
            rect.x = 1;
            rect.y = 1;
            rect.w = 0x1E;
            rect.h = 0x7E;
        }
    } else if (arg1 == 0x8D) {
        if (arg2 == 0) {
            rect.x = 0;
            rect.y = 0x80;
        } else if (arg2 == 1) {
            rect.x = 0x20;
            rect.y = 0x80;
        } else if (arg2 == 2) {
            rect.x = 0x40;
            rect.y = 0x80;
        }
        rect.w = 0x1F;
        rect.h = 0x7F;
    } else if (arg1 == 0x8E) {
        if (arg2 < 4) {
            rect.x = 0x21;
            rect.y = (arg2 << 5) + 1;
        } else {
            rect.x = 0x41;
            rect.y = ((arg2 - 4) << 5) + 1;
        }
        rect.w = 0x1E;
        rect.h = 0x1E;
    } else if (arg1 == 0x8F) {
        rect.x = 0x60;
        rect.y = 0xF0;
        rect.w = 0x9F;
        rect.h = 0xF;
    } else if (arg1 == 0x96) {
        rect.x = (arg2 * 0x2C) + 0xA;
        rect.y = 0;
        rect.w = 0x2B;
        rect.h = 0x7F;
    } else if (arg1 == 0x97) {
        rect.x = 0x8F;
        rect.y = 0;
        rect.w = 0x1F;
        rect.h = 0x7F;
    } else if (arg1 == 0x98) {
        rect.x = 0xB0;
        rect.y = 0;
        rect.w = 0x3B;
        rect.h = 0x1F;
    } else if (arg1 == 0x99) {
        rect.x = 0xF1;
        rect.y = arg2 * 0x10;
        rect.w = 0xE;
        rect.h = 0xF;
    } else if (arg1 == 0x9A) {
        rect.x = (arg2 << 5) + 0xB1;
        rect.y = 0x21;
        rect.w = 0x1E;
        rect.h = 0x1E;
    } else if (arg1 == 0x9B) {
        if (arg2 < 2) {
            rect.x = (arg2 << 5) + 0xB1;
            rect.y = 0x41;
        } else {
            rect.x = ((arg2 - 2) << 5) + 0xB1;
            rect.y = 0x61;
        }
        rect.w = 0x1E;
        rect.h = 0x1E;
    } else if (arg1 == 0x9C) {
        rect.x = 0x80;
        rect.y = 0x81;
        rect.w = 0x7F;
        rect.h = 0x3F;
    } else if (arg1 == 0x9D) {
        if (arg2 < 4) {
            rect.x = (arg2 << 5) + 0x80;
            rect.y = 0xC1;
        } else {
            rect.x = ((arg2 - 4) << 5) + 0x80;
            rect.y = 0xE1;
        }
        rect.w = 0x1F;
        rect.h = 0x1F;
    } else if (arg1 == 0x9E) {
        rect.x = 1;
        rect.y = (arg2 * 0xA) + 0xD1;
        rect.w = 8;
        rect.h = 8;
    } else if (arg1 == 0x9F) {
        if (arg2 < 2) {
            rect.x = (arg2 * 0x3B) + 0xB;
            rect.y = 0x81;
        } else {
            rect.x = ((arg2 - 2) * 0x3B) + 0xB;
            rect.y = 0xC1;
        }
        rect.w = 0x38;
        rect.h = 0x3F;
    } else if (arg1 == 0x64) {
        rect.w = 0x12;
        rect.h = 0x1F;
        if (arg2 == 0) {
            rect.x = 1;
            rect.y = 1;
        } else if (arg2 == 1) {
            rect.x = 0x15;
            rect.y = 1;
        } else if (arg2 == 2) {
            rect.x = 0x29;
            rect.y = 1;
        } else if (arg2 == 3) {
            rect.x = 0x3D;
            rect.y = 1;
        } else if (arg2 == 4) {
            rect.x = 1;
            rect.y = 0x21;
        } else if (arg2 == 5) {
            rect.x = 0x15;
            rect.y = 0x21;
        } else if (arg2 == 6) {
            rect.x = 0x29;
            rect.y = 0x21;
        } else if (arg2 == 7) {
            rect.x = 0x3D;
            rect.y = 0x21;
        }
    } else if (arg1 == 0x65) {
        rect.x = 0x50;
        rect.y = 0;
        rect.w = 0x2F;
        rect.h = 0x3F;
    } else if (arg1 == 0x6E) {
        rect.x = arg2 << 6;
        rect.y = 0;
        rect.w = 0x3F;
        rect.h = 0x3F;
    } else if (arg1 == 0x3C) {
        rect.x = arg2 << 6;
        rect.y = 0;
        rect.w = 0x3F;
        rect.h = 0x3F;
    } else if (arg1 == 0x3D) {
        rect.w = 0x1F;
        rect.h = 0x1F;
        if (arg2 == 0) {
            rect.x = 0x21;
            rect.y = 0;
        } else if (arg2 == 1) {
            rect.x = 0x21;
            rect.y = 0x21;
        } else if (arg2 == 2) {
            rect.x = 0x41;
            rect.y = 0;
        } else if (arg2 == 3) {
            rect.x = 0x41;
            rect.y = 0x21;
        }
    } else if (arg1 == 0x5A) {
        rect.x = arg2 << 6;
        rect.y = 1;
        rect.w = 0x3F;
        rect.h = 0x3F;
    } else if (arg1 == 0x14) {
        if (arg2 < 4) {
            rect.x = arg2 << 5;
            rect.y = 1;
        } else {
            rect.x = (arg2 - 4) << 5;
            rect.y = 0x21;
        }
        rect.w = 0x1F;
        rect.h = 0x1F;
    } else if (arg1 == 0x82) {
        rect.w = 0x12;
        rect.h = 0x1F;
        if (arg2 == 0) {
            rect.x = 1;
            rect.y = 0x41;
        } else if (arg2 == 1) {
            rect.x = 0x15;
            rect.y = 0x41;
        } else if (arg2 == 2) {
            rect.x = 0x29;
            rect.y = 0x41;
        } else if (arg2 == 3) {
            rect.x = 0x3D;
            rect.y = 0x41;
        } else if (arg2 == 4) {
            rect.x = 1;
            rect.y = 0x61;
        } else if (arg2 == 5) {
            rect.x = 0x15;
            rect.y = 0x61;
        } else if (arg2 == 6) {
            rect.x = 0x29;
            rect.y = 0x61;
        } else if (arg2 == 7) {
            rect.x = 0x3D;
            rect.y = 0x61;
        }
    } else if (arg1 == 0x83) {
        if (arg2 < 2) {
            rect.x = (arg2 * 0x18) + 0x51;
            rect.y = 0x51;
        } else {
            rect.x = ((arg2 - 2) * 0x18) + 0x51;
            rect.y = 0x51;
        }
        rect.w = 0x17;
        rect.h = 0x16;
    } else if (arg1 == 0x50) {
        rect.x = 1;
        rect.y = (arg2 * 0xA) + 1;
        rect.w = 8;
        rect.h = 8;
    } else if (arg1 == 0xA) {
        if (arg2 < 5) {
            rect.x = (arg2 * 0x18) + 1;
            rect.y = 1;
        } else {
            rect.x = ((arg2 - 5) * 0x18) + 1;
            rect.y = 0x21;
        }
        rect.w = 0x17;
        rect.h = 0x1F;
    } else if (arg1 == 0x1E) {
        if (arg2 < 4) {
            rect.x = arg2 * 0x10;
            rect.y = 0;
        } else if (arg2 < 8) {
            rect.x = (arg2 - 4) * 0x10;
            rect.y = 0x10;
        } else if (arg2 < 0xC) {
            rect.x = (arg2 - 8) * 0x10;
            rect.y = 0x20;
        } else if (arg2 < 0x10) {
            rect.x = (arg2 - 0xC) * 0x10;
            rect.y = 0x30;
        }
        rect.w = 0x10;
        rect.h = 0x10;
    }

    *(struct DotrRh2Rect *)arg0 = rect;
}
