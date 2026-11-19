#ifndef DOTR_EE_CAMP_SYSTEM_H
#define DOTR_EE_CAMP_SYSTEM_H

/* Taku_Camp_System, the per-frame script driver of the campaign map that runs the opening, voyage
 * and focus-move sequences by tick number.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* One of the 22 location records at 0x298ba0, 52 bytes (camp_entry.h, camp_leader_model.h,
 * camp_tikei.h and camp_3d_spots.h have other views). Assumed: the names; that the floats at
 * +0 and +8 are a map position (they are copied to the work area's floats at +0x40 and
 * +0x48); that the eight halfwords at +20 are two rows of four, one row per side, with the
 * count at +36 (camp_entry.h has them as one row of eight). */
struct DotrCampSystemSpot {
    float x;
    float unknown_04;
    float y;
    unsigned char unknown_0c[8];
    short list[2][4];           /* +20 */
    short n;                    /* +36 */
    unsigned char unknown_26[14];
};
extern struct DotrCampSystemSpot data_00298ba0_campsystem[];

/* View of the camp state at 0x299e10 (camp_entry.h has a fuller view; camp_leader_model.h
 * reads +199 and +200). Assumed: the names; that the bytes from +3 are one state per
 * location (indexes up to 16 are written here, others read; the dimension 63 is the room up
 * to +66); `h194` as the script's tick number, `b198` as the location handed to
 * Taku_Tyumoku_ido_katimake, `b199` as a location and `b201` as a 0/1 flag. The members
 * between are not used by this function. */
struct DotrCampSystemBoard {
    signed char b0;             /* +0: side */
    signed char b1;
    signed char b2;
    signed char lv[63];         /* +3 */
    short h66[32];
    short h130[32];
    short h194;                 /* +194 */
    short h196;
    signed char b198;
    signed char b199;
    signed char b200;
    signed char b201;
};
extern struct DotrCampSystemBoard data_00299e10_campsystem;

/* One of the 32 route records (two rows of 16, 516 bytes each) at +0x22c of the work area;
 * only the halfword at +0x202 is written here (with 1). Assumed: the two float runs and the
 * count, which give the record its size (camp_leader_model.h, camp_entry.h). */
struct DotrCampSystemRecord {
    float x[64];
    float y[64];
    short count;
    short flag;                 /* +0x202 */
};
struct DotrCampSystemBank {
    unsigned char unknown_0000[0x22c];
    struct DotrCampSystemRecord records[2][16];     /* 0x1ed022c */
};

/* The 76-byte lists at 0x299020, two rows of 16 (glide_steps.h and point_lists.h have other
 * views); only the halfword at +70 is read, as an index of the board's location bytes.
 * Assumed: the name `cell` and the two dimensions. */
struct DotrCampSystemList {
    unsigned char unknown_00[70];
    short cell;                 /* +70 */
    short unknown_48[2];
};
extern struct DotrCampSystemList data_00299020_campsystem[2][16];

/* 0x34f6f0: a word cleared by this function. Assumed: int. */
extern int data_0034f6f0_campsystem;

/* The work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated symbol).
 * The function reaches it by offset (macros WB, WH and WF in the source) and, for the route
 * records, through DotrCampSystemBank. On the host the base is a fixture of 0x5800 bytes that
 * the test defines. Offsets used, with ASSUMED types and no established meanings: halfwords
 * 0x1a, 0x1c, 0x60, 0x6c, 0x1a4, 0x536c (the step of the running sequence), 0x53b6, 0x53b8;
 * floats 0x40, 0x48 (the focus position), 0x50, 0x58 (its start for the interpolation),
 * 0x1b8, 0x1c0; signed bytes 0x18c (camp_entry.h: both sides done), 0x5372, 0x5373, 0x5375,
 * 0x537a, 0x537b, 0x537d, 0x53a0, 0x53b4, 0x53b5, 0x546c (BGM 4 already requested). */
#ifdef __MWERKS__
#define DOTR_CAMP_SYSTEM_BASE 0x1ed0000
#define DOTR_CAMP_SYSTEM_BANK ((struct DotrCampSystemBank *)0x1ed0000)
#else
union DotrCampSystemWork {
    unsigned char bytes[0x5800];
    struct DotrCampSystemBank bank;
};
extern union DotrCampSystemWork dotr_camp_system_work;
#define DOTR_CAMP_SYSTEM_BASE (dotr_camp_system_work.bytes)
#define DOTR_CAMP_SYSTEM_BANK (&dotr_camp_system_work.bank)
#endif

/* Callees as this function calls them (no other group header is included; the host test
 * stubs every callee). Taku_Fune_init and My_LedarModel_a_set as small_misc.h declares
 * them, Taku_Tyumoku_ido as glide_steps.h does.
 * Views of this caller, under symbols of their own because the definitions take
 * short parameters while this function passes ints without narrowing them (they link by
 * address): 0x229140 Taku_Tyumoku_ido_kettei (three), 0x2294b0 Taku_Tyumoku_ido_katimake
 * (side, location), 0x228660 Taku_Dodai_Root_init (side, row, value, list, count), 0x228f50
 * Taku_Tyumoku_ido1 (side, row), and 0x217980 SzSound_PlayBGM with two ints (small_state.h
 * declares it without parameters; camp_entry.h has the same view).
 * Also 0x21e330 Taku_Camp_Ivent (camp_ivent), no arguments seen, and 0x228460
 * Taku_Nagare2_set (glide_steps) with FIVE arguments here (side, location, list, count, flag).
 * Assumed: every parameter name, the counts beyond the argument registers loaded at these
 * call sites and the result types. */
extern int func_00229140_campsystem(int a, int b, int limit);
extern int func_002294b0_campsystem(int side, int pick);
extern void func_00217980_campsystem(int n, int b);
extern void func_0021e330_campsystem(void);
extern void Taku_Fune_init(int a, int b);
extern void func_00228660_campsystem(int side, int row, int value, short *list, int count);
extern int My_LedarModel_a_set(int dir);
extern int Taku_Tyumoku_ido(void);
extern void func_00228460_campsystem(int side, int row, short *list, int count, int flag);
extern int func_00228f50_campsystem(int side, int row);

/* 0x226600 Taku_Camp_System (name from the Japanese build's symbol table). One frame of the
 * campaign map's script; see docs/modules/campaign.md for the tick table. Host
 * callers must keep the board's side byte 0 or 1, the location counts at most 4 and the list
 * entries the function indexes with inside their fixtures: nothing is bounded here.
 * Assumed: no parameters and no result. */
void Taku_Camp_System(void);

#endif
