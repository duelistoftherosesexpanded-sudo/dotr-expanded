#include "dotr/ee/grid_scans.h"

int TaLogic012_IsLineEmpty(int id, int y)
{
    int block[24];
    unsigned char kind;
    int side;
    int k;
    unsigned char n;
    int x;
    int found;

    kind = TaHand_GetCardKind(id);
    func_001dcd70_item(block, id);
    if (kind >> 5 != 0) {
        return 0;
    }
    side = SzDuel_GetTurnOwner();
    for (k = 0; k < 7; k++) {
    }
    n = 0;
    for (x = 0; x < 7; x++) {
        if ((found = SzDuel_GetMapUnitID(x, y)) >= 0) {
            if (side == func_00250360_pair(found >> 8, found & 0xff)) {
                n++;
            }
        }
    }
    if (n == 0) {
        if (CheckLandAgreement(SzDuel_GetField(0, y), (struct DotrStateItem *)block) == 1) {
            return 1;
        }
    }
    return 0;
}
