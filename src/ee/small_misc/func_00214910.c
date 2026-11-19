#include "dotr/ee/small_misc.h"

int SzData_GetConstDeck(int deck, int slot)
{
    if (deck < 0 || deck >= 51) {
        return -1;
    }
    if (slot < 0 || slot >= 41) {
        return -1;
    }
    return gSzData_ConstDeck[deck][slot] & 0xfff;
}
