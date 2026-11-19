/* Host semantics only: this test does not emulate R5900 or boot PCSX2. */
#include "dotr/ee/menu.h"
#include <limits.h>
#include <string.h>

struct DotrMenuStatePrefix dotr_ee_state_00317250;

int main(void)
{
    static const int invalid[] = {INT_MIN, -1, 37, INT_MAX};
    struct DotrMenuStatePrefix before;
    unsigned int n;
    int i, j;
    memset(&dotr_ee_state_00317250, 0x5a, sizeof(dotr_ee_state_00317250));
    for (i = 0; i < 37; i++)
        dotr_ee_state_00317250.menu_nums[i] = (i - 18) * 1009;
    before = dotr_ee_state_00317250;
    for (n = 0; n < sizeof(invalid) / sizeof(invalid[0]); n++) {
        if (SzGame_GetMenuNum(invalid[n]) != -1 || SzGame_SetMenuNum(invalid[n], 12345) != 0)
            return 1;
        if (memcmp(&before, &dotr_ee_state_00317250, sizeof(before)) != 0)
            return 2;
    }
    for (i = 0; i < 37; i++) {
        if (SzGame_GetMenuNum(i) != before.menu_nums[i] || SzGame_SetMenuNum(i, INT_MIN + i) != 1)
            return 3;
        for (j = 0; j < 37; j++) {
            int expected = j <= i ? INT_MIN + j : before.menu_nums[j];
            if (dotr_ee_state_00317250.menu_nums[j] != expected)
                return 4;
        }
        if (SzGame_GetMenuNum(i) != INT_MIN + i)
            return 5;
    }
    if (memcmp(before.unknown_000, dotr_ee_state_00317250.unknown_000, sizeof(before.unknown_000)) != 0)
        return 6;
    return 0;
}
