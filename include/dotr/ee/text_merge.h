#ifndef DOTR_EE_TEXT_MERGE_H
#define DOTR_EE_TEXT_MERGE_H

/* SzText_LoadMerge (0x2009e0): loads the merged text and texture archives into the SzText item
 * table by mode, freeing the groups it replaces.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* The SzText item table gSzText_aItem (0x3229a0, 1,024 twelve-byte entries; entry_table.h
 * and small_misc.h have other views): a pointer (nonzero: the entry is in use), a word, a
 * halfword whose bits 3..15 are a signed 13-bit group, and a signed halfword id. Assumed:
 * the meanings, from SzText_GetItem's use of the same fields. */
struct DotrTextMergeItem {
    void *ptr;
    int x4;
    short lo : 3;
    short cat : 13;
    short type;
};
extern struct DotrTextMergeItem gSzText_aItem[0x400];

/* anOfs_TEXETC (0x293cc0): four offsets; files 15..17 load the byte range between two
 * neighbours. gSzText_apchMerge (0x293cd0): ten file names, formatted with the environment
 * directory. data_002d074b is the label passed to SzFile_GetEnv and data_002d0750 the
 * format (entry_table.h and side_table.h use the same objects). Names from the Japanese
 * build's symbols. */
extern int anOfs_TEXETC[4];
extern char *gSzText_apchMerge[10];
extern char data_002d074b[];
extern char data_002d0750[];

/* Callees (names from the Japanese build's symbols; func_0019b648 is the sprintf-like
 * formatter other headers declare the same way). Assumed: SzText_LoadFileEx(group, id, name,
 * address, size, file offset, read size) returns nonzero on failure, ORed into the result;
 * SzText_Free(group, id) frees one item or, with id -1, a whole group. */
extern unsigned char *SzFile_GetEnv(const char *label);
extern int func_0019b648(char *destination, const char *format, ...);
extern int SzModel_Preset(int modelId, int modelType, int modelNumber);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern int SzText_Free(int group, int id);
extern int SzText_LoadFileEx(int group, int id, char *name, int address, int byte_size, int byte_offset, int read_size);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzDuel_InqDeck(int side, int position);
extern int SzData_ExchSpecial(int side, int cardNo);
extern int SzData_GetConstDeck(int deck, int slot);

/* 0x2009e0 SzText_LoadMerge(mode, force) (name from the Japanese build's symbols). Modes
 * 0..17; others return 0. Most modes return 1 without loading when their item is already
 * present and force is 0. Returns the loader results ORed together (or the last one). */
int SzText_LoadMerge(int arg0, int arg1);

#endif
