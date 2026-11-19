#include "dotr/ee/sound_file.h"

int SDFS_ReadToEEBlock(char *name, char *destination, int offset, int size)
{
    int handle;
    int aligned;
    int chunk;
    int skip;
    int copy;
    func_00199b50(data_002b90da, name, destination, offset, size);
    func_00199b50(data_002b9102);
    if ((handle = RhFile_OpenFile(name)) < 0) {
        return 0;
    }
    if (((DotrResourceSubmitWord)destination & 15) || (offset % 2048) || (size % 2048)) {
        while (size > 0) {
            aligned = (offset >> 11) << 11;
            skip = offset - aligned;
            if (skip + size > 0x80000) {
                chunk = 0x80000;
            } else {
                chunk = skip + size;
            }
            size = skip + size - chunk;
            offset = aligned + chunk;
            func_00199b50(data_002b9112, name, handle);
            RhFile_ReadAndBlock(handle, aligned, ((chunk + 2047) >> 11) << 11, 0x4000000);
            copy = chunk - skip;
            func_00198790(destination, (char *)0x4000000 + skip, copy);
            destination += copy;
        }
    } else {
        func_00199b50(data_002b9127);
        RhFile_ReadAndBlock(handle, offset, size, (int)(DotrResourceSubmitWord)destination);
    }
    func_00199b50(data_002b9135);
    RhFile_CloseFile(handle);
    return 1;
}
