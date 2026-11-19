#ifndef DOTR_EE_CARD_BLUR_H
#define DOTR_EE_CARD_BLUR_H

/* TaLib_c615 (0x27eb00): card-transit effect step with its own work block (two card requests, four
 * blur coordinates); callees live in several groups with incompatible views.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Structure tags, member names and offsets are from the Japanese build's debug information
 * for TaLib_c615 (tags it left unnamed keep the jp_<n> form). Assumed here: the colour
 * members of jp_ab107 (four bytes, the last written as alpha), the contents of jp_aa9a1
 * (0x30 bytes, not touched), and the 16-byte alignment of the draw and card request
 * structures, which the 0x90 and 0xa0 sizes and the quadword copy of a request show. */
struct jp_aae7b { float x, y, z, w; };
struct jp_aa35d { int x, y, z, w; };
struct jp_ab1fb { float m[16]; };
struct jp_ab19f { float x, y; };
struct jp_ab107 { unsigned char r, g, b, a; };
struct jp_aa9a1 { int unknown[12]; };
struct jp_ab668;
struct jp_aa3f4 {
    struct jp_aae7b scale;
    struct jp_aa35d rot;
    struct jp_aae7b trans;
    struct jp_ab1fb matrix;
};
struct jp_aa66e {
    struct jp_aae7b pntPos;
    struct jp_aa35d ivecRot;
    struct jp_aa35d ivecLook;
    struct jp_aae7b vecScale;
    struct jp_ab107 icolRgba;
    struct jp_ab107 icolFilter;
    struct jp_aa9a1 Node;
    struct jp_ab668 *pNode;
    struct jp_ab668 *pWork;
    int nMax : 12;
    int fBig : 1;
    int fLook : 1;
    int fScale : 1;
    int fColor : 1;
    int nNode : 12;
    unsigned int bFilterMode : 2;
    int fIgnore : 1;
    int fBusy : 1;
} __attribute__((aligned(16)));
struct jp_aa2f3 {
    struct jp_aa66e Draw;
    void *pVarInfo;
} __attribute__((aligned(16)));
struct cardcor {
    struct jp_aae7b pntPos;
    struct jp_aa35d ivecRot;
    struct jp_aa35d ivecLook;
    struct jp_aae7b vecScale;
    struct jp_ab19f pntScr;
    int nCount;
    int pad;
};
struct jp_aa4a2 {
    struct jp_aa3f4 wcoord;
    struct jp_aa3f4 blur[4];
    struct jp_aa2f3 CardReqF;
    struct jp_aa2f3 CardReqT;
    struct cardcor corF;
    struct cardcor corT;
    int cardmove;
    int kind;
    int effflags;
    int fadeF;
    int fadeT;
    int count;
    int pad[2];
};
struct jp_ab71b { int eNO; unsigned int dwFrom; unsigned int dwTo; unsigned int dwPar; };


/* Callees declared in other groups with their own views; the argument types
 * here are the ones this function's work block gives them. Assumed: func_00195108 takes a
 * block and a byte count and has no result used; the third Ta_LkPrim_DrawCard argument is a
 * pointer (0 is passed); Ex3d_GetLookVector's result is not used. */
extern int *ExGetMyOT(void);
extern struct jp_aa2f3 *SzEffect_GetCardRequest2(int packed);
extern void func_00195108(void *a, int b);
extern void Ta_LkInitCoordinate(struct jp_aa4a2 *m);
extern int SzEffect_SetBusyFlag2(int packed, int flag);
extern void Ex3d_GetPointOnScreen(struct jp_aae7b *out, float x, float y);
extern void Ex3d_GetLookVector(struct jp_aa35d *out);
extern void Ta_LkSetCoordinate(struct jp_aa4a2 *frame);
extern void SzDuel_StartCardSession(void);
extern void SzDuel_FinishCardSession(void);
extern int Ta_LkPrim_DrawCard(struct jp_aa2f3 *owner, int alpha, void *point);
extern void Ta_LkOBJ_TransitVector(struct jp_aa2f3 *dst, struct jp_aa2f3 *src, int n);
extern int Ta_LkCheckFlag(int a, int b);

/* 0x27eb00. Prototype and locals from the Japanese build's debug information. */
int TaLib_c615(struct jp_aa4a2 *pobj, struct jp_ab71b *eInfo, int nID);

#endif
