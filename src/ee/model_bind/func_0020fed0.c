#include "dotr/ee/model_bind.h"

void lRhModel_DynamicAddressBind(struct DotrPatchHead *block, unsigned char *first, unsigned char *second, int flag)
{
    struct DotrModelHead *head = DOTR_MODEL_BLOCK_HEAD(block);
    int i;
    DotrModelAddress *slot;
    int *list;
    int w;
    DotrModelAddress v;

    if (flag) {
        list = (int *)((unsigned char *)block + head->bind_list_second);
    } else {
        list = (int *)((unsigned char *)block + head->bind_list);
    }
    for (i = 0; (w = *list) != -1; i++) {
        switch (w & 0xf0000000) {
        case 0x20000000:
        case 0x40000000:
            slot = (DotrModelAddress *)(second + (w & 0x0fffffff));
            v = *slot;
            switch (v & 0xf0000000) {
            case 0x20000000:
            case 0x40000000:
                *slot = (DotrModelAddress)(first + (v & 0x0fffffff));
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        list++;
    }
}
