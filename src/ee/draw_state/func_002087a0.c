#include "dotr/ee/draw_state.h"

/* Packet builder: when flag 0x20 of the second halfword is set, emits the
 * trailing record located after the p[0] eight-byte entries. */
int ExAssignFrameBuffer(unsigned short *p, int b)
{
    unsigned short n = p[0];
    unsigned char *e = (unsigned char *)(p + 8);
    if (!(p[1] & 0x20)) return 0;
    ExFrameBuffer2Texture_Pipe(0, 0, b, (DotrDrawWord)(e + n * 8 + 8), (DotrDrawWord)(e + n * 8 + 16));
    return 1;
}
