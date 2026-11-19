#ifndef DOTR_EE_PART_LISTS_H
#define DOTR_EE_PART_LISTS_H

/* 0x1ea2a0: walks the 16-byte records of a list found through a model header and emits one
 * draw step per record.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Cursor values and buffer positions are int words here (draw_state.h calls the cursor
 * DotrDrawWord). Record words +0, +8 and +0xc are used; +4 is not touched. */
struct DotrPartRecord {
    int unknown_0;
    int unknown_4;
    int unknown_8;
    int unknown_c;
};
/* Count at +0, records from +0x10; the three words between are not touched. */
struct DotrPartList {
    int count;
    int unknown_4[3];
    struct DotrPartRecord records[1];
};
/* The header at model +0x10 holds the list's byte offset from the model start at +0x28
 * and a word tested for zero at +0x2c. */
struct DotrPartHeader {
    int unknown_00[10];
    int list_offset;
    int unknown_2c;
};
struct DotrPartModel {
    int unknown_00[4];
    struct DotrPartHeader header;
};
/* Defined in draw_state / small_misc / object_table; same parameter words. */
extern int RhBase_GetUserGsAddress(void);
extern int SzText_InitClutManage(int base, int size);
extern int SzModel_GetPauseFlag(int modelId);
extern int SzModel_GetStoneFlag(int index);
/* Other callees. SzText_Load_Pipe is declared with int parameters in draw_state.h;
 * this caller passes the address of a local as the last one, so it uses a pointer view
 * of the same function (same registers). */
extern int func_00202fb0_out(int cursor, int a0, int a1, int *out);
extern int SzModel_LoadTexAnim_Pipe(int cursor, int index, int position, struct DotrPartRecord *record);
extern int RhText_SetTextureFlush(int cursor);
int SzModel_LoadTexture_Pipe(int cursor, int modelId, struct DotrPartModel *model, int *size_out);

#endif
