#include "dotr/ee/kind_sends.h"
void TaTuto_FocusUnit__00172510(int kind)
{
    int v[4];
    float out[4];
    int *block;
    int turn;
    int d;
    SzView_FreeRequest(0, 0);
    switch (kind) {
    case 60:
        block = SzDuel_GetUnit(0, 0);
        func_001f6160_words(0, v);
        v[0] += 63;
        turn = block[12];
        d = 18431 - turn;
        if (d < 0) {
            d = 83966 - turn;
        }
        v[1] += d;
        v[2] += 4096;
        SzView_IV2V(v, out);
        out[0] = out[0] + SzEffect_GetRadius2(0);
        func_001fa8a0_start(0, 0, 7, 0, SzDuel_GetUnitModelID(0, 0), 0);
        func_001fa8a0_start(0, 0, 4, 0, 0, out);
        break;
    case 0:
        block = SzDuel_GetUnit(1, 0);
        func_001f6160_words(256, v);
        v[0] += 63;
        turn = block[12];
        d = 18431 - turn;
        if (d < 0) {
            d = 83966 - turn;
        }
        v[1] += d;
        v[2] += 4096;
        SzView_IV2V(v, out);
        out[0] = out[0] + SzEffect_GetRadius2(256);
        func_001fa8a0_start(0, 0, 7, 0, SzDuel_GetUnitModelID(1, 0), 0);
        func_001fa8a0_start(0, 0, 4, 0, 0, out);
        break;
    case 156:
        block = SzDuel_GetUnit(0, 1);
        func_001f6160_words(1, v);
        v[0] += 63;
        turn = block[12];
        d = 51199 - turn;
        if (d < 0) {
            d = 116734 - turn;
        }
        v[1] += d;
        v[2] += 4096;
        SzView_IV2V(v, out);
        out[0] = out[0] + SzEffect_GetRadius2(1);
        func_001fa8a0_start(0, 0, 7, 0, SzDuel_GetUnitModelID(0, 1), 0);
        func_001fa8a0_start(0, 0, 4, 0, 0, out);
        break;
    case 3:
        block = SzDuel_GetUnit(1, 1);
        func_001f6160_words(257, v);
        v[0] += 63;
        turn = block[12];
        d = 18431 - turn;
        if (d < 0) {
            d = 83966 - turn;
        }
        v[1] += d;
        v[2] += 4096;
        SzView_IV2V(v, out);
        out[0] = out[0] + SzEffect_GetRadius2(257);
        func_001fa8a0_start(0, 0, 7, 0, SzDuel_GetUnitModelID(1, 1), 0);
        func_001fa8a0_start(0, 0, 4, 0, 0, out);
        break;
    }
}
