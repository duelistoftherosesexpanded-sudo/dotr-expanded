#include "dotr/ee/kind_gates.h"

void TaLogic_CommandDefenseUnit(struct DotrPositionOrder *order, unsigned char lineType, unsigned char hasTarget)
{
    int maxIndex;
    int i;
    int lineLoc;
    int id;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    func_0024de70_pair(order->position >> 8, order->position & 0xff);
    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    func_001df540_word(order->position >> 8, order->position & 0xff);
    order->kind = ((hasTarget == 1 ? 3 : 6) << 8) | 0xff;
    maxIndex = lineType == 1 ? 6 : 4;
    for (i = 0; i < maxIndex; i++) {
        lineLoc = TaLogic002_GetLinePiece(lineType, i) & 0xffff;
        id = SzDuel_GetMapUnitID(lineLoc & 0xff, lineLoc >> 8);
        if (id == order->position || id < 0) {
            order->flags.word = (lineLoc << 16) | order->flags.half;
            return;
        }
    }
}
