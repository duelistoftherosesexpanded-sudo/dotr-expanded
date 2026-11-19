#include "dotr/ee/draw_state.h"

/* Builds the packet at `tags` (through its uncached address) that uploads a 16x16-block
 * image from `image` when mode is 0, and then draws it as one textured sprite. */
void setImageTag(unsigned int tags, unsigned int image, int mode, int width, int height)
{
    struct DotrDrawGifPacket packet;
    DotrDrawQuad giftagAD = data_0029b2d0;
    DotrDrawQuad giftagDraw = data_0029b2e0;
    int columns = width >> 4;
    int rows = height >> 4;
    unsigned long *tag;
    int i;
    int j;

    func_001872f8(&packet, (DotrDrawWord128 *)(DotrDrawWord)((tags & 0x0fffffff) | 0x20000000));
    func_00187308(&packet);
    if (mode == 0) {
        func_00187370(&packet, 0, 0, 0);
        func_00187510(&packet, *(DotrDrawWord128 *)&giftagAD);
        func_001875d0(&packet, 80, 0x000c1b0000000000L);
        func_001875d0(&packet, 82, 0x0000001000000010L);
        func_00187528(&packet);
        for (i = 0; i < columns; i++) {
            for (j = 0; j < rows; j++) {
                func_00187370(&packet, 0, 0, 0);
                func_00187510(&packet, *(DotrDrawWord128 *)&giftagAD);
                func_001875d0(&packet, 81, ((unsigned long)(unsigned int)(i * 16) << 32) | ((unsigned long)(unsigned int)(j * 16) << 48));
                func_001875d0(&packet, 83, 0);
                func_00187528(&packet);
                tag = func_001874f8(&packet, 4);
                tag[0] = 64 | (0x08000000L << 32);
                tag[1] = 0;
                func_001873e8(&packet, image & 0x0fffffff, 64, 0, 0, 0);
                image += 1024;
            }
        }
    }
    func_00187480(&packet, 0, 0, 0);
    func_00187510(&packet, *(DotrDrawWord128 *)&giftagDraw);
    func_001875d0(&packet, 63, 0);
    func_001875d0(&packet, 20, 96);
    func_001875d0(&packet, 6, 0x0000000aa8031b00L);
    func_001875d0(&packet, 0, 278);
    func_001875d0(&packet, 3, 0x80008);
    func_001875d0(&packet, 5, 0x79006c00);
    func_001875d0(&packet, 3, (long)(width * 16 + 8) | ((long)(height * 16 + 8) << 16));
    func_001875d0(&packet, 5, 0x87009400L);
    func_00187528(&packet);
    func_00187318(&packet);
}
