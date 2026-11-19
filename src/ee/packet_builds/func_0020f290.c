#include "dotr/ee/packet_builds.h"

int RhText_SetTextureFlush(int cursor)
{
    func_001875f8(data_0032a5e0, cursor);
    func_00187660(data_0032a5e0, 0);
    func_001877f8(data_0032a5e0, 0x11000000);
    func_00187778(data_0032a5e0, 2, 3);
    func_00187858(data_0032a5e0, 0);
    ((unsigned long *)&data_0032a600)[0] = 0x1000000000008000;
    ((unsigned long *)&data_0032a600)[1] = 14;
    func_001876b8(data_0032a5e0, data_0032a600);
    func_00187828(data_0032a5e0, 63, 1);
    func_001876d0(data_0032a5e0);
    func_001878b0(data_0032a5e0);
    return func_00187608(data_0032a5e0);
}
