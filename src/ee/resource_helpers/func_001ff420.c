#include "dotr/ee/resource_helpers.h"

extern int lSzFile_NextRequest(void);
extern int lSzFile_DoRequestMain(int cached_result);

int SzFile_DoRequestS(void)
{
    struct DotrResourceCachePrefix *state = &gSzFile_fl_prefix;
    if (!data_00322980[0].bit_1) {
        int result = lSzFile_NextRequest();
        state->cached_result = result;
        if (result < 0) {
            return 0;
        }
    }
    return lSzFile_DoRequestMain(state->cached_result);
}
