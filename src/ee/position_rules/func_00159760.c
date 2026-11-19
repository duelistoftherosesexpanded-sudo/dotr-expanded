#include "dotr/ee/position_rules.h"

void TaLogic018_CommandLDUnit(struct DotrPositionOrder *order)
{
    unsigned char item[32];
    struct DotrPositionBlock block;
    int side;
    int here;
    int r;

    side = order->position >> 8;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xff, 0) == 1) {
        if (SzDuel_InqHandPhase(side) == 1) {
            order->kind = 0x50a;
        } else {
            order->kind = 0x5ff;
        }
        return;
    }
    r = func_0015a5f0_block(side, &block);
    if (r < 0) {
        if (SzDuel_InqHandPhase(side) == 1) {
            order->kind = 0x60a;
        } else {
            order->kind = 0x6ff;
        }
        order->flags.word = order->flags.half | (TaLogic018_GetVirusMove(order->position >> 8, order->position & 0xff) << 16);
        return;
    }
    if (r == 0) {
        func_0015ab10_block(side, item, &block);
    }
    if (block.unknown_3e >> 5 == 0) {
        if (func_001e02f0_block(&block, 0) > 1500) {
            if (SzDuel_InqHandPhase(side) == 1) {
                order->kind = 0x60a;
            } else {
                order->kind = 0x6ff;
            }
            order->flags.word = order->flags.half | (TaLogic018_GetSummonPoint(side, here & 0xff, here >> 8) << 16);
        }
    }
    if (SzDuel_InqHandPhase(side) == 1) {
        order->kind = 0x60a;
    } else {
        order->kind = 0x6ff;
    }
    order->flags.word = order->flags.half | (TaLogic018_GetVirusMove(order->position >> 8, order->position & 0xff) << 16);
}
