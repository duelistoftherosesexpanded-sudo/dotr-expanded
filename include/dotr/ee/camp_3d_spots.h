#ifndef DOTR_EE_CAMP_3D_SPOTS_H
#define DOTR_EE_CAMP_3D_SPOTS_H

/* Functions that draw the campaign map's 3D models and raise its 22 location models from the
 * counters in the work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* One of the 22 location records at 0x298ba0, 52 bytes (camp_tikei.h reads the same records
 * as struct DotrTikeiModel, with the halfword at +16 as `kind`). Assumed: the float types, the
 * names, and that the halfwords at +16 and +18 are two model numbers (each is drawn as model
 * number + 34). The float at +4 of record 0 is the ship height of camp_ship_route.h. */
struct DotrCamp3dSpot {
    float x, y, z, w;
    short m0, m1;
    char unknown_14[32];
};
extern struct DotrCamp3dSpot data_00298ba0_camp3d[22];

/* A position built on the stack and passed to the model drawer; `w` is never written. */
struct DotrCamp3dVec {
    float x, y, z, w;
};

/* View of the camp state at 0x299e10 (camp_tikei.h and camp_name_set.h have other views of the
 * same block; the halfword at +194 is `stage` in camp_tikei.h). Assumed: the names, and that
 * the bytes from +3 are one flag per location; only 22 of them are used here, the dimension is
 * merely the room up to +194. */
struct DotrCamp3dBoard {
    signed char mode;
    signed char unknown_01[2];
    signed char seen[191];
    short ticks;
};
extern struct DotrCamp3dBoard data_00299e10_camp3d;

/* View of the work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated
 * symbol) with the fields 0x22a710 uses. The three arrays and `ship_state` are reached through
 * one pointer to the block; the other scalars are literal addresses in the target and members
 * of the same object on the host. Assumed: every name and meaning, and the dimension 32 of the
 * three arrays (22 elements are used; camp_ship_route.h views the halfwords at +0x52ec as its
 * `flag` array and the halfword at +0x1a4 as the ship's state). */
struct DotrCamp3dWork {
    char unknown_0000[0x6e];
    signed char pick_ok;        /* 0x1ed006e: decided here from the flags and the ticks */
    char unknown_006f[0x18c - 0x6f];
    signed char all;            /* 0x1ed018c: DOTR_TIKEI_MODE of camp_tikei.h */
    char unknown_018d[0x1a4 - 0x18d];
    short ship_state;           /* 0x1ed01a4: cleared when a location has finished rising */
    char unknown_01a6[6];
    signed char ship_large;     /* 0x1ed01ac: 0 when the ship model is 27, else 1 */
    char unknown_01ad[5];
    short ship_case;            /* 0x1ed01b2: 0-4, selects the ship's route and model */
    char unknown_01b4[0x52ec - 0x1b4];
    short rise[32];             /* 0x1ed52ec: not 0 while a location is rising */
    short count[32];            /* 0x1ed532c: 0..252 in steps of 4 */
    char unknown_536c[6];
    signed char byte_5372;      /* 0x1ed5372 */
    char unknown_5373[3];
    signed char byte_5376;      /* 0x1ed5376 */
    char unknown_5377[0x5448 - 0x5377];
    signed char started[32];    /* 0x1ed5448: effect 404 has been started for the location */
    char unknown_5468[0x5481 - 0x5468];
    signed char byte_5481;      /* 0x1ed5481: DOTR_TIKEI_FLAG_5481 of camp_tikei.h */
    signed char byte_5482;      /* 0x1ed5482 */
};
#ifdef __MWERKS__
#define DOTR_CAMP3D_WORK ((struct DotrCamp3dWork *)0x1ed0000)
#define DOTR_CAMP3D_PICK_OK (*(signed char *)0x1ed006e)
#define DOTR_CAMP3D_ALL (*(signed char *)0x1ed018c)
#define DOTR_CAMP3D_SHIP_LARGE (*(signed char *)0x1ed01ac)
#define DOTR_CAMP3D_SHIP_CASE (*(short *)0x1ed01b2)
#define DOTR_CAMP3D_5372 (*(signed char *)0x1ed5372)
#define DOTR_CAMP3D_5376 (*(signed char *)0x1ed5376)
#define DOTR_CAMP3D_5481 (*(signed char *)0x1ed5481)
#define DOTR_CAMP3D_5482 (*(signed char *)0x1ed5482)
#else
extern struct DotrCamp3dWork dotr_camp_3d_spots_work;
#define DOTR_CAMP3D_WORK (&dotr_camp_3d_spots_work)
#define DOTR_CAMP3D_PICK_OK (dotr_camp_3d_spots_work.pick_ok)
#define DOTR_CAMP3D_ALL (dotr_camp_3d_spots_work.all)
#define DOTR_CAMP3D_SHIP_LARGE (dotr_camp_3d_spots_work.ship_large)
#define DOTR_CAMP3D_SHIP_CASE (dotr_camp_3d_spots_work.ship_case)
#define DOTR_CAMP3D_5372 (dotr_camp_3d_spots_work.byte_5372)
#define DOTR_CAMP3D_5376 (dotr_camp_3d_spots_work.byte_5376)
#define DOTR_CAMP3D_5481 (dotr_camp_3d_spots_work.byte_5481)
#define DOTR_CAMP3D_5482 (dotr_camp_3d_spots_work.byte_5482)
#endif

/* Callees as this caller uses them; all parameter counts and types are assumed from the
 * argument registers at these call sites. 0x1ee4b0 is SzModel_Draw_Pipe (declared in
 * object_frees.h with other parameter types, hence the view name): cursor, model index, a
 * position or 0, four zeros. SzEffect_Entry as in small_misc.h, SzEffect_IsUse2 as in
 * pool_table.h, Taku_Camp_3D_BOU as in glide_steps.h. 0x22cc40 is Taku_Camp_3D_FUNE, declared
 * in camp_ship_route.h with the same shape (cursor, route row, route column, model); it is kept
 * as this caller's own view. Taku_Camp_3D_LeaderModel (0x22b060, name from the Japanese build)
 * is declared in camp_leader_model.h with this shape (one cursor argument, returned). */
extern int func_001ee4b0_camp3d(int cursor, int index, void *pos, int d, int e, int f, int g);
extern int SzEffect_Entry(int a, int b, int c, int d, int e);
extern int SzEffect_IsUse2(int a, int b);
extern int Taku_Camp_3D_BOU(int cursor);
extern int func_0022cc40_camp3d(int cursor, short a, short b, short model);
extern int Taku_Camp_3D_LeaderModel(int cursor);

/* 0x22a710 Taku_Camp_3D (name from the Japanese build's symbol table). Draws models 34-39 and
 * the two models of location 11 at its record's position. For each of the 22 locations builds
 * a position: the record's when seen[i] is set; otherwise, when rise[i] is not 0, x + 0.4 *
 * ((count[i] >> 2) % 2) and y - h + h * count[i] / 256 (h = 12, or 18 for location 16), and
 * SzEffect_Entry(0, 404, i, 0, 0) once per location (started[i]) when count[i] is not 0 and i
 * is not 11; otherwise y - h. Then steps count[i] by 4 when rise[i] is not 0 (with mode not 0,
 * location 16 only when rise[16] is 2); at 256 the count and rise[i] are cleared, seen[i] set
 * and the ship state cleared. Sets the byte at 0x1ed006e: 1 when the byte at 0x1ed018c is set;
 * else 0 when ticks >= 10000; else, when nothing is rising and the flag chain holds (0x1ed5481
 * set, SzEffect_IsUse2(-1, -1) == 0, 0x1ed5376 set, 0x1ed5482 clear), 1 unless 0x1ed5372 is
 * set or ticks is 100 or 101 (mode 0) or 104, 105 or 111 (other modes); else 1 only when ticks
 * is 0 and the same chain holds. Draws location 0, then locations 1-10 and 12-21 (0x1ed018c
 * set), 1-10 (mode 0), 12-21 (mode 1) or neither. Calls Taku_Camp_3D_BOU, picks (row, column,
 * model) from the halfword at 0x1ed01b2: 0 (0, 15, 27), 1 (0, 0, 27), 2 (1, 4, 30),
 * 3 (1, 5, 30), 4 (1, 11, 27); stores model != 27 at 0x1ed01ac, calls 0x22cc40, draws models
 * 41 and 40 and returns Taku_Camp_3D_LeaderModel(cursor).
 * The switch has no default: for any other value the three picks are read uninitialised, as
 * in the original. Callers of the host build must keep the halfword within 0-4.
 * Assumed: one int parameter and an int result. */
int Taku_Camp_3D(int cursor);

#endif
