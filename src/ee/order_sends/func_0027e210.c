#include "dotr/ee/order_sends.h"

/* Sets the order's code: 0x05xx when TaLogic_IsAttackedNow answers 1 for the order's position
 * or TaLogic_IsAttackedNow2 answers non-zero, 0x06xx (with the goal location
 * (destinationRow << 8 | destinationCol) placed in the upper
 * half of the word at +8) when TaLogic_IsAttackedNow2 answers 0; the low byte is 0x0a when
 * SzDuel_InqHandPhase(row) is 1, else 0xff.
 * The repeated arms are the original's shape. */
void TaLogic_CommandStdLDUnit(struct DotrSendOrder *order, int destinationCol, int destinationRow, int c)
{
    if (TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xff, c) == 1) {
        if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
            order->code = 0x50a;
        } else {
            order->code = 0x5ff;
        }
    } else if (TaLogic_IsAttackedNow2(order->position >> 8, order->position & 0xff, destinationCol, destinationRow, c) == 0) {
        if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
            order->code = 0x60a;
            order->flags.word = order->flags.half | (((destinationRow << 8) | destinationCol) << 16);
        } else {
            order->code = 0x6ff;
            order->flags.word = order->flags.half | (((destinationRow << 8) | destinationCol) << 16);
        }
    } else {
        if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
            order->code = 0x50a;
        } else {
            order->code = 0x5ff;
        }
    }
}
