/* Synthetic host tests for the shslot_parts group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/shslot_parts.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

DotrShPartsWord data_00351228_parts;
DotrShPartsReel data_00351240_parts[5];
unsigned char data_00351241_parts;
unsigned char CanStillGet3InARow_parts;
unsigned short data_00351246_parts;
DotrShPartsItems data_002b87a0_parts;
DotrShPartsPulse data_002b8e30_parts;
struct { unsigned char pad[4]; unsigned char lv[12]; } data_00351210_parts;
unsigned char gSh_Lamp_parts[16];
static int sorts, clamps;

DotrShPartsWord SzText_DoRequest_Pipe(DotrShPartsWord arg0, int arg1, int arg2, int *arg3) { return arg0; }
DotrShPartsWord SzText_LoadRequest(int arg0, int arg1) { return 0; }
int SzText_FreeRequest(int arg0, int arg1) { return 0; }
int ExAssignAlpha(void *arg0, long long arg1) { return 0; }
int ExAssignTexture(void *arg0, void *arg1, int arg2) { return 0; }
int ExAssignClamp(void *arg0, long long arg1) { clamps++; return 0; }
int RhBase_GetUserGsAddress(void) { return 0; }
DotrShPartsWord RhText_SetTextureFlush(DotrShPartsWord arg0) { return arg0; }
int SzText_InitClutManage(int base, int size) { return 0; }
int ExSortPrim_Pipe(DotrShPartsWord cursor, void *packet, int a, int depth) { sorts++; return (int)cursor + 1; }

int main(void)
{
    /* Screen 0 with one part of kind 2 (fixed clamp) before the end mark: one clamp and one
     * sorted packet, the cursor advanced once. */
    data_002b87a0_parts.items[0].kind = 2;
    data_002b87a0_parts.items[1].x = -1;
    data_00351228_parts = 10;
    ShSlot_DrawParts(0);
    assert(clamps == 1 && sorts == 1 && data_00351228_parts == 11);
    return 0;
}
