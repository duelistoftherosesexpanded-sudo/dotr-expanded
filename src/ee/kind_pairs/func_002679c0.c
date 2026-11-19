#include "dotr/ee/kind_pairs.h"

/* -1 unless the second cell's side differs from SzDuel_GetTurnOwner(), both cells' SzDuel_GetUnitKind
 * values are below 32, b and d are non-zero and Ta_IsUnitHide2 passes the second cell's item;
 * then, when the second cell has bit 4 or Ta_SearchUncTypeNaturalEffectUserNum(61) is positive, the
 * ChkAttrAdv relation of the two cells' SzDuel_GetUnitAttr kinds plus one. */
int Ta_IsVisualAttrAdv(int a, int b, int c, int d)
{
    int side = SzDuel_GetTurnOwner();

    if (side == Ta_GetCurrentSide(c, d) || SzDuel_GetUnitKind(a, b) >> 5 != 0 || SzDuel_GetUnitKind(c, d) >> 5 != 0 || b == 0 ||
        d == 0) {
        return -1;
    }
    if (Ta_IsUnitHide2(SzDuel_GetUnit(c, d)) != 0) {
        return -1;
    }
    if (SzDuel_GetUnitStatus(c, d, 4) != 0) {
        return ChkAttrAdv(SzDuel_GetUnitAttr(a, b), SzDuel_GetUnitAttr(c, d)) + 1;
    }
    if (Ta_SearchUncTypeNaturalEffectUserNum(61) > 0) {
        return ChkAttrAdv(SzDuel_GetUnitAttr(a, b), SzDuel_GetUnitAttr(c, d)) + 1;
    }
    return -1;
}
