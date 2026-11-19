#include "dotr/ee/kind_gates.h"

int TaLogic002_GetLinePiece(unsigned char lineType, unsigned char lineIndex)
{
    if (lineType == 1) {
        switch (lineIndex) {
        case 0:
            return 0x203;
        case 1:
            return 0x302;
        case 2:
            return 0x301;
        case 3:
            return 0x300;
        case 4:
            return 0x104;
        case 6:
            return 4;
        case 5:
            break;
        }
        return 0x203;
    }
    switch (lineIndex) {
    case 0:
        return 0x102;
    case 1:
        return 0x201;
    case 2:
        return 0x200;
    case 3:
        return 2;
    }
    return 0x102;
}
