#include "dotr/ee/sound_queue.h"

void SD_vDoFsCommandQueue(void)
{
    struct DotrSoundQueueRead *read;
    struct DotrSoundQueueRequest *request;

    if (g_SDValue[0]->unknown_171c != 0) {
        read = &g_SDValue[0]->reads[g_SDValue[0]->unknown_1718];
        switch (g_SDValue[0]->unknown_171c) {
        case 1:
            if (SDFS_IsUsingNow() == 0) {
                func_0010c2d0_word(read->name, read->destination, read->offset, read->length);
                g_SDValue[0]->unknown_171c++;
                g_SDValue[0]->unknown_0000 |= 0x800000;
            }
            break;
        case 2:
            if (SDFS_IsUsingNow() == 0) {
                s_iHowManyTimesCDAreUsed[0]++;
                g_SDValue[0]->unknown_0000 &= ~0x800000;
                g_SDValue[0]->unknown_1718 = (g_SDValue[0]->unknown_1718 + 1) % 16;
                g_SDValue[0]->unknown_171c++;
            }
            break;
        default:
            g_SDValue[0]->unknown_171c = 0;
            break;
        }
    } else if (g_SDValue[0]->read_count != 0) {
        g_SDValue[0]->unknown_171c = 1;
        g_SDValue[0]->read_count--;
    }
    if (g_SDValue[0]->unknown_182c != 0) {
        request = &g_SDValue[0]->requests[g_SDValue[0]->unknown_1828];
        switch (g_SDValue[0]->unknown_182c) {
        case 1:
            if (SDRPC_TranserToIopStatus() < 0) {
                SDRPC_StartTransferToIop(request->unknown_000, request->unknown_004, request->unknown_008);
                g_SDValue[0]->unknown_182c++;
                g_SDValue[0]->unknown_0000 |= 0x8000000;
            }
            break;
        case 2:
            if (SDRPC_TranserToIopStatus() < 0) {
                g_SDValue[0]->unknown_0000 &= ~0x8000000;
                g_SDValue[0]->unknown_1828 = (g_SDValue[0]->unknown_1828 + 1) % 16;
                g_SDValue[0]->unknown_182c++;
            }
            break;
        default:
            g_SDValue[0]->unknown_182c = 0;
            break;
        }
    } else if (g_SDValue[0]->request_count != 0) {
        g_SDValue[0]->unknown_182c = 1;
        g_SDValue[0]->request_count--;
    }
}
