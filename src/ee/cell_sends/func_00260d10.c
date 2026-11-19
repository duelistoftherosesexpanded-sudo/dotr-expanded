#include "dotr/ee/cell_sends.h"

void TaView_ReqFocusCamera3(unsigned char who, int packed, int add, int value)
{
    struct DotrPresetVector at;
    int in[4];
    float out[4];
    int y;
    int x;
    int cell;

    SzView_FreeRequest(0, 0);
    if (SzEffect_IsModelStandby2(packed)) {
        x = packed >> 8;
        y = packed & 0xff;
        if (SzDuel_GetUnitStatus(x, y, 4) || y == 0) {
            if (SzDuel_GetUnitCtlRateM(x, y) > 0) {
                TaView_SetModelFrontView(in, packed, add);
                SzView_IV2V(in, out);
                out[0] += SzEffect_GetRadius2(packed);
                func_001fa8a0_tag(0, 0, 7, who, SzDuel_GetUnitModelID(x, y), 0);
                func_001fa8a0_tag(0, 0, 4, who, 0, out);
                return;
            }
        }
    }
    cell = SzDuel_GetUnitLoc(packed >> 8, packed & 0xff);
    SzDuel_CalcUnitPos(cell & 0xff, cell >> 8, 0, &at);
    Ta_GetStdFocusView2(in, value);
    SzView_IV2V(in, out);
    func_001fa8a0_tag(0, 0, 3, who, 0, &at);
    func_001fa8a0_tag(0, 0, 4, who, 0, out);
}
