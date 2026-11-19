#include "dotr/ee/packet_builds.h"

/* lRhBase_InitGsAndVif: stores the 64-bit word at 0x326180, then builds one fixed
 * packet at 0x325f00 whose register 64 value packs width - 1 and height - 1. */
void lRhBase_InitGsAndVif(int cursor)
{
    data_00326180_base.test = 0x7263051463721405;
    func_001875f8(data_00325f00, cursor);
    func_00187660(data_00325f00, 0);
    func_001877f8(data_00325f00, 0x1000404);
    func_00187810(data_00325f00, 0x20000000);
    func_00187810(data_00325f00, 0x5000000);
    func_00187778(data_00325f00, 2, 3);
    func_00187858(data_00325f00, 0);
    ((unsigned long *)&data_00325ef0)[0] = 0x1000000000008000;
    ((unsigned long *)&data_00325ef0)[1] = 14;
    func_001876b8(data_00325f00, data_00325ef0);
    func_00187828(data_00325f00, 70, 1);
    func_00187828(data_00325f00, 64, (long)(data_00326180_base.width - 1) << 16 | (long)(data_00326180_base.height - 1) << 48);
    func_00187828(data_00325f00, 26, 1);
    func_00187828(data_00325f00, 59, 0x8000000000);
    func_00187828(data_00325f00, 61, 0xc0c0c0);
    func_00187828(data_00325f00, 69, 1);
    func_00187828(data_00325f00, 68, data_00326180_base.test);
    func_001876d0(data_00325f00);
    func_001878b0(data_00325f00);
    func_00187608(data_00325f00);
}
