#ifndef DOTR_EE_SLOT_SETUP_H
#define DOTR_EE_SLOT_SETUP_H

/* Functions that prepare object-table slot 29 through the 0x1f.... setters, or start a resource
 * by id (0x1c1090).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#ifdef __MWERKS__
typedef __int128 DotrSlotSetupWord128;
#else
__extension__ typedef __int128 DotrSlotSetupWord128;
#endif

/* Local views: a 16-byte and a 144-byte value copied as 128-bit words, and three floats.
 * The setters' own groups declare these with their vector types. */
union DotrSlotSetupQuad {
    DotrSlotSetupWord128 quad;
    float values[4];
};
union DotrSlotSetupElement {
    DotrSlotSetupWord128 quads[9];
    float values[36];
};
struct DotrSlotSetupTriple {
    float x, y, z;
};

/* Initial values of 0x1c1090's four locals. In the executable they sit at 0x291370
 * (16 bytes, zero), 0x291380 (144 bytes), 0x291410 (16 bytes, zero) and 0x291420
 * (1.0, 0.4, 1.0). Assumed: compiler-made initialiser templates in the original;
 * declared here as named objects at those addresses. */
extern union DotrSlotSetupQuad data_00291370;
extern union DotrSlotSetupElement data_00291380;
extern union DotrSlotSetupQuad data_00291410;
extern struct DotrSlotSetupTriple data_00291420;

/* The storage at 0x317490 through its existing byte declaration; only +0xd35 is written. */
extern unsigned char data_00317490_bytes[];

/* Callees as their groups declare them, except SzModel_Load (declared as in lower_rules.h). Pointer
 * parameters are untyped here. */
extern int SzDuel_New2Xax(int cardNo);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int d);
extern int SzSound_TransferSEGrp(int index, int id, int force);
extern int SzModel_SetDrawFlag(int index, int value);
extern int SzModel_SetScale(int index, void *in);
extern int SzModel_SetRot(int index, void *in);
extern int SzModel_SetRotC(int index, void *in);
extern int SzModel_SetPos(int modelId, void *pos);
extern int SzModel_SetDeform(int modelId, int value);
extern int SzModel_SetDeformRatio(int index, int deformToSet);
extern int SzModel_SetAnimDirect(int index, int slot);
extern int SzModel_SetAnimDir(int modelId, int animDir);
extern int SzModel_SetAnimSpeed(int index, float value);
extern int SzModel_SetRgba(int index, int value, void *in);
extern int SzModel_SetFilter(int index, int value, void *in);
extern int SzModel_SetIntensity(int modelId, void *intensity);
extern int SzView_SetStraightLight(int index, int k, void *in);
extern int SzView_SetCP(int index, void *in);

/* 0x1c1090: value >= 0: id = SzDuel_New2Xax(value), SzModel_Load(29, -1, id, 1),
 * SzSound_TransferSEGrp(2, id, 0). value < 0: SzModel_SetDrawFlag(i, 0) for i 0-83,
 * SzModel_SetDrawFlag(i, 1) for i 34-73, then slot 29 through the setters in the order of the
 * source, SzView_SetStraightLight(0, 4, element), SzView_SetCP(0, second zero quad), and the byte
 * +0xd35 of 0x317490 cleared. Assumed: one int argument, no result. */
void SzDuel_Init3DView(int value);

#endif
