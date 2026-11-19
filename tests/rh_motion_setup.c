/* Synthetic host tests for the rh_motion_setup group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* The inline stack-switch blocks are left out of the host build (the walk runs on the host stack);
 * they are verified only by make match. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/rh_motion_setup.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned char dotr_rh_motion_setup_spr[0x200];
void *gRhModel_dwStack;
unsigned char data_70004000_rhmotion[16];
unsigned char gRhModel_vecScale[16];
static void *walk_data;
static int walk_calls;

void lRhModel_SetupNoMotionRecursive(void *arg0, void *arg1, void *arg2)
{
    walk_calls++;
    walk_data = arg1;
    assert(arg2 == gRhModel_vecScale);
}
int lRhModel_SetupMultiMotionRecursive(void *arg0, int depth, void *arg2)
{
    walk_calls++;
    walk_data = arg0;
    return depth;
}
int lRhModel_SetupFloatMotionRecursive(void *arg0, int depth, void *arg2)
{
    walk_calls++;
    walk_data = arg0;
    return depth;
}
int lRhModel_SetupPulseMotionRecursive(void *arg0, int depth, void *arg2)
{
    walk_calls++;
    walk_data = arg0;
    return depth;
}

int main(void)
{
    int model[16];
    int keys[4] = { 7, 0, 0, 0 };
    struct DotrMotionMulti motion;
    unsigned char *data = (unsigned char *)model + 0x20;

    /* Model data lies at model + the word at +0x10. */
    memset(model, 0, sizeof(model));
    model[4] = 0x20;
    memset(&motion, 0, sizeof(motion));
    motion.count = 3;
    motion.motion = keys;

    /* Each entry point hands the model data to its walk and returns 1. */
    assert(lRhModel_SetupNoMotion(model) == 1 && walk_data == data);
    assert(lRhModel_SetupMultiMotion(model, &motion, &motion, 0.0f, 0.0f, 0.0f, 0.0f) == 1 && walk_data == data);
    assert(lRhModel_SetupFloatMotion((unsigned char *)model, (unsigned char *)&motion, 1.0f) == 1 && walk_data == data);
    assert(lRhModel_SetupPulseMotion((unsigned char *)model, (unsigned char *)&motion, 1.0f) == 1 && walk_data == data
           && walk_calls == 4);
    return 0;
}
