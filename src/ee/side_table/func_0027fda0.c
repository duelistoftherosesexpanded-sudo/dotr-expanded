#include "dotr/ee/side_table.h"

int TaTuto_GetCardNameMsgID(int id)
{
    if (SzData_GetOption(SzDuel_GetTurnOwner(), 0) == 0) {
        return id;
    }
    switch (id - 2524) {
    case 27:
        return 3054;
    case 479:
        return 3055;
    case 480:
        return 3056;
    case 481:
        return 3057;
    case 311:
        return 3058;
    case 319:
        return 3059;
    case 325:
        return 3060;
    case 328:
        return 3061;
    case 338:
        return 3062;
    case 339:
        return 3063;
    case 341:
        return 3064;
    case 353:
        return 3065;
    case 358:
        return 3066;
    case 360:
        return 3067;
    case 381:
        return 3068;
    case 386:
        return 3069;
    case 388:
        return 3070;
    case 427:
        return 3071;
    case 430:
        return 3072;
    }
    return id;
}
