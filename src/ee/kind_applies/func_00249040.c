#include "dotr/ee/kind_applies.h"
int Ta_GetHandEnchant3(int cNo1, int cNo2)
{
    switch (gAnmPat_Bakura[cNo2]) {
    case 6: /* Elegant Egotist */
        return 273;
    case 7: /* Cocoon of Evolution */
        return 427;
    case 8: /* Metalmorph */
        switch (cNo1) {
        case 342:
            return 488;
        case 7:
            return 501;
        default:
            return -1;
        }
    case 9:
        switch (cNo1) {
        case 16:
            return func_0019a5c8() % 5 + 13;
        case 17:
            return func_0019a5c8() % 5 + 13;
        case 167:
            return func_0019a5c8() % 15 + 209;
        case 332:
            return func_0019a5c8() % 32 + 101;
        case 379:
            return (unsigned short)TaEnc_GetRandomElf();
        default:
            return -1;
        }
    default:
        return cNo1 | 0x10000;
    }
}
