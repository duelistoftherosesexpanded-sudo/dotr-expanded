#ifndef DOTR_EE_STACK_FORWARDERS_H
#define DOTR_EE_STACK_FORWARDERS_H

/* Forwarders that pass eight register arguments and six stack arguments on to 0x27de20,
 * inserting a flag and a per-copy constant.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */
/* 0x27de20: sixteen int arguments and an int result are assumed for this view
 * from the forwarders alone (eight registers left untouched, eight doubleword
 * stack slots written). */
extern int TaLogic_GetNearUnit(int pTask, int pTree, int pVarInfo, int nX, int nZ, int eSide, int bDist, int WarnLvl,
                         int nUnitLst, int wUnitNum, int wSize, int wNodeCount, int bChase, int fView,
                         int Powerful, int wIgnoreLst);

/* Seventeen copies differing only in one constant: forward the eight register
 * arguments unchanged and the stack arguments unitOwnerIdArray..distance in place
 * (numberOfValidTargets read as an unsigned halfword, distance as an unsigned byte),
 * then 0 (fView), the copy's constant (1500-3000, Powerful) and ignoreList. The
 * result of 0x27de20 is returned as left in v0; that the forwarders return a value
 * is assumed. */
int TaLogic002_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic001_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic003_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic004_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic005_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic006_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic007_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic008_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic010_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic011_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic012_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic013_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic014_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic015_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic018_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic019_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic022_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
/* Three further copies (88 bytes) that pass 1 instead of 0 before the constant
 * (2200 or 3000); same assumptions. */
int TaLogic009_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic020_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);
int TaLogic021_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList);

#endif
