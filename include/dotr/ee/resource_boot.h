#ifndef DOTR_EE_RESOURCE_BOOT_H
#define DOTR_EE_RESOURCE_BOOT_H

/* File-system start-up: RhFile_Init and the IOP module loaders it uses.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md.
 * Target offsets assume 32-bit pointers; hosts with wider pointers differ.
 */
typedef char DotrResourceBootIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];

struct DotrResourceBootModule {
    char *name;
    int argument;
};
/* Scanned until a null name; the terminator is part of the table data. */
extern struct DotrResourceBootModule gSzFile_aIopList[];

struct DotrResourceBootEntry {
    char *name;
    int unknown_004;
    int first_sector;
    int unknown_00c[3];
    int sector_count;
    int unknown_01c;
};
extern struct DotrResourceBootEntry mapFile[97];

/* Only the two leading words are read; the frame reserves more than 32 bytes. */
struct DotrResourceBootFile {
    unsigned int first_sector;
    unsigned int byte_size;
    unsigned char unknown_008[28];
};
typedef char DotrResourceBootFileSizeMustBe36[(sizeof(struct DotrResourceBootFile) == 36) ? 1 : -1];

/* Distinct view of the storage at 0x31c2f0 also described by resource_helpers.h. */
struct DotrResourceBootState {
    unsigned char unknown_000[0x80];
    unsigned char buffer_080[0x4000];
};
extern struct DotrResourceBootState dotr_ee_resource_boot_state_0031c2f0;

extern char data_002d0638[];
extern char data_002d0657[];
extern char data_002d066f[];
extern char data_002d0670[];
extern char data_002d067d[];

extern void func_0017e240(int mode);
extern void func_0017fa60(void);
extern void func_00182838(void);
extern void func_00182cc8(void);
extern int func_00183538(const char *path, int argument_size, const char *arguments);
extern int func_00183a00(void);
extern int func_00183a38(const char *image);
extern void func_00190a90(void (*callback)(void));
extern void func_00190c48(int argument, void *buffer, int size);
extern int func_00190ff0(struct DotrResourceBootFile *file, const char *name);
extern void func_00191778(int mode);
extern int func_00191a58(int mode);
extern void func_00191c50(int mode);
extern char *func_00197548(const char *label);
extern char *func_0019b9d0(char *destination, const char *source);
extern char *func_0019bdd8(char *destination, const char *source);
extern void lRhFile_CdCallback(void);
extern void lRhFile_StrUpper(char *text);

int RhFile_Init(void);
int RhFile_LoadIopModuleWrapper(char *name, int argument);
int RhFile_LoadIopModuleWrapperEx(char *name, int argument_size, char *arguments, int flag);

#endif
