#ifndef DOTR_EE_GLIDE_STEPS_H
#define DOTR_EE_GLIDE_STEPS_H

/* Taku_* / TkCamp_* campaign-screen functions on the block at the literal address 0x1ed0000 (above the
 * executable): glides of a float pair towards a target in steps of 16 (or 8) out of 256, the
 * path, cursor and camera steps around them and their initialisers.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Views of storage at fixed addresses above the main executable's data
 * (0x1ed0000, 0x1ed0040, 0x1ed01b8, 0x1ed5370). The function addresses them as literal
 * addresses (every access has its own `lui`, also in branch delay slots), so
 * the target build uses integer constants; the host build points the same
 * names at objects the test defines. Only the fields used are evidenced. */
struct DotrGlideHead {
    short width;
    short height;
    unsigned char unknown_04[0x16];
    short unknown_1a;
    short unknown_1c;
    short unknown_1e;
    short unknown_20;
    short unknown_22;
};
struct DotrGlideState {
    float x;
    float unknown_04;
    float y;
    float unknown_0c;
    float from_x;
    float unknown_14;
    float from_y;
    float unknown_1c;
    short step;
    short unknown_22[5];
    short wait;
    short unknown_2e;
    short unknown_30;
    short unknown_32;
    short unknown_34;
    short saved_width;
    short saved_height;
};
struct DotrGlideTarget {
    float x;
    float unknown_04;
    float y;
    float unknown_0c;
    float unknown_10[4];
    short unknown_20;
    short unknown_22;
    short unknown_24;
    short unknown_26;
};
struct DotrGlideFlags {
    unsigned char unknown_00[2];
    signed char first;
    signed char stage;
    unsigned char unknown_04;
    unsigned char second;
    unsigned char unknown_06[4];
    signed char phase;
    signed char unknown_0b;
    unsigned char unknown_0c;
    unsigned char unknown_0d;
    unsigned char unknown_0e[0x22];
    signed char hold;
    unsigned char unknown_31[0xa];
    unsigned char unknown_3b;
    unsigned char unknown_3c[0x14];
    unsigned char picked;
    unsigned char unknown_51;
    signed char unknown_52;
    unsigned char unknown_53[0x43];
    unsigned char unknown_96;
    unsigned char unknown_97[0x39];
    signed char countdown;
    unsigned char unknown_d1[0x2b];
    signed char unknown_fc;
    unsigned char unknown_fd[5];
    unsigned char fading;
    unsigned char unknown_103;
    int total;
    short fade;
};
/* View of 0x299e10 (movie_operations.h declares its first bytes as
 * data_00299e10) and of the 52-byte records at 0x298ba0 (ring_setup.h has
 * another view), as 0x2294b0 uses them. Member names are descriptive guesses. */
struct DotrGlideBoard {
    signed char mode;
    signed char unknown_01[2];
    signed char seen[59];
    short wins[2];
    short row_wins[2][32];
    short ticks;
    short unknown_c4;
    signed char kind[3];
    signed char unknown_c9;
    signed char streak;
};
struct DotrGlideSpot {
    float x;
    float unknown_04;
    float y;
    float unknown_0c[6];
    unsigned char unknown_24[2];
    short marks[7];
};
/* Taku_Screen_Init's view of the block at the literal 0x1ed0000: the head halfwords, two
 * four-float vectors at +0x30 / +0x40 (the second is the storage DotrGlideState starts
 * with) and two 22-word output arrays at +0x8c / +0x10c handed to Taku_3D_2D_Set. */
struct DotrGlideVector4 {
    float x;
    float y;
    float z;
    float w;
};
struct DotrGlideVector3 {
    float x;
    float y;
    float z;
};
struct DotrGlideView {
    short width;
    short height;
    unsigned char unknown_04[0x2c];
    struct DotrGlideVector4 from;
    struct DotrGlideVector4 to;
    unsigned char unknown_50[0x3c];
    int unknown_8c[32];
    int unknown_10c[22];
};
/* Taku_Dodai_Root_init's view of the same block: sixteen 516-byte records per side from
 * +0x22c (point_lists.h types them; only the last halfword is used here), then
 * per-side halfword tables and two 32-halfword tables indexed by board cell.
 * The halfword at +0x5370 is the storage DotrGlideFlags starts with. The
 * function keeps this base in a register for the indexed members and uses the
 * literal views above for single members. */
struct DotrGlideRecord {
    float x[64];
    float y[64];
    short count;
    short unknown_202;
};
struct DotrGlideBank {
    unsigned char unknown_0000[0x22c];
    struct DotrGlideRecord records[2][16];
    short unknown_42ac[2][16];
    short unknown_42ec[2][16][64];
    short unknown_52ec[32];
    short unknown_532c[32];
    short limit;
    short unknown_536e;
    short saved_ticks;
    unsigned char unknown_5372[4];
    signed char idle;
};
/* Taku_Dodai_Root_init's view of the 76-byte lists at 0x299020 (point_lists.h has the
 * fuller view): two cell halfwords. */
struct DotrGlideList {
    unsigned char unknown_00[70];
    short cell;
    short unknown_48;
    short other_cell;
};
#ifdef __MWERKS__
#define DOTR_GLIDE_BANK ((struct DotrGlideBank *)0x1ed0000)
#define DOTR_GLIDE_VIEW ((struct DotrGlideView *)0x1ed0000)
#define DOTR_GLIDE_HEAD ((struct DotrGlideHead *)0x1ed0000)
#define DOTR_GLIDE_STATE ((struct DotrGlideState *)0x1ed0040)
#define DOTR_GLIDE_TARGET ((struct DotrGlideTarget *)0x1ed01b8)
#define DOTR_GLIDE_FLAGS ((struct DotrGlideFlags *)0x1ed5370)
#define DOTR_GLIDE_LIMIT ((short *)0x1ed536c)
#else
extern struct DotrGlideBank dotr_glide_bank_01ed0000;
#define DOTR_GLIDE_BANK (&dotr_glide_bank_01ed0000)
extern struct DotrGlideView dotr_glide_view_01ed0000;
#define DOTR_GLIDE_VIEW (&dotr_glide_view_01ed0000)
extern struct DotrGlideHead dotr_glide_head_01ed0000;
extern struct DotrGlideState dotr_glide_state_01ed0040;
extern struct DotrGlideTarget dotr_glide_target_01ed01b8;
extern struct DotrGlideFlags dotr_glide_flags_01ed5370;
#define DOTR_GLIDE_HEAD (&dotr_glide_head_01ed0000)
#define DOTR_GLIDE_STATE (&dotr_glide_state_01ed0040)
#define DOTR_GLIDE_TARGET (&dotr_glide_target_01ed01b8)
#define DOTR_GLIDE_FLAGS (&dotr_glide_flags_01ed5370)
extern short dotr_glide_limit_01ed536c;
#define DOTR_GLIDE_LIMIT (&dotr_glide_limit_01ed536c)
#endif

extern struct DotrGlideBoard data_00299e10_glide;
extern struct DotrGlideSpot data_00298ba0_glide[];
extern struct DotrGlideList data_00299020_glide[2][16];

/* 0x228970. When the step is 0, remembers the current pair as the start.
 * Adds 16 to the step, limited to 256, and sets each of x and y to
 * start * (256 - step) / 256 + target * step / 256. At step 256 it clears
 * the step, takes the pair as the new start, clears the bytes 0x1ed5372 and
 * 0x1ed5375 and answers 1; otherwise 0.
 * Assumed: no arguments, int result. */
int Taku_Tyumoku_ido(void);

/* 0x2294b0. Stores the pick at 0x1ed53c0 and advances the phase byte at
 * 0x1ed537a: phase 0 marks the pick seen, copies its spot's pair, resets the
 * counters and goes to 1; phase 1 waits for the wait word to reach 256, then
 * goes to 2 when the board's tick count is at least 300 and its kind is one
 * of two fixed sets (chosen by the board's mode byte), else to 3; phase 2,
 * once the countdown is not positive, adds 4096 to the total and 32 to the
 * fade word until it reaches 4096, then goes to 3. Always answers 0.
 * Assumed: the first argument is unused (a0 is overwritten at once), the
 * second is a short, int result. */
int Taku_Tyumoku_ido_katimake(int unused, short pick);

/* 0x21d850. Calls Taku_Coord_Set on the object at 0x299f60, sets the vector at
 * 0x1ed0040 from the spot of the board's kind[0] and then kind[1] (x, 0, third float),
 * the vector at 0x1ed0030 to (x, 96 + y, 96 + z, w) of it, calls
 * SzView_SetCamera(0, from, to), projects the 22 spots (second float + 8.65) through
 * Taku_3D_2D_Set into the two word arrays and stores 320 / 190 in the head.
 * Assumed: no arguments, no result. The callee prototypes follow small_misc.h and
 * block_table.h with this group's own argument types. */
struct DotrGlideFrame;
extern struct DotrGlideFrame prim_data_glide;
extern void Taku_Coord_Set(struct DotrGlideFrame *frame);
extern int SzView_SetCamera(int index, struct DotrGlideVector4 *sourcePoint, struct DotrGlideVector4 *cameraPoint);
extern void Taku_3D_2D_Set(int *x, int *y, void *a);
void Taku_Screen_Init(void);

/* 0x229140. Runs only when the first flag is clear, the limit argument is at most
 * the halfword at 0x1ed536c and the byte at 0x1ed537b is set, or the board's
 * seen byte of kind[0] is at least 2, or kind[0] is 15 with seen[16] clear.
 * Stage byte 0x1ed5373: 1 saves kind[0], the pair and the head halfwords,
 * clears the step and two target halfwords, sets the target word to 1, copies
 * the flag byte +0x52 to the board's unknown_c9 and goes to 2; 2 adds 8 to the step and
 * moves the pair from the saved one towards the spot of kind[0], finishing
 * (stage 3, answer 1, ticks 12040 for kind 15 with seen[16] clear) at step 256
 * or when the pair equals the spot; 3 calls Taku_Camp_Set_Go_Duel once the hold byte
 * is not positive and the target word is 3. Otherwise answers 0.
 * Assumed: the first two arguments are unused (never read), the third is a
 * short, int result. Taku_Camp_Set_Go_Duel as movie_operations.h declares it. */
extern void Taku_Camp_Set_Go_Duel(void);
int Taku_Tyumoku_ido_kettei(short unused, short unused2, short limit);

/* 0x228660. Resets the step, stage and target halfwords and the hold byte,
 * sets both glide flag bytes to 1, records the row in the board (byte +0xc7)
 * and marks seen[row] with the value. For each of the
 * count list entries n: the cell is list n's halfword +0x4a when side is 0 and
 * the row's mark for that position is 2, else its halfword +0x46; when the
 * cell is unseen its two cell halfwords, record n's last halfword, table entry
 * n and the 64 halfwords of row n are cleared. For side 0 with record 12's
 * last halfword 0: counting cells 2-10 seen 3 or more times, the eighth and
 * every later one copies the board ticks to +0x5370 and sets them to 11120.
 * Assumed: short side/row/count, signed char value, no result. */
void Taku_Dodai_Root_init(short side, short row, signed char value, short *list, short count);

/* 0x228f50. By the first flag byte (0x1ed5372): 0 with the halfword at 0x1ed536c clear
 * calls Taku_Dodai_Root_init(side, row, 3, &zero halfword, 0) and adds 1 to that halfword;
 * 1 does one glide step of 16 as Taku_Tyumoku_ido does, but towards the row's spot
 * (step * spot first, the step added after the pair is set, finished only above 256);
 * any other value does nothing. Answers 1 when the glide finishes, else 0.
 * Assumed: short side/row (a0 is passed on unread), int result. */
int Taku_Tyumoku_ido1(short side, short row);
/* 0x228ad0. By the first flag byte: 1 glides the state towards the row's spot in steps
 * of 16 (limited to 256, the step added before the pair is set) and moves the flag to 2
 * when 256 is reached; 2 first (step 0) averages the spots of the cells the list names
 * (the other cell for side 0 where the row's mark is 2; cell 7 is left out and lowers
 * the divisor by one) into the target floats at +0x10 / +0x18, then glides towards them
 * in steps of 8 and, above 256, clears the step, both flag bytes and the board's ticks
 * and answers 1. Otherwise 0. Assumed: short arguments as Taku_Dodai_Root_init has them,
 * int result. */
int Taku_Tyumoku_ido0(short side, short row, short *list, short count);
/* 0x228460 Taku_Nagare2_set (compiled at optimisation level 3, see its source).
 * By the halfword at 0x1ed536c: 0 sets the halfword at 0x1ed001c to 1, clears the one at
 * 0x1ed001a, calls Taku_Dodai_Root_init(side, row, 3, list, count), adds 1 and goes on
 * as 1; 1 waits for 0x1ed001c to be 2, then for side 0 gives every listed record whose
 * last halfword is not 3 the row's mark for that position, for another side sets the
 * first listed record's last halfword to 1 unless it is 3, and adds 1. Always ends with
 * Taku_Tyumoku_ido0(side, row, list, count) and Taku_Tyumoku_ido_kettei(side, row, 2).
 * Assumed: short arguments as Taku_Dodai_Root_init has them, no result. */
void Taku_Nagare2_set(short side, short row, short *list, short count);
/* 0x228150. Copies kind[2] of the board to kind[1] and kind[0], adds 1 to the byte at
 * +0xca (limited to 5), adds 1 (limited to 9999) to the halfword at +0x3e and to entry
 * unknown_c4 of the 32 halfwords at +0x42 when side is not 0, else to +0x40 and the
 * table at +0x82. Then the ticks are cleared when bytes +1 and +2 are both set, or when
 * the seen byte of kind[0] is 3 or more unless the mode is 0, kind[0] is 11 and
 * unknown_c9 is 1; otherwise they become mark 4 (side not 0) or mark 5 of the spot of
 * kind[0], or 313 / 213 for kind 11 with unknown_c9 set. Stores 1 in the word 0x34f6f0.
 * Assumed: one int argument (also tested as a signed char), no result; the ticks test is
 * an inline helper answering a short. Member names are descriptive guesses. */
extern int ACT2_Rock;
/* 0x23b1f0. Camp cursor step. Calls Taku_Coord_Set on the object at 0x299f60, then for
 * each of the 22 spots at 0x298ba0 that is shown (force not 0, the byte at 0x1ed018c
 * set, spot 0 or 11, or its seen byte set and the spot in 1..10 for mode 0 / 12..21
 * otherwise) projects the spot (y + 8.65) into the word arrays at +0x8c / +0x10c and a
 * second point 5.0 to its right, whose screen distance is the spot's reach. With
 * ACT2_Rock set the spot counts as pointed at. Otherwise, when the byte 0x1ed5373 is set
 * the head pair is moved to the screen position of spot kind[0]; if the distance from
 * the head pair to the spot (func_001a6c60 of the sum of two func_001a6c48(d, 2.0f)) is
 * below the reach, kind[0] becomes the spot unless the halfword at +0x6c is set and the
 * byte 0x1ed5373 clear, the spot counts as pointed at when it is kind[0], and the head
 * pair is moved again under the same byte. Then a head pair inside one of the four
 * camp_sys_icon rectangles widened by 2 stores the icon index at +0x80, 1 at 0x1ed547a
 * and selects the icon state. Unless the halfword at +4 is 1 (then bytes +0x6a and +0x7e
 * are cleared): no spot clears +0x6a, +0x7e and +4; a spot stores 1 / 0 / 2 there when
 * its seen byte allows it (0..5 for spot 11, 1..3 otherwise) and the byte at +0x72 is
 * clear or 0x1ed537a set; an icon stores 0 / 1 / 2 and 1 at 0x1ed547a. Last, the three
 * halfwords at +0x224 / +0x226 / +0x228 move by 8 within 96..128: down for a head height
 * below 120, a height of 340 or more and a width of 456 or more respectively, else up.
 * Assumed: one int argument, no result; func_001a6c48 takes two floats and answers a
 * float (power), func_001a6c60 one float (root); both are library code. */
/* camp_tikei.h declares the same 38-byte records; the two headers cannot be included
 * together, so the type is repeated here under a shared guard. */
#ifndef DOTR_TIKEI_ITEM_DEFINED
#define DOTR_TIKEI_ITEM_DEFINED
struct DotrTikeiItem {
    short w[8];
    short unknown_10[10];
    short code;
};
#endif
extern struct DotrTikeiItem camp_sys_icon[];
extern float func_001a6c48(float value, float power);
extern float func_001a6c60(float value);
void TkCamp_Act2(int force);
void Taku_Camp_What_KATIMAKE(int win);
/* 0x233f50. Clears the halfword at 0x1ed001c, then for a mode of 0 to 3 stores the
 * mode at 0x1ed001a and a row of three halfwords at 0x1ed001e (0,0,0 / 64,0,0 /
 * 64,64,0 / 64,64,116); any other mode stores nothing more. The parameter is
 * assumed `signed char` from the sign extension at entry. */
void Taku_Camp_SINEMA_Init(signed char presetNum);

/* 0x21dda0. Below 10000 ticks the cursor goes through Taku_Camp_3D only. From 10000 it goes
 * through Taku_Camp_STON for the counts 11000, 11182, 11184, 12000, 12130, 12152, 12154 and
 * 12156, for 11160 / 12150 when the halfword at 0x1ed536c is at least 2, for 11162 when it
 * is below 9, for 11180 when it is at least 2, for 12132 when it is below 10 and for any
 * other count below 11000; otherwise through Taku_Camp_3D. Then, from 10000 ticks, through
 * Taku_Camp_Kumo.
 * Assumed: one int cursor in and out for all four functions (object_table.h has
 * Taku_Camp_STON; draw_state.h declares this function with its cursor typedef). */
int Taku_Camp_3D(int cursor);
int Taku_Camp_STON(int cursor);
int Taku_Camp_Kumo(int cursor);
int TkCamp_Draw_Pipe(int cursor);

/* 0x21d480, Taku_Camp_Data_Init's view of the whole 21680-byte block at the literal
 * 0x1ed0000 (the other views above name parts of it). Single members are addressed as
 * literals; each loop keeps the base in a register. Member names are offsets except
 * where another view already named the storage. DotrCampSave is the 65440-byte block
 * copied word by word from the literal 0x1e301e0; DotrCampRow is a 12-byte record of
 * which only the first halfword is read. */
struct DotrCampInitBlock {
    short width;
    short height;
    short unknown_04;
    unsigned char unknown_06[0x14];
    short unknown_1a;
    short unknown_1c;
    short unknown_1e;
    unsigned char unknown_20[0x10];
    struct DotrGlideVector4 from;
    struct DotrGlideVector4 to;
    unsigned char unknown_50[0x1a];
    signed char unknown_6a;
    unsigned char unknown_6b;
    short unknown_6c;
    signed char unknown_6e;
    unsigned char unknown_6f[3];
    signed char unknown_72;
    unsigned char unknown_73[0xb];
    signed char unknown_7e;
    unsigned char unknown_7f;
    short unknown_80;
    unsigned char unknown_82[0xa];
    int unknown_8c[32];
    int unknown_10c[32];
    int unknown_18c;
    float surface[4];
    unsigned char unknown_1a0[6];
    short unknown_1a6;
    unsigned char unknown_1a8[6];
    short unknown_1ae;
    short unknown_1b0;
    short unknown_1b2;
    unsigned char unknown_1b4[0x70];
    short unknown_224;
    short unknown_226;
    short unknown_228;
    short unknown_22a;
    struct DotrGlideRecord records[2][16];
    short unknown_42ac[2][16];
    short unknown_42ec[2][16][64];
    short unknown_52ec[32];
    short unknown_532c[32];
    short limit;
    short unknown_536e;
    short unknown_5370;
    signed char unknown_5372;
    signed char unknown_5373;
    unsigned char unknown_5374[6];
    signed char unknown_537a;
    unsigned char unknown_537b[0x54];
    signed char unknown_53cf[12];
    signed char unknown_53db[12];
    signed char unknown_53e7[12];
    unsigned char unknown_53f3[3];
    unsigned short unknown_53f6;
    unsigned short unknown_53f8[3];
    unsigned short unknown_53fe[3];
    unsigned char unknown_5404[0x76];
    signed char unknown_547a;
    unsigned char unknown_547b;
    short unknown_547c;
    unsigned char unknown_547e[0x32];
};
struct DotrCampSave { int words[16360]; };
struct DotrCampRow { unsigned short unknown_00; unsigned short unknown_02[5]; };
typedef char DotrCampInitBlockMustBe54b0[(sizeof(struct DotrCampInitBlock) == 0x54b0) ? 1 : -1];
#ifdef __MWERKS__
#define DOTR_CAMP_INIT ((struct DotrCampInitBlock *)0x1ed0000)
#define DOTR_CAMP_SAVE ((struct DotrCampSave *)0x1e301e0)
#else
extern struct DotrCampInitBlock dotr_camp_init_01ed0000;
extern struct DotrCampSave dotr_camp_save_01e301e0;
#define DOTR_CAMP_INIT (&dotr_camp_init_01ed0000)
#define DOTR_CAMP_SAVE (&dotr_camp_save_01e301e0)
#endif
/* Assumed: func_00195108 takes a pointer and a byte count (as the other headers declare
 * it); SzData_GetData answers a pointer (small_misc.h types it) used here as the save
 * block; HELP_flg is the byte array small_misc.h declares, read signed here. */
extern void func_00195108(void *a, int b);
void Ex3d_GetSurfaceC(float *a, float *b, float *c, float *out);
void *SzData_GetData(int side);
extern unsigned char HELP_flg[];
extern unsigned short DeckTab_halves[];
extern struct DotrCampRow data_00299d00[];
/* Clears the block through func_00195108, hands three constant points to
 * Ex3d_GetSurfaceC with the block's four floats at +0x190 as output, fills three 12-byte
 * rows with 0 / 6 / 1, sets the head (128 at +0x547c, 320 x 190), copies the board's
 * tick count to +0x536e, zeroes the target vector, derives the source vector from it
 * (+96 on y and z) and calls SzView_SetCamera(0, from, to). Clears the two 32-word
 * arrays, the per-side records and halfword tables and the two 32-halfword tables.
 * When the byte at 0x34f6f8 is set, copies the save block to SzData_GetData(0) and
 * clears the byte. Stores the low four bits of the sum of (halfword % 16) over the
 * twelve halfwords at +36 of the board at +0x53f6, then for i = 0..2 the 5-bit field i
 * of DeckTab_halves[that] at +0x53f8 and the low ten bits of data_00299d00[field] at
 * +0x53fe. Assumed: no arguments, no result. */
void Taku_Camp_Data_Init(void);

/* 0x22c570. Sets the byte at 0x1ed5376 to 1, then walks the 516-byte path records (sixteen
 * on side 0, twelve on side 1; Taku_Camp_3D_FUNE reads the same records as 64 x floats,
 * 64 y floats and a point count). Rows 0 and 15 of side 0 and rows 4, 5 and 11 of side 1
 * are skipped. The record's state halfword picks the drawing: 1 draws points 0..reach
 * rising (model height - 10 + 10 * grow / 256 until grow reaches 256) and advances
 * reach towards the count; 2 draws from the count down to reach and retreats reach
 * towards 0 (side 0 first sets reach to the count); 3 draws points 0..count at model
 * height. States 1 and 2 clear the byte; when the far end has grown to 256 they mark
 * the list's cell (state 1) or other cell (state 2) in the halfword table at +0x52ec,
 * set the state to 3 and clear reach and the 64 grow halfwords. Each point is one
 * SzModel_Draw_Pipe(cursor, 62 or 63, &position, 0, 0, 0, 0); the last cursor is returned.
 * Assumed: one int argument and int result; record members x / y / count and the byte
 * +0x5376 are named from this use. The callee prototype follows object_frees.h with this
 * group's own position type. The model height is the second float of the first spot. */
int SzModel_Draw_Pipe(int cursor, int modelId, struct DotrGlideVector3 *position, void *d, int e, int f, int g);
int Taku_Camp_3D_BOU(int cursor);

#endif
