#ifndef DOTR_EE_CELL_FORWARDS_H
#define DOTR_EE_CELL_FORWARDS_H

/* Range-check a packed cell word (x << 8 | y, x 0-2, y 0-10) and forward to state/draw/object
 * callees; 0x1f6f50-0x1f725c, adjacent.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/effects.md. */

/* The eight one-bit fields of the 92-byte item's byte +0x57 (item_fills.h has
 * the same layout as DotrFillBits); only bit 6 is read here. */
struct DotrForwardBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bit_4 : 1;
    signed char unknown_bit_5 : 1;
    signed char unknown_bit_6 : 1;
    signed char unknown_bit_7 : 1;
};
/* The bytes of the item 0x1df540 answers that this group reads. */
struct DotrForwardItem {
    unsigned char unknown_00[0x40];
    unsigned char locCol;
    unsigned char locRow;
    unsigned char toMoveCol;
    unsigned char toMoveRow;
    unsigned char unknown_44[0x13];
    struct DotrForwardBits status;
};
struct DotrObjectQuad;

/* Two packed cell words (x << 8 | y); packed_item.h declares the same words
 * as unbounded arrays. */
extern int data_0031be98;
extern int data_0031be9c;

/* Callees; 0x1df540 under this group's item view. */
extern int SzBattle_InqEntry(int index);
extern struct DotrForwardItem *func_001df540_forward(int a, int b);
int SzDuel_GetPlayMotion(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
extern int SzDuel_GetUnitModelID(int side, int pos);
void SzModel_ShotTextureWithFunc(int modelId, int b, int c, int d, int e, int f);
int SzModel_ReqMotionBlur(int modelId, int wFrame, int mode, struct DotrObjectQuad *a, struct DotrObjectQuad *b, int flag);

/* 0x1f6f50. Looks up the item of cell word SzBattle_InqEntry(1); -1 when there is none
 * or when either stored word (0x31be9c, then 0x31be98) has x outside 0-2 or y
 * outside 0-10. Otherwise forwards both cells, two item bytes (+0x42/+0x43
 * when bit 6 of +0x57 is set, else +0x40/+0x41) and the argument to 0x1de2b0.
 * Assumed: int argument and result (packed_item.h calls it with a flag). */
int SzEffect_GetPlayMotion(int a);
/* 0x1f7060. Tail-jumps to 0x1f70a0 with two zero arguments appended. */
int SzEffect_ModelShotTexture2(int packed, int a, int b, int c);
/* 0x1f7070. Tail-jumps to 0x1f70a0 with the word at 0x31be9c prepended. */
int SzEffect_ModelShotTextureWithFunc(int a, int b, int c, int d, int e);
/* 0x1f70a0. 0 when the cell word is out of range or SzDuel_GetUnitModelID(x, y) is
 * negative; otherwise passes that index and the five arguments to 0x1efd30
 * and answers 1. Assumed: all int. */
int SzEffect_ModelShotTextureWithFunc2(int packed, int a, int b, int c, int d, int e);
/* 0x1f7170. Tail-jumps to 0x1f71a0 with the word at 0x31be9c prepended. */
int SzEffect_ReqMotionBlur(int a, int b, struct DotrObjectQuad *c, struct DotrObjectQuad *d, int e);
/* 0x1f71a0. As 0x1f70a0, but the callee is 0x1edcb0 and its result is
 * returned. Assumed: argument types taken from object_table.h's 0x1edcb0. */
int SzEffect_ReqMotionBlur2(int packed, int a, int b, struct DotrObjectQuad *c, struct DotrObjectQuad *d, int e);

#endif
