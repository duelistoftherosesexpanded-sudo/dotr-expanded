#include "dotr/ee/glide_steps.h"

/* Threads the draw cursor through the camp's 3D pass (Taku_Camp_3D) or its stone pass
 * (Taku_Camp_STON), chosen by the board's tick count and, for five counts, by the halfword
 * at 0x1ed536c; then through Taku_Camp_Kumo when the count is at least 10000. */
int TkCamp_Draw_Pipe(int cursor)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;

    if (data_00299e10_glide.ticks >= 10000) {
        switch (board->ticks) {
        case 11160:
            if (*DOTR_GLIDE_LIMIT < 2) {
                cursor = Taku_Camp_3D(cursor);
            } else {
                cursor = Taku_Camp_STON(cursor);
            }
            break;
        case 11162:
            if (*DOTR_GLIDE_LIMIT >= 9) {
                cursor = Taku_Camp_3D(cursor);
            } else {
                cursor = Taku_Camp_STON(cursor);
            }
            break;
        case 11180:
            if (*DOTR_GLIDE_LIMIT >= 2) {
                cursor = Taku_Camp_STON(cursor);
            } else {
                cursor = Taku_Camp_3D(cursor);
            }
            break;
        case 12132:
            if (*DOTR_GLIDE_LIMIT < 10) {
                cursor = Taku_Camp_STON(cursor);
            } else {
                cursor = Taku_Camp_3D(cursor);
            }
            break;
        case 12150:
            if (*DOTR_GLIDE_LIMIT > 1) {
                cursor = Taku_Camp_STON(cursor);
            } else {
                cursor = Taku_Camp_3D(cursor);
            }
            break;
        case 11000:
        case 11182:
        case 11184:
        case 12000:
        case 12130:
        case 12152:
        case 12154:
        case 12156:
            cursor = Taku_Camp_STON(cursor);
            break;
        default:
            if (board->ticks < 11000) {
                cursor = Taku_Camp_STON(cursor);
            } else {
                cursor = Taku_Camp_3D(cursor);
            }
            break;
        }
    } else {
        cursor = Taku_Camp_3D(cursor);
    }
    if (board->ticks >= 10000) {
        cursor = Taku_Camp_Kumo(cursor);
    }
    return cursor;
}
