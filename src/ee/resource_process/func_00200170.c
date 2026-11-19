#include "dotr/ee/resource_process.h"

/* C with one hardware inline-assembly block: the original ends with `sync` and
 * `ei`, which have no C spelling and no intrinsic in the pinned compiler. Hosts
 * omit them. See docs/DECOMPILATION.md, "What a source may contain".
 */
void lRhFile_CdCallback(int reason)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    if (reason == 1 && state->flags.bit_1) {
        if (func_00191ef8() == 0) {
            func_0017bb90(0);
            lSzFile_TermRequest(state->cached_result, 1);
        } else {
            state->flags.bit_1 = 0;
        }
    }
#if defined(__MWERKS__)
    __asm__ volatile("sync.l; ei");
#endif
}
