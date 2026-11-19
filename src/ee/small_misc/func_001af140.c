#include "dotr/ee/small_misc.h"

int SzDuel_GetWindowTexture(int index)
{
    struct DotrMiscControlWords *c = &data_00317490_words;
    if (index < 0 || index >= 8) {
        return 0;
    }
    return c->unknown_6a0[index];
}
