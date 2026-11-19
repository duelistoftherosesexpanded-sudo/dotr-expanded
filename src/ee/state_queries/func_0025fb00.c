#include "dotr/ee/state_queries.h"

void Ta_CalcLPinEffectSeq(int side, int mode, int value)
{
    switch (mode) {
    case 0:
        value = Ta_GetLPRevByNaturalEffect(43, value);
        SzDuel_CalcLP(side, 0, value);
        break;
    case 1:
        SzDuel_CalcLP(side, 1, Ta_GetLPRevByNaturalEffect(44, value));
        break;
    case 2:
        SzDuel_CalcLP(side, 0, Ta_GetLPRevByNaturalEffect(43, SzDuel_GetLP(side)));
        break;
    case 3:
        SzDuel_CalcLP(side, 1, Ta_GetLPRevByNaturalEffect(44, SzDuel_GetLP(side)));
        break;
    case 4:
        if (SzDuel_GetLP(side) < value) {
            SzDuel_CalcLP(side, 4, value);
        }
        break;
    }
}
