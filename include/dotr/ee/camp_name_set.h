#ifndef DOTR_EE_CAMP_NAME_SET_H
#define DOTR_EE_CAMP_NAME_SET_H

/* Functions that build the player's name text on the campaign map from the name-entry work area at
 * 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* View of the block at 0x299e10 with the one field 0x224a40 writes: twelve halfwords at +36.
 * Assumed: that they are the stored player name (the function copies each built name
 * character there); nothing else about the block. */
struct DotrCampNameBoard {
    unsigned char pad00[36];
    short name[12];
};
/* The block's byte declaration, as camp_help.h and camp_tikei.h have it. MATCHING DEVICE:
 * 0x224a40 reaches the block as `(struct DotrCampNameBoard *)data_00299e10`; with a struct
 * object symbol at the same address the compiler builds the address twice instead of copying
 * the register as the original does. The host test therefore defines this symbol as a char
 * array large enough for the struct. */
extern signed char data_00299e10[];

/* View of the work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated
 * symbol) with the name-entry fields 0x224a40 uses. Assumed: every name. `end` is compared
 * with the slot number, `mode`, `code` and `page` are read per slot, `hash`, `pick` and `deck`
 * are written. */
struct DotrCampNameWork {
    unsigned char pad[21454];
    signed char end;            /* 0x1ed53ce */
    signed char mode[12];       /* 0x1ed53cf */
    signed char code[12];       /* 0x1ed53db */
    signed char page[12];       /* 0x1ed53e7 */
    unsigned char pad2[3];
    unsigned short hash;        /* 0x1ed53f6 */
    unsigned short pick[3];     /* 0x1ed53f8 */
    unsigned short deck[3];     /* 0x1ed53fe */
};
#ifdef __MWERKS__
#define DOTR_CAMP_NAME_WORK ((struct DotrCampNameWork *)0x1ed0000)
#else
extern struct DotrCampNameWork dotr_camp_name_set_work;
#define DOTR_CAMP_NAME_WORK (&dotr_camp_name_set_work)
#endif

/* Text storage: 0x2d19d0 holds one int per text, an index in halfwords into the halfword
 * array at 0x2d49d4 (other groups have their own views of both). Assumed: element types and
 * that the index counts halfwords, from this function's address arithmetic. */
extern int StringOffsetTable_nameset[];
extern unsigned short StringBlob_nameset[];
/* 0x299ce0: halfwords indexed by the 4-bit name hash, read as three 5-bit numbers.
 * 0x299d00: 12-byte records indexed by such a number; only the first halfword is read.
 * Assumed: both extents and the record's other five halfwords. */
struct DotrCampNameDeck {
    unsigned short id;
    unsigned short more[5];
};
extern unsigned short DeckTab_nameset[];
extern struct DotrCampNameDeck data_00299d00_nameset[];

#define DOTR_CAMP_NAME_TEXT(n) (&StringBlob_nameset[StringOffsetTable_nameset[n]])
/* MATCHING DEVICE: the character is addressed by byte arithmetic on the text's start; plain
 * indexing does not give the original's instructions. */
#define DOTR_CAMP_NAME_AT(n, c) (*(unsigned short *)((char *)DOTR_CAMP_NAME_TEXT(n) + (c) * 2))

/* 0x224a40 Taku_Camp_Ivent_Name_Set (name from the Japanese build's symbol table). For each of
 * the 12 name slots: row = code / 5, column = code % 5 + page * 6; the character is taken from
 * text row + 11 (mode 0, mask 0x1fff), row + 14 (mode 1, mask 0x1fff) or row + 8 (any other
 * mode, mask 0x3fff) and written to text 2 at halfword slot + 2 (slot 0 also to halfword 0);
 * the slot equal to `end` gets 0x8000 set; the result is copied to the board's name. Then
 * hash = sum of the name halfwords' low four bits, & 15; pick[j] = 5-bit field j of
 * 0x299ce0[hash]; deck[j] = id of record pick[j] & 0x3ff, for j = 0..2. Assumed: no
 * parameters, no result. */
void Taku_Camp_Ivent_Name_Set(void);

#endif
