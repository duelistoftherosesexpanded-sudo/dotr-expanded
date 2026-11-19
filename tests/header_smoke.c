#include "dotr/types.h"
#include "dotr/ee/menu.h"
#include <stddef.h>

_Static_assert(sizeof(u8) == 1, "u8 width");
_Static_assert(sizeof(u16) == 2, "u16 width");
_Static_assert(sizeof(u32) == 4, "u32 width");
_Static_assert(sizeof(u64) == 8, "u64 width");
_Static_assert(offsetof(struct DotrMenuStatePrefix, menu_nums) == 0x138, "verified menu member offset");
_Static_assert(sizeof(((struct DotrMenuStatePrefix *)0)->menu_nums) == 0x94, "verified menu extent");

int main(void)
{
    return 0;
}
