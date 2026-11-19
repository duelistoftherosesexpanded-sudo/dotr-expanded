#include "dotr/ee/part_lists.h"

/* Clears *size_out when given. Without a model, or with a zero word at model +0x3c,
 * returns the cursor unchanged. Otherwise takes a buffer position from RhBase_GetUserGsAddress,
 * advances it by SzText_InitClutManage(position, 2) and, for every record with a non-zero first
 * word, emits SzText_Load_Pipe (which reports a length), then, when the record's word +8 is
 * set and SzModel_GetPauseFlag(modelId) and SzModel_GetStoneFlag(modelId) are both zero, emits
 * SzModel_LoadTexAnim_Pipe and counts it in the record's word +0xc; the position advances by the
 * reported length. Stores the bytes used in *size_out and ends with RhText_SetTextureFlush. */
int SzModel_LoadTexture_Pipe(int cursor, int modelId, struct DotrPartModel *model, int *size_out)
{
    int currentGsAddress;
    int count;
    int i;
    struct DotrPartRecord *rec;
    struct DotrPartHeader *header;
    struct DotrPartList *list;
    int len;
    int start;

    if (size_out != 0) {
        *size_out = 0;
    }
    if (model == 0) {
        return cursor;
    }
    header = &model->header;
    if (header->unknown_2c == 0) {
        return cursor;
    }
    list = (struct DotrPartList *)((char *)model + header->list_offset);
    count = list->count;
    rec = list->records;
    start = currentGsAddress = RhBase_GetUserGsAddress();
    currentGsAddress += SzText_InitClutManage(currentGsAddress, 2);
    for (i = 0; i < count; i++, rec++) {
        if (rec->unknown_0 != 0) {
            cursor = func_00202fb0_out(cursor, rec->unknown_0, currentGsAddress, &len);
            if (rec->unknown_8 != 0 && SzModel_GetPauseFlag(modelId) == 0 && SzModel_GetStoneFlag(modelId) == 0) {
                cursor = SzModel_LoadTexAnim_Pipe(cursor, modelId, currentGsAddress, rec);
                rec->unknown_c++;
            }
            currentGsAddress += len;
        }
    }
    if (size_out != 0) {
        *size_out = currentGsAddress - start;
    }
    return RhText_SetTextureFlush(cursor);
}
