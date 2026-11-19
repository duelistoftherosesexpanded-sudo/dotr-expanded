#include "dotr/ee/preset_words.h"

/* After SzView_FreeRequest(0, 0), submits a with kind 3 and a + 16 with kind 4,
 * both tagged b. */
void TaTuto_RestoreOriginalView(char *a, unsigned char b)
{
    SzView_FreeRequest(0, 0);
    func_001fa8a0_tag(0, 0, 3, b, 0, a);
    func_001fa8a0_tag(0, 0, 4, b, 0, a + 16);
}
