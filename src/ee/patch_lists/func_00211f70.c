#include "dotr/ee/patch_lists.h"

void lRhModel_SetupTexture(struct DotrPatchHead *modelFile, unsigned char *scratchpadAddr1, unsigned char *scratchpadAddr2, int animationIndex)
{
    struct DotrPatchPick *pick;
    struct DotrPatchItem *cursor;
    struct DotrPatchSub *sub = &modelFile->sub;
    unsigned int w;
    int i;
    struct DotrPatchItem *item;
    int c;
    struct DotrPatchRun *run;
    int n;
    int j;
    unsigned int *word;
    struct DotrPatchList *list;

    if (sub->has_list != 0) {
        list = (struct DotrPatchList *)((unsigned char *)modelFile + sub->list_offset);
        if (sub->pick_offset == 0 || sub->has_pick == 0) {
            pick = 0;
        } else {
            pick = (struct DotrPatchPick *)((unsigned char *)modelFile + sub->pick_offset);
        }
        cursor = (struct DotrPatchItem *)(list + 1);
        run = (struct DotrPatchRun *)((unsigned char *)list + list->runs_offset);
        for (i = 0; i < list->count; i++) {
            if (pick != 0 && pick->key >= 0 && animationIndex >= 0 && i == pick->key
                && (c = pick->choice[animationIndex]) >= 0 && c < list->count) {
                item = (struct DotrPatchItem *)(list + 1) + c;
            } else {
                item = cursor;
            }
            n = run->count;
            word = (unsigned int *)((unsigned char *)list + run->words_offset);
            for (j = 0; j < n; j++) {
                w = *word;
                switch (w & 0xf0000000) {
                case 0x20000000:
                    RhMem_CopyMemory16(scratchpadAddr1 + ((union DotrPatchWord *)word)->bits.offset, (unsigned char *)(item->object->rows) + 32, 2);
                    break;
                case 0x40000000:
                    RhMem_CopyMemory16(scratchpadAddr2 + ((union DotrPatchWord *)word)->bits.offset, (unsigned char *)(item->object->rows) + 32, 2);
                    break;
                }
                word++;
            }
            cursor++;
            run++;
        }
    }
}
