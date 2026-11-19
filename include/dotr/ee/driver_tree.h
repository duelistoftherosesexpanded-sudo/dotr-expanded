#ifndef DOTR_EE_DRIVER_TREE_H
#define DOTR_EE_DRIVER_TREE_H

/* driver.c functions around the root search: the recursive root-tree builders
 * (TaLogic_GetRootTree2, TaLogic_GetInvadeRootTree2: adjacent addresses, same PieceTreetag node
 * type, same callees, each calls itself) and TaLogic_GetLandChange.
 * Names, prototypes, local names and struct PieceTreetag come from the Japanese build's debug
 * information (driver.c). struct jp_9a533 is only passed on and stays opaque here. The callee
 * declarations are the views these two functions need; small_misc.h and driver_root.h hold the
 * callees' own assumed declarations.
 * Evidence: docs/modules/talogic.md. */

struct jp_9a533;
struct PieceTreetag {
    unsigned char bPiece;
    struct PieceTreetag *pNextP[12];
    struct PieceTreetag *pPrevP;
};

/* TaLogic_GetInvadeRootTree2 walks the child slots as bytes: the first slot at +4 of the node,
 * 4 bytes per slot. MATCHING DEVICE: under __MWERKS__ these are literal 4s, which reproduce the
 * original; on the host (8-byte pointers) the same walk uses the host layout. */
#ifdef __MWERKS__
#define DRIVER_TREE_SLOT_OFFSET 4
#define DRIVER_TREE_SLOT_STEP 4
#else
#include <stddef.h>
#define DRIVER_TREE_SLOT_OFFSET offsetof(struct PieceTreetag, pNextP)
#define DRIVER_TREE_SLOT_STEP sizeof(struct PieceTreetag *)
#endif

extern unsigned int GetMoveRev(struct jp_9a533 *vInfo, int nX, int nZ);
extern struct PieceTreetag *TaLogic_RootTreeAloc(unsigned short *wSize, unsigned short *wNodeCount);
extern void TaLogic_SetRootTreeDat(struct PieceTreetag *pTree, struct PieceTreetag *pPrev, int nX, int nZ);
extern int GetCap(int limit, int value);
extern int TaLogic_IsPassRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned short *gUnitID);
extern int TaLogic_IsPassInvadeRoot2(struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, int *gUnitID, int fView, unsigned char Node0, unsigned short bWarnLvl);
extern int TaLogic_IsRootPast2(struct PieceTreetag *pTree, int nX, int nZ, int pX, int pZ, unsigned char bDepth);

/* Manhattan distance between the loop cell (i, j) and (nX, nZ), as the code computes it. */
#define DRIVER_TREE_MIN(a, b) ((b) < (a) ? (b) : (a))
#define DRIVER_TREE_MAX(a, b) ((a) < (b) ? (b) : (a))
#define DRIVER_TREE_DIST ((DRIVER_TREE_MAX(i, nX) - DRIVER_TREE_MIN(i, nX)) + (DRIVER_TREE_MAX(j, nZ) - DRIVER_TREE_MIN(j, nZ)))

/* 0x2794e0 TaLogic_GetRootTree2, with a prototype and views of its own: the node is a byte pointer
 * viewed as DotrTreeNode2 (parent word, then the child slots, as the PieceTreetag slots), and the
 * parent, the variable information, the unit-id pointer (`ownerId`) and the ignore list (`ignoreList`) are
 * 32-bit words (DotrTreeWord). Its callees are called through views that differ from the
 * declarations above: GetMoveRev, TaLogic_RootTreeAloc, TaLogic_SetRootTreeDat, TaLogic_IsPassRoot2
 * (also declared inside the body without a prototype) and TaLogic_IsRootPast2. Assumed: these
 * meanings, from use. */
#ifdef __MWERKS__
typedef int DotrTreeWord;
#else
#include <stdint.h>
typedef intptr_t DotrTreeWord;
#endif
#ifndef NULL
#define NULL 0
#endif
typedef struct DotrTreeNode2 {
    int parent;
    unsigned char *children[48];
} DotrTreeNode2;
extern int func_002464f0_tree(DotrTreeWord arg0, int arg1, int arg2);
extern unsigned char *func_0027b150_tree(unsigned short *arg0, unsigned short *arg1);
extern void func_0027af80_tree(void *arg0, DotrTreeWord arg1, int arg2, int arg3);
extern int func_002798c0_tree(DotrTreeWord arg0, int arg1, int arg2, int arg3, int arg4, DotrTreeWord arg5);
extern int func_0027b010_tree(void *arg0, int arg1, int arg2, int arg3, int arg4, unsigned char arg5);
unsigned char *TaLogic_GetRootTree2(unsigned char *node, DotrTreeWord parent, DotrTreeWord card, int x, int y,
                  int targetX, int targetY, unsigned char limit, signed char *found, DotrTreeWord ownerId,
                  unsigned char depth, unsigned short *pool, unsigned short *poolEnd, DotrTreeWord ignoreList);
struct PieceTreetag *TaLogic_GetInvadeRootTree2(struct PieceTreetag *pTree, struct PieceTreetag *pPrev, struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned char bDist, unsigned char *EndCond, int *gUnitID, int fView, unsigned short bWarnLvl, unsigned char bDepth, unsigned short *wSize, unsigned short *wNodeCount, unsigned short *wIgnoreLst);

/* 0x27c770 TaLogic_GetLandChange (driver.c in the Japanese build; in this group because it
 * shares GetCap and the distance macro). struct jp_9b2bd is the packed cell of the root list, from
 * the Japanese build's debug information; TaLogic_GetRoot2 is declared in the form the host
 * test's stub uses. */
struct jp_9b2bd {
    unsigned char nX : 4;
    unsigned char nZ : 4;
};
extern int SzDuel_GetField(int x, int y);
extern int TaLogic_GetRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ,
                            unsigned short gUnitID, unsigned char bAbort);
/* TaLogic_GetLandChange, with a prototype of its own: all int, the variable information as a
 * DotrTreeWord, the route cells as a nibble-pair view, and the route search through a view (also
 * declared inside the body without a prototype). */
typedef struct DotrTreeNibbles {
    unsigned char low : 4;
    unsigned char high : 4;
} DotrTreeNibbles;
/* The host compiler rejects this prototype next to the unprototyped declaration in the body
 * (short and unsigned char parameters), so the host build sees only the latter. */
#ifdef __MWERKS__
extern int func_002792c0_tree(DotrTreeWord arg0, DotrTreeNibbles *arg1, int arg2, int arg3, int arg4, int arg5, short arg6, unsigned char arg7);
#endif
int TaLogic_GetLandChange(DotrTreeWord unit, int col, int row, int targetFieldType, int arg4, int radius);

/* 0x2777f0 TaLogic_GetNearUnitPickUpTree (driver.c): the third recursive builder; prototype, local
 * names and struct jp_9b50e (one entry of the unit list) come from the Japanese build's debug
 * information. The callees are declared as this caller passes them; small_misc.h, state_grid.h
 * and movie_controls.h hold their own assumed declarations. */
struct jp_9b50e {
    unsigned short nUnitID;
    unsigned char bDist;
};
extern unsigned int SzGame_GetHSyncCount(void);
extern void *SzDuel_GetMapUnit(int x, int y);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int Ta_IsUnitControl2(struct jp_9a533 *vInfo);
extern int TaLogic_IsAdjustUnitLst(struct jp_9b50e *nUnitLst, int Unit, int bDist);
extern int TaLogic_IsUnitWin4(struct jp_9a533 *pVarInfo, int wUnit, int bWarnLvl, int x, int z, unsigned char fView);
struct PieceTreetag *TaLogic_GetNearUnitPickUpTree(struct PieceTreetag *pTree, struct PieceTreetag *pPrev,
    struct jp_9a533 *pVarInfo, int nX, int nZ, int nSide, unsigned char bDist, int fView, unsigned short bWarnLvl,
    unsigned char bDepth, unsigned short *wSize, unsigned short *wNodeCount, struct jp_9b50e *nUnitLst,
    unsigned short *wUnitNum, unsigned short *wIgnoreLst);

#endif
