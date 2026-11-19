#include "dotr/ee/kind_gates.h"

/* Each loop has its own locals: with shared ones the saved registers of the
 * second loop come out permuted. */
int TaLogic002_IsMakeDefense(unsigned char defenseNo)
{
    int owner;
    int i;
    int low;
    int owner2;
    int low2;
    int j;
    int count;
    int id;
    unsigned short pos;

    count = 0;
    if (defenseNo == 1) {
        for (i = 0; i < 6; i++) {
            pos = TaLogic002_GetLinePiece(defenseNo, i);
            id = SzDuel_GetMapUnitID(pos & 0xff, pos >> 8);
            if (id >= 0) {
                low = id & 0xff;
                owner = func_00250360_pair(id >> 8, low);
                if (owner == SzDuel_GetTurnOwner() && low != 0) {
                    count++;
                }
            }
        }
        if (count >= 2) {
            return 1;
        }
        return 0;
    } else {
        for (j = 0; j < 4; j++) {
            pos = TaLogic002_GetLinePiece(defenseNo, j);
            id = SzDuel_GetMapUnitID(pos & 0xff, pos >> 8);
            if (id >= 0) {
                low2 = id & 0xff;
                owner2 = func_00250360_pair(id >> 8, low2);
                if (owner2 == SzDuel_GetTurnOwner() && low2 != 0) {
                    count++;
                }
            }
        }
        if (count > 0) {
            return 1;
        }
        return 0;
    }
}
