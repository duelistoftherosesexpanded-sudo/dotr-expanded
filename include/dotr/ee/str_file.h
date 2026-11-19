#ifndef DOTR_EE_STR_FILE_H
#define DOTR_EE_STR_FILE_H

/* strFileOpen: opens a stream file by device-prefixed name, either through the CD file calls or
 * a host descriptor.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The stream file record, as far as strFileOpen writes it. strFileRead and strFileClose
 * (in sound_file and small_misc) read by_sector and descriptor through their own
 * views of the same record. Member names are assumed. */
struct DotrStrFile {
    int by_sector;           /* 1: CD path, 0: descriptor path */
    int size;
    int cd_position;         /* +0x08: start of the record func_00190ff0 fills */
    int cd_size;
    unsigned char unknown_10[0x1c];
    int cd_buffer;           /* +0x2c: result of func_001828c0(0x28010) */
    int descriptor;          /* +0x30 */
};

/* String constants at 0x30f7f0-0x30f88b; their text is not recorded here. */
extern char data_0030f7f0[]; /* device name compared with the prefix */
extern char data_0030f7f7[]; /* suffix when the name has a ';' */
extern char data_0030f7f8[]; /* suffix when it has none */
extern char data_0030f7fb[]; /* format: path, suffix */
extern char data_0030f800[]; /* format: device, path */
extern char data_0030f806[]; /* default device */
extern char data_0030f80c[]; /* message: full name */
extern char data_0030f816[]; /* message: CD search failed */
extern char data_0030f839[]; /* message: open failed */
extern char data_0030f854[]; /* message: seek to end failed */
extern char data_0030f86f[]; /* message: seek to start failed */

/* Library block; signatures assumed from these call sites (they behave like
 * strchr, a bounded copy, strcmp, strlen, toupper, sprintf, strcpy, printf, the CD ready
 * poll, an aligned allocation, CD buffer setup, CD file search, CD seek, open, lseek and
 * close). */
extern char *func_001976d8(const char *string, int c);
extern void *func_0019c2b8(void *destination, const void *source, int byte_count);
extern int func_0019bc90(const char *a, const char *b);
extern int func_0019bef0(const char *string);
extern int func_0019d9e0(int c);
extern int func_0019b648(char *destination, const char *format, ...);
extern char *func_0019bdd8(char *destination, const char *source);
extern int func_00199b50(const char *format, ...);
extern int func_00191a58(int mode);
extern int func_001828c0(int size);
extern int func_00191f90(int a, int b, int c);
extern int func_00190ff0(void *file, const char *name);
extern int func_00191fc0(int position, void *mode);
extern int func_0017fa98(const char *path, int flags);
extern int func_0017fea0(int descriptor, int offset, int origin);
extern int func_0017fd20(int descriptor);

/* 0x243a90. Parameter names are assumed; the Japanese build has no debug information for it. */
int strFileOpen(struct DotrStrFile *file, char *name);

#endif
