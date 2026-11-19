#include "dotr/ee/sheet_marks.h"

/* Draws the template item at a corner chosen by the byte at 0x1ed537e, with a row that
 * cycles 152, 153, 154 every five counts of the byte at 0x1ed53b2, when the state byte is 4
 * or the stage byte is 2 and the board is not in one of three (mode, ticks) pairs; then
 * counts the byte up. Returns Taku_Obj_Set's result, or a0 unchanged when nothing is drawn.
 * A corner byte outside 0-3 leaves x and y unset, as in the original. */
int Taku_Camp_BOTAN_MATI(int a0)
{
    struct DotrMarkBoard *board = &data_00299e10_mark;
    struct DotrMarkItem item = data_0029abc0;
    int show = 1;
    short x;
    short y;
    int phase;

    switch (DOTR_MARK_FLAGS->corner) {
    case 0:
        x = 460;
        y = 104;
        break;
    case 1:
        x = 600;
        y = 408;
        break;
    case 2:
        x = 600;
        y = 408;
        break;
    case 3:
        x = 460;
        y = 104;
        break;
    }
    phase = DOTR_MARK_FLAGS->frame % 15;
    if (phase < 5) {
        item.row = 152;
    } else if (phase < 10) {
        item.row = 153;
    } else if (phase < 15) {
        item.row = 154;
    }
    if (board->mode == 0 && board->ticks == 100) {
        show = 0;
    }
    if (board->mode == 1 && board->ticks == 105) {
        show = 0;
    }
    if (board->mode == 1 && board->ticks == 106) {
        show = 0;
    }
    if ((DOTR_MARK_FLAGS->state == 4 || DOTR_MARK_FLAGS->stage == 2) && show) {
        a0 = func_00217bd0_mark(a0, &item, x, y);
        DOTR_MARK_FLAGS->frame++;
    }
    return a0;
}
