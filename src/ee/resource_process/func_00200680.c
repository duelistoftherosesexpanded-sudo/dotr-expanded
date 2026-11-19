#include "dotr/ee/resource_process.h"

/* Record callback installed by RhFile_ReadAndCallbackByFileName (0x200510). The original
 * tests the stored word through its absolute address and loads it again through the state
 * pointer.
 */
void lRhFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    (void)completed;
    if (dotr_ee_resource_submit_state_0031c2f0.unknown_6680) {
        ((void (*)(int))(DotrResourceSubmitWord)state->unknown_6680)(record->unknown_01c);
    }
}
