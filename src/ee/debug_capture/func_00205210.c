#include "dotr/ee/debug_capture.h"

/* SzDebug_OutputFrameBuffer: finishes pending file requests, copies the frame to
 * 0x4000000 and writes it as a 640x448 24-bit bitmap under the first unused numbered
 * name, bottom row first, each 15-bit pixel widened to three bytes. */
void SzDebug_OutputFrameBuffer(void)
{
    unsigned char buf[1920];
    struct DotrDebugInfoHeader ih;
    struct DotrDebugFileHeader fh;
    int fd;
    int size;
    int y;
    int x;
    unsigned short *src;
    unsigned char *dst;

    while (SzFile_InqRequest(-1) != 0) {
        SzFile_DoRequestS();
    }
    RhBase_CopyFrameBuffer(0x4000000);
    fh.type = 0x4d42;
    /* The low halfword is stored from the whole constant (lui/ori, then sh). */
    size = 0xd2036;
    fh.size_lo = size;
    fh.size_hi = size >> 16;
    fh.reserved_1 = 0;
    fh.reserved_2 = 0;
    fh.offset_lo = 54;
    ih.size = 40;
    ih.width = 640;
    ih.height = 448;
    ih.planes = 1;
    ih.bits = 24;
    ih.compression = 0;
    ih.image_size = 0xd2000;
    ih.x_scale = 1;
    ih.y_scale = 1;
    ih.used = 0;
    fh.offset_hi = 0;
    ih.important = 0;
    for (;;) {
        func_0019b648((char *)buf, data_002d0806, SzFile_GetEnv(data_002d07fa), data_00325cb0);
        fd = func_0017fa98((char *)buf, 1);
        if (fd < 0) {
            break;
        }
        func_0017fd20(fd);
        data_00325cb0++;
    }
    data_00325cb0++;
    fd = func_0017fa98((char *)buf, 0x202);
    if (fd >= 0) {
        func_00180348(fd, &fh, 14);
        func_00180348(fd, &ih, 40);
        for (y = 447; y >= 0; y--) {
            dst = buf;
            for (x = 0, src = DOTR_DEBUG_FRAME + y * 640; x < 640; x++) {
                dst[0] = ((*src >> 7) & 0xf8) | ((*src >> 12) & 7);
                dst[1] = ((*src >> 2) & 0xf8) | ((*src >> 7) & 7);
                dst[2] = ((*src << 3) & 0xf8) | ((*src >> 2) & 7);
                src++;
                dst += 3;
            }
            func_00180348(fd, buf, 1920);
        }
        func_0017fd20(fd);
    }
}
