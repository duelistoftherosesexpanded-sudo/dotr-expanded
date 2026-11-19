#include "dotr/ee/sound_file.h"

void lSDFS_FinishCallback(int unused)
{
    int offset;
    int size;
    (void)unused;
    func_00199b50(data_002b9141);
    if (s_iUsingNow.busy) {
        if (s_iUsingNow.destination) {
            func_00198790(s_iUsingNow.destination, (char *)0x4000000 + s_iUsingNow.skip, s_iUsingNow.chunk);
        }
        if (s_iUsingNow.remaining) {
            offset = s_iUsingNow.offset;
            s_iUsingNow.destination += s_iUsingNow.chunk;
            s_iUsingNow.skip = 0;
            if (s_iUsingNow.remaining > 0x80000) {
                s_iUsingNow.chunk = 0x80000;
            } else {
                s_iUsingNow.chunk = s_iUsingNow.remaining;
            }
            if (s_iUsingNow.remaining > s_iUsingNow.chunk) {
                s_iUsingNow.offset += s_iUsingNow.chunk;
                s_iUsingNow.remaining -= s_iUsingNow.chunk;
            } else {
                s_iUsingNow.offset = 0;
                s_iUsingNow.remaining = 0;
            }
            size = ((s_iUsingNow.chunk + 2047) >> 11) << 11;
            func_00199b50(data_002b9168, gSdfs_sa, 0x4000000);
            RhFile_ReadAndCallbackByFileName(gSdfs_sa, offset, size, 0x4000000, lSDFS_FinishCallback, 0, 32);
        } else {
            s_iUsingNow.busy = 0;
        }
    }
}
