#include "dotr/ee/small_misc.h"

int SzText_DoRequest_Pipe(int cursor, int kind, int pos, int *out)
{
    struct DotrMiscScanEntry *textureItem = gSzText_aItem;
    int start = pos;
    int i;
    int n;

    for (i = 0; i < 1024; i++, textureItem++) {
        if (textureItem->unknown_00 == 0) {
            continue;
        }
        if (!textureItem->unknown_08.unknown_bit_0) {
            continue;
        }
        if (textureItem->unknown_08.unknown_bit_1) {
            continue;
        }
        if (kind >= 0 && textureItem->unknown_08.unknown_bits_3_15 != kind) {
            continue;
        }
        cursor = func_00202fb0_word(cursor, textureItem->unknown_00, pos, &n);
        pos += n;
    }
    if (out) {
        *out = pos - start;
    }
    return cursor;
}
