#ifndef DOTR_EE_CAMP_LEADER_MODEL_H
#define DOTR_EE_CAMP_LEADER_MODEL_H

/* Functions that move the leader's model (model 30) between two campaign-map locations along the
 * routes held in the work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* One route, 516 bytes: 64 x and 64 z coordinates and a point count (camp_ship_route.h has the
 * same record as DotrShipRoute). Assumed: the names and that the two float runs are 64 long
 * each (the function reads x at +0 and z at +256 from a point index, and the count at +512). */
struct DotrLeaderModelRoute {
    float x[64];
    float z[64];
    short n;
    short pad;
};
/* View of the work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated
 * symbol) with the fields 0x22b060 uses. Assumed: every name and meaning; the element types;
 * the first dimension of `route` (this function only uses row 0). `state`, `t` and `idx` are
 * the leader's own (0 idle, 1 start, 2 moving, 3 arrived; 0..1024 between two points; point
 * index): they are not the ship's fields at +420, +422 and +432 of camp_ship_route.h. `mx` and
 * `my` are read as a screen position (320 and 224 are subtracted), `plane` is handed to
 * Ex3d_IntersectP, `show` gates every moving branch and `alpha` goes to SzModel_SetAlpha. */
struct DotrLeaderModelWork {
    short mx;                   /* 0x1ed0000 */
    short my;                   /* 0x1ed0002 */
    unsigned char pad4[0x190 - 4];
    float plane[4];             /* 0x1ed0190 */
    unsigned char pad1a0[472 - 0x1a0];
    short state;                /* 0x1ed01d8 */
    short t;                    /* 0x1ed01da */
    short pad476;
    short idx;                  /* 0x1ed01de */
    unsigned char pad480[16];
    float pos[3];               /* 0x1ed01f0 */
    unsigned char pad508[4];
    int angle;                  /* 0x1ed0200 */
    unsigned char pad516[0x22c - 516];
    struct DotrLeaderModelRoute route[2][16];   /* 0x1ed022c */
    unsigned char pad17068[21363 - 17068];
    signed char show;           /* 0x1ed5373 */
    unsigned char pad21364[21628 - 21364];
    unsigned char alpha;        /* 0x1ed547c */
};
#ifdef __MWERKS__
#define DOTR_LEADER_WORK ((struct DotrLeaderModelWork *)0x1ed0000)
#else
extern struct DotrLeaderModelWork dotr_camp_leader_model_work;
#define DOTR_LEADER_WORK (&dotr_camp_leader_model_work)
#endif

/* One of the 22 location records at 0x298ba0, 52 bytes (camp_entry.h, camp_tikei.h and
 * camp_3d_spots.h have other views). Assumed: four floats named as a position; the other 36
 * bytes are not used here. */
struct DotrLeaderModelSpot {
    float x, y, z, w;
    char unknown_10[36];
};
extern struct DotrLeaderModelSpot data_00298ba0_leadermodel[22];

/* View of the camp state at 0x299e10 (camp_entry.h has a fuller view). The function reads the
 * bytes at +199 and +200 only, as the location the leader comes from and the one it goes to
 * (meanings assumed from their use as indexes of the location records).
 * MATCHING DEVICE: the function does not use these members; it reads the block through a
 * `signed char *` with the indexes 199 and 200 (DOTR_LEADER_FROM / DOTR_LEADER_TO). */
struct DotrLeaderModelBoard {
    signed char pad[199];
    signed char from;
    signed char to;
};
extern struct DotrLeaderModelBoard data_00299e10_leadermodel;
#define DOTR_LEADER_FROM w[199]
#define DOTR_LEADER_TO w[200]

/* 0x299a30: signed bytes indexed by the leader's id; the value is converted to float and added
 * to the model's height. Assumed: the meaning and the extent (not bounded by the function). */
extern signed char data_00299a30_leadermodel[];

/* A point of four floats, an int rotation and a float scale as handed to the callees. Assumed:
 * the member names; four members each (the function writes three of the rotation and of the
 * scale; the fourth gives the 16-byte stack slots). */
struct DotrLeaderModelVec {
    float x, y, z, w;
};
struct DotrLeaderModelRot {
    int x, y, z, w;
};
struct DotrLeaderModelScale {
    float x, y, z, w;
};

/* Callees as this caller uses them (the other headers are not included: the host test stubs
 * every callee). Declared as in their own groups' headers: SzData_GetCurrentDeck and
 * SzData_GetLeader (side_table), SzView_GetSP (block_table), SzModel_SetDeform,
 * SzModel_SetDeformRatio, SzModel_SetAlpha and SzModel_SetDrawFlag (object_table).
 * Views of this caller, because the parameter types in their own headers differ:
 * SzModel_SetScale (0x1f25d0), SzModel_SetPos (0x1f22b0) and SzModel_SetRotC (0x1f24c0) with
 * this group's blocks, Ex3d_GetPointOnScreen (0x205cb0) and Ex3d_IntersectP (0x206430) with
 * this group's vector. Assumed: the parameter counts and types seen at these call sites.
 * 0x1a6c18 and 0x1a22c0 are routines of the library block 0x17b48c-0x1a6c80
 * (out of scope: declared only), assumed to be atan2-like (two floats, float result) and a
 * float-to-int conversion, as camp_ship_route.h assumes. */
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern float func_001a6c18(float a, float b);
extern int func_001a22c0(float value);
extern void func_00205cb0_leadermodel(struct DotrLeaderModelVec *out, float x, float y);
extern void *SzView_GetSP(int index);
extern void func_00206430_leadermodel(float *plane, struct DotrLeaderModelVec *p1, struct DotrLeaderModelVec *p2,
                                      struct DotrLeaderModelVec *out);
extern int SzModel_SetDeform(int modelId, int value);
extern int SzModel_SetDeformRatio(int index, int deformToSet);
extern int func_001f25d0_leadermodel(int index, struct DotrLeaderModelScale *in);
extern int func_001f22b0_leadermodel(int index, float *in);
extern int func_001f24c0_leadermodel(int index, struct DotrLeaderModelRot *in);
extern int SzModel_SetAlpha(int index, int value);
extern int SzModel_SetDrawFlag(int index, int value);

/* MATCHING DEVICE: route points are reached by pointer arithmetic with the constant last. The
 * macros use the caller's `a`, `b` and `idx`. */
#define DOTR_LEADER_X(k) (*(DOTR_LEADER_WORK->route[a][b].x + idx + (k)))
#define DOTR_LEADER_Z(k) (*(DOTR_LEADER_WORK->route[a][b].z + idx + (k)))
#define DOTR_LEADER_N DOTR_LEADER_WORK->route[a][b].n
/* A direction as a 16-bit angle: 65536 * atan2 / 2 pi, converted and taken % 65536 (signed). */
#define DOTR_LEADER_ANGLE(x, z) (func_001a22c0(65536.0f * (func_001a6c18(x, z) / 6.2831855f)) % 65536)
#define DOTR_LEADER_SPOT data_00298ba0_leadermodel

/* 0x22b060 Taku_Camp_3D_LeaderModel (name from the Japanese build's symbol table). Moves and
 * places model 30 (the leader) for one frame. h = the byte of 0x299a30 for the leader of side
 * 0's current deck. The pair (from, to) = board bytes +199 and +200 picks a column b of route
 * row 0 and a direction:
 *   forward: 1>2 b1, 1>8 b2, 2>3 b3, 2>5 b4, 2>7 b12, 5>7 b13, 6>5 b10, 7>11 b14, 8>6 b5, 8>7 b11;
 *   reverse: 3>2 b3, 5>2 b4, 5>6 b10, 9>8 b6, 11>7 b14; any other pair has no route.
 * All moving branches need state != 0, 0 < show and t <= 1024.
 * - Forward route: heading = angle of segment idx..idx+1; position = point idx + that segment
 *   * t / 1024 with height (location 0's y) + h, or location `to` (height h + its y) in state
 *   3. Then when n <= idx the heading is that of segment idx-1..idx; when n - 1 <= idx it is
 *   kept; otherwise it is the angle of this segment and the next blended by t / 1024. When
 *   t < 1024 and idx < n: t += 256 and at 1024 t = 0 and idx++; when idx reaches n, state = 3
 *   and idx = n.
 * - Reverse route: state 1 first sets idx = n - 1 and state = 2. The segments are taken
 *   backwards (point idx - point idx+1, and point idx+1 - point idx+2 as the other one);
 *   position = point idx+1 + segment * t / 1024; the same three heading cases (blend: the
 *   other segment * (1024 - t) + this one * t). When state < 3, t < 1024 and 0 <= idx: t +=
 *   256 and at 1024 t = 0 and idx--; below 0, state = 3 and idx = 0.
 * - No route: heading = angle of (to.x - from.x, to.z - from.z); position = location `from` +
 *   that * t / 1024 (height h + from's y), or location `to` in state 3; t += 16 and at 1024
 *   t = 0, state = 3, idx = 0 and the position is location `to`.
 *   In all three, from == to gives t = 0, state = 3 and idx = n (forward) or 0 (in the two
 *   routed branches no pair of the table has from == to).
 * - Otherwise (idle): position = location `from` (height h + its y); the point (mx - 320,
 *   my - 224) goes through Ex3d_GetPointOnScreen, the line from it to *SzView_GetSP(0) is cut
 *   with the plane at +0x190 by Ex3d_IntersectP, and the angle from the position to that
 *   point is ADDED to the stored angle.
 * Then: SzModel_SetDeform(30, 1), SzModel_SetDeformRatio(30, 32), scale (0.5, 0.5, 0.5),
 * position, SzModel_SetRotC with (0, angle + 32768, 0), SzModel_SetAlpha(30, alpha), the stored
 * angle cleared, SzModel_SetDrawFlag(30, 1). Returns its argument unchanged.
 * The function discards one 0x1a6c18 result per routed call (the other segment's angle).
 * `from` and `to` index the location records unchecked and idx the route points (idx - 1 with
 * n <= idx): host callers must keep them within their fixtures.
 * Assumed: one int parameter (a cursor, as its caller 0x22a710 uses it) returned unchanged. */
int Taku_Camp_3D_LeaderModel(int cursor);

#endif
