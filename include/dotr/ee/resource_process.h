#ifndef DOTR_EE_RESOURCE_PROCESS_H
#define DOTR_EE_RESOURCE_PROCESS_H

#include "dotr/ee/resource_submit.h"

/* The queue load/release step and the RhFile wrappers around the request queue.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md.
 */
struct DotrResourceReadMode {
    unsigned char bytes[4];
};
extern struct DotrResourceReadMode data_00293cb0[];
extern char data_002d0648[];
extern char data_002d064d[];
extern char data_002d0680[];
extern char data_002d0686[];

extern void func_0017bb60(int unknown);
extern void func_0017bb90(int unknown);
extern int func_0017fa98(const char *path, int flags);
extern int func_0017fd20(int descriptor);
extern int func_0017fea0(int descriptor, int offset, int origin);
extern int func_001800d8(int descriptor, int buffer, int byte_count);
extern int func_00180608(int descriptor, int request, int *result);
extern int func_00191a58(int unknown);
extern int func_00191ef8(void);
extern int func_00191d18(int sector, int sector_count, int buffer, struct DotrResourceReadMode *mode);
extern int func_0019b648(char *destination, const char *format, ...);
extern int SzMem_Alloc(int byte_count);
extern void SzMem_Free(int address);

struct DotrResourcePair *SzFile_InqGroup(int fileType);
int lSzFile_DoRequestMain(int index);
void lSzFile_TermRequest(int index, int completed);
void lRhFile_CdCallback(int reason);
void lRhFile_StrUpper(char *text);
int RhFile_OpenFile(char *name);
int RhFile_ReadAndBlock(int unknown, int byte_offset, int byte_size, int unknown_014);
int RhFile_CloseFile(int unknown);
int RhFile_ReadAndCallbackByFileName(char *name, int byte_offset, int byte_size, int unknown_014,
                  void (*function)(int), int unknown_01c, int flag);
void lRhFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed);
int RhFile_ExistFile(char *name);

/* Views for 0x1fc9b0. The record returned by 0x1ffe00 is read as the submit
 * record with the word at +0x14 used as an address (pointer here, so the host
 * view is wider); only the halfword at +12 of its target is evidenced.
 * gSzView_pCamera_header is the word small_misc.h declares as gSzView_pCamera.
 * The two strings at 0x2cfc50 (format) and 0x2cfc67 (label) are passed by
 * address only. The int argument type is assumed. */
struct DotrLabelHeader {
    unsigned char unknown_00[12];
    unsigned short unknown_0c;
};
struct DotrLabelRecord {
    int status;
    short selector;
    short priority;
    signed int byte_size : 31;
    signed int flag_008 : 1;
    int unknown_00c[2];
    struct DotrLabelHeader *header;
};
extern struct DotrLabelHeader *gSzView_pCamera_header[];
extern unsigned short gSzView_wFrame[];
extern char data_002cfc50[];
extern char data_002cfc67[];
extern void SzView_TermCamera(void);
int SzView_InitCamera(int currentMode);

#endif
