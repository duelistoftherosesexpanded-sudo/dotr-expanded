#ifndef DOTR_EE_CAMP_SHIP_ROUTE_H
#define DOTR_EE_CAMP_SHIP_ROUTE_H

/* Functions that move and draw the ship along a campaign-map route held in the work area at
 * 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* One route, 516 bytes: 64 x and 64 z coordinates and a point count. Assumed: the names and
 * that the two float runs are 64 long each (the function reads x at +0 and z at +256 from a
 * point index, and the count at +512). */
struct DotrShipRoute {
    float x[64];
    float z[64];
    short n;
    short pad;
};
/* View of the work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated
 * symbol) with the fields 0x22cc40 uses. Assumed: every name; the first dimension of `route`
 * (only the 8256-byte stride of 16 routes is evidenced); the extent of `flag` (113 halfwords
 * is merely the room up to the name-entry bytes of camp_name_set.h at +21454, so that the host
 * object has storage: the function indexes it with a record's id without a bound). */
struct DotrShipWork {
    unsigned char pad0[420];
    short state;                /* 0x1ed01a4: 0 idle, 1 start, 2 moving, 3 arrived */
    short t;                    /* 0x1ed01a6: 0..1024 between two route points */
    int angle;                  /* 0x1ed01a8 */
    unsigned char pad428[4];
    short idx;                  /* 0x1ed01b0 */
    unsigned char pad434[2];
    signed char show;           /* 0x1ed01b4 */
    unsigned char pad437[3];
    float pos[3];               /* 0x1ed01b8 */
    unsigned char pad452[104];
    struct DotrShipRoute route[2][16];  /* 0x1ed022c */
    unsigned char pad17068[4160];
    short flag[113];            /* 0x1ed52ec */
};
#ifdef __MWERKS__
#define DOTR_SHIP_WORK ((struct DotrShipWork *)0x1ed0000)
#else
extern struct DotrShipWork dotr_camp_ship_route_work;
#define DOTR_SHIP_WORK (&dotr_camp_ship_route_work)
#endif

/* 0x299066: 76-byte records, 16 per row, indexed like the routes; only the first halfword is
 * read. 0x298ba4: the ship's height. Assumed: the record size's other 74 bytes, the float type
 * and both meanings. */
struct DotrShipRecord {
    short id;
    unsigned char pad[74];
};
extern struct DotrShipRecord data_00299066_shiproute[][16];
extern float data_00298ba4_shiproute;

/* Callees as this caller uses them. 0x1a6c18 and 0x1a22c0 are library-block routines,
 * assumed to be atan2-like (two floats, float result) and a float-to-int conversion, as
 * lk_frames.h and fx_calc.h assume. 0x1ee4b0 is SzModel_Draw_Pipe (declared in object_frees.h
 * with other parameter types, hence the view name); assumed here: a position and a three-int
 * rotation. SzEffect_Entry as in small_misc.h. */
extern float func_001a6c18(float a, float b);
extern int func_001a22c0(float value);
extern int func_001ee4b0_shiproute(int cursor, int index, float *pos, int *rot, int e, int f, int g);
extern int SzEffect_Entry(int a, int b, int c, int d, int e);

/* MATCHING DEVICE: route points are reached by pointer arithmetic with the constant last. */
#define DOTR_SHIP_X(k) (*(DOTR_SHIP_WORK->route[a][b].x + idx + (k)))
#define DOTR_SHIP_Z(k) (*(DOTR_SHIP_WORK->route[a][b].z + idx + (k)))
#define DOTR_SHIP_N DOTR_SHIP_WORK->route[a][b].n
/* A direction as a 16-bit angle: 65536 * atan2 / 2 pi, converted and taken % 65536 (signed). */
#define DOTR_SHIP_ANGLE(x, z) (func_001a22c0(65536.0f * (func_001a6c18(x, z) / 6.2831855f)) % 65536)

/* 0x22cc40 Taku_Camp_3D_FUNE (name from the Japanese build's symbol table). Places the ship on
 * route [a][b]: position = point idx + (point idx+1 - point idx) * t / 1024, height from
 * 0x298ba4. Heading: when idx >= n the angle of segment idx-1..idx; when idx >= n - 1 that of
 * segment idx..idx+1; otherwise the angle of the two segments idx..idx+1 and idx+1..idx+2 blended
 * by t / 1024. The heading is stored in `angle`; when `show` is set model `model + 34` is drawn
 * with rotation (0, heading, 0) and the returned cursor kept. Then, when state > 0, t < 1024 and
 * idx < n: state 1 calls SzEffect_Entry(0, 403, 0, 0, 0) and becomes 2 (handled as state 2 in the
 * same frame); state 2 adds 256 to t and at 1024 clears t and advances idx; when idx reaches n,
 * state = 3, idx = n and flag[record[a][b].id] = 1. Returns the cursor. Assumed: four parameters
 * with these types and an int cursor. */
int Taku_Camp_3D_FUNE(int cursor, short a, short b, short model);

#endif
