#include "dotr/ee/camp_name_set.h"

void Taku_Camp_Ivent_Name_Set(void)
{
    /* MATCHING DEVICE: the work pointer is declared first and the board pointer last. */
    struct DotrCampNameWork *w = DOTR_CAMP_NAME_WORK;
    short i;
    short row;
    short col;
    short rem;
    unsigned short *q;
    int sum;
    unsigned short j;
    /* MATCHING DEVICE: a cast of the byte array symbol, not a struct object symbol (see the header). */
    struct DotrCampNameBoard *b = (struct DotrCampNameBoard *)data_00299e10;

    for (i = 0; i < 12; i++) {
        row = w->code[i] / 5;
        rem = w->code[i] % 5;
        col = rem + w->page[i] * 6;
        if (i == 0) {
            /* MATCHING DEVICE: `i * 2` although i is 0 here. */
            q = (unsigned short *)((char *)DOTR_CAMP_NAME_TEXT(2) + i * 2);
            switch (w->mode[i]) {
            case 0:
                *q = DOTR_CAMP_NAME_AT(row + 11, col) & 0x1fff;
                break;
            case 1:
                *q = DOTR_CAMP_NAME_AT(row + 14, col) & 0x1fff;
                break;
            default:
                *q = DOTR_CAMP_NAME_AT(row + 8, col) & 0x3fff;
                break;
            }
        }
        q = (unsigned short *)((char *)DOTR_CAMP_NAME_TEXT(2) + i * 2 + 4);
        switch (w->mode[i]) {
        case 0:
            *q = DOTR_CAMP_NAME_AT(row + 11, col) & 0x1fff;
            break;
        case 1:
            *q = DOTR_CAMP_NAME_AT(row + 14, col) & 0x1fff;
            break;
        default:
            *q = DOTR_CAMP_NAME_AT(row + 8, col) & 0x3fff;
            break;
        }
        /* MATCHING DEVICE: `i == 12` cannot hold inside the loop; the original tests it. */
        if (i == 12 || i == w->end) {
            *q |= 0x8000;
        }
        b->name[i] = *q;
    }
    for (i = 0, sum = 0; i < 12; i++) {
        sum += (unsigned short)b->name[i] % 16;
    }
    w->hash = sum & 0xf;
    for (j = 0; j < 3; j++) {
        w->pick[j] = DeckTab_nameset[w->hash] >> (j * 5) & 0x1f;
        w->deck[j] = data_00299d00_nameset[w->pick[j]].id & 0x3ff;
    }
}
