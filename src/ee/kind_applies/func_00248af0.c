#include "dotr/ee/kind_applies.h"

unsigned int Chk_Enc(int nSide, int cID, int eNO) {
    int cNO;

    cNO = SzDuel_GetUnitCardNo(nSide, cID);
    if (Do_ChkEncMatch(nSide, cID, eNO)) {
        switch (EnchantId[eNO]) {
        case 4:
            switch (gAnmPat_Bakura[eNO]) {
            case 6:
                return eNO | 0x40444000;
            case 7:
                return eNO | 0x406ac000;
            case 8:
                switch (cNO) {
                case 342:
                    return eNO | 0x407a0000;
                case 7:
                    return eNO | 0x407d4000;
                }
                break;
            case 9:
                switch (cNO) {
                case 16:
                    return eNO | (((func_0019a5c8() % 5) + 13) << 14 | 0x40000000);
                case 17:
                    return eNO | (((func_0019a5c8() % 5) + 13) << 14 | 0x40000000);
                case 167:
                    return eNO | (((func_0019a5c8() % 15) + 209) << 14 | 0x40000000);
                case 332:
                    return eNO | (((func_0019a5c8() % 32) + 101) << 14 | 0x40000000);
                case 379:
                    return eNO | ((TaEnc_GetRandomElf() & 0xffff) << 14 | 0x40000000);
                }
                break;
            }
            break;
        case 0:
            switch (gAnmPat_Bakura[eNO]) {
            case 0:
                return eNO | 0x864000;
            default:
                return eNO | 0x860000;
            }
        case 3:
            return eNO | 0x874000;
        case 2:
            return eNO | 0x878000;
        case 1:
            switch (gAnmPat_Bakura[eNO]) {
            case 5:
                return eNO | 0x870000;
            default:
                return eNO | 0x860000;
            }
        default:
            return eNO | 0x860000;
        }
    } else {
        return 999;
    }
    return 999;
}
