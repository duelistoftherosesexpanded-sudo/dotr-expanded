/* Comparison unit for 0x1ffd30 and 0x1ffe00; not an original translation unit.
 * Both contain 0x1ff420 inline and keep a pointer in t2 across its call to
 * 0x1ff9c0, which the compiler does only for a previously defined static leaf.
 * Evidence: docs/modules/engine.md.
 */
#pragma auto_inline on
#include "dotr/ee/resource_submit.h"

static int lSzFile_NextRequest(void);
#include "src/ee/resource_helpers/func_001ff420.c"
#include "src/ee/resource_submit/func_001ff9c0.c"
#include "src/ee/resource_submit/func_001ffd30.c"
#include "src/ee/resource_submit/func_001ffe00.c"
