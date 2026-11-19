#ifndef DOTR_EE_DEBUG_CAPTURE_H
#define DOTR_EE_DEBUG_CAPTURE_H

/* SzDebug_OutputFrameBuffer (0x205210), the debug screenshot writer; its file callees are library
 * functions.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* 0x205210 SzDebug_OutputFrameBuffer (original name). The frame is read at the fixed
 * address 0x4000000 after RhBase_CopyFrameBuffer(0x4000000); the host build reads a
 * fixture instead. Assumed: the two headers as the stores show them (seven halfwords;
 * three words, two halfwords, six words), the library signatures (open, close, write and
 * sprintf by use), the counter at 0x325cb0 (`nSaveCount$392` in the Japanese build) as int. */
#ifdef __MWERKS__
#define DOTR_DEBUG_FRAME ((unsigned short *)0x4000000)
#else
extern unsigned short dotr_debug_host_frame[640 * 448];
#define DOTR_DEBUG_FRAME dotr_debug_host_frame
#endif
struct DotrDebugFileHeader {
    unsigned short type, size_lo, size_hi, reserved_1, reserved_2, offset_lo, offset_hi;
};
struct DotrDebugInfoHeader {
    int size, width, height;
    short planes, bits;
    int compression, image_size, x_scale, y_scale, used, important;
};
extern int data_00325cb0;
extern char data_002d07fa[];
extern char data_002d0806[];
extern int SzFile_InqRequest(int a);
extern int SzFile_DoRequestS(void);
extern void RhBase_CopyFrameBuffer(unsigned long x);
extern char *SzFile_GetEnv(const char *label);
extern int func_0019b648(char *destination, const char *format, ...);
extern int func_0017fa98(const char *path, int flags);
extern int func_0017fd20(int descriptor);
extern int func_00180348(int descriptor, void *data, int size);
void SzDebug_OutputFrameBuffer(void);

#endif
