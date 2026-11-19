/* Comparison unit for 0x1b76e0; not an original translation unit.
 * lSzDuel_GetFontSize keeps its counters in t2-t9 across the call to
 * lSzDuel_GetFontIndex (0x1b7660), which the compiler does only for a
 * previously defined static leaf.
 * Evidence: docs/modules/other.md.
 */
#include "dotr/ee/small_misc.h"

static int lSzDuel_GetFontIndex(char c);
#include "src/ee/small_misc/func_001b7660.c"
#include "src/ee/small_misc/func_001b76e0.c"
