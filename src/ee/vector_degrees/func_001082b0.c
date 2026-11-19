#include "dotr/ee/vector_degrees.h"

int tanD_360_(long x, long y)
{
    long ax;
    long ay;

    if (x == 0 && y == 0) {
        return 0;
    }
    ax = func_00195070(x);
    ay = func_00195070(y);
    if (ax + ay == 0) {
        return 0;
    }
    if (y >= 0) {
        if (x > 0) {
            if (ax == ay) {
                return 45;
            }
            if (ay < ax) {
                return ay * 45 / ax;
            }
            return 90 - ax * 45 / ay;
        } else {
            if (ax == ay) {
                return 135;
            }
            if (ay < ax) {
                return 180 - ay * 45 / ax;
            }
            return ax * 45 / ay + 90;
        }
    } else {
        if (x > 0) {
            if (ax == ay) {
                return 315;
            }
            if (ay < ax) {
                return 360 - ay * 45 / ax;
            }
            return ax * 45 / ay + 270;
        } else {
            if (ax == ay) {
                return 225;
            }
            if (ay < ax) {
                return ay * 45 / ax + 180;
            }
            return 270 - ax * 45 / ay;
        }
    }
}
