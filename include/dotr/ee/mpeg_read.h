#ifndef DOTR_EE_MPEG_READ_H
#define DOTR_EE_MPEG_READ_H

/* readMpeg (0x242600), the movie read/demultiplex loop: calls the read buffer, stream file,
 * video and audio decoder and pad routines defined in other groups.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Views for readMpeg; the names of the three globals in the Japanese build are isWithAudio
 * (0x29b2f0), isWithInter (0x3503b0) and controller_val (0x3505f0). Assumed from this
 * function alone: the decoder's word at +8 is a counter compared with 10, the read buffer
 * keeps its size word at +0x50008 after a 0x50000-byte data area (small_misc.h declares the
 * same object as DotrMiscLarge), the file has its size at +4 and a start sector at +8
 * (sound_file.h: DotrSoundReader). The objects at 0x350600, 0x350620 and 0x350680 are only
 * passed by address. 0x18dee8 lies in the library block (out of scope); its five
 * arguments are taken from the argument registers. The other callees are defined in
 * small_misc, sound_file, ring_pumps, side_table and small_state with their own parameter
 * types; here they take these views. */
struct DotrMpegDecoder {
    int unknown_00[2];
    int unknown_08;
};
struct DotrMpegReadBuf {
    char data[0x50000];
    int unknown_50000[2];
    int size;
};
struct DotrMpegFile {
    int unknown_00;
    int size;
    int start;
};
struct DotrMpegAudio;
struct DotrMpegVideoOut;
extern int isWithAudio;
extern int isWithInter;
extern unsigned int controller_val;
extern struct DotrMpegVideoOut voBuf;
extern struct DotrMpegAudio audioDec;
extern struct DotrMpegDecoder videoDec;
extern int SzPad_Status(void);
extern int SzPad_VSync(void);
extern int SzPad_Act(void);
extern int SzPad_Check(int unknown, int zero, int mask);
extern void endDisplay(void);
extern void startDisplay(int a);
extern void switchThread(void);
extern int isAudioOK(void);
extern int proceedAudio(void);
extern void audioDecPause(struct DotrMpegAudio *p);
extern void audioDecStart(struct DotrMpegAudio *p);
extern void audioDecReset(struct DotrMpegAudio *p);
extern void videoDecAbort(struct DotrMpegDecoder *p);
extern int videoDecGetState(struct DotrMpegDecoder *p);
extern int videoDecFlush(struct DotrMpegDecoder *p);
extern int videoDecIsFlushed(struct DotrMpegDecoder *p);
extern int voBufIsFull(struct DotrMpegVideoOut *p);
extern int readBufBeginPut(struct DotrMpegReadBuf *p, char **out);
extern int readBufEndPut(struct DotrMpegReadBuf *p, int count);
extern int readBufBeginGet(struct DotrMpegReadBuf *p, char **out);
extern int readBufEndGet(struct DotrMpegReadBuf *p, int count);
extern int strFileRead(struct DotrMpegFile *file, char *buffer, int byte_count, int position);
extern int func_0018dee8(struct DotrMpegDecoder *vd, char *data, int size, struct DotrMpegReadBuf *ring, int ring_size);
int readMpeg(struct DotrMpegDecoder *vd, struct DotrMpegReadBuf *rb, struct DotrMpegFile *file);

/* initAll (0x242950), the set-up routine of the Sony MPEG streaming sample this file follows
 * (local and global names from the Japanese build's symbols; the sample's names for the
 * library calls are given in comments, unverified: the callees below 0x1a6c80 lie in the
 * library block, out of scope). Assumed from this function alone: the buffer
 * words (mpegWork, viBufData, viBufTag, timeStamp, audioBuff, voBufData, voBufTag) are passed
 * as loaded and declared as int words here, voBufData with the uncached-segment bits
 * ((x & 0x0fffffff) | 0x20000000); readBuf is a pointer word (small_misc.h reaches the same word
 * as readBuf[0]); the two handler ids go to 0x350734 (vertical blank) and 0x350730 (image end),
 * the array small_misc.h declares. The thread parameter block is the 48-byte kernel layout as
 * far as the stores show (+4 entry, +8 stack, +12 size, +16 gp, +20 priority, +32 option); the
 * other members are padding names. The callbacks are passed through a cast to the callback
 * type, as in the sample: without the cast the compiler loads the arguments in another order.
 * 0x1000e000 and 0x1000e010 are the DMA controller's control and status words; host variables
 * stand in for them. Parameter types of the callees defined in other groups are views. */
struct DotrMpegThreadParam {
    int status;
    void (*entry)(void *);
    void *stack;
    int stackSize;
    void *gpReg;
    int initPriority;
    int currentPriority;
    unsigned attr;
    unsigned option;
    int waitType;
    int waitId;
    int wakeupCount;
};
struct DotrMpegCbData;
typedef int (*DotrMpegCallback)(struct DotrMpegDecoder *mp, struct DotrMpegCbData *cbdata, void *anyData);
#ifdef __MWERKS__
#define DOTR_MPEG_D_CTRL (*(volatile unsigned *)0x1000e000)
#define DOTR_MPEG_D_STAT (*(volatile unsigned *)0x1000e010)
#else
extern volatile unsigned dotr_mpeg_d_ctrl_1000e000;
extern volatile unsigned dotr_mpeg_d_stat_1000e010;
#define DOTR_MPEG_D_CTRL dotr_mpeg_d_ctrl_1000e000
#define DOTR_MPEG_D_STAT dotr_mpeg_d_stat_1000e010
#endif
extern struct DotrMpegFile infile;
extern struct DotrMpegReadBuf *readBuf;
extern int mpegWork;
extern int viBufData;
extern int viBufTag;
extern int timeStamp;
extern int audioBuff;
extern int voBufData;
extern int voBufTag;
extern void *defStack;
extern void *videoDecStack;
extern int defaultTh;
extern int videoDecTh;
extern int data_00350730[4];
extern char data_00317ef0[];
extern char data_0030f39a[];
extern void readBufCreate(struct DotrMpegReadBuf *p);
extern void func_0018ea68(void); /* sceMpegInit */
extern int videoDecCreate(struct DotrMpegDecoder *p, int a, int b, int c, int d, int e, int f, int g);
extern void func_001906d0(void); /* sceSdRemoteInit */
extern void func_00190810(int a, int b, int c); /* sceSdRemote; three arguments loaded here */
extern int audioDecCreate(struct DotrMpegAudio *p, int a, int b, int size);
extern int videoDecSetStream(struct DotrMpegDecoder *p, int kind, int channel, DotrMpegCallback cb, struct DotrMpegReadBuf *data);
extern void voBufCreate(struct DotrMpegVideoOut *p, int data, int tag, int capacity);
extern int func_0017b6e0(struct DotrMpegThreadParam *p); /* CreateThread */
extern int func_0017b700(int id, void *arg); /* StartThread */
extern int strFileOpen(struct DotrMpegFile *file, char *name);
extern int func_00199b50(const char *format, ...);
extern int func_0017b5c0(int cause, int (*handler)(int), int next); /* AddIntcHandler */
extern int func_0017c428(int cause); /* EnableIntc */
extern int func_0017b5f0(int cause, int (*handler)(int), int next); /* AddDmacHandler */
extern int func_0017c4f8(int cause); /* EnableDmac */
extern int videoCallback(struct DotrMpegDecoder *mp, struct DotrMpegCbData *str, struct DotrMpegReadBuf *data);
extern int pcmCallback(struct DotrMpegDecoder *mp, struct DotrMpegCbData *str, struct DotrMpegReadBuf *data);
extern void defMain(void *arg);
extern void videoDecMain(void *arg);
extern int vblankHandler(int cause);
extern int handler_endimage(int cause);
void initAll(char *bsfilename);

#endif
