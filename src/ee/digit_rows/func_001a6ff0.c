#include "dotr/ee/digit_rows.h"

void SzGame_DrawMenu(void)
{
    struct DotrDigitView *view = &gSzGame_gl_digitview;
    int first;
    unsigned char *label;
    int words[4];
    int source[4];
    int slot;
    int digit;
    struct DotrDigitEntry *low;
    struct DotrDigitEntry *high;
    int i;
    unsigned int j;
    int label_id;
    int row;
    struct DotrDigitEntry *entry;
    int x;
    int scale;
    int y;
    unsigned char *walk;

    SzMsg_GetDefRect(source);
    *(struct DotrDigitWords *)words = *(struct DotrDigitWords *)source;
    low = SzMsg_GetInfo(20);
    high = SzMsg_GetInfo(21);
    first = gSzGame_gl_digits.page * 20;
    SzMsg_Init(0, RhBase_GetUserGsAddress());
    label = &DebugMenuOptions[first];
    for (i = 0, walk = label; i < 20; i++, walk++) {
        if ((unsigned int)(first + i) < 29) {
            SzMsg_LoadFont(i, *walk + 284, i < 10 ? 0 : 320, (i % 10 + 2) * 22);
        }
    }
    SzMsg_LoadFont(i, 21, 0, 0);
    SzMsg_LoadFont(i + 1, 24, 0, 22);
    low->words[2] = 22;
    high->words[2] = 22;
    ExFlushTexture();
    for (row = 0, y = 0; row < 20; row++, label++, y += 22) {
        entry = SzMsg_GetInfo(row);
        if ((unsigned int)(first + row) < 29) {
            label_id = *label;
            if (view->cursor == row) {
                words[2] = entry->words[2];
                if (view->flag[label_id] != 0) {
                    words[2] = (view->count[label_id] + 10) * 22;
                }
                SzMsg_DrawGlass(0, y, words, 0);
            }
            SzMsg_DrawX(row, 0, y, 0, SzMsg_DrawRect_Pipe);
            if (view->flag[label_id] != 0) {
                scale = 1;
                x = 220;
                for (j = 1; j < view->count[label_id]; j++) {
                    scale *= view->radix[label_id];
                }
                for (j = 0; j < view->count[label_id]; j++) {
                    digit = view->value[label_id] / scale % view->radix[label_id];
                    slot = 20;
                    if (digit < 10) {
                        low->words[0] = digit * 22;
                    } else {
                        slot++;
                        high->words[0] = (digit - 10) * 22;
                    }
                    SzMsg_DrawX(slot, x, y, 0, SzMsg_DrawRect_Pipe);
                    x += 22;
                    scale /= view->radix[label_id];
                }
            }
        }
        SzMsg_Free(row);
    }
}
