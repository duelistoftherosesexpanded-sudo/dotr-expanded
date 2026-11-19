#include "dotr/ee/text_picks.h"

int lSzGame_GetMCardMsg(int kind, int flags, int *out)
{
    int on = flags & 0x80 ? 1 : 0;
    int result = 0;

    *out = (on ? 1 : 0) + 2482;
    switch (flags & 0x7f) {
    case 0:
        break;
    case 1:
        *out = 2521;
        result = 2;
        break;
    case 2:
    case 4:
        *out = (on ? 1 : 0) + 2485;
        result = 1;
        break;
    case 3:
    case 14:
        *out = (on ? 1 : 0) + 2506;
        result = 2;
        break;
    case 5:
        if (kind == 6) {
            *out = 2491;
        } else {
            *out = (on ? 3 : 0) + 2490;
        }
        result = 1;
        break;
    case 6:
        *out = (on ? 1 : 0) + 2488;
        result = 1;
        break;
    case 7:
        *out = (on ? 2 : 0) + 2495;
        result = 1;
        break;
    case 9:
        if (kind == 4 || kind == 5) {
            if (kind == 4) {
                *out = 2501;
            } else {
                *out = 2518;
            }
        } else if (kind == 0 || kind == 3) {
            *out = (on ? 1 : 0) + 2499;
        } else {
            *out = (on ? 1 : 0) + 2516;
        }
        break;
    case 8:
    case 10:
        if (kind == 0 || kind == 3 || kind == 4) {
            *out = 2503;
        } else {
            *out = 2520;
        }
        result = 1;
        break;
    case 11:
        *out = (on ? 1 : 0) + 2510;
        break;
    case 12:
        *out = (on ? 1 : 0) + 2512;
        result = 3;
        break;
    case 13:
        *out = (on ? 1 : 0) + 2514;
        result = 1;
        break;
    }
    switch (flags & 0xff00) {
    case 0x100:
    case 0x200:
    case 0x300:
    case 0x400:
    case 0x500:
        if (kind == 4) {
            *out = 2522;
        } else if (kind == 0 || kind == 3) {
            *out = 2502;
        } else {
            *out = 2519;
        }
        break;
    case 0x600:
        *out = 2523;
        break;
    }
    return result;
}
