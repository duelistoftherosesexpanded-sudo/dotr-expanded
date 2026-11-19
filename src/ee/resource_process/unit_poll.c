/* Comparison unit for 0x200360, 0x2003e0, 0x200510 and 0x2006c0; not an original translation unit.
 * 0x2003e0 and 0x2006c0 contain 0x1ff420 inline and keep a pointer in t2 across its call
 * to lSzFile_NextRequest (see src/ee/resource_submit/unit_wait.c); 0x200360, 0x200510 and
 * 0x2006c0 contain 0x1fff90 inline. Evidence: docs/modules/render-files.md.
 */
#pragma auto_inline on
#include "dotr/ee/resource_process.h"

static int lSzFile_NextRequest(void);
#include "src/ee/resource_helpers/func_001ff420.c"
#include "src/ee/resource_submit/func_001ff9c0.c"
#include "src/ee/resource_helpers/func_001fff90.c"
#include "src/ee/resource_process/func_00200360.c"
#include "src/ee/resource_process/func_002003e0.c"
#include "src/ee/resource_process/func_00200510.c"
#include "src/ee/resource_process/func_002006c0.c"
