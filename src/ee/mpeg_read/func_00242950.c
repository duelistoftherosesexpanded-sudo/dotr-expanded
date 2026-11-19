#include "dotr/ee/mpeg_read.h"

/* Set-up of the Sony MPEG streaming sample this follows (local names as there). Enables the
 * DMA controller bits and writes its status word, creates the read buffer, the video decoder,
 * the audio decoder and the output buffer, registers the video stream callback and, when the
 * movie has audio, the PCM one, creates and starts the default and the decoder threads, opens
 * the stream file (printing a message and retrying until it opens) and installs and enables
 * the vertical-blank and image-end handlers. */
void initAll(char *bsfilename)
{
    struct DotrMpegThreadParam th_param;

    DOTR_MPEG_D_CTRL |= 3;
    DOTR_MPEG_D_STAT = 4;
    readBufCreate(readBuf);
    func_0018ea68();
    videoDecCreate(&videoDec, mpegWork, 0x1c9168, viBufData, viBufTag, 256, timeStamp, 512);
    func_001906d0();
    func_00190810(1, 0x8000, 0);
    audioDecCreate(&audioDec, audioBuff, 0xc000, 24576);
    videoDecSetStream(&videoDec, 0, 0, (DotrMpegCallback)videoCallback, readBuf);
    if (isWithAudio) {
        videoDecSetStream(&videoDec, 2, 0, (DotrMpegCallback)pcmCallback, readBuf);
    }
    voBufCreate(&voBuf, (voBufData & 0x0fffffff) | 0x20000000, voBufTag, 2);
    th_param.entry = defMain;
    th_param.stack = defStack;
    th_param.stackSize = 2048;
    th_param.initPriority = 32;
    th_param.gpReg = data_00317ef0;
    th_param.option = 0;
    defaultTh = func_0017b6e0(&th_param);
    func_0017b700(defaultTh, 0);
    th_param.entry = videoDecMain;
    th_param.stack = videoDecStack;
    th_param.stackSize = 16384;
    th_param.initPriority = 32;
    th_param.gpReg = data_00317ef0;
    th_param.option = 0;
    videoDecTh = func_0017b6e0(&th_param);
    func_0017b700(videoDecTh, &videoDec);
    while (!strFileOpen(&infile, bsfilename)) {
        func_00199b50(data_0030f39a, bsfilename);
    }
    data_00350730[1] = func_0017b5c0(2, vblankHandler, 0);
    func_0017c428(2);
    data_00350730[0] = func_0017b5f0(2, handler_endimage, 0);
    func_0017c4f8(2);
}
