#ifndef DOTR_EE_RESOURCE_SUBMIT_H
#define DOTR_EE_RESOURCE_SUBMIT_H

#include "dotr/ee/resource_helpers.h"

/* File request submission: queue initialisation, request setup and the submit/wait routines.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md.
 */
struct DotrResourceSubmitRecord;
/* 0x1ffac0 calls a record's callback with the record and its completion flag. */
typedef void (*DotrResourceSubmitCallback)(struct DotrResourceSubmitRecord *record, int completed);

/* Both bits are sign-extracted by the submit/wait code. */
struct DotrResourceSubmitFlags {
    signed char bit_0 : 1;
    signed char bit_1 : 1;
};

/* Distinct view of the storage at 0x31c2f0 also described by resource_helpers.h
 * (gSzFile_fl_prefix, ReadRequestQueueArray at +0x4080 and data_00322980 at +0x6690).
 */
struct DotrResourceSubmitState {
    unsigned char unknown_0000[0x4080];
    struct DotrResourceQueueRecord records[256];
    unsigned char unknown_6480[0x200];
    int unknown_6680; /* 32-bit function address stored by 0x200510, called by 0x200680 */
    int unknown_6684; /* 32-bit name address passed to 0x1fecb0 by 0x2003e0 */
    int cached_result;
    unsigned short head;
    unsigned short count;
    struct DotrResourceSubmitFlags flags;
    unsigned char unknown_6691[0xf];
};
/* 0x1feb80 clears 0x66a0 bytes from 0x31c2f0. */
typedef char DotrResourceSubmitStateSizeMustBe66a0[(sizeof(struct DotrResourceSubmitState) == 0x66a0) ? 1 : -1];
extern struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;

/* Pointer-sized integer for the name argument, whose bit 31 selects sector mode.
 * The target has 32-bit pointers; hosts use their own pointer width.
 */
#if defined(__MWERKS__)
typedef int DotrResourceSubmitWord;
#else
#include <stdint.h>
typedef intptr_t DotrResourceSubmitWord;
#endif
typedef char DotrResourceSubmitLongMustBe64Bits[(sizeof(long) == 8) ? 1 : -1];

/* Field view of one 36-byte queue record as written by 0x1feda0. The callback
 * pointer makes this view larger than the record on hosts with wide pointers.
 * The 31-bit fields are sign-extracted by 0x1ff480.
 */
struct DotrResourceSubmitRecord {
    int status;
    short selector;
    short priority;
    signed int byte_size : 31;
    signed int flag_008 : 1;
    signed int byte_offset : 31;
    signed int flag_00c : 1;
    signed int unknown_010 : 31;
    signed int sector_mode : 1;
    int unknown_014;
    DotrResourceSubmitCallback callback;
    int unknown_01c;
    int handle;
};

/* Distinct view of the 97-entry table at 0x293090 (see resource_boot.h): this
 * code reads the word pair at +8 as one 64-bit value and +0x18 as 32 bits.
 */
struct DotrResourceSubmitEntry {
    char *name;
    long first_sector;
    long unknown_010;
    int sector_count;
    int unknown_01c;
};
extern struct DotrResourceSubmitEntry dotr_ee_resource_submit_entries_00293090[97];

extern char data_002d0638[];
extern char data_002d063d[];

extern char *func_00197548(const char *label);
extern char *func_0019bdd8(char *destination, const char *source);
extern int func_0019bef0(const char *string);
extern int func_0019c028(const char *left, const char *right, int length);
extern char *func_0019c478(const char *string, int character);
extern int SzDuel_Xax2New(int index);
extern void *RhMem_FillMemory(void *destination, int value, int byte_count);
extern int SzMem_StrDup(const char *name);
void lSzFile_StrNormalize(char *text);
void lSzFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed);

int lSzFile_SetReadRequestParam(struct DotrResourceSubmitRecord *record, int fileType, int priority, char *name,
                  int unknown_014, int byte_size, int byte_offset, int unknown_010,
                  DotrResourceSubmitCallback callback, int unknown_01c);
struct DotrResourceQueueRecord *SzFile_ReadRequestParam(int fileType, int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010,
                                              DotrResourceSubmitCallback callback, int unknown_01c);
void SzFile_Init(void);
struct DotrResourceQueueRecord *SzFile_ReadRequest(int fileType, char *name, int unknown_014, int byte_size,
                                              DotrResourceSubmitCallback callback, int unknown_01c);
struct DotrResourceQueueRecord *SzFile_Read(char *name, int unknown_014, int byte_size);
struct DotrResourceQueueRecord *SzFile_ReadParam(int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010);

#endif
