#include "dotr/ee/scaled_sprite.h"

DotrSpriteWord SzDuel_DrawHandCursor_Pipe(DotrSpriteWord cursor)
{
    unsigned char *control = data_00317490_bytes;
    int ready = SzGame_GetMode(1);
    int side = turnOwner[0];
    int count = SzDuel_GetHandCount(side);
    int i;
    struct DotrSpriteMark *mark;
    struct DotrSpritePoint *point;
    int x;
    int y;
    int word;
    int digit;

    if (ready != 1) {
        return cursor;
    }
    if (control[3397] == 0) {
        return cursor;
    }
    if (control[3399] != 0) {
        return cursor;
    }
    for (i = 0; i < count; i++) {
        mark = func_001df540_mark(2, i + 1);
        if (mark == 0) {
            continue;
        }
        point = mark->unknown_04;
        if (point == 0) {
            continue;
        }
        x = func_001a22c0(point->x) + 260;
        y = func_001a22c0(point->y) + 139;
        word = SzDuel_InqHand(side, i);
        if (word < 0) {
            continue;
        }
        if (control[3407] == i) {
            cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, -1, -1, 8, x, y - 4, 12, 19, 1);
        }
        if ((word & 0xfff) == 999) {
            continue;
        }
        digit = (word & 0xf000) >> 12;
        if (digit == 0) {
            continue;
        }
        cursor = func_001b3c80_cursor(cursor, data_002902a6, x + 1728, y + 1796, 1, 1, 0, 0, 1, 0, -1);
        cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, x + 16, y - 23, 0, 2, 1, digit);
    }
    return cursor;
}
