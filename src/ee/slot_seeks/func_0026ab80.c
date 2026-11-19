#include "dotr/ee/slot_seeks.h"

int Ta_IsActiveTrapB(int side, int trapEffect)
{
    switch (trapEffect) {
    case 41:
    case 42:
    case 44:
    case 35:
    case 74:
    case 75:
    case 14:
    case 4:
    case 48:
    case 49:
        return Ta_GetActiveTrapBUnit(side, 2, trapEffect);
    case 80:
    case 76:
    case 45:
    case 17:
    case 46:
    case 43:
        return Ta_GetActiveTrapBUnit(side, 3, trapEffect);
    default:
        return -1;
    }
}
