#include "dotr/ee/sound_queue.h"

/* Preserves the original unbounded behavior: a negative result never returns,
 * and the outer loop repeats until the word at +0x40 becomes nonzero.
 */
int SDRPC_Init(void)
{
    int i;
    do {
        if (func_0017e8b0(g_SDValue[0]->unknown_001c, 0x5730601, 0) < 0) {
            for (;;) {
            }
        }
        i = 1000000;
        while (i--) {
        }
    } while (!g_SDValue[0]->unknown_0040);
    func_0017bb60(0);
    return 1;
}
