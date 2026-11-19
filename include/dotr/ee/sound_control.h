#ifndef DOTR_EE_SOUND_CONTROL_H
#define DOTR_EE_SOUND_CONTROL_H

#include "dotr/ee/sound_queue.h"

/* Structure layouts and prototypes are inferred, not the original declarations. Parameter
 * meanings are unknown. Evidence: docs/modules/other.md.
 */
/* One word, used both as an enable test and as a base address by 0x10bc30;
 * unsized to avoid a small-data access absent from the original.
 */
extern char *g_SDBuffer[];

#if defined(__MWERKS__)
typedef int DotrSoundControlWord;
#else
#include <stdint.h>
typedef intptr_t DotrSoundControlWord;
#endif

extern char data_002b90b0[];
/* Format text of 0x10a8d0, taking the item identifier; contents not read here. */
extern char data_002b90a0[];
/* One 16-byte store (`sq`) of 0x10a8d0's zero fill. */
#ifdef __MWERKS__
typedef __int128 DotrSoundQuad;
#else
__extension__ typedef __int128 DotrSoundQuad __attribute__((aligned(4), may_alias));
#endif
extern int func_0019b648(char *destination, const char *format, ...);
extern void *func_0019c2b8(void *destination, const void *source, int byte_count);
extern void *func_00198790(void *destination, const void *source, int byte_count);
extern void *func_00198948(void *destination, int value, int byte_count);
extern void SD_InitializeDevice(void);
extern void SD_vRunAmbienceOnIopProc(void);
/* 0x10adb0. Does nothing unless +4 bit 1 and +0 bits 4, 5 and 6 are set. Calls
 * SD_iStopAutoDMA when +0x1ec is 0, then steps the sequence word +0x1e4:
 * 100 waits for SD_BGMGetVolume() == 0, stops and clears +0x1c8/+0x1cc/+0x1d0;
 * 101-103 count up; 104 takes the id at +0x2248 into +0x1dc (9127: clears +0
 * bits 8, 13, 9 and ends at 0; otherwise goes to 1); 1 loads the id's file
 * (length 0x90000 with +0x1fc bit 1, else 0x100000); 2 and 3 wait for the CD
 * and SIF queues and decode; 4 waits for the SIF queue, then either clears
 * +0x1fc bit 1 and goes on or sets +0x1fc to 4 and ends; 5 loads 0x70000 at
 * 0x90000 and restarts the transfer; 6 waits for the CD queue and ends; any
 * other value becomes 0. Steps 1, 2, 3 and 5 return at once. Otherwise, with
 * +0 bit 8 set and bit 24 of +0x18 different from that of +0x1c4, copies
 * +0x18 to +0x1c4 and +0x1c0, stores bit 24 inverted in +0x1d8, decodes, loads
 * 0x80000 at +0x1cc when bit 8 is still set and +0x1c8 is 0 or 0xa0000, and
 * clears +0x1c0. Assumed: no arguments, no result. */
extern void SD_vAutoDMASequentialWatchDog(void);
/* 0x10b6c0. Nothing without the manager or with +4 bit 2 clear. Steps the word +0x20b0:
 * 1 sends 0x80800 bytes at g_SDBuffer + 0x101800 to +0x14 + 0x40000 (SD_AddEeToIopReq) once
 * SDFS_IsUsingNow() is 0; 2 sends command 0x5100 with +0x20b4 once no SIF request is left;
 * 3 waits for the byte +0x5a, then goes to 0 and clears +0 bit 14; 20 waits for the CD
 * jobs and goes to 1; 30 and 34 wait for them and go on; 31 takes the size word at
 * g_SDBuffer + 0x101804 plus 0x800 as the bytes left (+0x20b8), clears the offset (+0x20bc)
 * and sends the first part (+0x20c0, at most 0x80800); 32 and 36 send command 0x5102 for
 * the part and go on (33) while bytes are left, else to 3; 33 queues the read of the next
 * part of the file named at +0x2070; 35 sends it. Any other value becomes 0 and starts the
 * next queued group (SD_FetchFromXferQueue, SD_KickSEGroupReadSequence). What the steps
 * mean beyond these calls is not established. Assumed: no arguments, no result. */
void SD_vTransferSEGrpToIopProc(void);
extern void SD_vDoFsCommandQueue(void);

/* Three floats copied by 0x109a70. */
struct DotrSoundControlVector {
    float x;
    float y;
    float z;
};

/* Variadic routine at 0x10c720; separate minimal view in sound_command.h. */
extern int SDRPC_SendCommand(int command, ...);

/* Four floats and a pair of them, as read by 0x1099e0. */
struct DotrSoundControlVector4 {
    float x;
    float y;
    float z;
    float w;
};
struct DotrSoundControlPair {
    struct DotrSoundControlVector4 first;
    struct DotrSoundControlVector4 second;
};

void Card_V_IN(struct DotrSoundControlVector4 *result, struct DotrSoundControlVector4 *value,
                   struct DotrSoundControlPair *pair, float divisor);
int SD_Init(struct DotrSoundQueueManager *block);
int VEC_POI_IN(struct DotrSoundControlVector *destination, struct DotrSoundControlVector *source);
int SD_Term(void);
void SD_SuspendSoundSystem(int value);
void SD_CallEveryVSync(void);
int SD_BGMPreLoad(int unknown_000, int flag);
int SD_IsLoadingNow(void);
void SD_SetOutputType(int value);
void SD_StopAll(void);
void SD_BGMStop(void);
void SD_BGMFadeOut(short value);
int SD_BGMGetVolume(void);
void SD_BGMSetSourceLevel(int a, int b);
void SD_SEPlay2(int a, int b, int c);
void SD_SEPlayWithParam6(int a, int b, int c, int d, int e, int g);
void SD_SEPlayOnlyOnce(int a, int b, int c);
void SD_SEStop(int a, int b);
void SD_SEStopAll(void);
int _iDecodeLPCM(int position);
/* The parameter is never read by 0x10ac20; every call in 0x10adb0 passes 0. */
void _vDecodeLPCMThenSendToIOP(int unused);
/* 0x10a8d0. Nothing when the id is not in the item table or the position is not below
 * the item's first word rounded down to 16. Formats the id into +0x1210 and queues reads
 * of [position, position + length) into the 1 MB ring at g_SDBuffer + 0x800 (write
 * position +0x1d0, split at the ring end). With a loop end (item word +0xc, times 4,
 * rounded down) the range is cut there and continues from the loop start (+8); without
 * one, the part past the end is not read and up to 0x20000 bytes after the data are
 * zeroed. Stores the final position at +0x1cc. Parameter names are assumptions. */
void _vLoadBGMFileToEE(int id, int position, int length);
long SD_iGetSDStatus(void);
int SD_iStartAutoDMA(void);
int SD_iStopAutoDMA(void);
void SD_vInitSDThread(void);
void SD_vInitializeAutoDMA(void);
int SD_iPreparePreBlockOnIOP(int unknown_000);
/* 0 unless +4 bit 1 is set and the id is in the item table or is 9127. Then
 * stores the id at +0x2248 and: when +0x1dc is the id and +0 bit 8 is set,
 * sends 0x5004 (256) if +0x1ec < 32767, clears +0 bit 13; when +0x1fc is 4
 * and +0x1dc is the id, sets +0 bit 8, clears +0x1fc bit 2 and +0x1d4, copies
 * +0x1dc to +0x2254 and sends 0x1020 (+0x14, 0x40000, 1); otherwise runs the
 * body of SD_iPreparePreBlockOnIOP (inline, result unused) and sets +0x1fc bit 1. Answers 1.
 * Assumed: int id and result. */
int SD_iPlayBGMByAutoDMA(int unknown_000);
int SD_iTransferSEGrpToIop(int unknown_000, void *data);
void SD_KickSEGroupReadSequence(int unknown_20b4, char *text);
int SD_AddToXferQueue(int unknown_000, void *data);
int SD_FetchFromXferQueue(int *unknown_000, void *data);
void SD_ClearXferQueue(void);
int SD_DoesExistAnyCDReq(void);
int SD_DoesExistAnySIFReq(void);

#endif
