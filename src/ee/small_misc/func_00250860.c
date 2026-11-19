#include "dotr/ee/small_misc.h"

/* The larger value is selected by two returns and the smaller by a `>=`
 * conditional; each spelling alone reproduces its observed selection. */
int GetCap(int limit, int value)
{
    switch (limit) {
    case 0:
    case 1:
        if (limit > value) {
            return limit;
        } else {
            return value;
        }
    case 7:
    case 6:
    case 9999:
    case 500:
    case 1500:
        return value >= limit ? limit : value;
    }
    return 0;
}
