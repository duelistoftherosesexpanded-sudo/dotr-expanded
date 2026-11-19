#include "dotr/ee/link_step.h"
int TakuCard_inti(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    int status[2];
    short i;
    p_kaban_data = SzData_GetData(0);
    data_0034faec = SzData_GetData(1);
    Taku_card_info.first_byte = 1;
    func_00195108(&data_0034fb00, 0x840);
    data_0034fb00.unknown_30 = 0;
    data_0034fb00.unknown_34 = 0;
    data_0034fb00.unknown_36 = 0;
    data_0034fb00.unknown_38 = 0;
    data_0034fb00.unknown_3a = 0;
    data_0034fb00.unknown_3c = 0;
    data_0034fb00.unknown_3e = 0;
    data_0034fb00.unknown_40 = 0;
    data_0034fb00.unknown_42 = 0;
    data_0034fb00.unknown_54 = 0;
    data_0034fb00.unknown_56 = 0;
    data_0034fb00.unknown_48 = 0;
    data_0034fb00.unknown_4c = 0;
    data_0034fb00.unknown_50 = 0;
    data_0034fb00.unknown_52 = 0;
    data_0034fb00.unknown_08 = 0;
    data_0034fb00.unknown_0c = 0;
    data_0034fb00.unknown_10 = 0;
    data_0034fb00.unknown_14 = 0;
    data_0034fb00.unknown_18 = 0;
    data_0034fb00.unknown_1c = 0;
    data_0034fb00.unknown_20 = 0;
    data_0034fb00.unknown_24 = 0;
    data_0034fb00.unknown_2c = 0;
    data_0034fb00.unknown_2e = 0;
    data_0034fb00.unknown_5c = 0;
    data_0034fb00.unknown_60 = 0;
    data_0034fb00.unknown_68 = 6;
    data_0034fb00.unknown_64 = 0;
    data_0034fb00.unknown_284 = 0;
    data_0034fb00.unknown_288 = 0;
    data_0034fb00.unknown_28c = 0;
    data_0034fb00.unknown_290 = 0;
    data_0034fb00.unknown_294 = 0;
    data_0034fb00.unknown_296 = 0;
    data_0034fb00.unknown_2b4 = 0;
    data_0034fb00.unknown_280 = 0;
    data_0034fb00.unknown_2a8 = 0;
    data_0034fb00.unknown_2aa = 0;
    data_0034fb00.unknown_2ac = 0;
    data_0034fb00.unknown_2ae = 0;
    data_0034fb00.unknown_2b0 = 0;
    data_0034fb00.unknown_2b2 = 0;
    data_0034fb00.unknown_2b6 = 0;
    data_0034fb00.unknown_2b8 = 0;
    for (i = 0; i < 2; i++) {
        func_00194380(i, link->unknown_0c, &link->unknown_10, &link->unknown_14, &link->unknown_18);
        func_00194248(0, &link->unknown_20, &status[i]);
    }
    if (mode == 1) {
        if (status[0] != 0 || status[1] != 0) {
            return 1;
        }
    }
    return 0;
}
