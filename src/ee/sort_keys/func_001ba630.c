#include "dotr/ee/sort_keys.h"

int lSzDuel_SortFunc1(unsigned short *a, unsigned short *b)
{
    struct DotrSortBlock *block = (struct DotrSortBlock *)&data_00317490;
    int result = 0;
    int i, j;

    for (i = 0; i < 3; i++) {
        if (i != 0) {
            for (j = 0; j < i; j++) {
                if (block->keys_a[i] == block->keys_a[j])
                    break;
            }
            if (j < i)
                continue;
        }
        result = data_00291330[block->keys_a[i]](a, b);
        if (result != 0)
            break;
    }
    return result;
}
