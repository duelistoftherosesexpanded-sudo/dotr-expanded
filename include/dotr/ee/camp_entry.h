#ifndef DOTR_EE_CAMP_ENTRY_H
#define DOTR_EE_CAMP_ENTRY_H

/* Functions that set up the campaign map on entry: the reset of the camp state at 0x299e10 and of
 * the work area at 0x1ed0000 for the requested mode, with the model, text, effect and BGM loads.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* A point or direction of four floats. Assumed: the member names. */
struct DotrCampEntryVec {
    float x, y, z, w;
};
/* One of the 32 records (two rows of 16, 516 bytes each) at +0x22c of the work area. Only the
 * halfword at +0x202 is written here (always with 3). Assumed: the two float arrays and the
 * count before it, which give the record its size and are not used by this function. */
struct DotrCampEntryRecord {
    float x[64];
    float y[64];
    short count;
    short state;                /* +0x202 */
};
/* View of the work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated
 * symbol) with the fields 0x21c510 uses. Assumed: every name and meaning, the element types,
 * the dimension 32 of the two halfword arrays at +0x52ec and +0x532c (camp_3d_spots.h calls
 * them `rise` and `count`), and the two arrays before them, which only fill the room.
 * camp_name_set.h calls the halfwords at +0x53f6, +0x53f8 and +0x53fe hash, pick and deck;
 * camp_3d_spots.h has the byte at +0x18c as `all`. */
struct DotrCampEntryWork {
    unsigned char unknown_0000[0x30];
    struct DotrCampEntryVec from;   /* 0x1ed0030: camera position handed to SzView_SetCamera */
    struct DotrCampEntryVec to;     /* 0x1ed0040: camera target */
    unsigned char unknown_0050[0x18c - 0x50];
    signed char all;                /* 0x1ed018c: 1 when board bytes +1 and +2 are both set */
    unsigned char unknown_018d[0x22c - 0x18d];
    struct DotrCampEntryRecord records[2][16];  /* 0x1ed022c */
    short unknown_42ac[2][16];
    short unknown_42ec[2][16][64];
    short rise[32];                 /* 0x1ed52ec */
    short count[32];                /* 0x1ed532c */
    unsigned char unknown_536c[0x53a8 - 0x536c];
    signed char byte_53a8;          /* 0x1ed53a8 */
    unsigned char unknown_53a9[0x53b3 - 0x53a9];
    signed char byte_53b3;          /* 0x1ed53b3 */
    unsigned char unknown_53b4[0x53c7 - 0x53b4];
    signed char byte_53c7;          /* 0x1ed53c7: always set to 2 */
    unsigned char unknown_53c8[0x53f6 - 0x53c8];
    unsigned short hash;            /* 0x1ed53f6 */
    unsigned short pick[3];         /* 0x1ed53f8 */
    unsigned short deck[3];         /* 0x1ed53fe */
    short leader;                   /* 0x1ed5404: result of SzData_GetLeader */
};
#ifdef __MWERKS__
#define DOTR_CAMP_ENTRY_WORK ((struct DotrCampEntryWork *)0x1ed0000)
#else
extern struct DotrCampEntryWork dotr_camp_entry_work;
#define DOTR_CAMP_ENTRY_WORK (&dotr_camp_entry_work)
#endif

/* View of the camp state at 0x299e10 (camp_tikei.h, camp_name_set.h and camp_3d_spots.h have
 * other views of the same block). Assumed: the names; that the bytes from +3 are one state per
 * location (32 are reset here, indexes up to 21 are read; the dimension 57 is merely the room
 * up to +60); the extent of 332 bytes, taken from the byte count that mode 0 hands to
 * func_00195108 with this block's address. */
struct DotrCampEntryBoard {
    signed char side;               /* +0 */
    signed char done0;              /* +1 */
    signed char done1;              /* +2 */
    signed char seen[57];           /* +3 */
    short started;                  /* +60 */
    short unknown_3e[66];
    short ticks;                    /* +194 */
    short unknown_c4;
    signed char kind[3];            /* +198: two of them index the location records */
    signed char byte_c9;            /* +201 */
    unsigned char unknown_ca[332 - 202];
};
extern struct DotrCampEntryBoard data_00299e10_campentry;

/* One of the 22 location records at 0x298ba0, 52 bytes (camp_tikei.h and camp_3d_spots.h have
 * other views). Assumed: the float names (the floats at +0 and +8 become x and z of the camera
 * target), and that the eight halfwords at +20 with their count at +36 list records of row 0
 * of the work area. */
struct DotrCampEntrySpot {
    float x;
    float y;
    float z;
    unsigned char unknown_0c[8];
    short list[8];                  /* +20 */
    short n;                        /* +36 */
    unsigned char unknown_26[14];
};
extern struct DotrCampEntrySpot data_00298ba0_campentry[];

/* Two 16-byte blocks at 0x29a790 and 0x29a7a0, copied to the stack and handed to SzModel_SetPos
 * and SzModel_SetRot for model 30. Assumed: four floats each (only whole-block copies are
 * seen). */
struct DotrCampEntryQuad {
    float f[4];
};
extern struct DotrCampEntryQuad data_0029a790_campentry;
extern struct DotrCampEntryQuad data_0029a7a0_campentry;

/* Bytes at 0x34f710 (cleared on every call), 0x34f6f8 and 0x34f700 (cleared in mode 0) and
 * 0x34f708 (read and cleared in modes 1, 2, 4-7: the "just won" request). Assumed: signed
 * bytes; the meaning of 0x34f708 from its use here only. */
extern signed char Move_END_flg_byte;
extern signed char data_0034f6f8;
extern signed char data_0034f700;
extern signed char data_0034f708;

/* 0x299ce0: halfwords read as three 5-bit numbers; 0x299d00: 12-byte records indexed by such
 * a number, of which the first halfword's low ten bits are used (glide_steps.h and
 * camp_name_set.h read the same tables). Assumed: both extents and the record's other five
 * halfwords. */
struct DotrCampEntryRow {
    unsigned short id;
    unsigned short more[5];
};
extern unsigned short DeckTab_campentry[];
extern struct DotrCampEntryRow data_00299d00_campentry[];

/* Callees as this caller uses them (the other headers are not included: the host test stubs
 * every callee). Declared as in their own groups' headers: the four Taku_* initialisers
 * without arguments (glide_steps, point_lists, ring_setup), SzModel_SetDrawFlag,
 * SzModel_IsModelLoading, SzModel_Load, SzModel_LoadMerge and SzEffect_SetIntensityOnly
 * (object_table), SzGame_SetMenuNum (menu.h), SzData_GetCurrentDeck and SzData_GetLeader (side_table),
 * SzDuel_New2Xax (resource_support), SzText_LoadMerge as build_deck.h has it, func_00195108
 * (library block; a pointer and a byte count, as the other headers assume).
 * Views of this caller, with parameter counts and types assumed from the argument registers at
 * these call sites: Taku_Camp_SINEMA_Init (0x233f50) with an int mode, SzEffect_LoadModule
 * (0x1f3910) with three ints and no result, SzSound_PlayBGM (0x217980) with two ints,
 * SzModel_SetPos (0x1f22b0) and SzModel_SetRot (0x1f2430) with this group's 16-byte block,
 * SzView_SetStraightLightColor (0x1f93f0) with a fourth argument of 0 and SzView_SetCamera
 * (0x1f9fe0) with this group's vectors. */
extern void Taku_Camp_Data_Init(void);
extern void Taku_spline_init(void);
extern void Taku_Camp_3D_Poly_Init(void);
extern void Taku_Screen_Init(void);
extern void func_00233f50_campentry(int mode);
extern int SzModel_SetDrawFlag(int index, int value);
extern int SzGame_SetMenuNum(int index, int value);
extern void func_00195108(void *a, int b);
extern int SzModel_LoadMerge(int modelType, int no, int force);
extern int SzText_LoadMerge(int a, int b);
extern void func_001f3910_campentry(int a, int b, int c);
extern void func_00217980_campentry(int n, int b);
extern int SzModel_IsModelLoading(int modelId);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern int func_001f22b0_campentry(int index, struct DotrCampEntryQuad *in);
extern int func_001f2430_campentry(int index, struct DotrCampEntryQuad *in);
extern int SzDuel_New2Xax(int cardNo);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int d);
extern int func_001f93f0_campentry(int a, int b, int c, int d);
extern void SzEffect_SetIntensityOnly(int kind, float value);
extern int func_001f9fe0_campentry(int index, struct DotrCampEntryVec *from, struct DotrCampEntryVec *to);

/* The five effect modules 400-404 every mode loads, and the reset of the first 32 location
 * bytes. Macros of this source only (the original has the calls and the loop written out in
 * every case); the loop uses the caller's `i` and `board`. */
#define DOTR_CAMP_ENTRY_MODULES() \
    func_001f3910_campentry(0, 400, 0); \
    func_001f3910_campentry(0, 401, 0); \
    func_001f3910_campentry(0, 402, 0); \
    func_001f3910_campentry(0, 403, 0); \
    func_001f3910_campentry(0, 404, 0)
#define DOTR_CAMP_ENTRY_CLEAR_SEEN() \
    for (i = 0; i < 32; i++) { \
        board->seen[i] = 0; \
    }

/* 0x21c510 TkCamp_Init (name from the Japanese build's symbol table). Entry to the campaign
 * map. Always: clears the byte at 0x34f710, calls Taku_Camp_Data_Init, Taku_spline_init,
 * Taku_Camp_3D_Poly_Init and Taku_Camp_SINEMA_Init(3), hides models 30 and 31, sets the byte at
 * 0x1ed018c to (board +1 and +2 both set), calls Taku_Screen_Init and SzGame_SetMenuNum(11, 1)
 * and stores 2 at 0x1ed53c7. Then by mode (any other value returns here):
 * 0: func_00195108(board, 332), model pack (3, 4, 0), text pack (10, 0), modules, the reset
 *    (board +0..+2 and 0x1ed018c cleared, 32 seen bytes cleared), ticks 10020, started 1,
 *    cinema 3, 0x1ed53b3 = 1, 0x34f6f8 and 0x34f700 cleared, BGM (24, 0).
 * 3 / 20 / 30: model pack (3, 4, 0) / (3, 0, 0) / (3, 0, 0), modules, the reset, ticks 10040 /
 *    11182 / 12154, started 1, cinema 2, BGM (24, 1) / (34, 1) / (38, 1).
 * 40 / 50: model pack (3, 4, 0), modules, the reset (50 sets board +0 to 1), ticks 11000 /
 *    12000, BGM (24, 1); started and the cinema are left alone.
 * 60 / 10: hash = 0, pick[k] = bits 5k..5k+4 of the halfword at 0x299ce0 and deck[k] = the low
 *    ten bits of the first halfword of record pick[k] at 0x299d00 (k = 0..2); model pack
 *    (3, 4, 0), modules, the reset, ticks 10050 / 10070, started 1, cinema 2, BGM (25, 1).
 * 1, 2, 4, 5, 6, 7: when model 30 is not loading, the leader of side 0's current deck is asked
 *    twice (the second result goes to 0x1ed5404) and model 30 is placed, turned and loaded
 *    with SzDuel_New2Xax(leader). Then one of four states:
 *    - 0x1ed018c set: model pack (3, 2, 0), light (0, 7, 0, 0), intensity (7, 1.0), board +1 and
 *      +2 = 1, ticks 0, rise and count cleared and seen = 3 for 32 locations, all 32 record
 *      states = 3, cinema 0, 0x1ed53b3 = 0, BGM (4, 0). The modules are NOT loaded here.
 *    - else 0x34f708 set: cleared; model pack (3, 4, 0), text pack (10, 0); board +1 set gives
 *      (+0, +1, +2) = (1, 1, 0), else board +2 set gives (0, 0, 1), else unchanged; 32 seen
 *      bytes cleared, ticks 10060, started 1, cinema 3, 0x1ed53b3 = 3, BGM (25, 1), modules.
 *    - else started not 0: model pack (3, 2, 0), light, intensity; with board +0 == 0: seen[0]
 *      = 1, for each location 1-11 with seen == 3 the row-0 records its list names get state
 *      3, and once eight of the locations 2-10 have seen >= 3 records 12, 13 and 11 of row 0
 *      get state 3; otherwise row-1 record 0 gets state 3, record j - 11 for each location
 *      12-21 with seen == 3, and record 11 when seen[0] == 3. Then started 1, cinema 0,
 *      0x1ed53b3 = 0, the camera target = (x, 0, z) of location kind[1] (written first from
 *      kind[0] and overwritten), the camera position = target + (0, 96, 96) with the target's
 *      w, SzView_SetCamera(0, position, target), BGM (4, 0), modules, BGM (26, 0).
 *    - else: model pack (3, 4, 0), text pack (10, 0); board +1 set gives (1, 1, 0), 0x1ed53a8
 *      = 0 and board +201 = 0, else board +2 set gives (0, 0, 1), 0x1ed53a8 = 1 and board
 *      +201 = 2; 32 seen bytes cleared, ticks 10090, started 1, cinema 3, 0x1ed53b3 = 3,
 *      modules, BGM (26, 0).
 * The target's w member (0x1ed004c) is read without being written here. kind[0] and kind[1]
 * index the location records unchecked: host callers must keep them within their fixture.
 * Assumed: one int parameter and no result. */
void TkCamp_Init(int mode);

#endif
