#ifndef DOTR_EE_SOUND_FILE_H
#define DOTR_EE_SOUND_FILE_H

#include "dotr/ee/resource_process.h"

/* Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md.
 * Target offsets assume 32-bit pointers; hosts with wider pointers differ.
 */
struct DotrSoundFileRead {
    int busy;
    unsigned char unknown_004[0x44]; /* includes the name text viewed as gSdfs_sa */
    char *destination;
    int skip;
    int chunk;
    int offset;
    int remaining;
};
extern struct DotrSoundFileRead s_iUsingNow;
/* Distinct view of the name text at +8 of the storage above. */
extern char gSdfs_sa[];

extern char data_002b90c0[];
extern char data_002b90da[];
extern char data_002b9102[];
extern char data_002b9112[];
extern char data_002b9127[];
extern char data_002b9135[];
extern char data_002b9141[];
extern char data_002b9168[];

extern void *func_00198790(void *destination, const void *source, int byte_count);
extern int func_00199b50(const char *format, ...);

/* Copies two sources into two destinations in order; 0 if they cannot hold both. */
int copy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size);

int cpy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size);

/* An address held in a word: 32 bits on the target. */
#ifdef __MWERKS__
typedef int DotrSoundAddress;
#else
#include <stdint.h>
typedef intptr_t DotrSoundAddress;
#endif

/* Assumed ring view: mode, 40 data bytes, then offsets/base/size words. */
struct DotrSoundRing {
    int mode;
    unsigned char unknown_004[40];
    int unknown_2c;
    int unknown_30;
    int unknown_34;
    int unknown_38;
    int unknown_3c;
};
void audioDecBeginPut(struct DotrSoundRing *ring, int *start1, int *count1, int *start2, int *count2);

int SDFS_IsUsingNow(void);
int SDFS_ReadToEE(char *name, char *destination, int offset, int size);
int SDFS_ReadToEEBlock(char *name, char *destination, int offset, int size);
void lSDFS_FinishCallback(int unused);
/* Same declarations as small_misc.h (library callees). */
extern void func_0017b900(int unknown);
extern void func_0017b920(int unknown);
extern void func_00190810(int a, int b, int c, int d);
/* small_misc.h declares this callee with the same type. */
void changeMasterVolume(int a);
/* Words at +0x14 and +0x40 and the 64-bit total at +0x48 are observed. */
struct DotrSoundStream {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
    int unknown_18;
    int unknown_1c[9];
    int unknown_40;
    int unknown_44;
    long unknown_48;
    int unknown_50;
    int unknown_54;
    int unknown_58;
    int unknown_5c;
};
/* small_misc.h keeps a one-argument int view of this function for its caller. */
void viBufEndPut(struct DotrSoundStream *p, int n);
/* 32-byte local record of viBufCreate; only the words at +4 and +8 are set. */
struct DotrSoundSema {
    int unknown_0;
    int unknown_4;
    int unknown_8;
    int unknown_c[5];
};
extern int func_0017b8e0(struct DotrSoundSema *sema);
/* 24-byte records at the address held in +0x50 (count at +0x54): two 64-bit words and two
 * 32-bit words are observed. */
struct DotrSoundSlot {
    long unknown_00;
    long unknown_08;
    int unknown_10;
    int unknown_14;
};
/* The stream keeps addresses in int words and writes DMA channel 4 and the DMA enable
 * registers at fixed addresses. The host build maps the low 16 bits of an address into a
 * test-supplied byte array and the registers (0x1000b400-0x1000f590, by (addr & 0xffff) >> 4)
 * into a test-supplied word array; neither is a claim about an original object. */
#ifdef __MWERKS__
#define DOTR_SOUND_PTR(type, addr) ((type *)(addr))
#define DOTR_SOUND_REG(addr) (*(volatile int *)(addr))
#else
extern unsigned long dotr_sound_host_memory[];
extern volatile int dotr_sound_host_regs[];
#define DOTR_SOUND_PTR(type, addr) ((type *)((char *)dotr_sound_host_memory + ((addr) & 0xffff)))
#define DOTR_SOUND_REG(addr) (dotr_sound_host_regs[((addr) & 0xffff) >> 4])
#endif
/* Return value 1 is observed; viBufCreate ignores it. */
int viBufReset(struct DotrSoundStream *p);
/* small_misc.h keeps a void view with an int pointer for its caller; return value 1 is
 * observed here. The words at 0x10002010/0x10002020 use the same host register array. */
int viBufStopDMA(struct DotrSoundStream *p);
/* SDK-block callee; one int argument (the semaphore id) is observed. */
extern void func_0017b8f0(int unknown);
int viBufDelete(struct DotrSoundStream *p);
/* b is an address: its low 28 bits are kept and 0x20000000 is set. */
int viBufCreate(struct DotrSoundStream *p, int a, unsigned b, int c, int d, int e);
/* Same declarations as sound_queue.h (library callees). */
extern void func_0017bb60(int unknown);
extern int func_0017bc80(int unknown);
extern int func_0017bca0(int *block, int unknown);
int sendToIOP(int a, int b, int n);
/* Copies two source runs into two destination runs through sendToIOP, clamping the
 * source total to the destination total; returns the copied total. Address-as-int
 * parameters follow sendToIOP and are assumed. */
int sendToIOP2area(int dst1, int dlen1, int dst2, int dlen2, int src1, int slen1, int src2, int slen2);
/* Defined in small_misc; declared here for audioDecPause. */
extern void changeInputVolume(int value);
/* Six- and seven-argument views of func_00190810 (same address). */
extern unsigned func_00190810_ret6(int a, int b, int c, int d, int e, int f);
extern void func_00190810_args7(int a, int b, int c, int d, int e, int f, int g);
void audioDecPause(struct DotrSoundStream *p);
extern void func_001843e0(void);
extern void func_00184428(void);
/* Item stride 314944 bytes from the index arithmetic; only the first word is written. */
struct DotrSoundBig {
    int state;
    unsigned char unknown_04[60];
    unsigned int tags[2][0x99c0]; /* decBs0: two tag blocks handed to setImageTag */
};
struct DotrSoundBigQueue {
    unsigned char (*images)[0x195000]; /* decBs0: one image per item */
    struct DotrSoundBig *items;
    int index;
    int count;
    int capacity;
};
void voBufIncCount(struct DotrSoundBigQueue *p);
/* decBs0 (original name; `voBuf` is the Japanese build's name for the
 * 20-byte object at 0x350600). The decoder layout is only what the function
 * reads: width, height, a frame count and a state word at +0xa8. The three
 * library callees take the decoder; their results are assumed
 * int. small_misc.h declares voBufGetData/switchThread/ErrMessage with its
 * own types for the same functions. */
struct DotrSoundVideoDec {
    int width;
    int height;
    int frame_count;
    int unknown_0c[39];
    int state;
};
extern struct DotrSoundBigQueue data_00350600_sound;
extern char data_0030f8b0[];
extern char data_0030f8c8[];
extern int func_0018ee58(struct DotrSoundVideoDec *dec);
extern int func_0018ed40(struct DotrSoundVideoDec *dec, DotrSoundAddress data, int size);
extern int func_0018ee78(struct DotrSoundVideoDec *dec);
extern DotrSoundAddress voBufGetData(struct DotrSoundBigQueue *queue);
extern void switchThread(void);
extern void ErrMessage(DotrSoundAddress text);
extern void setImageTag(unsigned int *tags, void *image, int index, int w, int h);
int decBs0(struct DotrSoundVideoDec *dec);
/* Word view of the object audioDecReset resets; +0x48 is not touched here. */
struct DotrSoundVoice {
    int unknown_00;
    unsigned char unknown_04[0x28];
    int unknown_2c;
    int unknown_30;
    int unknown_34;
    int unknown_38;
    int unknown_3c;
    int unknown_40;
    int unknown_44;
    int unknown_48;
    int unknown_4c;
    int unknown_50;
    int unknown_54;
    int unknown_58;
};
void audioDecReset(struct DotrSoundVoice *p);
/* Reports the readable part of the stream's ring as up to two spans
 * (address, size pairs; the second is zero unless the data wraps). The
 * `void` return and the four `int *` outputs are assumed from 0x2451a0;
 * small_misc.h keeps a one-argument int view for 0x2450d0. */
void viBufBeginPut(struct DotrSoundStream *p, int *first, int *first_size, int *second, int *second_size);

/* audioDecCreate takes a word view of the stream record (+0 .. +0x58): the
 * 64-bit total at +0x48 is written as two words there. The allocator-like
 * 0x1828c0 prototype, the two format texts at 0x30f600 / 0x30f61c and the
 * 2048-byte buffer at 0x3507c0 are assumed from this one caller. */
extern int func_001828c0(int size);
extern void *func_00198948(void *destination, int value, int byte_count);
extern char data_0030f600[];
extern char data_0030f61c[];
extern char _0_buf[];
int audioDecCreate(int *p, int a, int b, int size);

/* 0x243de0. The record's first word selects the sector path and the word at
 * +0x30 is the descriptor of the other path; the rest is not read here. The
 * four bytes at 0x29b2f8 are copied to a local as one unit and passed by
 * address to 0x191fc0. isWithAudio is the word small_misc.h declares as an
 * array, and data_00350620_sound is the object small_misc.h declares as
 * audioDec (same address; the two headers cannot be included together).
 * Callees 0x192090, 0x191fc0, 0x192058 are library routines: signatures assumed. */
struct DotrSoundReader {
    int by_sector;
    unsigned char unknown_04[0x2c];
    int descriptor;
};
struct DotrSoundReadMode {
    unsigned char bytes[4];
};
extern struct DotrSoundReadMode data_0029b2f8;
extern int isWithAudio;
extern struct DotrSoundStream data_00350620_sound;
extern int func_00192090(int sectors, int buffer, int unknown, int *error);
extern void func_00192058(void);
extern int func_00191fc0(int position, struct DotrSoundReadMode *mode);
/* Defined in small_misc with a struct DotrMiscStream parameter. */
extern void func_00243230_sound(struct DotrSoundStream *p);
int strFileRead(struct DotrSoundReader *reader, int buffer, int byte_count, int position);

/* 0x243450. A 68-byte record as this routine uses it: a state word, 40 bytes
 * (+4..+43) that it prints as signed bytes and words once `have` reaches 40,
 * and four running words. `have` is reloaded after every store, with no call
 * between, so it is declared volatile; nothing else about the object's
 * sharing is known. The three texts are format strings in the executable's
 * data, passed by address. */
struct DotrSoundFeed {
    int state;
    signed char head[4];
    int unknown_08[7];
    signed char tag[4];
    int unknown_28;
    volatile unsigned have;
    int unknown_30;
    int position;
    int total;
    int size;
    int sum;
};
extern char data_0030f645[];
extern char data_0030f676[];
extern char data_0030f7b1[];
void audioDecEndPut(struct DotrSoundFeed *feed, int count);

/* Decoder ring view of audioDecSendToIOP: state, ring data/put/count/size and the IOP
 * buffer words are observed offsets; the member names are assumed. The three-argument
 * unsigned view of func_00190810 (same address) and the int types of the areas are assumed. */
struct DotrSoundDec {
    int state;
    unsigned char unknown_04[0x2c];
    int data;
    int put;
    int count;
    int size;
    int unknown_40;
    int iopBuff;
    int iopBuffSize;
    int iopLastPos;
    int unknown_50;
    int totalBytesSent;
};
extern unsigned func_00190810_ret3(int a, int b, int c);
/* Defined in small_misc with its own ring view of the same object. */
void iopGetArea(int *a, int *b, int *c, int *d, struct DotrSoundDec *ring, int position);
int audioDecSendToIOP(struct DotrSoundDec *ad);

/* viBufRestartDMA (0x244680). Named-member view of the first 0x48 bytes of DotrSoundStream for
 * this function, which follows the Sony MPEG streaming sample's routine of the same name: the
 * member names are the sample's (not from this build's symbols); the offsets and
 * the unsigned word types are what the code shows (data +0, tag +4, element count +8, DMA
 * start index and count +0xc/+0x10, saved channel-4 address, tag address, count and control
 * +0x1c..+0x28, saved channel-3 address, count and control +0x2c..+0x34, saved IPU bit
 * position and control +0x38/+0x3c, semaphore +0x40, active flag +0x44). Elements are 2048
 * bytes with 16-byte tags. Registers written: 0x1000b000/10/20 (channel 3), 0x1000b400/10/20/30
 * (channel 4), 0x1000f590 from 0x1000f520 (DMA enable), 0x10002000 and 0x10002010 (IPU command
 * and control; the routine waits while the control word is negative). small_misc.h keeps a
 * void view with an int pointer for its caller; return value 1 is observed. */
struct DotrSoundViBuf {
    unsigned data;
    unsigned tag;
    int n;
    int dmaStart;
    int dmaN;
    int readBytes;
    int buffSize;
    unsigned d4madr;
    unsigned d4tadr;
    unsigned d4qwc;
    unsigned d4chcr;
    unsigned d3madr;
    unsigned d3qwc;
    unsigned d3chcr;
    unsigned ipubp;
    unsigned ipuctrl;
    int sema;
    int isActive;
};
int viBufRestartDMA(struct DotrSoundStream *p);

#endif
