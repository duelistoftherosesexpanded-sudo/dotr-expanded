#include "dotr/ee/mpeg_read.h"

/* The movie loop of the Sony MPEG sample this follows (local names as there). While more
 * than four bytes remain to demultiplex and the decoder has not ended: polls the pad and, if
 * interruption is allowed, a newly pressed button of mask 0x8e0 after the decoder counter
 * passed 10 ends the display, pauses audio, aborts the decoder and returns -1. Otherwise
 * reads 0x10000 bytes into the read buffer when it has room, feeds the buffered data to the
 * demultiplexer, pumps audio and starts display and audio once the output buffer is full
 * and audio is ready. Afterwards flushes the decoder, waits for it, ends the display, resets
 * audio and returns 1. */
int readMpeg(struct DotrMpegDecoder *vd, struct DotrMpegReadBuf *rb, struct DotrMpegFile *file)
{
    char *put_ptr;
    char *get_ptr;
    int putsize;
    int getsize;
    int readrest = file->size;
    int sector = 0;
    int writerest = file->size;
    int count;
    int proceed;
    int isStarted = 0;
    unsigned int button_old = 0;
    unsigned int pushed;
    int isPaused = 0;
    int keep;

    while (isPaused || (writerest > 4 && videoDecGetState(vd) != 3)) {
        /* MATCHING DEVICE: the pause flag is never set in this build, yet the target keeps
         * it in a register and tests it; saving and restoring it through `keep` stops the
         * compiler folding it to zero (the Sony sample toggles it on a button). */
        keep = isPaused;
        SzPad_Status();
        SzPad_VSync();
        SzPad_Act();
        controller_val = SzPad_Check(0, 1, 0xffff) & 0xffff;
        pushed = controller_val & (button_old ^ controller_val) & 0x8e0;
        button_old = controller_val;
        if (isWithInter && pushed && vd->unknown_08 > 10 && !isPaused) {
            endDisplay();
            if (isWithAudio) {
                audioDecPause(&audioDec);
            }
            videoDecAbort(&videoDec);
            return -1;
        }
        putsize = readBufBeginPut(rb, &put_ptr);
        if (readrest > 0 && putsize >= 0x10000) {
            count = strFileRead(file, put_ptr, 0x10000, file->start + sector);
            readBufEndPut(rb, count);
            sector += count >> 11;
            readrest -= count;
        }
        switchThread();
        getsize = readBufBeginGet(rb, &get_ptr);
        if (getsize > 0) {
            proceed = func_0018dee8(vd, get_ptr, getsize, rb, rb->size);
            readBufEndGet(rb, proceed);
            writerest -= proceed;
        }
        isPaused = keep;
        proceedAudio();
        if (!isStarted && voBufIsFull(&voBuf) && isAudioOK()) {
            startDisplay(1);
            if (isWithAudio) {
                audioDecStart(&audioDec);
            }
            isStarted = 1;
        }
    }
    while (!videoDecFlush(vd)) {
        switchThread();
    }
    while (!videoDecIsFlushed(vd) && videoDecGetState(vd) != 3) {
        switchThread();
    }
    endDisplay();
    if (isWithAudio) {
        audioDecReset(&audioDec);
    }
    return 1;
}
