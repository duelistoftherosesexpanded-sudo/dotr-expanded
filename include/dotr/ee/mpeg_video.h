#ifndef DOTR_EE_MPEG_VIDEO_H
#define DOTR_EE_MPEG_VIDEO_H

/* The MPEG sample's video stream callback: copies demultiplexed video data from the read ring
 * into the video decoder's input.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */
#ifdef __MWERKS__
typedef unsigned int DotrMvAddr;
#else
#include <stdint.h>
typedef uintptr_t DotrMvAddr;
#endif

/* The stream callback data: the layout of the Sony sample's sceMpegCbDataStr (type, header,
 * data, len, 64-bit pts and dts), as the loads show; type and header are not read here. */
struct DotrMvCbData {
    int type;
    char *header;
    char *data;
    unsigned int len;
    long pts;
    long dts;
};
/* The read ring: a 0x50000-byte data area with its size word at +0x50008 (mpeg_read.h
 * DotrMpegReadBuf, small_misc.h DotrMiscLarge are other views). */
struct DotrMvReadBuf {
    char data[0x50000];
    int unknown_50000[2];
    int size;
};
/* The video decoder object videoDec (0x350680), only passed by address. */
struct DotrMvDecoder;
extern struct DotrMvDecoder data_00350680_mv;
/* "pts buffer overflow\n" at 0x30f5e0. */
extern char data_0030f5e0_mv[];
/* Callees as this function uses them (small_misc.h and sound_file.h declare them with other
 * views): videoDecBeginPut (0x2450d0) returns two destination areas and their sizes;
 * copy2area (0x242f20) copies two source parts into the two areas and returns the bytes
 * copied; videoDecPutTs (0x245160) records the time stamps and returns 0 on failure
 * (small_misc.h declares it void); ErrMessage (0x242cf0) takes the message; videoDecEndPut
 * (0x2450e0) takes the byte count. Assumed: the int results and pointer types. */
extern int func_002450d0_mv(struct DotrMvDecoder *vd, char **pd0, int *d0, char **pd1, int *d1);
extern int func_00242f20_mv(char *pd0, int d0, char *pd1, int d1, char *ps0, int s0, char *ps1, int s1);
extern int func_00245160_mv(struct DotrMvDecoder *vd, long pts, long dts, char *pd0, int len);
extern void func_00242cf0_mv(char *text);
extern int func_002450e0_mv(struct DotrMvDecoder *vd, int len);

/* 0x242d10 videoCallback (name from the Japanese build; follows the Sony ezmpeg sample):
 * the data part s0 = min(len, ring end - data) and s1 = len - s0 (the rest from the ring
 * start); the destination areas from videoDecBeginPut are made uncached
 * ((p & 0x0fffffff) | 0x20000000) for copy2area; when it copied more than 0 bytes,
 * videoDecPutTs(pts, dts, first area, count), with ErrMessage when that returns 0;
 * videoDecEndPut(count); returns 1 when bytes were copied, else 0. MATCHING DEVICES: the
 * third parameter as void * cast in the first local (a typed parameter changes registers);
 * locals declared in the order pd0, pd1, d0, d1, ps0, s0, diff, s1, len and assigned
 * separately; s0 and diff unsigned. mpeg_read.h declares this function with a typed third
 * parameter for its caller initAll. */
int videoCallback(void *mp, struct DotrMvCbData *cbstr, void *data);

#endif
