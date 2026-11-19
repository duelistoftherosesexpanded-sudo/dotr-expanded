#include "dotr/ee/small_misc.h"

/* compMode 1 forwards (searchType, b, c) and 2 forwards (searchType, d, e) to
 * Do_Match_AtkEfcObj; any other compMode returns 0. */
int Match_AtkEfcObj(int compMode, int searchType, int b, int c, int d, int e)
{
    switch (compMode) {
    case 1:
        return Do_Match_AtkEfcObj(searchType, b, c);
    case 2:
        return Do_Match_AtkEfcObj(searchType, d, e);
    }
    return 0;
}
