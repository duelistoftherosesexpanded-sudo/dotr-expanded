#ifndef DOTR_EE_MODEL_LOAD_H
#define DOTR_EE_MODEL_LOAD_H

/* lSzModel_* load callbacks that finish a model object after its file arrives.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Load-callback view of the gSzModel_ahModel objects. Member names are
 * descriptive guesses from this function's stores; the other views are in object_table.h. */
struct DotrModelLoadHeader {
    int node_offset;
    unsigned int flags;
    unsigned char unknown_08[80];
    int gnode_flag;
};
struct DotrModelLoadNode {
    unsigned char unknown_00[0x30];
    float x, y, z, radius;
};
struct DotrModelLoadObject {
    unsigned char unknown_000[0x90];
    float pos[4];
    unsigned char unknown_0a0[0x94];
    struct DotrModelLoadNode *node;
    char *base;
    struct DotrModelLoadHeader *desc;
    unsigned char unknown_140[0x38];
    int heads[8];
    unsigned char unknown_198[0x48];
    signed char motion[4];
    float radius;
    float scale;
    unsigned char unknown_1ec[6];
    signed char index;
    signed char kind;
    short id;
    unsigned short look : 14;
    unsigned char unknown_1f8[6];
    signed char level;
    unsigned char unknown_1ff[7];
    unsigned short loaded_id;
    unsigned char unknown_208[4];
    signed char unknown_bit_0 : 3;
    signed char loading : 1;
};
extern struct DotrModelLoadObject *gSzModel_ahModel[84];
/* Halfword tables indexed by the object's id, then by the first table's value. */
extern unsigned short AlternateCardIds[];
extern unsigned short gSzDuel_Camera[];
/* "DBDIR" and the label that follows "%s/%s/%s.rh0" in the image. */
extern char data_002cfa17[];
extern char data_002cfa2a[];
/* Defined in object_table and resource_helpers. */
int lSzModel_GetGNode(int modelId, int gNodeId, int *maxDepth);
extern char *SzFile_GetEnv(const char *label);
/* 0x1f3910, defined in effect_module (which declares the result, see pool_table.h); three
 * arguments observed, result unused here. */
extern void SzEffect_LoadModule(int a, int b, int c);

/* 0x1e8bf0: finishes object `index & 0x3ff` once its model file is bound: clears the loading
 * bit, resolves the root node, copies its position and radius, clamps the 40/radius scale to
 * 0.3..1, repairs motion selectors that name an absent motion and, for kinds 4-6, takes the
 * id's look value and requests effect module 1. Argument and void return assumed. */
void lSzModel_LoadCB(int index);

#endif
