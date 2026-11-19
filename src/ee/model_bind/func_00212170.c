#include "dotr/ee/model_bind.h"

void lRhModel_SetupOptionObject(struct DotrPatchHead *block, unsigned char *first, unsigned char *second, int flag, int mask)
{
    struct DotrModelHead *head = DOTR_MODEL_BLOCK_HEAD(block);
    struct DotrModelOptionTable *table;
    int *slot;
    int i;
    int *list;
    int *entry;
    int w;

    /* MATCHING DEVICE: the two tests share one `if` (comma expression); two separate early
     * returns compile two words longer. */
    if (head->option_table == 0 ||
        (table = (struct DotrModelOptionTable *)((unsigned char *)block + head->option_table), head->option_enabled == 0)) {
        return;
    }
    if (flag) {
        mask = table->mask;
    }
    for (i = 0, entry = table->list; i < 32; i++, entry++) {
        if (!(mask & (1 << i))) {
            if (*entry != 0) {
                list = (int *)((unsigned char *)table + *entry);
                while ((w = *list) != 0) {
                    switch (w & 0xf0000000) {
                    case 0x20000000:
                        slot = (int *)(first + (w & 0x0fffffff));
                        goto store;
                    case 0x40000000:
                        if (second != 0) {
                            slot = (int *)(second + (w & 0x0fffffff));
                        store:
                            slot[0] = 0x10000000;
                            slot[1] = 0;
                            slot[2] = 0;
                            slot[3] = 0;
                        }
                        break;
                    }
                    list++;
                }
            }
        }
    }
}
