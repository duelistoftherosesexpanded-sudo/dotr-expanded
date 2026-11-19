#include "dotr/ee/draw_state.h"

/* Builds a one-register tag and default environment in the block at 0x350410, submits it,
 * then sends a six-quadword packet that draws one sprite in colour (r, g, b). */
void clearGsMem(int r, int g, int b, int disp_width, int disp_height)
{
    DotrDrawQuad giftag_clear = data_0029b2c0;
    struct DotrDrawGifPacket packet;
    long packetBase[12];
    DotrDrawWord dmaGif;

    dmaGif = func_00186120(2);
    *(DotrDrawWord128 *)&data_00350410.giftag = 0;
    data_00350410.giftag.bits.NLOOP = 8;
    data_00350410.giftag.bits.EOP = 1;
    data_00350410.giftag.bits.NREG = 1;
    data_00350410.giftag.bits.REGS0 = 14;
    func_00184e10(&data_00350410.draw[0], 0, disp_width, ((((disp_height >> 1) + 31) >> 5) + ((disp_height + 31) >> 5)) << 6, 0, 0);
    *(long *)&data_00350410.draw[4] = 0;
    func_0017bb60(0);
    func_00185578(0, 0);
    func_00185100(&data_00350410);
    func_001872f8(&packet, (DotrDrawWord128 *)packetBase);
    func_00187308(&packet);
    func_00187480(&packet, 0, 0, 0);
    func_00187510(&packet, *(DotrDrawWord128 *)&giftag_clear);
    func_001875d0(&packet, 0, 6);
    func_001875d0(&packet, 1, (long)r | ((long)g << 8) | ((long)b << 16));
    func_001875d0(&packet, 5, 0);
    func_001875d0(&packet, 5, 0xb4002d00L);
    func_00187528(&packet);
    func_00187318(&packet);
    func_0017bb60(0);
    func_00185578(0, 0);
    func_00186470(dmaGif, packet.base);
    func_00185578(0, 0);
}
