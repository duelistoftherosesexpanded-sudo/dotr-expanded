#include "dotr/ee/sound_control.h"

/* The two argument words are assembled byte by byte through a pointer to each
 * local, as the original stores them; byte order follows the target.
 */
void SD_SEPlayWithParam6(int a, int b, int c, int d, int e, int g)
{
    int p;
    int q;
    unsigned char *bytes;
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0004 & 4) {
        if (g_SDValue[0]->unknown_0000 & 1) {
            if ((int)SD_iGetSDStatus() & 0x80) {
                bytes = (unsigned char *)&p;
                bytes[0] = a;
                bytes[1] = b;
                bytes[2] = b;
                bytes[3] = e;
                bytes = (unsigned char *)&q;
                bytes[0] = c ? 1 : 0;
                bytes[1] = c & 0x7f;
                bytes[2] = d & 0x7f;
                bytes[3] = g;
                SDRPC_SendCommand(0x5220, p, q);
            }
        }
    }
}
