#include "dotr/ee/kind_gates.h"

int Ta_GetDestinyDraw(int side, unsigned short *cardPool, short *poolWeights, int *note)
{
    int flags;
    int kind;
    int delta;
    int n;

    flags = SzDuel_InqDestinyFlag(side);
    kind = SzDuel_GetUnitKind(side, 0) & 0x1f;
    delta = (flags & 0xf0) * 5;
    cardPool[0] = 999;
    cardPool[1] = 999;
    cardPool[2] = 999;
    switch (kind) {
    case 0:
        poolWeights[0] = 0;
        poolWeights[1] = 0;
        poolWeights[2] = 0;
        break;
    case 1:
        poolWeights[0] = 2;
        poolWeights[1] = 1;
        poolWeights[2] = -3;
        break;
    case 2:
        poolWeights[0] = 12;
        poolWeights[1] = 6;
        poolWeights[2] = -18;
        break;
    case 3:
        poolWeights[0] = 2;
        poolWeights[1] = 1;
        poolWeights[2] = -3;
        break;
    case 4:
        poolWeights[0] = 5;
        poolWeights[1] = 3;
        poolWeights[2] = -8;
        break;
    case 5:
        poolWeights[0] = 8;
        poolWeights[1] = 4;
        poolWeights[2] = -12;
        break;
    case 6:
        poolWeights[0] = 6;
        poolWeights[1] = 3;
        poolWeights[2] = -9;
        break;
    case 7:
        poolWeights[0] = 0;
        poolWeights[1] = 0;
        poolWeights[2] = 0;
        break;
    case 8:
        poolWeights[0] = 12;
        poolWeights[1] = 6;
        poolWeights[2] = -18;
        break;
    case 9:
        poolWeights[0] = 10;
        poolWeights[1] = 5;
        poolWeights[2] = -15;
        break;
    case 10:
        poolWeights[0] = 14;
        poolWeights[1] = 7;
        poolWeights[2] = -21;
        break;
    case 11:
        poolWeights[0] = 15;
        poolWeights[1] = 8;
        poolWeights[2] = -23;
        break;
    case 12:
        poolWeights[0] = 8;
        poolWeights[1] = 4;
        poolWeights[2] = -12;
        break;
    case 13:
        poolWeights[0] = 14;
        poolWeights[1] = 7;
        poolWeights[2] = -21;
        break;
    case 14:
        poolWeights[0] = 3;
        poolWeights[1] = 1;
        poolWeights[2] = -4;
        break;
    case 15:
        poolWeights[0] = 10;
        poolWeights[1] = 5;
        poolWeights[2] = -15;
        break;
    case 16:
        poolWeights[0] = 2;
        poolWeights[1] = 1;
        poolWeights[2] = -3;
        break;
    case 17:
        poolWeights[0] = 13;
        poolWeights[1] = 6;
        poolWeights[2] = -19;
        break;
    case 18:
        poolWeights[0] = 10;
        poolWeights[1] = 5;
        poolWeights[2] = -15;
        break;
    case 19:
        poolWeights[0] = 10;
        poolWeights[1] = 5;
        poolWeights[2] = -15;
        break;
    case 20:
        poolWeights[0] = -5;
        poolWeights[1] = -2;
        poolWeights[2] = 7;
        break;
    default:
        return -1;
    }
    if (!(flags & 0x80)) {
        n = flags & 0xf;
        cardPool[0] = Destiny_Tab[n][0];
        cardPool[1] = Destiny_Tab[n][1];
        cardPool[2] = Destiny_Tab[n][2];
        poolWeights[0] += delta;
        poolWeights[1] += (short)(delta >> 1);
        poolWeights[2] = -(poolWeights[0] + poolWeights[1]);
        poolWeights[0] += 10;
        poolWeights[1] += 20;
        poolWeights[2] += 70;
        flags |= 0x80;
        SzDuel_SetDestinyFlag(side, flags & 0xff);
        if (note) {
            note[0] = 114;
            note[1] = SzDuel_GetTurnOwner();
            note[2] = 0;
            note[3] = 0;
        }
        return 1;
    }
    return -1;
}
