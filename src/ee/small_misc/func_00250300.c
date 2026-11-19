#include "dotr/ee/small_misc.h"

/* The compound subtraction keeps the narrowed value in the argument register. */
int Ta_CalcCtrlRate(unsigned short value, int amount, int direction)
{
    if (direction > 0) {
        value = value + amount;
        if (value > 128) {
            value = 128;
        }
        return value;
    } else {
        if (amount < value) {
            value -= amount;
        } else {
            value = 0;
        }
    }
    return value;
}
