#ifndef DOTR_EE_RESOURCE_HELPERS_H
#define DOTR_EE_RESOURCE_HELPERS_H

/* File request queue helpers: queue searches, the request step and the directory lookup.
 * Structure layouts and prototypes are inferred, not the original declarations; only the
 * members these functions use are laid out. Evidence: docs/modules/engine.md.
 */
typedef char DotrResourceIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];
typedef char DotrResourceShortMustBe16Bits[(sizeof(short) == 2) ? 1 : -1];

struct DotrResourceQueueRecord {
    int status;
    short selector;
    short priority;
    unsigned char unknown_008[28];
};
typedef char DotrResourceRecordSizeMustBe36[(sizeof(struct DotrResourceQueueRecord) == 36) ? 1 : -1];
extern struct DotrResourceQueueRecord ReadRequestQueueArray[256];

struct DotrResourcePair {
    int selector;
    int value;
};
extern struct DotrResourcePair FileGroupEntryArray[64];

struct DotrResourceCachePrefix {
    unsigned char unknown_000[0x6688];
    int cached_result;
};
extern struct DotrResourceCachePrefix gSzFile_fl_prefix;

struct DotrResourceFlags {
    unsigned char bit_0 : 1;
    signed char bit_1 : 1;
    unsigned char unknown_bits : 6;
};
extern struct DotrResourceFlags data_00322980[];

char *SzFile_GetEnv(const char *label);
struct DotrResourceQueueRecord *SzFile_InqRequestX(int selector, int grouped);
int SzFile_InqRequestEx(void);
int SzFile_EntryGroup(int fileType, int value);
struct DotrResourcePair *SzFile_InqGroupX(int fileType, int grouped);
int SzFile_DoRequestS(void);
void SzFile_DoRequest(void);

#endif
