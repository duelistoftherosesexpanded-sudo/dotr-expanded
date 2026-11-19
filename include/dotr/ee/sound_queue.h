#ifndef DOTR_EE_SOUND_QUEUE_H
#define DOTR_EE_SOUND_QUEUE_H

/* Layout reached through the pointer at 0x30ff40, as far as it is accessed. Field names are
 * hypotheses from the accessed offsets. Structure layouts and prototypes are inferred, not the
 * original declarations.
 * Evidence: docs/modules/other.md.
 */
typedef char DotrSoundQueueIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];

struct DotrSoundQueueRead {
    char name[64];
    int destination; /* +0x40: second argument minus the sector remainder */
    int offset;      /* +0x44: third argument rounded down to 2048 */
    int length;      /* +0x48: fourth argument plus the remainder, rounded up to 2048 */
};
typedef char DotrSoundQueueReadSizeMustBe76[(sizeof(struct DotrSoundQueueRead) == 76) ? 1 : -1];

struct DotrSoundQueueRequest {
    int unknown_000;
    int unknown_004;
    int unknown_008;
    int unknown_00c;
};

/* 16-byte table entry. */
struct DotrSoundQueueItem {
    int unknown_000; /* shifted right by two and compared by 0x10ac20 */
    int unknown_004; /* identifier compared by 0x10b3c0 and 0x10ac20 */
    int unknown_008;
    int unknown_00c;
};

/* 32-byte ring entry: one word and 28 copied bytes. */
struct DotrSoundQueueMessage {
    int unknown_000;
    unsigned char data[28];
};

struct DotrSoundQueueManager {
    int unknown_0000;                 /* +0: bit words tested by the control routines */
    int unknown_0004;
    unsigned char unknown_0008[0xc];
    int unknown_0014;
    int unknown_0018;
    unsigned char unknown_001c[0x24]; /* passed by address to 0x17e8b0 */
    int unknown_0040;                 /* +0x40: 0x10c8b0 repeats until nonzero */
    int unknown_0044[4];              /* +0x44: three arguments and zero, passed to 0x17bca0 */
    int unknown_0054[4];              /* +0x54: 16 bytes refreshed by 0x109dc0 */
    int unknown_0064;                 /* +0x64: result of 0x17bca0 */
    unsigned char unknown_0068[0x118];
    unsigned char unknown_0180[0x40]; /* +0x180: source of that refresh; passed to 0x17ea80 */
    int unknown_01c0;
    int unknown_01c4;
    int unknown_01c8;                 /* +0x1c8: position passed to and returned by 0x10a780 */
    int unknown_01cc;                 /* +0x1cc: end position stored by 0x10a8d0 */
    int unknown_01d0;                 /* +0x1d0: write position in the 1 MB ring, 0x10a8d0 */
    int unknown_01d4;
    int unknown_01d8;
    int unknown_01dc;
    int unknown_01e0;
    int unknown_01e4;
    int unknown_01e8;
    int unknown_01ec;
    int unknown_01f0;
    int unknown_01f4;
    int unknown_01f8;
    int unknown_01fc;
    unsigned char unknown_0200[0x10];
    struct DotrSoundQueueItem items[256]; /* +0x210: 0x10ac20 reads all four words of one index */
    unsigned char unknown_1210[0x40];
    struct DotrSoundQueueRead reads[16]; /* +0x1250 */
    int read_count;                      /* +0x1710 */
    int read_write;                      /* +0x1714 */
    int unknown_1718;
    int unknown_171c;
    struct DotrSoundQueueRequest requests[16]; /* +0x1720 */
    int request_count;                         /* +0x1820 */
    int request_write;                         /* +0x1824 */
    int unknown_1828;
    int unknown_182c;                          /* +0x182c: tested by 0x10be20 */
    unsigned char unknown_1830[0x81c];
    int unknown_204c;                          /* +0x204c: nonzero holds back 0x10a4d0's file step */
    unsigned char unknown_2050[0x20];
    char unknown_2070[64];                     /* +0x2070: text formatted by 0x10bc30 */
    int unknown_20b0;
    int unknown_20b4;
    int unknown_20b8;                          /* +0x20b8: bytes left to send, 0x10b6c0 */
    unsigned int unknown_20bc;                 /* +0x20bc: next part's file offset (unsigned: addition order) */
    int unknown_20c0;                          /* +0x20c0: size of the current part, at most 0x80800 */
    signed char ring_write;                    /* +0x20c4 */
    signed char ring_read;                     /* +0x20c5 */
    unsigned char unknown_20c6[2];
    struct DotrSoundQueueMessage ring[8];      /* +0x20c8 */
    char ambience_name[64];                    /* +0x21c8: file name formatted by 0x10a4d0 */
    int ambience_step[2];                      /* +0x2208: per-slot step 0..5 of 0x10a4d0 */
    int ambience_slot[2];                      /* +0x2210: sent with 0x5401; index into unknown_2258 */
    int ambience_file[2];                      /* +0x2218: number formatted into the name */
    unsigned char unknown_2220[0x28];
    int unknown_2248;
    int unknown_224c[2];
    int unknown_2254;
    int unknown_2258;
    int unknown_225c;
    unsigned char unknown_2260[0x80c];
    int unknown_2a6c;                          /* +0x2a6c: last value sent by 0x109f60 */
    unsigned char unknown_2a70[0x10];
};
/* 0x109ca0 clears 0x2a80 bytes and takes the address just past them. */
typedef char DotrSoundQueueManagerSizeMustBe2a80[(sizeof(struct DotrSoundQueueManager) == 0x2a80) ? 1 : -1];
/* One pointer word; the unsized array avoids a small-data access absent from the original. */
extern struct DotrSoundQueueManager *g_SDValue[];

extern void func_0017bb60(int unknown);
extern int func_0017bc80(int unknown);
extern int func_0017bca0(int *block, int unknown);
extern int func_0017e8b0(void *block, int number, int unknown);
extern char *func_0019bdd8(char *destination, const char *source);

void SD_AddReadToEeReq(char *name, int destination, int offset, int length);
void SD_AddEeToIopReq(int unknown_000, int unknown_004, int unknown_008);
int SD_IsAnyJobOnCD(void);
int SDRPC_StartTransferToIop(int unknown_0, int unknown_1, int unknown_2);
int SDRPC_TranserToIopStatus(void);
int SDRPC_Init(void);

/* 0x10be60: steps the read queue and then the request queue once each.
 * Read side, on the state word +0x171c: 0 with a nonzero read_count sets it
 * to 1 and takes one from the count; 1, when SDFS_IsUsingNow() answers 0, passes
 * the entry at index +0x1718 to 0x10c2d0, steps the state and sets bit
 * 0x800000 of the first word; 2, when SDFS_IsUsingNow() answers 0, counts in
 * 0x30ff50, clears that bit, advances the index modulo 16 and steps the
 * state; any other value becomes 0. The request side does the same with
 * +0x182c, +0x1828, request_count, SDRPC_TranserToIopStatus() < 0, 0x10c800 on the three
 * request words and bit 0x8000000, without a counter.
 * 0x10c2c0 and 0x10c2d0 are defined in sound_file; 0x10c2d0 is declared
 * through a `_word` view because the stored destination is an int here.
 * 0x30ff50: one counted word, unsized to keep the absolute access. */
extern int SDFS_IsUsingNow(void);
extern int func_0010c2d0_word(char *name, int destination, int offset, int size);
extern int s_iHowManyTimesCDAreUsed[];
void SD_vDoFsCommandQueue(void);

/* SD_vRunAmbienceOnIopProc (0x10a4d0): the ambience members above are named for what
 * this function does with them; the byte at +0x58 is read through the second word of
 * unknown_0054 and the table at +0x2258 through unknown_2258. The buffer word 0x30ff48
 * is used arithmetically (sound_control.h declares it as a pointer array; this group
 * casts that declaration's element). 0x10bde0 / 0x10be20 are defined in sound_control,
 * 0x10c720 in sound_command, 0x2006c0 in resource_process; 0x19b648 is the library's
 * formatter, and 0x2b9090 holds its format text. */
extern char *g_SDBuffer[];
extern char data_002b9090[];
#if defined(__MWERKS__)
typedef int DotrSoundQueueWord;
#else
#include <stdint.h>
typedef intptr_t DotrSoundQueueWord;
#endif
extern int func_0019b648(char *destination, const char *format, ...);
extern int SD_DoesExistAnyCDReq(void);
extern int SD_DoesExistAnySIFReq(void);
extern int RhFile_ExistFile(char *name);
extern int SDRPC_SendCommand(int command, ...);
void SD_vRunAmbienceOnIopProc(void);

#endif
