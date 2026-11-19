#include "dotr/ee/sound_file.h"

int SDFS_ReadToEE(char *name, char *destination, int offset, int size)
{
    if (!((DotrResourceSubmitWord)destination & 15) && !(offset % 2048) && !(size % 2048)) {
        s_iUsingNow.destination = 0;
        s_iUsingNow.skip = 0;
        s_iUsingNow.chunk = 0;
        s_iUsingNow.offset = 0;
        s_iUsingNow.remaining = 0;
        func_00199b50(data_002b90c0, name, destination);
        if (!RhFile_ReadAndCallbackByFileName(name, offset, size, (int)(DotrResourceSubmitWord)destination,
                           lSDFS_FinishCallback, 0, 32)) {
            return -1;
        }
    }
    s_iUsingNow.busy = 1;
    return 1;
}
