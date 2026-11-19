#ifndef DOTR_EE_ENTRY_TABLE_H
#define DOTR_EE_ENTRY_TABLE_H

/* The SzText texture item table (gSzText_aItem, 1024 entries) and the texture file loader
 * around it.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md.
 */
typedef char DotrEntryTableIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];

/* 12-byte entry. The halfword at +8 holds three one-bit fields and a signed
 * 13-bit field; a nonzero first word marks the entry as used.
 */
struct DotrEntryTableEntry {
    int pointerToTextureSource;
    int fileNameItCameFrom;
    short flag_0 : 1;
    short flag_1 : 1;
    short flag_2 : 1;
    short group : 13;
    short id;
};
typedef char DotrEntryTableEntrySizeMustBe12[(sizeof(struct DotrEntryTableEntry) == 12) ? 1 : -1];
extern struct DotrEntryTableEntry gSzText_aItem[1024];
/* Six bytes cleared by 0x2007d0; their meaning is unknown. */
extern unsigned char gSzText_tl[];

extern void *RhMem_FillMemory(void *destination, int value, int byte_count);

void SzText_Init(void);
struct DotrEntryTableEntry *SzText_GetItem(int group, int id);
struct DotrEntryTableEntry *SzText_GetReadyItem(int group, int id);

/* 0x202810 stores the result of 0x217b60 on its third argument (assumed to be
 * a name, as resource_submit.h declares it) in the word at +4. 0x2029b0 passes
 * the words at +4 and (when flag_2 is set) +0 to 0x217b50, declared with an
 * int argument as in resource_process.h. */
extern int SzMem_StrDup(const char *name);
extern void SzMem_Free(int address);
int SzText_Free(int group, int id);
int SzText_AddFile(int group, int id, const char *name, int srcImageAddress);

/* SzText_LoadFileEx (0x202220). Callees as their groups declare them; func_0019bdd8 and
 * func_0019b648 are library code used here like strcpy and sprintf.
 * data_002bd0c1 is indexed by the first character of the name and its low two bits are
 * tested (assumed: a character class table); data_002d074b and data_002d0760 are the
 * label passed to SzFile_GetEnv and the format, contents not decoded here. The callback
 * 0x2024d0 is passed by address only. */
extern char data_002bd0c1[];
extern char data_002d074b[];
extern char data_002d0760[];
extern char *func_0019bdd8(char *destination, const char *source);
extern int func_0019b648(char *destination, const char *format, ...);
extern char *SzFile_GetEnv(const char *label);
extern void *SzFile_ReadRequestParam(int fileType, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
                                     int unknown_010, void (*callback)(void *record, int completed),
                                     void *unknown_01c);
void lSzText_LoadFileCB(void *record, int completed);
int SzText_LoadFileEx(int group, int id, const char *name, int address, int byte_size, int byte_offset, int unknown);

#endif
