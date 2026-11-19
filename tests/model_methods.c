/* Synthetic host tests for the model_methods group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/model_methods.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned char dotr_model_methods_spr[0x200];
unsigned char gRhModel_mlsmSwapArea[0x110];
static void *common_part_arg;
static int change_flags, change_calls, straight_calls;

void RhGeomEx_SetMatrixForPointLight(void) {}
void RhGeomEx_SetMatrixForStraightLight(void *arg0) { straight_calls++; }
void *RhVu1Mac_ChangeDrawMethod(void *arg0, int arg1)
{
    change_calls++;
    change_flags = arg1;
    return NULL;
}
void *RhVu1Mac_ReserveChangeDrawMethod(void *arg0) { return NULL; }
void lRhModel_DrawCommonPart(void *arg0) { common_part_arg = arg0; }
void *RhMem_CopyMemory16(void *dest, void *source, int count) { return dest; }
DotrMethodsWord RhBase_SetZBufferMode(DotrMethodsWord arg0, int arg1) { return arg0; }
unsigned char *RhModelCpu_DrawAlpha(unsigned char *arg0, int arg1, int arg2, int arg3) { return arg0; }
void RhView_GetViewScreenMatrix(void *dst) {}
void lRhModel_ChangeL2VtoL2S(int arg0, int arg1, int arg2) {}

int main(void)
{
    struct DotrMethodsNormalFlags normal;
    struct DotrMethodsFastFlags fast;

    /* lRhModel_DrawNormalMethod: flag4_1 and flag5_2 set, lit; the scratchpad word at +0xb0 is
     * zero, so the draw method gets 1 | 0x10 | 0x40 | 2 and it returns after the common part. */
    memset(&normal, 0, sizeof(normal));
    normal.flag4_1 = 1;
    normal.flag5_2 = 1;
    lRhModel_DrawNormalMethod(&normal, 1);
    assert(change_calls == 1 && change_flags == 0x53 && common_part_arg == &normal);

    /* lRhModel_DrawFastMethod: no flags; mode 1, straight-light matrix set, common part called. */
    memset(&fast, 0, sizeof(fast));
    lRhModel_DrawFastMethod(&fast);
    assert(change_flags == 1 && straight_calls == 1 && common_part_arg == &fast);
    return 0;
}
