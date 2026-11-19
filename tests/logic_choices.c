/* Synthetic host tests for the logic_choices group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/logic_choices.h"

/* TaLogic011_IsUseLifeHourGlass: 2000 life points, three strong kind-0 monsters in each
 * graveyard, every summon order answers, one own monster on the board. */
int SzDuel_GetLP(int side) { (void)side; return 2000; }
int SzDuel_GetGraveCount(int side) { (void)side; return 3; }
int SzDuel_InqGrave(int side, int index) { return side * 16 + index; }
unsigned int TaHand_GetCardAPDP(int id) { (void)id; return 2100u << 16; }
int TaHand_GetCardKind(int id) { (void)id; return 0; }
int Ta_GetSummonablePieceWithOrder(int side, int i, int c) { (void)side; (void)c; return i; }
int SzDuel_GetUnitMonsterCount(int side) { (void)side; return 1; }

/* TaLogic018_GetVirusMove: unit at column 3, row 3, range 1; only (4, 3) is a passable field 9. */
static int warning_calls;
int SzDuel_GetUnitLoc(int a, int b) { (void)a; (void)b; return 0x0303; }
int SzDuel_GetUnit(int a, int b) { (void)a; (void)b; return 1; }
int GetMoveRev(int unit, int x, int y) { (void)unit; (void)x; (void)y; return 1; }
int GetCap(int limit, int value) { return value < limit ? limit : value; }
int SzDuel_GetField(int x, int y) { return x == 4 && y == 3 ? 9 : 0; }
int TaLogic_IsPassRoot(int a, int b, int x, int y, int tx, int ty, short *out)
{
    (void)a; (void)b; (void)x; (void)y; (void)tx; (void)ty;
    *out = 0;
    return 1;
}
int TaLogic_IsAttackedNow2(int a, int b, int x, int y, int e) { (void)a; (void)b; (void)x; (void)y; (void)e; return 0; }
int TaLogic_GetUnitWarning(int nX, int nZ, int nOwnerID, unsigned char bWarnRange, unsigned short bWarnLvl,
                           unsigned char fView)
{
    (void)nX; (void)nZ; (void)nOwnerID; (void)bWarnRange; (void)bWarnLvl; (void)fView;
    warning_calls++;
    return 10;
}
int TaLogic_GetWarnHideLvl__0015d120(int side) { (void)side; return 0; }
struct jp_9a533 *SzDuel_GetMapUnit(int x, int y) { (void)x; (void)y; return NULL; }

int main(void)
{
    assert(TaLogic011_IsUseLifeHourGlass(0) == 1);
    assert(TaLogic018_GetVirusMove(0, 1) == 0x0304 && warning_calls == 1);
    return 0;
}
